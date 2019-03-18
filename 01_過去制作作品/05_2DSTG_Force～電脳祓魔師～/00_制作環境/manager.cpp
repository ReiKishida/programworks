//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
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
#include "result.h"
#include "fade.h"
#include "menu.h"
#include "ranking.h"
#include "menuranking.h"
#include "boss.h"
#include "tutorial.h"
#include "inputx.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;			//レンダラー
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	//キーボード
CInputX *CManager::m_pInputX = NULL;	//XInput
CPlayer *CManager::m_pPlayer = NULL;	//プレイヤー
CEnemy *CManager::m_pEnemy = NULL;		//敵
CBoss *CManager::m_pBoss = NULL;		//ボス
CSound *CManager::m_pSound = NULL;		//サウンド
CScore *CManager::m_pScore = NULL;		//スコア
CMenu *CManager::m_pMenu = NULL;		//メニュー
CTitle *CManager::m_pTitle  = NULL;//タイトル
CGame *CManager::m_pGame = NULL;//ゲーム
CTutorial *CManager::m_pTutorial = NULL;//チュートリアル
CResult *CManager::m_pResult = NULL;//リザルト
CRanking *CManager::m_pRanking = NULL;//ランキング
CMenuRanking *CManager::m_pMenuRanking = NULL;//ランキング
CFade *CManager::m_pFade = NULL;//フェード
CManager::MODE CManager::m_mode = MODE_TITLE;//モード
int CManager::m_nScore = 0;	//スコア
int CManager::m_nHighScore = 0;	//ハイスコア
//=============================================================================
//コンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CManager::~CManager()
{

}



//=============================================================================
//初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//ハイスコア
	m_nHighScore = 40000;
	//スコア
	m_nScore = 0;

	//レンダラーの生成
	m_pRenderer = new CRenderer;
	if (m_pRenderer != NULL)
	{
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{
			return -1;
		}
	}


	//キーボードの生成
	m_pInputKeyboard = new CInputKeyboard;
	if (m_pInputKeyboard != NULL)
	{
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}


	//ゲームパッドの初期化
	InitJoyPad();

	//サウンドの作成
	m_pSound = new CSound;
	if (m_pSound != NULL)
	{
		if (FAILED(m_pSound->InitSound(hWnd)))
		{
			return -1;
		}
	}

	//モードの設定
	SetMode(MODE_TITLE);



	return S_OK;

}
//=============================================================================
//終了処理
//=============================================================================
void CManager::Uninit()
{
	CScene::ReleaseSceneAll();

	//タイトル画面の終了処理
	m_pTitle->Uninit();
	//タイトル画面の破棄
	delete m_pTitle;
	m_pTitle = NULL;

	//メニュー画面の終了
	m_pMenu->Uninit();
	//メニュー画面の破棄
	delete m_pMenu;
	m_pMenu = NULL;

	//ゲーム画面の終了
	m_pGame->Uninit();
	//ゲーム画面の破棄
	delete m_pGame;
	m_pGame = NULL;

	//リザルト画面の終了
	m_pResult->Uninit();
	//リザルト画面の破棄
	delete m_pResult;
	m_pResult = NULL;

	//ランキング画面の終了
	m_pRanking->Uninit();
	//ランキング画面の破棄
	delete m_pRanking;
	m_pRanking = NULL;

	//チュートリアル画面の終了
	m_pTutorial->Uninit();
	//チュートリアル画面の破棄
	delete m_pRanking;
	m_pTutorial = NULL;



	//メニューランキング画面の終了
	m_pMenuRanking->Uninit();
	//ランキング画面の破棄
	delete m_pMenuRanking;
	m_pMenuRanking = NULL;

	//ゲームパッド終了
	UninitJoyPad();

	//サウンドの終了処理
	if (m_pSound != NULL)
	{
		//終了処理
		m_pSound->UninitSound();
		//サウンドの破棄
		delete m_pSound;
		m_pSound = NULL;

	}

	//サウンドの終了処理
	if (m_pFade != NULL)
	{
		//終了処理
		m_pFade->Uninit();

		//サウンドの破棄
		delete m_pFade;
		m_pFade = NULL;

	}


	//キーボードの終了処理
	if (m_pInputKeyboard != NULL)
	{
		// 終了処理
		m_pInputKeyboard->Uninit();

		//キーボードの破棄
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}


	//レンダラーの終了処理
	if (m_pRenderer != NULL)
	{
		// 終了処理
		m_pRenderer->Uninit();

		//レンダラーの破棄
		delete m_pRenderer;
		m_pRenderer = NULL;
	}


}
//=============================================================================
//更新処理
//=============================================================================
void CManager::Update()
{
	//キーボードの更新
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//レンダラーの更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}


	switch (m_mode)
	{//モード
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_MENU:
		m_pMenu->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Update();
		break;

	case MODE_RESULT:
		m_pResult->Update();
		break;

	case MODE_RANKING:
		m_pRanking->Update();
		break;

	case MODE_MENURANKING:
		m_pMenuRanking->Update();
		break;
	}

}

//=============================================================================
//描画処理
//=============================================================================
void CManager::Draw()
{
	//レンダラーの更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}

	switch (m_mode)
	{//モード
	case MODE_TITLE:
		m_pTitle->Draw();
		break;

	case MODE_MENU:
		m_pMenu->Draw();
		break;

	case MODE_GAME:
		m_pGame->Draw();
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Draw();
		break;

	case MODE_RESULT:
		m_pResult->Draw();
		break;

	case MODE_RANKING:
		m_pRanking->Draw();
		break;

	case MODE_MENURANKING:
		m_pMenuRanking->Draw();
		break;
	}

}

//=============================================================================
// レンダリングクラスの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
// インプットクラスの取得(キーボード)
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// インプットクラスの取得(XInput)
//=============================================================================
CInputX * CManager::GetInputX(void)
{
	return m_pInputX;
}

//=============================================================================
// プレイヤークラスの取得
//=============================================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// プレイヤークラスの取得
//=============================================================================
CEnemy *CManager::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// ボスクラスの取得
//=============================================================================
CBoss *CManager::GetBoss(void)
{
	return m_pBoss;
}

//=============================================================================
// サウンドクラスの取得
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// スコアクラスの取得
//=============================================================================
CScore * CManager::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		 m_pTitle->Uninit();
		 delete m_pTitle;
		 m_pTitle = NULL;
		 //サウンドの再生
		 m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE);
		break;

	case MODE_MENU:
		m_pMenu->Uninit();
		delete m_pMenu;
		m_pMenu = NULL;
		//サウンドの再生
		 m_pSound->StopSound(CSound::SOUND_LABEL_BGM_MENU);

		break;

	case MODE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
		//サウンドの再生
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		break;


	case MODE_TUTORIAL:
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
		//サウンドの再生
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TUTORIAL);
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = NULL;
		//サウンドの再生
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_RESULT);
		break;

	case MODE_RANKING:
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
		break;

	case MODE_MENURANKING:
		m_pMenuRanking->Uninit();
		delete m_pMenuRanking;
		m_pMenuRanking = NULL;
		break;
	}

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = new CTitle;
		m_pTitle->Init();
		//サウンドの再生
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
		break;

	case MODE_MENU:
		m_pMenu = new CMenu;
		m_pMenu->Init();
		//サウンドの再生
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_MENU);
		break;

	case MODE_GAME:
		m_pGame = new CGame;
		m_pGame->Init();
		//サウンドの再生
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
		break;

	case MODE_TUTORIAL:
		m_pTutorial = new CTutorial;
		m_pTutorial->Init();
		//サウンドの再生
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TUTORIAL);
		break;

	case MODE_RESULT:
		m_pResult = new CResult;
		m_pResult->Init();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
		break;

	case MODE_RANKING:
		m_pRanking = new CRanking;
		m_pRanking->Init();
		break;

	case MODE_MENURANKING:
		m_pMenuRanking = new CMenuRanking;
		m_pMenuRanking->Init();
		break;
	}

	m_mode = mode;
}

//=============================================================================
// モードの取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//ハイスコアの設定
//=============================================================================
void CManager::SetHighScore(int nValue)
{
	if (nValue > m_nHighScore)
	{
		m_nHighScore = nValue;
	}
}

//=============================================================================
//ハイスコアの取得
//=============================================================================
int CManager::GetHighScoreNum(void)
{
	return m_nHighScore;
}

//=============================================================================
//スコアの設定
//=============================================================================
void CManager::SetScore(int nValue)
{
	m_nScore = nValue;

}

//=============================================================================
//スコアの取得
//=============================================================================
int CManager::GetScoreNum(void)
{
	return m_nScore;
}
