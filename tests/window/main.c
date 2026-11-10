#include <dok/dok_core.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

alignas(uint32_t) const uint8_t spirvShader[] = {
    #embed "demo.spv"
};

const unsigned char data[] = {
    #embed "test_image.jpg" limit(400000)
};

void observerCallback(DokWindow window, void* pUserData) {
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
    
    uint64_t timeFrequency;
    uint64_t time;


    dokGetTimeFrequency(&timeFrequency);
    dokGetTime(&time);

    double scale = 1.0 / timeFrequency;
    double elapsedTime = 0;

    while (exists) {

        uint64_t oldTime = time;
        dokGetTime(&time);

        double deltaTime = (time - oldTime) * scale;
        elapsedTime += deltaTime;

        printf("%f\n" , elapsedTime);

        dokPollWindow(window);
    }

    dokDestroyResizeObserver(window, resizeObserver);
    dokDestroyDestroyObserver(window, destroyObserver);

    return 0;
}