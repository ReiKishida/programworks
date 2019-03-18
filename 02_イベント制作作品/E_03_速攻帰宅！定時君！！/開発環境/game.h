#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ゲーム処理 [game.h]
// Author :Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_

#define GAME_TIME	(180)	// タイム

#include "main.h"
// ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,	// 何もしていない状態
	GAMESTATE_START ,	// 始まりの状態
	GAMESTATE_GAME,	// 通常状態
	GAMESTATE_END,	// 終わり
	GAMESTATE_MAX,
}GAMESTATE;

typedef enum
{
	GAMESTAGE1 = 0,
	GAMESTAGEMAX
}GAMESTAGE;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
int GetGameStateCounter(void);
void ReleaseGamePause(void);

void SetPlayerFileData(int nIdy, char *cFilename);						// プレイヤーファイルデータ
void SetObjectFileData(void);						// オブジェクトファイルデータ
void SetStagePos(char *cFilename);					// ステージの位置データ
bool GetPause(void);
bool GetPauseView(void);

void PrintStagePos(void);
#endif
