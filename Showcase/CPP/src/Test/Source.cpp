#include"../automation/windowHook.h"
#include<thread>


WindowHookData * wd;
void q()
{
    HWND handle = FindWindowA(0, "Untitled - Notepad");
    if(!handle) 
    {
        printf("error finding window \n");
        return;
    }

    DWORD thread_id = GetWindowThreadProcessId(handle,NULL);
    if(!thread_id)
    {
        printf("error getting thread proc id\n");
        return ;
    }

	if(setHook(handle, "a.dll", wd))
    {
        MSG msg;
        while(true)
        {
            if (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}

int main(void)
{
    std::thread th(q);
    while (true)
    {
        if(wd)
        {
            printf("%d %d %d\n", wd->leftClick, wd->middleClick, wd->rightClick);
            //printf("%d \n",wd->keys[0x20]);
        }
        //printf("%d \n", keys[0x20]);
        Sleep(100);
    }
    return 0;

}