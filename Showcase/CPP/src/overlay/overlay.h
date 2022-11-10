#pragma once
#include"../utils/utils.h"
#include"../hooks/windowHook.h"
#include<thread>
#include<vector>


#define REGISTERCLASSNAME "overlayClass"

struct OverlayRect
{
    int x, y, width, height, color;
    bool onlyFrame; // true if to draw only the border false the all rect
};

struct OverlayText
{
    std::string text;
    int x, y, color, fontSize;
};


namespace Overlay
{

    void init(HINSTANCE hInstance);
    void cleanUp();
    void updateWindowRect(int x, int y, int width, int height);
    
    EXPORT void drawText(const char* text,int x, int y, int color, int fontSize);
    EXPORT void drawRect(int x, int y, int width, int height, int color, bool onlyFrame);

    void drawRect(HDC hdc, const OverlayRect& oRect);
    void drawText(HDC hdc, const OverlayText& oText);
    
    void draw(HDC hdc);
    EXPORT void present();
};

