//=============================================================================
//
// スコアアイテム処理 [scoreitem.h]
// Author : 
//
//=============================================================================
#ifndef _ScoreItem_H_
#define _ScoreItem_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCOREITEM (20)

//*****************************************************************************
// モデルの構造体
//*****************************************************************************

typedef struct
{
	DWORD g_nNumMatScoreItem = 0;	//マテリアルの情報の数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
						//DWORD g_nNumMatScoreItem;	//マテリアルの情報の数

	D3DXVECTOR3 VtxMinScoreItem;
	D3DXVECTOR3 VtxMaxScoreItem;

	D3DXMATRIX mtxWorld;	//ワールドマトリックス

	int nIndexShadow;	//影の番号
	int nType;
	bool bUse;
}SCOREITMEM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitScoreItem(void);
void UninitScoreItem(void);
void UpdateScoreItem(void);
void DrawScoreItem(void);

bool CollisionScoreItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetScoreItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

SCOREITMEM *GetScoreItem(void);
#endif
