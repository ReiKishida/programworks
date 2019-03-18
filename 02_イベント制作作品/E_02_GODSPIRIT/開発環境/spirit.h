////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// スピリット処理 [spirit.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SPIRIT_H_
#define _SPIRIT_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	SPIRITTEX_BG2 = 0,
	SPIRITTEX_GAGE,
	SPIRITTEX_BG,
	SPIRITTEX_MAX
}SPIRITTEX;

typedef struct
{
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
	char	*pFileName;	// テクスチャへのポインタ
}SpiritData;

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
	int nType;							// 種類
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
	int nPatternAnim;					// パターん
	int nCounterAnim;					// カウンター
	bool bUse;							// 使用されているかどうか
	bool bUseDown;
	int nDrawDown;						// 差分用
}Spirit;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitSpirit(void);
void UninitSpirit(void);
void UpdateSpirit(void);
void DrawSpirit(void);
void SetSpirit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bUseDown);
Spirit *GetSpirit(void);
SpiritData *GetSpiritData(void);
void SetPlayerSpirit(int nSpirit);
void GetPlayerSpirit(int nSpirit);
#endif
