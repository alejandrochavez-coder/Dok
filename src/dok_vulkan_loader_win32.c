#include <dok/dok_vulkan_loader.h>
#include <windows.h>

struct DokVkLoader_T {
    HMODULE dll;
};

DokVkLoader dokLoadVulkan() {
    HMODULE dll = LoadLibrary("vulkan-1.dll");
    if (!dll) {
        return NULL;
    }

    DokVkLoader loader = (DokVkLoader) malloc(sizeof(struct DokVkLoader_T));
    loader->dll = dll;

    return loader;
}

void* dokGetFunction(DokVkLoader loader, const char* name) {
    return GetProcAddress(loader->dll, name);
}

void dokUnloadVulkan(DokVkLoader loader) {
    FreeLibrary(loader->dll);
    free(loader);
}