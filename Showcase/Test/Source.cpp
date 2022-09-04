// Global variable 
#include <windows.h> 

HINSTANCE hinst;

// Function prototypes. 
BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK MainWProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Application entry point. 

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;

    if (!InitApplication(hinstance))
        return FALSE;

    if (!InitInstance(hinstance, nCmdShow))
        return FALSE;

    BOOL fGotMessage;
    while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
    UNREFERENCED_PARAMETER(lpCmdLine);
}

BOOL InitApplication(HINSTANCE hinstance)
{
    WNDCLASSEXW wcx;

    // Fill in the window class structure with parameters 
    // that describe the main window. 
    wcx.cbSize = sizeof(wcx);          // size of structure 
    wcx.style = CS_HREDRAW |
        CS_VREDRAW;                    // redraw if size changes 
    wcx.lpfnWndProc = (WNDPROC)MainWProc;     // points to window procedure 
    wcx.cbClsExtra = 0;                // no extra class memory 
    wcx.cbWndExtra = 0;                // no extra window memory 
    wcx.hInstance = hinstance;         // handle to instance 
    wcx.hIcon = LoadIcon(NULL,
        IDI_APPLICATION);              // predefined app. icon 
    wcx.hCursor = LoadCursor(NULL,
        IDC_ARROW);                    // predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // white background brush 
    wcx.lpszMenuName = L"MainMenu";    // name of menu resource 
    wcx.lpszClassName = L"MainWClass";  // name of window class 
    wcx.hIconSm = (HICON)LoadImage(hinstance,MAKEINTRESOURCE(5),IMAGE_ICON,GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),LR_DEFAULTCOLOR);
    // Register the window class. 
    return RegisterClassExW(&wcx);
}

BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
{
    HWND hwnd;

    // Save the application-instance handle. 

    hinst = hinstance;

    // Create the main window. 

    hwnd = CreateWindow(
        L"MainWClass",        // name of window class 
        L"Sample",            // title-bar string 
        WS_OVERLAPPEDWINDOW, // top-level window 
        CW_USEDEFAULT,       // default horizontal position 
        CW_USEDEFAULT,       // default vertical position 
        CW_USEDEFAULT,       // default width 
        CW_USEDEFAULT,       // default height 
        (HWND)NULL,         // no owner window 
        (HMENU)NULL,        // use class menu 
        hinstance,           // handle to application instance 
        (LPVOID)NULL);      // no window-creation data 

    if (!hwnd)
        return FALSE;

    // Show the window and send a WM_PAINT message to the window 
    // procedure. 

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return TRUE;

}

