//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCER_H_
#define _SENCER_H_

#include "main.h"


//*****************************************************************************
// センサーの構造体
//*****************************************************************************
typedef enum
{
	SENCERTYPE_NONE = 0,
	SENCERTYPE_MAGICSQUARE,
	SENCERTYPE_ATACK,
	SENCERTYPE_NEXTSTAGE,
	SENCERTYPE_COLLISION,
	MAXTYPE_SENCER
}SENCERTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;
	float fRadius;
	SENCERTYPE type;
}SENCER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencer(void);		//センサー初期化
void UninitSencer(void);	//センサー終了
void UpdateSencer(void);	//センサー更新
void DrawSencer(void);		//センサー描画

void DeleteSencer(int nIdxSencer);
int SetSencer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, SENCERTYPE type);
bool CollisionSencer(D3DXVECTOR3 *pos);
int CollisionSencerReturnCnt(D3DXVECTOR3 *pos);

#endif
