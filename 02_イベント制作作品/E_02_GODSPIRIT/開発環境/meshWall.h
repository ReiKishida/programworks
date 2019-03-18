////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッシュ壁処理 [meshWall.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	int nNumTex;
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
	char	*pFileName;	// テクスチャへのポインタ
}MeshWallData;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXCOLOR col;						// 色
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	float fWidth;						// 幅
	float fHeight;						// 高さ
	int nBlock_X;						// ブロックX
	int nBlock_Y;						// ブロックY
	int nType;					// 種類
	float fMoveVtx;						// 頂点ごとの動き
	float fTexmoveMesh;					// テクスチャの移動速度
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
	bool bCull;							// カリングするかしないか
	bool bCollision;					// 判定あるかないか
	bool bUse;							// 使用されているかどうか
}MeshWall;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
bool CollisionMeshWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight);
bool CollisionVecMeshWall(D3DXVECTOR3 pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bCull);
MeshWall *GetMeshWall(void);
MeshWallData *GetMeshWallData(void);

#endif
