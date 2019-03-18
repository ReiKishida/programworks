//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERNEXT_H_
#define _SENCERNEXT_H_

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
}SENCERNEXT;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerNext(void);		//センサー初期化
void UninitSencerNext(void);	//センサー終了
void UpdateSencerNext(void);	//センサー更新
void DrawSencerNext(void);		//センサー描画

int SetSencerNext(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
bool CollisionSencerNext(D3DXVECTOR3 *pos);

void DeleteSencerNext(int nCnt);
#endif
