//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERENABLEATTACK_H_
#define _SENCERENABLEATTACK_H_

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
}SENCERENABLEATTACK;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerEnableAttack(void);		//センサー初期化
void UninitSencerEnableAttack(void);	//センサー終了
void UpdateSencerEnableAttack(void);	//センサー更新
void DrawSencerEnableAttack(void);		//センサー描画

void SetPositionSencerEnableAttack(int nIdxSencer, D3DXVECTOR3 pos);
void DeleteSencerEnableAttack(int nIdxSencerCol);

int SetSencerEnableAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

bool CollisionSencer_Enable(D3DXVECTOR3 *pos);

#endif
