#include <dok/dok_window.h>
#include <windows.h>

struct DokWindow_T {
    HWND handle;
};

const char* CLASS_NAME = "Dok Window";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    // case WM_PAINT: {
    //         PAINTSTRUCT ps;
    //         HDC hdc = BeginPaint(hwnd, &ps);

    //         FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

    //         EndPaint(hwnd, &ps);
    //     }
    //     return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void dokCreateWindow(const char* title, int height, int width, DokWindow* pOut) {

    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS window_class = {0};    

    if (!GetClassInfo(hInstance, CLASS_NAME, &window_class)) {
        window_class.lpfnWndProc = WindowProc;
        window_class.hInstance = hInstance;
        window_class.lpszClassName = CLASS_NAME;

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

    ShowWindow(window_handle, SW_SHOW);

    DokWindow window = (DokWindow) malloc(sizeof(struct DokWindow_T));
    *pOut = window;

}

void dokPollWindow(DokWindow window) {
    MSG msg = { };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}