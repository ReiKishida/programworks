//=============================================================================
//
// ポリゴンの処理 [Castler.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _CASTLE_H_
#define _CASTLE_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	CASTLESTATE_APPEAR = 0,	//出現状態
	CASTLESTATE_NORMAL,		//通常状態
	CASTLESTATE_DAMAGE,		//ダメージ状態
	CASTLESTATE_DEATH,		//死亡状態
	CASTLEYSTATE_MAX			//種類の総数
}CASTLESTATE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	CASTLESTATE state;	//ステート
	int nCounterState;	//状態管理のカウンター
	int nCntCharge;
	int nCntReload;		//リロードカウント
	int nSpreadReload;	//スプレッドリロード
	int nValcanReload; //バルカンリロード
	int nCanonReload;	//キャノンリロード
	int nLife;			//敵の体力
	int nStock;			//残機
	bool bDisp;			//使用されているか
}CASTLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCastle(void);
void UnInitCastle(void);
void UpdateCastle(void);
void DrawCastle(void);

 


CASTLE *GetCastle();

void HitCastle(int nDamage);
#endif