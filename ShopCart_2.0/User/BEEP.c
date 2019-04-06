

#include "BEEP.h"

u8 Flag_BEEP = 0;

void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	BEEP_OFF;
}


void BEEP_Click(void)
{
	if(Flag_BEEP)
	{
		Flag_BEEP = 0;
		BEEP_OFF;
	}else
		BEEP_ON;
	
}
