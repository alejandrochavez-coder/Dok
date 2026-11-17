#define DOK_VULKAN_TABLES_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
// #define KGL_DEBUG
#define TINYGLTF3_IMPLEMENTATION

#include <dok/dok_vulkan_tables.h>
#include <dok/dok_core.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vk_mem_alloc.h"
#include "stb_image.h"
#include "linmath.h"
#include "stb_ds.h"
#include "tiny_gltf_v3.h"

alignas(uint32_t) const uint8_t spirvShader[] = {
    #embed "demo.spv"
};

const uint8_t glb[] = {
    #embed "v1_ultrakill.glb"
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

typedef struct KglTexture_T* KglTexture;
struct KglTexture_T {
    size_t index;
};

typedef struct KglTextureInfo {
    VkDescriptorImageInfo descriptor;
    KglImage image;
} KglTextureInfo;

struct KglDevice_T {
    DokDeviceTable table;
    VkDevice handle;
    KglPhysicalDevice physicalDevice;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet constantDescriptorSet;
    VkDescriptorSet textureDescriptorSet;
    KglTextureInfo* textures;
    VkCommandPool commandPool;
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

typedef struct FrameConstants {
    mat4x4 projection;
    mat4x4 view;
} FrameConstants;

typedef struct Position {
    mat4x4 model;
} Position;

typedef struct PushConstants {
    VkDeviceAddress frameConstants;
    VkDeviceAddress position;
    VkDeviceAddress vertices;
    VkDeviceAddress indices;
    uint32_t textureIndex;
} PushConstants;

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

void kglBufferCopyData(KglBuffer buffer, const void* data, size_t dataSize) {
    memcpy(buffer->allocationInfo.pMappedData, data, dataSize);
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

typedef struct KglDeviceCreateInfo {
    DokWindow window;
    uint32_t maxTextures;
} KglDeviceCreateInfo;

void kglDestroyDevice(KglDevice device) {
    KglInstance instance = device->physicalDevice->instance;
    if (device->descriptorSetLayout) device->table.vkDestroyDescriptorSetLayout(device->handle, device->descriptorSetLayout, NULL);
    if (device->descriptorPool) device->table.vkDestroyDescriptorPool(device->handle, device->descriptorPool, NULL);
    if (device->commandPool) device->table.vkDestroyCommandPool(device->handle, device->commandPool, NULL);
    if (device->surface) instance->table.vkDestroySurfaceKHR(instance->handle, device->surface, NULL);
    if (device->allocator) vmaDestroyAllocator(device->allocator);
    device->table.vkDestroyDevice(device->handle, NULL);
    free(device);
}

void kglCreateDevice(
    KglPhysicalDevice   physicalDevice,
    const KglDeviceCreateInfo* pInfo,
    KglDevice*          pOut
) {
    
    KglDevice device = calloc(1, sizeof(struct KglDevice_T));
    const KglDeviceCreateInfo info = *pInfo;
    if (!device) return;

    KglInstance instance = physicalDevice->instance;
    const DokVulkanInfo vulkanInfo = {
        .instanceProcAddr = instance->table.vkGetInstanceProcAddr,
        .instance = instance->handle
    };
    dokCreateWindowSurface(&vulkanInfo, info.window, &device->surface);
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
    const VkPhysicalDeviceVulkan14Features enabledVk14Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
        .pNext = &enabledVk13Features,
        .maintenance5 = VK_TRUE
    };
    const VkPhysicalDeviceFeatures enabledVk10Features = {
        .samplerAnisotropy = VK_TRUE,
        .shaderInt16 = VK_TRUE
    };
    const VkDeviceCreateInfo deviceInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &enabledVk14Features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueInfo,
        .enabledExtensionCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]),
        .ppEnabledExtensionNames = deviceExtensions,
        .pEnabledFeatures = &enabledVk10Features
    };
    if (instance->table.vkCreateDevice(physicalDevice->handle, &deviceInfo, NULL, &device->handle) != VK_SUCCESS) goto cleanup;
    dokLoadDeviceTable(instance->table.vkGetDeviceProcAddr, device->handle, &device->table);
    const VkCommandPoolCreateInfo commandPoolInfo = {
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
    VkDescriptorPoolSize poolSizes[] = {
        (VkDescriptorPoolSize) {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = info.maxTextures
        }
    };
    const VkDescriptorPoolCreateInfo descriptorPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 1,
        .poolSizeCount = sizeof(poolSizes) / sizeof(poolSizes[0]),
        .pPoolSizes = poolSizes
    };
    device->table.vkCreateDescriptorPool(device->handle, &descriptorPoolInfo, NULL, &device->descriptorPool);
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
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .descriptorCount = info.maxTextures
        }
    };
    const VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = &descriptorBindingFlags,
        .bindingCount = sizeof(descriptorLayoutBindings) / sizeof(descriptorLayoutBindings[0]),
        .pBindings = descriptorLayoutBindings  
    };
    device->table.vkCreateDescriptorSetLayout(device->handle, &descriptorLayoutInfo, NULL, &device->descriptorSetLayout);
    const VkDescriptorSetVariableDescriptorCountAllocateInfo variableDescriptorCountInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
        .descriptorSetCount = 1,
        .pDescriptorCounts = &info.maxTextures
    };
    const VkDescriptorSetAllocateInfo textureDescriptorSetInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = &variableDescriptorCountInfo,
        .descriptorPool = device->descriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &device->descriptorSetLayout
    };
    // const VkDescriptorSetAllocateInfo
    device->table.vkAllocateDescriptorSets(device->handle, &textureDescriptorSetInfo, &device->textureDescriptorSet);
    device->physicalDevice = physicalDevice;
    device->window = info.window;
    *pOut = device;
    return;

cleanup:
    kglDestroyDevice(device);
}

typedef struct KglSwapchain_T* KglSwapchain;
typedef struct KglSwapchainResource {
    KglDevice device;
    KglImage depthImage;
    VkSemaphore semaphore;
    VkImageView view;
    VkImage image;
} KglSwapchainResource;

struct KglSwapchain_T {
    KglDevice device;
    VkSwapchainKHR handle;
    KglSwapchainResource* resources;
    VkSwapchainCreateInfoKHR info;
    VkFormat depthFormat;
};

typedef struct KglSwapchainCreateInfo {
    VkPresentModeKHR presentMode;
    VkColorSpaceKHR colorSpace;
    VkFormat format;
} KglSwapchainCreateInfo;

void kglDestroySwapchainResource(KglSwapchainResource* resource) {
    const KglDevice device = resource->device;
    device->table.vkDestroySemaphore(device->handle, resource->semaphore, NULL);
    device->table.vkDestroyImageView(device->handle, resource->view, NULL);
    kglDestroyImage(resource->depthImage);
}

void kglCreateSwapchainResource(KglDevice device, VkImage image, VkFormat format, VkExtent2D extent, VkFormat depthFormat, KglSwapchainResource* pOut) {
    const VkSemaphoreCreateInfo semaphoreInfo = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    const VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .format = format,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .subresourceRange = (VkImageSubresourceRange) { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .layerCount = 1, .levelCount = 1 }
    };
    const KglImageCreateInfo depthImageInfo = {
        .extent = (VkExtent3D) {.width = extent.width, .height = extent.height, .depth = 1 },
        .format = depthFormat,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT
    };
    device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &pOut->semaphore);
    device->table.vkCreateImageView(device->handle, &viewInfo, NULL, &pOut->view);
    kglCreateImage(device, &depthImageInfo, &pOut->depthImage);
    pOut->device = device;
    pOut->image = image;
}

void kglSwapchainUpdate(KglSwapchain swapchain) {
    const KglDevice device = swapchain->device;
    const KglPhysicalDevice physicalDevice = device->physicalDevice;
    VkSurfaceCapabilitiesKHR surfaceCapabilites;
    physicalDevice->instance->table.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->handle, device->surface, &surfaceCapabilites);
    VkExtent2D swapchainExtent = surfaceCapabilites.currentExtent;
    if (swapchainExtent.width == 0xFFFFFFFF) {
        dokGetWindowExtent(device->window, &swapchainExtent);
    }
    swapchain->info.imageExtent = swapchainExtent;
    swapchain->info.oldSwapchain = swapchain->handle;
    device->table.vkCreateSwapchainKHR(device->handle, &swapchain->info, NULL, &swapchain->handle);
    device->table.vkDestroySwapchainKHR(device->handle, swapchain->info.oldSwapchain, NULL);
    for (uint32_t i = 0; i < arrlen(swapchain->resources); i++) {
        kglDestroySwapchainResource(&swapchain->resources[i]);
    }
    uint32_t imageCount;
    device->table.vkGetSwapchainImagesKHR(device->handle, swapchain->handle, &imageCount, NULL);
    VkImage* images = malloc(imageCount * sizeof(VkImage));
    arrsetlen(swapchain->resources, imageCount);
    device->table.vkGetSwapchainImagesKHR(device->handle, swapchain->handle, &imageCount, images);
    for (uint32_t i = 0; i < imageCount; i++) {
        kglCreateSwapchainResource(device, images[i], swapchain->info.imageFormat, swapchainExtent, swapchain->depthFormat, &swapchain->resources[i]);
    }
    free(images);
}

void kglDestroySwapchain(KglSwapchain swapchain) {
    const KglDevice device = swapchain->device;
    for (uint32_t i = 0; i < arrlen(swapchain->resources); i++) {
        kglDestroySwapchainResource(&swapchain->resources[i]);
    }
    device->table.vkDestroySwapchainKHR(device->handle, swapchain->handle, NULL);
}

void kglCreateSwapchain(
    KglDevice                       device, 
    const KglSwapchainCreateInfo*   pInfo,
    KglSwapchain*                   pOut
) {
    KglSwapchain swapchain = calloc(1, sizeof(struct KglSwapchain_T));
    const KglSwapchainCreateInfo info = *pInfo;
    const KglPhysicalDevice physicalDevice = device->physicalDevice;
    const KglInstance instance = physicalDevice->instance;
    VkSurfaceCapabilitiesKHR surfaceCapabilites;
    instance->table.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->handle, device->surface, &surfaceCapabilites);
    VkExtent2D swapchainExtent = surfaceCapabilites.currentExtent;
    if (swapchainExtent.width == 0xFFFFFFFF) {
        dokGetWindowExtent(device->window, &swapchainExtent);
    }
    VkFormat depthFormats[] = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT };
    for (size_t i = 0; i < (sizeof(depthFormats) / sizeof(depthFormats[0])); i++) {
        //Laboratory 802 laptops primary gpu dont support 'vkGetPhysicalDeviceFormatProperties2'
        //for some reason
        VkFormatProperties formatProperties;
        instance->table.vkGetPhysicalDeviceFormatProperties(physicalDevice->handle, depthFormats[i], &formatProperties);
        if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            swapchain->depthFormat = depthFormats[i];
            break;
        }
    }
    swapchain->info = (VkSwapchainCreateInfoKHR) {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = device->surface,
        .minImageCount = surfaceCapabilites.minImageCount,
        .imageFormat = info.format,
        .imageColorSpace = info.colorSpace,
        .imageExtent = swapchainExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = surfaceCapabilites.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = info.presentMode,
        .clipped = VK_TRUE,
    };
    device->table.vkCreateSwapchainKHR(device->handle, &swapchain->info, NULL, &swapchain->handle);
    uint32_t imageCount;
    device->table.vkGetSwapchainImagesKHR(device->handle, swapchain->handle, &imageCount, NULL);
    VkImage* images = malloc(imageCount * sizeof(VkImage));
    arrsetlen(swapchain->resources, imageCount);
    device->table.vkGetSwapchainImagesKHR(device->handle, swapchain->handle, &imageCount, images);
    for (uint32_t i = 0; i < imageCount; i++) {
        kglCreateSwapchainResource(device, images[i], swapchain->info.imageFormat, swapchainExtent, swapchain->depthFormat, &swapchain->resources[i]);
    }
    free(images);
    swapchain->device = device;
    *pOut = swapchain;
}

void kglSwapchainAcquireNextResource(KglSwapchain swapchain, VkSemaphore semaphore, KglSwapchainResource* pOut) {
    const KglDevice device = swapchain->device;

    uint32_t imageIndex;
    device->table.vkAcquireNextImageKHR(device->handle, swapchain->handle, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);
    *pOut = swapchain->resources[imageIndex];
}

typedef struct KglRenderer_T* KglRenderer;

typedef struct KglFrameResource {
    KglDevice device;
    VkCommandBuffer commandBuffer;
    VkSemaphore semaphore;
    KglBuffer uniformBuffer;
    VkFence fence;
} KglFrameResource;

struct KglRenderer_T {
    KglSwapchain swapchain;
    KglFrameResource* frameResources;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    uint32_t framesInFlight;
    uint32_t frameIndex;
    uint32_t imageIndex;
};

void kglDestroyFrameResource(KglFrameResource* frameResource) {
    KglDevice device = frameResource->device;
    kglDestroyBuffer(frameResource->uniformBuffer);
    device->table.vkDestroySemaphore(device->handle, frameResource->semaphore, NULL);
    device->table.vkDestroyFence(device->handle, frameResource->fence, NULL);
}

void kglCreateFrameResource(
    KglDevice device, 
    KglFrameResource* pOut
) {

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
        .size = sizeof(FrameConstants),
        .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
            VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | 
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    device->table.vkAllocateCommandBuffers(device->handle, &commandBufferInfo, &pOut->commandBuffer);
    device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &pOut->semaphore);
    device->table.vkCreateFence(device->handle, &fenceInfo, NULL, &pOut->fence);
    kglCreateBuffer(device, &bufferInfo, &pOut->uniformBuffer);
    pOut->device = device;
}

void kglFrameResourceCopyUniform(KglFrameResource* frameResource, FrameConstants* pUniform) {
    memcpy(frameResource->uniformBuffer->allocationInfo.pMappedData, pUniform, sizeof(*pUniform));
}

typedef struct KglModel_T* KglModel;
struct KglModel_T {
    KglBuffer vertexBuffer;
    KglBuffer indexBuffer;
    size_t indexCount;
};

typedef struct KglModelCreateInfo {
    size_t vertexDataSize;
    const void* pVertexData;
    size_t indexDataSize;
    const void* pIndexData;
    size_t indexCount;
} KglModelCreateInfo;

void kglDestroyModel(KglModel model) {
    kglDestroyBuffer(model->vertexBuffer);
    kglDestroyBuffer(model->indexBuffer);
    free(model);
}

void kglCreateModel(KglDevice device, const KglModelCreateInfo* pInfo, KglModel* pOut) {
    KglModel model = malloc(sizeof(struct KglModel_T));
    const KglModelCreateInfo info = *pInfo;
    const KglBufferCreateInfo vertexBufferInfo = {
        .size = info.vertexDataSize,
        .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
            VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    const KglBufferCreateInfo indexBufferInfo = {
        .size = info.indexDataSize,
        .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
            VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    kglCreateBuffer(device, &vertexBufferInfo, &model->vertexBuffer);
    kglCreateBuffer(device, &indexBufferInfo, &model->indexBuffer);
    memcpy(model->vertexBuffer->allocationInfo.pMappedData, info.pVertexData, info.vertexDataSize);
    memcpy(model->indexBuffer->allocationInfo.pMappedData, info.pIndexData, info.indexDataSize);
    model->indexCount = info.indexCount;
    *pOut = model;
}

typedef struct KglRendererCreateInfo {
    uint32_t framesInFlight;
} KglRendererCreateInfo;

void kglDestroyRenderer(KglRenderer renderer) {
    KglDevice device = renderer->swapchain->device;
    for (int i = 0; i < renderer->framesInFlight; i++) {
        kglDestroyFrameResource(&renderer->frameResources[i]);
    }
    device->table.vkDestroyPipeline(device->handle, renderer->pipeline, NULL);
    device->table.vkDestroyPipelineLayout(device->handle, renderer->pipelineLayout, NULL);
    free(renderer);
}

void kglCreateRenderer(KglSwapchain swapchain, const KglRendererCreateInfo* pInfo, KglRenderer* pOut) {
    const KglRendererCreateInfo info = *pInfo;
    const KglDevice device = swapchain->device;
    KglRenderer renderer = calloc(1, sizeof(struct KglRenderer_T));
    if (!renderer) return;
    renderer->framesInFlight = info.framesInFlight;
    renderer->swapchain = swapchain;

    KglPhysicalDevice physicalDevice = device->physicalDevice;
    KglInstance instance = physicalDevice->instance;

    renderer->frameResources = malloc(info.framesInFlight * sizeof(KglFrameResource));
    for (uint32_t i = 0; i < info.framesInFlight; i++) {
        kglCreateFrameResource(device, &renderer->frameResources[i]);
    }
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
            .size = sizeof(PushConstants)
        }
    };
    const VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &device->descriptorSetLayout,
        .pushConstantRangeCount = sizeof(pushConstantRanges) / sizeof(pushConstantRanges[0]),
        .pPushConstantRanges = pushConstantRanges
    };
    device->table.vkCreatePipelineLayout(device->handle, &pipelineLayoutInfo, NULL, &renderer->pipelineLayout);
    const VkPipelineVertexInputStateCreateInfo vertexInputState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
    };
    const VkPipelineDepthStencilStateCreateInfo depthStencilState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL
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
        .pColorAttachmentFormats = &swapchain->info.imageFormat,
        .depthAttachmentFormat = swapchain->depthFormat
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
        .pDepthStencilState = &depthStencilState,
        .layout = renderer->pipelineLayout
    };
    device->table.vkCreateGraphicsPipelines(device->handle, NULL, 1, &pipelineInfo, NULL, &renderer->pipeline);

    *pOut = renderer;
    return;

cleanup:
    kglDestroyRenderer(renderer);
}

void kglBeginRendering(KglRenderer renderer) {
    const KglSwapchain swapchain = renderer->swapchain;
    const KglDevice device = swapchain->device;
    KglFrameResource frameResource = renderer->frameResources[renderer->frameIndex];
    VkCommandBuffer commandBuffer = frameResource.commandBuffer;
    device->table.vkWaitForFences(device->handle, 1, &frameResource.fence, VK_TRUE, UINT64_MAX);
    device->table.vkResetFences(device->handle, 1, &frameResource.fence);
    device->table.vkAcquireNextImageKHR(device->handle, swapchain->handle, UINT64_MAX, frameResource.semaphore, VK_NULL_HANDLE, &renderer->imageIndex);
    KglSwapchainResource swapchainResource = swapchain->resources[renderer->imageIndex];
    VkExtent2D windowExtent;
    dokGetWindowExtent(device->window, &windowExtent);
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
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .image = swapchainResource.image,
            .subresourceRange = (VkImageSubresourceRange) { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1 }
        },
        (VkImageMemoryBarrier2) {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .srcStageMask = VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
            .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
            .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            .newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
            .image = swapchainResource.depthImage->handle,
            .subresourceRange = (VkImageSubresourceRange) { .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, .levelCount = 1, .layerCount = 1 }
        }
    };
    const VkDependencyInfo writeBarrierInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = sizeof(outputBarriers) / sizeof(outputBarriers[0]),
        .pImageMemoryBarriers = outputBarriers
    };
    device->table.vkCmdPipelineBarrier2(commandBuffer, &writeBarrierInfo);
    const VkRenderingAttachmentInfo colorAttachmentInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = swapchainResource.view,
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = (VkClearValue) { .color = (VkClearColorValue) {.float32 = {0.0f, 0.0f, 0.0f, 1.0f}} }
    };
    const VkRenderingAttachmentInfo depthAttachmentInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = swapchainResource.depthImage->view,
        .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .clearValue = (VkClearValue) { .depthStencil = (VkClearDepthStencilValue) {.depth = 1.0f} }
    }; 
    const VkRenderingInfo renderingInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = (VkRect2D) {.extent = windowExtent},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentInfo,
        .pDepthAttachment = &depthAttachmentInfo,
    };
    device->table.vkCmdBeginRendering(commandBuffer, &renderingInfo);
    const VkViewport viewport = {
        .width = windowExtent.width,
        .height = windowExtent.height,
        .maxDepth = 1.0f
    };
    const VkRect2D scissor = { .extent = windowExtent };
    const VkDeviceSize vertexOffset = 0;
    device->table.vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    device->table.vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    device->table.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipeline);
    device->table.vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelineLayout, 0, 1, &device->textureDescriptorSet, 0, NULL);
}

typedef struct KglMeshInfo {
    uint32_t* indices;
    Vertex* vertices;
} KglMeshInfo;

typedef struct KglNode KglNode;
struct KglNode {
    //NULL IF NO MODEL
    KglModel model;
    KglBuffer positionBuffer;
    mat4x4 transform;
    //STB DYNAMIC ARRAY
    KglNode* children;
};

typedef struct KglMeshInfo {
    uint32_t* indices;
    Vertex* vertices;
} KglMeshInfo;

int get_attribute_index(const tg3_primitive* primitive, const char* name) {
    for (uint32_t i = 0; i < primitive->attributes_count; i++) {
        const tg3_str_int_pair* attribute = &primitive->attributes[i];
        if (attribute->key.len == (uint32_t)strlen(name) && strncmp(attribute->key.data, name, attribute->key.len) == 0) {
            return attribute->value;
        }
    }
    return -1;
}

const uint8_t* get_accessor_ptr(const tg3_model* model, const tg3_accessor* accesor, uint64_t* pOutStride) {
    const tg3_buffer_view* bufferView = &model->buffer_views[accesor->buffer_view];
    const tg3_buffer* buffer = &model->buffers[bufferView->buffer];
    uint32_t comps;
    uint64_t elem;
    switch (accesor->component_type) {
        case TG3_COMPONENT_TYPE_FLOAT:  elem = 4; break;
        case TG3_COMPONENT_TYPE_DOUBLE: elem = 8; break;
        case TG3_COMPONENT_TYPE_UNSIGNED_BYTE: elem = 1; break;
        case TG3_COMPONENT_TYPE_BYTE:   elem = 1; break;
        case TG3_COMPONENT_TYPE_UNSIGNED_SHORT: elem = 2; break;
        case TG3_COMPONENT_TYPE_SHORT:  elem = 2; break;
        case TG3_COMPONENT_TYPE_UNSIGNED_INT: elem = 4; break;
        case TG3_COMPONENT_TYPE_INT:    elem = 4; break;
        default: return NULL;
    }
    switch (accesor->type) {
        case TG3_TYPE_SCALAR: comps = 1; break;
        case TG3_TYPE_VEC2:   comps = 2; break;
        case TG3_TYPE_VEC3:   comps = 3; break;
        case TG3_TYPE_VEC4:   comps = 4; break;
        case TG3_TYPE_MAT4:   comps = 16; break;
        default:              return NULL;
    }
    uint64_t stride = bufferView->byte_stride ? bufferView->byte_stride : elem * comps;
    *pOutStride = stride;
    return buffer->data.data + bufferView->byte_offset + accesor->byte_offset;
}

void kglParseTg3Nodes(KglDevice device, const tg3_model* model, const tg3_node* node, KglMeshInfo* meshesInfo, KglNode* pOut) {
    pOut->model = NULL;
    if (node->mesh != -1) {
        size_t index = node->mesh;
        const KglModelCreateInfo modelInfo = {
            .pVertexData = meshesInfo[index].vertices,
            .vertexDataSize = arrlen(meshesInfo[index].vertices) * sizeof(meshesInfo[index].vertices[0]),
            .pIndexData = meshesInfo[index].indices,
            .indexDataSize = arrlen(meshesInfo[index].indices) * sizeof(meshesInfo[index].indices[0]),
            .indexCount = arrlen(meshesInfo[index].indices)
        };
        kglCreateModel(device, &modelInfo, &pOut->model);
    }
    const KglBufferCreateInfo positionBufferInfo = {
        .size = sizeof(mat4x4),
        .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
            VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
            VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .memoryUsage = VMA_MEMORY_USAGE_AUTO
    };
    kglCreateBuffer(device, &positionBufferInfo, &pOut->positionBuffer);
    for (uint32_t i = 0; i < node->children_count; i++) {
        KglNode child = { 0 };
        kglParseTg3Nodes(device, model, &model->nodes[node->children[i]], meshesInfo, &child);
        arrpush(pOut->children, child);
    }
    mat4x4_identity(pOut->transform);
    if (node->has_matrix) {
        for (uint32_t i = 0; i < 16; i++) {
            pOut->transform[i / 4][i % 4] = node->matrix[i];
        }
    } else {
        mat4x4_translate(pOut->transform, node->translation[0], node->translation[1], node->translation[2]);
    }
    printf("%f ", pOut->transform[0][0]);
}

void kglModelFromTg3(KglDevice device, const tg3_model* model, KglNode* pOut) {
    KglMeshInfo* meshesInfo = NULL;
    for (uint32_t meshIndex = 0; meshIndex < model->meshes_count; meshIndex++) {
        const tg3_mesh* mesh = &model->meshes[meshIndex];
        KglMeshInfo meshInfo = { 0 };
        for (uint32_t primitiveIndex = 0; primitiveIndex < mesh->primitives_count; primitiveIndex++) {
            const tg3_primitive* primitive = &mesh->primitives[primitiveIndex];
            int positionIndex = get_attribute_index(primitive, "POSITION");

            const tg3_accessor* indexAccesor = &model->accessors[primitive->indices];
            const tg3_accessor* positionAccesor = &model->accessors[positionIndex];
            uint64_t positionStride;
            uint64_t indexStride;
            const uint8_t* positionData = get_accessor_ptr(model, positionAccesor, &positionStride);
            const uint8_t* indexData = get_accessor_ptr(model, indexAccesor, &indexStride);
            for (uint32_t vertexIndex = 0; vertexIndex < positionAccesor->count; vertexIndex++) {
                const float* positions = (const float*)(positionData + vertexIndex * positionStride);
                Vertex vertex = { 0 };
                vertex.position[0] = positions[0];
                vertex.position[1] = positions[1];
                vertex.position[2] = positions[2];
                arrpush(meshInfo.vertices, vertex);
            }
            for (size_t indexIndex = 0; indexIndex < indexAccesor->count; indexIndex++) {
                uint32_t index = 0;
                switch (indexAccesor->component_type) {
                    case TG3_COMPONENT_TYPE_UNSIGNED_BYTE: index = ((const uint8_t*)indexData)[indexIndex * indexStride]; break;
                    case TG3_COMPONENT_TYPE_UNSIGNED_SHORT: index = ((const uint16_t*)indexData)[indexIndex * indexStride / 2]; break;
                    case TG3_COMPONENT_TYPE_UNSIGNED_INT: index = ((const uint32_t*)indexData)[indexIndex * indexStride / 4]; break;
                    default: break;
                }
                arrpush(meshInfo.indices, index);
            }
        }
        arrpush(meshesInfo, meshInfo);
    }
    const tg3_scene* scene = &model->scenes[model->default_scene];
    pOut->children = NULL;
    mat4x4_identity(pOut->transform);
    for (uint32_t i = 0; i < scene->nodes_count; i++) {
        KglNode child = { 0 };
        kglParseTg3Nodes(device, model, &model->nodes[scene->nodes[i]], meshesInfo, &child);
        arrpush(pOut->children, child);
    }
}

void kglDestroyNode(KglNode* pNode) {
    for (uint32_t i = 0; i < arrlen(pNode->children); i++) {
        kglDestroyNode(&pNode->children[i]);
    }
    if (pNode->model) {
        kglDestroyModel(pNode->model); 
    }
    if (pNode->positionBuffer) {
        kglDestroyBuffer(pNode->positionBuffer);
    }
}

typedef struct KglDrawInfo {
    KglNode node;
    KglTexture texture;
    mat4x4 view;
} KglDrawInfo;

void kglDraw(KglRenderer renderer, const KglDrawInfo* pInfo) {
    const KglSwapchain swapchain = renderer->swapchain;
    const KglDevice device = renderer->swapchain->device;
    const KglDrawInfo info = *pInfo;
    const KglNode node = info.node;
    if (node.model) {
        KglFrameResource frameResource = renderer->frameResources[renderer->frameIndex];
        VkCommandBuffer commandBuffer = frameResource.commandBuffer;
        FrameConstants frameConstants = { 0 };
        VkExtent2D windowExtent;
        dokGetWindowExtent(device->window, &windowExtent);
        mat4x4_perspective(frameConstants.projection, 0.8, (float)(windowExtent.width) / (float)(windowExtent.height), 0.1, 32.0f);
        mat4x4_dup(frameConstants.view, info.view);
        kglFrameResourceCopyUniform(&frameResource, &frameConstants);
        memcpy(node.positionBuffer->allocationInfo.pMappedData, &node.transform, sizeof(mat4x4));

        PushConstants pushConstants = {
            .frameConstants = frameResource.uniformBuffer->address,
            .vertices = node.model->vertexBuffer->address,
            .indices = node.model->indexBuffer->address,
            .position = node.positionBuffer->address
        };

        device->table.vkCmdPushConstants(commandBuffer, renderer->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);
        device->table.vkCmdDraw(commandBuffer, node.model->indexCount, 1, 0, 0);
    }
    for (uint32_t i = 0; i < arrlen(node.children); i++) {
        KglDrawInfo childInfo = *pInfo;
        childInfo.node = node.children[i];
        mat4x4_mul(childInfo.node.transform, node.transform, childInfo.node.transform);
        mat4x4 identity;
        mat4x4_identity(identity);
        kglDraw(renderer, &childInfo);
    }
}

void kglEndRendering(KglRenderer renderer) {
    const KglSwapchain swapchain = renderer->swapchain;
    const KglDevice device = swapchain->device;
    KglSwapchainResource swapchainResource = swapchain->resources[renderer->imageIndex];
    KglFrameResource frameResource = renderer->frameResources[renderer->frameIndex];
    VkCommandBuffer commandBuffer = frameResource.commandBuffer;
    device->table.vkCmdEndRendering(commandBuffer);
    const VkImageMemoryBarrier2 presentBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
        .dstAccessMask = VK_ACCESS_2_NONE,
        .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .image = swapchainResource.image,
        .subresourceRange = (VkImageSubresourceRange) { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1 }
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
        .semaphore = frameResource.semaphore,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    const VkCommandBufferSubmitInfo commandBufferSubmitInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = commandBuffer
    };
    const VkSemaphoreSubmitInfo signalSemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = swapchainResource.semaphore,
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
    device->table.vkQueueSubmit2(device->queue, 1, &submitInfo, frameResource.fence);
    const VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &swapchainResource.semaphore,
        .swapchainCount = 1,
        .pSwapchains = &swapchain->handle,
        .pImageIndices = &renderer->imageIndex
    };
    device->table.vkQueuePresentKHR(device->queue, &presentInfo);
    renderer->frameIndex = (renderer->frameIndex + 1) % renderer->framesInFlight;
}

typedef struct KglTextureCreateInfo {
    const void* pData;
    uint32_t channels;
    uint32_t height;
    uint32_t width;
    VkFormat format;
} KglTextureCreateInfo;

void kglDeviceAddTexture(
    KglDevice device, 
    const KglTextureCreateInfo* pInfo, 
    KglTexture* pOut
) {
    KglTextureInfo textureInfo = { 0 };
    const KglTextureCreateInfo info = *pInfo;
    const VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST
    };
    const KglImageCreateInfo imageInfo = {
        .format = info.format,
        .extent = (VkExtent3D) { .width = info.width, .height = info.height, .depth = 1 },
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
    };
    device->table.vkCreateSampler(device->handle, &samplerInfo, NULL, &textureInfo.descriptor.sampler);
    kglCreateImage(device, &imageInfo, &textureInfo.image);
    kglImagePasteData(textureInfo.image, info.width * info.height * info.channels, info.pData);
    textureInfo.descriptor.imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    textureInfo.descriptor.imageView = textureInfo.image->view;
    KglTexture view = malloc(sizeof(struct KglTexture_T));
    view->index = arrlen(device->textures);
    arrput(device->textures, textureInfo);
    *pOut = view;
}

void kglDeviceRemoveTexture(KglDevice device, KglTexture texture) {
    kglDestroyImage(device->textures[texture->index].image);
    device->table.vkDestroySampler(device->handle, device->textures[texture->index].descriptor.sampler, NULL);
}

void kglDeviceUpdateTextures(KglDevice device) {
    size_t textureCount = arrlen(device->textures);
    VkDescriptorImageInfo* descriptors = malloc(textureCount * sizeof(VkDescriptorImageInfo));
    for (uint32_t i = 0; i < textureCount; i++) {
        descriptors[i] = device->textures[i].descriptor;
    }
    const VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = device->textureDescriptorSet,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = textureCount,
        .pImageInfo = descriptors
    };
    device->table.vkUpdateDescriptorSets(device->handle, 1, &writeDescriptorSet, 0, NULL);
    free(descriptors);    
}

void destroyObserverCallback(DokWindow window, void* pUserData) {
    *((bool*)pUserData) = false;
}

void resizeObserverCallback(DokWindow window, uint32_t width, uint32_t height, void* pUserData) {
    KglSwapchain swapchain = *((KglSwapchain*)pUserData);
    KglDevice device = swapchain->device;
    device->table.vkDeviceWaitIdle(device->handle);
    kglSwapchainUpdate(swapchain);
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
    KglPhysicalDevice physicalDevice = physicalDevices[1];
    free(physicalDevices);
    const DokWindowCreateInfo windowInfo = {
        .pTitle = "Kgl-Test",
        .height = 420,
        .width = 720
    };
    DokWindow window;
    dokCreateWindow(&windowInfo, &window);
    const KglDeviceCreateInfo deviceInfo = {
        .window = window,
        .maxTextures = 100
    };
    KglDevice device;
    kglCreateDevice(physicalDevice, &deviceInfo, &device);
    const KglSwapchainCreateInfo swapchainInfo = {
        .colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .format = VK_FORMAT_B8G8R8A8_SRGB,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR
    };
    KglSwapchain swapchain;
    kglCreateSwapchain(device, &swapchainInfo, &swapchain);
    const KglRendererCreateInfo rendererInfo = {
        .framesInFlight = 2
    };
    KglRenderer renderer;
    kglCreateRenderer(swapchain, &rendererInfo, &renderer);
    const DokWindowResizeObserverCreateInfo resizeObserverInfo = {
        .pUserData = &swapchain,
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

    tg3_model glb_model;
    tg3_parse_glb(&glb_model, NULL, glb, sizeof(glb), NULL, 0, 0);

    KglNode node = { 0 };
    kglModelFromTg3(device, &glb_model, &node);
    tg3_model_free(&glb_model);

    double pepe = 0;
    while (exists) {
        kglBeginRendering(renderer);
        KglDrawInfo drawInfo = {
            .node = node
        };
        mat4x4_translate(drawInfo.view, 0, 0, 4);
        mat4x4_rotate_Y(drawInfo.view, drawInfo.view, 3.14);
        mat4x4_rotate_X(drawInfo.view, drawInfo.view, -pepe);
        kglDraw(renderer, &drawInfo);
        kglEndRendering(renderer);
        dokPollWindow(window);
        pepe += 0.005;
    }
    device->table.vkDeviceWaitIdle(device->handle);
    kglDestroyNode(&node);
    kglDestroyRenderer(renderer);
    kglDestroySwapchain(swapchain);
    kglDestroyDevice(device);
    kglDestroyInstance(instance);
}