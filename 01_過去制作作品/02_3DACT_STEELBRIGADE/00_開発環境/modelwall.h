//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODELWALL_H_
#define _MODELWALL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODELWALL (15)
#define MAXTYPE_MODELWALL (5)

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//テクスチャへのポインタ
	LPD3DXMESH pMesh = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMat;			//マテリアル情報の数
	D3DXVECTOR3 VtxMinModelWall = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModelWall = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
}MODELWALL;

typedef struct
{
	DWORD g_nNumMatModel = 0;	//マテリアルの情報の数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
						//DWORD g_nNumMatModel;	//マテリアルの情報の数

	D3DXVECTOR3 VtxMinModelWall;
	D3DXVECTOR3 VtxMaxModelWall;

	D3DXMATRIX mtxWorld;	//ワールドマトリックス

	int g_nIndexShadow;	//影の番号
	int nCntWall;
	int nType;
	bool bUse;
	MODELWALL aBuilding[MAX_MODELWALL];
}WALL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModelWall(void);
void UninitModelWall(void);
void UpdateModelWall(void);
void DrawModelWall(void);

bool CollisionModelWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetModelWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

WALL *GetModelWall(void);
#endif
