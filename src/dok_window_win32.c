#define VK_USE_PLATFORM_WIN32_KHR
#include <dok/dok_window.h>
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

struct DokWindow_T {
    HWND handle;
    HINSTANCE hInstance;
    bool exists;
};

const char* CLASS_NAME = "Dok Window";
const char* PROP_KEY = "Dok";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    DokWindow window = GetProp(hwnd, PROP_KEY);

    switch (uMsg) {
    case WM_DESTROY:
        window->exists = false;
        PostQuitMessage(0);
        return 0;

    case WM_QUIT:
        window->exists = false;

    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void dokCreateWindow(const char* title, int height, int width, DokWindow* pOut) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS window_class = {0};

    if (!GetClassInfo(hInstance, CLASS_NAME, &window_class)) {
        window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        window_class.lpszClassName = CLASS_NAME;
        window_class.lpfnWndProc = WindowProc;
        window_class.hInstance = hInstance;

        RegisterClass(&window_class);
    }

    HWND window_handle = CreateWindowEx(
        0,
        CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    DokWindow window = (DokWindow) malloc(sizeof(struct DokWindow_T));
    window->hInstance = hInstance;
    window->handle = window_handle;
    window->exists = true;
    
    SetProp(window_handle, PROP_KEY, window);
    ShowWindow(window_handle, SW_SHOW);

    *pOut = window;
}

const char* dokGetPlatformSurfaceExtension() {
    return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}

VkBool32 dokGetPhysicalDevicePresentationSupport(DokInstanceTable* instanceTable, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) {
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR presentationSupport = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)
        instanceTable->vkGetInstanceProcAddr(instanceTable->instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR");

    if (!presentationSupport) {
        return VK_FALSE;
    }

    return presentationSupport(physicalDevice, queueFamilyIndex);
}

VkResult dokCreateWindowSurface(DokInstanceTable* instanceTable, DokWindow window, VkSurfaceKHR* pSurface) {
    VkWin32SurfaceCreateInfoKHR info = { 0 };
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hinstance = window->hInstance;
    info.hwnd = window->handle;

    PFN_vkCreateWin32SurfaceKHR createSurface = (PFN_vkCreateWin32SurfaceKHR) 
        instanceTable->vkGetInstanceProcAddr(instanceTable->instance, "vkCreateWin32SurfaceKHR");

    if (!createSurface) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    return createSurface(instanceTable->instance, &info, NULL, pSurface);
}

bool dokWindowExists(DokWindow window) {
    return window->exists;
}

void dokFreeWindow(DokWindow window) {
    if (window->handle) {
        RemovePropW(window->handle, L"Dok");
        DestroyWindow(window->handle);
        window->handle = NULL;
    }

    free(window);
}

void dokPollWindow(DokWindow window) {
    MSG msg = { };
    while (PeekMessage(&msg, window->handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}