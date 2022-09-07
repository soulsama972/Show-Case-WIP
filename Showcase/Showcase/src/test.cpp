#include"manager/manager.h"


//bilge pos:  width /2 - 350  height / 2 - 250  size: 270 540
#ifdef _DEBUG
int main(void)
{

    // HWND no = FindWindowA(0, "made by michael b");
    // PuzzlePirateAutomation com("C:\\games\\Puzzle Pirates", true);
    //com.attachToWindow(no);
    //com.removeTitleBar();
    //SetFocus(no);
    //bool a = SetWindowPos(com.getHWND(),0, 0, 0, 0, 0, SWP_NOSIZE);
    //com.hide();
    //com.sendChar('1');
    //HWND a = Manager::createInstace("", BROADCAST);
    //Sleep(3000);
    //Manager::sendKeyPress(a, 'A');
    //Manager::login(a, "danielkun2", "Aa123456!", 1);

    HWND a = Manager::createInstace("C:\\games\\Puzzle Pirates", CreateType::CREATE_PROCESS);
    Sleep(3000);
    Manager::login(a,"danielkun2", "Aa123456!", 1);
    Sleep(1000);
   //com.sendMouseClick(614, 280);
   //com.clickAtScreenPoint(FIRST_PIRATE);
   //com.show();
   //com.minGame();

   //com.createAccount("C:\\games\\Puzzle Pirates\\accounts.txt", "Aa123456!");

   //com.Init("C:\\Users\\a\\Desktop\\PP\\Puzzle Pirates", true);
   //com.minGame();

   //com.createAccount("C:\\Users\\a\\Desktop\\PP\\Puzzle Pirates\\accounts.txt", "Aa123456!");

   //com.login("soul972","Aa123456!");
   // 
   //Sleep(5000);
   ////com.clickAtScreenPoint(TIP_OFF);
   //com.clickAtScreenPoint(YE);
   //Sleep(100);
   //com.clickAtScreenPoint(NOTICE_BOARD);
   //Sleep(2000);
   //com.clickAtScreenPoint(PUZZLE);
   //Sleep(200);
   //com.clickAtScreenPoint(BILGE_PUZZLE);
   //Sleep(200);
   //com.clickAtScreenPoint(PLAY_NOW_PUZZLE);
   //Sleep(200);
   //Sleep(100);
   //com.clickAtScreenPoint(BOTTOM_RIGHT_CONER_VIEW_AREA, true);
   ////com.killGame();
    Sleep(10);
    return 0;
}

#endif // DEBUG


