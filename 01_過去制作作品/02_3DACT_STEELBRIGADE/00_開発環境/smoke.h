//=============================================================================
//
// 煙処理 [smoke.h]
// Author : 
//
//=============================================================================
#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SMOKE (50)



//*****************************************************************************
// 壁の構造体
//*****************************************************************************


typedef enum
{
	SMOKETYPE_SHOWER = 0, //シャワー
	SMOKETYPE_EXPLOSION,	//爆発
	SMOKETYPE_SMOKE,		//煙
	SMOKETYPE_MAX
}SMOKETYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 posold;		//前回の位置(中心座標)
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//回転
	D3DXCOLOR col;			//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nLife;				//ライフ
	bool bUse;				//使用しているか
	float fRadius;			//半径
	SMOKETYPE type;		//パーティクルの種類
}SMOKE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSmoke(void);		//パーティクル初期化
void UninitSmoke(void);	//パーティクル終了
void UpdateSmoke(void);	//パーティクル更新
void DrawSmoke(void);		//パーティクル描画

void SetSmoke(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col);
#endif
