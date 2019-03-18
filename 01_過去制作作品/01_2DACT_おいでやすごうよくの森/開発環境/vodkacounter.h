//=============================================================================
//
// ウォッカカウンター処理 [vodkaounter.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _VODKACOUNTER_H_
#define _VODKACOUNTER_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVodkaCounter(void);
void UninitVodkaCounter(void);
void UpdateVodkaCounter(void);
void DrawVodkaCounter(void);

void AddVodkaCounter(int nValue);

int GetVodkaCounter(void);






#endif
