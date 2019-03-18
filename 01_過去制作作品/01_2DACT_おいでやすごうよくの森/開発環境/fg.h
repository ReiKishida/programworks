//=============================================================================
//
// ブロック処理 [item.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _FG_H_
#define _FG_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_FG	(1024)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	FGTYPE_GLASS = 0,		//草
	FGYPE_MAX		//種類の総数
}FGTYPE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold; //移動前位置
	D3DXVECTOR3 move;   //移動量
	D3DXCOLOR col;		//カラー
	int nType;			//種類
	FGTYPE FgType;	//種類
	float fWidth;		//幅
	float fHeight;		//高さ
	
	bool bUse;			//使用しているかどうか
}FG;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitFg(void);
void UnInitFg(void);
void UpdateFg(void);
void DrawFg(void);



void SetFg(D3DXVECTOR3 pos, int type, float fHeight, float fWidth);

//void DeleteItem(int nCntItem);

//ITEM *GetItem(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
