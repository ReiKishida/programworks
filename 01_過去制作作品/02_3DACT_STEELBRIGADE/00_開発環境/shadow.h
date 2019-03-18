//=============================================================================
//
// 影の処理 [shadow.h]
// Author : 
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SHADOW (30)

//*****************************************************************************
// 影の構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き	
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
	float fWidth;			//影の幅
	float fDepth;			//影の奥行き
}SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShadow(void);		//シャドウ初期化
void UninitShadow(void);	//シャドウ終了
void UpdateShadow(void);	//シャドウ更新
void DrawShadow(void);		//シャドウ描画

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float Depth);

void SetPositionShadow(int IndexShadow, D3DXVECTOR3 pos);
void SetPositionBulletShadow(int IndexShadow, D3DXVECTOR3 pos);

void DeleteShadow(int IndexShadow);
void SetScaleShadow(int IndexShadow);

#endif
