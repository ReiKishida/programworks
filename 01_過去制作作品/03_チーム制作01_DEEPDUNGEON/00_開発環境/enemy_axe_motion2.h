//=============================================================================
//
// アニメーションの処理 [motion.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYAXEMOTION2_H_
#define _ENEMYAXEMOTION2_H_

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
}EnemyAxePartsMotion2;

typedef struct
{
	int			MAX_FLAME;			// フレーム数
	int			KeyFlame;			// キーフレーム
	bool		Loop;				// ループするかどうか
	int			nNumEnemyAxeMotion;	// モーションの番号
	int			nCntFlame;			// フレームカウンター
	EnemyAxePartsMotion2 aParts[256];
}EnemyAxeMotion2;

//------------------------------------------------------------------------
// プロトタイプ宣言														  |
//------------------------------------------------------------------------
void InitEnemyAxeMotion2(void);
void UpdateEnemyAxeMotion2(void);
void SetEnemyAxeMotion2(int nNumEnemyAxemotion);
EnemyAxeMotion2 *GetEnemyAxeMotion2(void);

#endif