

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
#define Max_GoodsNumber 15				//商品结点数目上限
#define SYS_INFO "[@ShopCart-System]#"	//系统log前缀 
#define SYS_DEBUG 0						//系统debug标志位  0:USART2重定向(运行模式)	1:USART1重定向		2:USART2重定向(无线debug)


#define Goods_Name1	"goods1"	
#define Goods_Name2 "goods2"
#define Goods_Price1 10
#define Goods_Price2 20




//---------- 商品结构体 ----------//
typedef struct Commodity
{
	u8 			Name[MinSize];				//商品名称
	u8 			Serial_Number[MaxSize];  	//商品条形码
	u8 			Count;						//(自身计数：当前商品有多少个;Head_Goods:商品链表上的结点数)
	ElemType 	Price;						//商品价格
	struct 		Commodity *next;			//指向下一件商品的指针

}Commodity, *__Commodity;



//---------- 购物车结构体 ----------//
typedef struct Shopping_Cart
{
	ElemType 	Goods_Number;		//购物车中商品的数目
	ElemType 	Cart_Value;			//购物车中商品的价值总和
	__Commodity Head_Goods;			//购物车首个商品的指针

}Shopping_Cart, *__Shopping_Cart;



//---------- 用户枚举类型 ----------//
typedef enum
{
	Customer = 0,		//顾客
	Manager  = 1		//超市管理者
}User_Options;
//---------- 用户结构体 ----------//
typedef struct User
{
	ElemType Attributes;	//用户属性标识
}User;



extern User MyUser;					//全局用户
extern __Shopping_Cart ShopCart;	//购物车





//--------------------- 购物车交互接口 ---------------------//
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


//--------------------- 商品交互接口 ---------------------//
void 		Read_Goods(__Commodity* New_Goods, const u8 Goods_SerNum[MaxSize]);
void 		MakeLabel_Goods(const __Commodity Goods);
u8* 		Get_Goods_Name(const __Commodity Goods);
u8* 		Get_Goods_SerialNumber(const __Commodity Goods);
ElemType 	Get_Goods_Price(const __Commodity Goods);
ElemType    Get_Goods_Count(const __Commodity Goods);
void 		Print_Goods(const __Commodity Goods);



//--------------------- 用户交互接口 ---------------------//
bool 		User_Initial(User *MyUser, User_Options Attributes);
ElemType 	Get_User(User MyUser);
void 		SysPrint(u8 *LogData);




#endif
