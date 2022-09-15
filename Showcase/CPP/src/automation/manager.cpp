#include "manager.h"

namespace Manager
{
	std::unordered_map<HWND, Communication> instances;

	bool isValidKey(HWND key)
	{
		return instances.find(key) != instances.end();
	}
	
	void errorMsg()
	{
		Utils::printMsg("key is not valid \n");
	}

	void AddInstance(HWND key, Communication value)
	{
		instances[key] = value;
	}

	Communication * getInstance(HWND key)
	{
		return isValidKey(key) ? &instances[key] : 0;
	}

	void removeInstace(HWND key)
	{
		if(isValidKey(key)) instances.erase(key);
		else errorMsg();
	}

	void mouseClick(HWND key, int x, int y, bool rightClick)
	{
		isValidKey(key) ? instances[key].sendMouseClick(x, y, rightClick) : errorMsg();
	}
	
	void sendKeyPress(HWND key, char keyStroke)
	{
		isValidKey(key) ? instances[key].sendKeyPress(keyStroke) : errorMsg();
	}
	
	void sendKeyDown(HWND key, char keyStroke)
	{
		isValidKey(key) ? instances[key].sendKeyDown(keyStroke) : errorMsg();
	}
	
	void sendKeyUp(HWND key, char keyStroke)
	{
		isValidKey(key) ? instances[key].SendKeyUp(keyStroke) : errorMsg();
	}
	
	void sendChar(HWND key, char chr)
	{
		isValidKey(key) ? instances[key].sendChar(chr) : errorMsg();
	}
	
	void sendString(HWND key, const char* str)
	{
		isValidKey(key) ? instances[key].sendString(std::string(str)) : errorMsg();
	}
	
	void hide(HWND key)
	{
		isValidKey(key) ? instances[key].hide() : errorMsg();
	}
	
	void show(HWND key)
	{
		isValidKey(key) ? instances[key].show() : errorMsg();
	}
	
	void killGame(HWND key)
	{
		isValidKey(key) ? instances[key].killGame() : errorMsg();
	}
	
	void removeTitleBar(HWND key)
	{
		isValidKey(key) ? instances[key].removeTitleBar() : errorMsg();
	}
	
	void restoreTitleBar(HWND key)
	{
		isValidKey(key) ? instances[key].restoreTitleBar() : errorMsg();
	}
	
	bool isFullScreen(HWND key)
	{
		if (isValidKey(key)) 
		{
			return instances[key].isFullscreen();
		}
		else
		{
			errorMsg();
			return false;
		}
	}
	
	void attachToWindow(HWND key, HWND attachTo)
	{
		isValidKey(key) ? instances[key].attachToWindow(attachTo) : errorMsg();
	}
	
	HWND findWindow(const char* str)
	{
		return FindWindowA(0, str);
	}

	void updateRectWindow(HWND key, int x, int y, int width, int height)
	{
		isValidKey(key) ? instances[key].setWindowRect(x, y, width, height) : errorMsg();
	}

	void drawText(HWND key, const char* text,int x, int y, int color,int fontSize)
	{
		isValidKey(key) ? instances[key].drawText(key, std::string(text), x, y, color, fontSize) : errorMsg();
	}

	void drawRect(HWND key, int x, int y, int w, int h, int color)
	{
		isValidKey(key) ? instances[key].drawRect(key, x, y, w, h, color) : errorMsg();
	}

}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	WindowHookData* whd = &Communication::whd;
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
			// AllocConsole();
			// freopen("CONOUT$", "w", stdout);
			whd->instance = (HMODULE)hModule;
			srand((unsigned)time(NULL));

            whd->hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE, 0, sizeof(WindowData), WINDATASMNAME);
			if (!whd->hMapFile) Utils::printMsg("Could not create file mapping object (%ld).\n",GetLastError());
                    
            whd->wd = (WindowData*)MapViewOfFile(whd->hMapFile, FILE_MAP_ALL_ACCESS,0,0, 0);
            if (!whd->wd)
            {
                Utils::printMsg("Could not map view of file (%ld).\n",GetLastError());
                CloseHandle(whd->hMapFile);
                return false;
            }
            memset(whd->wd, 0, sizeof(WindowData));
        }
        break;
    }
    return TRUE;
}