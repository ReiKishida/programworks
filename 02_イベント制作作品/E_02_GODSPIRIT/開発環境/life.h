////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ライフ処理 [life.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	LIFETEX_ICONBG = 0,
	LIFETEX_ICON,
	LIFETEX_BG2,
	LIFETEX_GAGERED,
	LIFETEX_GAGE,
	LIFETEX_BG,
	LIFETEX_MAX
}LIFETEX;

typedef struct
{
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
	char	*pFileName;	// テクスチャへのポインタ
}LifeData;

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
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
	int nPatternAnim;					// パターん
	int nCounterAnim;					// カウンター
	bool bUse;							// 使用されているかどうか
	bool bUseDown;
	int nDrawDown;						// 差分用
	bool bDownRed;						// 赤いライフ
	int nDownRedCounter;				// 赤いライフ減少間隔保管
}Life;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
void SetLife(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bUseDown);
Life *GetLife(void);
LifeData *GetLifeData(void);
void SetPlayerLife(int nLife);
void GetPlayerLife(int nLife);
#endif
