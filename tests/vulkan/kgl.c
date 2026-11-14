#define DOK_VULKAN_TABLES_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
#define KGL_DEBUG

#include <dok/dok_vulkan_tables.h>
#include <dok/dok_core.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vk_mem_alloc.h"
#include "stb_image.h"
#include "linmath.h"
#include "stb_ds.h"

alignas(uint32_t) const uint8_t spirvShader[] = {
    #embed "demo.spv"
};

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
    VkCommandPool commandPool;
    KglSwapchain swapchain;
    VmaAllocator allocator;
    VkSurfaceKHR surface;
    DokWindow window;
    VkQueue queue;
};

typedef struct Vertex {
    vec3 position;
    vec2 uv;
} Vertex;

typedef struct PushInfo {
    VkDeviceAddress address;
} PushInfo;

typedef struct UniformBuffer {
    mat4x4 projection;
    mat4x4 view;
    mat4x4 model;
    VkDeviceAddress vertices;
} UniformBuffer;

Vertex vertices[] = {
    (Vertex){
        .position = (float[]){ 0.0f, -0.5f, 0.0f },
        .uv = (float[]){ 1.0f, 0.0f }
    },
    (Vertex){
        .position = (float[]){ 0.5f, 0.5f, 0.0f },
        .uv = (float[]){ 0.0f, 0.0f }
    },
    (Vertex){
        .position = (float[]){ 0.0f, 0.5f, 0.0f },
        .uv = (float[]){ 0.0f, 1.0f }
    }
};

uint16_t indices[] = {
    0, 1, 2
};

// Vertex vertices[] = {
//     (Vertex){
//         .position = (float[]){ -1.0f, -0.5f, 0.0f },
//         .uv = (float[]){ 1.0f, 0.0f } 
//     },
//     (Vertex){
//         .position = (float[]){ 0.5f, -1.5f, 0.0f },
//         .uv = (float[]){ 0.0f, 0.0f }
//     },
//     (Vertex){
//         .position = (float[]){ -0.5f, 1.5f, 0.0f },
//         .uv = (float[]){ 0.0f, 1.0f }
//     },
//     (Vertex){
//         .position = (float[]){ 0.0f, 1.5f, 0.0f },
//         .uv = (float[]){ 0.0f, 1.0f }
//     }
// };

// uint16_t indices[] = {
//     0, 1, 2, 2, 3, 0
// };

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
            VkPhysicalDeviceProperties2 properties = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
            };
            instance->table.vkGetPhysicalDeviceProperties2(handle, &properties);
            physicalDevice->properties = properties.properties;
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

typedef struct KglBuffer_T* KglBuffer;

struct KglBuffer_T {
    VmaAllocator        allocator;
    VmaAllocation       allocation;
    VmaAllocationInfo   allocationInfo;
    VkBuffer            handle;
    VkDeviceAddress     address;
    VkDeviceSize        size;
};

typedef struct KglBufferCreateInfo {
    VkDeviceSize                size;
    VkBufferUsageFlags          usage;
    VmaAllocationCreateFlags    flags;
    VmaMemoryUsage              memoryUsage;
} KglBufferCreateInfo;

void kglDestroyBuffer(KglBuffer buffer) {
    vmaDestroyBuffer(buffer->allocator, buffer->handle, buffer->allocation);
    free(buffer);
}

void kglCreateBuffer(
    KglDevice                   device, 
    const KglBufferCreateInfo*  pInfo,
    KglBuffer*                  pOut
) {
    KglBuffer buffer = malloc(sizeof(struct KglBuffer_T));
    const KglBufferCreateInfo info = *pInfo;
    const VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = info.size,
        .usage = info.usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    const VmaAllocationCreateInfo allocationInfo = {
        .flags = info.flags,
        .usage = info.memoryUsage
    };
    vmaCreateBuffer(device->allocator, &bufferInfo, &allocationInfo, &buffer->handle, &buffer->allocation, &buffer->allocationInfo);
    if (info.usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) {
        const VkBufferDeviceAddressInfo addressInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = buffer->handle
        };
        buffer->address = device->table.vkGetBufferDeviceAddress(device->handle, &addressInfo);
    }
    buffer->allocator = device->allocator;
    buffer->size = info.size;
    *pOut = buffer;
}

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

typedef struct KglImageCreateInfo {
    VkExtent3D extent;
    VkFormat format;
    VkImageUsageFlags usage;
    VmaAllocationCreateFlags flags;
    VkImageAspectFlags aspectMask;
} KglImageCreateInfo;

void kglDestroyImage(KglImage image) {
    KglDevice device = image->device;
    device->table.vkDestroyImageView(device->handle, image->view, NULL);
    vmaDestroyImage(device->allocator, image->handle, image->allocation);
    free(image);
}

void kglCreateImage(
    KglDevice                   device,
    const KglImageCreateInfo*   pInfo,
    KglImage*                   pOut
) {
    KglImage image = malloc(sizeof(struct KglImage_T));
    KglImageCreateInfo info = *pInfo;
    const VkImageCreateInfo imageInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = info.format,
        .extent = info.extent,
        .arrayLayers = 1,
        .mipLevels = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = info.usage
    };
    const VmaAllocationCreateInfo allocationInfo = {
        .flags = info.flags,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    vmaCreateImage(device->allocator, &imageInfo, &allocationInfo, &image->handle, &image->allocation, NULL);
    const VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = info.format,
        .image = image->handle,
        .subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = info.aspectMask,
            .levelCount = 1,
            .layerCount = 1
        }
    };
    device->table.vkCreateImageView(device->handle, &viewInfo, NULL, &image->view);
    image->extent = info.extent;
    image->device = device;
    *pOut = image;
}

void kglImagePasteData(KglImage image, VkDeviceSize dataSize, const void* pData) {
    KglDevice device = image->device;
    KglBuffer stagingBuffer;
    const KglBufferCreateInfo stagingBufferInfo = {
        .size = dataSize,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    kglCreateBuffer(device, &stagingBufferInfo, &stagingBuffer);
    memcpy(stagingBuffer->allocationInfo.pMappedData, pData, dataSize);
    VkCommandBuffer commandBuffer;
    kglBeginSingleTimeCommands(device, &commandBuffer);
    const VkImageMemoryBarrier2 writeBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        .dstAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .image = image->handle,
        .subresourceRange = (VkImageSubresourceRange){.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1}
    };
    const VkDependencyInfo barrierTexInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &writeBarrier
    };
    device->table.vkCmdPipelineBarrier2(commandBuffer, &barrierTexInfo);
    VkBufferImageCopy copyInfo = {
        .imageSubresource = (VkImageSubresourceLayers) {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .layerCount = 1},
        .imageExtent = image->extent
    };
    device->table.vkCmdCopyBufferToImage(commandBuffer, stagingBuffer->handle, image->handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyInfo);
    const VkImageMemoryBarrier2 readBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
        .srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
        .dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT,
        .subresourceRange = (VkImageSubresourceRange){.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1},
        .image = image->handle
    };
    const VkDependencyInfo readBarrierInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &readBarrier
    };
    device->table.vkCmdPipelineBarrier2(commandBuffer, &readBarrierInfo);
    kglEndSubmitSingleTimeCommands(device, commandBuffer);
    kglDestroyBuffer(stagingBuffer);
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
    for (uint32_t i = 0; i < arrlen(pSwapchain->images); i++) {
        device->table.vkDestroySemaphore(device->handle, pSwapchain->imageSemaphores[i], NULL);
        device->table.vkDestroyImageView(device->handle, pSwapchain->imageViews[i], NULL);
    }
    if (swapchainInfo.oldSwapchain != VK_NULL_HANDLE) {
        device->table.vkDestroySwapchainKHR(device->handle, swapchainInfo.oldSwapchain, NULL);
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
    const KglImageCreateInfo depthImageInfo = {
        .extent = (VkExtent3D) {.width = swapchainExtent.width, .height = swapchainExtent.height, .depth = 1 },
        .format = pSwapchain->depthImageFormat,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT
    };
    kglCreateImage(device, &depthImageInfo, &pSwapchain->depthImage);
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
    device->window = window;

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

    const char* deviceExtensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    uint32_t queueFamilyIndex = physicalDevice->queueFamilyIndex;
    float priority = 1.0f;

    const VkDeviceQueueCreateInfo queueInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &priority
    };

    VkPhysicalDeviceVulkan11Features enabledVk11Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .shaderDrawParameters = VK_TRUE
    };

    VkPhysicalDeviceVulkan12Features enabledVk12Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = &enabledVk11Features,
        .descriptorIndexing = VK_TRUE,
        .shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
        .descriptorBindingVariableDescriptorCount = VK_TRUE,
        .descriptorBindingSampledImageUpdateAfterBind = VK_TRUE,
        .descriptorBindingPartiallyBound = VK_TRUE,
        .runtimeDescriptorArray = VK_TRUE,
        .bufferDeviceAddress = VK_TRUE,
        .scalarBlockLayout = VK_TRUE
    };

    VkPhysicalDeviceVulkan13Features enabledVk13Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &enabledVk12Features,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE
    };

    VkPhysicalDeviceVulkan14Features enabledVk14Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
        .pNext = &enabledVk13Features,
        .maintenance5 = VK_TRUE
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
typedef struct KglFrameResource_T* KglFrameResource;

void kglMapCopyBufferData(KglBuffer buffer, const void* data, size_t byteSize) {
    void* pData;
    vmaMapMemory(buffer->allocator, buffer->allocation, &pData);
    memcpy(pData, data, byteSize);
    vmaUnmapMemory(buffer->allocator, buffer->allocation);
}

struct KglFrameResource_T {
    KglDevice device;
    VkCommandBuffer commandBuffer;
    VkSemaphore semaphore;
    KglBuffer uniformBuffer;
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
    const VkSemaphoreCreateInfo semaphoreInfo = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    const VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };
    const KglBufferCreateInfo bufferInfo = {
        .size = sizeof(UniformBuffer),
        .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
            VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | 
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    device->table.vkAllocateCommandBuffers(device->handle, &commandBufferInfo, &frameResource->commandBuffer);
    device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &frameResource->semaphore);
    device->table.vkCreateFence(device->handle, &fenceInfo, NULL, &frameResource->fence);
    kglCreateBuffer(device, &bufferInfo, &frameResource->uniformBuffer);
    frameResource->device = device;
    *pOut = frameResource;
}

void kglFrameResourceCopyUniform(KglFrameResource frameResource, UniformBuffer* pUniform) {
    memcpy(frameResource->uniformBuffer->allocationInfo.pMappedData, pUniform, sizeof(*pUniform));
}

typedef struct KglTextureDescriptor_T* KglTextureDescriptor;
struct KglTextureDescriptor_T {
    VkDescriptorImageInfo info;
    KglImage image;
};

typedef struct KglTextureDescriptorCreateInfo {
    VkDeviceSize dataSize;
    const void* pData;
    VkFormat format;
    VkExtent3D extent;
} KglTextureDescriptorCreateInfo;

void kglDestroyTextureDescriptor(KglTextureDescriptor textureDescriptor) {
    KglDevice device = textureDescriptor->image->device;
    device->table.vkDestroySampler(device->handle, textureDescriptor->info.sampler, NULL);
    kglDestroyImage(textureDescriptor->image);
    free(textureDescriptor);
}

void kglCreateTextureDescriptor(
    KglDevice device, 
    const KglTextureDescriptorCreateInfo* pInfo, 
    KglTextureDescriptor* pOut
) {

    KglTextureDescriptor textureDescriptor = malloc(sizeof(struct KglTextureDescriptor_T));
    KglTextureDescriptorCreateInfo info = *pInfo;
    const VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST
    };
    device->table.vkCreateSampler(device->handle, &samplerInfo, NULL, &textureDescriptor->info.sampler);
    const KglImageCreateInfo textureImageInfo = {
        .format = info.format,
        .extent = info.extent,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    };
    kglCreateImage(device, &textureImageInfo, &textureDescriptor->image);
    kglImagePasteData(textureDescriptor->image, info.dataSize, info.pData);
    textureDescriptor->info.imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    textureDescriptor->info.imageView = textureDescriptor->image->view;
    *pOut = textureDescriptor;
}

typedef struct KglRendererCreateInfo {
    uint32_t framesInFlight;
    uint32_t maxIndices;
    uint32_t maxVertex;
} KglRendererCreateInfo;

struct KglRenderer_T {
    KglDevice device;
    KglFrameResource* frameResources;
    KglTextureDescriptor textureDescriptor;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptolPool;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    VkDescriptorSet textureDescriptorSet;
    KglBuffer vertexBuffer;
    KglBuffer indexBuffer;
    uint32_t indexCount;
    uint32_t framesInFlight;
    uint32_t frameIndex;
    double elapsedTime;
};

void kglDestroyRenderer(KglRenderer renderer) {
    KglDevice device = renderer->device;
    for (int i = 0; i < renderer->framesInFlight; i++) {
        kglDestroyFrameResource(renderer->frameResources[i]);
    }
    kglDestroyTextureDescriptor(renderer->textureDescriptor);
    kglDestroyBuffer(renderer->vertexBuffer);
    kglDestroyBuffer(renderer->indexBuffer);
    device->table.vkDestroyPipeline(device->handle, renderer->pipeline, NULL);
    device->table.vkDestroyPipelineLayout(device->handle, renderer->pipelineLayout, NULL);
    device->table.vkDestroyDescriptorSetLayout(device->handle, renderer->descriptorSetLayout, NULL);
    device->table.vkDestroyDescriptorPool(device->handle, renderer->descriptolPool, NULL);
    free(renderer);
}

void kglCreateRenderer(KglDevice device, const KglRendererCreateInfo* pInfo, KglRenderer* pOut) {
    KglRenderer renderer = calloc(1, sizeof(struct KglRenderer_T));
    if (!renderer) return;
    renderer->framesInFlight = pInfo->framesInFlight;
    renderer->indexCount = sizeof(indices) / sizeof(indices[0]);
    renderer->device = device;

    KglPhysicalDevice physicalDevice = device->physicalDevice;
    KglInstance instance = physicalDevice->instance;

    renderer->frameResources = malloc(pInfo->framesInFlight * sizeof(KglFrameResource));
    for (uint32_t i = 0; i < pInfo->framesInFlight; i++) {
        kglCreateFrameResource(device, &renderer->frameResources[i]);
    }

    VkDeviceSize vertexBufferSize = sizeof(vertices);
    VkDeviceSize indexBufferSize = sizeof(indices);
    const KglBufferCreateInfo vertexBufferInfo = {
        .size = vertexBufferSize,
        .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
            VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    const KglBufferCreateInfo indexBufferInfo = {
        .size = indexBufferSize,
        .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
            VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    kglCreateBuffer(device, &vertexBufferInfo, &renderer->vertexBuffer);
    kglCreateBuffer(device, &indexBufferInfo, &renderer->indexBuffer);
    assert(renderer->vertexBuffer->allocationInfo.pMappedData != NULL);
    assert(renderer->indexBuffer->allocationInfo.pMappedData != NULL);
    memcpy(renderer->vertexBuffer->allocationInfo.pMappedData, vertices, vertexBufferSize);
    memcpy(renderer->indexBuffer->allocationInfo.pMappedData, indices, (size_t)indexBufferSize);

    int width, height, channels;
    stbi_uc* pixels = stbi_load_from_memory(imageTexture, sizeof(imageTexture), &width, &height, &channels, STBI_rgb_alpha);
    const KglTextureDescriptorCreateInfo textureDescriptorInfo = {
        .dataSize = width * height * channels,
        .extent = (VkExtent3D) {.width = width, .height = height, .depth = 1 },
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .pData = pixels
    };
    kglCreateTextureDescriptor(device, &textureDescriptorInfo, &renderer->textureDescriptor);
    stbi_image_free(pixels);

    VkDescriptorBindingFlags descriptorVariableFlags[] = {
        VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT
    };
    const VkDescriptorSetLayoutBindingFlagsCreateInfo descriptorBindingFlags = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
        .bindingCount = sizeof(descriptorVariableFlags) / sizeof(descriptorVariableFlags[0]),
        .pBindingFlags = descriptorVariableFlags
    };
    VkDescriptorSetLayoutBinding descriptorLayoutBindings[] = {
        (VkDescriptorSetLayoutBinding) {
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
        }
    };
    const VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = &descriptorBindingFlags,
        .bindingCount = sizeof(descriptorLayoutBindings) / sizeof(descriptorLayoutBindings[0]),
        .pBindings = descriptorLayoutBindings  
    };
    device->table.vkCreateDescriptorSetLayout(device->handle, &descriptorLayoutInfo, NULL, &renderer->descriptorSetLayout);
    VkDescriptorPoolSize poolSizes[] = {
        (VkDescriptorPoolSize) {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1
        }
    };
    const VkDescriptorPoolCreateInfo descriptorPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 1,
        .poolSizeCount = sizeof(poolSizes) / sizeof(poolSizes[0]),
        .pPoolSizes = poolSizes
    };
    device->table.vkCreateDescriptorPool(device->handle, &descriptorPoolInfo, NULL, &renderer->descriptolPool);
    uint32_t variableDescriptorCount = 1;
    VkDescriptorSetVariableDescriptorCountAllocateInfo variableDescriptorCountInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
        .descriptorSetCount = 1,
        .pDescriptorCounts = &variableDescriptorCount
    };
    const VkDescriptorSetAllocateInfo textureDescriptorSetInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = &variableDescriptorCountInfo,
        .descriptorPool = renderer->descriptolPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &renderer->descriptorSetLayout
    };
    device->table.vkAllocateDescriptorSets(device->handle, &textureDescriptorSetInfo, &renderer->textureDescriptorSet);
    const VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = renderer->textureDescriptorSet,
        .dstBinding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .pImageInfo = &renderer->textureDescriptor->info
    };
    device->table.vkUpdateDescriptorSets(device->handle, 1, &writeDescriptorSet, 0, NULL);
    const VkShaderModuleCreateInfo shaderInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = sizeof(spirvShader),
        .pCode = (uint32_t*)spirvShader
    };
    VkPipelineShaderStageCreateInfo shaderStages[] = {
        (VkPipelineShaderStageCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .pName = "vertMain",
            .pNext = &shaderInfo
        },
        (VkPipelineShaderStageCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pName = "fragMain",
            .pNext = &shaderInfo
        }
    };
    VkPushConstantRange pushConstantRanges[] = {
        (VkPushConstantRange) {
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .size = sizeof(VkDeviceAddress)
        }
    };
    const VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &renderer->descriptorSetLayout,
        .pushConstantRangeCount = sizeof(pushConstantRanges) / sizeof(pushConstantRanges[0]),
        .pPushConstantRanges = pushConstantRanges
    };
    device->table.vkCreatePipelineLayout(device->handle, &pipelineLayoutInfo, NULL, &renderer->pipelineLayout);
    const VkPipelineVertexInputStateCreateInfo vertexInputState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
    };
    const VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };
    const VkPipelineViewportStateCreateInfo viewportState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1
    };
    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    const VkPipelineDynamicStateCreateInfo dynamicState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]),
        .pDynamicStates = dynamicStates
    };
    const VkPipelineRenderingCreateInfo renderingInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &device->swapchain.format
    };
    const VkPipelineColorBlendAttachmentState blendAttachment = { .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | 
        VK_COLOR_COMPONENT_G_BIT | 
        VK_COLOR_COMPONENT_B_BIT | 
        VK_COLOR_COMPONENT_A_BIT 
    };
    const VkPipelineColorBlendStateCreateInfo colorBlendState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &blendAttachment
    };
    const VkPipelineRasterizationStateCreateInfo rasterizationState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .lineWidth = 1.0f
    };
    const VkPipelineMultisampleStateCreateInfo multisampleState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
    };
    const VkGraphicsPipelineCreateInfo pipelineInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &renderingInfo,
        .stageCount = sizeof(shaderStages) / sizeof(shaderStages[0]),
        .pStages = shaderStages,
        .pVertexInputState = &vertexInputState,
        .pInputAssemblyState = &inputAssemblyState,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizationState,
        .pMultisampleState = &multisampleState,
        .pColorBlendState = &colorBlendState,
        .pDynamicState = &dynamicState,
        .layout = renderer->pipelineLayout
    };
    device->table.vkCreateGraphicsPipelines(device->handle, NULL, 1, &pipelineInfo, NULL, &renderer->pipeline);

    *pOut = renderer;
    return;

cleanup:
    kglDestroyRenderer(renderer);
}

void destroyObserverCallback(DokWindow window, void* pUserData) {
    *((bool*)pUserData) = false;
}

void resizeObserverCallback(DokWindow window, uint32_t width, uint32_t height, void* pUserData) {
    KglDevice device = *((KglDevice*)pUserData);
    device->table.vkDeviceWaitIdle(device->handle);
    kglUpdateSwapchain(device, VK_PRESENT_MODE_FIFO_KHR);
}

void kglProcessRenderer(KglRenderer renderer, double deltaTime) {
    KglDevice device = renderer->device;
    uint32_t frameIndex = renderer->frameIndex;
    renderer->elapsedTime += deltaTime;

    KglFrameResource frameResource = renderer->frameResources[frameIndex];
    VkFence fence = frameResource->fence;

    device->table.vkWaitForFences(device->handle, 1, &fence, VK_TRUE, UINT64_MAX);
    device->table.vkResetFences(device->handle, 1, &fence);

    VkCommandBuffer commandBuffer = frameResource->commandBuffer;
    VkSemaphore imageAcquireSemaphore = frameResource->semaphore;

    uint32_t imageIndex;
    device->table.vkAcquireNextImageKHR(device->handle, device->swapchain.handle, UINT64_MAX, imageAcquireSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkSemaphore renderCompleteSemaphore = device->swapchain.imageSemaphores[imageIndex];
    VkImageView imageView = device->swapchain.imageViews[imageIndex];
    VkImage image = device->swapchain.images[imageIndex];

    VkExtent2D windowExtent;
    dokGetWindowExtent(device->window, &windowExtent);

    mat4x4 identity;
    mat4x4_identity(identity);

    UniformBuffer ubo = { 0 };
    // mat4x4_rotate(ubo.model, identity, 0.0f, 0.0f, 1.0f, 0);
    mat4x4_identity(ubo.model);
    // mat4x4_look_at(ubo.view, (vec3){renderer->elapsedTime, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f});
    mat4x4_identity(ubo.view);
    // mat4x4_perspective(ubo.projection, 0.8, (float)(windowExtent.width) / (float)(windowExtent.height), 0.1, 32.0f);
    // mat4x4 vulkanCorrection = {
    //     {1, 0,   0,   0},
    //     {0, 1,   0,   0},
    //     {0, 0, 0.5f,  0},
    //     {0, 0, 0.5f,  1}
    // };
    // mat4x4_mul(ubo.projection, vulkanCorrection, ubo.projection);
    mat4x4_identity(ubo.projection);
    ubo.vertices = renderer->vertexBuffer->address;
    kglFrameResourceCopyUniform(frameResource, &ubo);

    device->table.vkResetCommandBuffer(commandBuffer, 0);
    const VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    device->table.vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VkImageMemoryBarrier2 outputBarriers[] = {
        (VkImageMemoryBarrier2) {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .image = image,
            .subresourceRange = (VkImageSubresourceRange) {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .levelCount = 1,
                .layerCount = 1
            }
        },
        // (VkImageMemoryBarrier2) {
        //     .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        //     .srcStageMask = VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
        //     .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        //     .dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
        //     .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        //     .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        //     .newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
        //     .image = device->swapchain.depthImage->handle,
        //     .subresourceRange = (VkImageSubresourceRange) {
        //         .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
        //         .levelCount = 1,
        //         .layerCount = 1
        //     }
        // }
    };
    const VkDependencyInfo writeBarrierInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = sizeof(outputBarriers) / sizeof(outputBarriers[0]),
        .pImageMemoryBarriers = outputBarriers
    };
    device->table.vkCmdPipelineBarrier2(commandBuffer, &writeBarrierInfo);
    const VkRenderingAttachmentInfo colorAttachmentInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = imageView,
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = (VkClearValue) { .color = (VkClearColorValue) {.float32 = {0.0f, 0.0f, 0.0f, 1.0f}} }
    };
    // const VkRenderingAttachmentInfo depthInfo = {
    //     .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
    //     .imageView = depthImage->view,
    //     .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
    //     .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    //     .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    //     .clearValue = (VkClearValue) { .depthStencil = (VkClearDepthStencilValue) {.depth = 1.0f} }
    // }; 
    const VkRenderingInfo renderingInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = (VkRect2D) {.extent = windowExtent},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentInfo,
        // .pDepthAttachment = &depthInfo,
    };
    device->table.vkCmdBeginRendering(commandBuffer, &renderingInfo);

    const VkViewport viewport = {
        // .y = windowExtent.height,
        .width = windowExtent.width,
        .height = (float)windowExtent.height,
        .maxDepth = 1.0f
    };
    const VkRect2D scissor = { .extent = windowExtent };
    const VkDeviceSize vertexOffset = 0;
    device->table.vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    device->table.vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    device->table.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipeline);
    device->table.vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelineLayout, 0, 1, &renderer->textureDescriptorSet, 0, NULL);
    // device->table.vkCmdBindVertexBuffers(commandBuffer, 0, 1, &renderer->buffer->handle, &vertexOffset);
    device->table.vkCmdBindIndexBuffer(commandBuffer, renderer->indexBuffer->handle, 0, VK_INDEX_TYPE_UINT16);

    device->table.vkCmdPushConstants(commandBuffer, renderer->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VkDeviceAddress), &frameResource->uniformBuffer->address);
    
    device->table.vkCmdDrawIndexed(commandBuffer, sizeof(indices) / sizeof(indices[0]), 1, 0, 0, 0);

    device->table.vkCmdEndRendering(commandBuffer);
    const VkImageMemoryBarrier2 presentBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
        .dstAccessMask = VK_ACCESS_2_NONE,
        .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .image = image,
        .subresourceRange = (VkImageSubresourceRange) {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .levelCount = 1,
            .layerCount = 1
        }
    };
    const VkDependencyInfo presentBarrierInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &presentBarrier
    };
    device->table.vkCmdPipelineBarrier2(commandBuffer, &presentBarrierInfo);
    device->table.vkEndCommandBuffer(commandBuffer);
    device->table.vkQueueWaitIdle(device->queue);

    const VkSemaphoreSubmitInfo waitSemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = imageAcquireSemaphore,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    const VkCommandBufferSubmitInfo commandBufferSubmitInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = commandBuffer
    };
    const VkSemaphoreSubmitInfo signalSemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = renderCompleteSemaphore,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    const VkSubmitInfo2 submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos = &waitSemaphoreInfo,
        .commandBufferInfoCount = 1,
        .pCommandBufferInfos = &commandBufferSubmitInfo,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos = &signalSemaphoreInfo
    };
    device->table.vkQueueSubmit2(device->queue, 1, &submitInfo, fence);
    const VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderCompleteSemaphore,
        .swapchainCount = 1,
        .pSwapchains = &device->swapchain.handle,
        .pImageIndices = &imageIndex
    };
    device->table.vkQueuePresentKHR(device->queue, &presentInfo);
    renderer->frameIndex = (frameIndex + 1) % renderer->framesInFlight;
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
        .framesInFlight = 1,
        .maxVertex = 1000,
        .maxIndices = 10000
    };

    KglRenderer renderer;
    kglCreateRenderer(device, &rendererInfo, &renderer);

    const DokWindowResizeObserverCreateInfo resizeObserverInfo = {
        .pUserData = &device,
        .pfnCallback = resizeObserverCallback
    };
    DokWindowResizeObserver resizeObserver;
    dokCreateResizeObserver(window, &resizeObserverInfo, &resizeObserver);

    bool exists = true;
    const DokWindowDestroyObserverCreateInfo destroyObserverInfo = {
        .pUserData = &exists,
        .pfnCallback = destroyObserverCallback
    };
    DokWindowDestroyObserver destroyObserver;
    dokCreateDestroyObserver(window, &destroyObserverInfo, &destroyObserver);

    uint64_t timeFrequency;
    uint64_t time;
    dokGetTimeFrequency(&timeFrequency);
    dokGetTime(&time);

    double scale = 1.0 / timeFrequency;

    while (exists) {
        uint64_t oldTime = time;
        dokGetTime(&time);

        double deltaTime = (time - oldTime) * scale;
        kglProcessRenderer(renderer, deltaTime);
        dokPollWindow(window);
    }

    device->table.vkDeviceWaitIdle(device->handle);
    kglDestroyRenderer(renderer);

    kglDestroyDevice(device);
    kglDestroyInstance(instance);
}