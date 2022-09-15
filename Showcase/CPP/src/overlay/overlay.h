#pragma once
#include"../utils/utils.h"
#include<thread>



#define REGISTERCLASSNAME "overlayClass"

class Overlay
{
public:
    Overlay() = default;
    inline void setOverlayTargetWin(HWND hwnd) const {overlayTargetWin = hwnd;}
    inline HWND getOverlayHWND() const {return overlayHwnd;}
    inline void present() const {SendMessageA(overlayHwnd, WM_PAINT, 0,0);}

    static void init(HINSTANCE hInstance);
    void updateWindowRect(int x, int y, int w, int h) const;
    void addText(const std::string& text,int x, int y, int color, int fontSize) const;
    void addRect(int x, int y, int w, int h, int color) const;
    void drawRect(HWND key, int x, int y, int w, int h, int color) const;
    void drawText(HWND key, const std::string& text,int x, int y, int color, int fontSize) const;

private:
    static HWND overlayTargetWin;
    static HWND overlayHwnd;
    static std::thread th;
    static void run(HINSTANCE hInstance);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

