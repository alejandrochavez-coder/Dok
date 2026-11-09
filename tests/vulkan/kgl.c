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
    uint32_t queueFamilyIndex;
};

typedef struct KglSwapchain {
    VkSwapchainKHR handle;
    VkSemaphore* imageSemaphores;
    VkImageView* imageViews;
    VkImage* images;
    VkImageView depthImageView;
    VmaAllocation depthImageAllocation;
    VkImage depthImage;
    VkFormat depthImageFormat;
    VkFormat format;
} KglSwapchain;

struct KglDevice_T {
    DokDeviceTable table;
    VkDevice handle;
    KglPhysicalDevice physicalDevice;
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
        .apiVersion = VK_API_VERSION_1_3
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

void kglCreateImage(
    KglDevice                   device, 
    VkExtent2D                  extent,
    VkFormat                    format,
    VkImageUsageFlags           usage,
    VmaAllocationCreateFlagBits flags,
    VkImage*                    pOutImage,
    VmaAllocation*              pOutImageAllocation) {

    VkImageCreateInfo imageInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent = (VkExtent3D){.width = extent.width, .height = extent.height, .depth = 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };

    VmaAllocationCreateInfo allocationInfo = {
        .flags = flags,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    vmaCreateImage(device->allocator, &imageInfo, &allocationInfo, pOutImage, pOutImageAllocation, NULL);
}

void kglCreateImageView(
    KglDevice           device, 
    VkImageAspectFlags  aspectMask,
    VkFormat            format,
    VkImage             image,
    VkImageView*        pOut
) {

    VkImageViewCreateInfo imageViewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .image = image,
        .subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = aspectMask,
            .levelCount = 1,
            .layerCount = 1
        }
    };

    device->table.vkCreateImageView(device->handle, &imageViewInfo, NULL, pOut);
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

    VkSwapchainCreateInfoKHR swapchainInfo = {
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

    VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    for (uint32_t i = 0; i < arrlen(pSwapchain->images); i++) {
        kglCreateImageView(
            device,
            VK_IMAGE_ASPECT_COLOR_BIT,
            pSwapchain->format,
            pSwapchain->images[i],
            &pSwapchain->imageViews[i]
        );
        device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &pSwapchain->imageSemaphores[i]);
    }

    if (pSwapchain->depthImage) vmaDestroyImage(device->allocator, pSwapchain->depthImage, pSwapchain->depthImageAllocation);
    if (pSwapchain->depthImageView) device->table.vkDestroyImageView(device->handle, pSwapchain->depthImageView, NULL);

    kglCreateImage(
        device,
        swapchainExtent,
        pSwapchain->depthImageFormat,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        &pSwapchain->depthImage,
        &pSwapchain->depthImageAllocation
    );

    kglCreateImageView(
        device,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        pSwapchain->depthImageFormat,
        pSwapchain->depthImage,
        &pSwapchain->depthImageView
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

    if (pSwapchain->depthImageView) device->table.vkDestroyImageView(device->handle, pSwapchain->depthImageView, NULL);
    if (pSwapchain->depthImage) vmaDestroyImage(device->allocator, pSwapchain->depthImage, pSwapchain->depthImageAllocation);
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

    VkDeviceQueueCreateInfo queueInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &priority
    };

    VkPhysicalDeviceVulkan12Features enabledVk12Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = VK_TRUE,
        .shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
        .descriptorBindingVariableDescriptorCount = VK_TRUE,
        .runtimeDescriptorArray = VK_TRUE,
        .bufferDeviceAddress = VK_TRUE  
    };

    VkPhysicalDeviceVulkan13Features enabledVk13Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &enabledVk12Features,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE
    };

    VkPhysicalDeviceFeatures enabledvk10Features = {
        .samplerAnisotropy = VK_TRUE
    };

    VkDeviceCreateInfo deviceInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &enabledVk13Features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueInfo,
        .enabledExtensionCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]),
        .ppEnabledExtensionNames = deviceExtensions,
        .pEnabledFeatures = &enabledvk10Features
    };

    if (instance->table.vkCreateDevice(physicalDevice->handle, &deviceInfo, NULL, &device->handle) != VK_SUCCESS) goto cleanup;
    dokLoadDeviceTable(instance->table.vkGetDeviceProcAddr, device->handle, &device->table);

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

struct KglRenderer_T {
    KglDevice device;
    VmaAllocation textureAllocation;
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkImage textureImage;
    VkCommandBuffer* commandBuffers;
    VkSemaphore* semaphores;
    VkFence* fences;
    VkDeviceAddress* uniformBufferAddresses;
    VmaAllocation* uniformBufferAllocations;
    VkBuffer* uniformBuffers;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
    uint32_t framesInFlight;
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

void kglDestroyRenderer(KglRenderer renderer) {
    KglDevice device = renderer->device;

    for (uint32_t i = 0; i < renderer->framesInFlight; i++) {
        if (renderer->semaphores) device->table.vkDestroySemaphore(device->handle, renderer->semaphores[i], NULL);
        if (renderer->fences) device->table.vkDestroyFence(device->handle, renderer->fences[i], NULL);
        if (renderer->uniformBuffers) vmaDestroyBuffer(device->allocator, renderer->uniformBuffers[i], renderer->uniformBufferAllocations[i]);
    }
    if (renderer->commandBuffers) free(renderer->commandBuffers);
    if (renderer->semaphores) free(renderer->semaphores);
    if (renderer->fences) free(renderer->fences);
    if (renderer->textureImage) vmaDestroyImage(device->allocator, renderer->textureImage, renderer->textureAllocation);
    if (renderer->textureImageView) device->table.vkDestroyImageView(device->handle, renderer->textureImageView, NULL);
    if (renderer->textureSampler) device->table.vkDestroySampler(device->handle, renderer->textureSampler, NULL);
    if (renderer->descriptorPool) device->table.vkDestroyDescriptorPool(device->handle, renderer->descriptorPool, NULL);
    if (renderer->descriptorSetLayout) device->table.vkDestroyDescriptorSetLayout(device->handle, renderer->descriptorSetLayout, NULL);
    free(renderer);
}

void kglCreateTextureImage(
    KglDevice               device, 
    const unsigned char*    imageData,
    VkExtent2D              extent,
    int                     channels,
    VkImage*                pOutImage,
    VmaAllocation*          pOutImageAllocation
) {
    kglCreateImage(
        device,
        extent,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0,
        pOutImage,
        pOutImageAllocation
    );

    VkCommandBuffer commandBuffer;
    kglBeginSingleTimeCommands(device, &commandBuffer);

    VkImageMemoryBarrier2 writeBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        .dstAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
        .image = *pOutImage,
        .subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .levelCount = 1,
            .layerCount = 1
        }
    };
    VkDependencyInfo writeBarrierInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &writeBarrier
    };
    device->table.vkCmdPipelineBarrier2(commandBuffer, &writeBarrierInfo);

    VkDeviceSize byteSize = extent.width * extent.height * channels;

    VmaAllocationInfo stagingBufferAllocationInfo;
    VmaAllocation stagingBufferAllocation;
    VkBuffer stagingBuffer;
    VkBufferCreateInfo stagingBufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = byteSize,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
    };
    VmaAllocationCreateInfo stagingBufferAllocationCreateInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    VkBufferImageCopy imageCopy = {
        .imageExtent = (VkExtent3D){.width = extent.width, .height = extent.height, .depth = 1},
        .imageSubresource = (VkImageSubresourceLayers) {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .layerCount = 1}
    };
    vmaCreateBuffer(device->allocator, &stagingBufferInfo, &stagingBufferAllocationCreateInfo, &stagingBuffer, &stagingBufferAllocation, &stagingBufferAllocationInfo);
    memcpy(stagingBufferAllocationInfo.pMappedData, imageData, byteSize);
    device->table.vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, *pOutImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);

    VkImageMemoryBarrier2 readBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
        .srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        .srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
        .dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT,
        .image = *pOutImage,
        .subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .levelCount = 1,
            .layerCount = 1
        }
    };
    VkDependencyInfo readBarrierInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &readBarrier
    };
    device->table.vkCmdPipelineBarrier2(commandBuffer, &readBarrierInfo);
    kglEndSubmitSingleTimeCommands(device, commandBuffer);
    vmaDestroyBuffer(device->allocator, stagingBuffer, stagingBufferAllocation);
}

void kglCreateRenderer(KglDevice device, uint32_t framesInFlight, KglRenderer* pOut) {
    KglRenderer renderer = calloc(1, sizeof(struct KglRenderer_T));
    if (!renderer) return;
    renderer->framesInFlight = framesInFlight;
    renderer->device = device;

    renderer->uniformBufferAllocations = malloc(framesInFlight * sizeof(VmaAllocation));
    renderer->uniformBufferAddresses = malloc(framesInFlight * sizeof(VkDeviceAddress));
    renderer->uniformBuffers = malloc(framesInFlight * sizeof(VkBuffer));
    renderer->commandBuffers = malloc(framesInFlight * sizeof(VkCommandBuffer));
    renderer->semaphores = malloc(framesInFlight * sizeof(VkSemaphore));
    renderer->fences = malloc(framesInFlight * sizeof(VkFence));
    if (!renderer->commandBuffers || !renderer->semaphores || !renderer->fences || 
        !renderer->uniformBufferAllocations || !renderer->uniformBuffers) goto cleanup;

    VkCommandBufferAllocateInfo commandBufferInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = framesInFlight,
        .commandPool = device->commandPool
    };

    device->table.vkAllocateCommandBuffers(device->handle, &commandBufferInfo, renderer->commandBuffers);
    VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };
    VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };
    VkBufferCreateInfo uniformBufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(UniformBufferObject),
        .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
    };
    VmaAllocationCreateInfo uniformBufferAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    for (uint32_t i = 0; i < framesInFlight; i++) {
        device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &renderer->semaphores[i]);
        device->table.vkCreateFence(device->handle, &fenceInfo, NULL, &renderer->fences[i]);
        vmaCreateBuffer(device->allocator, &uniformBufferInfo, &uniformBufferAllocationInfo, &renderer->uniformBuffers[i], &renderer->uniformBufferAllocations[i], NULL);
        
        VkBufferDeviceAddressInfo bufferAddressInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = renderer->uniformBuffers[i]
        };
        renderer->uniformBufferAddresses[i] = device->table.vkGetBufferDeviceAddress(device->handle, &bufferAddressInfo);
    }

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
    kglCreateTextureImage(
        device,
        pixels,
        imageExtent,
        channels,
        &renderer->textureImage,
        &renderer->textureAllocation
    );
    stbi_image_free(pixels);
    kglCreateImageView(
        device,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_FORMAT_R8G8B8A8_SRGB,
        renderer->textureImage,
        &renderer->textureImageView
    );
    VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST
    };
    device->table.vkCreateSampler(device->handle, &samplerInfo, NULL, &renderer->textureSampler);

    VkDescriptorImageInfo textureDescriptors[] = {
        (VkDescriptorImageInfo) {
            .imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
            .imageView = renderer->textureImageView,
            .sampler = renderer->textureSampler
        }
    };
    uint32_t textureDescriptorCount = sizeof(textureDescriptors) / sizeof(textureDescriptors[0]);

    VkDescriptorBindingFlags descriptorVariableFlags = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;
    VkDescriptorSetLayoutBindingFlagsCreateInfo descriptorBindingFlags = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
        .pBindingFlags = &descriptorVariableFlags,
        .bindingCount = 1
    };
    VkDescriptorSetLayoutBinding descriptorLayoutBindingTexture = {
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorCount = 1
    };
    VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = &descriptorBindingFlags,
        .pBindings = &descriptorLayoutBindingTexture,
        .bindingCount = 1
    };
    device->table.vkCreateDescriptorSetLayout(device->handle, &descriptorLayoutInfo, NULL, &renderer->descriptorSetLayout);

    VkDescriptorPoolSize poolSize = {
        .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = textureDescriptorCount
    };
    VkDescriptorPoolCreateInfo descriptorPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 1,
        .poolSizeCount = 1,
        .pPoolSizes = &poolSize
    };
    device->table.vkCreateDescriptorPool(device->handle, &descriptorPoolInfo, NULL, &renderer->descriptorPool);

    VkDescriptorSetVariableDescriptorCountAllocateInfo descriptorCountInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
        .pDescriptorCounts = &textureDescriptorCount,
        .descriptorSetCount = 1
    };
    VkDescriptorSetAllocateInfo descriptorAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = &descriptorCountInfo,
        .descriptorPool = renderer->descriptorPool,
        .pSetLayouts = &renderer->descriptorSetLayout,
        .descriptorSetCount = 1
    };
    device->table.vkAllocateDescriptorSets(device->handle, &descriptorAllocateInfo, &renderer->descriptorSet);

    VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .descriptorCount = textureDescriptorCount,
        .pImageInfo = textureDescriptors,
        .dstSet = renderer->descriptorSet,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
    };
    device->table.vkUpdateDescriptorSets(device->handle, 1, &writeDescriptorSet, 0, NULL);

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

    KglPhysicalDevice physicalDevice = physicalDevices[0];
    free(physicalDevices);

    DokWindowCreateInfo windowInfo = {
        .pTitle = "Kgl-Test",
        .height = 420,
        .width = 720
    };

    DokWindow window;
    dokCreateWindow(&windowInfo, &window);

    KglDevice device;
    kglCreateDevice(physicalDevice, window, &device);

    KglRenderer renderer;
    kglCreateRenderer(device, 2, &renderer);

    kglDestroyRenderer(renderer);

    kglDestroyDevice(device);
    kglDestroyInstance(instance);
}