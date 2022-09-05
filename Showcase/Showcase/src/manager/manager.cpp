#include "manager.h"

namespace Manager
{
	std::unordered_map<HWND, Automate> instances;


	HWND addInstace(const char* puzzlePiratePath, const char* userName, const char* password, int pirateNumber, bool login)
	{
		Automate inst(std::string(puzzlePiratePath), false);
		if (login) inst.login(std::string(userName), std::string(password), pirateNumber);

		HWND key = inst.getHWND();
		instances[key] = inst;

		return key;
	}
}


