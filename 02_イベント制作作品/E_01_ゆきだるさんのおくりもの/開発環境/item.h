//=============================================================================
//
// アイテム処理 [item.h]
// Author : komatsu
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_ITEM			(128)							//アイテムの最大数

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
}ItemInfo;

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	int nCounterAnim;		//アニメーションカウンター
	int nPatternAnim;		//アニメーションパターン
	int nType;				//アイテムの種類
	float fHeight;			//アイテムの縦幅
	float fWidth;			//アイテムの横幅
	bool bUse;				//使用しているかどうか
}Item;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int nType);
void CollisionItem(D3DXVECTOR3  *pPos, D3DXVECTOR3 *pPosOld, float fHeight, float fWidth);

#endif
