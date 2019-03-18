//=============================================================================
//
// 情報画面処理 [tips.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _Tips_H_
#define _Tips_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	TIPSSTATE_NONE = 0,	
	TIPSSTATE_USE,
	TIPSSTATE_MAX
}TIPS_STATE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//カラ－
	int nType;			//種類
	TIPS_STATE state;
}TIPS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTips(void);
void UnInitTips(void);
void UpdateTips(void);
void DrawTips(void);


#endif


