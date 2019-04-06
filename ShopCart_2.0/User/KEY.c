

#include "KEY.h"


u8 Flag_KEY = 0;
u8 KEY_Con = 0;


#define KEY_1 (GPIOA->IDR & 0X01)

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void KEY_Read(void)
{
	static u8 Key_Time[3] = {0};
	KEY_Con = 0;
	
	if(KEY_1 == KEY_PRESS)
	{
		Key_Time[1]++;
		if(Key_Time[1] == 1)
			KEY_Con = 1;
		
		if(Key_Time[1] == 20)
		{
			Key_Time[1] = 15;
			KEY_Con = 3;		//������ɾ�������ϻ�����Home��
		}
	}else
		Key_Time[1] = 0;


	if(KEY_2_STATUS == KEY_PRESS)
	{
		Key_Time[2]++;
		if(Key_Time[2] == 1)
			KEY_Con = 2;
		
		if(Key_Time[2] == 20)
		{
			Key_Time[2] = 15;
			KEY_Con = 2;
		}
	}else
		Key_Time[2] = 0;

}


void KEY_Control(void)
{

	if(KEY_Con == 1)
	{
		LED_Control(LED1, ON);
		
		if(Display_Index == 0)
		{
			User_Initial(&MyUser, Customer);				//��ʼ��ϵͳ�û�
			Print_ShopCart(ShopCart);						//��ӡ���ﳵ
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}else if(Display_Index == 2)						//λ��ѯ�ʵ�����ȷ�Ϲ���
		{					
			Buy_Goods(In_Code);								//ȷ�Ϲ���
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}else if(Display_Index == 1)						//λ��Home��������ת����Ʒ�б��ȴ�ɾ������
		{
			Display_Index = 5;								//������Ʒ�б�ɾ�����棩
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}else if(Display_Index == 3)						//λ�ڽ�����棬ȷ�Ͻ���
		{
			Pay_ShopCart(ShopCart);							//ȷ�Ͻ��㹺�ﳵ
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
			
		}else if(Display_Index == 5)						//λ����Ʒ�б�ɾ�����棩
		{
			if(++Del_Index > Get_ShopCart_HeadGoods_Count(ShopCart) )		//�л�ѡ����Ҫɾ������Ʒ����
				Del_Index = 1;
			
		}	
		
	}else
		LED_Control(LED1, OFF);

	
	
	if(KEY_Con == 2)
	{
		LED_Control(LED2, ON);
				
		if(Display_Index == 0)
		{
			User_Initial(&MyUser, Manager);		//��ʼ��ϵͳ�û�
			Display_Index = 4;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}
		else if(Display_Index == 1)								//λ��Home����
		{
			Display_Index = 3;								//�����������
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}
		else if(Display_Index == 2)							//λ��ѯ�ʵ�����ȡ������
		{
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}
		else if(Display_Index == 3)							//ȡ�����㣬����Home����
		{
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}else if(Display_Index == 5)						//λ����Ʒ�б�ɾ�����棩
		{
			Cancel_Goods();				//ɾ��ѡ�е���Ʒ
			if(--Del_Index <= 1)		//ˢ������
				Del_Index = 1;												
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}
		
	}else
		LED_Control(LED2, OFF);
	
	
	if(KEY_Con == 3)				//����1����
	{		
		LED_Control(LED1, ON);
		
		if(Display_Index == 5)		//ɾ�������£���������1������Home����
		{
			Display_Index = 1;
			ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		}
	}else
		LED_Control(LED1, OFF);
	
}
