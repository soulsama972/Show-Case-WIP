#pragma once
#include"puzzlePirateAutomation.h"



namespace Manager
{
	EXPORT HWND createInstace(const char* puzzlePiratePath, CreateType type);
	EXPORT void removeInstace(HWND key);
	EXPORT void mouseClick(HWND key, int x, int y, bool rightClick);
	EXPORT void sendKeyPress(HWND key, char keyStroke);
	EXPORT void sendKeyDown(HWND key, char keyStroke);
	EXPORT void sendKeyUp(HWND key, char keyStroke);
	EXPORT void sendChar(HWND key, char chr);
	EXPORT void sendString(HWND key, const char* str);
	EXPORT void hide(HWND key);
	EXPORT void show(HWND key);
	EXPORT void killGame(HWND key);
	EXPORT void removeTitleBar(HWND key);
	EXPORT void restoreTitleBar(HWND key);
	EXPORT bool isFullScreen(HWND key);
	EXPORT void attachToWindow(HWND key, HWND attachTo);
	EXPORT void login(HWND key, const char* userName, const char* password, int pirateNumber);
	EXPORT HWND findWindow(const char * str);
	EXPORT void updateRectWindow(HWND key, int x, int y, int width, int height);
};