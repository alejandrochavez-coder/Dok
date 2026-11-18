#pragma once

#include <vulkan/vulkan.h>

typedef struct DokWindowMouseMotionObserver_T* DokWindowMouseMotionObserver;
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

typedef void (*PFN_dokWindowMouseMotionCallback)(
    DokWindow window,
    double x,
    double y,
    void* pUserData
);

typedef struct DokWindowMouseMotionObserverCreateInfo {
    PFN_dokWindowMouseMotionCallback    pfnCallback;
    void*                               pUserData;
} DokWindowMouseMotionObserverCreateInfo;

typedef struct DokWindowCreateInfo {
    const char*         pTitle;
    uint32_t            height;
    uint32_t            width;
} DokWindowCreateInfo;

typedef struct DokVulkanInfo {
    PFN_vkGetInstanceProcAddr   instanceProcAddr;
    VkInstance                  instance;
} DokVulkanInfo;

typedef enum DokKey {
    DOK_KEY_A = 0,
    DOK_KEY_S = 1,
    DOK_KEY_D = 2,
    DOK_KEY_W = 3,
    DOK_KEY_SPACEBAR = 4
} DokKey;

typedef enum DokKeyState {
    DOK_KEY_STATE_UNPRESSED = 0,
    DOK_KEY_STATE_PRESSED = 1,
    DOK_KEY_STATE_JUST_PRESSED = 3
} DokKeyState;

typedef enum DokCursorState {
    DOK_CURSOR_STATE_HIDDEN = 0
} DokCursorState;

void dokGetTime(uint64_t* pOut);

void dokGetTimeFrequency(uint64_t* pOut);

DokResult dokLoadVulkan(DokVkLoader* pOut);

PFN_vkVoidFunction dokGetFunction(DokVkLoader loader, const char* name);

void dokUnloadVulkan(DokVkLoader loader);

DokResult dokCreateWindow(
    const DokWindowCreateInfo*  pCreateInfo, 
    DokWindow*                  pOut);

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
    const DokWindowDestroyObserverCreateInfo* pCreateInfo, 
    DokWindowDestroyObserver*           pOut);

void dokDestroyDestroyObserver(
    DokWindow                   window, 
    DokWindowDestroyObserver    observer);

DokResult dokCreateResizeObserver(
    DokWindow                           window,
    const DokWindowResizeObserverCreateInfo*  pCreateInfo,
    DokWindowResizeObserver*            pOut);

void dokDestroyResizeObserver(
    DokWindow               window,
    DokWindowResizeObserver observer);

DokKeyState dokGetKeyState(
    DokWindow window,
    DokKey key
);

void dokSetCursorState(
    DokWindow window,
    DokCursorState state
);

void dokWindowCreateMouseMotionObserver(DokWindow window, const DokWindowMouseMotionObserverCreateInfo* pCreateInfo, DokWindowMouseMotionObserver* pOut);
void dokPollWindow(DokWindow window);