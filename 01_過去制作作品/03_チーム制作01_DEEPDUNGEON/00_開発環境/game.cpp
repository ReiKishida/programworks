//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "inputx.h"
#include "fade.h"
#include "light.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "pause.h"
#include "Stage1.h"
#include "Stage2.h"
#include "StageBoss.h"
#include "Stagefade.h"
#include "Wall1.h"
#include "Wall2.h"
#include "WallBoss.h"
#include "Select.h"
#include "life.h"
#include "txt.h"
#include "motion.h"
#include "magic.h"
#include "particle.h"
#include "magicsquare.h"
#include "sencer.h"
#include "brazen.h"
#include "erosion.h"
#include "SencerAtk.h"
#include "model.h"

//敵
#if 1
#include "enemy.h"
#include "enemymotion.h"
#include "enemytext.h"

#include "enemy0.h"
#include "enemymotion0.h"
#include "enemytext0.h"

#include "enemy1.h"
#include "enemymotion1.h"
#include "enemytext1.h"

#include "enemy2.h"
#include "enemymotion2.h"
#include "enemytext2.h"

#include "enemy3.h"
#include "enemymotion3.h"
#include "enemytext3.h"

#include "enemy4.h"
#include "enemymotion4.h"
#include "enemytext4.h"


#include "enemy5.h"
#include "enemymotion5.h"
#include "enemytext5.h"

#include "enemy6.h"
#include "enemymotion6.h"
#include "enemytext6.h"


#include "enemy7.h"
#include "enemymotion7.h"
#include "enemytext7.h"

#include "enemy8.h"
#include "enemymotion8.h"
#include "enemytext8.h"

#endif

//敵(弓)
#if 1

#include "enemy_arrow.h"
#include "enemy_arrow_motion.h"
#include "enemy_arrow_text.h"

#include "enemyarcher0.h"
#include "enemyarchermotion0.h"
#include "enemyarchertext0.h"


#include "enemyarcher1.h"
#include "enemyarchermotion1.h"
#include "enemyarchertext1.h"


#include "enemyarcher2.h"
#include "enemyarchermotion2.h"
#include "enemyarchertext2.h"

#endif

//敵(斧)
#if 1

#include "enemy_axe.h"
#include "enemy_axe_motion.h"
#include "enemy_axe_text.h"

#include "enemy_axe2.h"
#include "enemy_axe_motion2.h"
#include "enemy_axe_text2.h"
#endif


#include "SencerCol.h"
#include "SencerLock.h"
#include "SencerEnemy.h"
#include "sencerArrow.h"
#include "SencerNextStage.h"
#include "SencerEnableAttack.h"
#include "SencerEnableBoss.h"
#include "fog.h"
#include "died.h"
#include "boss.h"
#include "bossmotion.h"
#include "bosstext.h"
#include "message.h"
#include "arrow.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_NCNT (50)
//=============================================================================
// グローバル変数宣言
//=============================================================================
GAMESTATE g_gameState;			// ゲームの状態
GAMESTATE g_gameState2;	// ゲームの最終状態
int g_nCounterGameState = 0;					// 状態管理カウンタ
STAGEMODE g_StageMode = STAGEMODE_STAGE1;		// ステージの状態
JoyState g_PauseJoyState;						// ポーズのジョイパッド状態
bool g_bPause = false;							// ポーズの状態
bool g_bSelect = false;							// 階段移動


int nCntMagicSquare[MAX_NCNT];
int nCntSencer[MAX_NCNT];
int nCntEnemy[MAX_NCNT];

//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
void InitGame(void)
{
	g_gameState = GAMESTATE_NORMAL;		// 通常状態に設定
	g_nCounterGameState = 0;			// カウンタを初期化
	g_bPause = false;
	g_bSelect = false;
	g_StageMode = STAGEMODE_STAGE1;
	//ステージフェードの初期化
	InitStageFade(g_StageMode);

	/////////////////////////////////////////////////////////////////////////////////
	/////					テキスト											////
	///////////////////////////////////////////////////////////////////////////////

	//エネミーテキスト読み込み(スケ剣)
#if 1
	LoadEnemyData();
	LoadEnemyData_0();
	LoadEnemyData_1();
	LoadEnemyData_2();
	LoadEnemyData_3();
	LoadEnemyData_4();
	LoadEnemyData_5();
	LoadEnemyData_6();
	LoadEnemyData_7();
	LoadEnemyData_8();

#endif

	//エネミーテキスト読み込み(スケ弓)
#if 1

	LoadEnemyArrowData();
	LoadEnemyArcherData_0();
	LoadEnemyArcherData_1();
	LoadEnemyArcherData_2();
#endif

	//エネミーテキスト読み込み(斧)
#if 1

	LoadEnemyAxeData();
	LoadEnemyAxeData2();
#endif

	//ボステキストの読み込み
	LoadBossData();
	//テキストデータの読み込み
	LoadData();
	//カメラの初期化
	InitCamera();
	//ライトの初期化
	InitLight();



	//モーションデータの初期化
	InitMotion();
	
	

	//ボスモーション初期化
	InitBossMotion();
	//影の初期化
	InitShadow();
	//パーティクルの初期化
	InitParticle();
	//魔法陣の初期化
	InitMagicSquare();
	//魔法陣の設置
	MagicSquareSet();
	//当たり判定用センサー初期化
	InitSencerCollision();
	//ロック用センサー初期化
	InitSencerLock();
	//エネミー用センサー初期化
	InitSencerEnemy();
	InitSencerEnemyArrow();
	//攻撃センサー初期化
	InitSencerAtk();
	//攻撃可能センサー初期化
	InitSencerEnableAttack();
	//攻撃可能センサー初期化
	InitSencerEnableBoss();

	//弓
	InitArrow();

	//次ステージ移動センサー
	InitSencerNext();
	//プレイヤーの初期化
	InitPlayer();
	
	//ライフの初期化
	InitLife();
	//瘴気濃度初期化
	InitBrazen();
	//浸食度初期化
	InitErosion();

	//魔法の初期化
	InitMagic();
	//選択肢の初期化
	InitSelect();
	//ポーズの初期化
	InitPause();
	//各種センサー初期化
	InitSencer();

	/////////////////////////////////////////////////////////////////////////////////
	/////					敵													////
	///////////////////////////////////////////////////////////////////////////////
	//敵の初期化
#if 1
	InitEnemy();
	InitEnemy_0();
	InitEnemy_1();
	InitEnemy_2();
	InitEnemy_3();
	InitEnemy_4();
	InitEnemy_5();
	InitEnemy_6();
	InitEnemy_7();
	InitEnemy_8();

#endif
	//敵(弓)の初期化
#if 1
	InitEnemyArrow();
	InitEnemyARCHER_0();
	InitEnemyARCHER_1();
	InitEnemyARCHER_2();
#endif
	//斧
#if 1

	InitEnemyAxe();
	//斧
	InitEnemyAxe2();
#endif
	//ボスの初期化
	InitBoss();

	//エネミーモーション初期化(スケ剣)
#if 1
	InitEnemyMotion();
	InitEnemyMotion_0();
	InitEnemyMotion_1();
	InitEnemyMotion_2();
	InitEnemyMotion_3();
	InitEnemyMotion_4();
	InitEnemyMotion_5();
	InitEnemyMotion_6();
	InitEnemyMotion_7();
	InitEnemyMotion_8();
#endif

	//エネミーモーション初期化(弓)
#if 1
	InitEnemyArrowMotion();
	InitEnemyArcherMotion_0();
	InitEnemyArcherMotion_1();
	InitEnemyArcherMotion_2();

#endif

	//エネミーモーション初期化(斧)
#if 1
	InitEnemyAxeMotion();
	InitEnemyAxeMotion2();
#endif
	//メッセージ初期化
	InitMessage();
	//死亡時処理
	InitDied();
	//フォグ
	//InitFog();
	//SetStageMode(STAGEMODE_STAGE1);
	SetStageMode(g_StageMode);
	//ステージの初期化処理
	switch (g_StageMode)
	{
	case STAGEMODE_STAGE1://ステージ１の時
		InitStage1();
		InitWall1();
		InitModel();

		break;
	case STAGEMODE_STAGE2://ステージ2の時
		InitStage2();
		InitWall2();
		break;
	case STAGEMODE_STAGE3://ステージ3の時
		InitStageBoss();
		InitWallBoss();
		break;
	}
}

//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UninitGame(void)
{
	//カメラの終了処理
	UninitCamera();
	//ライトの終了処理
	UninitLight();

	

	//ステージの終了処理
	switch (g_StageMode)
	{
	case STAGEMODE_STAGE1:
		UninitStage1();
		//壁の終了処理
		UninitWall1();
		UninitModel();
		break;
	case STAGEMODE_STAGE2:
		UninitStage2();
		UninitWall2();
		break;

	case STAGEMODE_STAGE3:
		UninitStageBoss();
		UninitWallBoss();
		break;
	}
	//影の終了処理
	UninitShadow();
	//魔法陣の終了処理
	UninitMagicSquare();
	//プレイヤーの終了処理
	UninitPlayer();
	//弓
	UninitArrow();
	
	//ライフの終了処理
	UninitLife();
	//魔法の終了処理
	UninitMagic();
	//パーティクルの終了処理
	UninitParticle();
	//選択肢の終了処理
	UninitSelect();
	//ステージフェードの終了処理
	UninitStageFade();
	//ポーズの終了処理
	UninitPause();
	//各種センサー終了処理
	UninitSencer();
	//ロック用センサー終了処理
	UninitSencerLock();
	//エネミー用センサー終了処理
	UninitSencerEnemy();
	UninitSencerEnemyArrow();

	//次ステージ移動センサー
	UninitSencerNext();
	//攻撃可能センサー終了処理
	UninitSencerEnableAttack();
	//攻撃可能センサー終了処理
	UninitSencerEnableBoss();

	//瘴気濃度終了処理
	UninitBrazen();
	//浸食度初期化
	UninitErosion();

	//攻撃センサー終了処理
	UninitSencerAtk();

	/////////////////////////////////////////////////////////////////////////////////
	/////					敵													////
	///////////////////////////////////////////////////////////////////////////////
	//敵の終了処理
#if 1
	UninitEnemy();
	UninitEnemy_0();
	UninitEnemy_1();
	UninitEnemy_2();
	UninitEnemy_3();
	UninitEnemy_4();
	UninitEnemy_5();
	UninitEnemy_6();
	UninitEnemy_7();
	UninitEnemy_8();

#endif
	//敵(弓)の初期化
#if 1
	UninitEnemyArrow();
	UninitEnemyARCHER_0();
	UninitEnemyARCHER_1();
	UninitEnemyARCHER_2();

#endif 
	//敵(斧)の初期化
#if 1
	UninitEnemyAxe();
	//敵(斧)の初期化
	UninitEnemyAxe2();
#endif
	//ボスの終了処理
	UninitBoss();


	//当たり判定用センサー終了処理
	UninitSencerCollision();
	//死亡時処理
	UninitDied();
	//メッセージ終了処理
	UninitMessage();


}

//=============================================================================
// ゲーム画面の更新処理
//=============================================================================
void UpdateGame(void)
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (GetKeyboardTrigger(DIK_P) == true || state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{// Pキー押下
		if (g_PauseJoyState.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			g_bPause = g_bPause ? false : true;
		}
		g_PauseJoyState.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	if (g_bPause == false)
	{// ゲームプレイ状態
		if (g_bSelect == false)
		{
			//カメラの更新処理
			UpdateCamera();
			//ライトの更新処理
			UpdateLight();
			
			//ステージの更新処理
			switch (g_StageMode)
			{
			case STAGEMODE_STAGE1:
				UpdateStage1();
				//壁の更新処理
				UpdateWall1();
				UpdateModel();
				break;
			case STAGEMODE_STAGE2:
				UpdateStage2();
				UpdateWall2();
				break;
			case STAGEMODE_STAGE3:
				UpdateStageBoss();
				UpdateWallBoss();

				break;
			}
			//影の更新処理
			UpdateShadow();
			//魔法陣の更新処理
			UpdateMagicSquare();
			
			//ライフの更新処理
			UpdateLife();
			//魔法の更新処理
			UpdateMagic();
			//パーティクルの更新処理
			UpdateParticle();


			/////////////////////////////////////////////////////////////////////////////////
			/////				モーション												////
			///////////////////////////////////////////////////////////////////////////////
			//モーションデータの更新処理
			UpdateMotion();

			
			//エネミーモーションデータの更新処理
#if  1

			UpdateEnemyMotion();
			UpdateEnemyMotion_0();
			UpdateEnemyMotion_1();
			UpdateEnemyMotion_2();
			UpdateEnemyMotion_3();
			UpdateEnemyMotion_4();
			UpdateEnemyMotion_5();
			UpdateEnemyMotion_6();
			UpdateEnemyMotion_7();
			UpdateEnemyMotion_8();

#endif 

			//エネミーモーションデータの更新処理(弓)
#if  1
			UpdateEnemyArrowMotion();
			UpdateEnemyArcherMotion_0();
			UpdateEnemyArcherMotion_1();
			UpdateEnemyArcherMotion_2();

#endif
			//エネミーモーションデータの更新処理(斧)
#if 1
			UpdateEnemyAxeMotion();
			UpdateEnemyAxeMotion2();
#endif
			//ボスモーションデータの更新処理
			UpdateBossMotion();
			//ロック用センサー更新処理
			UpdateSencerLock();
			//エネミー用センサー更新処理
			UpdateSencerEnemy();
			UpdateSencerEnemyArrow();
			

			//攻撃可能センサー更新処理
			UpdateSencerEnableAttack();
			UpdateSencerEnableBoss();
			//各種センサー更新処理
			UpdateSencer();
			//次ステージ移動センサー
			UpdateSencerNext();
			//瘴気濃度更新処理
			UpdateBrazen();
			//浸食度更新処理
			UpdateErosion();
			//弓
			UpdateArrow();
			//攻撃センサー更新処理
			UpdateSencerAtk();

			/////////////////////////////////////////////////////////////////////////////////
			/////					敵													////
			///////////////////////////////////////////////////////////////////////////////
#if  1
#endif
			//敵の更新処理
#if  1
			UpdateEnemy();
			UpdateEnemy_0();
			UpdateEnemy_1();
			UpdateEnemy_2();
			UpdateEnemy_3();
			UpdateEnemy_4();
			UpdateEnemy_5();
			UpdateEnemy_6();
			UpdateEnemy_7();
			UpdateEnemy_8();

#endif

#if 1
			//敵(弓)の初期化
			UpdateEnemyArrow();
			UpdateEnemyARCHER_0();
			UpdateEnemyARCHER_1();
			UpdateEnemyARCHER_2();

#endif

			//敵(斧)の初期化
#if 1
			UpdateEnemyAxe();
			//敵(斧)の初期化
			UpdateEnemyAxe2();
#endif
			//ボスの終了処理
			UpdateBoss();
			//当たり判定用センサー初期化
			UpdateSencerCollision();
			//プレイヤーの更新処理
			UpdatePlayer();
			//死亡時処理
			UpdateDied();
			//メッセージ更新処理
			UpdateMessage();
		}
		else
		{
			//エリア移動の選択の更新処理
			UpdateSelect();
		}

		//ステージフェードの更新処理
		UpdateStageFade();

		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:
			break;
		case GAMESTATE_CLEAR:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 50)
			{
				g_gameState2 = GAMESTATE_CLEAR;
				g_gameState = GAMESTATE_NONE;
				// 画面(モード)の設定
				SetFade(MODE_RESULT);
			}
			break;
		

		case GAMESTATE_CLEAR_75:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 50)
			{
				g_gameState2 = GAMESTATE_CLEAR_75;
				g_gameState = GAMESTATE_NONE;
				// 画面(モード)の設定
				SetFade(MODE_RESULT);
			}
			break;

		case GAMESTATE_CLEAR_50:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 50)
			{
				g_gameState2 = GAMESTATE_CLEAR_50;
				g_gameState = GAMESTATE_NONE;
				// 画面(モード)の設定
				SetFade(MODE_RESULT);
			}
			break;

		case GAMESTATE_CLEAR_25:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 50)
			{
				g_gameState2 = GAMESTATE_CLEAR_25;
				g_gameState = GAMESTATE_NONE;
				// 画面(モード)の設定
				SetFade(MODE_RESULT);
			}
			break;

		case GAMESTATE_CLEAR_TRUE:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 50)
			{
				g_gameState2 = GAMESTATE_CLEAR_TRUE;
				g_gameState = GAMESTATE_NONE;
				// 画面(モード)の設定
				SetFade(MODE_RESULT);
			}
			break;
		}
	}
	else
	{// ポーズ状態
		// ポーズの更新処理
		UpdatePause();
	}

	// ジョイパッドの状態更新
	if (g_PauseJoyState.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_PauseJoyState.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_PauseJoyState.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_PauseJoyState.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawGame(void)
{
	//カメラの設置
	SetCamera();
	//ステージの描画処理
	switch (g_StageMode)
	{
	case STAGEMODE_STAGE1:
		DrawStage1();
		//壁の描画処理
		DrawWall1();
		DrawModel();

		break;
	case STAGEMODE_STAGE2:
		DrawStage2();
		DrawWall2();
		break;
	case STAGEMODE_STAGE3:
		DrawStageBoss();
		DrawWallBoss();
		break;
	}
	//影の描画処理
	DrawShadow();
	//魔法陣の描画処理
	DrawMagicSquare();
	//プレイヤーの描画処理
	DrawPlayer();
	
	//ライフの描画処理
	DrawLife();
	//瘴気濃度描画処理
	DrawBrazen();
	DrawErosion();
	//魔法の描画処理
	DrawMagic();
	//パーティクルの描画処理
	DrawParticle();
	

	/////////////////////////////////////////////////////////////////////////////////
	/////				センサー												////
	///////////////////////////////////////////////////////////////////////////////
	//攻撃センサー描画処理
	//DrawSencerAtk();
	//攻撃可能センサー更新処理
	//DrawSencerEnableAttack();
	//DrawSencerEnableBoss();
	//ロック用センサー描画処理
	//DrawSencerLock();
	//エネミー用センサー描画処理
	//DrawSencerEnemy();
	//DrawSencerEnemyArrow();
	//当たり判定用センサー描画処理
	//DrawSencerCollision();

	/////////////////////////////////////////////////////////////////////////////////
	/////					敵													////
	///////////////////////////////////////////////////////////////////////////////
	//敵の描画処理
#if  1
	DrawEnemy();
	DrawEnemy_0();
	DrawEnemy_1();
	DrawEnemy_2();
	DrawEnemy_3();
	DrawEnemy_4();
	DrawEnemy_5();
	DrawEnemy_6();
	DrawEnemy_7();
	DrawEnemy_8();

#endif 

	
	//敵(弓)の初期化
#if 1
	DrawEnemyArrow();
	DrawEnemyARCHER_0();
	DrawEnemyARCHER_1();
	DrawEnemyARCHER_2();
#endif

	//敵(斧)の初期化
#if 1
	DrawEnemyAxe();
	//敵(斧)の初期化
	DrawEnemyAxe2();
#endif
	//弓
	DrawArrow();
	//ボスの終了処理
	DrawBoss();
	//各種センサー描画処理
	DrawSencer();

	//次ステージ移動センサー
	DrawSencerNext();
	
	//死亡時処理
	DrawDied();
	//メッセージ描画処理
	DrawMessage();
	if (g_bSelect == true)
	{
		//エリア移動の選択の描画処理
		DrawSelect();
	}

	//ステージフェードの描画処理
	DrawStageFade();

	if (g_bPause == true)
	{// ポーズ状態
		DrawPause();
	}
}

//=============================================================================
//モードの設定
//=============================================================================
void SetStageMode(STAGEMODE Stagemode)
{
	switch (g_StageMode)
	{
	case STAGEMODE_STAGE1:
		UninitStage1();
		UninitWall1();

		break;
	case STAGEMODE_STAGE2:
		UninitStage2();
		UninitWall2();
		break;
	case STAGEMODE_STAGE3:
		UninitStageBoss();
		UninitWallBoss();

		break;
	}

	//新しい画面モードの初期化設定
	switch (Stagemode)
	{
	case STAGEMODE_STAGE1:
		InitStage1();
		InitWall1();
		Set_Stage1();

		break;
	case STAGEMODE_STAGE2:
		Delete_Stage1();

		InitStage2();
		InitWall2();
		Set_Stage2();
		break;
	case STAGEMODE_STAGE3:
		InitStageBoss();
		InitWallBoss();
		Set_BossStage();

		break;
	}

	g_StageMode = Stagemode;//現在のモードを切り替える
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
// 現在のゲームの状態の取得
//=============================================================================
GAMESTATE *GetGameState2(void)
{
	return &g_gameState2;
}

//=============================================================================
// ポーズの状態
//=============================================================================
void SetPauseState(bool bPause)
{
	g_bPause = bPause;
}

//=============================================================================
// ゲームの取得処理[SetStage]
//=============================================================================
STAGEMODE GetStageMode(void)
{
	return g_StageMode;
}

//=============================================================================
// 選択状態の取得処理[SetGame]
//=============================================================================
void SetSelect(bool bSelect)
{
	g_bSelect = bSelect;
}

//=============================================================================
// 選択状態の取得[SetGame]
//=============================================================================
bool GetSelect(void)
{
	return g_bSelect;
}

//=============================================================================
// 魔法陣の設置
//=============================================================================
void MagicSquareSet(void)
{
}
//=============================================================================
// 各ステージの敵の配置
//=============================================================================
void Set_Stage1(void)
{
	//敵配置
	nCntEnemy[0] =	SetEnemy(D3DXVECTOR3(0.0f, 0.1f, 2650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	nCntEnemy[1] =	SetEnemyArrow(D3DXVECTOR3(0.0f, 0.1f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7);
	nCntEnemy[2] = SetEnemyAxe(D3DXVECTOR3(500.0f, 0.1f, 2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
	nCntEnemy[3] = SetEnemyAxe2(D3DXVECTOR3(200.0f, 0.1f, 2050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
	nCntEnemy[4] = SetEnemy_0(D3DXVECTOR3(700.0f, 0.1f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	nCntEnemy[4] = SetEnemy_1(D3DXVECTOR3(0.0f, 0.1f, 1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	nCntEnemy[5] = SetEnemy_2(D3DXVECTOR3(-100.0f, 0.1f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

	//各種センサー設置
	nCntSencer[0] =		 SetSencerNext(D3DXVECTOR3(180.0f, 0.0f, 1350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	nCntMagicSquare[0] = SetMagicSquare(D3DXVECTOR3(730.0f, 0.1f, 2130.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120.0f);
}
void Set_Stage2(void)
{
	nCntEnemy[0] = SetEnemy(D3DXVECTOR3(0.0f, 0.1f, 2850.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	nCntEnemy[1] = SetEnemyArrow(D3DXVECTOR3(100.0f, 0.1f, 2250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5);
	nCntEnemy[2] = SetEnemyAxe(D3DXVECTOR3(1500.0f, 0.0f, 750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
	nCntEnemy[3] = SetEnemyAxe2(D3DXVECTOR3(1250.0f, 0.1f, 1050), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
	nCntEnemy[4] = SetEnemy_0(D3DXVECTOR3(0, 0.1f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	nCntEnemy[5] = SetEnemy_1(D3DXVECTOR3(0, 0.1f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);


	nCntMagicSquare[0] = SetMagicSquare(D3DXVECTOR3(600.0f, 0.1f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120.0f);
	nCntMagicSquare[1] = SetMagicSquare(D3DXVECTOR3(-680.0f, 0.1f, 2330.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120.0f);

	nCntSencer[0] = SetSencerNext(D3DXVECTOR3(1500.0f, 0.0f, 950.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

}
void Set_BossStage(void)
{
	SetBoss(D3DXVECTOR3(0.0f, 0.1f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 70);
}


//=============================================================================
// 各ステージの削除
//=============================================================================
void Delete_Stage1(void)
{
	MAGICSQUARE *pMagicSquare;
	pMagicSquare = GetMagicSquare();

	DeleteEnemy(nCntEnemy[0]);
	DeleteEnemyArrow(nCntEnemy[1]);	
	DeleteEnemyAxe(nCntEnemy[2]);
	DeleteEnemyAxe2(nCntEnemy[3]);
	DeleteEnemy_0(nCntEnemy[3]);
	DeleteEnemy_1(nCntEnemy[4]);
	DeleteEnemy_2(nCntEnemy[5]);

	DeleteOnlyMagicSquare(nCntMagicSquare[0]);
	DeleteSencerNext(nCntSencer[0]);


	//nCntEnemy[0] = SetEnemy(D3DXVECTOR3(0.0f, 0.1f, 2650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	//nCntEnemy[1] = SetEnemyArrow(D3DXVECTOR3(0.0f, 0.1f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7);
	//nCntEnemy[2] = SetEnemyAxe(D3DXVECTOR3(500.0f, 0.1f, 2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
	//nCntEnemy[3] = SetEnemyAxe2(D3DXVECTOR3(200.0f, 0.1f, 2050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
	//nCntEnemy[4] = SetEnemy_0(D3DXVECTOR3(700.0f, 0.1f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	//nCntEnemy[4] = SetEnemy_1(D3DXVECTOR3(0.0f, 0.1f, 1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	//nCntEnemy[5] = SetEnemy_2(D3DXVECTOR3(-100.0f, 0.1f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);


}

void Delete_Stage2(void)
{
	DeleteEnemy(nCntEnemy[0]);
	DeleteEnemyArrow(nCntEnemy[1]);
	DeleteEnemyAxe(nCntEnemy[2]);
	DeleteEnemyAxe2(nCntEnemy[3]);


	DeleteSencerNext(nCntSencer[2]);
	DeleteOnlyMagicSquare(nCntMagicSquare[0]);

}

void Delete_BossStage(void)
{

}