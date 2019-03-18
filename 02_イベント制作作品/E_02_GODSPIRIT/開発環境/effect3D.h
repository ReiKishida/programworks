////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクト３D処理 [effect3D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	EFFECT3D_ARM = 0,
	EFFECT3D_ARM2,
	EFFECT3D_SORD ,
	EFFECT3D_MAX,
}EFFECT3DTYPE;

typedef struct
{
	LPD3DXMESH pMesh;							// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;						// マテリアル情報へのポインタ
	DWORD nNumMat;								// マテリアル情報の数
	LPDIRECT3DTEXTURE9 *pTextuer;			// テクスチャ
	char *pFileName;
}Effect3DData;
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 posOld;					// 前回の位置
	D3DXVECTOR3 Initpos;				// 初期位置
	D3DXVECTOR3 move;					// 移動量
	D3DXCOLOR col;						// カラー
	EFFECT3DTYPE nType;					// 種類
	D3DXVECTOR3 Diffrot;				// 角度の差分
	D3DXVECTOR3 Destrot;				// 目的の角度
	D3DXVECTOR3 rot;					// 角度
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	int nIdyShadow;						// 影の番号
	int nIdy;						// 番号
	bool bCollision;					// 当たり判定
	bool bUse;
	bool bDraw;
}Effect3D;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitEffect3D(void);
void UninitEffect3D(void);
void UpdateEffect3D(void);
void DrawEffect3D(void);
void SetEffect3D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, EFFECT3DTYPE nType);
Effect3D *GetEffect3D(void);
Effect3DData *GetEffect3DData(void);
#endif
