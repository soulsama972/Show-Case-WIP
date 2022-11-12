#include"windowHook.h"

WindowHookData whd = {0};

namespace WindowHook
{

    void setWindowHook(HWND hwnd)
    {
        removeHook();
        DWORD threadID = GetWindowThreadProcessId(hwnd,NULL);
        if(!threadID && hwnd != HWND_DESKTOP)
        {
            Utils::printMsg("error getting thread proc id\n");
            return;
        }

        whd.hList[WinHookType::KEYBOARD] = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)keyBoardCallBack, whd.instance, threadID);
        if (!whd.hList[WinHookType::KEYBOARD])
        {
            Utils::printMsg("Failed to install keyboard hook! %ld \n",  GetLastError());
            return;
        }

        whd.hList[WinHookType::MOUSE] = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseCallBack, whd.instance, threadID);
        if (!whd.hList[WinHookType::MOUSE])
        {
            Utils::printMsg("Failed to install mouse hook!  %ld \n", GetLastError());
            return;
        }

        whd.hList[WinHookType::WINPROC] = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)winProcCallBack,  whd.instance, threadID);
        if (!whd.hList[WinHookType::WINPROC])
        {
            Utils::printMsg("Failed to install hWinProc hook! %ld \n",  GetLastError());
            return;
        }
        whd.wd->target = hwnd;
    }

    void setWindowHook(HWND hwnd, WindowData*& winData)
    {
        setWindowHook(hwnd);
        winData = whd.wd;
    }

    void removeHook()
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

    bool is_valid_to_block_communication()
    {
        return whd.wd->blockInputCommunication && whd.wd->target != HWND_DESKTOP;
    }

    LRESULT __stdcall keyBoardCallBack(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if(nCode >= 0)
        {
            bool press = !((lParam >> 31) && 1);
            whd.wd->keys[wParam] = press;
        }
        if (is_valid_to_block_communication()) return true;

        return CallNextHookEx(whd.hList[WinHookType::KEYBOARD], nCode, wParam, lParam);
    }

    LRESULT __stdcall mouseCallBack(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if(nCode >= 0)
        {
            MOUSEHOOKSTRUCT mData = *(MOUSEHOOKSTRUCT*)lParam;

            if (mData.wHitTestCode == HTCLIENT)
            {
                if(!whd.wd->screenRect.left)  GetWindowRect(whd.wd->target, &whd.wd->screenRect);

                whd.wd->xPos = mData.pt.x - whd.wd->screenRect.left;
                whd.wd->yPos = mData.pt.y - whd.wd->screenRect.top;
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
        if (is_valid_to_block_communication()) return true;

        return CallNextHookEx(whd.hList[WinHookType::MOUSE], nCode, wParam, lParam);
    }

    LRESULT __stdcall winProcCallBack(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if(nCode >= 0)
        {
            CWPSTRUCT * msg =  (CWPSTRUCT*)lParam;
            switch (msg->message)
            {
            case WM_WINDOWPOSCHANGING:
            case WM_SIZE:
                GetWindowRect(whd.wd->target, &whd.wd->screenRect);
                break;
            
            default:
                break;
            }
        }
        if (is_valid_to_block_communication()) return true;

        return CallNextHookEx(whd.hList[WinHookType::WINPROC], nCode, wParam, lParam);
    }

}
