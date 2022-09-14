#include"automation/manager.h"
#include"automation/windowHook.h"


//bilge pos:  width /2 - 350  height / 2 - 250  size: 270 540

HINSTANCE hinst; 
WindowHookData * d;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL InitApplication(HINSTANCE hinstance) 
{ 
    WNDCLASSEXA wcx; 
 
 
    wcx.cbSize = sizeof(wcx);          // size of structure 
    wcx.style = CS_HREDRAW | CS_VREDRAW; // redraw if size changes 
    wcx.lpfnWndProc = WindowProc;     // points to window procedure 
    wcx.cbClsExtra = 0;                // no extra class memory 
    wcx.cbWndExtra = 0;                // no extra window memory 
    wcx.hInstance = hinstance;         // handle to instance 
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);              // predefined app. icon 
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);                    // predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH); // white background brush 
    wcx.lpszMenuName =  "MainMenu";    // name of menu resource 
    wcx.lpszClassName = "TEST";  // name of window class 
    wcx.hIconSm = 0; 
 
    // Register the window class. 
 
    return RegisterClassExA(&wcx); 
} 
 
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow) 
{ 
    HWND hwnd; 
 
    // Save the application-instance handle. 
 
    hinst = hinstance; 
 
    // Create the main window. 
 
    hwnd = CreateWindowA( 
        "TEST",        // name of window class 
        "TEST",            // title-bar string 
        WS_OVERLAPPEDWINDOW, // top-level window 
        CW_USEDEFAULT,       // default horizontal position 
        CW_USEDEFAULT,       // default vertical position 
        CW_USEDEFAULT,       // default width 
        CW_USEDEFAULT,       // default height 
        (HWND) NULL,         // no owner window 
        (HMENU) NULL,        // use class menu 
        hinstance,           // handle to application instance 
        (LPVOID) NULL);      // no window-creation data 
 
    if (!hwnd) 
        return FALSE; 
 
    // Show the window and send a WM_PAINT message to the window 
    // procedure. 
 
    ShowWindow(hwnd, nCmdShow); 
    UpdateWindow(hwnd); 
    return TRUE; 
 
} 
   
#include<thread>

void a()
{
    while (true)
    {
        printf("%d \n", d->xPos);
        Sleep(1000);
    }
    


}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance,  LPSTR lpCmdLine, int nCmdShow) 
{ 
    UNREFERENCED_PARAMETER(lpCmdLine); 
    MSG msg; 
 
    // if (!InitApplication(hinstance)) 
    //     return 0; 
 
    // if (!InitInstance(hinstance, nCmdShow)) 
    //     return 0; 
 
    HWND handle = FindWindowA(0,"GH Injector - x64");
    if(!handle) 
    {
        printf("error finding window \n");
        return 0;
    }

	setHook(handle, "G:/projects/Showcase/Showcase/Python/cppWrapper/windowHook.dll", d);
    std::thread th(a);
    
    BOOL fGotMessage;
    while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1) 
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    } 
    
    return msg.wParam; 

//     //PuzzlePirateAutomation c("C:\\games\\Puzzle Pirates", CreateType::CREATE_PROCESS);

//     //c.login("danielkun2", "Aa123456!", 1);
//     //return 0;
//     // HWND a = c.getHWND();
//     // SetWindowLong(a, GWL_EXSTYLE, GetWindowLong(a, GWL_EXSTYLE) | WS_EX_NOACTIVATE | WS_EX_APPWINDOW);

//     // c.hide();
//     // Sleep(1000);
//     // c.show();
//     Sleep(1000);

//    //Sleep(5000);
//    ////com.clickAtScreenPoint(TIP_OFF);
//    //com.clickAtScreenPoint(YE);
//    //Sleep(100);
//    //com.clickAtScreenPoint(NOTICE_BOARD);
//    //Sleep(2000);
//    //com.clickAtScreenPoint(PUZZLE);
//    //Sleep(200);
//    //com.clickAtScreenPoint(BILGE_PUZZLE);
//    //Sleep(200);
//    //com.clickAtScreenPoint(PLAY_NOW_PUZZLE);
//    //Sleep(200);
//    //Sleep(100);
//    //com.clickAtScreenPoint(BOTTOM_RIGHT_CONER_VIEW_AREA, true);
//    ////com.killGame();
//     return 0;


} 



