#pragma once
#include<windows.h>
#include<TlHelp32.h>
#include<unordered_map>
#include<filesystem>

#include"windowHook.h"
#include"../overlay/overlay.h"
#include"../utils/utils.h"


enum CreateType
{
    CREATE_PROCESS, // used for create puzzle pirate process and attach to it
    ATTACH, // used for attach the first puzzle pirate id found
    BROADCAST // used for sent to all top-level windows in the system
};


struct ScreenPoint
{
    ScreenPoint() :x(0), y(0) {}
    ScreenPoint(int x, int y) :x(x), y(y) {}
    int x;
    int y;
};


class Communication: public Overlay
{
public:
    static WindowHookData whd;
    
    Communication(): yPading(30), left(0), top(0), width(0), height(0)
    {
        memset(&wi, 0, sizeof(wi));
    }
    
    HWND init(WindowData*& winData, CreateType type);

    inline void sendKeyDown(char keyStroke) const
    {
        PostMessageA(wi.hwnd, WM_KEYDOWN, keyStroke, 0);
        Sleep(10);
    }

    inline void SendKeyUp(char keyStroke) const
    {
        PostMessageA(wi.hwnd, WM_KEYUP, keyStroke, 0);
    }

    inline void sendKeyPress(char keyStroke) const
    {
        sendKeyDown(keyStroke);
        SendKeyUp(keyStroke);
    }

    inline void sendChar(char ch) const
    {
        PostMessageA(wi.hwnd, WM_CHAR, ch, 0);
    }

    inline void sendString(const std::string& str) const
    {
        for (auto& ch : str)
            sendChar(ch);
    }
    
    inline void sendMouseClick(int x, int y, bool RightClick = false) const
    {
        LPARAM pos = MAKELPARAM(x + left, y + top + yPading);
        PostMessageA(wi.hwnd, WM_MOUSEMOVE, 0, pos);
        PostMessageA(wi.hwnd, WM_LBUTTONDOWN + RightClick * 3, MK_LBUTTON + RightClick, pos);
        PostMessageA(wi.hwnd, WM_LBUTTONUP + RightClick * 3, 0, pos);
    }

    inline void sendClickAtScreenPoint(int namedPoint, bool rightClick = false) const
    {
        if (msp.find(namedPoint) != msp.end())
        {
            auto sp = msp.at(namedPoint);
            sendMouseClick(sp.x, sp.y, rightClick);
        }
    }

    inline void hide() const
    {
        ShowWindow(wi.hwnd, SW_HIDE);
    }

    inline void show() const
    {
        ShowWindow(wi.hwnd, SW_SHOW);
        setWindowHook(wi.hwnd);
    }
    
    void setWindowRect(int x, int y, int width, int height);
    
    void setWindowHook(HWND hwnd) const;

    inline HWND getHWND() const
    {
        return wi.hwnd;
    }
    
    inline void setHWND(HWND hwnd)
    {
        wi.hwnd = hwnd;
    }

    inline void setFocus() const
    {
       SetFocus(wi.hwnd);
    }

    inline void killGame()const
    {
        HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, wi.prcoessId);
        TerminateProcess(h, 0);
        CloseHandle(h);
    }

    void attachToWindow(HWND attachTo);

    bool isFullscreen() const;

    void removeTitleBar();

    void restoreTitleBar();

protected:
    void updateWindowRect();
    virtual WindowInfo openProcess();
    virtual WindowInfo attachProcess();
    virtual void updateScreenPoint(std::unordered_map<int, ScreenPoint>& msp, int left, int top, int width, int height);

private:
    void removeHook() const;
    static LRESULT __stdcall keyBoardCallBack(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT __stdcall mouseCallBack(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT __stdcall winProcCallBack(int nCode, WPARAM wParam, LPARAM lParam);

    std::unordered_map<int, ScreenPoint> msp;
    int yPading;
    int left, top, width, height;
    WindowInfo wi;

};