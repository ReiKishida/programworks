//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERARROW_H_
#define _SENCERARROW_H_

#include "main.h"


//*****************************************************************************
// センサーの構造体
//*****************************************************************************
typedef enum
{
	SENCERARROWTYPE_PLAYER = 0,
	SENCERARROWTYPE_ENEMY,
	SENCERARROWTYPE_MAX
}SENCERARROWTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	SENCERARROWTYPE type;
	bool bUse;
	float fRadius;
}SENCERARROW;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerEnemyArrow(void);		//センサー初期化
void UninitSencerEnemyArrow(void);	//センサー終了
void UpdateSencerEnemyArrow(void);	//センサー更新
void DrawSencerEnemyArrow(void);		//センサー描画

void SetPositionSencerEnemyArrow(int nIdxSencer, D3DXVECTOR3 pos);
void DeleteSencerEnemyArrow(int nIdxSencerCol);

int SetSencerEnemyArrow(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

bool CollisionSencer_EnemyArrow(D3DXVECTOR3 *pos, int nCntSencerEnemy);

#endif
