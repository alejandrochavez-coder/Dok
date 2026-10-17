#pragma once

#include <vulkan/vulkan.h>
#include <stdbool.h>

typedef struct DokWindow_T* DokWindow;

void dokCreateWindow(const char* title, int width, int height, DokWindow* pOut);

bool dokWindowExists(DokWindow window);

void dokFreeWindow(DokWindow window);

void dokPollWindow(DokWindow window);