

#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

#define BEEP_ON		GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define BEEP_OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_15)

extern u8 Flag_BEEP;

void BEEP_Init(void);
void BEEP_Click(void);

// ªπ√ª’“µΩ£øs

#endif
