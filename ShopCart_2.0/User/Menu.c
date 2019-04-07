

#include "Menu.h"


/*
   0 : System_Menu
   1 : Home_Menu
   2 : Check_Menu
   3 : Pay_Menu
   4 : Manager_Menu
   5 : ShowGoods_Menu
   6 ：Check_PayCode
*/
u8 Display_Index = 0;

u8 Flag_Display = 0;


void Display_Menu(void)
{
	switch(Display_Index)
	{
		case 0 : System_Menu();    break;
		case 1 : Home_Menu(); 	   break;
		case 2 : Check_Menu();	   break;
		case 3 : Pay_Menu();       break;
		case 4 : Manager_Menu();   break;
		case 5 : ShowGoods_Menu(); break;
		case 6 : Check_PayCode();  break;
	}
}


static void Time_Mark(void)
{
	u8 String_1[50];

	LCD_SetFont(&Font8x16);
	LCD_SetColors(BLUE2,WHITE);
	sprintf((char *)String_1, "             Time : %0.2d:%0.2d:%0.2d", THH, TMM, TSS);
	ILI9341_DispStringLine_EN(LINE(0),(char *)String_1);
}


static void System_Menu(void)
{
	u8 String_1[30];
	
	
	Time_Mark();
	LCD_SetFont(&Font16x24);
	LCD_SetColors(RED,WHITE);
	sprintf((char *)String_1, "    [  Customer ]");
	ILI9341_DispStringLine_EN(LINE(4),(char *)String_1);
	sprintf((char *)String_1, "    [  Manager  ]");
	ILI9341_DispStringLine_EN(LINE(6),(char *)String_1);
	
}

static void Home_Menu(void)
{
	u8 String_1[50];


	Time_Mark();
	LCD_SetFont(&Font8x16);
	LCD_SetColors(BRED,WHITE);			//粉红
	if(MyUser.Attributes == Customer)
		sprintf((char *)String_1, "              [ Customer ]");
	ILI9341_DispStringLine_EN(LINE(2),(char *)String_1);
	
	
	LCD_SetColors(BLUE,WHITE);
	sprintf((char *)String_1, " ShopCart @ Value  -> $%d", Get_ShopCart_Value(ShopCart) );
	ILI9341_DispStringLine_EN(LINE(4),(char *)String_1);
	sprintf((char *)String_1, " ShopCart @ Number ->  %d", Get_ShopCart_Number(ShopCart) );
	ILI9341_DispStringLine_EN(LINE(6),(char *)String_1);
	
	
	LCD_SetColors(RED,WHITE);
	sprintf((char *)String_1, " < ^ Delete ^ >");
	ILI9341_DispStringLine_EN(LINE(9),(char *)String_1);
	
	LCD_SetColors(RED,WHITE);
	sprintf((char *)String_1, " < ^ Pay ^ >");
	ILI9341_DispStringLine_EN(LINE(12),(char *)String_1);
	
			
}


static void Check_Menu(void)
{
	u8 String_1[30];
	
	LCD_SetFont(&Font16x24);
	LCD_SetColors(RED,WHITE);

	sprintf((char *)String_1, "      GET GOODS!");
	ILI9341_DispStringLine_EN(LINE(3),(char *)String_1);
	LCD_SetColors(BLUE,WHITE);
	sprintf((char *)String_1, "      BUY or RET");
	ILI9341_DispStringLine_EN(LINE(6),(char *)String_1);
	
}


static void Check_PayCode(void)
{
	
}

static void Pay_Menu(void)
{
	u8 String_1[30];
	u8 Display_Offset;
	
	Time_Mark();
	ShowGoods_Menu();
	
	Display_Offset = Get_ShopCart_HeadGoods_Count(ShopCart);
	if(Display_Offset == 0)
	{
		LCD_SetFont(&Font8x16);
		LCD_SetColors(BLUE2,WHITE);
		sprintf((char *)String_1, "      ShopCart @ Value  ->  $%d", Get_ShopCart_Value(ShopCart) );
		ILI9341_DispStringLine_EN(LINE(8),(char *)String_1);
		sprintf((char *)String_1, "      ShopCart @ Number ->   %d", Get_ShopCart_Number(ShopCart) );
		ILI9341_DispStringLine_EN(LINE(9),(char *)String_1);
	}
	else
	{
		LCD_SetFont(&Font8x16);
		LCD_SetColors(BLUE2,WHITE);
		sprintf((char *)String_1, " ShopCart @ Value  ->  $%d", Get_ShopCart_Value(ShopCart) );
		ILI9341_DispStringLine_EN(LINE(5 + Display_Offset),(char *)String_1);
		sprintf((char *)String_1, " ShopCart @ Number ->   %d", Get_ShopCart_Number(ShopCart) );
		ILI9341_DispStringLine_EN(LINE(6 + Display_Offset),(char *)String_1);
	}		

}


static void Manager_Menu(void)
{
	u8 String_1[30];
	
	
	Time_Mark();
	LCD_SetFont(&Font8x16);
	LCD_SetColors(BRED,WHITE);			//粉红
	if(MyUser.Attributes == Manager)
		sprintf((char *)String_1, "              [ Manager ]");
	ILI9341_DispStringLine_EN(LINE(3),(char *)String_1);
	
	LCD_SetFont(&Font16x24);
	LCD_SetColors(RED,WHITE);
	sprintf((char *)String_1, "    - Scanning  -");
	ILI9341_DispStringLine_EN(LINE(4),(char *)String_1);
	sprintf((char *)String_1, "    - To Clinet -");
	ILI9341_DispStringLine_EN(LINE(6),(char *)String_1);
}


static void ShowGoods_Menu(void)
{
	u8 String_1[30];
	u8 Goods_Count = Get_ShopCart_HeadGoods_Count(ShopCart);					//获取购物车当前商品数目，确定需要显示几条商品信息;
	__Commodity Goods_Cnt = Get_ShopCart_HeadGoods(ShopCart)->next;				//获取购物车中第一个商品结点
	
	
	Time_Mark();
	LCD_SetFont(&Font8x16);
	LCD_SetColors(BRED,WHITE);			//粉红
	sprintf((char *)String_1, "              < Goods List >");
	ILI9341_DispStringLine_EN(LINE(2),(char *)String_1);
	
	if(Goods_Count == 0)					//当前购物车没有商品
	{
		LCD_SetColors(BLUE,WHITE);			//
		sprintf((char *)String_1, "      There is no goods in ShopCart!");
		ILI9341_DispStringLine_EN(LINE(5),(char *)String_1);
	}else
	{
		u8 Display_Offset;	//显示行数偏移量
		
		
		for(Display_Offset = 0;Display_Offset < Goods_Count;Display_Offset++)
		{
			if(Display_Offset == Del_Index - 1)									//切换选择需要删除的商品
			{
				LCD_SetColors(RED,WHITE);
				strcpy((char *)Out_Code, (char *)Goods_Cnt->Serial_Number);		//获取选中的商品的条码
			}
			else
				LCD_SetColors(BLUE2,WHITE);
			sprintf((char *)String_1, "   %d. %s  ->  $%d x%d", (Display_Offset+1), Get_Goods_Name(Goods_Cnt), Get_Goods_Price(Goods_Cnt), Get_Goods_Count(Goods_Cnt));	
			ILI9341_DispStringLine_EN(LINE(4 + Display_Offset),(char *)String_1);
			Goods_Cnt = Goods_Cnt->next;
		}
		
	}
	
	
}
