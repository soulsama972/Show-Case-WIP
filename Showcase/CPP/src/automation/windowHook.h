#pragma once
#include<windows.h>

#define WINDATASMNAME "Global\\SMWindowData"

constexpr int winHookCount = 3; // size = type of hook

enum WinHookType
{
    KEYBOARD,
    MOUSE,
    WINPROC
};

struct WindowData /* used for hooking the target application for infomation like keyboard mouse etc..*/
{
    bool keys[256];
    bool leftClick;
    bool middleClick;
    bool rightClick;
    int xPos, yPos;
};


struct WindowHookData
{
    HHOOK hList[winHookCount];
    HINSTANCE instance;
    HWND target;
    HANDLE hMapFile;
    WindowData *wd;
    RECT screenRect;
};