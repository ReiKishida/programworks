//=============================================================================
//
// ブロック処理 [item.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _POPSCORE_H_
#define _POPSCORE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_POPSCORE	(255)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	POPSCORETYPE_COIN_SCORE = 0,	//コインスコア表示
	POPSCORETYPE_VODKA_SCORE,	//ウォッカスコア表示
	POPSCORETYPE_SATSU_SCORE,	//お札スコア表示
	POPSCORETYPE_AREY_SCORE,	//アレイスコア表示
	POPSCORETYPE_MAX		//種類の総数
}POPSCORETYPE;

typedef enum
{
	POPSCORESTATE_NOMAL = 0,	//通常状態
	POPSCORESTATE_LAND,			//着地状態
	POPSCORESTATE_LOST,			//消失状態
	POPSCORESTATE_MAX			//最大数
}POPSCORESTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold; //移動前位置
	D3DXVECTOR3 move;   //移動量
	D3DXVECTOR3 rot;
	D3DXCOLOR col;		//カラー
	POPSCORETYPE PopScoreType;	//種類
	POPSCORESTATE state;	//状態
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
}POPSCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPopScore(void);
void UnInitPopScore(void);
void UpdatePopScore(void);
void DrawPopScore(void);



void SetPopScore(D3DXVECTOR3 pos, int type, D3DXVECTOR3 move);


POPSCORE *GetPopScore(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
