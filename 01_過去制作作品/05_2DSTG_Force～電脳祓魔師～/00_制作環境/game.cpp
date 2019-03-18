//=======================================================================================================================
//
// マネージャ処理 [manager.cpp]
// Author :岸田怜(Kishida Rei)
//
//=======================================================================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "ui.h"
#include "effect.h"
#include "sound.h"
#include "score.h"
#include"number.h"
#include "polygon.h"
#include "force.h"
#include "timer.h"
#include "followforce.h"
#include "title.h"
#include "game.h"
#include "fade.h"
#include "remaining.h"
#include "boss.h"
#include "highscore.h"
#include "forcenumber.h"
#include "cutin.h"
#include "particle.h"
#include "pause.h"
#include "log.h"
#include "inputx.h"
//***************************************************************************************************************************************
// 静的メンバ変数
//***************************************************************************************************************************************
CGame *CGame::m_pGame = NULL;				//ゲームのポインタ
CPlayer *CGame::m_pPlayer = NULL;			//プレイヤーのポインタ
CEnemy *CGame::m_apEnemy[MAX_ENEMY] = {};	//敵のポインタ
CUi *CGame::m_apUi[MAX_UI] = {};			//UIのポインタ
CBg *CGame::m_pBg = NULL;					//背景のポインタ
CFollowForce *CGame::m_pFollowForce = NULL;	//フォースのポインタ
CForce *CGame::m_pForce = NULL;				//フォースのポインタ
CLog *CGame::m_pLog = NULL;					//ログのポインタ
CBullet *CGame::m_pBullet = NULL;			//弾のポインタ
CEffect *CGame::m_pEffect = NULL;			//エフェクトのポインタ
CExplosion *CGame::m_pExplosion = NULL;		//爆発のポインタ
CNumber *CGame::m_pNumber = NULL;			//数字のポインタ
CCutin *CGame::m_pCutin = NULL;				//カットインのポインタ
CRemaining *CGame::m_pRemaining = NULL;		//残機数のポインタ
CParticle *CGame::m_pParticle = NULL;		//パーティクルのポインタ
CBoss *CGame::m_pBoss = NULL;				//ボスのポインタ
CForceNumber *CGame::m_pForceNumber = NULL;	//フォース数のポインタ
CHighScore *CGame::m_pHighScore = NULL;		//ハイスコアのポインタ
CScore *CGame::m_pScore = NULL;				//スコアのポインタ

CGame::GAMESTATE CGame::m_GameState = GAMESTATE_NORMAL;	//ゲームの状態
int CGame::m_nCntGame = 0;//シーンの移り
int CGame::m_nCntFrame = 0;//フレーム数のカウント
JoyState g_JoyStateGame;
//======================================================================================================================================
//コンストラクタ
//======================================================================================================================================
CGame::CGame()
{

}

//======================================================================================================================================
//デストラクタ
//======================================================================================================================================
CGame::~CGame()
{

}




//======================================================================================================================================
//初期化処理
//======================================================================================================================================
HRESULT CGame::Init(void)
{
	//ゲームパッドの状態
	g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


	if (CRenderer::GetPause() == true)
	{
		CRenderer::SetPause();
		CPause::SetSelect();
	}

	m_nCntFrame = 0;


	// ->
#if 1
	//背景のテクスチャの読み込み
	m_pBg->Load();

	//敵のテクスチャ読み込み
	CEnemy::Load();


	//UIのテクスチャの読み込み
	CUi::Load();

	//フォースのテクスチャ読み込み
	m_pFollowForce->Load();

	//フォースのテクスチャ読み込み
	m_pForce->Load();

	//ログのテクスチャの読み込み
	m_pLog->Load();

	//プレイヤーのテクスチャの読み込み
	m_pPlayer->Load();

	//弾のテクスチャの読み込み
	m_pBullet->Load();

	//エフェクトテクスチャの読み込み
	m_pEffect->Load();

	//爆発のテクスチャの読み込み
	m_pExplosion->Load();

	//数字のテクスチャの読み込み
	m_pNumber->Load();

	//カットインのテクスチャ読み込み
	m_pCutin->Load();

	//残機数のテクスチャ読み込み
	m_pRemaining->Load();

	//パーティクルのテクスチャの読み込み
	m_pParticle->Load();

	//ボスのテクスチャ読み込み
	m_pBoss->Load();
#endif


	//背景の生成
	m_pBg->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 300.0f, 0.0f));


	//UIの生成
	m_apUi[0]->CUi::Create(D3DXVECTOR3(100.0f, SCREEN_HEIGHT / 2, 0.0f), m_apUi[0]->UI_TYPE_BG_L);					//下地(L)
	m_apUi[1]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT / 2, 0.0f), m_apUi[0]->UI_TYPE_BG_R);	//下地(R)
	m_apUi[2]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[1]->UI_TYPE_HIGHSCORE);	//ハイスコア
	m_apUi[3]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[2]->UI_TYPE_SCORE);		//スコア
	m_apUi[4]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[3]->UI_TYPE_ATK);			//攻のフォース数
	m_apUi[5]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[4]->UI_TYPE_SPR);			//拡のフォース数
	m_apUi[6]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[5]->UI_TYPE_SPD);			//速のフォース数
	m_apUi[7]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[6]->UI_TYPE_DEF);			//護のフォース数
	m_apUi[8]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[7]->UI_TYPE_TIME);		//タイムの文字
	m_apUi[9]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[8]->UI_TYPE_COLON);		//コロン
	m_apUi[10]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[9]->UI_TYPE_DOT);		//ドット
	m_apUi[11]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[10]->UI_TYPE_LOG);		//ログの文字

	//追従フォースの生成
	m_pFollowForce->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//フォース数の生成
	m_pForceNumber->Create(D3DXVECTOR3(150.0f, SCREEN_HEIGHT / 2 - 25.0f, 0.0f));

	//スコアの生成
	m_pScore->Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 610.0f, SCREEN_HEIGHT / 2 - 170.0f, 0.0f), CScore::SCORETYPE_GAME);
	m_pHighScore->Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 610.0f, SCREEN_HEIGHT / 2 - 280.0f, 0.0f));

	//フォースの生成
	m_pForce->Create(D3DXVECTOR3(25.0f, SCREEN_HEIGHT / 2 - 100.0f, 0.0f));

	//タイマーの生成
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));

	//残機数の生成
	m_pRemaining->Create(D3DXVECTOR3(80.0f, SCREEN_HEIGHT / 2 + 250.0f, 0.0f));

	//m_pBoss->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150.0f, 0.0f));

	m_GameState = GAMESTATE_NORMAL;//ゲームの状態設定
	m_nCntGame = 0;
	return S_OK;

}
//======================================================================================================================================
//終了処理
//======================================================================================================================================
void CGame::Uninit()
{

	//全シーンの破棄
	CScene::ReleaseSceneAll();


	// ->
#if 1
	//ボスのテクスチャ破棄
	m_pBoss->Unload();

	//パーティクルのテクスチャの破棄
	m_pParticle->Unload();

	//残機数のテクスチャの破棄
	m_pRemaining->Unload();

	//カットインのテクスチャ破棄
	m_pCutin->Unload();

	//数字のテクスチャの破棄
	m_pNumber->Unload();

	//爆発のテクスチャの破棄
	m_pExplosion->Unload();

	//エフェクトテクスチャの破棄
	m_pEffect->Unload();

	//弾のテクスチャの破棄
	m_pBullet->Unload();

	//プレイヤーのテクスチャの破棄
	m_pPlayer->Unload();

	//ログのテクスチャの破棄
	m_pLog->Unload();

	//フォースのテクスチャ破棄
	m_pForce->Unload();

	//追従フォースのテクスチャ破棄
	m_pFollowForce->Unload();

	//敵のテクスチャの破棄

	CEnemy::Unload();


	//UIのテクスチャの破棄

	CUi::Unload();

	//背景のテクスチャの破棄
	m_pBg->Unload();
#endif

}
//======================================================================================================================================
//更新処理
//======================================================================================================================================
void CGame::Update()
{
	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (m_GameState == GAMESTATE_NORMAL)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_P) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//ポーズON/OFF
			if (g_JoyStateGame.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				CRenderer::SetPause();
			}
			g_JoyStateGame.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	//敵の設置
	SetEnemy();


	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();



	switch (m_GameState)
	{
	case GAMESTATE_NORMAL:
		break;

	case GAMESTATE_CLEAR:
		m_nCntGame--;
		if (m_nCntGame <= 0)
		{
			m_GameState = GAMESTATE_NONE;
			CManager::SetHighScore(CScore::GetScore());
			CManager::SetScore(CScore::GetScore());

			//モード設定
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;

	case GAMESTATE_FAILED:
		m_nCntGame--;
		if (m_nCntGame <= 0)
		{
			m_GameState = GAMESTATE_NONE;
			CManager::SetHighScore(CScore::GetScore());
			CManager::SetScore(CScore::GetScore());

			//モード設定
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;
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

//======================================================================================================================================
//描画処理
//======================================================================================================================================
void CGame::Draw()
{
}
//======================================================================================================================================
//プレイヤーの取得
//======================================================================================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

CBoss * CGame::GetBoss(void)
{
	return m_pBoss;
}

//======================================================================================================================================
//敵の取得
//======================================================================================================================================
CEnemy *CGame::GetEnemy(int nCntEnemy)
{
	return m_apEnemy[nCntEnemy];
}

//======================================================================================================================================
//敵の設置
//======================================================================================================================================
void CGame::SetEnemy(void)
{
#if 1
	bool bPause;
	bPause = CRenderer::GetPause();
	//フレーム数加算
	if (bPause == false)
	{
		m_nCntFrame++;
	}
	switch (m_nCntFrame)
	{
		//==================================================================================================================================
		//第一陣(合計：6秒)
		//==================================================================================================================================
	case 80:
		m_apEnemy[0]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
	case 120:
		m_apEnemy[1]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;

		//==================================================================================================================================
		//第二陣(合計：6秒)
		//==================================================================================================================================
	case 510:
		m_apEnemy[3]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;
	case 550:
		m_apEnemy[4]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;

		//==================================================================================================================================
		//第三陣(合計：12秒)
		//==================================================================================================================================
	case 1060:
		m_apEnemy[6]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[7]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[8]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		break;
		//==================================================================================================================================
		//第四陣(合計：12秒)
		//==================================================================================================================================
	case 1500:
		m_apEnemy[10]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		m_apEnemy[11]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		m_apEnemy[12]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		break;
		//==================================================================================================================================
		//第五陣(合計：9秒)
		//==================================================================================================================================
	case 2000:
		m_apEnemy[14]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_THROUGH_UNDER);
		m_apEnemy[15]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f), CEnemy::ENEMYTYPE_SPEED, CEnemy::ENEMYMOVE_STOP);
		m_apEnemy[16]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 220.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_THROUGH_UNDER);
		break;
		//==================================================================================================================================
		//第六陣(合計：6秒)
		//==================================================================================================================================
	case 2430:
		m_apEnemy[17]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;
	case 2670:
		m_apEnemy[18]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
		//==================================================================================================================================
		//第六陣(合計：6秒)
		//==================================================================================================================================
	case 3160:
		m_apEnemy[19]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -250.0f, 0.0f), CEnemy::ENEMYTYPE_SPREAD, CEnemy::ENEMYMOVE_STOP);
		m_apEnemy[20]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f), CEnemy::ENEMYTYPE_DEFENSE, CEnemy::ENEMYMOVE_THROUGH_UNDER);
		break;
		//==================================================================================================================================
		//第七陣(合計：6秒)
		//==================================================================================================================================
	case 3700:
		m_apEnemy[21]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, -150.0f, 0.0f), CEnemy::ENEMYTYPE_SPEED, CEnemy::ENEMYMOVE_STOP);
		m_apEnemy[22]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, -150.0f, 0.0f), CEnemy::ENEMYTYPE_SPEED, CEnemy::ENEMYMOVE_STOP);
		break;
		//==================================================================================================================================
		//第八陣(合計：6秒)
		//==================================================================================================================================
	case 4020:
		m_apEnemy[23]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
	case 4060:
		m_apEnemy[24]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
	case 4000:
		m_apEnemy[25]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
		//==================================================================================================================================
		//第八.五陣(合計：6秒)
		//==================================================================================================================================
	case 4300:
		m_apEnemy[26]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f), CEnemy::ENEMYTYPE_DEFENSE, CEnemy::ENEMYMOVE_THROUGH_UNDER);
		break;
		//==================================================================================================================================
		//第九陣(合計：6秒)
		//==================================================================================================================================
	case 4410:
		m_apEnemy[27]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
	case 4450:
		m_apEnemy[28]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
	case 4490:
		m_apEnemy[29]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
		//==================================================================================================================================
		//第十陣(合計：6秒)
		//==================================================================================================================================
	case 4700:
		m_apEnemy[30]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;
	case 5000:
		m_apEnemy[31]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
		//==================================================================================================================================
		//第十一陣(合計：12秒)
		//==================================================================================================================================
	case 5420:
		m_apEnemy[32]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[33]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[34]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		break;
		//==================================================================================================================================
		//第十二陣(合計：12秒)
		//==================================================================================================================================
	case 5720:
		m_apEnemy[35]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		m_apEnemy[36]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		m_apEnemy[37]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		break;
		//==================================================================================================================================
		//第十三陣(合計：5秒)
		//==================================================================================================================================
	case 6000:
		m_apEnemy[38]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_THROUGH_UNDER);
		break;
		//==================================================================================================================================
		//第十四陣(合計：12秒)
		//==================================================================================================================================
	case 6400:
		m_apEnemy[39]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -200.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_SPEED, CEnemy::ENEMYMOVE_STOP);
		m_apEnemy[40]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_SPEED, CEnemy::ENEMYMOVE_STOP);
		break;
		//==================================================================================================================================
		//第十五陣(合計：12秒)
		//==================================================================================================================================
	case 6700:
		m_apEnemy[41]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_DEFENSE, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[42]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[43]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[44]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -260.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		break;
		//==================================================================================================================================
		//第十六陣(合計：12秒)
		//==================================================================================================================================
	case 7200:
		m_apEnemy[45]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_DEFENSE, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[46]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[47]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[48]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -260.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		break;
		//==================================================================================================================================
		//ボス出現
		//==================================================================================================================================
	case 7700:
		m_pBoss->CBoss::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 , -150.0f, 0.0f));

		break;
	}
#endif
}

//======================================================================================================================================
//ゲーム状態の設定
//======================================================================================================================================
void CGame::SetGameState(GAMESTATE state)
{
	m_GameState = state;
	m_nCntGame = 40;
}

//======================================================================================================================================
//ゲーム状態の取得
//======================================================================================================================================
CGame::GAMESTATE CGame::GetGameState(void)
{
	return m_GameState;
}

//======================================================================================================================================
//フレーム数の取得
//======================================================================================================================================
int CGame::GetFrame(void)
{
	return m_nCntFrame;


}


