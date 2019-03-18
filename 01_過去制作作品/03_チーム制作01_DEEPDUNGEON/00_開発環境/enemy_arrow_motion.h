//=============================================================================
//
// アニメーションの処理 [motion.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYARROWMOTION_H_
#define _ENEMYARROWMOTION_H_

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
}EnemyArrowPartsMotion;

typedef struct
{
	int			MAX_FLAME;			// フレーム数
	int			KeyFlame;			// キーフレーム
	bool		Loop;				// ループするかどうか
	int			nNumEnemyArrowMotion;	// モーションの番号
	int			nCntFlame;			// フレームカウンター
	EnemyArrowPartsMotion aParts[256];
}EnemyArrowMotion;

//------------------------------------------------------------------------
// プロトタイプ宣言														  |
//------------------------------------------------------------------------
void InitEnemyArrowMotion(void);
void UpdateEnemyArrowMotion(void);
void SetEnemyArrowMotion(int nNumEnemymotion);
EnemyArrowMotion *GetEnemyArrowMotion(void);

#endif