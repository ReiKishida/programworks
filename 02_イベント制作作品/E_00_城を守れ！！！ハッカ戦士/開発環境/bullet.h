//=============================================================================
//
// 弾処理 [bullet.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,		//プレイヤーの弾
	BULLETTYPE_ENEMY,
	BULLETYPE_MAX				//種類の総数
}BULLETTYPE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	BULLETTYPE type;	//弾の種類
	float fRadius;		//弾の半径
	float fangle;		//角度
	int nLife;			//弾の寿命
	bool bUse;			//使用しているかどうか
} Bullet;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UnInitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move , BULLETTYPE type, float fRadius, int nLife);

Bullet *GetBullet(void);
#endif
