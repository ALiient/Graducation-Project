/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "IO.h"

u8 Rx_Cnt = 17;

extern u32 TimingDelay;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	static u8  KEY_Time = 0;
	static u8  Display_Time = 0; 
	static u16 WaitTime_3s = 0;
	
	if((Display_Index == 2) || (Display_Index == 3) || (Display_Index == 5) || (Display_Index == 6) )			//在询问窗口超过5秒未响应，自动跳转回Home界面
	{
		if(++WaitTime_3s >= 5000)
		{
			WaitTime_3s = 0;
			Display_Index = 1;
			Action_Pay = 0;
			LCD_ClearLine(LINE(7));
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
			SysPrint((u8 *)"The action has been canceled !");
		}
	}else
		WaitTime_3s = 0;										//只要不在询问窗口就清零不计
	
	
	if(++Display_Time >= 100)
	{
		Display_Time = 0;
		Flag_Display = 1;
	}
	
	
	if(++KEY_Time == 50)
	{
		KEY_Time = 0;
		Flag_KEY = 1;
	}
	

	
	TimingDelay--;
}

void USART1_IRQHandler(void)
{
	static __IO uint8_t str_num = 0;
	
	if(Action_Pay == 1)
		Rx_Cnt = 48;
	else
		Rx_Cnt = 17;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
    /* Read one byte from the receive data register */
		RxBuffer1[str_num++] = USART_ReceiveData(USART1);
	 
		if(str_num >= Rx_Cnt)
		{
			str_num = 0;
			Flag_USART1 = 1;
		}
	}
  
}


void USART2_IRQHandler(void)
{
	static __IO u8 str_num = 0;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		RxBuffer2[str_num++] = USART_ReceiveData(USART2);
		
	
#if ( (SYS_DEBUG == 0) || (SYS_DEBUG == 2) )
		if(str_num >= 3)
		{
			str_num = 0;
			Flag_USART2 = 1;
		}
#endif	
		
	}
		
}



void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_SEC);
		TimeDisplay = 1;
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	  
		if (RTC_GetCounter() == 0x0001517F)
		{	
			RTC_SetCounter(0x0);
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
		} 
    
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
