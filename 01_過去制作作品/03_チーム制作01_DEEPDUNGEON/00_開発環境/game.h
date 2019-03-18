//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//=============================================================================
// ゲームの状態
//=============================================================================
typedef enum
{
	GAMESTATE_NONE = 0,		// 何もしていない状態
	GAMESTATE_NORMAL,		// 通常状態
	GAMESTATE_CLEAR,		// クリア状態
	GAMESTATE_GAMEOVER,		// ゲームオーバー状態
	GAMESTATE_CLEAR_75,
	GAMESTATE_CLEAR_50,
	GAMESTATE_CLEAR_25,
	GAMESTATE_CLEAR_TRUE,
	GAMESTATE_MAX
}GAMESTATE;

//ステージ遷移
typedef enum
{//列挙型
	STAGEMODE_STAGE1 = 0,
	STAGEMODE_STAGE2,
	STAGEMODE_STAGE3,
	STAGEMODE_MAX
}STAGEMODE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetStageMode(STAGEMODE Stagemode);
void SetGameState(GAMESTATE state);
STAGEMODE GetStageMode(void);
GAMESTATE GetGameState(void);
GAMESTATE *GetGameState2(void);
void SetPauseState(bool bPause);
void SetSelect(bool bSelect);
bool GetSelect(void);

void MagicSquareSet(void);
void Set_Stage1(void);
void Set_Stage2(void);
void Set_BossStage(void);


void Delete_Stage1(void);
void Delete_Stage2(void);
void Delete_BossStage(void);

#endif