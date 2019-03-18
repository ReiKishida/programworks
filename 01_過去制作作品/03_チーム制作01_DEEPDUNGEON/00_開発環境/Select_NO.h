//=============================================================================
//
// 背景処理 [Select.h]
// Author :TEAM_Fascinator
//
//=============================================================================
#ifndef _SELECTNO_H_ //二十インクルード防止のマクロ定義
#define _SELECTNO_H_

#include "main.h"

typedef enum
{
	NOSTATE_SELECT = 0,
	NOSTATE_NOSELECT,
	NOSTATE_MAX

} NOSTATE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSelectNo(void);
void UninitSelectNo(void);
void UpdateSelectNo(void);
void DrawSelectNo(void);

void SetSelectNo(NOSTATE state);

#endif