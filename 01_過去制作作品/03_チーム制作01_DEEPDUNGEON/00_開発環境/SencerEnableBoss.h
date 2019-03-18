//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERENABLEBOSS_H_
#define _SENCERENABLEBOSS_H_

#include "main.h"


//*****************************************************************************
// センサーの構造体
//*****************************************************************************

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;
	float fRadius;
}SENCERENABLEBOSS;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerEnableBoss(void);		//センサー初期化
void UninitSencerEnableBoss(void);	//センサー終了
void UpdateSencerEnableBoss(void);	//センサー更新
void DrawSencerEnableBoss(void);		//センサー描画

void SetPositionSencerEnableBoss(int nIdxSencer, D3DXVECTOR3 pos);
void DeleteSencerEnableBoss(int nIdxSencerCol);

int SetSencerEnableBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

bool CollisionSencer_EnableBoss(D3DXVECTOR3 *pos);

#endif
