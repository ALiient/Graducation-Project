

#ifndef __SUPERMARKET_H
#define __SUPERMARKET_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef unsigned int ElemType;


#define MaxSize 40
#define MinSize 20
#define Max_GoodsNumber 15				//��Ʒ�����Ŀ����
#define SYS_INFO "[@ShopCart-System]#"	//ϵͳlogǰ׺ 
#define SYS_DEBUG 0						//ϵͳdebug��־λ  0:USART2�ض���(����ģʽ)	1:USART1�ض���		2:USART2�ض���(����debug)


#define Goods_Name1	"goods1"	
#define Goods_Name2 "goods2"
#define Goods_Price1 10
#define Goods_Price2 20




//---------- ��Ʒ�ṹ�� ----------//
typedef struct Commodity
{
	u8 			Name[MinSize];				//��Ʒ����
	u8 			Serial_Number[MaxSize];  	//��Ʒ������
	u8 			Count;						//(�����������ǰ��Ʒ�ж��ٸ�;Head_Goods:��Ʒ�����ϵĽ����)
	ElemType 	Price;						//��Ʒ�۸�
	struct 		Commodity *next;			//ָ����һ����Ʒ��ָ��

}Commodity, *__Commodity;



//---------- ���ﳵ�ṹ�� ----------//
typedef struct Shopping_Cart
{
	ElemType 	Goods_Number;		//���ﳵ����Ʒ����Ŀ
	ElemType 	Cart_Value;			//���ﳵ����Ʒ�ļ�ֵ�ܺ�
	__Commodity Head_Goods;			//���ﳵ�׸���Ʒ��ָ��

}Shopping_Cart, *__Shopping_Cart;



//---------- �û�ö������ ----------//
typedef enum
{
	Customer = 0,		//�˿�
	Manager  = 1		//���й�����
}User_Options;
//---------- �û��ṹ�� ----------//
typedef struct User
{
	ElemType Attributes;	//�û����Ա�ʶ
}User;



extern User MyUser;					//ȫ���û�
extern __Shopping_Cart ShopCart;	//���ﳵ





//--------------------- ���ﳵ�����ӿ� ---------------------//
bool 		ShopCart_Initial(__Shopping_Cart *Cart_ToCreate);
bool 		ShopCart_AddGoods(__Shopping_Cart Shopping_Cart, const __Commodity In_Goods);
bool 		ShopCart_DelGoods(__Shopping_Cart Shopping_Cart, const u8 Goods_SerNum[MaxSize]);
bool 		ShopCart_IsEmpty(const __Shopping_Cart Shopping_Cart);
static	    __Commodity ShopCart_FindGoods(const __Commodity Head_Node, const u8 Goods_SerNum[MaxSize]);
ElemType 	Get_ShopCart_Number(const __Shopping_Cart Shopping_Cart);
ElemType 	Get_ShopCart_Value(const __Shopping_Cart Shopping_Cart);
ElemType    Get_ShopCart_HeadGoods_Count(const __Shopping_Cart Shopping_Cart);
__Commodity Get_ShopCart_HeadGoods(const __Shopping_Cart Shopping_Cart);
bool 		Clear_ShopCart(__Shopping_Cart Shopping_Cart);
ElemType 	Pay_ShopCart(__Shopping_Cart Shopping_Cart);
void 		Print_ShopCart(const __Shopping_Cart ShopCart);


//--------------------- ��Ʒ�����ӿ� ---------------------//
void 		Read_Goods(__Commodity* New_Goods, const u8 Goods_SerNum[MaxSize]);
void 		MakeLabel_Goods(const __Commodity Goods);
u8* 		Get_Goods_Name(const __Commodity Goods);
u8* 		Get_Goods_SerialNumber(const __Commodity Goods);
ElemType 	Get_Goods_Price(const __Commodity Goods);
ElemType    Get_Goods_Count(const __Commodity Goods);
void 		Print_Goods(const __Commodity Goods);



//--------------------- �û������ӿ� ---------------------//
bool 		User_Initial(User *MyUser, User_Options Attributes);
ElemType 	Get_User(User MyUser);
void 		SysPrint(u8 *LogData);




#endif
