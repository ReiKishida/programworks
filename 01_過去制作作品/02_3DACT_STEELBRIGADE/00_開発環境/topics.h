//=============================================================================
//
// 情報処理 [topics.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _TOPICS_H_
#define _TOPICS_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TOPICS	(10)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	TOPICSTYPE_NONE = 0,		//メール(モビィディックの管轄下、云々)
	TOPICSTYPE_MAIL_0 ,		//メール(モビィディックの管轄下、云々)
	TOPICSTYPE_MAIL_0_1,		//メール(依頼来てます、云々)
	TOPICSTYPE_MAIL_1 ,		//メール
	TOPICSTYPE_MAIL_1_1,		//ニュース
	TOPICSTYPE_NEWS_0,		//ニュース
	TOPICSTYPE_NEWS_0_1,		//ニュース
	TOPICSTYPE_MISSION_0,	//ミッション01
	TOPICSTYPE_MISSION_1,//ミッション02
	TOPICSTYPE_MAX		//種類の総数
}TOPICSTYPE;

typedef enum
{
	MAILSTATE_UNREAD = 0,	//未読状態
	MAILSTATE_ALREADY,		//既読状態
	MAILSTATE_MAX			//最大数
}TOPICSSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold; //移動前位置
	D3DXVECTOR3 FallenPoint;
	D3DXVECTOR3 move;   //移動量
	TOPICSSTATE state;	//状態
	TOPICSTYPE type;
	int nType;			//種類
	float fWidth;		//幅
	float fHeight;		//高さ
	float Move_X;	//移動量のカウント(X)
	float Move_Y;	//移動量のカウント(Y)
	
	bool bUse;			//使用しているかどうか
	int nIndexPolygon;
}TOPICS;

//*****************************************************************************
// グローバル変数
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTopics(void);
void UnInitTopics(void);
void UpdateTopics(void);
void DrawTopics(void);

void DeleteTopics(int nIndexPolygon);

void SetTopics(D3DXVECTOR3 pos, TOPICSTYPE Type, int nType, float fHeight, float fWidth);

TOPICSTYPE CollisionTopics(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold);
TOPICS *GetTopics(void);


#endif
