#include<windows.h>
#include<iostream>

#define EXPORT extern "C" __declspec(dllexport)

struct WindowHookData
{
    bool keys[256];
    bool leftClick;
    bool middleClick;
    bool rightClick;
    int xPos, yPos;
};

HHOOK hKeyboard = 0;
HHOOK hMouse = 0;
HHOOK hWinProc = 0;
HINSTANCE instance = 0;
HWND target = 0;
HANDLE hMapFile = 0;
WindowHookData *wd = 0;
RECT screenRect = {0};

const char *szName = "Global\\SMWindowHookData";


LRESULT __stdcall keyBoardCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        bool press = !((lParam >> 31) && 1);
        wd->keys[wParam] = press;
    }
    return CallNextHookEx(hKeyboard, nCode, wParam, lParam);
}

LRESULT __stdcall mouseCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    
    if(nCode >= 0)
    {
        MOUSEHOOKSTRUCT mData = *(MOUSEHOOKSTRUCT*)lParam;


        if (mData.wHitTestCode == HTCLIENT)
        {
            if(!screenRect.left)  GetWindowRect(target, &screenRect);

            wd->xPos = mData.pt.x - screenRect.left;
            wd->yPos = mData.pt.y - screenRect.top;
        }
        switch (wParam)
        {
        case WM_LBUTTONDOWN: wd->leftClick = true; break;
        case WM_LBUTTONUP:   wd->leftClick = false; break;
        case WM_RBUTTONDOWN: wd->rightClick = true; break;
        case WM_RBUTTONUP:   wd->rightClick = false; break;
        case WM_MBUTTONDOWN: wd->middleClick = true; break;
        case WM_MBUTTONUP: wd->middleClick = false; break;
        default: break;
        }
    }
    return CallNextHookEx(hMouse, nCode, wParam, lParam);
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
            GetWindowRect(target, &screenRect);
            break;
        
        default:
            break;
        }
        target = msg->hwnd;
    }
    return CallNextHookEx(hWinProc, nCode, wParam, lParam);
}

void __stdcall removeHook()
{
    if (hKeyboard) 
    {
        UnhookWindowsHookEx(hKeyboard);
        hKeyboard = 0;
    }

    if (hMouse) 
    {
        UnhookWindowsHookEx(hMouse);
        hMouse = 0;
    }

    if (hWinProc) 
    {
        UnhookWindowsHookEx(hWinProc);
        hWinProc = 0;
    }

}

EXPORT bool setHook(HWND hwnd)
{
    removeHook();
    DWORD thread_id = GetWindowThreadProcessId(hwnd,NULL);
    if(!thread_id)
    {
        printf("error getting thread proc id\n");
        return false;
    }
    hKeyboard = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)keyBoardCallBack, instance, thread_id);
    if (!hKeyboard)
    {
        printf("Failed to install keyboard hook! %ld \n",  GetLastError());
        return false;
    }
    hMouse = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseCallBack, instance, thread_id);
    if (!hMouse)
    {
        printf("Failed to install mouse hook!  %ld \n", GetLastError());
        return false;
    }
    hWinProc = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)winProcCallBack,  instance, thread_id);
    if (!hWinProc)
    {
        printf("Failed to install hWinProc hook! %ld \n",  GetLastError());
        return false;
    }
    return true;
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE,szName);

            if (!hMapFile) printf("Could not open file mapping object (%ld).\n",GetLastError());

            wd = (WindowHookData*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS,0,0, sizeof(WindowHookData));

            if (!wd)
            {
                printf("Could not map view of file (%ld).\n",GetLastError());
                CloseHandle(hMapFile);
            }
            instance = (HINSTANCE)hModule;
        }
        break;
    }
    return TRUE;
}