//=============================================================================
//
// 弓処理 [arrow.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posold;			//位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//回転
	D3DXCOLOR col;				//色
	D3DXVECTOR3 vtxMin;			//モデルの最小値
	D3DXVECTOR3 vtxMax;			//モデルの最大値
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int nIdxShadow;				//影番号
	int nLife;					//ライフ
	bool bUse;					//使用状況
}ARROW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitArrow(void);
void UninitArrow(void);
void UpdateArrow(void);
void DrawArrow(void);

ARROW*GetArrow(void);

void SetArrow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nLife);
#endif
