//=============================================================================
//
// 攻撃用センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERATK_H_
#define _SENCERATK_H_

#include "main.h"


//*****************************************************************************
// センサーの構造体
//*****************************************************************************
typedef enum
{
	SENCERATKTYPE_NONE = 0,
	SENCERATKTYPE_MAGICSQUARE,
	SENCERATKTYPE_ATACK,
	SENCERATKTYPE_NEXTSTAGE,
	SENCERATKTYPE_COLLISION,
	MAXTYPE_SENCERATK
}SENCERATKTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;
	float fRadius;
	SENCERATKTYPE type;
	int nIdxParent;
}SENCERATK;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerAtk(void);		//センサー初期化
void UninitSencerAtk(void);	//センサー終了
void UpdateSencerAtk(void);	//センサー更新
void DrawSencerAtk(void);		//センサー描画


void SetSencerAtk(D3DXVECTOR3 pos);
bool CollisionSencerAtk(float *fRadius, D3DXVECTOR3 *pos);
int CollisionSencerReturnCntAtk(D3DXVECTOR3 *pos);

#endif
