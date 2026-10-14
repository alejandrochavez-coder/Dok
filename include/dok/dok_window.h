#pragma once

#include <stdbool.h>

typedef struct DokWindow_T* DokWindow;

void dokCreateWindow(const char* title, int width, int height, DokWindow* pOut);

bool dokWindowExists(DokWindow window);

void dokPollWindow(DokWindow window);