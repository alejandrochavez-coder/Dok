#pragma once

typedef struct DokWindow_T* DokWindow;

void dokCreateWindow(const char* title, int width, int height, DokWindow* pOut);

void dokPollWindow(DokWindow window);