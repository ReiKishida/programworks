//=============================================================================
//
// ポリゴンの処理 [player.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//出現状態
	PLAYERSTATE_NORMAL,		//通常状態
	PLAYERSTATE_CHARGE,		//通常状態
	PLAYERSTATE_TIRED,		//リロード状態
	PLAYERSTATE_A,
	PLAYERSTATE_B,
	PLAYERYSTATE_MAX			//種類の総数
}PLAYERSTATE;

typedef enum
{
	WEAPON_SPREAD = 0,	//スプレッド
	WEAPON_VALCAN,		//バルカン
	WEAPON_CANON,		//キャノン
	WEAPON_MAX			//最大数
}PLAYERWEAPON;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	PLAYERSTATE state;	//プレイヤーの状態
	int nCounterState;	//状態管理のカウンター
	int nCntCharge;
	int nCntStamina;		//スタミナカウント
	int nCounterAnim;
	int nPatternAnim;
	bool bDisp;			//使用されているか
}PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UnInitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer();

void HitPlayer(int nDamage);
#endif