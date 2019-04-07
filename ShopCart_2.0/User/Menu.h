

#ifndef __MENU_H
#define __MENU_H

#include "IO.h"



/*
   0 : System_Menu
   1 : Home_Menu
   2 : Check_Menu
   3 : Pay_Menu
   4 : Manager_Menu
   5 : ShowGoods_Menu
*/
extern u8 Display_Index;
extern u8 Flag_Display;


       void Display_Menu(void);
static void Time_Mark(void);
static void System_Menu(void);
static void Home_Menu(void);
static void Check_Menu(void);
static void Pay_Menu(void);
static void Check_PayCode(void);
static void Manager_Menu(void);
static void ShowGoods_Menu(void);


// ƒ„ «≤Ó¿À£ø

#endif
