#pragma once
#include"automate.h"
#define EXPORT extern "C" __declspec(dllexport)


namespace Manager
{
	EXPORT HWND addInstace(const char* puzzlePiratePath,const char* userName,const char* password, int pirateNumber, bool login);


};