#pragma once
#include<unordered_map>
#include<filesystem>

#include"../overlay/overlay.h"
#include"../hooks/windowHook.h"
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


class Communication
{
public:
    virtual ~Communication();

    Communication(): yPading(30), left(0), top(0), width(0), height(0), isSafe(true)
    {
        memset(&wi, 0, sizeof(wi));
    }
    
    HWND init(WindowData*& winData,uint32_t procId, CreateType type);

    void setWindowRect(int x, int y, int width, int height);

    void attachToWindow(HWND attachTo);

    bool isFullscreen() const;

    void removeTitleBar();

    void restoreTitleBar();

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
        for (auto& ch : str) sendChar(ch);
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
        if(getSafeStatus())
        WindowHook::setWindowHook(wi.hwnd);    
    }
    
    inline HWND getHWND() const
    {
        return wi.hwnd;
    }
    
    inline void setFocus() const
    {
       SetFocus(wi.hwnd);
    }

    inline void removeInstance()const
    {
        HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, wi.prcoessId);
        if(!h) return;
        TerminateProcess(h, 0);
        CloseHandle(h);
    }

protected:
    void updateWindowRect();
    virtual WindowInfo openProcess() = 0;
    virtual WindowInfo attachProcess(uint32_t procId);
    virtual void updateScreenPoint(std::unordered_map<int, ScreenPoint>& msp, int left, int top, int width, int height) = 0;


private:
    inline void setSafeStatus(bool enable) {isSafe = enable;};
    inline bool getSafeStatus() const {return isSafe;}

private:
    std::unordered_map<int, ScreenPoint> msp;
    int yPading;
    int left, top, width, height;
    WindowInfo wi;
    bool isSafe;

};