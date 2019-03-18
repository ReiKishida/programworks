//=============================================================================
//
// 前景２処理 [fg2.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _FG2_H_
#define _FG2_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_FG2	(1024)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	FG2TYPE_GLASS = 0,		//草
	FG2TYPE_MAX		//種類の総数
}FG2TYPE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold; //移動前位置
	D3DXVECTOR3 move;   //移動量
	D3DXCOLOR col;		//カラー
	int nType;			//種類
	FG2TYPE FgType;	//種類
	float fWidth;		//幅
	float fHeight;		//高さ
	bool bUse;			//使用しているかどうか
}FG2;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitFg2(void);
void UnInitFg2(void);
void UpdateFg2(void);
void DrawFg2(void);


 //InitFg2();
 //UnInitFg2();
 //UpdateFg2();
 //DrawFg2();


void SetFg2(D3DXVECTOR3 pos, int type, float fHeight, float fWidth);

//void DeleteItem(int nCntItem);

//ITEM *GetItem(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
