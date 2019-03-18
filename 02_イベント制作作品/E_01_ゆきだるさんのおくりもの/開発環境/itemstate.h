//=============================================================================
//
// アイテムステータス処理 [itemstate.h]
// Author : komatsu
//
//=============================================================================
#ifndef _ITEMSTATE_H_
#define _ITEMSTATE_H_

#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	int nCounterAnim;		//アニメーションカウンター
	int nPatternAnim;		//アニメーションパターン
	int nType;				//アイテムの種類
	float fHeight;			//アイテムの縦幅
	float fWidth;			//アイテムの横幅
	bool bUse;				//使用しているかどうか
}ItemState;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItemState(void);
void UninitItemState(void);
void UpdateItemState(void);
void DrawItemState(void);
void SetItemState(int nType);
ItemState *GetItemState(void);
#endif
