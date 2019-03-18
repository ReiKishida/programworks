//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : 
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshField(void);		//ポリゴン初期化
void UninitMeshField(void);	//ポリゴン終了
void UpdateMeshField(void);	//ポリゴン更新
void DrawMeshField(void);		//ポリゴン描画

//bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

#endif
