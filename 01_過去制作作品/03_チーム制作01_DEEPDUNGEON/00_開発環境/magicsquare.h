//=============================================================================
//
// 魔法陣処理 [magicsquare.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _MAGICSQUARE_H_
#define _MAGICSQUARE_H_

#include "main.h"

//=============================================================================
// 影の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 回転
	D3DXCOLOR col;				//カラー
	D3DXMATRIX mtxWorldMagicSquare;	// ワールドマトリックス
	float fRadius;				// 幅
	float fWidth;				// 幅
	float fDepth;				// 奥行
	int nIdxSencer;				//センサーの番号
	bool bUse;
}MAGICSQUARE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitMagicSquare(void);
void UninitMagicSquare(void);
void UpdateMagicSquare(void);
void DrawMagicSquare(void);
int SetMagicSquare(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius);
void DeleteMagicSquare(int nIdxMagicSquare);

void DeleteOnlyMagicSquare(int nCntMagicsquare);
MAGICSQUARE *GetMagicSquare(void);

#endif
