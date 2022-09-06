#include "manager.h"

namespace Manager
{
	std::unordered_map<HWND, Automate> instances;

	bool isValidKey(HWND key)
	{
		return instances.find(key) != instances.end();
	}

	HWND createInstace(const char* puzzlePiratePath, const char* userName, const char* password, int pirateNumber, bool login)
	{
		Automate inst(std::string(puzzlePiratePath), CREATE_PROCESS);
		if (login) inst.login(std::string(userName), std::string(password), pirateNumber);

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
		if(isValidKey(key))
			instances[key].sendMouseClick(x, y, rightClick);
	}
	void sendKeyPress(HWND key, char keyStroke)
	{
		if (isValidKey(key))
			instances[key].sendKeyPress(keyStroke);
	}
	void sendKeyDown(HWND key, char keyStroke)
	{
		if (isValidKey(key)) instances[key].SendKeyDown(keyStroke);
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
}


