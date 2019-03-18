//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "input.h"
#include "pause.h"
#include "pausemenu.h"
#include "block.h"
#include "callout.h"
#include "snow.h"
#include "bg.h"
#include "statusbar.h"
#include "player.h"
#include "score.h"
#include "time.h"
#include "item.h"
#include "itemstate.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BLOCK_NUM		(15)		// ブロックの数

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;	// ゲームの状態
int g_nCounterGameState;				// 状態管理のカウンター
PAUSECHECK g_bPause;
BlockInfo g_aBlockInfo[] =
{//SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
	{ D3DXVECTOR3(530.0f, 320.0f, 0.0f), 50.0f, 50.0f, 0 },
	{ D3DXVECTOR3(750.0f, 320.0f, 0.0f), 50.0f, 50.0f, 3 },
	{ D3DXVECTOR3(530.0f, 500.0f, 0.0f), 50.0f, 50.0f, 2 },
	{ D3DXVECTOR3(750.0f, 500.0f, 0.0f), 50.0f, 50.0f, 1 },

	{ D3DXVECTOR3(230.0f, 320.0f, 0.0f), 50.0f, 50.0f, 0 },
	{ D3DXVECTOR3(750.0f, 320.0f, 0.0f), 50.0f, 50.0f, 3 },
	{ D3DXVECTOR3(850.0f, 500.0f, 0.0f), 50.0f, 50.0f, 2 },
	{ D3DXVECTOR3(1050.0f, 500.0f, 0.0f), 50.0f, 50.0f, 1 },

	{ D3DXVECTOR3(530.0f, 120.0f, 0.0f), 50.0f, 50.0f, 3 },
	{ D3DXVECTOR3(750.0f, 120.0f, 0.0f), 50.0f, 50.0f, 0 },
	{ D3DXVECTOR3(530.0f, 700.0f, 0.0f), 50.0f, 50.0f, 1 },
	{ D3DXVECTOR3(750.0f, 700.0f, 0.0f), 50.0f, 50.0f, 2 },

	{ D3DXVECTOR3(230.0f, 120.0f, 0.0f), 50.0f, 50.0f, 1 },
	{ D3DXVECTOR3(750.0f, 120.0f, 0.0f), 50.0f, 50.0f, 2 },
	{ D3DXVECTOR3(850.0f, 700.0f, 0.0f), 50.0f, 50.0f, 3 },
	{ D3DXVECTOR3(1050.0f, 700.0f, 0.0f), 50.0f, 50.0f, 0 },
};

ItemInfo g_aItemInfo[] =
{//SetItem(D3DXVECTOR3 pos, int nType);
	{ D3DXVECTOR3(635.0f, 410.0f, 0.0f), 0 },
	{ D3DXVECTOR3(200.0f, 250.0f, 0.0f), 0 },
	{ D3DXVECTOR3(235.0f, 610.0f, 0.0f), 0 },
	{ D3DXVECTOR3(1035.0f, 200.0f, 0.0f), 0 },
	{ D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 0 },

};

//=============================================================================
// ゲーム初期化処理
//=============================================================================
void InitGame(void)
{
	// イエェェェ
	InitBlock();
	for (int nCntBlock = 0; nCntBlock < sizeof g_aBlockInfo / sizeof(BlockInfo); nCntBlock++)
	{
		SetBlock(g_aBlockInfo[nCntBlock].pos, g_aBlockInfo[nCntBlock].fWidth, g_aBlockInfo[nCntBlock].fHeight, g_aBlockInfo[nCntBlock].nType);
	}

	InitItem();
	for (int nCntItem = 0; nCntItem < sizeof g_aItemInfo / sizeof(ItemInfo); nCntItem++)
	{
		SetItem(g_aItemInfo[nCntItem].pos, g_aItemInfo[nCntItem].nType);
	}

	// 吹き出し
	InitCallout();

	// 雪
	InitSnow();

	// プレイヤー
	InitPlayer();

	// 背景
	InitBG();

	// ステータスバー
	InitStatusBar();
	InitItemState();
	InitTime();
	InitScore();

	// ポーズ初期化処理
	InitPause();

	// ポーズメニュー初期化処理
	InitPmenu();

	g_gameState = GAMESTATE_NORMAL;	// 通常状態に設定
	g_nCounterGameState = 0;

	g_bPause.bPause = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// 家
	UninitBlock();

	UninitItem();

	// 炊き出し
	UninitCallout();

	// 雪
	UninitSnow();

	// プレイヤー
	UninitPlayer();

	// 背景
	UninitBG();

	// ステータス
	UninitStatusBar();
	UninitItemState();
	UninitTime();
	UninitScore();

	// ポーズ終了処理
	UninitPause();

	// ポーズメニュー終了処理
	UninitPmenu();
}

//=============================================================================
// ゲーム更新処理
//=============================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true)
	{// ポーズON/OFF
		g_bPause.bPause = g_bPause.bPause ? false : true;
	}

	if (g_bPause.bPause == false)
	{
		// ハウス
		UpdateBlock();

		UpdateItem();

		// ふっきー
		UpdateCallout();

		// ゆき
		UpdateSnow();

		// プレイヤー
		UpdatePlayer();

		// 背景
		UpdateBG();

		// ステータスバー
		UpdateStatusBar();
		UpdateItemState();
		UpdateTime();
		UpdateScore();

	}
	else
	{
		// ポーズ更新処理
		UpdatePause();
		
		// ポーズメニュー更新処理
		UpdatePmenu();
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;

	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;

			// 画面(モード)の設定
			//SetFade(MODE_GAMEOVER);
		}
		break;

	case GAMESTATE_CLEAR:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;

			// 画面(モード)の設定
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void DrawGame(void)
{
	//背景
	DrawBG();

	// おうち
	DrawBlock();

	DrawItem();

	// ふきだし
	DrawCallout();

	// 由紀
	DrawSnow();

	// プレイヤー
	DrawPlayer();

	// ステータスバー
	DrawStatusBar();
	DrawItemState();
	DrawTime();
	DrawScore();

	if (g_bPause.bPause == true)
	{
		// ポーズ描画処理
		DrawPause();

		// ポーズメニュー描画処理
		DrawPmenu();
	}
}

//=============================================================================
// ゲームの状態の設定
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//=============================================================================
// ゲームの状態の取得
//=============================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=============================================================================
// ポーズ状態の取得
//=============================================================================
PAUSECHECK *GetPauseMenu(void)
{
	return &g_bPause;
}
