#define DOK_VULKAN_TABLES_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
// #define KGL_DEBUG

#include <dok/dok_vulkan_tables.h>
#include <dok/dok_core.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vk_mem_alloc.h"
#include "stb_image.h"
#include "linmath.h"
#include "stb_ds.h"

const unsigned char imageTexture[] = {
    #embed "test_image.png"
};

typedef struct KglInstance_T* KglInstance;
typedef struct KglPhysicalDevice_T* KglPhysicalDevice;
typedef struct KglDevice_T* KglDevice;
typedef struct KglImage_T* KglImage;

struct KglInstance_T {
    DokInstanceTable table;
    DokVkLoader loader;
    VkInstance handle;
    KglPhysicalDevice* physicalDevices;
#ifdef KGL_DEBUG
    VkDebugUtilsMessengerEXT messenger;
#endif
};

struct KglPhysicalDevice_T {
    VkPhysicalDevice handle;
    KglInstance instance;
    VkPhysicalDeviceProperties properties;
    uint32_t queueFamilyIndex;
};

struct KglImage_T {
    KglDevice       device;
    VmaAllocation   allocation;
    VkImageView     view;
    VkImage         handle;
    VkExtent3D      extent;
};

typedef struct KglSwapchain {
    VkSwapchainKHR handle;
    VkSemaphore* imageSemaphores;
    VkImageView* imageViews;
    VkImage* images;
    KglImage depthImage;
    VkFormat depthImageFormat;
    VkFormat format;
} KglSwapchain;

struct KglDevice_T {
    DokDeviceTable table;
    VkDevice handle;
    KglPhysicalDevice physicalDevice;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
    VkCommandPool commandPool;
    KglSwapchain swapchain;
    VmaAllocator allocator;
    VkSurfaceKHR surface;
    DokWindow window;
    VkQueue queue;
};

typedef struct UniformBufferObject {
    mat4x4 model;
    mat4x4 view;
    mat4x4 projection;
} UniformBufferObject;

typedef struct Vertex {
    vec3 position;
    vec3 color;
} Vertex;

#ifdef KGL_DEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL kglDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
) {
    printf("[VULKAN VALIDATION] %s\n", callbackData->pMessage);
    return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT kglGetDebugInfo() {
    return (VkDebugUtilsMessengerCreateInfoEXT) {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = kglDebugCallback
    };
}
#endif

const char** kglGetInstanceExtensions() {
    const char** instanceExtensions = NULL;
    arrput(instanceExtensions, dokGetPlatformSurfaceExtension());
    arrput(instanceExtensions, VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef KGL_DEBUG
    arrput(instanceExtensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return instanceExtensions;
}

const char** kglGetInstanceLayers() {
    const char** extensionsLayers = NULL;
#ifdef KGL_DEBUG
    arrput(extensionsLayers, "VK_LAYER_KHRONOS_validation");
#endif
    return extensionsLayers;
}

void kglDestroyInstance(KglInstance instance) {
#ifdef KGL_DEBUG
    if (instance->messenger) instance->table.vkDestroyDebugUtilsMessengerEXT(instance->handle, instance->messenger, NULL);
#endif
    if (instance->handle) instance->table.vkDestroyInstance(instance->handle, NULL);
    if (instance->loader) dokUnloadVulkan(instance->loader);
    arrfree(instance->physicalDevices);
    free(instance);
}

void kglCreateInstance(KglInstance* pOut) {
    KglInstance instance = calloc(1, sizeof(struct KglInstance_T));
    if (!instance) return;
    if (dokLoadVulkan(&instance->loader) != DOK_SUCCESS) goto cleanup;

    PFN_vkGetInstanceProcAddr instanceProcAddr = (PFN_vkGetInstanceProcAddr)dokGetFunction(instance->loader, "vkGetInstanceProcAddr");

    DokGlobalTable globalTable;
    dokLoadGlobalTable(instanceProcAddr, &globalTable);

    const char** extensions = kglGetInstanceExtensions();
    const char** layers = kglGetInstanceLayers();

    const VkApplicationInfo appInfo = { 
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Kgl-App",
        .apiVersion = VK_API_VERSION_1_4
    };

    const VkInstanceCreateInfo instanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = arrlen(extensions),
        .ppEnabledExtensionNames = extensions,
        .enabledLayerCount = arrlen(layers),
        .ppEnabledLayerNames = layers
    };

    VkResult result = globalTable.vkCreateInstance(&instanceInfo, NULL, &instance->handle);
    arrfree(extensions);
    arrfree(layers);

    if (result != VK_SUCCESS) goto cleanup;
    dokLoadInstanceTable(instanceProcAddr, instance->handle, &instance->table);

#ifdef KGL_DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugInfo = kglGetDebugInfo();
    if (instance->table.vkCreateDebugUtilsMessengerEXT(instance->handle, &debugInfo, NULL, &instance->messenger) != VK_SUCCESS) goto cleanup;
#endif

    *pOut = instance;
    return;

cleanup:
    kglDestroyInstance(instance);
}

VkBool32 kglGetQueueFamily(
    KglInstance         instance, 
    VkPhysicalDevice    handle, 
    uint32_t*           pOut) {

    uint32_t queueFamilyCount = 0;
    instance->table.vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*) malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    if (!queueFamilies) return VK_FALSE;
    instance->table.vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, queueFamilies);

    const DokVulkanInfo vulkanInfo = {
        .instanceProcAddr = instance->table.vkGetInstanceProcAddr,
        .instance = instance->handle
    };

    for (size_t i = 0; i < queueFamilyCount; i++) {
        if (dokGetPhysicalDevicePresentationSupport(&vulkanInfo, handle, i) == VK_FALSE) {
            continue;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            *pOut = i;
            return VK_TRUE;
        }
    }

    return VK_FALSE;
}

void kglEnumeratePhysicalDevices(
    KglInstance         instance, 
    uint32_t*           pOutPhysicalDeviceCount, 
    KglPhysicalDevice*  pOutPhysicalDevices) {

    uint32_t handleCount = 0;
    instance->table.vkEnumeratePhysicalDevices(instance->handle, &handleCount, NULL);

    if (handleCount == arrlen(instance->physicalDevices)) goto end;

    for (uint32_t i = 0; i < arrlen(instance->physicalDevices); i++) {
        free(instance->physicalDevices[i]);
    }
    arrfree(instance->physicalDevices);

    VkPhysicalDevice* handles = malloc(handleCount * sizeof(VkPhysicalDevice));
    if (!handles) return;
    instance->table.vkEnumeratePhysicalDevices(instance->handle, &handleCount, handles);

    for (uint32_t i = 0; i < handleCount; i++) {
        VkPhysicalDevice handle = handles[i];
        uint32_t queueFamilyIndex;

        if (kglGetQueueFamily(instance, handle, &queueFamilyIndex) == VK_TRUE) {

            KglPhysicalDevice physicalDevice = malloc(sizeof(struct KglPhysicalDevice_T));
            instance->table.vkGetPhysicalDeviceProperties(handle, &physicalDevice->properties);
            physicalDevice->queueFamilyIndex = queueFamilyIndex;
            physicalDevice->instance = instance;
            physicalDevice->handle = handle;

            arrpush(instance->physicalDevices, physicalDevice);
        }
    }

end:
    if (pOutPhysicalDeviceCount) *pOutPhysicalDeviceCount = arrlen(instance->physicalDevices);
    if (pOutPhysicalDevices) {
        for (uint32_t i = 0; i < arrlen(instance->physicalDevices); i++) {
            pOutPhysicalDevices[i] = instance->physicalDevices[i];
        }
    }
}

void kglDestroyImage(KglImage image) {
    KglDevice device = image->device;
    device->table.vkDestroyImageView(device->handle, image->view, NULL);
    vmaDestroyImage(device->allocator, image->handle, image->allocation);
    free(image);
}

void kglCreateImage(
    KglDevice                   device,
    VkExtent2D                  extent,
    VkFormat                    format,
    VkImageUsageFlags           usage,
    VmaAllocationCreateFlagBits flags,
    VkImageAspectFlags          aspectMask,
    KglImage*                   pOut
) {
    KglImage image = malloc(sizeof(struct KglImage_T));
    const VkExtent3D imageExtent = {
        .width = extent.width,
        .height = extent.height,
        .depth = 1
    };
    const VkImageCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent = imageExtent,
        .arrayLayers = 1,
        .mipLevels = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = usage
    };
    const VmaAllocationCreateInfo allocationInfo = {
        .flags = flags,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    vmaCreateImage(device->allocator, &info, &allocationInfo, &image->handle, &image->allocation, NULL);
    const VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .image = image->handle,
        .subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = aspectMask,
            .levelCount = 1,
            .layerCount = 1
        }
    };
    device->table.vkCreateImageView(device->handle, &viewInfo, NULL, &image->view);
    image->extent = imageExtent;
    image->device = device;
    *pOut = image;
}

void kglUpdateSwapchain(
    KglDevice           device, 
    VkPresentModeKHR    presentMode) {

    KglPhysicalDevice physicalDevice = device->physicalDevice;
    KglSwapchain* pSwapchain = &device->swapchain;
    KglInstance instance = physicalDevice->instance;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    instance->table.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->handle, device->surface, &surfaceCapabilities);

    VkExtent2D swapchainExtent = surfaceCapabilities.currentExtent;
    if (swapchainExtent.width == 0xFFFFFFFF) {
        dokGetWindowExtent(device->window, &swapchainExtent);
    }

    const VkSwapchainCreateInfoKHR swapchainInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = device->surface,
        .minImageCount = surfaceCapabilities.minImageCount,
        .imageFormat = pSwapchain->format,
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = swapchainExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = pSwapchain->handle
    };

    if (device->table.vkCreateSwapchainKHR(device->handle, &swapchainInfo, NULL, &pSwapchain->handle) != VK_SUCCESS) return;
    if (swapchainInfo.oldSwapchain != VK_NULL_HANDLE) {
        device->table.vkDestroySwapchainKHR(device->handle, swapchainInfo.oldSwapchain, NULL);
    }

    for (uint32_t i = 0; i < arrlen(pSwapchain->images); i++) {
        device->table.vkDestroySemaphore(device->handle, pSwapchain->imageSemaphores[i], NULL);
        device->table.vkDestroyImageView(device->handle, pSwapchain->imageViews[i], NULL);
    }

    uint32_t imageCount;
    device->table.vkGetSwapchainImagesKHR(device->handle, pSwapchain->handle, &imageCount, NULL);

    arrsetlen(pSwapchain->imageSemaphores, imageCount);
    arrsetlen(pSwapchain->imageViews, imageCount);
    arrsetlen(pSwapchain->images, imageCount);
    device->table.vkGetSwapchainImagesKHR(device->handle, pSwapchain->handle, &imageCount, pSwapchain->images);

    const VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    for (uint32_t i = 0; i < arrlen(pSwapchain->images); i++) {
        const VkImageViewCreateInfo viewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = pSwapchain->images[i],
            .format = pSwapchain->format,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .subresourceRange = (VkImageSubresourceRange) {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .layerCount = 1,
                .levelCount = 1
            }
        };
        device->table.vkCreateImageView(device->handle, &viewInfo, NULL, &pSwapchain->imageViews[i]);
        device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &pSwapchain->imageSemaphores[i]);
    }

    if (pSwapchain->depthImage) kglDestroyImage(pSwapchain->depthImage);
    kglCreateImage(
        device,
        swapchainExtent,
        pSwapchain->depthImageFormat,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        &pSwapchain->depthImage
    );
}

void kglDestroyDevice(KglDevice device) {
    KglInstance instance = device->physicalDevice->instance;
    KglSwapchain* pSwapchain = &device->swapchain;

    for (uint32_t i = 0; i < arrlen(pSwapchain->images); i++) {
        device->table.vkDestroySemaphore(device->handle, pSwapchain->imageSemaphores[i], NULL);
        device->table.vkDestroyImageView(device->handle, pSwapchain->imageViews[i], NULL);
    }
    arrfree(pSwapchain->imageSemaphores);
    arrfree(pSwapchain->imageViews);
    arrfree(pSwapchain->images);

    if (device->descriptorSetLayout) device->table.vkDestroyDescriptorSetLayout(device->handle, device->descriptorSetLayout, NULL);
    if (device->descriptorPool) device->table.vkDestroyDescriptorPool(device->handle, device->descriptorPool, NULL);

    if (pSwapchain->depthImage) kglDestroyImage(pSwapchain->depthImage);
    if (pSwapchain) device->table.vkDestroySwapchainKHR(device->handle, pSwapchain->handle, NULL);
    if (device->commandPool) device->table.vkDestroyCommandPool(device->handle, device->commandPool, NULL);
    if (device->surface) instance->table.vkDestroySurfaceKHR(instance->handle, device->surface, NULL);
    if (device->allocator) vmaDestroyAllocator(device->allocator);
    device->table.vkDestroyDevice(device->handle, NULL);
    free(device);
}

void kglCreateDevice(
    KglPhysicalDevice   physicalDevice,
    DokWindow           window,
    uint32_t            maxTextures,
    KglDevice*          pOut) {
    
    KglDevice device = calloc(1, sizeof(struct KglDevice_T));
    if (!device) return;

    KglInstance instance = physicalDevice->instance;
    device->physicalDevice = physicalDevice;
    device->swapchain.format = VK_FORMAT_B8G8R8A8_SRGB;

    VkFormat depthFormats[] = {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };
    
    size_t depthFormatsCount = sizeof(depthFormats) / sizeof(depthFormats[0]);

    for (size_t i = 0; i < depthFormatsCount; i++) {
        VkFormatProperties formatProperties;
        //Laboratory 802 laptops primary gpu dont support 'vkGetPhysicalDeviceFormatProperties2'
        //for some reason

        VkFormat format = depthFormats[i];
        instance->table.vkGetPhysicalDeviceFormatProperties(physicalDevice->handle, format, &formatProperties);
        if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            device->swapchain.depthImageFormat = format;
            break;
        }
    }

    const DokVulkanInfo vulkanInfo = {
        .instanceProcAddr = instance->table.vkGetInstanceProcAddr,
        .instance = instance->handle
    };

    dokCreateWindowSurface(&vulkanInfo, window, &device->surface);

    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    uint32_t queueFamilyIndex = physicalDevice->queueFamilyIndex;
    float priority = 1.0f;

    const VkDeviceQueueCreateInfo queueInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &priority
    };

    VkPhysicalDeviceVulkan12Features enabledVk12Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = VK_TRUE,
        .shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
        // .descriptorBindingVariableDescriptorCount = VK_TRUE,
        .descriptorBindingSampledImageUpdateAfterBind = VK_TRUE,
        .descriptorBindingPartiallyBound = VK_TRUE,
        .runtimeDescriptorArray = VK_TRUE,
        .bufferDeviceAddress = VK_TRUE  
    };

    VkPhysicalDeviceVulkan13Features enabledVk13Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &enabledVk12Features,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE
    };

    const VkPhysicalDeviceVulkan14Features enabledVk14Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
        .pNext = &enabledVk13Features,
        .hostImageCopy = VK_TRUE
    };

    const VkPhysicalDeviceFeatures enabledvk10Features = {
        .samplerAnisotropy = VK_TRUE
    };

    const VkDeviceCreateInfo deviceInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &enabledVk14Features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueInfo,
        .enabledExtensionCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]),
        .ppEnabledExtensionNames = deviceExtensions,
        .pEnabledFeatures = &enabledvk10Features
    };

    if (instance->table.vkCreateDevice(physicalDevice->handle, &deviceInfo, NULL, &device->handle) != VK_SUCCESS) goto cleanup;
    dokLoadDeviceTable(instance->table.vkGetDeviceProcAddr, device->handle, &device->table);

    VkDescriptorPoolSize descriptorPoolSizes[] = {
        (VkDescriptorPoolSize) {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = maxTextures
        }
    };
    const VkDescriptorPoolCreateInfo descriptorPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
        .poolSizeCount = sizeof(descriptorPoolSizes) / sizeof(descriptorPoolSizes[0]),
        .pPoolSizes = descriptorPoolSizes,
        .maxSets = 1
    };
    device->table.vkCreateDescriptorPool(device->handle, &descriptorPoolInfo, NULL, &device->descriptorPool);

    VkDescriptorSetLayoutBinding bindings[] = {
        (VkDescriptorSetLayoutBinding) {
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .descriptorCount = maxTextures
        }
    };
    VkDescriptorBindingFlags bindingFlags[] = {
        VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
    };
    const VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
        .bindingCount = sizeof(bindingFlags) / sizeof(bindingFlags[0]),
        .pBindingFlags = bindingFlags
    };
    const VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = &bindingFlagsInfo,
        .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
        .bindingCount = sizeof(bindings) / sizeof(bindings[0]),
        .pBindings = bindings
    };
    device->table.vkCreateDescriptorSetLayout(device->handle, &descriptorLayoutInfo, NULL, &device->descriptorSetLayout);
    const VkDescriptorSetAllocateInfo descriptorInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = device->descriptorPool,
        .pSetLayouts = &device->descriptorSetLayout,
        .descriptorSetCount = 1
    };
    device->table.vkAllocateDescriptorSets(device->handle, &descriptorInfo, &device->descriptorSet);

    VkCommandPoolCreateInfo commandPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex
    };
    device->table.vkCreateCommandPool(device->handle, &commandPoolInfo, NULL, &device->commandPool);

    const VmaVulkanFunctions vkFunctions = {
        .vkGetInstanceProcAddr = instance->table.vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr = instance->table.vkGetDeviceProcAddr,
        .vkCreateImage = device->table.vkCreateImage
    };

    const VmaAllocatorCreateInfo vmaInfo = {
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice = physicalDevice->handle,
        .device = device->handle,
        .pVulkanFunctions = &vkFunctions,
        .instance = instance->handle
    };

    if (vmaCreateAllocator(&vmaInfo, &device->allocator) != VK_SUCCESS) goto cleanup;

    device->table.vkGetDeviceQueue(device->handle, queueFamilyIndex, 0, &device->queue);
    kglUpdateSwapchain(device, VK_PRESENT_MODE_FIFO_KHR);

    *pOut = device;
    return;

cleanup:
    kglDestroyDevice(device);
}

typedef struct KglRenderer_T* KglRenderer;
typedef struct KglBuffer_T* KglBuffer;
typedef struct KglFrameResource_T* KglFrameResource;

struct KglBuffer_T {
    VmaAllocator    allocator;
    VmaAllocation   allocation;
    VkDeviceAddress address;
    VkBuffer        handle;
};

void kglBeginSingleTimeCommands(KglDevice device, VkCommandBuffer* pOut) {
    VkCommandBufferAllocateInfo allocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool = device->commandPool,
        .commandBufferCount = 1
    };
    device->table.vkAllocateCommandBuffers(device->handle, &allocateInfo, pOut);
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    device->table.vkBeginCommandBuffer(*pOut, &beginInfo);
}

void kglEndSubmitSingleTimeCommands(KglDevice device, VkCommandBuffer commandBuffer) {
    device->table.vkEndCommandBuffer(commandBuffer);
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer
    };
    device->table.vkQueueSubmit(device->queue, 1, &submitInfo, NULL);
    device->table.vkQueueWaitIdle(device->queue);
}

void kglMapCopyBufferData(KglBuffer buffer, const void* data, size_t byteSize) {
    void* pData;
    vmaMapMemory(buffer->allocator, buffer->allocation, &pData);
    memcpy(pData, data, byteSize);
    vmaUnmapMemory(buffer->allocator, buffer->allocation);
}

void kglCreateBuffer(
    KglDevice                   device, 
    VkDeviceSize                size,
    VkBufferUsageFlags          usage, 
    VmaAllocationCreateFlags    flags,
    KglBuffer*                  pOut
    ) {

    KglBuffer buffer = malloc(sizeof(struct KglBuffer_T));
    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage
    };
    VmaAllocationCreateInfo allocationInfo = {
        .flags = flags,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    vmaCreateBuffer(device->allocator, &bufferInfo, &allocationInfo, &buffer->handle, &buffer->allocation, NULL);
    if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) {
        const VkBufferDeviceAddressInfo addressInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = buffer->handle
        };
        buffer->address = device->table.vkGetBufferDeviceAddress(device->handle, &addressInfo);
    }
    buffer->allocator = device->allocator;
    *pOut = buffer;
}

void kglDestroyBuffer(KglBuffer buffer) {
    vmaDestroyBuffer(buffer->allocator, buffer->handle, buffer->allocation);
    free(buffer);
}

struct KglFrameResource_T {
    KglDevice device;
    KglBuffer uniformBuffer;
    VkCommandBuffer commandBuffer;
    VkSemaphore semaphore;
    VkFence fence;
};

void kglDestroyFrameResource(KglFrameResource frameResource) {
    KglDevice device = frameResource->device;
    kglDestroyBuffer(frameResource->uniformBuffer);
    device->table.vkDestroySemaphore(device->handle, frameResource->semaphore, NULL);
    device->table.vkDestroyFence(device->handle, frameResource->fence, NULL);
    free(frameResource);
}

void kglCreateFrameResource(
    KglDevice device, 
    KglFrameResource* pOut
) {

    KglFrameResource frameResource = malloc(sizeof(struct KglFrameResource_T));
    const VkCommandBufferAllocateInfo commandBufferInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool = device->commandPool,
        .commandBufferCount = 1
    };
    const VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };
    const VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };
    device->table.vkAllocateCommandBuffers(device->handle, &commandBufferInfo, &frameResource->commandBuffer);
    device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &frameResource->semaphore);
    device->table.vkCreateFence(device->handle, &fenceInfo, NULL, &frameResource->fence);
    kglCreateBuffer(
        device,
        sizeof(UniformBufferObject),
        VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        &frameResource->uniformBuffer
    );
    frameResource->device = device;
    *pOut = frameResource;
}

void kglUpdateTextureDescriptors(
    KglDevice               device,
    VkDescriptorImageInfo*  descriptors, 
    uint32_t                descriptorCount
) {
    const VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .descriptorCount = descriptorCount,
        .pImageInfo = descriptors,
        .dstSet = device->descriptorSet,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
    };
    device->table.vkUpdateDescriptorSets(device->handle, 1, &writeDescriptorSet, 0, NULL);
}

typedef struct KglRendererCreateInfo {
    uint32_t framesInFlight;
    uint32_t maxIndices;
    uint32_t maxVertex;
} KglRendererCreateInfo;

struct KglRenderer_T {
    KglDevice device;
    VkPipelineLayout pipelineLayout;
    KglImage textureImage;
    KglFrameResource* frameResources;
    KglBuffer indicesBuffer;
    KglBuffer vertexBuffer;
    VkSampler sampler;
    uint32_t framesInFlight;
};

void kglDestroyRenderer(KglRenderer renderer) {
    KglDevice device = renderer->device;
    kglDestroyImage(renderer->textureImage);
    for (uint32_t i = 0; i < renderer->framesInFlight; i++) {
        kglDestroyFrameResource(renderer->frameResources[i]);
    }
    device->table.vkDestroySampler(device->handle, renderer->sampler, NULL);
    device->table.vkDestroyPipelineLayout(device->handle, renderer->pipelineLayout, NULL);
    kglDestroyBuffer(renderer->vertexBuffer);
    kglDestroyBuffer(renderer->indicesBuffer);
    free(renderer);
}

void kglCreateRenderer(KglDevice device, const KglRendererCreateInfo* pInfo, KglRenderer* pOut) {
    KglRenderer renderer = calloc(1, sizeof(struct KglRenderer_T));
    if (!renderer) return;
    renderer->framesInFlight = pInfo->framesInFlight;
    renderer->device = device;

    renderer->frameResources = malloc(pInfo->framesInFlight * sizeof(KglFrameResource));
    for (uint32_t i = 0; i < pInfo->framesInFlight; i++) {
        kglCreateFrameResource(device, &renderer->frameResources[i]);
    }
    const VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST
    };
    device->table.vkCreateSampler(device->handle, &samplerInfo, NULL, &renderer->sampler);

    int width, height, channels;
    stbi_uc* pixels = stbi_load_from_memory(
        imageTexture,
        sizeof(imageTexture),
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );
    VkExtent2D imageExtent = {.width = width, .height = height};
    kglCreateImage(
        device,
        imageExtent,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_HOST_TRANSFER_BIT,
        0,
        VK_IMAGE_ASPECT_COLOR_BIT,
        &renderer->textureImage
    );
    const VkHostImageLayoutTransitionInfo transition = {
        .sType = VK_STRUCTURE_TYPE_HOST_IMAGE_LAYOUT_TRANSITION_INFO,
        .image = renderer->textureImage->handle,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .subresourceRange = (VkImageSubresourceRange) { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1 }
    };
    device->table.vkTransitionImageLayout(device->handle, 1, &transition);
    const VkMemoryToImageCopy memoryCopy = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_TO_IMAGE_COPY,
        .pHostPointer = pixels,
        .imageExtent = renderer->textureImage->extent,
        .imageSubresource = (VkImageSubresourceLayers) { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .layerCount = 1 }
    } ;
    const VkCopyMemoryToImageInfo copyInfo = {
        .sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_IMAGE_INFO,
        .dstImage = renderer->textureImage->handle,
        .dstImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .regionCount = 1,
        .pRegions = &memoryCopy
    };
    device->table.vkCopyMemoryToImage(device->handle, &copyInfo);
    stbi_image_free(pixels);
    kglCreateBuffer(
        device,
        pInfo->maxVertex * sizeof(Vertex),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        0,
        &renderer->vertexBuffer
    );
    kglCreateBuffer(
        device,
        pInfo->maxIndices * sizeof(uint32_t),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        0,
        &renderer->indicesBuffer
    );
    VkDescriptorImageInfo imageDescriptors[] = {
        (VkDescriptorImageInfo) {
            .imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
            .imageView = renderer->textureImage->view,
            .sampler = renderer->sampler
        }
    };
    uint32_t imageDescriptorCount = sizeof(imageDescriptors) / sizeof(imageDescriptors[0]);
    kglUpdateTextureDescriptors(device, imageDescriptors, imageDescriptorCount);
    const VkPipelineDepthStencilStateCreateInfo depthStencilState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL
    };
    const VkPushConstantRange pushContantRange = {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .size = sizeof(VkDeviceAddress)
    };
    const VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &device->descriptorSetLayout,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &pushContantRange
    };
    device->table.vkCreatePipelineLayout(device->handle, &pipelineLayoutInfo, NULL, &renderer->pipelineLayout);

    *pOut = renderer;
    return;

cleanup:
    kglDestroyRenderer(renderer);
}

int main() {
    KglInstance instance;
    kglCreateInstance(&instance);

    uint32_t physicalDeviceCount;
    kglEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);

    KglPhysicalDevice* physicalDevices = malloc(physicalDeviceCount * sizeof(KglPhysicalDevice));
    kglEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);

    for (int i = 0; i < physicalDeviceCount; i++) {
        VkPhysicalDeviceProperties properties = physicalDevices[i]->properties;
        uint32_t apiVersion = properties.apiVersion;
        printf("Index: %d; Version: %d.%d.%d; %s\n", 
            i, VK_API_VERSION_MAJOR(apiVersion), VK_API_VERSION_MINOR(apiVersion), VK_API_VERSION_PATCH(apiVersion), properties.deviceName);
    }

    KglPhysicalDevice physicalDevice = physicalDevices[0];
    free(physicalDevices);

    const DokWindowCreateInfo windowInfo = {
        .pTitle = "Kgl-Test",
        .height = 420,
        .width = 720
    };

    DokWindow window;
    dokCreateWindow(&windowInfo, &window);

    uint32_t maxTextures = 100;
    KglDevice device;
    kglCreateDevice(physicalDevice, window, maxTextures, &device);

    const KglRendererCreateInfo rendererInfo = {
        .framesInFlight = 3,
        .maxVertex = 1000,
        .maxIndices = 10000
    };

    KglRenderer renderer;
    kglCreateRenderer(device, &rendererInfo, &renderer);

    kglDestroyRenderer(renderer);

    kglDestroyDevice(device);
    kglDestroyInstance(instance);
}