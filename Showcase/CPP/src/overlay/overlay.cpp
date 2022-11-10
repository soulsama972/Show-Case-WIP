#include"overlay.h"



namespace Overlay
{
    std::vector<OverlayRect> overlayRect;
    std::vector<OverlayText> overlayText;
    HWND overlayHwnd = 0;
    std::thread th;
    int screenLeftPos = 0;
    int screenTopPos = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    bool isDone = false;
    
    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void run(HINSTANCE hInstance);

    void init(HINSTANCE hInstance)
    {
        if(overlayHwnd) return; // we only to init once
        th = std::thread(run, hInstance);

    }

    void cleanUp()
    {
        isDone = true;
        th.join();
    }

    void run(HINSTANCE hInstance)
    {
        WNDCLASSEXA wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style          = 0;
        wcex.lpfnWndProc    = WndProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = hInstance;
        wcex.hIcon          = 0;
        wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0));
        wcex.lpszMenuName   = REGISTERCLASSNAME;
        wcex.lpszClassName  = REGISTERCLASSNAME;
        wcex.hIconSm        = 0;

        if(!RegisterClassExA(&wcex))
        {
            Utils::printMsg("error register overlay class\n");
            return;
        }
    
        //WS_EX_PALETTEWINDOW
        overlayHwnd = CreateWindowExA( WS_EX_PALETTEWINDOW | WS_EX_LAYERED, REGISTERCLASSNAME, REGISTERCLASSNAME, WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

        if (!overlayHwnd)
        {
            Utils::printMsg("error createWindowExA overlay \n");
            return;
        }

        SetLayeredWindowAttributes(overlayHwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
        ShowWindow(overlayHwnd, SW_SHOW);

        MSG msg;

        SetTimer(overlayHwnd, 0, 1, NULL);

        while (GetMessageA(&msg, overlayHwnd, 0, 0) && !isDone)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            
        }
    }

    void present() 
    {

        HDC hdc = GetDC(overlayHwnd);
        HDC memDC = CreateCompatibleDC ( hdc );
        HBITMAP memBM = CreateCompatibleBitmap(hdc, screenWidth, screenHeight);
        
        SelectObject ( memDC, memBM );

        draw(memDC);

        BitBlt(hdc, 0, 0, screenWidth, screenHeight, memDC, 0, 0, SRCCOPY);

        
        DeleteObject(memBM);
        DeleteDC(memDC);
        ReleaseDC(overlayHwnd, hdc);

        overlayRect.clear();
        overlayText.clear();
    }

    void drawText(const char* text,int x, int y, int color, int fontSize)
    {
        overlayText.push_back({std::string(text), x, y, color, fontSize});
    }
    
    void drawRect(int x, int y, int width, int height, int color, bool onlyFrame)
    {
        overlayRect.push_back({x, y, width, height, color, onlyFrame});
    }

    void drawRect(HDC hdc, const OverlayRect& oRect)
    {
        RECT r = {oRect.x, oRect.y, oRect.width, oRect.height};
        HBRUSH bursh = CreateSolidBrush(oRect.color);
        oRect.onlyFrame ? FrameRect(hdc, &r, bursh) : FillRect(hdc, &r, bursh);
        DeleteObject(bursh);
    }

    void drawText(HDC hdc, const OverlayText& oText)
    {
        RECT textRect = {oText.x, oText.y, screenWidth, screenHeight};


        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        LOGFONT logfont;
        GetObject(hFont, sizeof(LOGFONT), &logfont);

        logfont.lfHeight = oText.fontSize;

        HFONT hNewFont = CreateFontIndirect(&logfont);
        HFONT hOldFont = (HFONT)SelectObject(hdc, hNewFont);

        SetTextColor(hdc, oText.color);
        SetBkMode(hdc,TRANSPARENT);
        DrawTextA(hdc, oText.text.c_str(), -1, &textRect, DT_TOP | DT_LEFT| DT_NOCLIP);
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hNewFont);
    }

    void updateWindowRect(int x, int y, int w, int h)
    {
        if(overlayHwnd) MoveWindow(overlayHwnd, x, y, w, h, true);
    }

    void draw(HDC hdc)
    {
        //if(overlayTargetWin == GetForegroundWindow() && overlayTargetWin != 0)
        {
            for (const auto &oRect : overlayRect) drawRect(hdc, oRect);
            for (const auto &oText : overlayText) drawText(hdc, oText);
        }
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        
        switch (message)
        {
        case WM_DESTROY: PostQuitMessage(0); break;
        case WM_MOVE:
            screenLeftPos = LOWORD(lParam);   // horizontal position 
            screenTopPos = HIWORD(lParam);   // vertical position 
        break;
        case WM_SIZE:
        {
            screenWidth = LOWORD(lParam);
            screenHeight = HIWORD(lParam);
        }
        break;
        case WM_TIMER:
        {
            updateWindowRect(whd.wd->screenRect.left, whd.wd->screenRect.top, whd.wd->screenRect.right - whd.wd->screenRect.left, whd.wd->screenRect.bottom - whd.wd->screenRect.top);
        }break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }

}