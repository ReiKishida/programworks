//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _PEDESTAL_H_
#define _PEDESTAL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PEDESTAL (10)

//*****************************************************************************
// モデルの構造体
//*****************************************************************************

typedef struct
{
	DWORD g_nNumMatModel = 0;	//マテリアルの情報の数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
						//DWORD g_nNumMatModel;	//マテリアルの情報の数

	D3DXVECTOR3 VtxMinPedestal;
	D3DXVECTOR3 VtxMaxPedestal;

	D3DXMATRIX mtxWorld;	//ワールドマトリックス

	int g_nIndexShadow;	//影の番号
	int nType;
	bool bUse;

}PEDESTAL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPedestal(void);
void UninitPedestal(void);
void UpdatePedestal(void);
void DrawPedestal(void);

//bool CollisionBill(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
bool CollisionPedestal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetPedestal(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

PEDESTAL *GetPedestal(void);
#endif
