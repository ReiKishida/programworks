//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : komatsu
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "block.h"

//=============================================================================
// プレーヤーの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	float fHeight;
	float fWidth;
	int nCounterAnim;
	int nPatternAnim;
	int nDirectionMove;
	bool bChange;
	bool bItem;
}Player;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif