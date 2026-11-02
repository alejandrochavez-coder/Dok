#include <dok/dok_core.h>
#include <stdbool.h>
#include <stdio.h>

void observerCallback(DokWindow window, void* pUserData) {
    printf("PEPE");
    *((bool*)pUserData) = false;
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

    while (exists) {
        dokPollWindow(window);
    }

    dokDestroyDestroyObserver(window, destroyObserver);

    return 0;
}