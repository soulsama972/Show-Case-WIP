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
    Communication() :isInitDone(false), remoteThreadId(0), yPading(30), left(0), top(0), width(0), height(0)
    {
        memset(&wi, 0, sizeof(wi));
    }

    HWND Init(std::string puzzlePath, bool attach);

    inline void SendKeyDown(char keyStroke) const
    {
        PostMessageA(wi.hwnd, WM_KEYDOWN, keyStroke, 0);
        Sleep(1);
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
        LPARAM pos = MAKELPARAM(x + left, y + top + yPading);
        setFocus();
        PostMessageA(wi.hwnd, WM_MOUSEMOVE, 0, pos);
        PostMessageA(wi.hwnd, WM_LBUTTONDOWN + RightClick * 3, MK_LBUTTON + RightClick, pos);
        PostMessageA(wi.hwnd, WM_LBUTTONUP + RightClick * 3, 0, pos);
    }

    inline void setFocus() const
    {
        PostMessageA(wi.hwnd, WM_SETFOCUS, 0, 0);
    }

    inline void killGame()const
    {
        HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, wi.prcoessId);
        TerminateProcess(h, 0);
        CloseHandle(h);
    }

    inline void minGame() const
    {
        ShowWindow(wi.hwnd, SW_MINIMIZE);
    }

    bool isFullscreen(HWND windowHandle);

protected:
    int left, top, width, height;


private:
    void openPuzzle(std::string puzzlePath) const;

    DWORD getJavaProcId(const std::unordered_map<DWORD, bool>& procIds) const;

    static BOOL CALLBACK getHWND(HWND hwnd, LPARAM lParam);


    bool isInitDone;
    int yPading;
    DWORD remoteThreadId;
    WindowInfo wi;
};