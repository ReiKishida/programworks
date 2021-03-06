//=============================================================================
//
// ブーストゲージ処理 [life.h]
// Author : 
//
//=============================================================================
#ifndef _BRAZEN_H_
#define _BRAZEN_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//****************************************************************************
#define MAX_BRAZEN (2)
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
	int nBrazen;
	bool bUse;
}BRAZEN;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBrazen(void);		//ブーストゲージ初期化
void UninitBrazen(void);	//ブーストゲージ終了
void UpdateBrazen(void);	//ブーストゲージ更新
void DrawBrazen(void);		//ブーストゲージ描画

void DecreaseBrazen(int nNumBrazen);

int GetBrazen(void);

#endif
