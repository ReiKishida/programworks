//=============================================================================
//
// 発射位置処理 [shoot.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SHOOT_H_
#define _SHOOT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_SHOOT	(128)	// 発射位置の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	SHOOTTYPE_NORMAL = 0,	//ノーマル
	SHOOTTYPE_FEAVER,	//フィーバー
	SHOOTTYPE_MAX
}SHOOTTYPPE;


typedef enum
{
	SHOOTDIRECTION_0 = 0,	//左
	SHOOTDIRECTION_1,	//右
	SHOOTDIRECTION_MAX
}SHOOTDIRECTION;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold; //移動前位置
	D3DXVECTOR3 rot;
	D3DXVECTOR3 FallenPoint;
	D3DXVECTOR3 move;   //移動量
	SHOOTTYPPE type;	//状態
	SHOOTDIRECTION direction;
	float fWidth;		//幅
	float fHeight;		//高さ
	float fLength;
	float fAngle;
	bool bUse;			//使用しているかどうか
}SHOOT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShoot(void);
void UnInitShoot(void);
void UpdateShoot(void);
void DrawShoot(void);


 

void SetShoot(D3DXVECTOR3 pos);
//void SetShoot(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, ShootSTATE state);

SHOOT *GetShoot(void);

//bool CollisionShoot(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
