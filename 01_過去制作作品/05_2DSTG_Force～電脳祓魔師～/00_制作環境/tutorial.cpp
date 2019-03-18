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
#include "tutorial.h"
#include "tutoriallogo.h"
#include "inputx.h"
//***************************************************************************************************************************************
// 静的メンバ変数
//***************************************************************************************************************************************
CTutorial *CTutorial::m_pTutorial = NULL;				//チュートリアルのポインタ
CTutorialLogo *CTutorial::m_pTutorialLogo = NULL;				//チュートリアルロゴのポインタ
CPlayer *CTutorial::m_pPlayer = NULL;			//プレイヤーのポインタ
CEnemy *CTutorial::m_apEnemy[MAX_ENEMY] = {};	//敵のポインタ
CUi *CTutorial::m_apUi[MAX_UI] = {};			//UIのポインタ
CBg *CTutorial::m_pBg = NULL;					//背景のポインタ
CFollowForce *CTutorial::m_pFollowForce = NULL;	//フォースのポインタ
CForce *CTutorial::m_pForce = NULL;				//フォースのポインタ
CBullet *CTutorial::m_pBullet = NULL;			//弾のポインタ
CEffect *CTutorial::m_pEffect = NULL;			//エフェクトのポインタ
CExplosion *CTutorial::m_pExplosion = NULL;		//爆発のポインタ
CNumber *CTutorial::m_pNumber = NULL;			//数字のポインタ
CCutin *CTutorial::m_pCutin = NULL;				//カットインのポインタ
CRemaining *CTutorial::m_pRemaining = NULL;		//残機数のポインタ
CParticle *CTutorial::m_pParticle = NULL;		//パーティクルのポインタ
CBoss *CTutorial::m_pBoss = NULL;				//ボスのポインタ
CForceNumber *CTutorial::m_pForceNumber = NULL;	//フォース数のポインタ
CHighScore *CTutorial::m_pHighScore = NULL;		//ハイスコアのポインタ
CScore *CTutorial::m_pScore = NULL;				//スコアのポインタ

CTutorial::TUTORIALSTATE CTutorial::m_TutorialState = TUTORIALSTATE_NORMAL;//ゲームの状態
int CTutorial::m_nCntTutorial = 0;//シーンの移り
int CTutorial::m_nCntFrame = 0;//フレーム数のカウント
JoyState g_JoyStateTutorial;

//======================================================================================================================================
//コンストラクタ
//======================================================================================================================================
CTutorial::CTutorial()
{

}

//======================================================================================================================================
//デストラクタ
//======================================================================================================================================
CTutorial::~CTutorial()
{

}




//======================================================================================================================================
//初期化処理
//======================================================================================================================================
HRESULT CTutorial::Init(void)
{
	//ゲームパッドの状態
	g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

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

	//チュートリアルロゴのテクスチャの読み込み
	m_pTutorialLogo->Load();

	//ボスのテクスチャ読み込み
	m_pBoss->Load();
#endif


	//背景の生成
	m_pBg->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	////プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50.0f, 0.0f));


	////UIの生成
	m_apUi[0]->CUi::Create(D3DXVECTOR3(100.0f, SCREEN_HEIGHT / 2, 0.0f), m_apUi[0]->UI_TYPE_BG_L);			//下地
	m_apUi[1]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT / 2, 0.0f), m_apUi[0]->UI_TYPE_BG_R);			//下地
	m_apUi[2]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[1]->UI_TYPE_HIGHSCORE);	//ハイスコア
	m_apUi[3]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[2]->UI_TYPE_SCORE);		//スコア
	m_apUi[4]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[3]->UI_TYPE_ATK);			//攻のフォース数
	m_apUi[5]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[4]->UI_TYPE_SPR);			//拡のフォース数
	m_apUi[6]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[5]->UI_TYPE_SPD);			//速のフォース数
	m_apUi[7]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[6]->UI_TYPE_DEF);			//護のフォース数
	m_apUi[8]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[7]->UI_TYPE_TIME);			//タイムの文字
	m_apUi[9]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[8]->UI_TYPE_COLON);		//コロン
	m_apUi[10]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[9]->UI_TYPE_DOT);			//ドット
	m_apUi[11]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[10]->UI_TYPE_LOG);			//ログの文字


	//敵の配置
	m_apEnemy[0]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f),CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_NONE);

	////追従フォースの生成
	m_pFollowForce->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	////チュートリアルロゴの生成
	m_pTutorialLogo->CTutorialLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120.0f, 0.0f));


	////フォース数の生成
	m_pForceNumber->Create(D3DXVECTOR3(150.0f, SCREEN_HEIGHT / 2 - 25.0f, 0.0f));

	////スコアの生成
	m_pScore->Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 610.0f, SCREEN_HEIGHT / 2 - 170.0f, 0.0f), CScore::SCORETYPE_GAME);
	m_pHighScore->Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 610.0f, SCREEN_HEIGHT / 2 - 280.0f, 0.0f));

	////フォースの生成
	m_pForce->Create(D3DXVECTOR3(25.0f, SCREEN_HEIGHT / 2 - 100.0f, 0.0f));

	////タイマーの生成
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));

	////残機数の生成
	m_pRemaining->Create(D3DXVECTOR3(80.0f, SCREEN_HEIGHT / 2 + 250.0f, 0.0f));


	m_nCntTutorial = 0;
	return S_OK;

}
//======================================================================================================================================
//終了処理
//======================================================================================================================================
void CTutorial::Uninit()
{

	//全シーンの破棄
	CScene::ReleaseSceneAll();


	// ->
#if 1
	//ボスのテクスチャ破棄
	m_pBoss->Unload();

	//チュートリアルロゴのテクスチャの破棄
	m_pTutorialLogo->Unload();

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
void CTutorial::Update()
{
	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();



	//敵の設置
	//SetEnemy();


	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();



	switch (m_TutorialState)
	{
	case TUTORIALSTATE_NORMAL:
		break;

	case TUTORIALSTATE_CLEAR:
		m_nCntTutorial--;
		if (m_nCntTutorial <= 0)
		{
			m_TutorialState = TUTORIALSTATE_NONE;
			//CManager::SetHighScore(CScore::GetScore());
			//CManager::SetScore(CScore::GetScore());

			//モード設定
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;

	case TUTORIALSTATE_FAILED:
		m_nCntTutorial--;
		if (m_nCntTutorial <= 0)
		{
			m_TutorialState = TUTORIALSTATE_NONE;
			//CManager::SetHighScore(CScore::GetScore());
			//CManager::SetScore(CScore::GetScore());

			//モード設定
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;
	}

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//Rキーが押されたとき
			if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
				CFade::SetFade(CManager::MODE_GAME);
			}
			g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//======================================================================================================================================
//描画処理
//======================================================================================================================================
void CTutorial::Draw()
{
}
//======================================================================================================================================
//プレイヤーの取得
//======================================================================================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

CBoss * CTutorial::GetBoss(void)
{
	return m_pBoss;
}

//======================================================================================================================================
//敵の取得
//======================================================================================================================================
CEnemy *CTutorial::GetEnemy(int nCntEnemy)
{
	return m_apEnemy[nCntEnemy];
}

//======================================================================================================================================
//敵の設置
//======================================================================================================================================
void CTutorial::SetEnemy(void)
{
#if 0
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
	case 2460:
		m_apEnemy[17]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;
	case 2700:
		m_apEnemy[18]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
		//==================================================================================================================================
		//第六陣(合計：6秒)
		//==================================================================================================================================
	case 3160:
		m_apEnemy[19]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -250.0f, 0.0f), CEnemy::ENEMYTYPE_SPREAD, CEnemy::ENEMYMOVE_STOP);
		m_apEnemy[20]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f), CEnemy::ENEMYTYPE_DEFENSE, CEnemy::ENEMYMOVE_THROUGH_UNDER);

		break;
	}
#endif
}

//======================================================================================================================================
//ゲーム状態の設定
//======================================================================================================================================
void CTutorial::SetTutorialState(TUTORIALSTATE state)
{
	m_TutorialState = state;
	m_nCntTutorial = 40;
}

//======================================================================================================================================
//ゲーム状態の取得
//======================================================================================================================================
CTutorial::TUTORIALSTATE CTutorial::GetTutorialState(void)
{
	return m_TutorialState;
}

//======================================================================================================================================
//フレーム数の取得
//======================================================================================================================================
int CTutorial::GetFrame(void)
{
	return m_nCntFrame;


}


