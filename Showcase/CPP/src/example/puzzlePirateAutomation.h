#pragma once
#include"../overlay/overlay.h"
#include"../automation/communication.h"
#include"../automation/manager.h"
#include<TlHelp32.h>
#include<fstream>

enum NamedPoint
{
    RETURN_TO_GAME,
    TIP_OFF,

    ACCOUNT_NAME,
    CREATE_ACCOUNT,
    LOGIN,
    FIRST_PIRATE,
    SEC_PIRATE,
    THIRD_PIRATE,


    CREW,
    ISLAND,
    YE,
    BOOTY,
    AHOY,

    NEWS,
    PUZZLE,
    MISSIONS,
    VOYAGES,
    EVENTS,
    SHOOPE_JOBS,
    BLOACKADES,

    BILGE_PUZZLE,
    SAIL_PUZZLE,
    CARPENTRY_PUZZLE,
    RIGGING_PUZZLE,
    TREASURE_HAEL_PUZZLE,
    GUNNING_PUZZLE,
    NAVI_PUZZLE,
    BNAVI_PUZZLE,
    PATCHING_PUZZLE,
    PLAY_NOW_PUZZLE,
    PLAY_PUZZLE_TUTORIAL,

    GO_HOME,
    NOTICE_BOARD,
    TEXT_BOX,


    CREATE_ACCOUNT_DAY,
    CREATE_ACCOUNT_MONTH,
    CREATE_ACCOUNT_YEAR,


    BOTTOM_RIGHT_CONER_VIEW_AREA,

};

class PuzzlePirateAutomation : public Communication
{
public:
    PuzzlePirateAutomation() = default;
    PuzzlePirateAutomation(const std::string& puzzlePath, WindowData*& winData, CreateType type = CREATE_PROCESS);
    HWND init(const std::string& puzzlePath, WindowData*& winData, CreateType type = CREATE_PROCESS);
    void login(const std::string& userName,const std::string& password, int whichPirate = 1) const;
    void createAccount(const std::string& outputFile, const std::string& password) const;
private:
    WindowInfo openProcess() override;
    WindowInfo attachProcess() override;
    void updateScreenPoint(std::unordered_map<int, ScreenPoint>& msp, int left, int top, int width, int height) override;


    DWORD getJavaProcId(const std::unordered_map<DWORD, bool>& ignoreProcList) const;
    std::string path;
};

namespace Manager
{
    EXPORT HWND createInstace(const char* puzzlePath, WindowData*& winData, CreateType type);
    EXPORT void login(HWND key, const char* userName, const char* password, int pirateNumber);
}