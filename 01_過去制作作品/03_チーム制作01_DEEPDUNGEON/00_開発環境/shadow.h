//=============================================================================
//
// 影処理 [shadow.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//=============================================================================
// 影の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX mtxWorldShadow;	// ワールドマトリックス
	float fWidth;				// 幅
	float fDepth;				// 奥行
	bool bUse;
}SHADOW;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void DeletShadow(int nIdxShadow);

#endif
