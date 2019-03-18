//=============================================================================
//
// アイテムカウンター処理 [itemcounter.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _ITEMCOUNTER_H_
#define _ITEMCOUNTER_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitItemCounter(void);
void UninitItemCounter(void);
void UpdateItemCounter(void);
void DrawItemCounter(void);

void AddCounter(int nValue);

int GetItemCounter(void);
#endif
