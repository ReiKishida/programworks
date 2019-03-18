//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _CANON_H_
#define _CANON_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CANON (5)
#define MAXTYPE_CANON (2)

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef enum
{
	CANONTYPE_G,
	CANONTYPE_C,
	CANONTYPE_MAX
}CANONTYPE;

typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//テクスチャへのポインタ
	LPD3DXMESH pMesh = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMat;			//マテリアル情報の数
	D3DXVECTOR3 VtxMinModelWall = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModelWall = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
}CANONMODEL;

typedef struct
{
	DWORD g_nNumMatModel = 0;	//マテリアルの情報の数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
						//DWORD g_nNumMatModel;	//マテリアルの情報の数
	CANONTYPE type;

	D3DXVECTOR3 VtxMinCanon;
	D3DXVECTOR3 VtxMaxCanon;

	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nLife;
	int g_nIndexShadow;	//影の番号
	int nType;
	bool bUse;
	CANONMODEL aCanonModel[MAX_CANON];
}CANON;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCanon(void);
void UninitCanon(void);
void UpdateCanon(void);
void DrawCanon(void);
void HitCanon(int nDamage, int nCntCanon);

bool CollisionCanon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetCanon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife);

CANON *GetCanon(void);
#endif
