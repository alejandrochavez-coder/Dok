#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DokResult {
    DOK_SUCCESS = 0,
    DOK_OUT_OF_MEMORY = 1,
    DOK_VULKAN_UNSUPPORTED = 2
} DokResult;

typedef struct DokInstance_T* DokInstance;
typedef struct DokGlobalFunctions DokGlobalFunctions;
typedef struct DokInstanceFunctions DokInstanceFunctions;
typedef struct DokDeviceFunctions DokDeviceFunctions;

DokResult dokCreateInstance(DokInstance* pInstance);

void dokDestroyInstance(DokInstance instance);

void dokInitializeGlobalFunctions(DokInstance instance, DokGlobalFunctions* pGlobalFunctions);

void dokInitializeInstanceFunctions(VkInstance instance, DokGlobalFunctions* pGlobalFunctions, DokInstanceFunctions* pInstanceFunctions);

void dokInitializeDeviceFunctions(VkDevice device, DokInstanceFunctions* pInstanceFunctions, DokDeviceFunctions* pDeviceFunctions);

#ifdef __cplusplus
}
#endif

struct DokGlobalFunctions {
//  PFN_{Global} {Global};
// Global(name)     PFN_{name} {name};
};

struct DokInstanceFunctions {
//  PFN_{Instance} {Instance};
// Instance(name)   PFN_{name} {name};
};

struct DokDeviceFunctions {
// PFN_{Device} {Device};
// Instance(name)   PFN_{name} {name};
};