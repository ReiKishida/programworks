//=============================================================================
//
// ハイスコア処理 [highscore.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

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
	int g_aHighScoreNumber;
	int nHighScore;
}HIGHSCORE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitHighScore(void);
void UnInitHighScore(void);
void UpdateHighScore(void);
void DrawHighScore(void);

void SetHighScore(int nScore);


#endif
