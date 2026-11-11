#define VK_USE_PLATFORM_WIN32_KHR
#include <dok/dok_vector.h>
#include <dok/dok_core.h>
#include <windows.h>

const char* CLASS_NAME = "Dok Window";
const char* PROP_KEY = "Dok";

struct DokWindow_T {
    HWND hwnd;
    HINSTANCE hinstance;
    DokVector destroyObservers;
    DokVector resizeObservers;
};

struct DokWindowResizeObserver_T {
    PFN_dokWindowResizeCallback pfnCallback;
    void* pUserData;
    size_t index;
};

struct DokWindowDestroyObserver_T {
    PFN_dokWindowDestroyCallback pfnCallback;
    void* pUserData;
    size_t index;
};

struct DokVkLoader_T {
    HMODULE dll;
};

void dokGetTime(uint64_t* pOut) {
    QueryPerformanceCounter((LARGE_INTEGER*) pOut);
}

void dokGetTimeFrequency(uint64_t* pOut) {
    QueryPerformanceFrequency((LARGE_INTEGER*) pOut);
}

DokResult dokLoadVulkan(DokVkLoader* pOut) {
    HMODULE dll = LoadLibrary("vulkan-1.dll");
    if (!dll) {
        return DOK_FAIL_DLL_LOAD;
    }

    DokVkLoader loader = (DokVkLoader) malloc(sizeof(struct DokVkLoader_T));
    loader->dll = dll;

    *pOut = loader;
    return DOK_SUCCESS;
}

PFN_vkVoidFunction dokGetFunction(DokVkLoader loader, const char* name) {
    return (PFN_vkVoidFunction)GetProcAddress(loader->dll, name);
}

void dokUnloadVulkan(DokVkLoader loader) {
    FreeLibrary(loader->dll);
    free(loader);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    DokWindow window = GetProp(hwnd, PROP_KEY);
    if (!window) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    switch (uMsg) {

    case WM_CLOSE:
        for (int i = 0; i < window->destroyObservers.length; i++) {
            DokWindowDestroyObserver destroyObserver = dokVectorCast(DokWindowDestroyObserver, window->destroyObservers.data)[i];
            destroyObserver->pfnCallback(window, destroyObserver->pUserData);
        }
        DestroyWindow(hwnd);
        break;

    case WM_SIZE:
        uint32_t width = (uint32_t)LOWORD(lParam);
        uint32_t height =  (uint32_t)HIWORD(lParam);
        for (int i = 0; i < window->resizeObservers.length; i++) {
            DokWindowResizeObserver resizeObserver = dokVectorCast(DokWindowResizeObserver, window->resizeObservers.data)[i];
            resizeObserver->pfnCallback(window, width, height, resizeObserver->pUserData);
        } 
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

DokResult dokCreateWindow(const DokWindowCreateInfo* pCreateInfo, DokWindow* pOut) {
    VkResult result = DOK_ERROR_INITIALIZATION_FAILED;
    if (!pCreateInfo || !pOut) {
        goto exit;
    }

    DokWindow window = (DokWindow) malloc(sizeof(struct DokWindow_T));
    if (!window) {
        result = DOK_OUT_OF_HOST_MEMORY;
        goto exit;
    }

    window->hinstance = GetModuleHandle(NULL);
    WNDCLASS windowClass = {0};

    if (!GetClassInfo(window->hinstance, CLASS_NAME, &windowClass)) {
        windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = CLASS_NAME;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = window->hinstance;

        if (RegisterClass(&windowClass) == 0) {
            result = DOK_WIN32_FAILED_CLASS_REGISTRY;
            goto cleanupWindow;
        }
    }

    DokWindowCreateInfo createInfo = *pCreateInfo;
    window->hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        createInfo.pTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        createInfo.width, createInfo.height,
        NULL,
        NULL,
        window->hinstance,
        NULL
    );

    if (!window->hwnd) {
        result = DOK_WIN32_FAILED_WINDOW_CREATION;
        goto cleanupWin32;
    }
    
    SetProp(window->hwnd, PROP_KEY, window);
    dokCreateVector(DokWindowDestroyObserver, &window->destroyObservers);
    dokCreateVector(DokWindowResizeObserver, &window->resizeObservers);
    ShowWindow(window->hwnd, SW_SHOW);

    *pOut = window;
    return DOK_SUCCESS;

cleanupWin32:
    UnregisterClass(CLASS_NAME, window->hinstance);
cleanupWindow:
    free(window);
exit:
    return result;
}

const char* dokGetPlatformSurfaceExtension() {
    return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}

VkBool32 dokGetPhysicalDevicePresentationSupport(const DokVulkanInfo* pVulkanInfo, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) {
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR presentationSupport = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)
        pVulkanInfo->instanceProcAddr(pVulkanInfo->instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR");

    if (!presentationSupport) {
        return VK_FALSE;
    }

    return presentationSupport(physicalDevice, queueFamilyIndex);
}

VkResult dokCreateWindowSurface(const DokVulkanInfo* pVulkanInfo, DokWindow window, VkSurfaceKHR* pSurface) {
    VkWin32SurfaceCreateInfoKHR info = { 0 };
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hinstance = window->hinstance;
    info.hwnd = window->hwnd;

    PFN_vkCreateWin32SurfaceKHR createSurface = (PFN_vkCreateWin32SurfaceKHR) 
        pVulkanInfo->instanceProcAddr(pVulkanInfo->instance, "vkCreateWin32SurfaceKHR");

    if (!createSurface) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    return createSurface(pVulkanInfo->instance, &info, NULL, pSurface);
}

DokResult dokGetWindowExtent(DokWindow window, VkExtent2D* pOut) {
    if (!pOut) {
        return DOK_ERROR_INITIALIZATION_FAILED;
    }

    RECT size;
    GetClientRect(window->hwnd, &size);

    pOut->height = (uint32_t)size.bottom;
    pOut->width = (uint32_t)size.right;

    return DOK_SUCCESS;
}

void dokFreeWindow(DokWindow window) {
    if (window->hwnd) {
        RemovePropW(window->hwnd, L"Dok");
        DestroyWindow(window->hwnd);
    }

    if (window->hinstance) {
        UnregisterClass(CLASS_NAME, window->hinstance);
    }

    free(window);
}

DokResult dokCreateDestroyObserver(DokWindow window, const DokWindowDestroyObserverCreateInfo* pCreateInfo, DokWindowDestroyObserver* pOut) {
    DokWindowDestroyObserver observer = (DokWindowDestroyObserver) malloc(sizeof(struct DokWindowDestroyObserver_T));
    observer->index = window->destroyObservers.length;
    observer->pfnCallback = pCreateInfo->pfnCallback;
    observer->pUserData = pCreateInfo->pUserData;

    dokVectorPush(DokWindowDestroyObserver, window->destroyObservers, observer);
    *pOut = observer;
    return DOK_SUCCESS;
}

void dokDestroyDestroyObserver(DokWindow window, DokWindowDestroyObserver observer) {
    dokVectorRemove(DokWindowDestroyObserver, window->destroyObservers, observer->index);
    free(observer);
}

DokResult dokCreateResizeObserver(DokWindow window, DokWindowResizeObserverCreateInfo* pCreateInfo, DokWindowResizeObserver* pOut) {
    DokWindowResizeObserver observer = (DokWindowResizeObserver) malloc(sizeof(struct DokWindowResizeObserver_T));
    observer->index = window->resizeObservers.length;
    observer->pfnCallback = pCreateInfo->pfnCallback;
    observer->pUserData = pCreateInfo->pUserData;

    dokVectorPush(DokWindowResizeObserver, window->resizeObservers, observer);
    *pOut = observer;
    return DOK_SUCCESS;
}

void dokDestroyResizeObserver(DokWindow window, DokWindowResizeObserver observer) {
    dokVectorRemove(DokWindowDestroyObserver, window->resizeObservers, observer->index);
    free(observer);
}

void dokPollWindow(DokWindow window) {
    MSG msg = { 0 };
    while (PeekMessage(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}