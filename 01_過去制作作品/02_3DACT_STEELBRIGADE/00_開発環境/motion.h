//=============================================================================
//
// モーション処理 [text.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_
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
}PartsMotion;




typedef struct
{
	int MAX_FLAME;
	int KeyFlame;
	int Loop;
	int nNumPlayerMotion;
	int nCntFlame;
	PartsMotion aParts[256];
}Motion;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void InitMotion(void);
void UninitMotion(void);
void UpdateMotion(void);
void SetMotion(int nNumPlayermotion);
Motion *GetMotion(void);

#endif

