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
typedef struct DokGlobalTable DokGlobalTable;
typedef struct DokInstanceTable DokInstanceTable;
typedef struct DokDeviceTable DokDeviceTable;

DokResult dokCreateInstance(DokInstance* pOut);

void dokDestroyInstance(DokInstance instance);

void dokLoadGlobalTable(DokInstance instance, DokGlobalTable* pOut);

void dokLoadInstanceTable(VkInstance instance, DokGlobalTable* pFunctions, DokInstanceTable* pOut);

void dokLoadDeviceTable(VkDevice device, DokInstanceTable* pFunctions, DokDeviceTable* pOut);

struct DokGlobalTable {
//  PFN_{Global} {Global};
};

struct DokInstanceTable {
//  PFN_{Instance} {Instance};
};

struct DokDeviceTable {
//  PFN_{Device} {Device};
};

#ifdef __cplusplus
}
#endif