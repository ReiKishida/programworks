//=============================================================================
//
// 弾の発射処理 [bullet.h]
// Author : 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BULLET (200)



//*****************************************************************************
// 壁の構造体
//*****************************************************************************

typedef enum
{
	BULLETTYPE_NONE = 0,
	BULLETTYPE_PLAYER,
	BULLETTYPE_PLAYER_HOMING,
	BULLETTYPE_ENEMY,
	BULLETTYPE_ENEMY_HOMING,
	BUULLETTYPE_MAX
}BULLETTYPE;


typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 posold;		//位置(中心座標)
	D3DXVECTOR3 rot;		//回転
	D3DXVECTOR3 move;		//移動速度
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	BULLETTYPE type;
	bool bUse;				//使用しているか
	int IndexShadow;		//影の番号
	int nLife;				//弾の寿命
	float fRadius;			//半径
	float fLimif;
}BULLET;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 VecA;	//ベクトル
	D3DXVECTOR3 VecB;	//ベクトル
	D3DXVECTOR3 VecC;	//ベクトル
	float fLimit;	//位置

}STAGE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);		//壁初期化
void UninitBullet(void);	//壁終了
void UpdateBullet(void);	//壁更新
void DrawBullet(void);		//壁描画

//void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 rot, int nLife);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, float fRadius, BULLETTYPE type);
bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void CollisionVec(void);
BULLET *GetBullet(void);
#endif
