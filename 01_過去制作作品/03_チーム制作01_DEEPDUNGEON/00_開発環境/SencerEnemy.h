//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERENEMY_H_
#define _SENCERENEMY_H_

#include "main.h"


//*****************************************************************************
// センサーの構造体
//*****************************************************************************
typedef enum
{
	SENCERENEMYTYPE_PLAYER = 0,
	SENCERENEMYTYPE_ENEMY,
	SENCERENEMYTYPE_MAX
}SENCERENEMYTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	SENCERENEMYTYPE type;
	bool bUse;
	float fRadius;
}SENCERENEMY;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerEnemy(void);		//センサー初期化
void UninitSencerEnemy(void);	//センサー終了
void UpdateSencerEnemy(void);	//センサー更新
void DrawSencerEnemy(void);		//センサー描画

void SetPositionSencerEnemy(int nIdxSencer, D3DXVECTOR3 pos);
void DeleteSencerEnemy(int nIdxSencerCol);

int SetSencerEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
int CollisionSencer_Lock_ReturnEnemy(D3DXVECTOR3 *pos, int nCntEnemy);

bool CollisionSencer_Enemy(D3DXVECTOR3 *pos, int nCntSencerEnemy);
bool CollisionSencer_Enemy_Arrow(D3DXVECTOR3 *pos);

#endif
