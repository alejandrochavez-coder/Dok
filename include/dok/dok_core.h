#pragma once

#include <vulkan/vulkan.h>

typedef struct DokWindowDestroyObserver_T* DokWindowDestroyObserver;
typedef struct DokWindowResizeObserver_T* DokWindowResizeObserver;
typedef struct DokVkLoader_T* DokVkLoader;
typedef struct DokWindow_T* DokWindow;

typedef enum DokResult {
    DOK_SUCCESS = 0,
    DOK_FAIL_DLL_LOAD = 1,
    DOK_ERROR_INITIALIZATION_FAILED = 2,
    DOK_WIN32_FAILED_CLASS_REGISTRY = 3,
    DOK_OUT_OF_HOST_MEMORY = 4,
    DOK_WIN32_FAILED_WINDOW_CREATION = 5
} DokResult;

typedef void (*PFN_dokWindowResizeCallback)(
    DokWindow   window,
    uint32_t    width,
    uint32_t    height,
    void*       pUserData);

typedef struct DokWindowResizeObserverCreateInfo {
    PFN_dokWindowResizeCallback pfnCallback;
    void*                       pUserData;
} DokWindowResizeObserverCreateInfo;

typedef void (*PFN_dokWindowDestroyCallback)(
    DokWindow   window,
    void*       pUserData);

typedef struct DokWindowDestroyObserverCreateInfo {
    PFN_dokWindowDestroyCallback    pfnCallback;
    void*                           pUserData;
} DokWindowDestroyObserverCreateInfo;

typedef struct DokWindowCreateInfo {
    const char*         pTitle;
    uint32_t            height;
    uint32_t            width;
} DokWindowCreateInfo;

typedef struct DokVulkanInfo {
    PFN_vkGetInstanceProcAddr   instanceProcAddr;
    VkInstance                  instance;
} DokVulkanInfo;

void dokGetTime(uint64_t* pOut);

void dokGetTimeFrequency(uint64_t* pOut);

DokResult dokLoadVulkan(DokVkLoader* pOut);

PFN_vkVoidFunction dokGetFunction(DokVkLoader loader, const char* name);

void dokUnloadVulkan(DokVkLoader loader);

DokResult dokCreateWindow(
    const DokWindowCreateInfo*    pCreateInfo, 
    DokWindow*              pOut);

const char* dokGetPlatformSurfaceExtension();

VkBool32 dokGetPhysicalDevicePresentationSupport(
    const DokVulkanInfo*      pVulkanInfo,
    VkPhysicalDevice    physicalDevice,
    uint32_t            queueFamilyIndex);

VkResult dokCreateWindowSurface(
    const DokVulkanInfo*  pVulkanInfo,
    DokWindow       window,
    VkSurfaceKHR*   pSurface);

DokResult dokGetWindowExtent(
    DokWindow   window,
    VkExtent2D* pOut);

void dokFreeWindow(DokWindow window);

DokResult dokCreateDestroyObserver(
    DokWindow                           window, 
    DokWindowDestroyObserverCreateInfo* pCreateInfo, 
    DokWindowDestroyObserver*           pOut);

void dokDestroyDestroyObserver(
    DokWindow                   window, 
    DokWindowDestroyObserver    observer);

DokResult dokCreateResizeObserver(
    DokWindow                           window,
    DokWindowResizeObserverCreateInfo*  pCreateInfo,
    DokWindowResizeObserver*            pOut);

void dokDestroyResizeObserver(
    DokWindow               window,
    DokWindowResizeObserver observer);

void dokPollWindow(DokWindow window);