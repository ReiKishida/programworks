//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTICLE (2800)

//=============================================================================
// 壁の構造体
//=============================================================================
typedef enum
{
	PARTICLETYPE_SHOWER = 0, //シャワー
	PARTICLETYPE_EXPLOSION,	//爆発
	PARTICLETYPE_SMOKE,		//煙
	PARTICLETYPE_BULLET,
	PARTICLETYPE_MAX
}PARTICLETYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 posold;		//前回の位置(中心座標)
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//回転
	D3DXCOLOR col;			//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nLife;				//ライフ
	bool bUse;				//使用しているか
	float fRadius;			//半径
	PARTICLETYPE type;		//パーティクルの種類
}PARTICLE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitParticle(void);		//パーティクル初期化
void UninitParticle(void);		//パーティクル終了
void UpdateParticle(void);		//パーティクル更新
void DrawParticle(void);		//パーティクル描画

void SetParticle(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col, PARTICLETYPE type);
#endif
