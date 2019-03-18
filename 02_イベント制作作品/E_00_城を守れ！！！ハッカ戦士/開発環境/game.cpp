//=============================================================================
//
// ゲーム画面 [game.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "pause.h"
#include "pausemenu.h"
#include "sound.h"
#include "bg.h"
#include "player.h"
#include "castle.h"
#include "chargebar.h"
#include "enemy.h"
#include "bullet.h"
#include "enemy.h"
#include "stamina.h"
#include "castlelife.h"
#include "life.h"
#include "explosion.h"
#include "timer.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル関数
//*****************************************************************************
GAMESTATE g_GameState;	//ゲームの状態
GAMESTATE g_GameState2;
int g_nCounterGameState;
PAUSECHECK g_bPause;		//ポーズ中か


//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
void InitGame(void)
{


	//背景初期化
	InitBG();

	//プレイヤー初期化
	InitPlayer();

	//城の初期化
	InitCastle();
	
	

	//弾初期化
	InitBullet();




	//敵の初期化
	InitEnemy();

	InitLife();

	InitExplosion();

	InitTimer();


	SetEnemy(D3DXVECTOR3(1000, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(1200, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(1400, 540, 0.0f), 2, 10);
	SetEnemy(D3DXVECTOR3(1600, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(1800, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(2000, 540, 0.0f), 2, 10);

	SetEnemy(D3DXVECTOR3(2200, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(2400, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(2600, 540, 0.0f), 2, 10);
	SetEnemy(D3DXVECTOR3(2800, 540, 0.0f), 0, 10);
	SetEnemy(D3DXVECTOR3(3000, 540, 0.0f), 1, 10);
	SetEnemy(D3DXVECTOR3(3200, 540, 0.0f), 2, 10);

	//SetEnemy(D3DXVECTOR3(800, 540, 0.0f), 1, 3);

	//SetEnemy(D3DXVECTOR3(600, 540, 0.0f), 2, 3);

	g_bPause.bPause = false;
	g_GameState = GAMESTATE_NORMAL;	//通常状態に設定
	g_nCounterGameState = 0;

}

//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UnInitGame(void)
{

	//背景の終了
	UnInitBG();

	//プレイヤー終了
	UnInitPlayer();

	//弾終了
	UnInitBullet();

	//敵の終了
	UnInitEnemy();

	//城の終了
	UnInitCastle();

	UninitLife();

	UnInitExplosion();

	UninitTimer();

}


//=============================================================================
// ゲーム画面の更新処理
//=============================================================================
void UpdateGame(void)
{

	if (GetKeyboardTrigger(DIK_P) == true)
	{//ポーズON/OFF
		g_bPause.bPause = g_bPause.bPause ? false : true;
	}

	if (g_bPause.bPause == false)
	{
		//背景の更新
		UpdateBG();

		//プレイヤー終了
		UpdatePlayer();

		//城の更新
		UpdateCastle();

		
		
		//弾更新
		UpdateBullet();


		//敵の更新
		UpdateEnemy();

		UpdateLife();

		UpdateExplosion();


		
		UpdateTimer();

		switch (g_GameState)
		{
		case GAMESTATE_NORMAL:
			break;

		case GAMESTATE_CLEAR:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState2 = GAMESTATE_CLEAR;
				g_GameState = GAMESTATE_NONE;
				


				SetFade(MODE_RESULT);



			}
			break;

		case GAMESTATE_FAILED:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState2 = GAMESTATE_FAILED;
				g_GameState = GAMESTATE_NONE;





				SetFade(MODE_RESULT);

			}
			break;
		}


		

	}

	if (g_bPause.bPause == false)
	{
		

	}

}


//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawGame(void)
{
	//背景の描画
	DrawBG();

	//プレイヤーの描画
	DrawPlayer();

	//城の描画
	DrawCastle();

	
	
	DrawTimer();



	

	//弾更新
	DrawBullet();


	//敵の描画
	DrawEnemy();

	DrawLife();

	DrawExplosion();
}


//=============================================================================
// モード設定処理
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 0;
}

//=============================================================================
// ゲーム状態の取得
//=============================================================================
GAMESTATE *GetGameState(void)
{
	return &g_GameState;
}

GAMESTATE *GetGameState2(void)
{
	return &g_GameState2;
}


//=============================================================================
// ゲーム状態の取得
//=============================================================================
PAUSECHECK *GetPauseCheck(void)
{
	return &g_bPause;
}
