
#include "stm32f10x.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include <stdio.h>
#include "IO.h"


u32 TimingDelay = 0;
void Delay_Ms(u32 nTime);



/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{
	SysTick_Config(SystemCoreClock/1000);
	ILI9341_Init ();        //LCD 初始化
	IO_Init();				//GPIO初始化
	Module_Init();			//模块初始化
	
		
 //其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向  
	ILI9341_GramScan ( 3 );
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	

	
	while ( 1 )
	{
		if(Flag_USART1)				//接收识别模组发送的条码数据
		{
			Flag_USART1 = 0;
			Check_Code(RxBuffer1);
		}
		
		if(Flag_USART2)				//接收上位机发送的应答
		{
			Flag_USART2 = 0;
			Check_Cmd(RxBuffer2);
		}
		
		
		if(Flag_Display)			//LCD显示及界面切换
		{
			Flag_Display = 0;
			Display_Menu();
		}
		
		if(Flag_KEY)				//按键响应
		{
			Flag_KEY = 0;
			KEY_Read();
			KEY_Control();
		}
		
		if(TimeDisplay == 1)		//RTC时间
		{
			Time_Display(RTC_GetCounter());
			TimeDisplay = 0;
		}
			
	}
	
	
}



//
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}



