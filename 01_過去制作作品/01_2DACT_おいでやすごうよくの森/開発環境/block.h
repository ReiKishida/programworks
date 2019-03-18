//=============================================================================
//
// 敵処理 [Block.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOC_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BLOCK	(128)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BLOCKSTATE_NORMAL = 0,	//通常状態
	BLOCKSTATE_NORMAL2,	//通常状態2
	BLOCKSTATE_MOVE_X,	//移動床_X
	BLOCKSTATE_MOVE_Y,	//移動床_Y
	BLOCKSTATE_MOVE_XY,	//移動床_XY
	BLOCKSTATE_SPRING,	//スプリング床
	BLOCKSTATE_CRASH,	//壊れる床
	BLOCKSTATE_AREY,	//鉄アレイ
	BLOCKSTATE_MAX
}BLOCKSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold; //移動前位置
	D3DXVECTOR3 FallenPoint;
	D3DXVECTOR3 move;   //移動量
	BLOCKSTATE state;	//状態
	int nType;			//種類
	float fWidth;		//幅
	float fHeight;		//高さ
	float Move_X;	//移動量のカウント(X)
	float Move_Y;	//移動量のカウント(Y)
	
	bool bUse;			//使用しているかどうか
}BLOCK;

//*****************************************************************************
// グローバル変数
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBlock(void);
void UnInitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, BLOCKSTATE state, float fMove_X, float fMove_Y);

BLOCK *GetBlock(void);

bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
bool CollisionBlockItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fLength);
bool CollisionBlockItem2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif
