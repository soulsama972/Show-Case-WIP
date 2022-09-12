#pragma once
#include"../utils/utils.h"

/* this file used for hooking the target application for infomation like keyboard mouse etc..*/

struct WindowHookData
{
    bool keys[256];
    bool leftClick;
    bool middleClick;
    bool rightClick;
    int xPos, yPos;
};

using _GetWindowData = WindowHookData *(__stdcall *)();
using _SetHook = bool(__stdcall *)(HWND hwnd, const char* pathToDll, WindowHookData*& winData);


EXPORT bool setHook(HWND hwnd, const char* pathToDll, WindowHookData *& winData);
EXPORT void removeHook();
