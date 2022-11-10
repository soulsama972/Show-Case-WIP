#pragma once
#include<windows.h>
#include"../utils/utils.h"

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
    bool blockInputCommunication;
    HWND target;
    RECT screenRect;
};


struct WindowHookData
{
    HHOOK hList[winHookCount];
    HINSTANCE instance;
    HANDLE hMapFile;
    WindowData *wd;
};

namespace WindowHook
{
    void setWindowHook(HWND hwnd, WindowData*& winData);
    void setWindowHook(HWND hwnd);
    void removeHook();

    LRESULT __stdcall keyBoardCallBack(int nCode, WPARAM wParam, LPARAM lParam);
    LRESULT __stdcall mouseCallBack(int nCode, WPARAM wParam, LPARAM lParam);
    LRESULT __stdcall winProcCallBack(int nCode, WPARAM wParam, LPARAM lParam);

}
extern WindowHookData whd;