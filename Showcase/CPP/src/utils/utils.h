#pragma once
#include<Windows.h>
#include<iostream>
#include<string>
#include<algorithm>

struct WindowInfo
{
    DWORD prcoessId;
    HWND hwnd;
    bool attach;
};

namespace Utils
{
	BOOL CALLBACK getHWND(HWND hwnd, LPARAM lParam);
    std::string genreateString(int length);
}

