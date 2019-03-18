//=============================================================================
//
// スコア処理 [Ranking.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _MENURANKING_H_
#define _MENURANKING_H_

#include "main.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	MENURANKINGTYPE_NUMBER = 0, //数字
	MENURANKINGTYPE_CHARACTER,
	MENURANKINGTYPE_MAX
}MENURANKINGTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;
	bool bUse;
	int nType;
	int g_aRankingNumber;
	int nRankScore;
}MENURANKING;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMenuRanking(void);
void UnInitMenuRanking(void);
void UpdateMenuRanking(void);
void DrawMenuRanking(void);

void SetMenuRanking(int nScore);

//void SetRankingpos(D3DXVECTOR3 pos);

#endif
