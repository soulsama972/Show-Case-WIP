#include"windowHook.h"


HANDLE hMapFile = 0;
WindowHookData* wd = 0;
HMODULE dll_handle = 0;
const char *szName = "Global\\SMWindowHookData";

using _setHook = bool (*)(HWND hwnd);

bool setHook(HWND hwnd, const char* pathToDll, WindowHookData*& winData)
{

    if(!dll_handle)
    {
        dll_handle = LoadLibraryA(pathToDll);
        if (!dll_handle)
        {
            printf("Failed to load lib %s \n", pathToDll);
            return false;
        }
    }

    winData = wd;
    return ((_setHook)GetProcAddress(dll_handle, "setHook"))(hwnd);

    
    return false;
}


BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(WindowHookData), szName);
            if (!hMapFile) printf("Could not create file mapping object (%ld).\n",GetLastError());
                    
            wd = (WindowHookData*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS,0,0, 0);

            if (!wd)
            {
                printf("Could not map view of file (%ld).\n",GetLastError());
                CloseHandle(hMapFile);
                return false;
            }
            memset(wd, 0, sizeof(WindowHookData));
        }
        break;
    }
    return TRUE;
}