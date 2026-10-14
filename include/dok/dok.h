#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DokResult {
    DOK_SUCCESS = 0,
    DOK_OUT_OF_MEMORY = 1,
    DOK_VULKAN_UNSUPPORTED = 2
} DokResult;

typedef struct DokInstance_T* DokInstance;

DokResult dokCreateInstance(DokInstance* pOut);

void dokDestroyInstance(DokInstance instance);

#ifdef __cplusplus
}
#endif