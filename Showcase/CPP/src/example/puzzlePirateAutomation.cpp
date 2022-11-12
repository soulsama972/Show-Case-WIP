#include"puzzlePirateAutomation.h"

PuzzlePirateAutomation::PuzzlePirateAutomation(const std::string& puzzlePath, WindowData*& winData, uint32_t procId, CreateType type)
{
    init(puzzlePath, winData, procId, type);
}

HWND PuzzlePirateAutomation::init(const std::string& puzzlePath, WindowData*& winData, uint32_t procId, CreateType type)
{
    path = puzzlePath;
    return Communication::init(winData, procId, type);
}

void PuzzlePirateAutomation::login(const std::string& userName, const std::string& password, int whichPirate) const
{
    sendClickAtScreenPoint(ACCOUNT_NAME);

    // we need to remove the previus user name by press backspace
    for (int i = 0; i < 50; i++)
        sendKeyPress(VK_BACK);

    sendString(userName);
    sendKeyPress(VK_TAB);

    sendString(password);

    sendKeyPress(VK_RETURN);

    Sleep(2000);
    sendClickAtScreenPoint(FIRST_PIRATE + whichPirate - 1);
}

void PuzzlePirateAutomation::createAccount(const std::string& outputFile, const std::string& password) const
{
    std::fstream out(outputFile, std::fstream::app);
    if (!out.is_open()) return;

    sendClickAtScreenPoint(CREATE_ACCOUNT);
    Sleep(2000);
    
    std::string randomUserName = Utils::genreateString(8);
    std::string randomPirateName = Utils::genreateString(8);

    sendString(randomUserName);
    sendKeyPress(VK_TAB);

    sendString(password);
    sendKeyPress(VK_TAB);

    sendString(password); // confirm password
    sendKeyPress(VK_TAB);

    sendString(randomUserName + "@gmail.com");
    sendKeyPress(VK_TAB);

    sendKeyPress(VK_SPACE);

    Sleep(3000);
    sendString(randomPirateName);
    Sleep(100);
    for (int i = 0; i < 10; i ++)
    {
        sendClickAtScreenPoint(CREATE_ACCOUNT_DAY);
        Sleep(1000);
    }

    out << randomUserName << std::endl;
    out.close();
}

WindowInfo PuzzlePirateAutomation::openProcess()
{
    std::unordered_map<DWORD, bool> procIds;
    DWORD procId = 0;
    WindowInfo wi = { 0 };
    
    auto createProcess = [](const std::string& path)
    {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string  command = path + "\\java_vm\\bin\\java.exe" + " -jar getdown-dop.jar .";
        auto currentDir = std::filesystem::current_path();
        std::filesystem::current_path(path);

        if (!CreateProcessA(NULL, (char*)command.c_str(), NULL, NULL, false, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
            return;

        std::filesystem::current_path(path);

        

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        Sleep(2000);
    };
    
    do
    {
        procId = getJavaProcId(procIds);
        procIds[procId] = true;

    } while (procId);

    createProcess(path);

    wi.prcoessId = getJavaProcId(procIds);
    EnumWindows(Utils::getHWND, (LPARAM)&wi);
    return wi;

}

void PuzzlePirateAutomation::updateScreenPoint(std::unordered_map<int, ScreenPoint>& msp, int left, int top, int width, int height)
{
    msp[RETURN_TO_GAME] = ScreenPoint(width - 175, 11);
    msp[TIP_OFF] = ScreenPoint(width - 130, 514);
    msp[CREATE_ACCOUNT] = ScreenPoint(20, 15);
    msp[LOGIN] = ScreenPoint(width / 2, height / 2 + 100);
    msp[FIRST_PIRATE] = ScreenPoint(width / 2 - 150, height / 2);
    msp[SEC_PIRATE] = ScreenPoint(width / 2, height / 2);
    msp[THIRD_PIRATE] = ScreenPoint(width / 2 + 150, height / 2);

    msp[TEXT_BOX] = ScreenPoint(150, height - 5);
    msp[GO_HOME] = ScreenPoint(width - 80, height - 200);
    msp[NOTICE_BOARD] = ScreenPoint(width - 80, height - 210);

    msp[BOOTY] = ScreenPoint(width - 30, 210);
    msp[YE] = ScreenPoint(width - 80, 210);
    msp[ISLAND] = ScreenPoint(width - 150, 210);
    msp[CREW] = ScreenPoint(width - 150, 240);
    msp[AHOY] = ScreenPoint(width - 30, 240);

    for (int i = NEWS; i <= BLOACKADES; i++)
        msp[i] = ScreenPoint(50 + (i - NEWS) * 70, 50);

    for (int i = BILGE_PUZZLE; i <= PATCHING_PUZZLE; i++)
        msp[i] = ScreenPoint(width / 2 - 366 + ((i - BILGE_PUZZLE) * 33), 270);

    msp[PLAY_NOW_PUZZLE] = ScreenPoint(width / 2 + 60, 450);
    msp[PLAY_PUZZLE_TUTORIAL] = ScreenPoint(width / 2 - 200, height / 2 - 160);

    msp[CREATE_ACCOUNT_DAY] = ScreenPoint(width / 2 + 60, height / 2 + 50);
    //msp[MONTH] = ScreenPoint(width / 2 + 200, height / 2 - 160);
    //msp[YEAR] = ScreenPoint(width / 2 + 200, height / 2 - 160);

    msp[BOTTOM_RIGHT_CONER_VIEW_AREA] = ScreenPoint(width - 200, height - 80);

    msp[ACCOUNT_NAME] = ScreenPoint(width / 2, height / 2);
}

DWORD PuzzlePirateAutomation::getJavaProcId(const std::unordered_map<DWORD, bool>& ignoreProcList) const
{
    PROCESSENTRY32W pe32 = { 0 };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return 0;
    pe32.dwSize = sizeof(pe32);
    if (!Process32FirstW(hSnap, &pe32))
        return 0;
    do
    {
        if (!wcscmp(L"javaw.exe", pe32.szExeFile) && ignoreProcList.find(pe32.th32ProcessID) == ignoreProcList.end())
        {
            CloseHandle(hSnap);
            return pe32.th32ProcessID;
        }

    } while (Process32NextW(hSnap, &pe32));
    CloseHandle(hSnap);
    return 0;
}


namespace Manager
{
    HWND createInstace(const char* puzzlePath, WindowData*& winData, uint32_t procId, CreateType type)
    {
        PuzzlePirateAutomation * inst = new PuzzlePirateAutomation(std::string(puzzlePath), winData, procId, type);
        HWND key = inst->getHWND();
        AddInstance(key, inst);
        return key;
    }

    void login(HWND key, const char* userName, const char* password, int pirateNumber)
    {
        auto instance = dynamic_cast<PuzzlePirateAutomation*>(getInstance(key));
        if(instance) instance->login(userName, password, pirateNumber);

    }
}


BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
			// AllocConsole();
			// freopen("CONOUT$", "w", stdout);
			whd.instance = (HMODULE)hModule;
			srand((unsigned)time(NULL));

            whd.hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE, 0, sizeof(WindowData), WINDATASMNAME);
			if (!whd.hMapFile) Utils::printMsg("Could not create file mapping object (%ld).\n",GetLastError());
                    
            whd.wd = (WindowData*)MapViewOfFile(whd.hMapFile, FILE_MAP_ALL_ACCESS,0,0, 0);
            if (!whd.wd)
            {
                Utils::printMsg("Could not map view of file (%ld).\n",GetLastError());
                CloseHandle(whd.hMapFile);
                return false;
            }
            memset(whd.wd, 0, sizeof(WindowData));
        }
        break;
    }
    return TRUE;
}