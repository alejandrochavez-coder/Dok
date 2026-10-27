#pragma once

// #include <vulkan/vulkan.h>
#include <dok/dok_vulkan_tables.h>
#include <stdbool.h>

typedef struct DokWindow_T* DokWindow;


void dokCreateWindow(const char* title, int width, int height, DokWindow* pOut);

const char* dokGetPlatformSurfaceExtension();

VkBool32 dokGetPhysicalDevicePresentationSupport(
    DokInstanceTable*   instanceTable,
    VkPhysicalDevice    physicalDevice,
    uint32_t            queueFamilyIndex);

VkResult dokCreateWindowSurface(
    DokInstanceTable*   instanceTable,
    DokWindow           window,
    VkSurfaceKHR*       pSurface);

void dokGetWindowSize(
    DokWindow   window,
    uint32_t*   widthOut, 
    uint32_t*   heightOut);

bool dokWindowExists(DokWindow window);

void dokFreeWindow(DokWindow window);

void dokPollWindow(DokWindow window);