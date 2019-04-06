

#include <Supermarket.h>

User MyUser;				//全局用户
__Shopping_Cart ShopCart;	//购物车


//---------------------------------------


/**
 * @brief  通过条形码读取商品
 * @param  New_Goods
 * @param  Goods_SerNum
 * @retval 无
 */

void Read_Goods(__Commodity* New_Goods, const u8 Goods_SerNum[MaxSize])
{
	*New_Goods = (Commodity *)malloc(sizeof(Commodity));
	(*New_Goods)->next = NULL;
	strcpy((char *) (*New_Goods)->Serial_Number, (char *)Goods_SerNum);
}


/**
 * @brief  将新商品加入购物车商品链表中（头插法）
 * @param  Head_Node
 * @param  In_Goods
 * @note   将新扫描完的商品添加至购物车中链表中 
 * @retval 无
 */
static void Add_Goods(__Commodity Head_Node, const __Commodity In_Goods)
{
	Commodity *New_Goods;
	Commodity *Result = ShopCart_FindGoods(Head_Node, In_Goods->Serial_Number);
	
	
	
	if(Result != NULL)					//优先查找当前购物车的商品链表中是否存在该商品
	{
		Result->Count++;				//商品链表已存在该商品，递增计数表示该商品又购买一个
		SysPrint((u8 *)"The goods' count added !");
	}else								//商品链表没有该商品，判定为新的商品
	{
		New_Goods = (Commodity *)malloc(sizeof(Commodity));
		New_Goods->next = NULL;
		strcpy((char *) New_Goods->Serial_Number, (char *)(In_Goods->Serial_Number));	//传递条码
		strcpy((char *) New_Goods->Name, (char *)(In_Goods->Name));						//传递商品名称
		New_Goods->Price = In_Goods->Price;												//传递商品价格
		New_Goods->Count = 1;															//新的商品的第一个
	
		New_Goods->next = Head_Node->next;
		Head_Node->next = New_Goods;													//head in
		Head_Node->Count++;																//商品链表的结点数+1
	}
	
}


/**
 * @brief  删除商品
 * @param  Head_Node
 * @param  Goods_SerNum
 * @note   通过读入的链表头，比对条形码，然后删除
 * @retval 返回删除的商品的价值，便于购物车减掉相应的价值总和
 */
static ElemType Delete_Goods(__Commodity Head_Node, const u8 Goods_SerNum[MaxSize])
{
	Commodity *search = Head_Node;
	Commodity *del;
	ElemType Goods_Value;

	while ( (strcmp((char *) search->next->Serial_Number, (char *) Goods_SerNum)) != 0 )		//search the one in front of the goods
	{
		search = search->next;
	}

	del = search->next;
	Goods_Value = del->Price;
	search->next = del->next;
	free(del);
	search = NULL;
	free(search);		//break out
	Head_Node->Count--;	//商品链表结点数-1
	
	return Goods_Value;
}


/**
 * @brief  给商品打上名称和价格标签
 * @param  Goods
 * @retval 无
 */
void MakeLabel_Goods(const __Commodity Goods)
{
	strcpy((char *)Goods->Name, Goods_Name1);
	Goods->Price = Goods_Price1;
}


/**
 * @brief  获取商品的名称
 * @param  Goods
 * @retval 返回商品名称字符串
 */
u8* Get_Goods_Name(const __Commodity Goods)
{
	return Goods->Name;
}


/**
 * @brief  获取商品的条形码
 * @param  Goods
 * @retval 返回商品的条形码字符串
 */
u8* Get_Goods_SerialNumber(const __Commodity Goods)
{
	return Goods->Serial_Number;
}


/**
 * @brief  获取商品的价格
 * @param  Goods
 * @retval 返回商品的价格
 */
ElemType Get_Goods_Price(const __Commodity Goods)
{
	return Goods->Price;
}


/**
 * @brief  获取商品的计数
 * @param  Goods
 * @retval 返回商品的计数
 */
ElemType Get_Goods_Count(const __Commodity Goods)
{
	return Goods->Count;
}


//*********************************************************************************************************************
//*********************************************************************************************************************

/**
 * @brief  初始化购物车
 * @param  Cart_ToCreate
 * @note   初始化成功返回true，否则返回flase
 * @retval true or flase
 */
bool ShopCart_Initial(__Shopping_Cart *Cart_ToCreate)
{
	if ((*Cart_ToCreate) == NULL)
	{
		*Cart_ToCreate = (Shopping_Cart *)malloc(sizeof(Shopping_Cart));
		(*Cart_ToCreate)->Goods_Number = 0;
		(*Cart_ToCreate)->Cart_Value = 0;
		(*Cart_ToCreate)->Head_Goods = (Commodity *)malloc(sizeof(Commodity));
		(*Cart_ToCreate)->Head_Goods->next = NULL;
		SysPrint((u8 *)"ShopCart Initial Successful !");
		
		return true;
	}
	else
	{
		SysPrint((u8 *)"ShopCart Initial Failed !");
		return false;
	}
}


/**
 * @brief  向购物车加入商品
 * @param  Shopping_Cart
 * @param  In_Goods
 * @retval 无
 */
bool ShopCart_AddGoods(__Shopping_Cart Shopping_Cart, const __Commodity In_Goods)
{
	
	if(Shopping_Cart->Head_Goods->Count >= Max_GoodsNumber)			//超出商品结点数目上限
	{
		SysPrint((u8 *)"Failed to add goods , ShopCart is full !");
		return false;
	}
	
	Add_Goods(Shopping_Cart->Head_Goods, In_Goods);
	Shopping_Cart->Goods_Number++;
	Shopping_Cart->Cart_Value += In_Goods->Price;
	SysPrint((u8 *)"Add goods to ShopCart successful !");
	
	return true;
}


/**
 * @brief  删除 ShopCart_Structured 中的商品
 * @param  __Shopping_Cart
 * @param  Goods_SerNum
 * @note   条形码为Key值
 * @retval 无
 */
bool ShopCart_DelGoods(__Shopping_Cart Shopping_Cart, const u8 Goods_SerNum[MaxSize])
{
	ElemType Out_Goods_Value;
	Commodity *Result;
	
	if(ShopCart_IsEmpty(Shopping_Cart))			//购物车中没有商品可以删除
	{
		SysPrint((u8 *)"Failed to delete goods , there is no goods in ShopCart !");
		return false;
	}
	
	Result = ShopCart_FindGoods(Shopping_Cart->Head_Goods, Goods_SerNum);
	
	if(Result != NULL)
	{
		if(Result->Count > 1)
			Result->Count--;															//该商品在商品链表中不止一个，直接减去计数即可
		else
			Out_Goods_Value = Delete_Goods(Shopping_Cart->Head_Goods, Goods_SerNum);	//该商品在商品链表只有一个，直接删除掉
		
		Shopping_Cart->Goods_Number--;
		Shopping_Cart->Cart_Value -= Out_Goods_Value;
		SysPrint((u8 *)"Delete goods successful !");
	}else
	{
		SysPrint((u8 *)"Delete goods error !");
		return false;
	}

	return true;
}


/**
 * @brief  清空购物车中的商品
 * @param  __Shopping_Cart
 * @retval true or false
 */
bool Clear_ShopCart(__Shopping_Cart Shopping_Cart)
{
	Commodity *traverse;
	Commodity *del;
	
	if(Shopping_Cart->Head_Goods->next == NULL)			//判空
	{
		SysPrint((u8 *)"It's not necessary to clear !");
		return false;
	}
	
	traverse  = Shopping_Cart->Head_Goods->next;
	while(traverse->next != NULL)
	{
		del = traverse;					
		traverse = traverse->next;			//遍历购物车商品
		free(del);							//一一删除直到剩下最后一个
	}
	
	traverse = NULL;
	free(traverse);
	
	Shopping_Cart->Cart_Value = 0;			//重置金额为0
	Shopping_Cart->Goods_Number = 0;		//重置商品数目为0
	Shopping_Cart->Head_Goods->next = NULL;	//购物车商品链表头结点置空
	Shopping_Cart->Head_Goods->Count = 0;	//商品链表结点数清零
	SysPrint((u8 *)"Clear the ShopCart successful !");
	
	return true;
}


/**
 * @brief  结算购物车中的商品
 * @param  __Shopping_Cart
 * @retval 0 或 购物车总金额
 */
ElemType Pay_ShopCart(__Shopping_Cart Shopping_Cart)
{
	ElemType Pay_Value;

	
	if( ShopCart_IsEmpty(Shopping_Cart) )			//购物车为空
	{
		SysPrint((u8 *)"The ShopCart's value : 0");
		return 0;
	}
	Pay_Value = Get_ShopCart_Value(Shopping_Cart);	//获取购物车总金额
	Clear_ShopCart(Shopping_Cart);					//清空购物车
	SysPrint((u8 *)"The ShopCart payed successful !");
	
	return Pay_Value;
}


/**
 * @brief  获得购物车中商品总价值
 * @param  Shopping_Cart
 * @note   返回购物车内所有商品价值的总和
 * @retval Shopping_Cart->Cart_Value
 */
ElemType Get_ShopCart_Value(const __Shopping_Cart Shopping_Cart)
{
	Commodity *traverse = Shopping_Cart->Head_Goods->next;
	ElemType Value_Sum  = 0;

	if(Shopping_Cart->Head_Goods->next == NULL)		//判空
		return 0;
	
	while (traverse != NULL)						
	{
		Value_Sum += (traverse->Price)*(traverse->Count);	//当前商品的个数*商品价格，累加所有商品
		traverse = traverse->next;
	}

	if (Shopping_Cart->Cart_Value != Value_Sum)
		Shopping_Cart->Cart_Value = Value_Sum;

	return Shopping_Cart->Cart_Value;
}


/**
 * @brief  查找当前购物车中是否已存在该商品
 * @param  __Commodity
 * @note   存在返回其指针，否则返回NULL
 * @retval 指针 or NULL
 */
static __Commodity ShopCart_FindGoods(const __Commodity Head_Node, const u8 Goods_SerNum[MaxSize])
{
	Commodity *traverse;
	
	if (Head_Node->next == NULL)		//购物车为空,直接return
	{
		SysPrint((u8 *)"The ShopCart is empty !");
		return NULL;
	}
	else
	{
		traverse  = Head_Node->next;
		while(traverse != NULL)
		{
			if( strcmp((char *)traverse->Serial_Number, (char *)Goods_SerNum) == 0)		//查找到存在该商品
			{
				SysPrint((u8 *)"The goods is in ShopCart !");
				return traverse;														//将其指针return接受操作
			}
			else
				traverse = traverse->next;												//往下遍历继续查找
		}
		
	}
	
	SysPrint((u8 *)"This goods is not in ShopCart !");
	return NULL;						//目前购物车的商品链表还没有这个商品
}


/**
 * @brief  判断购物车是否为空
 * @param  __Shopping_Cart
 * @note   购物车空返回true，否则返回flase
 * @retval true or flase
 */
bool ShopCart_IsEmpty(const __Shopping_Cart Shopping_Cart)
{
	if (Shopping_Cart->Head_Goods->next == NULL)
	{
		SysPrint((u8 *)"The ShopCart is empty !");
		return true;
	}
	else
	{
		SysPrint((u8 *)"The ShopCart is not empty !");
		return false;
	}
}


/**
 * @brief  获得购物车内商品的数目
 * @param  Shopping_Cart
 * @note   返回购物车内商品的数目
 * @retval Shopping_Cart->Goods_Number
 */
ElemType Get_ShopCart_Number(const __Shopping_Cart Shopping_Cart)
{
	return Shopping_Cart->Goods_Number;
}


/**
 * @brief  获得购物车第一商品的指针
 * @param  Shopping_Cart
 * @note   返回购物车中第一个商品的地址
 * @retval Shopping_Cart->Head_Goods
 */
__Commodity Get_ShopCart_HeadGoods(const __Shopping_Cart Shopping_Cart)
{
	return Shopping_Cart->Head_Goods;
}


/**
 * @brief  获得购物车商品链表的结点数
 * @param  Shopping_Cart
 * @note   返回购物车商品链表的结点数
 * @retval Shopping_Cart->Head_Goods
 */
ElemType Get_ShopCart_HeadGoods_Count(const __Shopping_Cart Shopping_Cart)
{
	return Shopping_Cart->Head_Goods->Count;
}


/**
 * @brief  打印商品信息至串口
 * @param  Goods
 */
void Print_Goods(const __Commodity Goods)
{
#if SYS_DEBUG == 2
	printf("\n-------------- Goods Info --------------\n");
	printf("Good's Serial_Number : %s\n", Get_Goods_SerialNumber(Goods) );
	printf("Good's name : %s\n", Get_Goods_Name(Goods) );
	printf("Good's price : %d\n", Get_Goods_Price(Goods) );
	printf("---------------------------------------\n\n");
#else
	return ;
	
#endif
}


/**
 * @brief  打印购物车信息至串口
 * @param  ShopCart
 */
void Print_ShopCart(const __Shopping_Cart ShopCart)
{
#if SYS_DEBUG == 2
	printf("\n------------- ShopCart Info --------------\n");
	printf("ShopCart's Number : %d\n", Get_ShopCart_Number(ShopCart) );
	printf("ShopCart's Value : %d\n", Get_ShopCart_Value(ShopCart) );
	printf("The HeadGood's name : %s\n", ShopCart->Head_Goods->next->Name);
	printf("----------------------------------------\n\n");
#else
	return ;
	
#endif
}



//-----------------------------------------------------------------------------------------

/**
 * @brief  初始化系统用户
 * @param  User
 * @retval Customer or Manager
 */
bool User_Initial(User *MyUser, User_Options Attributes)
{
	MyUser->Attributes = Attributes;
	SysPrint((u8 *)"MyUser Initial Successful !");
	Get_User(*MyUser);
	
	return true;
}


/**
 * @brief  获得系统用户
 * @param  User
 * @note   返回当前系统用户
 * @retval Customer or Manager
 */
ElemType Get_User(User MyUser)
{
	if(MyUser.Attributes == Customer)
	{
		SysPrint((u8 *)"The current User : Customer");
		return Customer;
	}
	else if(MyUser.Attributes == Manager)
	{
		SysPrint((u8 *)"The current User : Manager");
		return Manager;
	}
	else
	{
		SysPrint((u8 *)"Failed to get the User !");
		return false;
	}
}


/**
 * @brief  打印系统log
 * @param  LogData
 */
void SysPrint(u8 *LogData)
{
	
#if SYS_DEBUG == 2

	printf(">>>>>>>%s  %s\n", SYS_INFO, LogData);
	
#else
	return ;
	
#endif

	
}

