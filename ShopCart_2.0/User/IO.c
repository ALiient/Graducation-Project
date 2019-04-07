
#include "IO.h"


u8 RxBuffer1[50];
u8 RxBuffer2[20];
u8 Flag_USART1 = 0;
u8 Flag_USART2 = 0;
u32 TimeDisplay = 0;
u32 THH = 0, TMM = 0, TSS = 0;


void IO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	
	GPIO_Config();
	USART1_Config();
	USART2_Config();
	RTC_Confing();
	NVIC_Config();
	
	BEEP_Init();			//��������ʼ��
	LED_Init();				//LED��ʼ��
	KEY_Init();				//������ʼ��
	
}


static void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//UASRT2 TX2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//���츴��ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2 RX2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  	//��������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
}


static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel =  USART2_IRQn;			//����UASRTΪ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);								//��ʼ������NVIC
	
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
		
}



static void USART1_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
		
}


static void USART2_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
		//����2����ģʽ����
	USART_InitStructure.USART_BaudRate = 9600;						//���ò�����19200		
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����֡�����ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//����ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//����Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//���ù���ģʽ���շ�һ��
	USART_Init(USART2, &USART_InitStructure);						//��ɴ��ڳ�ʼ������
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);					//ʹ�ܴ���2�����ж�
	USART_Cmd(USART2, ENABLE);										//ʹ�ܴ���
		
}

static void RTC_Confing(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */
	BKP_DeInit();
	
	
	RCC_LSICmd(ENABLE);	
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();	
		
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	RTC_WaitForLastTask();
	RTC_SetCounter(HH*3600+MM*60+SS);
	RTC_WaitForLastTask();
	
}


void Time_Display(uint32_t TimeVar)
{
  
  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

}




int fputc(int ch, FILE *f)
{
	
#if SYS_DEBUG == 1						//USART1�ض���
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
#else
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART2, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
#endif
	
	
		return (ch);
}



