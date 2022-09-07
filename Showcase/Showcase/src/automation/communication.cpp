#include"communication.h"


HWND Communication::Init(CreateType type)
{
    if (wi.hwnd) return wi.hwnd;
    
    switch (type)
    {
    case CREATE_PROCESS: wi = openProcess(); break;  
    case ATTACH: wi = attachProcess();break; 
    case BROADCAST: wi = {0, HWND_BROADCAST, false}; break;
    default:
        break;
    }
    if (!wi.hwnd)
        printf("failed to get hwnd \n");
    else
        updateWindowRect();
    
    return wi.hwnd;
}

void Communication::setWindowRect(int x, int y, int width, int height)
{
    MoveWindow(wi.hwnd, x, y, width, height, true);
    updateWindowRect();
}

void Communication::getMousePos(int& x, int& y)
{
    POINT p;
    GetCursorPos(&p);
    x = p.x - left;
    y = p.y -  top;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > width) x = width;
    if (y > height) y = height;
}

void Communication::attachToWindow(HWND attachTo)
{
    SetParent(wi.hwnd, attachTo);
    updateWindowRect();
}

bool Communication::isFullscreen() const
{
    MONITORINFO monitorInfo = { 0 };
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(MonitorFromWindow(wi.hwnd, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

    RECT windowRect;
    GetWindowRect(wi.hwnd, &windowRect);

    return windowRect.left == monitorInfo.rcMonitor.left
        && windowRect.right == monitorInfo.rcMonitor.right
        && windowRect.top == monitorInfo.rcMonitor.top
        && windowRect.bottom == monitorInfo.rcMonitor.bottom;
}

void Communication::removeTitleBar()
{
    yPading = 0;
    LONG lStyle = GetWindowLong(wi.hwnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION);
    SetWindowLong(wi.hwnd, GWL_STYLE, lStyle);
    updateWindowRect();
}

void Communication::restoreTitleBar()
{
    yPading = 30;
    LONG lStyle = GetWindowLong(wi.hwnd, GWL_STYLE);
    lStyle |= (WS_CAPTION);
    SetWindowLong(wi.hwnd, GWL_STYLE, lStyle);
    updateWindowRect();
}

void Communication::updateWindowRect()
{
    RECT screenRect;
    GetWindowRect(wi.hwnd, &screenRect);
    left = screenRect.left;
    top = screenRect.top;
    width = screenRect.right - screenRect.left;
    height = screenRect.bottom - screenRect.top;

    LONG lStyle = GetWindowLong(wi.hwnd, GWL_STYLE);
    yPading = lStyle & WS_CAPTION ? 30 : 0;

    updateScreenPoint(msp,left, top, width, height);
}

