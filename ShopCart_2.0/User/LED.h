

#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define LED1_OFF GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define LED1_ON  GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define LED2_OFF GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define LED2_ON  GPIO_ResetBits(GPIOC, GPIO_Pin_3)


#define LED1 1		//D4
#define LED2 2		//D5
#define ON  1
#define OFF 0




void LED_Init(void);
void LED_Control(u8 LEDn, u8 Status);

#endif
