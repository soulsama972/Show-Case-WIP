#include"communication.h"


WindowHookData Communication::whd = {0};

HWND Communication::init(WindowData*& winData, CreateType type)
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
    if (!wi.hwnd) Utils::printMsg("failed to get hwnd \n");
    else
        updateWindowRect();

    winData = whd.wd;
    
    setWindowHook(wi.hwnd);
    std::thread  th(Overlay::init,whd.instance);
    th.join();
    
    return wi.hwnd;
}

void Communication::setWindowRect(int x, int y, int width, int height)
{
    MoveWindow(wi.hwnd, x, y, width, height, true);
    updateWindowRect();
}

void Communication::setWindowHook(HWND hwnd) const
{
    removeHook();

    DWORD thread_id = GetWindowThreadProcessId(hwnd,NULL);
    if(!thread_id)
    {
        Utils::printMsg("error getting thread proc id\n");
        return;
    }
    whd.hList[WinHookType::KEYBOARD] = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)keyBoardCallBack, whd.instance, thread_id);
    if (!whd.hList[WinHookType::KEYBOARD])
    {
        Utils::printMsg("Failed to install keyboard hook! %ld \n",  GetLastError());
        return;
    }
    whd.hList[WinHookType::MOUSE] = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseCallBack, whd.instance, thread_id);
    if (!whd.hList[WinHookType::MOUSE])
    {
        Utils::printMsg("Failed to install mouse hook!  %ld \n", GetLastError());
        return;
    }
    whd.hList[WinHookType::WINPROC] = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)winProcCallBack,  whd.instance, thread_id);
    if (!whd.hList[WinHookType::WINPROC])
    {
        Utils::printMsg("Failed to install hWinProc hook! %ld \n",  GetLastError());
        return;
    }
}

void Communication::attachToWindow(HWND attachTo)
{
    setOverlayTargetWin(wi.hwnd);
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
    lStyle &= ~(WS_BORDER);
    SetWindowLong(wi.hwnd, GWL_STYLE, lStyle);
    updateWindowRect();
}

void Communication::restoreTitleBar()
{
    yPading = 30;
    LONG lStyle = GetWindowLong(wi.hwnd, GWL_STYLE);
    lStyle |= (WS_BORDER);
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
    Overlay::updateWindowRect(left,top, width, height);
    updateScreenPoint(msp,left,top, width, height);
}

WindowInfo Communication::openProcess() { return {0};}

WindowInfo Communication::attachProcess() {return {0};}

void Communication::updateScreenPoint(std::unordered_map<int, ScreenPoint>& msp, int left, int top, int width, int height) {}

LRESULT __stdcall Communication::keyBoardCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        bool press = !((lParam >> 31) && 1);
        whd.wd->keys[wParam] = press;
    }
    return CallNextHookEx(whd.hList[WinHookType::KEYBOARD], nCode, wParam, lParam);
}

LRESULT __stdcall Communication::mouseCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        MOUSEHOOKSTRUCT mData = *(MOUSEHOOKSTRUCT*)lParam;

        if (mData.wHitTestCode == HTCLIENT)
        {
            if(!whd.screenRect.left)  GetWindowRect(whd.target, &whd.screenRect);

            whd.wd->xPos = mData.pt.x - whd.screenRect.left;
            whd.wd->yPos = mData.pt.y - whd.screenRect.top;
        }
        switch (wParam)
        {
        case WM_LBUTTONDOWN: whd.wd->leftClick = true; break;
        case WM_LBUTTONUP:   whd.wd->leftClick = false; break;
        case WM_RBUTTONDOWN: whd.wd->rightClick = true; break;
        case WM_RBUTTONUP:   whd.wd->rightClick = false; break;
        case WM_MBUTTONDOWN: whd.wd->middleClick = true; break;
        case WM_MBUTTONUP: whd.wd->middleClick = false; break;
        default: break;
        }
    }
    return CallNextHookEx(whd.hList[WinHookType::MOUSE], nCode, wParam, lParam);
}

LRESULT __stdcall Communication::winProcCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        CWPSTRUCT * msg =  (CWPSTRUCT*)lParam;
        
        switch (msg->message)
        {
        case WM_WINDOWPOSCHANGING:
        case WM_SIZE:
            GetWindowRect(whd.target, &whd.screenRect);
            break;
        
        default:
            break;
        }
        whd.target = msg->hwnd;
    }
    return CallNextHookEx(whd.hList[WinHookType::WINPROC], nCode, wParam, lParam);
}

void Communication::removeHook() const
{
    for(int i = 0; i < winHookCount; i++)
    {
        if(whd.hList[i])
        {
            UnhookWindowsHookEx(whd.hList[i]);
            whd.hList[i] = 0;
        }
    }
}