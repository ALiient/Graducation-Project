

#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "IO.h"

#define KEY_1_STATUS GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY_2_STATUS GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define KEY_PRESS 1

extern u8 Flag_KEY;


void KEY_Init(void);
void KEY_Read(void);
void KEY_Control(void);

#endif
