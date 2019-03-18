//=============================================================================
//
// ハイスコア処理 [ResultItem.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _RESULTITEM_H_
#define _RESULTITEM_H_

#include "main.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitResultItem(void);
void UnInitResultItem(void);
void UpdateResultItem(void);
void DrawResultItem(void);

void SetResultCoin(int nScore);
void SetResultArey(int nScore);
void SetResultSatsu(int nScore);
void SetResultVodka(int nScore);


#endif
