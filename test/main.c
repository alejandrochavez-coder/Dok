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

    while (dokWindowExists(window)) {
        dokPollWindow(window);
    }

    dokUnloadVulkan(loader);
    dokFreeWindow(window);
    return 0;
}