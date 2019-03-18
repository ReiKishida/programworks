//=============================================================================
//
// ブロック処理 [block.h]
// Author : komatsu
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	float fWidth;
	float fHeight;
	int nType;
}BlockInfo;

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	int nCounterAnim;		//アニメーションカウンタ
	int nPatternAnim;		//アニメーションパターン
	float fWidth;			//幅
	float fHeight;			//高さ
	int nType;				//テクスチャのタイプ
	bool bChange;
	bool bUse;				//使用
}Block;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
bool CollisionBlock(D3DXVECTOR3  *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fWidth);

Block *GetBlock(void);
#endif
