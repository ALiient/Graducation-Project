
#ifndef __MODULE_H
#define __MODULE_H

#include "IO.h"


extern u8 In_Code[20];
extern u8 Out_Code[20];
extern u8 Del_Index;

void Module_Init(void);
bool Check_Code(u8 *In_Code);
bool Buy_Goods(u8 *In_Code);
void Cancel_Goods(void);
void Check_Cmd(u8 *Rece_Cmd);

#endif
