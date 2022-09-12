#include"automation/manager.h"

//bilge pos:  width /2 - 350  height / 2 - 250  size: 270 540
int main(void)
{
    PuzzlePirateAutomation c("C:\\games\\Puzzle Pirates", CreateType::CREATE_PROCESS);

    //c.login("danielkun2", "Aa123456!", 1);
    //return 0;
    HWND a = c.getHWND();
    SetWindowLong(a, GWL_EXSTYLE, GetWindowLong(a, GWL_EXSTYLE) | WS_EX_NOACTIVATE | WS_EX_APPWINDOW);

    c.hide();
    Sleep(1000);
    c.show();
    Sleep(1000);

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
    return 0;
}




