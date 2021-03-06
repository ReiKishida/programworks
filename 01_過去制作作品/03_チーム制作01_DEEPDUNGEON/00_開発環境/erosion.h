//=============================================================================
//
// 浸食度処理 [erosion.h]
// Author : 侵食度 
//
//=============================================================================
#ifndef _EROSION_H_
#define _EROSION_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//****************************************************************************
#define MAX_EROSION (2)
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
	int nErosion;		//見かけの浸食度
	int nErosionNum;		//内部数値

}EROSION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitErosion(void);		//ブーストゲージ初期化
void UninitErosion(void);	//ブーストゲージ終了
void UpdateErosion(void);	//ブーストゲージ更新
void DrawErosion(void);		//ブーストゲージ描画

void IncreaseErosion(int nNumErosion);

int GetnCntErosion(void);

#endif
