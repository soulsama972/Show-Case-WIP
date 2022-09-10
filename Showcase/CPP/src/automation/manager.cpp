#include "manager.h"

namespace Manager
{
	std::unordered_map<HWND, PuzzlePirateAutomation> instances;

	bool isValidKey(HWND key)
	{
		return instances.find(key) != instances.end();
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
		if (isValidKey(key)) instances.erase(key);
	}

	void mouseClick(HWND key, int x, int y, bool rightClick)
	{
		if(isValidKey(key)) instances[key].sendMouseClick(x, y, rightClick);
	}
	
	void sendKeyPress(HWND key, char keyStroke)
	{
		if (isValidKey(key)) instances[key].sendKeyPress(keyStroke);
	}
	
	void sendKeyDown(HWND key, char keyStroke)
	{
		if (isValidKey(key)) instances[key].sendKeyDown(keyStroke);
	}
	
	void sendKeyUp(HWND key, char keyStroke)
	{
		if (isValidKey(key)) instances[key].SendKeyUp(keyStroke);
	}
	
	void sendChar(HWND key, char chr)
	{
		if (isValidKey(key)) instances[key].sendChar(chr);
	}
	
	void sendString(HWND key, const char* str)
	{
		if (isValidKey(key)) instances[key].sendString(std::string(str));

	}
	
	void hide(HWND key)
	{
		if (isValidKey(key)) instances[key].hide();
	}
	
	void show(HWND key)
	{
		if (isValidKey(key)) instances[key].show();
	}
	
	void killGame(HWND key)
	{
		if (isValidKey(key)) instances[key].killGame();
	}
	
	void removeTitleBar(HWND key)
	{
		if (isValidKey(key)) instances[key].removeTitleBar();
	}
	
	void restoreTitleBar(HWND key)
	{
		if (isValidKey(key)) instances[key].restoreTitleBar();
	}
	
	bool isFullScreen(HWND key)
	{
		if (isValidKey(key)) return instances[key].isFullscreen();
		return false;
	}
	
	void attachToWindow(HWND key, HWND attachTo)
	{
		if (isValidKey(key)) instances[key].attachToWindow(attachTo);
	}
	
	void login(HWND key, const char* userName, const char* password, int pirateNumber)
	{
		if (isValidKey(key)) instances[key].login(userName, password, pirateNumber);
	}
	
	bool getMousePos(HWND key, int& x, int& y)
	{
		if (!isValidKey(key)) return false;

		instances[key].getMousePos(x, y);
		return true;
	}
	
	HWND findWindow(const char* str)
	{
		return FindWindowA(0, str);
	}

	void updateRectWindow(HWND key, int x, int y, int width, int height)
	{
		if (isValidKey(key)) instances[key].setWindowRect(x, y, width, height);
	}

	void updateImage(HWND key, HWND target, int x, int y)
	{
		if (isValidKey(key)) instances[key].updateImage(target, x, y);
	}

}


