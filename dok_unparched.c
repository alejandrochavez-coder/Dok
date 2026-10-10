#include "dok.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

struct DokInstance_T {
    PFN_vkGetInstanceProcAddr instanceProcAddr;
#if defined(_WIN32)
    HMODULE module;
#else
    void* module;
#endif
};

DokResult dokCreateInstance(DokInstance* pOut) {
    struct DokInstance_T* ptrInstance = malloc(sizeof(struct DokInstance_T));
    if (!ptrInstance) {
        return DOK_OUT_OF_MEMORY;
    }
#ifdef _WIN32
    ptrInstance->module = LoadLibrary("vulkan-1.dll");
    if (!ptrInstance->module) {
        return DOK_VULKAN_UNSUPPORTED;
    }
    ptrInstance->instanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(ptrInstance->module, "vkGetInstanceProcAddr");
#endif
    *pOut = ptrInstance;
    return DOK_SUCCESS;
}

void dokDestroyInstance(DokInstance instance) {
#ifdef _WIN32
    FreeLibrary(instance->module);
#endif
    free(instance);
}

void dokLoadGlobalTable(DokInstance instance, DokGlobalTable* pOut) {
    PFN_vkGetInstanceProcAddr instanceProcAddr = instance->instanceProcAddr;

//  pOut->{Global} = instanceProcAddr(NULL, "{Global}");

}

void dokLoadInstanceTable(VkInstance instance, DokGlobalTable* pFunctions, DokInstanceTable* pOut) {
    PFN_vkGetInstanceProcAddr instanceProcAddr = pFunctions->instanceProcAddr;

//  pOut->{Instance} = instanceProcAddr(instance, "{Instance}");

}

void dokLoadDeviceTable(VkDevice device, DokInstanceTable* pFunctions, DokDeviceTable* pOut) {
    PFN_vkGetDeviceProcAddr deviceProcAddr = pFunctions->vkGetDeviceProcAddr;

//  pOut->{Device} = deviceProcAddr(device, "{Device}");

}