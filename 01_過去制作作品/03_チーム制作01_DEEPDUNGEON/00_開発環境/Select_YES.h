//=============================================================================
//
// 背景処理 [Select_Yes.h]
// Author :TEAM_Fascinator
//
//=============================================================================
#ifndef _SELECT_YES_H_ //二十インクルード防止のマクロ定義
#define _SELECT_YES_

#include "main.h"

typedef enum
{
	YESSTATE_SELECT = 0,
	YESSTATE_NOSELECT,
	YESSTATE_MAX

} YESSTATE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSelectYes(void);
void UninitSelectYes(void);
void UpdateSelectYes(void);
void DrawSelectYes(void);

void SetSelectYes(YESSTATE state);

#endif
