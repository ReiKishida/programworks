////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ビルボードの処理 [billboard.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	BILLBOARDTYPE_TREE = 0,
	BILLBOARDTYPE_BAMBOO ,
	BILLBOARDTYPE_MAX
}BILLBOARDTYPE;
typedef struct
{
	int nNumTex;
	LPDIRECT3DTEXTURE9	pTexture;	// テクスチャへのポインタ
	char *pFileName;	// テクスチャへのポインタ
}BillboardData;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 前回の位置
	D3DXVECTOR3 Initpos;// 初期位置
	D3DXVECTOR3 move;	// 速さ
	D3DXVECTOR3 rot;	// 向き
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	float fWidth;		// 幅
	float fHeight;		// 高さ
	float fDepth;		// 奥行き
	BILLBOARDTYPE nType;	// 種類
	int nIdyShadow;		// 影のID
	bool bUse;			// 使用されているかどうか
}Billboard;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos,D3DXVECTOR3 rot, BILLBOARDTYPE nType,float fWidth,float fHeight);
Billboard *GetBillboard(void);
BillboardData *GetBillboardData(void);
#endif
