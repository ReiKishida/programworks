//=============================================================================
//
// ゲーム画面 [game.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,	//何もしていない状態
	GAMESTATE_NORMAL,	//通常状態
	GAMESTATE_CLEAR,		//終了状態
	GAMESTATE_FAILED,		//終了状態
	GAMESTATE_MAX		//最大数
}GAMESTATE;

typedef struct
{
	bool bPause;
}PAUSECHECK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UnInitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE *GetGameState(void);
GAMESTATE *GetGameState2(void);

PAUSECHECK *GetPauseCheck(void);
#endif

