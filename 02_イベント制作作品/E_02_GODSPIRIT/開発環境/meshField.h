////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッシュ床処理 [meshField.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	MESHFIRLDTYPE_GREEN = 0,
	MESHFIRLDTYPE_BRIDGE,
	MESHFIRLDTYPE_SEAJPG,
	MESHFIRLDTYPE_SEAPNG,
	MESHFIRLDTYPE_RENGA,
	MESHFIRLDTYPE_STAIRS,
	MESHFIRLDTYPE_GREENPNG,
	MESHFIRLDTYPE_WALL,
	MESHFIRLDTYPE_BLACK,
	MESHFIRLDTYPE_MAX,
}MESHFIRLDTYPE;
typedef struct
{
	int nNumTex;
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
	char *pFileName;	// テクスチャへのポインタ
}MeshFieldData;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXCOLOR col;						// 色
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	float fWidth;						// 幅
	float fDepth;						// 奥行き
	int nBlock_X;						// ブロックX
	int nBlock_Z;						// ブロックZ
	float fSlopeSin;					// 斜め
	float fSlopeSinMove;				// 斜めの角度計算
	float fSlopeCos;						// 斜め
	float fSlopeCosMove;					// 斜めの角度計算
	int nType;				// 種類
	float fMoveVtx;					// 頂点ごとの高さ
	D3DXVECTOR2 fTexmove;				// テクスチャの移動速度
	D3DXVECTOR2 fTexPos;				// テクスチャの移動速度
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
	bool bCull;							// カリングするかどうか
	bool bCollision;							// カリングするかどうか
	bool bUse;							// 使用されているかどうか
}MeshField;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight,MeshField **pMeshField);
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nBlock_X, int nBlock_Z, float fWidth, float fDepth,float fSlopeSin, bool bCull);
MeshField *GetMeshField(void);
MeshFieldData *GetMeshFieldData(void);
#endif
