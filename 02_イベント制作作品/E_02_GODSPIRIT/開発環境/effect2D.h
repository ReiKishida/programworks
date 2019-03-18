////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクトの処理 [effect2D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

#include "main.h"

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	EFFECTTYPE_2D_ARROW = 0, // ちょうど
	EFFECTTYPE_2D_SUNDER,
	EFFECTTYPE_2D_MAX
}EFFECTTYPE_2D;
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fRadius;			// 半径(大きさ)
	int nLife;				// 表示時間(寿命)
	int nCounterAnim;	// カウンター
	int nPatternAnim;	// パターン
	EFFECTTYPE_2D nType;		// 種類
	bool bUse;				// 使用しているかどうか
} EFFECT2D;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitEffect2D(void);
void UninitEffect2D(void);
void UpdateEffect2D(void);
void DrawEffect2D(void);

void SetEffect2D(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE_2D nType);
float SetRandomEffect2D(void);
EFFECT2D *GetEffect2D(void);
#endif
