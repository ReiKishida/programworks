//=============================================================================
//
// フェード処理 [fade.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _STAGEFADE_H_
#define _STAGEFADE_H_

#include "main.h"
#include "game.h"

//*************************************
// フェードの状態
//*************************************
typedef enum
{
	STAGEFADE_NONE = 0,		// 何もしていない状態
	STAGEFADE_IN,			// フェードイン状態
	STAGEFADE_OUT,			// フェードアウト状態
	STAGEFADE_MAX
} STAGEFADE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitStageFade(STAGEMODE modeNext);
void UninitStageFade(void);
void UpdateStageFade(void);
void DrawStageFade(void);

void SetStageFade(STAGEMODE modeNext);
STAGEFADE GetStageFade(void);

#endif
