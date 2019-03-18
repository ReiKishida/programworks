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
#include "light.h"
#include "camera.h"
#include "sceneX.h"
#include "debugproc.h"
#include "player.h"
#include "scene_meshfield.h"
#include "model.h"
#include "number.h"
#include "object.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "sound.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;			//レンダラーのポインタ
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	//キーボード入力のポインタ
CSound *CManager::m_pSound = NULL;					//サウンド
CCamera *CManager::m_pCamera = NULL;				//カメラ
CLight *CManager::m_pLight = NULL;					//ライト
CDebugProc *CManager::m_pDebugProc = NULL;			//デバッグ表示
CTitle *CManager::m_pTitle = NULL;					//タイトル
CGame *CManager::m_pGame = NULL;					//ゲーム
CTutorial *CManager::m_pTutorial = NULL;			//チュートリアル
CResult *CManager::m_pResult = NULL;				//リザルト
CRanking *CManager::m_pRanking = NULL;				//ランキング
CInputX *CManager::m_pInputX = NULL;				//ジョイパッド
CInputGamePad *CManager::m_pInputGamePad = NULL;     //ゲームパッド
// タイマーで使用
int CManager::m_nTimer;										//タイマー
int CManager::m_nTenMinutes = 0;							//10分台
int CManager::m_nOneMinutes = 0;							//1分台
int CManager::m_nTenSeconds = 4;							//10秒台
int CManager::m_nOneSeconds = 0;							//10秒台
int CManager::m_nTenComma = 0;								//コンマ秒
int CManager::m_nOneComma = 0;								//コンマ秒

//スコアで使用
int CManager::m_nScore = 0;
int CManager::m_nFinalScore = 0;
float CManager::m_fComboGaugeMax = 0.0f;							// コンボゲージに値を返す
float CManager::m_fDamage = 0.0f;
//ボーナスで使用
int CManager::m_nScoreBonus = 0;							// スコアに値を返す
int CManager::m_nFinalScoreBonus = 0;					// スコアに値を返す
int CManager::m_nComboGaugeMaxBonus = 0;			// コンボゲージに値を返す
int CManager::m_nDamageBonus = 0;					// ダメージに値を返す
int CManager::m_nTimerBonus = 0;


CManager::MODE CManager::m_mode = MODE_GAME;		//モードの設定
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される。
//=============================================================================
CManager::~CManager()
{

}


//=============================================================================
// 関数名：マネージャの初期化処理
// 関数の概要：レンダラー、デバック、キーボード、カメラの生成
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//レンダラーの生成
	m_pRenderer = new CRenderer;
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	//デバッグ表示の初期化
	m_pDebugProc = new CDebugProc;
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Init();
	}

	//サウンドの作成
	m_pSound = new CSound;
	if (m_pSound != NULL)
	{
		m_pSound->InitSound(hWnd);
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

	//ゲームパッドの生成
	if (m_pInputGamePad == NULL)
	{
		m_pInputGamePad = new CInputGamePad;
	}
	if (m_pInputGamePad != NULL)
	{// ゲームパッドが生成されている
		if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
		{// ゲームパッドの初期化
			return -1;
		}
	}


	//カメラの作成
	m_pCamera = new CCamera;
	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	//ライトの作成
	m_pLight = new CLight;
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}


	//===============================================================
	//モードの設定
	//===============================================================
	SetMode(MODE_TITLE);

	//ワイヤーフレームの有効
	m_bDebug = false;

	return S_OK;

}
//=============================================================================
// 関数名：マネージャの終了処理
// 関数の概要：レンダラー、デバック、キーボード、カメラの破棄
//=============================================================================
void CManager::Uninit()
{
	CScene::ReleaseSceneAll();

	switch (m_mode)
	{
	case MODE_TITLE:
		//タイトル画面の終了処理
		m_pTitle->Uninit();
		//タイトル画面の破棄
		delete m_pTitle;
		m_pTitle = NULL;
		break;

	case MODE_TUTORIAL:
		//チュートリアル画面の終了
		m_pTutorial->Uninit();
		//チュートリアル画面の破棄
		delete m_pTutorial;
		m_pTutorial = NULL;
		break;

	case MODE_GAME:
		//ゲーム画面の終了
		m_pGame->Uninit();
		//ゲーム画面の破棄
		delete m_pGame;
		m_pGame = NULL;
		break;

	case MODE_RESULT:
		//リザルト画面の終了
		m_pResult->Uninit();
		//リザルト画面の破棄
		delete m_pResult;
		m_pResult = NULL;
		break;

	case MODE_RANKING:
		//ランキング画面の終了
		m_pRanking->Uninit();
		//ランキング画面の破棄
		delete m_pRanking;
		m_pRanking = NULL;
		break;
	}

	//ライトの終了処理
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	//カメラの終了処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pInputGamePad != NULL)
	{// ゲームパッドの破棄
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
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

	//サウンドの終了処理
	if (m_pSound != NULL)
	{
		//終了処理
		m_pSound->UninitSound();
		//サウンドの破棄
		delete m_pSound;
		m_pSound = NULL;
	}


	//デバッグ表示の終了処理
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
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

	//オブジェクトのテクスチャの破棄
	CObject::Unload();


}
//=============================================================================
// 関数名：マネージャの更新処理
// 関数の概要：レンダラー、デバック、キーボード、カメラの更新
//=============================================================================
void CManager::Update()
{
	//キーボードの更新
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}


	//カメラの更新
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	//ライトの更新
	if (m_pLight != NULL)
	{
		m_pLight->Update();
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
	case MODE_TUTORIAL:
		m_pTutorial->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;


	case MODE_RESULT:
		m_pResult->Update();
		break;

	case MODE_RANKING:
		m_pRanking->Update();
		break;

	}

	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_0) == true)
	{//2キーが押されたとき
		m_bDebug = m_bDebug ? false : true;;
	}

	if (m_pInputGamePad != NULL)
	{// ゲームパッドの更新
		m_pInputGamePad->Update();
	}

}

//=============================================================================
// 関数名：マネージャの描画処理
// 関数の概要：レンダラーの描画、カメラの設置
//=============================================================================
void CManager::Draw()
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	if (m_bDebug == true)
	{
		// ワイヤーフレーム
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	}
	else if (m_bDebug == false)
	{
		// 非ワイヤーフレーム
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//カメラの設置
	if (m_pCamera != NULL)
	{
		m_pCamera->SetCamera();
	}

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

	case MODE_TUTORIAL:
		m_pTutorial->Draw();
		break;


	case MODE_GAME:
		m_pGame->Draw();
		break;


	case MODE_RESULT:
		m_pResult->Draw();
		break;

	case MODE_RANKING:
		m_pRanking->Draw();
		break;

	}
}

//=============================================================================
// 関数名：レンダリングクラスの取得
// 関数の概要：レンダラークラスを返す
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
// 関数名：インプットクラスの取得
// 関数の概要：インプットクラスを返す
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// 関数名：カメラクラスの取得
// 関数の概要：カメラクラスを返す
//=============================================================================
CCamera * CManager::GetCamera(void)
{
	return m_pCamera;
}


//=============================================================================
// ゲームパッドの取得
//=============================================================================
CInputGamePad *CManager::GetInputGamePad(void)
{
	return m_pInputGamePad;
}


//=============================================================================
// 関数名：サウンドクラスの取得
// 関数の概要：サウンドクラスを返す
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

CInputX * CManager::GetInput(void)
{
	return m_pInputX;
}


//=============================================================================
// 関数名：モードの設定
// 関数の概要：つぎのモードを設定する
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

	case MODE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
		//サウンドの再生
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		m_pSound->StopSound();
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
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGM_RESULT);
		break;

	case MODE_RANKING:
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
		//サウンドの再生
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGM_RANKING);

		m_nFinalScore = 0;
		m_nFinalScoreBonus = 0;
		m_nTimerBonus = 0;
		m_nDamageBonus = 0;
		m_nComboGaugeMaxBonus = 0;
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
		//サウンドの再生
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
		break;

	case MODE_RANKING:
		m_pRanking = new CRanking;
		m_pRanking->Init();
		//サウンドの再生
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RANKING);
		break;


	}

	m_mode = mode;
}

//=============================================================================
// 関数名：モードの取得
// 関数の概要：現在のモードを取得する
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}


//=============================================================================
// タイマーの設定
//=============================================================================
void CManager::SetTimer(int nTenMinutes, int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma)
{
	m_nTenMinutes = nTenMinutes;
	m_nOneMinutes = nOneMinutes;
	m_nTenSeconds = nTenSeconds;
	m_nOneSeconds = nOneSeconds;
	m_nTenComma = nTenComma;
	m_nOneComma = nOneComma;
}

//=============================================================================
//タイマーの取得
//=============================================================================
int CManager::GetTimer(void)
{
	int nTenMinutes = m_nTenMinutes;
	int nOneMinutes = m_nOneMinutes;
	int nTenSeconds = m_nTenSeconds;
	int nOneSeconds = m_nOneSeconds;
	int nTenComma = m_nTenComma;
	int nOneComma = m_nOneComma;

	m_nTenMinutes = nTenMinutes * 100000;
	m_nOneMinutes = nOneMinutes * 10000;
	nTenSeconds = nTenSeconds * 1000;
	nOneSeconds = nOneSeconds * 100;
	nTenComma = nTenComma * 10;
	nOneComma = nOneComma;

	m_nTimer = m_nTenMinutes + nOneMinutes + nTenSeconds + nOneSeconds + nTenComma + nOneComma;

	return m_nTimer;
}

//=============================================================================
//スコアの設定
//=============================================================================
void CManager::SetScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
//スコアの設定
//=============================================================================
void CManager::SetFinalScore(int nScore)
{
	m_nFinalScore = nScore;
}


//=============================================================================
//コンボ表記の設定
//=============================================================================
void CManager::SetComboGaugeMax(float fValue)
{
	m_fComboGaugeMax = fValue;
}

void CManager::SetDamage(float fValue)
{
	m_fDamage = fValue;
}

void CManager::SetScoreBonus(int nScore)
{
	m_nScoreBonus = nScore;
}

void CManager::SetFinalScoreBonus(int nScore)
{
	m_nFinalScoreBonus = nScore;
}

void CManager::SetTimeBonus(int nScore)
{
	m_nTimerBonus = nScore;
}

void CManager::SetComboGaugeMaxBonus(int nScore)
{
	m_nComboGaugeMaxBonus = nScore;
}

void CManager::SetDamageBonus(int nScore)
{
	m_nDamageBonus = nScore;
}
