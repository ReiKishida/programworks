//=============================================================================
//
// 札カウンター処理 [satsuounter.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _SATSUCOUNTER_H_
#define _SATSUCOUNTER_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSatsuCounter(void);
void UninitSatsuCounter(void);
void UpdateSatsuCounter(void);
void DrawSatsuCounter(void);

void AddSatsuCounter(int nValue);

int GetSatsuCounter(void);







#endif
