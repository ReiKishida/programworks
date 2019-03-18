//=============================================================================
//
// キャラクター処理 [character.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_CHARACTER	(128)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	CHARACTERSTATE_NORMAL = 0,	//通常状態
	CHARACTERSTATE_DAMAGE,		//ダメージ状態
	CHARACTERSTATE_MAX			//種類の総数
}CHARACTERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 move;	//移動量
	CHARACTERSTATE state;	//敵の状態
	//int nCountAnim;		//アニメーションカウンター
	//int nPatternAnim;	//アニメーションパターンNo.
	float fWidth;
	float fHeight;
	int nCounterState;	//状態管理のカウンター
	int nType;			//種類
	//int nLife;			//敵の体力
	int nCountTime;		//カウント
	float fSpeed;		//速さ
	bool bUse;			//使用しているかどうか
}CHARACTER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCharacter(void);
void UnInitCharacter(void);
void UpdateCharacter(void);
void DrawCharacter(void);


void SetCharacter(D3DXVECTOR3 pos, int nType);
CHARACTER *GetCharacter(void);


#endif
