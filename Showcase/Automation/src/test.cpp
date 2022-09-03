#include"automate.h"



std::unordered_map<HWND, Communication> coms;



//bilge pos:  width /2 - 350  height / 2 - 250  size: 270 540
int main(void)
{
    Automate com;
    //HWND hwnd = FindWindowA("SunAwtFrame", 0);
   //.\java_vm\bin\javaw.exe -classpath "F:\games\Puzzle Pirates\.\code\config.jar;
   com.Init("C:\\games\\Puzzle Pirates", true);
   //com.minGame();
   
   //com.createAccount("F:\\games\\Puzzle Pirates\\accounts.txt", "Aa123456!");

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
   Sleep(100);
   com.clickAtScreenPoint(BOTTOM_RIGHT_CONER_VIEW_AREA, true);
   ////com.killGame();
   return 0;
}

