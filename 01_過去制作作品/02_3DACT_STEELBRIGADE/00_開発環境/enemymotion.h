//=============================================================================
//
// モーション処理 [text.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _ENEMYMOTION_H_
#define _ENEMYMOTION_H_
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "stdio.h"
#include "conio.h"
#include "string.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 RotDest;
	D3DXVECTOR3 fRotAngle;

	D3DXVECTOR3 Pos;
	D3DXVECTOR3 PosDest;
	D3DXVECTOR3 fPosAngle;
}PartsEnemyMotion;




typedef struct
{
	int MAX_FLAME;
	int KeyFlame;
	int Loop;
	int nNumEnemyMotion;
	int nCntFlame;
	PartsEnemyMotion aParts[256];
}EnemyMotion;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void InitEnemyMotion(void);
void UninitEnemyMotion(void);
void UpdateEnemyMotion(void);
void SetEnemyMotion(int nNumEnemymotion);
EnemyMotion *GetEnemyMotion(void);

#endif

