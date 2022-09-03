#include"communication.h"

HWND Communication::Init(std::string puzzlePath, bool attach)
{
    RECT screenRect;
    DWORD procId = 0;
    std::unordered_map<DWORD, bool> procIds;
    if (isInitDone) return 0;

    if (attach)
        wi.prcoessId = getJavaProcId(procIds);
    
    else
    {
        do
        {
            procId = getJavaProcId(procIds);
            procIds[procId] = true;

        } while (procId);


        openPuzzle(puzzlePath);
        wi.prcoessId = getJavaProcId(procIds);
    }

    EnumWindows(getHWND, (LPARAM)&wi);

    if (!wi.hwnd) return 0;

    if (isFullscreen(wi.hwnd)) yPading = 0;

    //attching the input for the app so we can send input from our app
    remoteThreadId = GetWindowThreadProcessId(wi.hwnd, 0);

    // storing the pos of app and size
    GetWindowRect(wi.hwnd, &screenRect);
    left = screenRect.left;
    top = screenRect.top;
    width = screenRect.right - screenRect.left;
    height = screenRect.bottom - screenRect.top;

    isInitDone = true;
    return wi.hwnd;
}

bool Communication::isFullscreen(HWND windowHandle)
{
    MONITORINFO monitorInfo = { 0 };
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

    RECT windowRect;
    GetWindowRect(windowHandle, &windowRect);

    return windowRect.left == monitorInfo.rcMonitor.left
        && windowRect.right == monitorInfo.rcMonitor.right
        && windowRect.top == monitorInfo.rcMonitor.top
        && windowRect.bottom == monitorInfo.rcMonitor.bottom;
}

void Communication::openPuzzle(std::string puzzlePath) const
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::string  command = puzzlePath + "\\java_vm\\bin\\java.exe" + " -jar getdown-dop.jar .";
    auto currentDir = std::filesystem::current_path();
    std::filesystem::current_path(puzzlePath);

    if (!CreateProcessA(NULL, (char*)command.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi))
        return;

    std::filesystem::current_path(puzzlePath);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    Sleep(2000);
}

DWORD Communication::getJavaProcId(const std::unordered_map<DWORD, bool>& procIds) const
{
    PROCESSENTRY32 pe32 = { 0 };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return 0;
    pe32.dwSize = sizeof(pe32);
    if (!Process32First(hSnap, &pe32))
        return 0;
    do
    {
        if (!wcscmp(L"javaw.exe", pe32.szExeFile) && procIds.find(pe32.th32ProcessID) == procIds.end())
        {
            CloseHandle(hSnap);
            return pe32.th32ProcessID;
        }

    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return 0;
}

BOOL Communication::getHWND(HWND hwnd, LPARAM lParam)
{
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    auto wi = reinterpret_cast<WindowInfo*>(lParam);
    if (lpdwProcessId == wi->prcoessId)
    {
        wi->hwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}