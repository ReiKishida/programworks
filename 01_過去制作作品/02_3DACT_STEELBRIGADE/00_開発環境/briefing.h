//=============================================================================
//
// 情報画面処理 [Briefing.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _BRIEFING_H_
#define _BRIEFING_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BRIEFINGSTATE_NONE = 0,
	BRIEFINGSTATE_USE,
	BRIEFINGSTATE_MAX
}BRIEFING_STATE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//カラ－
	int nType;			//種類
	BRIEFING_STATE state;
}BRIEFING;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBriefing(void);
void UnInitBriefing(void);
void UpdateBriefing(void);
void DrawBriefing(void);


#endif


