//=============================================================================
//
// ハイスコア処理 [highscore.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _GAMEHIGHSCORE_H_
#define _GAMWHIGHSCORE_H_

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
	int g_aGameHighScoreNumber;
	int nGameHighScore;
}GAMEHIGHSCORE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGameHighScore(void);
void UnInitGameHighScore(void);
void UpdateGameHighScore(void);
void DrawGameHighScore(void);



void SetGameHighScore(int nScore);


#endif
