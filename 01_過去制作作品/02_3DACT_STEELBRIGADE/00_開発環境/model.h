//=============================================================================
//
// モデル処理 [model.h]
// Author :
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODEL (11)
#define MAXTYPE_MODEL (4)

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//テクスチャへのポインタ
	LPD3DXMESH pMesh = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMat;			//マテリアル情報の数
	D3DXVECTOR3 VtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
}OBJECT;

typedef struct
{
	DWORD g_nNumMatModel = 0;	//マテリアルの情報の数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 VtxMinModel;
	D3DXVECTOR3 VtxMaxModel;
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int g_nIndexShadow;	//影の番号
	int nType;
	bool bUse;
	OBJECT aObject[MAX_MODEL];
}MODEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

MODEL *GetModel(void);
#endif
