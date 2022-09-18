#include "manager.h"

namespace Manager
{
	std::unordered_map<HWND, Communication*> instances;

	bool isValidKey(HWND key)
	{
		return instances.find(key) != instances.end();
	}
	
	void errorMsg()
	{
		Utils::printMsg("key is not valid \n");
	}

	void AddInstance(HWND key, Communication* value)
	{
		instances[key] = value;
	}

	Communication * getInstance(HWND key)
	{
		return isValidKey(key) ? instances[key] : 0;
	}

	void init()
	{
		Overlay::init(Communication::whd.instance);
	}

	void cleanUp()
	{
		for (auto& instance: instances)
			delete instance.second;
		
		Overlay::cleanUp();
	}

	void removeInstace(HWND key)
	{
		if(isValidKey(key)) 
		{
			instances[key]->removeInstance();
			delete instances[key];
			instances.erase(key);
		}
		else errorMsg();
	}

	void mouseClick(HWND key, int x, int y, bool rightClick)
	{
		isValidKey(key) ? instances[key]->sendMouseClick(x, y, rightClick) : errorMsg();
	}
	
	void sendKeyPress(HWND key, char keyStroke)
	{
		isValidKey(key) ? instances[key]->sendKeyPress(keyStroke) : errorMsg();
	}
	
	void sendKeyDown(HWND key, char keyStroke)
	{
		isValidKey(key) ? instances[key]->sendKeyDown(keyStroke) : errorMsg();
	}
	
	void sendKeyUp(HWND key, char keyStroke)
	{
		isValidKey(key) ? instances[key]->SendKeyUp(keyStroke) : errorMsg();
	}
	
	void sendChar(HWND key, char chr)
	{
		isValidKey(key) ? instances[key]->sendChar(chr) : errorMsg();
	}
	
	void sendString(HWND key, const char* str)
	{
		isValidKey(key) ? instances[key]->sendString(std::string(str)) : errorMsg();
	}
	
	void hide(HWND key)
	{
		isValidKey(key) ? instances[key]->hide() : errorMsg();
	}
	
	void show(HWND key)
	{
		isValidKey(key) ? instances[key]->show() : errorMsg();
	}
	
	void removeTitleBar(HWND key)
	{
		isValidKey(key) ? instances[key]->removeTitleBar() : errorMsg();
	}
	
	void restoreTitleBar(HWND key)
	{
		isValidKey(key) ? instances[key]->restoreTitleBar() : errorMsg();
	}
	
	bool isFullScreen(HWND key)
	{
		if (isValidKey(key))  return instances[key]->isFullscreen();
		else
		{
			errorMsg();
			return false;
		}
	}
	
	void attachToWindow(HWND key, HWND attachTo)
	{
		isValidKey(key) ? instances[key]->attachToWindow(attachTo) : errorMsg();
	}
	
	HWND findWindow(const char* str)
	{
		return FindWindowA(0, str);
	}

	void updateRectWindow(HWND key, int x, int y, int width, int height)
	{
		isValidKey(key) ? instances[key]->setWindowRect(x, y, width, height) : errorMsg();
	}
}

