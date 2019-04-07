
#ifndef __IO_H
#define __IO_H

#include "stm32f10x.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Module.h"
#include "KEY.h"
#include "LED.h"
#include "BEEP.h"
#include "Menu.h"
#include "Supermarket.h"


#define HH 9
#define MM 7
#define SS 7

extern u32 TimeDisplay;
extern u32 THH, TMM, TSS;



static void GPIO_Config(void);
static void NVIC_Config(void);
static void USART1_Config(void);
static void USART2_Config(void);
static void RTC_Confing(void);

extern u8 RxBuffer1[50];
extern u8 RxBuffer2[20];
extern u8 Flag_USART1;
extern u8 Flag_USART2;

void IO_Init(void);
void Time_Display(uint32_t TimeVar);



#endif
