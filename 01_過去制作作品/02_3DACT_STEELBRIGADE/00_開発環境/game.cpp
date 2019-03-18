//=============================================================================
//
// ゲーム画面 [game.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "game.h"

#include "fade.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "meshfield.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "enemy.h"
#include "score.h"
#include "ranking.h"
#include "life.h"
#include "pausemenu.h"
#include "pause.h"
#include "scoreitem.h"
#include "boost.h"
#include "motion.h"
#include "inputx.h"
#include "reticle.h"
#include "damage.h"
#include "danger.h"
#include "hit.h"
#include "model.h"
#include "result.h"
#include "modelwall.h"
#include "gate.h"
#include "pedestal.h"
#include "canon.h"
//*****************************************************************************
// グローバル関数
//*****************************************************************************
GAMESTATE g_GameState;	//ゲームの状態
GAMESTATE g_GameState2;

int g_nCounterGameState;
PAUSECHECK g_bPause;		//ポーズ中か
PAUSECHECK2 g_GameScroll;   //スクロールポーズ
JoyState g_JoyStateGame;

//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
void InitGame(void)
{
	//ゲームパッドの状態
	g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA pCamera;
	pCamera = *GetCamera();

	WALL *pWall;
	pWall = GetModelWall();

	GATE *pGate;
	pGate = GetGate();

	//ゲームで使うもの初期化
	SetInitGame();

	//スコア初期化
	InitScore();

	//ライフ初期化
	 InitLife();

	//ブーストゲージ初期化
	 InitBoost();



	 //ポーズ関連初期化
	 InitPauseMenu();
	 InitPause();

	 //スコア加算初期化
	 InitScoreItem();

	 //モーションの初期化
	InitMotion();

	 //モデル(壁)
	 InitModelWall();

	 //門初期化
	 InitGate();

	 //台座初期化
	 InitPedestal();

	 //砲台初期化
	 InitCanon();

	 //レティクル初期化
	 InitReticle();

	 //ダメージコール初期化
	 InitDamage();

	 //警告初期化
	 InitDanger();

	 //ヒットコール初期化
	 InitItem();

	 //モデル初期化
	 InitModel();

	 //設置系処理
	 SetGameModelWall();

	 SetAreaZero();
	 SetAreaOne();
	 SetAreaTwo();
	 SetAreaThree();



	g_bPause.bPause = false;
	g_GameScroll.bPause = false;
	g_GameState = GAMESTATE_NORMAL;	//通常状態に設定
	g_nCounterGameState = 0;


}

//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UnInitGame(void)
{
	//ゲームで使うもの終了
	SetUninitGame();

	//スコア終了
	UninitScore();

	//ポーズメニュー終了
	UnInitPauseMenu();

	//ポーズ終了
	UnInitPause();

	//ライフ終了
	UninitLife();

	//スコア加算アイテム終了
	UninitScoreItem();
	//ブーストゲージ終了
	UninitBoost();

	//モーションの終了
	UninitMotion();

	//敵モーション終了
	//UninitEnemyMotion();

	//壁モデル終了
	UninitModelWall();

	//門終了
	UninitGate();

	//台座終了
	UninitPedestal();

	//砲台終了
	UninitCanon();

	//レティクル終了
	UnInitReticle();

	//ダメージコール終了
	UninitDamage();

	//警告終了
	UninitDanger();

	//ヒットコール終了
	UninitItem();

	//モデル終了
	UninitModel();

}


//=============================================================================
// ゲーム画面の更新処理
//=============================================================================
void UpdateGame(void)
{

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();


	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (GetKeyboardTrigger(DIK_P) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
	{//ポーズON/OFF
		if (g_JoyStateGame.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			g_bPause.bPause = g_bPause.bPause ? false : true;
		}
		g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_PUSH;

	}




		if (g_bPause.bPause == false && g_GameScroll.bPause == false)
		{

			//ゲームで使うもの更新処理
			SetUpdateGame();

			//スコア更新処理
			UpdateScore();
			UpdateLife();	//ライフ更新
			//UpdateItem();
			UpdateScoreItem();

			//ブーストゲージ更新
			UpdateBoost();

			//モーションの更新
			UpdateMotion();
			//UpdateEnemyMotion();

			//壁モデル更新
			UpdateModelWall();

			//門更新
			UpdateGate();

			//台座更新
			UpdatePedestal();

			//砲台更新
			UpdateCanon();

			//レティクル更新
			UpdateReticle();

			//ダメージコール更新
			UpdateDamage();

			//警告更新
			UpdateDanger();

			//ヒットコール更新
			UpdateItem();

			//モデル更新
			UpdateModel();

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
					SetRanking(GetScore());
					SetResult(GetScore());
					//モード設定
					SetFade(MODE_RESULT);
					//SetFade(MODE_RANKING);
				}
				break;

			case GAMESTATE_FAILED:
				g_nCounterGameState++;
				if (g_nCounterGameState >= 60)
				{
					g_GameState2 = GAMESTATE_FAILED;
					g_GameState = GAMESTATE_NONE;
					SetRanking(GetScore());
					SetResult(GetScore());
					//モード設定
					SetFade(MODE_RESULT);
					//SetFade(MODE_RANKING);
				}
				break;
			}


		}


		if (g_bPause.bPause == true)
		{
			UpdatePause();
			UpdatePauseMenu();


		}

		if (g_JoyStateGame.nJoypadState == INPUT_JOYSTATE_PUSH)
		{
			g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_NONE;
		}
		else if (g_JoyStateGame.nJoypadState == INPUT_JOYSTATE_NONE)
		{
			g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
		}


	}


//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawGame(void)
{
	SetDrawGame();

	DrawScoreItem();
	//ブーストゲージ描画
	DrawBoost();

	//ライフ描画
	DrawLife();

	//壁モデル描画
	DrawModelWall();
	//門描画
	DrawGate();

	//台座描画
	DrawPedestal();

	//砲台描画
	DrawCanon();
	//エフェクト描画処理
	DrawEffect();
	//パーティクル描画処理
	DrawParticle();
	//煙の更新処理
	DrawSmoke();

	//ダメージコール描画処理
	DrawDamage();

	//警告描画処理
	DrawDanger();

	//ヒット描画
	DrawItem();

	//モデル描画
	DrawModel();


	DrawReticle();
	DrawScore();

		if (g_bPause.bPause == true)
		{
			DrawPause();
			DrawPauseMenu();
		}
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



//=============================================================================
// ゲーム状態2の取得
//=============================================================================
PAUSECHECK2 GetPauseCheck2(void)
{
	return g_GameScroll;
}

//=============================================================================
// ゲーム初期化設定
//=============================================================================
void SetInitGame(void)
{
	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	//メッシュフィールドの初期化処理
	InitMeshField();

	//影の初期化
	InitShadow();


	//弾の初期化
	InitBullet();

	//エフェクト初期化処理
	InitEffect();

	//パーティクル初期化処理
	InitParticle();

	//プレイヤーの初期化処理
	InitPlayer();

	//煙の初期化処理
	InitSmoke();

	InitEnemy();

}


//=============================================================================
// ゲーム終了設定
//=============================================================================
void SetUninitGame(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//影の終了処理
	UninitShadow();


	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//プレイヤーの終了処理
	UninitPlayer();

	//煙の終了処理
	UninitSmoke();

	UninitEnemy();


}

//=============================================================================
// ゲーム更新設定
//=============================================================================
void SetUpdateGame(void)
{
	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	//メッシュフィールドの更新処理
	UpdateMeshField();

	//弾の更新処理
	UpdateBullet();

	//影の更新処理
	UpdateShadow();

	//エフェクト更新処理
	UpdateEffect();

	//パーティクル更新処理
	UpdateParticle();

	//プレイヤーの更新処理
	UpdatePlayer();

	//煙の更新処理
	UpdateSmoke();

	UpdateEnemy();
}

//=============================================================================
// ゲーム描画設定
//=============================================================================
void SetDrawGame(void)
{
	// カメラの設定
	SetCamera();
	//メッシュフィールドの更新処理
	DrawMeshField();

	//影の描画処理
	DrawShadow();


	//プレイヤーの描画処理
	DrawPlayer();

	DrawEnemy();

	//弾の描画
	DrawBullet();
}


//=============================================================================
// モデルウォール設置
//=============================================================================
void SetGameModelWall(void)
{
	//左右端
	SetModelWall(D3DXVECTOR3(-100.0f, 0.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),0);
	SetModelWall(D3DXVECTOR3(1500.0f, 0.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),0);

	//上下端
	SetModelWall(D3DXVECTOR3(680.0f, -10.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetModelWall(D3DXVECTOR3(680.0f, 0.0f, -3900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

	//裏
	SetModelWall(D3DXVECTOR3(560.0f, 0.0f, -3780.0f), D3DXVECTOR3(0.0f, 0.0f , 0.0f), 4);



	//プレイヤーの後ろ
	SetModelWall(D3DXVECTOR3(250.0f, 0.0f, -3260.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);
	SetModelWall(D3DXVECTOR3(720.0f, 140.0f, -3260.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3);
	SetGate(D3DXVECTOR3(720.0f, 0.0f, -3230.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	SetModelWall(D3DXVECTOR3(1200.0f, 0.0f, -3260.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);


	//第1ゲート
	SetModelWall(D3DXVECTOR3(250.0f, 0.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);
	SetModelWall(D3DXVECTOR3(720.0f, 140.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3);
	SetGate(D3DXVECTOR3(720.0f, 0.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),5);
	SetModelWall(D3DXVECTOR3(1200.0f, 0.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);


	//第2ゲート
	SetModelWall(D3DXVECTOR3(250.0f, 0.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);
	SetModelWall(D3DXVECTOR3(720.0f, 140.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3);
	SetGate(D3DXVECTOR3(720.0f, 0.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6);
	SetModelWall(D3DXVECTOR3(1200.0f, 0.0f, -900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);



}


//=============================================================================
// エリア0オブジェクト設置
//=============================================================================
void SetAreaZero(void)
{
	//スコアアイテム設置
	SetScoreItem(D3DXVECTOR3(20.0f, 20.0f, -3730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点
	SetScoreItem(D3DXVECTOR3(80.0f, 20.0f, -3730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点
	SetScoreItem(D3DXVECTOR3(140.0f, 20.0f, -3730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点

}

//=============================================================================
// エリア１オブジェクト設置
//=============================================================================
void SetAreaOne(void)
{
	//瓦礫
	SetModel(D3DXVECTOR3(320, 0.0f, -3050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	SetModel(D3DXVECTOR3(1200, 0.0f, -2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetModel(D3DXVECTOR3(1200, 0.0f, -3050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//スコアアイテム設置
	SetScoreItem(D3DXVECTOR3(180.0f, 20.0f, -2900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点
	SetScoreItem(D3DXVECTOR3(1400.0f, 20.0f, -2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点
	SetScoreItem(D3DXVECTOR3(20.0f, 20.0f, -3100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点
	SetScoreItem(D3DXVECTOR3(1400.0f, 20.0f, -3100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点

	SetEnemy(D3DXVECTOR3(750.0f, 0.0f, -2600.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 3);
}


//=============================================================================
// エリア2オブジェクト設置
//=============================================================================
void SetAreaTwo(void)
{

	//スコアアイテム設置
	SetScoreItem(D3DXVECTOR3(600.0f, 20.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetScoreItem(D3DXVECTOR3(920.0f, 20.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetScoreItem(D3DXVECTOR3(40.0f, 20.0f, -1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	//瓦礫
	SetModel(D3DXVECTOR3(800, 0.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetModel(D3DXVECTOR3(700, 0.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//砲台
	SetPedestal(D3DXVECTOR3(500.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(500.0f, 80.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);
	SetPedestal(D3DXVECTOR3(900.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(900.0f, 80.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);
	SetEnemy(D3DXVECTOR3(750.0f, 0.0f, -1180.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 4);

}

//=============================================================================
// エリア3オブジェクト設置
//=============================================================================
void SetAreaThree(void)
{
	SetScoreItem(D3DXVECTOR3(20.0f, 20.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点
	SetScoreItem(D3DXVECTOR3(1200.0f, 20.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//スタート地点

	 //砲台
	SetPedestal(D3DXVECTOR3(300.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(300.0f, 80.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);

	SetPedestal(D3DXVECTOR3(1000.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(1000.0f, 80.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3);

	SetPedestal(D3DXVECTOR3(700.0f, 0.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetCanon(D3DXVECTOR3(700.0f, 80.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 5);

	//敵
	SetEnemy(D3DXVECTOR3(750.0f, 0.0f, -70.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 3);

}