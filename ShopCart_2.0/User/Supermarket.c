

#include <Supermarket.h>

User MyUser;				//ȫ���û�
__Shopping_Cart ShopCart;	//���ﳵ


//---------------------------------------


/**
 * @brief  ͨ���������ȡ��Ʒ
 * @param  New_Goods
 * @param  Goods_SerNum
 * @retval ��
 */

void Read_Goods(__Commodity* New_Goods, const u8 Goods_SerNum[MaxSize])
{
	*New_Goods = (Commodity *)malloc(sizeof(Commodity));
	(*New_Goods)->next = NULL;
	strcpy((char *) (*New_Goods)->Serial_Number, (char *)Goods_SerNum);
}


/**
 * @brief  ������Ʒ���빺�ﳵ��Ʒ�����У�ͷ�巨��
 * @param  Head_Node
 * @param  In_Goods
 * @note   ����ɨ�������Ʒ��������ﳵ�������� 
 * @retval ��
 */
static void Add_Goods(__Commodity Head_Node, const __Commodity In_Goods)
{
	Commodity *New_Goods;
	Commodity *Result = ShopCart_FindGoods(Head_Node, In_Goods->Serial_Number);
	
	
	
	if(Result != NULL)					//���Ȳ��ҵ�ǰ���ﳵ����Ʒ�������Ƿ���ڸ���Ʒ
	{
		Result->Count++;				//��Ʒ�����Ѵ��ڸ���Ʒ������������ʾ����Ʒ�ֹ���һ��
		SysPrint((u8 *)"The goods' count added !");
	}else								//��Ʒ����û�и���Ʒ���ж�Ϊ�µ���Ʒ
	{
		New_Goods = (Commodity *)malloc(sizeof(Commodity));
		New_Goods->next = NULL;
		strcpy((char *) New_Goods->Serial_Number, (char *)(In_Goods->Serial_Number));	//��������
		strcpy((char *) New_Goods->Name, (char *)(In_Goods->Name));						//������Ʒ����
		New_Goods->Price = In_Goods->Price;												//������Ʒ�۸�
		New_Goods->Count = 1;															//�µ���Ʒ�ĵ�һ��
	
		New_Goods->next = Head_Node->next;
		Head_Node->next = New_Goods;													//head in
		Head_Node->Count++;																//��Ʒ����Ľ����+1
	}
	
}


/**
 * @brief  ɾ����Ʒ
 * @param  Head_Node
 * @param  Goods_SerNum
 * @note   ͨ�����������ͷ���ȶ������룬Ȼ��ɾ��
 * @retval ����ɾ������Ʒ�ļ�ֵ�����ڹ��ﳵ������Ӧ�ļ�ֵ�ܺ�
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
	Head_Node->Count--;	//��Ʒ��������-1
	
	return Goods_Value;
}


/**
 * @brief  ����Ʒ�������ƺͼ۸��ǩ
 * @param  Goods
 * @retval ��
 */
void MakeLabel_Goods(const __Commodity Goods)
{
	strcpy((char *)Goods->Name, Goods_Name1);
	Goods->Price = Goods_Price1;
}


/**
 * @brief  ��ȡ��Ʒ������
 * @param  Goods
 * @retval ������Ʒ�����ַ���
 */
u8* Get_Goods_Name(const __Commodity Goods)
{
	return Goods->Name;
}


/**
 * @brief  ��ȡ��Ʒ��������
 * @param  Goods
 * @retval ������Ʒ���������ַ���
 */
u8* Get_Goods_SerialNumber(const __Commodity Goods)
{
	return Goods->Serial_Number;
}


/**
 * @brief  ��ȡ��Ʒ�ļ۸�
 * @param  Goods
 * @retval ������Ʒ�ļ۸�
 */
ElemType Get_Goods_Price(const __Commodity Goods)
{
	return Goods->Price;
}


/**
 * @brief  ��ȡ��Ʒ�ļ���
 * @param  Goods
 * @retval ������Ʒ�ļ���
 */
ElemType Get_Goods_Count(const __Commodity Goods)
{
	return Goods->Count;
}


//*********************************************************************************************************************
//*********************************************************************************************************************

/**
 * @brief  ��ʼ�����ﳵ
 * @param  Cart_ToCreate
 * @note   ��ʼ���ɹ�����true�����򷵻�flase
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
 * @brief  ���ﳵ������Ʒ
 * @param  Shopping_Cart
 * @param  In_Goods
 * @retval ��
 */
bool ShopCart_AddGoods(__Shopping_Cart Shopping_Cart, const __Commodity In_Goods)
{
	
	if(Shopping_Cart->Head_Goods->Count >= Max_GoodsNumber)			//������Ʒ�����Ŀ����
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
 * @brief  ɾ�� ShopCart_Structured �е���Ʒ
 * @param  __Shopping_Cart
 * @param  Goods_SerNum
 * @note   ������ΪKeyֵ
 * @retval ��
 */
bool ShopCart_DelGoods(__Shopping_Cart Shopping_Cart, const u8 Goods_SerNum[MaxSize])
{
	ElemType Out_Goods_Value;
	Commodity *Result;
	
	if(ShopCart_IsEmpty(Shopping_Cart))			//���ﳵ��û����Ʒ����ɾ��
	{
		SysPrint((u8 *)"Failed to delete goods , there is no goods in ShopCart !");
		return false;
	}
	
	Result = ShopCart_FindGoods(Shopping_Cart->Head_Goods, Goods_SerNum);
	
	if(Result != NULL)
	{
		if(Result->Count > 1)
			Result->Count--;															//����Ʒ����Ʒ�����в�ֹһ����ֱ�Ӽ�ȥ��������
		else
			Out_Goods_Value = Delete_Goods(Shopping_Cart->Head_Goods, Goods_SerNum);	//����Ʒ����Ʒ����ֻ��һ����ֱ��ɾ����
		
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
 * @brief  ��չ��ﳵ�е���Ʒ
 * @param  __Shopping_Cart
 * @retval true or false
 */
bool Clear_ShopCart(__Shopping_Cart Shopping_Cart)
{
	Commodity *traverse;
	Commodity *del;
	
	if(Shopping_Cart->Head_Goods->next == NULL)			//�п�
	{
		SysPrint((u8 *)"It's not necessary to clear !");
		return false;
	}
	
	traverse  = Shopping_Cart->Head_Goods->next;
	while(traverse->next != NULL)
	{
		del = traverse;					
		traverse = traverse->next;			//�������ﳵ��Ʒ
		free(del);							//һһɾ��ֱ��ʣ�����һ��
	}
	
	traverse = NULL;
	free(traverse);
	
	Shopping_Cart->Cart_Value = 0;			//���ý��Ϊ0
	Shopping_Cart->Goods_Number = 0;		//������Ʒ��ĿΪ0
	Shopping_Cart->Head_Goods->next = NULL;	//���ﳵ��Ʒ����ͷ����ÿ�
	Shopping_Cart->Head_Goods->Count = 0;	//��Ʒ������������
	SysPrint((u8 *)"Clear the ShopCart successful !");
	
	return true;
}


/**
 * @brief  ���㹺�ﳵ�е���Ʒ
 * @param  __Shopping_Cart
 * @retval 0 �� ���ﳵ�ܽ��
 */
ElemType Pay_ShopCart(__Shopping_Cart Shopping_Cart)
{
	ElemType Pay_Value;

	
	if( ShopCart_IsEmpty(Shopping_Cart) )			//���ﳵΪ��
	{
		SysPrint((u8 *)"The ShopCart's value : 0");
		return 0;
	}
	Pay_Value = Get_ShopCart_Value(Shopping_Cart);	//��ȡ���ﳵ�ܽ��
	Clear_ShopCart(Shopping_Cart);					//��չ��ﳵ
	SysPrint((u8 *)"The ShopCart payed successful !");
	
	return Pay_Value;
}


/**
 * @brief  ��ù��ﳵ����Ʒ�ܼ�ֵ
 * @param  Shopping_Cart
 * @note   ���ع��ﳵ��������Ʒ��ֵ���ܺ�
 * @retval Shopping_Cart->Cart_Value
 */
ElemType Get_ShopCart_Value(const __Shopping_Cart Shopping_Cart)
{
	Commodity *traverse = Shopping_Cart->Head_Goods->next;
	ElemType Value_Sum  = 0;

	if(Shopping_Cart->Head_Goods->next == NULL)		//�п�
		return 0;
	
	while (traverse != NULL)						
	{
		Value_Sum += (traverse->Price)*(traverse->Count);	//��ǰ��Ʒ�ĸ���*��Ʒ�۸��ۼ�������Ʒ
		traverse = traverse->next;
	}

	if (Shopping_Cart->Cart_Value != Value_Sum)
		Shopping_Cart->Cart_Value = Value_Sum;

	return Shopping_Cart->Cart_Value;
}


/**
 * @brief  ���ҵ�ǰ���ﳵ���Ƿ��Ѵ��ڸ���Ʒ
 * @param  __Commodity
 * @note   ���ڷ�����ָ�룬���򷵻�NULL
 * @retval ָ�� or NULL
 */
static __Commodity ShopCart_FindGoods(const __Commodity Head_Node, const u8 Goods_SerNum[MaxSize])
{
	Commodity *traverse;
	
	if (Head_Node->next == NULL)		//���ﳵΪ��,ֱ��return
	{
		SysPrint((u8 *)"The ShopCart is empty !");
		return NULL;
	}
	else
	{
		traverse  = Head_Node->next;
		while(traverse != NULL)
		{
			if( strcmp((char *)traverse->Serial_Number, (char *)Goods_SerNum) == 0)		//���ҵ����ڸ���Ʒ
			{
				SysPrint((u8 *)"The goods is in ShopCart !");
				return traverse;														//����ָ��return���ܲ���
			}
			else
				traverse = traverse->next;												//���±�����������
		}
		
	}
	
	SysPrint((u8 *)"This goods is not in ShopCart !");
	return NULL;						//Ŀǰ���ﳵ����Ʒ����û�������Ʒ
}


/**
 * @brief  �жϹ��ﳵ�Ƿ�Ϊ��
 * @param  __Shopping_Cart
 * @note   ���ﳵ�շ���true�����򷵻�flase
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
 * @brief  ��ù��ﳵ����Ʒ����Ŀ
 * @param  Shopping_Cart
 * @note   ���ع��ﳵ����Ʒ����Ŀ
 * @retval Shopping_Cart->Goods_Number
 */
ElemType Get_ShopCart_Number(const __Shopping_Cart Shopping_Cart)
{
	return Shopping_Cart->Goods_Number;
}


/**
 * @brief  ��ù��ﳵ��һ��Ʒ��ָ��
 * @param  Shopping_Cart
 * @note   ���ع��ﳵ�е�һ����Ʒ�ĵ�ַ
 * @retval Shopping_Cart->Head_Goods
 */
__Commodity Get_ShopCart_HeadGoods(const __Shopping_Cart Shopping_Cart)
{
	return Shopping_Cart->Head_Goods;
}


/**
 * @brief  ��ù��ﳵ��Ʒ����Ľ����
 * @param  Shopping_Cart
 * @note   ���ع��ﳵ��Ʒ����Ľ����
 * @retval Shopping_Cart->Head_Goods
 */
ElemType Get_ShopCart_HeadGoods_Count(const __Shopping_Cart Shopping_Cart)
{
	return Shopping_Cart->Head_Goods->Count;
}


/**
 * @brief  ��ӡ��Ʒ��Ϣ������
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
 * @brief  ��ӡ���ﳵ��Ϣ������
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
 * @brief  ��ʼ��ϵͳ�û�
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
 * @brief  ���ϵͳ�û�
 * @param  User
 * @note   ���ص�ǰϵͳ�û�
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
 * @brief  ��ӡϵͳlog
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

