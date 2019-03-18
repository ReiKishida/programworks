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
// マクロ定義
//*****************************************************************************
#define JUMP_POWER (-23.2f)	//ジャンプ力
#define GRAVITY (1.2f)		//重力
#define GRAVITY_MAX (50.0f)	//重力の最大量



//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//出現状態
	PLAYERSTATE_NORMAL,		//通常状態
	PLAYERSTATE_DAMAGE,		//ダメージ状態
	PLAYERYSTATE_MAX			//種類の総数
}PLAYERSTATE;



typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//前回の位置
	D3DXVECTOR3 move;	//移動量
	PLAYERSTATE state;	//プレイヤーの状態
	int nCounterState;	//ステートのカウンター
	int nCounterAnim;	//アニメーションパターンのカウント
	int nPatternAnim;	//アニメーションパターン
	int nJumpCount;		//ジャンプのカウント
	int nStock;			//残機
	bool bDisp;			//使用されているか
	bool bPress;		//押されているか
	bool bUseJump;	//ジャンプが使用されているか
	float fWidth;		//幅
	float fHeight;		//高さ
	//BLOCK *pBlock;		//ブロック
}PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UnInitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER GetPlayer();

bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif