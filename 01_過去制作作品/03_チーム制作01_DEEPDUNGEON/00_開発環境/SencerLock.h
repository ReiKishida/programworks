//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERLOCK_H_
#define _SENCERLOCK_H_

#include "main.h"


//*****************************************************************************
// センサーの構造体
//*****************************************************************************
typedef enum
{
	SENCERLOCKTYPE_PLAYER = 0,
	SENCERLOCKTYPE_ENEMY,
	SENCERLOCKTYPE_MAX
}SENCERLOCKTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	SENCERLOCKTYPE type;
	bool bUse;
	float fRadius;
}SENCERLOCK;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerLock(void);		//センサー初期化
void UninitSencerLock(void);	//センサー終了
void UpdateSencerLock(void);	//センサー更新
void DrawSencerLock(void);		//センサー描画

void SetPositionSencerLock(int nIdxSencer, D3DXVECTOR3 pos);
void DeleteSencerLock(int nIdxSencerCol);

int SetSencerLock(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
int CollisionSencer_Lock_ReturnEnemy(D3DXVECTOR3 *pos, int nCntEnemy);

bool CollisionSencer_Lock(D3DXVECTOR3 *pos);

#endif
