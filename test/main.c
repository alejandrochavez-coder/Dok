#define DOK_VULKAN_TABLES_IMPLEMENTATION
#include <dok/dok_vulkan_tables.h>
#include <dok/dok_vulkan_loader.h>
#include <dok/dok_window.h>
#include <stdio.h>

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

    DokWindow window;
    printf("PE");
    dokCreateWindow("PEPE", 930, 200, &window);
    while (dokWindowExists(window)) {
        dokPollWindow(window);
    }
    return 0;
}