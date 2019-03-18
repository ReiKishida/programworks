////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 影処理 [shadow.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 向き
	D3DXCOLOR col;		// 色
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	float fWidth;		// 幅
	float fDepth;		// 奥行き
	bool bUse;			// 使用されているかどうか
}Shadow;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos,D3DXVECTOR3 rot,float fWidth,float fDepth);
void SetPositionShadow(int nIdyShadow, D3DXVECTOR3 pos, D3DXVECTOR3 posOld);
void SetColorShadow(int nIdyShadow, float fAlpha);
void DeleteShadow(int nIdyShadow);
Shadow *GetShadow(void);
#endif
