#include <dok/dok_window.h>
#include <windows.h>

struct DokWindow_T {
    HWND handle;
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
        window_class.lpfnWndProc = WindowProc;
        window_class.hInstance = hInstance;
        window_class.lpszClassName = CLASS_NAME;
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW);

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
        window_class.hInstance,
        NULL
    );

    DokWindow window = (DokWindow) malloc(sizeof(struct DokWindow_T));
    window->handle = window_handle;
    window->exists = true;
    
    ShowWindow(window_handle, SW_SHOW);
    SetProp(window_handle, PROP_KEY, &window);

    *pOut = window;
}

bool dokWindowExists(DokWindow window) {
    return window->exists;
}

void dokPollWindow(DokWindow window) {
    MSG msg = { };
    while (PeekMessage(&msg, window->handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}