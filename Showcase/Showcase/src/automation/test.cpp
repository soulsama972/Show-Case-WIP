#include"automate.h"

std::unordered_map<HWND, Communication> coms;

//bilge pos:  width /2 - 350  height / 2 - 250  size: 270 540
int main(void)
{
    
    HWND no = (HWND)0x001A0554;
    Automate com("C:\\games\\Puzzle Pirates", true);

   com.attachToWindow(no);
   //SetFocus(no);
   //bool a = SetWindowPos(com.getHWND(),0, 0, 0, 0, 0, SWP_NOSIZE);
   //com.hide();
   //com.sendChar('1');
   com.sendMouseClick(614, 280);
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

