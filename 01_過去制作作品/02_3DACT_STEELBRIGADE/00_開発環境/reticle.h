//=============================================================================
//
// ブロック処理 [Reticle.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _RETICLE_H_
#define _RETICLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_RETICLE	(1)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;   //移動量
	D3DXVECTOR3 rot;
	D3DXCOLOR col;		//カラー
	float fWidth;		//幅
	float fHeight;		//高さ
	float fLength;		//対角線の角度
	float fAngle;		//対角線の長さ
	float fAngle2;		//対角線の長さ
	bool bUse;			//使用しているかどうか
}RETICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitReticle(void);
void UnInitReticle(void);
void UpdateReticle(void);
void DrawReticle(void);
RETICLE GetReticle(void);





#endif
