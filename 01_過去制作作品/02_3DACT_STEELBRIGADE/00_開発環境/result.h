//=============================================================================
//
// スコア処理 [Result.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;
	bool bUse;
	int nType;
	int g_aResultNumber;
	int nResultScore;
}RESULT;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitResult(void);
void UnInitResult(void);
void UpdateResult(void);
void DrawResult(void);

void SetResult(int nScore);


#endif
