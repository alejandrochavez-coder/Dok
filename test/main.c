#define DOK_VULKAN_TABLES_IMPLEMENTATION
#include <dok/dok_vulkan_tables.h>
#include <dok/dok_vulkan_loader.h>
#include <dok/dok_window.h>
#include <stdlib.h>
#include <stdio.h>

VkBool32 dokGetDeviceQueueFamily(
    DokInstanceTable*   instanceTable,
    VkPhysicalDevice    physicalDevice,
    uint32_t            requirements,
    uint32_t*           pOut) {

    uint32_t queueFamilyCount = 0;
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*) malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++) {
        if (dokGetPhysicalDevicePresentationSupport(instanceTable, physicalDevice, i) == VK_FALSE) {
            continue;
        }

        if (queueFamilies[i].queueFlags & requirements) {
            *pOut = i;
            return VK_TRUE;
        }
    }

    return VK_FALSE;
}

int main() {
    DokVkLoader loader;
    dokLoadVulkan(&loader);

    PFN_vkGetInstanceProcAddr instanceProcAddr = (PFN_vkGetInstanceProcAddr) dokGetFunction(loader, "vkGetInstanceProcAddr");

    DokGlobalTable globalTable = { 0 };
    dokLoadGlobalTable(instanceProcAddr, &globalTable);

    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Dok";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    const char* instanceExtensions[] = { VK_KHR_SURFACE_EXTENSION_NAME, dokGetPlatformSurfaceExtension() };

    VkInstanceCreateInfo instanceInfo = { 0 };
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = 2;
    instanceInfo.ppEnabledExtensionNames = instanceExtensions;

    VkInstance instance;
    globalTable.vkCreateInstance(&instanceInfo, NULL, &instance);

    DokInstanceTable instanceTable = { 0 };
    dokLoadInstanceTable(instanceProcAddr, instance, &instanceTable);

    DokWindow window;
    dokCreateWindow("PEPE", 930, 200, &window);

    VkSurfaceKHR surface;
    dokCreateWindowSurface(&instanceTable, window, &surface);

    uint32_t deviceCount = 0;
    instanceTable.vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*) malloc(deviceCount * sizeof(VkPhysicalDevice));
    instanceTable.vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices);

    VkPhysicalDevice physicalDevice = physicalDevices[0];
    free(physicalDevices);

    uint32_t queueFamily = 0;
    dokGetDeviceQueueFamily(&instanceTable, physicalDevice, VK_QUEUE_GRAPHICS_BIT, &queueFamily);

    float priority = 1.0;

    VkDeviceQueueCreateInfo queueInfo = { 0 };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamily;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDeviceVulkan12Features enabledVk12Features = { 0 };
    enabledVk12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    enabledVk12Features.descriptorIndexing = VK_TRUE;
    enabledVk12Features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    enabledVk12Features.descriptorBindingVariableDescriptorCount = VK_TRUE;
    enabledVk12Features.runtimeDescriptorArray = VK_TRUE;
    enabledVk12Features.bufferDeviceAddress = VK_TRUE;

    VkPhysicalDeviceVulkan13Features enabledVk13Features = { 0 };
    enabledVk13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    enabledVk13Features.pNext = &enabledVk12Features;
    enabledVk13Features.synchronization2 = VK_TRUE;
    enabledVk13Features.dynamicRendering = VK_TRUE;

    VkPhysicalDeviceFeatures enabledVk10Features = { 0 };
    enabledVk10Features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceInfo = { 0 };
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = &enabledVk13Features;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = deviceExtensions;
    deviceInfo.pEnabledFeatures = &enabledVk10Features;

    VkDevice device;
    instanceTable.vkCreateDevice(physicalDevice, &deviceInfo, NULL, &device);

    DokDeviceTable deviceTable = { 0 };
    dokLoadDeviceTable(instanceTable.vkGetDeviceProcAddr, device, &deviceTable);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    instanceTable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

    VkSwapchainCreateInfoKHR swapchainInfo = { 0 };
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = surface;
    swapchainInfo.minImageCount = surfaceCapabilities.minImageCount,
    swapchainInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
    swapchainInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchainInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

    VkSwapchainKHR swapchain;
    deviceTable.vkCreateSwapchainKHR(device, &swapchainInfo, NULL, &swapchain);

    uint32_t imageCount;
    deviceTable.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL);

    VkImage* images = (VkImage*) malloc(imageCount * sizeof(VkImage));
    deviceTable.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images);

    VkFormat formats[] = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;

    for (uint32_t i = 0; i < 2; i++) {
        VkFormatProperties2 formatProperties = { 0 };
        formatProperties.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
        instanceTable.vkGetPhysicalDeviceFormatProperties2(physicalDevice, formats[i], &formatProperties);

        if (formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            depthFormat = formats[i];
            break;
        }
    }

    VkImageSubresourceRange subresourceRange = { 0 };
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    for (uint32_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo viewInfo = { 0 };
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = depthFormat;
        viewInfo.subresourceRange = subresourceRange;

        VkImageView imageView;
        deviceTable.vkCreateImageView(device, &viewInfo, NULL, &imageView);
    }

    VkCommandPoolCreateInfo commandPoolInfo = { 0 };
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = queueFamily;

    VkCommandPool commandPool;
    deviceTable.vkCreateCommandPool(device, &commandPoolInfo, NULL, &commandPool);


    // uint32_t windowWidth;
    // uint32_t windowHeight;

    // dokGetWindowSize(window, &windowWidth, &windowHeight);
    // VkExtent3D imageExtent = { 0 };
    // imageExtent.width = windowWidth;
    // imageExtent.height = windowHeight;
    // imageExtent.depth = 1;

    // VkImageCreateInfo imageInfo = { 0 };
    // imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    // imageInfo.imageType = VK_IMAGE_TYPE_2D;
    // imageInfo.format = depthFormat;
    // imageInfo.extent = imageExtent;
    // imageInfo.mipLevels = 1;
    // imageInfo.arrayLayers = 1;
    // imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    // imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    // imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    // imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    //

    dokUnloadVulkan(loader);
    dokFreeWindow(window);
    return 0;
}