//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _GATE_H_
#define _GATE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_GATE (3)

//*****************************************************************************
// モデルの構造体
//*****************************************************************************

typedef struct
{
	DWORD g_nNumMatModel = 0;	//マテリアルの情報の数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 VtxMinGate;
	D3DXVECTOR3 VtxMaxGate;
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int g_nIndexShadow;	//影の番号
	int nType;
	bool bUse;
	int nLife;
	int nCnt;
}GATE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGate(void);
void UninitGate(void);
void UpdateGate(void);
void DrawGate(void);

bool CollisionGate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);

void HitGate(int nDamage, int nCntGate);

GATE *GetGate(void);
#endif
