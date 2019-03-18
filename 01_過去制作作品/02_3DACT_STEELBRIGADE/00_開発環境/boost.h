//=============================================================================
//
// ブーストゲージ処理 [life.h]
// Author : 
//
//=============================================================================
#ifndef _BOOST_H_
#define _BOOST_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//****************************************************************************
#define MAX_BOOST (1)
//*****************************************************************************
// 構造体宣言
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 ParentPos;
	D3DXVECTOR3 ParentRot;
	D3DXCOLOR col;
	bool bUse;
}BOOST;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBoost(void);		//ブーストゲージ初期化
void UninitBoost(void);	//ブーストゲージ終了
void UpdateBoost(void);	//ブーストゲージ更新
void DrawBoost(void);		//ブーストゲージ描画

void UseBoost(int nUseBoost);

int GetBoost(void);

#endif
