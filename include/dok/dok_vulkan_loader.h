#pragma once

typedef struct DokVkLoader_T* DokVkLoader;

DokVkLoader dokLoadVulkan();

void* dokGetFunction(DokVkLoader loader, const char* name);

void dokUnloadVulkan(DokVkLoader loader);