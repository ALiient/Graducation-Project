

#include "Module.h"

u8 Del_Index = 1;

u8 In_Code[20];
u8 Out_Code[20];
u8 String_Text[40];

void Module_Init(void)
{
	ShopCart_Initial(&ShopCart);					//��ʼ�����ﳵ
	Print_ShopCart(ShopCart);						//��ӡ���ﳵ
}


bool Check_Code(u8 *Code_Data)
{

	u8 Data_Pack[15];
	
	if(strlen((char *)Code_Data) <= 15)								//�������룬ʶ��ʧ��
		return false;
	
	strcpy((char *)In_Code, (char *)Code_Data);						//������Ʒ������
	SysPrint((u8 *)"Get code successful !");
	strncpy((char *)Data_Pack, (char *)Code_Data + 0, 7);			//����ǰ�߸��ַ�
	sprintf((char *)String_Text, "%c%s%c", 'A', Data_Pack, 'B');	//��Ӱ�ͷA�Ͱ�βB
	strcpy((char *)Data_Pack, (char *)String_Text);					//�����In_Code
	
	printf("%s", Data_Pack);										//���ﳵģʽ�£��������������Ʒ��ѯ
																	//����ģʽ�£�ֱ�ӷ���������Ϣ,ѡ��ɾ�������
	return true;
}


void Check_Cmd(u8 *Rece_Cmd)
{

	if(MyUser.Attributes == Customer)							//���ﳵģʽ��
	{
		if( (Rece_Cmd[0] == 'A') && (Rece_Cmd[2] == 'B') )		//���ݰ���ʽ��ȷ
		{
			if(Rece_Cmd[1] == '0')								//��ѯ������Ʒ
			{
				return ;
			}else												//��ѯ����Ʒ�����й�����
			{
				Display_Index = 2;								//�л�ʶ�����
				ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
				
				//��Ʒ���ǩ
			}
		}else			//����ȷ���ݰ�
			return ;
	}	
}


void Cancel_Goods(void)
{
	ShopCart_DelGoods(ShopCart, Out_Code);					//��������ɾ����Ӧ���
}


bool Buy_Goods(u8 *In_Code)
{
	__Commodity goods;
	
	
	//create goods
	Read_Goods(&goods, (u8 *)In_Code);			//��ȡ��Ʒ
	MakeLabel_Goods(goods);						//��Ʒ���ǩ
	Print_Goods(goods);							//��ӡ��Ʒ
	Print_ShopCart(ShopCart);					//��ӡ���ﳵ
	if( ! ShopCart_AddGoods(ShopCart, goods) )	//�����Ʒ�����ﳵ,���ﳵ�����޷����
	{
		free(goods);
		return false;
	}	
	free(goods);								//free��ʱ�ṹ��
	Print_ShopCart(ShopCart);					//��ӡ���ﳵ
	
	return true;
}




