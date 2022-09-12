#include"windowHook.h"

HHOOK hKeyboard = 0;
HHOOK hMouse = 0;

bool setHook(HWND hwnd, const char* pathToDll, WindowHookData*& winData)
{
    removeHook();
    
    DWORD thread_id = GetWindowThreadProcessId(hwnd,NULL);
    if(!thread_id)
    {
        printf("error getting thread proc id\n");
        return false;
    }    

    HMODULE dll_handle = LoadLibraryA(pathToDll);
    if (!dll_handle)
    {
        printf("Failed to load lib %s \n", pathToDll);
        return false;
    }

    winData = ((_GetWindowData)GetProcAddress(dll_handle, "getWindowData"))();
    if(!winData)
    {
        printf("Failed to _set Keys! \n");
        return false;
    }

    hKeyboard = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)GetProcAddress(dll_handle, "keyBoardCallBack"), dll_handle, thread_id);
    if (!hKeyboard)
    {
        printf("Failed to install keyboard hook! \n");
        printf("%ld \n",  GetLastError());
        return false;
    }

    hMouse = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)GetProcAddress(dll_handle, "mouseCallBack"), dll_handle, thread_id);
    if (!hMouse)
    {
        printf("Failed to install mouse hook! \n");
        printf("%ld \n",  GetLastError());
        return false;
    }
    
    return true;
}

void removeHook()
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

}
