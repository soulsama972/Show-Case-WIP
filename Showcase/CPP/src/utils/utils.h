#pragma once
#include<Windows.h>
#include<iostream>
#include<string>
#include<algorithm>

#define EXPORT extern "C" __declspec(dllexport)

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

