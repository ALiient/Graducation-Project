

#include "LED.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	LED1_OFF;
	LED2_OFF;
}


void LED_Control(u8 LEDn, u8 Status)
{
	if(LEDn == LED1)
	{
		if(Status == ON)
			LED1_ON;
		else
			LED1_OFF;
	}
	
	if(LEDn == LED2)
	{
		if(Status == ON)
			LED2_ON;
		else
			LED2_OFF;
	}
	
}
