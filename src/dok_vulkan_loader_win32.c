#include <dok/dok_vulkan_loader.h>
#include <windows.h>

struct DokVkLoader_T {
    HMODULE dll;
};

void dokLoadVulkan(DokVkLoader* pOut) {
    HMODULE dll = LoadLibrary("vulkan-1.dll");
    if (!dll) {
        return;
    }

    DokVkLoader loader = (DokVkLoader) malloc(sizeof(struct DokVkLoader_T));
    loader->dll = dll;

    *pOut = loader;
}

void* dokGetFunction(DokVkLoader loader, const char* name) {
    return GetProcAddress(loader->dll, name);
}

void dokUnloadVulkan(DokVkLoader loader) {
    FreeLibrary(loader->dll);
    free(loader);
}