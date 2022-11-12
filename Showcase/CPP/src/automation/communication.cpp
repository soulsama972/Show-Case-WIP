#include"communication.h"

Communication::~Communication()
{
    setSafeStatus(false);
    restoreTitleBar(); 
    attachToWindow(HWND_DESKTOP);
    show();
    setSafeStatus(true);
}

HWND Communication::init(WindowData*& winData, uint32_t procId, CreateType type)
{
    if (wi.hwnd) return wi.hwnd;
    
    switch (type)
    {
    case CREATE_PROCESS: wi = openProcess(); break;  
    case ATTACH: wi = attachProcess(procId);break; 
    case BROADCAST: wi = {0, HWND_BROADCAST, false}; break;
    default:
        break;
    }
    if (!wi.hwnd) 
    {
        Utils::printMsg("failed to get hwnd \n");
        return 0;
    }

    if(getSafeStatus())
    {
        updateWindowRect();
        WindowHook::setWindowHook(wi.hwnd, winData);
    }

    return wi.hwnd;
}

void Communication::setWindowRect(int x, int y, int width, int height)
{
    MoveWindow(wi.hwnd, x, y, width, height, true);
    updateWindowRect();
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
    lStyle &= ~(WS_BORDER | WS_DLGFRAME);
    SetWindowLong(wi.hwnd, GWL_STYLE, lStyle);
    updateWindowRect();
}

void Communication::restoreTitleBar()
{
    yPading = 30;
    LONG lStyle = GetWindowLong(wi.hwnd, GWL_STYLE);
    lStyle |= (WS_BORDER | WS_DLGFRAME);
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
    yPading = lStyle & WS_BORDER ? 30 : 0;

    if(getSafeStatus()) updateScreenPoint(msp,left,top, width, height);
}

WindowInfo Communication::attachProcess(uint32_t procId)
{
    WindowInfo wi = {0};
    wi.prcoessId = procId;
    EnumWindows(Utils::getHWND, (LPARAM)&wi);
    wi.attach = true;
    return wi;
}