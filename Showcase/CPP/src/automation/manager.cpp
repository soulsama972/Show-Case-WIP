#include "manager.h"

namespace Manager
{
	std::unordered_map<HWND, PuzzlePirateAutomation> instances;

	bool isValidKey(HWND key)
	{
		return instances.find(key) != instances.end();
	}
	
	void errorMsg()
	{
		printf("key is not valid \n");
	}

	HWND createInstace(const char* puzzlePiratePath, CreateType type)
	{
		PuzzlePirateAutomation inst(std::string(puzzlePiratePath), type);
		HWND key = inst.getHWND();
		instances[key] = inst;
		return key;
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
	
	void login(HWND key, const char* userName, const char* password, int pirateNumber)
	{
		isValidKey(key) ? instances[key].login(userName, password, pirateNumber) : errorMsg();
	}
	
	HWND findWindow(const char* str)
	{
		return FindWindowA(0, str);
	}

	void updateRectWindow(HWND key, int x, int y, int width, int height)
	{
		isValidKey(key) ? instances[key].setWindowRect(x, y, width, height) : errorMsg();
	}

}


