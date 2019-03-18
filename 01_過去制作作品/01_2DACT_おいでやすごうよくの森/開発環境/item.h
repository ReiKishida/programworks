//=============================================================================
//
// ブロック処理 [item.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ITEM	(500)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	ITEMTYPE_COIN = 0,		//コイン
	ITEMTYPE_VODKA,		//ウォッカ
	ITEMTYPE_AREY,		//鉄アレイ
	ITEMTYPE_PAPER,		//時計
	ITEMTYPE_H_COIN,		//HUD用アイテム画像
	ITEMTYPE_H_VODKA,		//HUD用アイテム画像
	ITEMTYPE_H_AREY,		//HUD用アイテム画像
	ITEMTYPE_H_PAPER,		//HUD用アイテム画像
	ITEMTYPE_COIN_SCORE,	//コインスコア表示
	ITEMTYPE_VODKA_SCORE,	//ウォッカスコア表示
	ITEMTYPE_SATSU_SCORE,	//お札スコア表示
	ITEMTYPE_AREY_SCORE,	//アレイスコア表示
	ITEMTYPE_MAX		//種類の総数
}ITEMTYPE;

typedef enum
{
	ITEMSTATE_NOMAL = 0,	//通常状態
	ITEMSTATE_LAND,			//着地状態
	ITEMSTATE_LOST,			//消失状態
	ITEMSTATE_MAX			//最大数
}ITEMSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold; //移動前位置
	D3DXVECTOR3 move;   //移動量
	D3DXVECTOR3 rot;
	D3DXCOLOR col;		//カラー
	ITEMTYPE ItemType;	//種類
	ITEMSTATE state;	//状態
	float fWidth;		//幅
	float fHeight;		//高さ
	float fLength;		//対角線の角度
	float fAngle;		//対角線の長さ
	float fAngle2;		//対角線の長さ
	int nType;			//種類
	int nCntAnim;		//アニメーションのカウント
	int nPatternAnim;	//アニメーションパターン
	int nCntLost;		//消えるまでの時間
	int nCntGet;		//消えるまでの時間2
	int nland;
	bool bUse;			//使用しているかどうか
	bool bItemLand;		//アイテム着地
}ITEM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitItem(void);
void UnInitItem(void);
void UpdateItem(void);
void DrawItem(void);



void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, D3DXVECTOR3 move);


ITEM *GetItem(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
