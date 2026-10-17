#define DOK_VULKAN_TABLES_IMPLEMENTATION
#include <dok/dok_vulkan_tables.h>
#include <dok/dok_vulkan_loader.h>
#include <dok/dok_window.h>
#include <stdlib.h>
#include <stdio.h>

VkResult dok_get_device_queue_family(
    DokInstanceTable*   instanceTable,
    VkPhysicalDevice    device,
    uint32_t            requirements,
    uint32_t*           pOut) {

    uint32_t queueFamilyCount = 0;
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*) malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & requirements) {
            *pOut = i;
            return VK_SUCCESS;
        }
    }

    return VK_ERROR_INITIALIZATION_FAILED;
}

int main() {
    DokVkLoader loader = dokLoadVulkan();
    if (!loader) {
        return 1;
    }

    PFN_vkGetInstanceProcAddr instanceProcAddr = (PFN_vkGetInstanceProcAddr) dokGetFunction(loader, "vkGetInstanceProcAddr");

    DokGlobalTable globalTable = { 0 };
    dokLoadGlobalTable(instanceProcAddr, &globalTable);

    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Dok";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceInfo = { 0 };
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;

    VkInstance instance;
    if (globalTable.vkCreateInstance(&instanceInfo, NULL, &instance) != VK_SUCCESS) {
        dokUnloadVulkan(loader);
        return 1;
    }

    DokInstanceTable instanceTable = { 0 };
    dokLoadInstanceTable(globalTable.vkGetInstanceProcAddr, instance, &instanceTable);

    uint32_t deviceCount = 0;
    instanceTable.vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    VkPhysicalDevice* devices = (VkPhysicalDevice*) malloc(deviceCount * sizeof(VkPhysicalDevice));
    instanceTable.vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    VkPhysicalDevice device = devices[0];
    free(devices);

    uint32_t queueFamily = 0;
    dok_get_device_queue_family(&instanceTable, device, VK_QUEUE_GRAPHICS_BIT, &queueFamily);

    float priority = 1.0;

    VkDeviceQueueCreateInfo queueInfo = { 0 };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamily;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    DokWindow window;
    dokCreateWindow("PEPE", 930, 200, &window);

    while (dokWindowExists(window)) {
        dokPollWindow(window);
    }

    dokUnloadVulkan(loader);
    dokFreeWindow(window);
    return 0;
}