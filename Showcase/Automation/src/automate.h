#pragma once
#include"communication.h"

enum NamedPoint
{
    RETURN_TO_GAME,
    TIP_OFF,

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


struct ScreenPoint
{
    ScreenPoint() :x(0), y(0) {}
    ScreenPoint(int x, int y) :x(x), y(y) {}
    int x;
    int y;
};


class Automate : public Communication
{
public:
    Automate() = default;
    HWND Init(std::string puzzlePath, bool attach = false);
    void login(const std::string& userName,const std::string& password, int whichPirate = FIRST_PIRATE) const;
    void createAccount(const std::string& outputFile, const std::string& password);
    void clickAtScreenPoint(int namedPoint, bool rightClick = false) const;
private:
    void addScreenPoint();
    std::string genreateString(int length);
    std::unordered_map<int, ScreenPoint> msp;
};

