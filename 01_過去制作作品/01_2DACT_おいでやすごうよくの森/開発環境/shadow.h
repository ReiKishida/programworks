//=============================================================================
//
// エフェクトの処理 [Shadow.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOWJUMP_POWER (-23.0f)	//ジャンプ力
#define SHADOWGRAVITY (1.2f)		//重力
#define SHADOWGRAVITY_MAX (50.0f)	//重力の最大量

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	SHADOWSTATE_APPEAR = 0,	//出現状態
	SHADOWSTATE_NORMAL,		//通常状態
	SHADOWSTATE_DAMAGE,		//ダメージ状態
	SHADOWSTATE_MAX			//種類の総数
}SHADOWSTATE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//前回の位置
	D3DXVECTOR3 move;	//移動量
	SHADOWSTATE state;
	int nCounterState;	//ステートのカウンター
	int nCounterAnim;	//アニメーションパターンのカウント
	int nPatternAnim;	//アニメーションパターン
	int nJumpCount;		//ジャンプのカウント
	bool bDisp;			//使用されているか
	bool bPress;		//押されているか
	bool bUseJump;	//ジャンプが使用されているか
	float fWidth;		//幅
	float fHeight;		//高さ
						//BLOCK *pBlock;		//ブロック
}SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShadow(void);
void UnInitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

//void SetShadow(D3DXVECTOR3 pos, float fRadius, int nLife);
//void SetShadow(D3DXVECTOR3 pos);

#endif
