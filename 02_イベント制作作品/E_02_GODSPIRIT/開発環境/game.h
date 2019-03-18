#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ゲーム処理 [game.h]
// Author :Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
// ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,	// 何もしていない状態
	GAMESTATE_GAME1,	// 通常状態
	GAMESTATE_BOSS_START,// ボス
	GAMESTATE_BOSS,		// 通常状態
	GAMESTATE_END,		// 終了状態
	GAMESTATE_END2,		// 終了状態
	GAMESTATE_MAX,
}GAMESTATE;

typedef enum
{
	GAMESTAGE1 = 0,
	GAMESTAGEBOSS,
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
void ReleaseGamePause(void);

void SetPlayerFileData(void);						// プレイヤーファイルデータ
void SetObjectFileData(void);						// オブジェクトファイルデータ
void SetBillboardFileData(char *cFilename);			// ビルボードの位置データ
void SetStagePos(char *cFilename);					// ステージの位置データ
bool GetPause(void);
bool GetPauseView(void);

void PrintStagePos(void);
#endif
