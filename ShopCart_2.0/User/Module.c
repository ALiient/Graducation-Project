

#include "Module.h"

u8 Del_Index = 1;

u8 In_Code[20];
u8 Out_Code[20];
u8 String_Text[40];

void Module_Init(void)
{
	ShopCart_Initial(&ShopCart);					//初始化购物车
	Print_ShopCart(ShopCart);						//打印购物车
}


bool Check_Code(u8 *Code_Data)
{

	u8 Data_Pack[15];
	
	if(strlen((char *)Code_Data) <= 15)								//错误条码，识别失败
		return false;
	
	strcpy((char *)In_Code, (char *)Code_Data);						//拷贝商品条形码
	SysPrint((u8 *)"Get code successful !");
	strncpy((char *)Data_Pack, (char *)Code_Data + 0, 7);			//拷贝前七个字符
	sprintf((char *)String_Text, "%c%s%c", 'A', Data_Pack, 'B');	//添加包头A和包尾B
	strcpy((char *)Data_Pack, (char *)String_Text);					//打包至In_Code
	
	printf("%s", Data_Pack);										//购物车模式下，发送条码进行商品查询
																	//管理模式下，直接发送条码信息,选择删除或添加
	return true;
}


void Check_Cmd(u8 *Rece_Cmd)
{

	if(MyUser.Attributes == Customer)							//购物车模式下
	{
		if( (Rece_Cmd[0] == 'A') && (Rece_Cmd[2] == 'B') )		//数据包格式正确
		{
			if(Rece_Cmd[1] == '0')								//查询不到商品
			{
				return ;
			}else												//查询到商品，进行购买动作
			{
				Display_Index = 2;								//切换识别界面
				ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
				
				//商品打标签
			}
		}else			//非正确数据包
			return ;
	}	
}


void Cancel_Goods(void)
{
	ShopCart_DelGoods(ShopCart, Out_Code);					//根据条码删除对应结点
}


bool Buy_Goods(u8 *In_Code)
{
	__Commodity goods;
	
	
	//create goods
	Read_Goods(&goods, (u8 *)In_Code);			//读取商品
	MakeLabel_Goods(goods);						//商品打标签
	Print_Goods(goods);							//打印商品
	Print_ShopCart(ShopCart);					//打印购物车
	if( ! ShopCart_AddGoods(ShopCart, goods) )	//添加商品至购物车,购物车满则无法添加
	{
		free(goods);
		return false;
	}	
	free(goods);								//free临时结构体
	Print_ShopCart(ShopCart);					//打印购物车
	
	return true;
}




