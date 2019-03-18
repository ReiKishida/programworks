////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 円の処理 [meshFar.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHFAR_H_
#define _MESHFAR_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	int nNumTex;
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
	char *pFileName;	// テクスチャへのポインタ

}MeshFarData;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXCOLOR col;						// 色
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	float fDepth;						// 奥行き
	int nBlock_X;						// ブロックX
	int nBlock_Z;						// ブロックZ
	int nType;				// 種類
	float fMoveVtx;					// 頂点ごとの高さ
	float fTexmove;				// テクスチャの移動速度
	float fTexPos;				// テクスチャの移動速度
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
	bool bCull;							// カリングするかどうか
	bool bUse;							// 使用されているかどうか
}MeshFar;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitMeshFar(void);
void UninitMeshFar(void);
void UpdateMeshFar(void);
void DrawMeshFar(void);
void SetMeshFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nBlock_X, int nBlock_Z,  float fDepth, bool bCull);
MeshFar *GetMeshFar(void);
MeshFarData *GetMeshFarData(void);
#endif
