//=============================================================================
//
// アニメーションの処理 [motion.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYARCHERMOTION02_H_
#define _ENEMYARCHERMOTION02_H_

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
}EnemyArcherPartsMotion_2;

typedef struct
{
	int			MAX_FLAME;			// フレーム数
	int			KeyFlame;			// キーフレーム
	bool		Loop;				// ループするかどうか
	int			nNumEnemymotion;	// モーションの番号
	int			nCntFlame;			// フレームカウンター
	EnemyArcherPartsMotion_2 aParts[256];
}EnemyArcherMotion_2;

//------------------------------------------------------------------------
// プロトタイプ宣言														  |
//------------------------------------------------------------------------
void InitEnemyArcherMotion_2(void);
void UpdateEnemyArcherMotion_2(void);
void SetEnemyArcherMotion_2(int nNumEnemymotion);
EnemyArcherMotion_2 *GetEnemyArcherMotion_2(void);

#endif