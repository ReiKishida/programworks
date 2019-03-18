//=============================================================================
//
// アニメーションの処理 [motion.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYMOTION05_H_
#define _ENEMYMOTION05_H_

#include "main.h"

// モデルの構造体
typedef struct
{
	D3DXVECTOR3 Rot;			// 現在の向き
	D3DXVECTOR3 RotDest;		// 目標の向き
	D3DXVECTOR3 fRotAngle;		// 向きの差分

	D3DXVECTOR3 Pos;			// 現在の座標
	D3DXVECTOR3 PosDest;		// 目標の座標
	D3DXVECTOR3 fPosAngle;		// 座標の差分
}EnemyPartsMotion_5;

typedef struct
{
	int			MAX_FLAME;			// フレーム数
	int			KeyFlame;			// キーフレーム
	bool		Loop;				// ループするかどうか
	int			nNumEnemymotion;	// モーションの番号
	int			nCntFlame;			// フレームカウンター
	EnemyPartsMotion_5 aParts[256];
}EnemyMotion_5;

//------------------------------------------------------------------------
// プロトタイプ宣言														  |
//------------------------------------------------------------------------
void InitEnemyMotion_5(void);
void UpdateEnemyMotion_5(void);
void SetEnemyMotion_5(int nNumEnemymotion);
EnemyMotion_5 *GetEnemyMotion_5(void);

#endif