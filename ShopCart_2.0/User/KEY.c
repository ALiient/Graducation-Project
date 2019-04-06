

#include "KEY.h"


u8 Flag_KEY = 0;
u8 KEY_Con = 0;


#define KEY_1 (GPIOA->IDR & 0X01)

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void KEY_Read(void)
{
	static u8 Key_Time[3] = {0};
	KEY_Con = 0;
	
	if(KEY_1 == KEY_PRESS)
	{
		Key_Time[1]++;
		if(Key_Time[1] == 1)
			KEY_Con = 1;
		
		if(Key_Time[1] == 20)
		{
			Key_Time[1] = 15;
			KEY_Con = 3;		//长按（删除界面上回退至Home）
		}
	}else
		Key_Time[1] = 0;


	if(KEY_2_STATUS == KEY_PRESS)
	{
		Key_Time[2]++;
		if(Key_Time[2] == 1)
			KEY_Con = 2;
		
		if(Key_Time[2] == 20)
		{
			Key_Time[2] = 15;
			KEY_Con = 2;
		}
	}else
		Key_Time[2] = 0;

}


void KEY_Control(void)
{

	if(KEY_Con == 1)
	{
		LED_Control(LED1, ON);
		
		if(Display_Index == 0)
		{
			User_Initial(&MyUser, Customer);				//初始化系统用户
			Print_ShopCart(ShopCart);						//打印购物车
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}else if(Display_Index == 2)						//位于询问弹窗，确认购买
		{					
			Buy_Goods(In_Code);								//确认购买
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}else if(Display_Index == 1)						//位于Home，按下跳转至商品列表，等待删除操作
		{
			Display_Index = 5;								//跳往商品列表（删除界面）
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}else if(Display_Index == 3)						//位于结算界面，确认结算
		{
			Pay_ShopCart(ShopCart);							//确认结算购物车
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
			
		}else if(Display_Index == 5)						//位于商品列表（删除界面）
		{
			if(++Del_Index > Get_ShopCart_HeadGoods_Count(ShopCart) )		//切换选择需要删除的商品索引
				Del_Index = 1;
			
		}	
		
	}else
		LED_Control(LED1, OFF);

	
	
	if(KEY_Con == 2)
	{
		LED_Control(LED2, ON);
				
		if(Display_Index == 0)
		{
			User_Initial(&MyUser, Manager);		//初始化系统用户
			Display_Index = 4;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}
		else if(Display_Index == 1)								//位于Home界面
		{
			Display_Index = 3;								//跳往结算界面
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}
		else if(Display_Index == 2)							//位于询问弹窗，取消购买
		{
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}
		else if(Display_Index == 3)							//取消结算，返回Home界面
		{
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}else if(Display_Index == 5)						//位于商品列表（删除界面）
		{
			Cancel_Goods();				//删除选中的商品
			if(--Del_Index <= 1)		//刷新索引
				Del_Index = 1;												
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}
		
	}else
		LED_Control(LED2, OFF);
	
	
	if(KEY_Con == 3)				//按键1长按
	{		
		LED_Control(LED1, ON);
		
		if(Display_Index == 5)		//删除界面下，长按按键1回退至Home界面
		{
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		}
	}else
		LED_Control(LED1, OFF);
	
}
