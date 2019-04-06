
#include "stm32f10x.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include <stdio.h>
#include "IO.h"


u32 TimingDelay = 0;
void Delay_Ms(u32 nTime);



/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main ( void )
{
	SysTick_Config(SystemCoreClock/1000);
	ILI9341_Init ();        //LCD ��ʼ��
	IO_Init();				//GPIO��ʼ��
	Module_Init();			//ģ���ʼ��
	
		
 //����0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 //���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 //���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����  
	ILI9341_GramScan ( 3 );
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
	

	
	while ( 1 )
	{
		if(Flag_USART1)				//����ʶ��ģ�鷢�͵���������
		{
			Flag_USART1 = 0;
			Check_Code(RxBuffer1);
		}
		
		if(Flag_USART2)				//������λ�����͵�Ӧ��
		{
			Flag_USART2 = 0;
			Check_Cmd(RxBuffer2);
		}
		
		
		if(Flag_Display)			//LCD��ʾ�������л�
		{
			Flag_Display = 0;
			Display_Menu();
		}
		
		if(Flag_KEY)				//������Ӧ
		{
			Flag_KEY = 0;
			KEY_Read();
			KEY_Control();
		}
		
		if(TimeDisplay == 1)		//RTCʱ��
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



