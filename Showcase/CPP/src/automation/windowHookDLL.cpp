#include<windows.h>
#include<iostream>

#define EXPORT extern "C" __declspec(dllexport)

struct WindowHookData
{
    bool keys[256];
    bool leftClick;
    bool middleClick;
    bool rightClick;
    int xPos, yPos;
};

WindowHookData wd;

EXPORT WindowHookData* getWindowData()
{
    return &wd;
}

EXPORT LRESULT keyBoardCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        bool press = !((lParam >> 31) && 1);
        wd.keys[wParam] = press;
    }
    return CallNextHookEx(0, nCode, wParam, lParam);
}

EXPORT LRESULT mouseCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        MOUSEHOOKSTRUCT mData = *(MOUSEHOOKSTRUCT*)lParam;
        if (mData.wHitTestCode == HTCLIENT)
        {
            wd.xPos = mData.pt.x;
            wd.yPos = mData.pt.y;
        }
        switch (wParam)
        {
        case WM_LBUTTONDOWN: wd.leftClick = true; break;
        case WM_LBUTTONUP:   wd.leftClick = false; break;
        case WM_RBUTTONDOWN: wd.rightClick = true; break;
        case WM_RBUTTONUP:   wd.rightClick = false; break;
        case WM_MBUTTONDOWN: wd.middleClick = true; break;
        case WM_MBUTTONUP: wd.middleClick = false; break;
        default: break;
        }
    }
    return CallNextHookEx(0, nCode, wParam, lParam);
}