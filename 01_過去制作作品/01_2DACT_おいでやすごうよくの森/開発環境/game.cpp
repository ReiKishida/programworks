//=============================================================================
//
// ゲーム画面 [game.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "bg.h"
#include "player.h"
#include "score.h"
#include "effect.h"
#include "timer.h"
#include "input.h"
#include "pause.h"
#include "pausemenu.h"
#include "fade.h"
#include "block.h"
#include "item.h"
#include "shoot.h"
#include "character.h"
#include "itemcounter.h"
#include "vodkacounter.h"
#include "satsucounter.h"
#include "areycounter.h"
#include "ranking.h"
#include "highscore.h"
#include "gamehighscore.h"
#include "resultitem.h"
#include "resultarey.h"
#include "resultitemall.h"
#include "menuranking.h"
#include "shadow.h"
#include "fg.h"
#include "fg2.h"
#include "sound.h"
#include "popscore.h"
#include "fever.h"

//*****************************************************************************
// グローバル関数
//*****************************************************************************
GAMESTATE g_GameState;	//ゲームの状態
//GAMESTATE g_GameState2;
int g_nCounterGameState;
PAUSECHECK g_bPause;		//ポーズ中か
PAUSECHECK2 g_GameScroll;   //スクロールポーズ
//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
void InitGame(void)
{

	//背景初期化
	InitBG();

	
	//ﾌブロックの初期化
	InitBlock();

	//影初期化
	//InitShadow();

	//カウンター初期化//
	InitItemCounter();
	InitVodkaCounter();
	InitSatsuCounter();
	InitAreyCounter();
	

	//アイテム初期化
	InitItem();

	//点数
	InitPopScore();

	//キャラクター初期化
	InitCharacter();

	//エフェクトの初期化
	InitEffect();


	//発射位置の初期化
	InitShoot();


	//タイマーの初期化
	InitTimer();
	
	////ゲーム内ハイスコア
	InitGameHighScore();


	//ポーズの初期化
	InitPause();
		
	//メニューの初期化
	InitPauseMenu();

	//前景の初期化
	InitFg();
	InitFg2();
	
	//フィーバー初期化
	InitFever();

	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 80, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);

	//草
	SetFg(D3DXVECTOR3(100, 640, 0.0f), 2, 50, 25);
	SetFg(D3DXVECTOR3(640, 630, 0.0f), 2, 60, 30);
	SetFg(D3DXVECTOR3(450, 630, 0.0f), 2, 50, 30);
	SetFg(D3DXVECTOR3(600, 650, 0.0f), 2, 40, 20);



	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2, 590, 0.0f), 0, 50, 480);
	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);

	//SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2, 660, 0.0f), 2, SCREEN_HEIGHT -100, SCREEN_WIDTH / 2 + 20);


	


	//ブロック設置(障害物系)
	SetBlockType000();

	//ブロック設置(地面)
	SetBlockType001();

	//ブロック設置(移動床)
	SetBlockType002();

	//ブロック設置(バネ床)
	SetBlockType003();

	SetBlockType004();

	//スコアの初期化
	InitScore();

	//アイテム設置
	SetItem();

	//プレイヤーの初期化
	InitPlayer();

	
	//発射位置の設置
	SetShoot(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 - 220 , 0.0f));


	
		//キャラクター配置
		//SetCharacter(D3DXVECTOR3(1300, 550, 0.0f), 0);
	

	g_bPause.bPause = false;
	g_GameScroll.bPause = false;
	g_GameState = GAMESTATE_NORMAL;	//通常状態に設定
	g_nCounterGameState = 0;

	//PlaySound(SOUND_LABEL_SE_TRUMPET);
}

//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UnInitGame(void)
{
	//背景終了
	UnInitBG();


	//アイテム初期化
	UnInitItem();

	//発射位置の終了
	UnInitShoot();

	//ブロックの終了
	UnInitBlock();

	//影初期化
	//UnInitShadow();


	//キャラクター終了
	UnInitCharacter();

	


	//エフェクトの終了
	UninitEffect();
	
	



	//タイマーの終了
	UninitTimer();
	

	//ポーズの終了
	UnInitPause();

	
	//メニューの終了
	UnInitPauseMenu();

	
	//スコアの終了
	UninitScore();

	//カウンター終了
	UninitItemCounter();

	//カウンター//
	UninitVodkaCounter();

	UninitSatsuCounter();

	UninitAreyCounter();

	//ゲーム内ハイスコア
	UnInitGameHighScore();

	//プレイヤーの終了
	UnInitPlayer();

	//前景の終了
	UnInitFg();
	UnInitFg2();
	
	//点数
	UnInitPopScore();

	//フィーバー初期化
	UnInitFever();
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

	//if (GetKeyboardTrigger(DIK_O) == true)
	//{//ポーズON/OFF
	//	g_GameScroll.bPause = g_GameScroll.bPause ? false : true;
	//}


	if (g_bPause.bPause == false || g_GameScroll.bPause == true )
	{
		//ブロックの更新
		UpdateBlock();

		
	}
		

		if (g_bPause.bPause == false && g_GameScroll.bPause == false)
		{

			//ブロックの更新
			//UpdateBlock();

			//背景更新
			UpdateBG();


			//影更新
			//UpdateShadow();

			//プレイヤーの更新
			UpdatePlayer();

			//カウンター更新
			UpdateItemCounter();

			//ウォッカ更新
			UpdateVodkaCounter();

			//札更新
			UpdateSatsuCounter();

			UpdateAreyCounter();


			//アイテム更新
			UpdateItem();

			//発射位置の更新
			UpdateShoot();

			//キャラクター更新
			UpdateCharacter();

			////ゲーム内ハイスコア
			UpdateGameHighScore();

			//前景の更新
			UpdateFg();
			UpdateFg2();
			


			//タイマーの更新
			UpdateTimer();


			//スコアの更新
			UpdateScore();

			//エフェクトの更新
			UpdateEffect();

			//点数
			UpdatePopScore();

			//フィーバー初期化
			UpdateFever();

			switch (g_GameState)
			{
			case GAMESTATE_NORMAL:
				break;

			case GAMESTATE_END:
				g_nCounterGameState++;
				if (g_nCounterGameState >= 10)
				{
					g_GameState = GAMESTATE_NONE;

					////////////////////
					//ハイスコアに代入//
					////////////////////
					SetGameHighScore(GetScore());
					SetRanking(GetScore());
					SetMenuRanking(GetScore());
					SetHighScore(GetScore());


					//////////////////
					//アイテムの清算//
					//////////////////
					SetResultCoin(GetItemCounter());
					SetResultArey(GetAreyCounter());
					SetResultVodka(GetVodkaCounter());
					SetResultSatsu(GetSatsuCounter());

					SetResultItemAll(GetItemCounter(), GetVodkaCounter(), GetSatsuCounter(), GetAreyCounter());


					//モード設定
					//SetMode(MODE_RESULT);
					SetFade(MODE_RESULT);

				}
				
				break;
			}

			
		}


		if (g_bPause.bPause == true)
		{
			UpdatePause();
			UpdatePauseMenu();


		}

	}


//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawGame(void)
{
		//背景描画
		DrawBG();

		//フィーバー初期化
		DrawFever();

		//ブロックの描画
		DrawBlock();


		//キャラクター描画
		DrawCharacter();


		//発射位置の描画
		DrawShoot();


		//影更新
		//DrawShadow();
	

		DrawFg2();

		//エフェクトの描画
		DrawEffect();

		//プレイヤーの描画
		DrawPlayer();
		

		//アイテムの描画
		DrawItem();

		//点数
		DrawPopScore();

		//前景の描画
		DrawFg();

		//タイマーの描画
		DrawTimer();

	
		//スコアの描画
		DrawScore();

		

		//ゲーム内ハイスコア
		DrawGameHighScore();

		//カウンター描画
		DrawItemCounter();

		//ウォッカカウンター
		DrawVodkaCounter();

		//札カウンター
		DrawSatsuCounter();

		//鉄アレイカウンター
		DrawAreyCounter();

	


		if (g_bPause.bPause == true)
		{
			DrawPause();
			DrawPauseMenu();
		}
}


//=============================================================================
// Type000設置
//=============================================================================

void SetBlockType000()
{

	SetBlock(D3DXVECTOR3(280, 410, 0), 120, 80, 0, BLOCKSTATE_NORMAL, 0.0f, 0.0f);
	
	SetBlock(D3DXVECTOR3(90, 350, 0), 120.0f, 60.0f, 2, BLOCKSTATE_MOVE_Y, 0.0f, -2.0f);



	SetBlock(D3DXVECTOR3(910, 410, 0), 120, 80, 0, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

	SetBlock(D3DXVECTOR3(1090, 350, 0), 120.0f, 60.0f, 3, BLOCKSTATE_MOVE_Y, 0.0f, 2.0f);


	//SetBlock(D3DXVECTOR3(940, 200, 0), 100, 50, 2, BLOCKSTATE_MOVE_X, -1.5f, 0.0f);

	
}

//=============================================================================
// Type001設置
//=============================================================================
void SetBlockType001()
{
	
	SetBlock(D3DXVECTOR3(0, 580, 0), SCREEN_WIDTH, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

	

	
		//SetBlock(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400, -250, 0), 300.0f, 50.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);
		//SetBlock(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, -250, 0), 300.0f, 50.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

}

//=============================================================================
// Type002設置
//=============================================================================
void SetBlockType002()
{
	// X軸移動床
	//SetBlock(D3DXVECTOR3(790, 480, 0), 100.0f, 40.0f, 2, BLOCKSTATE_MOVE_X, 1.4f, 0.0f);


	// Y軸移動床
	//SetBlock(D3DXVECTOR3(1100, 420, 0), 100.0f, 40.0f, 2, BLOCKSTATE_MOVE_Y, 0.0f, 0.0f);


	//SetBlock(D3DXVECTOR3(150, 600, 0), 150.0f, 40.0f, 2, BLOCKSTATE_MOVE_XY, 1.0f, -1.0f);


}

//=============================================================================
// Type003設置
//=============================================================================

void SetBlockType003()
{

	//SetBlock(D3DXVECTOR3(20, 630, 0), 50.0f, 50.0f, 3, BLOCKSTATE_SPRING, 0.0f, 0.0f);
	//SetBlock(D3DXVECTOR3(1235, 630, 0), 50.0f, 50.0f, 3, BLOCKSTATE_SPRING, 0.0f, 0.0f);

}


//=============================================================================
// Type004設置
//=============================================================================

void SetBlockType004()
{

	//SetBlock(D3DXVECTOR3(400, 450, 0), 60.0f, 60.0f, 0, BLOCKSTATE_CRASH, 0.0f, 0.0f);

}

//=============================================================================
// アイテムの設置
//=============================================================================
void SetItem()
{

	SetItem(D3DXVECTOR3(30, 40, 0.0f), ITEMTYPE_H_COIN, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(220, 40, 0.0f), ITEMTYPE_H_VODKA, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(400, 40, 0.0f), ITEMTYPE_H_PAPER, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(600, 40, 0.0f), ITEMTYPE_H_AREY, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

}


//=============================================================================
// モード設定処理
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 10;
}

//=============================================================================
// ゲーム状態の取得
//=============================================================================
GAMESTATE *GetGameState(void)
{
	return &g_GameState;
}

//GAMESTATE *GetGameState2(void)
//{
//	return &g_GameState2;
//}


//=============================================================================
// ゲーム状態の取得
//=============================================================================
PAUSECHECK *GetPauseCheck(void)
{
	return &g_bPause;
}



//=============================================================================
// ゲーム状態2の取得
//=============================================================================
PAUSECHECK2 GetPauseCheck2(void)
{
	return g_GameScroll;
}

