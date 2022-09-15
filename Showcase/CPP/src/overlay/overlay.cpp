#include"overlay.h"

HWND Overlay::overlayHwnd = 0;
HWND Overlay::overlayTargetWin = 0;
std::thread Overlay::th;


void Overlay::init(HINSTANCE hInstance)
{
    if(overlayHwnd) return; // we only to init once
    th = std::thread(Overlay::run, hInstance);
}

void Overlay::run(HINSTANCE hInstance)
{
    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(nullptr, IDC_CROSS);
    wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName   = REGISTERCLASSNAME;
    wcex.lpszClassName  = REGISTERCLASSNAME;
        wcex.hIconSm        = 0;

    if(!RegisterClassExA(&wcex))
    {
        Utils::printMsg("error register overlay class\n");
        return;
    }
  
    overlayHwnd = CreateWindowExA(WS_EX_PALETTEWINDOW | WS_EX_LAYERED, REGISTERCLASSNAME, REGISTERCLASSNAME, WS_POPUP,
      0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!overlayHwnd)
    {
        Utils::printMsg("error createWindowExA overlay \n");
        return;
    }

    SetLayeredWindowAttributes(overlayHwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(overlayHwnd, SW_SHOWNA);


    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Overlay::drawRect(HWND key,int x, int y, int w, int h, int color) const
{
    HDC hdc = GetDC(key);
    RECT r = {x, y, w, h};
    HBRUSH bursh = CreateSolidBrush(color);
    FillRect(hdc, &r, bursh);
    DeleteObject(bursh);
    ReleaseDC(key, hdc);
}

void Overlay::drawText(HWND key,const std::string& text,int x, int y, int color, int fontSize) const
{
    RECT r;
    HDC hdc = GetDC(key);
    GetWindowRect(key, &r);
    r.left = x;
    r.top = y;

    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    LOGFONT logfont;
    GetObject(hFont, sizeof(LOGFONT), &logfont);

    logfont.lfHeight = fontSize;

    HFONT hNewFont = CreateFontIndirect(&logfont);
    HFONT hOldFont = (HFONT)SelectObject(hdc, hNewFont);

    SetTextColor(hdc, color);
    SetBkMode(hdc,TRANSPARENT);
    DrawTextA(hdc, text.c_str(), -1, &r, DT_TOP | DT_LEFT| DT_NOCLIP);
    
    // Always select the old font back into the DC
    SelectObject(hdc, hOldFont);
    DeleteObject(hNewFont);

    ReleaseDC(key, hdc);

}

void Overlay::updateWindowRect(int x, int y, int w, int h) const
{
    MoveWindow(overlayHwnd, x, y, w, h, true);
}

LRESULT CALLBACK Overlay::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT     ps;
    HDC             hdc;
    RECT r;
    switch (message)
    {
    case WM_PAINT:
    {
        if(overlayTargetWin == GetForegroundWindow() && overlayTargetWin != 0)
        {
            printf("here \n");
            hdc = BeginPaint(hwnd, &ps);
            GetWindowRect(hwnd, &r);
            r.left = 50;
            r.top = 50;

            HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            LOGFONT logfont;
            GetObject(hFont, sizeof(LOGFONT), &logfont);

            logfont.lfHeight = 30;

            HFONT hNewFont = CreateFontIndirect(&logfont);
            HFONT hOldFont = (HFONT)SelectObject(hdc, hNewFont);

            SetTextColor(hdc, 0xffff00);
            SetBkMode(hdc,TRANSPARENT);
            DrawTextA(hdc, "test 12345", -1, &r, DT_TOP | DT_LEFT| DT_NOCLIP);
            
            // Always select the old font back into the DC
            SelectObject(hdc, hOldFont);
            DeleteObject(hNewFont);

            EndPaint(hwnd, &ps);
        }
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}