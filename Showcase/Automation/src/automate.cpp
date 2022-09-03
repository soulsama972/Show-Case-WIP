#include"automate.h"
#include<fstream>

HWND Automate::Init(std::string puzzlePath, bool attach)
{
    HWND hwnd = Communication::Init(puzzlePath, attach);
    addScreenPoint();

    static bool once = true;
    if (once)
    {
        srand((unsigned)time(NULL));
        once = false;
    }

    return hwnd;
}

void Automate::login(const std::string& userName, const std::string& password, int whichPirate) const
{
    Sleep(1000);
    setFocus();
    sendKeyPress(VK_TAB);

    // we need to remove the previus user name by press backspace
    for (int i = 0; i < 100; i++)
        sendKeyPress(VK_BACK);

    sendString(userName);
    sendKeyPress(VK_TAB);

    sendString(password);

    sendKeyPress(VK_RETURN);

    Sleep(3000);
    clickAtScreenPoint(whichPirate);
}

void Automate::createAccount(const std::string& outputFile, const std::string& password)
{
    std::fstream out(outputFile, std::fstream::app);
    if (!out.is_open()) return;

    clickAtScreenPoint(CREATE_ACCOUNT);
    Sleep(1500);
    
    std::string randomUserName = genreateString(8);
    std::string randomPirateName = genreateString(8);

    sendString(randomUserName);
    sendKeyPress(VK_TAB);

    sendString(password);
    sendKeyPress(VK_TAB);

    sendString(password); // confirm password
    sendKeyPress(VK_TAB);

    sendString(randomUserName + "@walla.com");
    sendKeyPress(VK_TAB);

    sendKeyPress(VK_SPACE);

    Sleep(3000);
    sendString(randomPirateName);
    Sleep(100);
    for (int i = 0; i < 10; i ++)
    {

        clickAtScreenPoint(CREATE_ACCOUNT_DAY);
        Sleep(1000);
    }

    //out << randomUserName << std::endl;
    //out.close();
}

void Automate::clickAtScreenPoint(int namedPoint, bool rightClick) const
{
    if (msp.find(namedPoint) != msp.end())
    {
        auto sp = msp.at(namedPoint);
        sendMouseClick(sp.x, sp.y, rightClick);
    }
}

void Automate::addScreenPoint()
{
    //adding all the points that we need inorder to preform automation
    msp[RETURN_TO_GAME] = ScreenPoint(width - 175, 11);
    msp[TIP_OFF] = ScreenPoint(width - 130, 514);
    msp[CREATE_ACCOUNT] = ScreenPoint(20, 15);
    msp[LOGIN] = ScreenPoint(width / 2, height / 2 + 100);
    msp[FIRST_PIRATE] = ScreenPoint(width / 2 - 150, height / 2);
    msp[SEC_PIRATE] = ScreenPoint(width / 2, height / 2);
    msp[THIRD_PIRATE] = ScreenPoint(width / 2 + 150, height / 2);

    msp[TEXT_BOX] = ScreenPoint(150, height - 5);
    msp[GO_HOME] = ScreenPoint(width - 80, height - 200);
    msp[NOTICE_BOARD] = ScreenPoint(width - 80, height - 210);

    msp[BOOTY] = ScreenPoint(width - 30, 210);
    msp[YE] = ScreenPoint(width - 80, 210);
    msp[ISLAND] = ScreenPoint(width - 150, 210);
    msp[CREW] = ScreenPoint(width - 150, 240);
    msp[AHOY] = ScreenPoint(width - 30, 240);

    for (int i = NEWS; i <= BLOACKADES; i++)
        msp[i] = ScreenPoint(50 + (i - NEWS) * 70, 50);

    for (int i = BILGE_PUZZLE; i <= PATCHING_PUZZLE; i++)
        msp[i] = ScreenPoint(width / 2 - 366 + ((i - BILGE_PUZZLE) * 33), 270);

    msp[PLAY_NOW_PUZZLE] = ScreenPoint(width / 2 + 60, 450);
    msp[PLAY_PUZZLE_TUTORIAL] = ScreenPoint(width / 2 - 200, height / 2 - 160);

    msp[CREATE_ACCOUNT_DAY] = ScreenPoint(width / 2 + 60, height / 2 + 50);
    //msp[MONTH] = ScreenPoint(width / 2 + 200, height / 2 - 160);
    //msp[YEAR] = ScreenPoint(width / 2 + 200, height / 2 - 160);

    msp[BOTTOM_RIGHT_CONER_VIEW_AREA] = ScreenPoint(width - 200, height - 80);
}

std::string Automate::genreateString(int length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    str[0] = 'f';
    std::generate_n(str.begin() + 1, length - 1, randchar);
    return str;
}
