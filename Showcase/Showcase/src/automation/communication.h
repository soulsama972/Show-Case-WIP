#pragma once
#include<windows.h>
#include<TlHelp32.h>

#include<iostream>
#include<string>
#include<unordered_map>
#include<filesystem>

struct WindowInfo
{
    DWORD prcoessId;
    HWND hwnd;
};

class Communication
{
public:
    Communication() :isInitDone(false), attach(false), yPading(30), left(0), top(0), width(0), height(0)
    {
        memset(&wi, 0, sizeof(wi));
    }
    inline ~Communication()
    {
        if (attach)
        {
            attachToWindow(HWND_DESKTOP);
            restoreTitleBar();
        }
    }

    HWND Init(std::string puzzlePath, bool attach);

    inline void SendKeyDown(char keyStroke) const
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
        SendKeyDown(keyStroke);
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
        POINT p = { x, y };
        LPARAM pos = MAKELPARAM(x + left, y + top + yPading);
        PostMessageA(wi.hwnd, WM_MOUSEMOVE, 0, pos);
        PostMessageA(wi.hwnd, WM_LBUTTONDOWN + RightClick * 3, MK_LBUTTON + RightClick, pos);
        PostMessageA(wi.hwnd, WM_LBUTTONUP + RightClick * 3, 0, pos);
    }

    inline void hide() const
    {
        ShowWindow(wi.hwnd, SW_HIDE);
    }

    inline void show() const
    {
        ShowWindow(wi.hwnd, SW_SHOW);
    }
    
    inline HWND getHWND() const
    {
        return wi.hwnd;
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

    bool isFullscreen(HWND windowHandle) const;

    void removeTitleBar();

    void restoreTitleBar();

protected:
    int left, top, width, height;


private:
    void openPuzzle(std::string puzzlePath) const;

    DWORD getJavaProcId(const std::unordered_map<DWORD, bool>& ignoreProcList) const;

    static BOOL CALLBACK getHWND(HWND hwnd, LPARAM lParam);

    bool attach;
    bool isInitDone;
    int yPading;
    WindowInfo wi;
};