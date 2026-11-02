#include <dok/dok_core.h>
#include <stdbool.h>
#include <stdio.h>

void observerCallback(DokWindow window, void* pUserData) {
    printf("PEPE");
    *((bool*)pUserData) = false;
}

void resizeCallback(DokWindow, uint32_t width, uint32_t height, void* pUserData) {
    printf("%d, %d\n", width, height);
}

int main() {
    DokWindowCreateInfo windowInfo = { 0 };
    windowInfo.pTitle = "PEPE";
    windowInfo.height = 420;
    windowInfo.width = 720;

    DokWindow window;
    dokCreateWindow(&windowInfo, &window);

    bool exists = true;

    DokWindowDestroyObserverCreateInfo observerInfo = { 0 };
    observerInfo.pfnCallback = observerCallback;
    observerInfo.pUserData = &exists;

    DokWindowDestroyObserver destroyObserver;
    dokCreateDestroyObserver(window, &observerInfo, &destroyObserver);

    DokWindowResizeObserverCreateInfo resizeInfo = { 0 };
    resizeInfo.pfnCallback = resizeCallback;
    resizeInfo.pUserData = "PEPE";

    DokWindowResizeObserver resizeObserver;
    dokCreateResizeObserver(window, &resizeInfo, &resizeObserver);

    while (exists) {
        dokPollWindow(window);
    }

    dokDestroyResizeObserver(window, resizeObserver);
    dokDestroyDestroyObserver(window, destroyObserver);

    return 0;
}