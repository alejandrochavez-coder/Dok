#pragma once

typedef struct DokVkLoader_T* DokVkLoader;

void dokLoadVulkan(DokVkLoader* pOut);

void* dokGetFunction(DokVkLoader loader, const char* name);

void dokUnloadVulkan(DokVkLoader loader);