//=============================================================================
//
// 敵処理 [enemy.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ENEMY	(128)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//通常状態
	ENEMYSTATE_DAMAGE,		//ダメージ状態
	ENEMYSTATE_MAX			//種類の総数
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	ENEMYSTATE state;	//敵の状態
	int nCountAnim;		//アニメーションカウンター
	int nPatternAnim;	//アニメーションパターンNo.
	int nCounterState;	//状態管理のカウンター
	int nType;			//種類
	int nLife;			//敵の体力
	int nCountTime;		//カウント
	float fSpeed;		//速さ
	bool bUse;			//使用しているかどうか
}ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UnInitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void HitEnemy(int nCntEnemy, int nDamage);

void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife);
ENEMY *GetEnemy(void);


#endif
