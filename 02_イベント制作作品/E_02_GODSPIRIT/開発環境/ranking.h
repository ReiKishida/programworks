////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ランキング処理 [ranking.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//******************************************************************************************************************************
// 構造体の定義
//******************************************************************************************************************************
typedef enum
{
	RANKINGTYPE_SCOREBG1 = 0,	// ランキング背景
	RANKINGTYPE_SCOREBG2,	// ランキング背景
	RANKINGTYPE_SCOREBG3,	// ランキング背景
	RANKINGTYPE_SCOREBG4,	// ランキング背景
	RANKINGTYPE_SCOREBG5,	// ランキング背景
	RANKINGTYPE_SCORE1,	// ランキング
	RANKINGTYPE_SCORE2,	// ランキング
	RANKINGTYPE_SCORE3,	// ランキング
	RANKINGTYPE_SCORE4,	// ランキング
	RANKINGTYPE_SCORE5,	// ランキング
	RANKINGTYPE_SCORENUM1,	// ランキング順位
	RANKINGTYPE_SCORENUM2,	// ランキング順位
	RANKINGTYPE_SCORENUM3,	// ランキング順位
	RANKINGTYPE_SCORENUM4,	// ランキング順位
	RANKINGTYPE_SCORENUM5,	// ランキング順位
	RANKINGTYPE_MAX,
}RANKINGTYPE;
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	RANKINGTYPE nType;
	float fWidth;
	float fHeight;
	bool bUse;
}Ranking;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(D3DXVECTOR3 pos, D3DXCOLOR col, RANKINGTYPE nType, float fWidth, float fHeight);
void SortRanking(int nScore);

#endif
