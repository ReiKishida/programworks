////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ビルボードライフ処理 [BillLife.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _BILLLIFE_H_
#define _BILLLIFE_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	BILLLIFETYPE_ENEMYBG = 0,
	BILLLIFETYPE_ENEMYBGRED,
	BILLLIFETYPE_ENEMY,
	BILLLIFETYPE_MAX
}BILLLIFETYPE;
typedef struct
{
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
	char	*pFileName;	// テクスチャへのポインタ
}BillLifeData;

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
	BILLLIFETYPE nType;					// 種類
	float fMoveVtx;						// 頂点ごとの動き
	float fTexmoveMesh;					// テクスチャの移動速度
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
	int nLife;							// ライフ
	int nInitLife;						// 初期ライフ
	bool bUseDown;						// なくなるやつかどうか
	bool bDown;							// なくなっている最中かどうか
	int nDrawDown;						// 差分用
	bool bDownRed;						// 赤いライフ
	int nDownRedCounter;				// 赤いライフ減少間隔保管
	int nIdy;							// 番号
	float fOffset;						// オフセット
	int nCntTime;						// 死ぬ時間
	bool bUse;							// 使用されているかどうか
}BillLife;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitBillLife(void);
void UninitBillLife(void);
void UpdateBillLife(void);
void DrawBillLife(void);
void SetBillLife(int nIdy, D3DXVECTOR3 pos,int nLife,D3DXCOLOR col, BILLLIFETYPE nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight,float fOffset, bool bUseDown);
BillLife *GetBillLife(void);
BillLifeData *GetBillLifeData(void);
void GetBillLife(int nIdy, int nLife);
void SetPositionBillLife(int nIdy, D3DXVECTOR3 pos);
void DeleteBillLife(int nIdy);
#endif
