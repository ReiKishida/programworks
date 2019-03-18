//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "Texture.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "create.h"
#include "DebugProc.h"
#include "line.h"
#include "objCreate.h"

#include <time.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR (2024)
#define FAILE_TIME ("data/system.txt")
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;			// レンダラー
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// キーボード
CInputMouse *CManager::m_pInputMouse = NULL;		// マウス
CInputGamePad *CManager::m_pInputGamePad = NULL;	// ゲームパッド
CSound *CManager::m_pSound = NULL;					// 音
CTexture *CManager::m_pTexture = NULL;				// テクスチャ
CManager::MODE CManager::m_mode = MODE_TITLE;		// モード
CTitle *CManager::m_pTitle = NULL;					// タイトル
CGame *CManager::m_pGame = NULL;					// ゲーム
CResult *CManager::m_pResult = NULL;				// リザルト
CCreate *CManager::m_pCreate = NULL;				// クリエイト
CDebugProc *CManager::m_pDebugProc = NULL;			// デバッグ

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}
//=============================================================================
// ロード
//=============================================================================
void CManager::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(FAILE_TIME, "r");		// ファイルの名前の読み込み

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// 行の読み込み
			if (strcmp(cData, "SETGAME") == 0)
			{// OBJECTSETだった場合
				while (strcmp(cData, "END_SETGAME") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "MODE") == 0)
					{// モード
						sscanf(&cLine[0], "%s %s %d ", &cData[0], &cData[0], &m_mode);
					}

				}
			}
		}
		fclose(pFile);// 終了
	}
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	srand(unsigned int(time(0)));
	if (m_pRenderer == NULL)
	{// レンダリングの生成
		m_pRenderer = new CRenderer;
	}
	if (m_pRenderer != NULL)
	{// レンダリングが生成されている
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{// 初期化処理
			return -1;
		}
	}
	if (m_pInputKeyboard == NULL)
	{// キーボードの生成
		m_pInputKeyboard = new CInputKeyboard;
	}
	if (m_pInputKeyboard != NULL)
	{// キーボードが生成されている
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{// キーボードの初期化
			return -1;
		}
	}

	if (m_pInputMouse == NULL)
	{// マウスの生成
		m_pInputMouse = new CInputMouse;
	}
	if (m_pInputMouse != NULL)
	{// マウスが生成されている
		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{// ゲームパッドの初期化
			return -1;
		}
	}
	if (m_pInputGamePad == NULL)
	{// ゲームパッド生成
		m_pInputGamePad = new CInputGamePad;
	}
	if (m_pInputGamePad != NULL)
	{// ゲームパッドが生成されている
		if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
		{// ゲームパッドの初期化
			return -1;
		}
	}
	if (m_pSound == NULL)
	{// サウンドの生成
		m_pSound = new CSound;
	}
	if (m_pSound != NULL)
	{// サウンドの初期化
		if (FAILED(m_pSound->Init(hWnd)))
		{// サウンドの初期化
			return -1;
		}
	}
	if (m_pDebugProc == NULL)
	{// デバッグテキストの生成
		m_pDebugProc = new CDebugProc;
	}
	if (m_pDebugProc != NULL)
	{// デバッグテキストの初期化
		if (FAILED(m_pDebugProc->Init()))
		{// デバッグテキストの初期化
			return -1;
		}
	}
	if (m_pTexture == NULL)
	{// テクスチャの生成
		m_pTexture = new CTexture;
	}
	if (m_pTexture != NULL)
	{// ロード
		m_pTexture->Load();
	}

	CManager::SetMode(m_mode);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// 各種終了処理
	switch (m_mode)
	{
	case MODE_TITLE:	// タイトル
		if (m_pTitle != NULL)
		{// タイトルの破棄
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		if (m_pSound != NULL)
		{// サウンドの停止
			m_pSound->Stop();
		}
		break;

	case MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// ゲームの破棄
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		if (m_pSound != NULL)
		{// サウンドの停止
			m_pSound->Stop();
		}
		break;

	case MODE_RESULT:	// リザルト
		if (m_pResult != NULL)
		{// リザルトの破棄
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		if (m_pSound != NULL)
		{// サウンドの停止
			m_pSound->Stop();
		}
		break;
	}

	if (m_pDebugProc != NULL)
	{// デバッグテキストの破棄
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{// キーボードの破棄
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	if (m_pInputMouse != NULL)
	{// マウスの破棄
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}
	if (m_pInputGamePad != NULL)
	{// ゲームパッドの破棄
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}
	if (m_pSound != NULL)
	{// サウンドの破棄
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	if (m_pTexture != NULL)
	{// 破棄
		m_pTexture->UnLoad();
		delete m_pTexture;
		m_pTexture = NULL;
	}
	if (m_pRenderer != NULL)
	{// レンダリングクラスの破棄
		m_pRenderer->Uninit(); // 終了処理
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	static bool bWireFrame = false;
#ifdef _DEBUG
	int nCntFPS = GetCntFPS();
	CDebugProc::print("%d\n",nCntFPS);
#endif
	CDebugProc::print("---------------------------\n");
	CDebugProc::print("ワイヤーフレーム：F2\n");
	CDebugProc::print("判定表示/非表示：F3\n");
	CDebugProc::print("判定付けなおし：F9\n");
	CDebugProc::print("---------------------------\n");

	if (m_pSound != NULL)
	{// サウンドの更新
		m_pSound->Update();
	}
	if (m_pInputKeyboard != NULL)
	{// キーボードの更新
		m_pInputKeyboard->Update();
	}
	if (m_pInputMouse != NULL)
	{// マウスの更新
		m_pInputMouse->Update();
	}
	if (m_pInputGamePad != NULL)
	{// ゲームパッドの更新
		m_pInputGamePad->Update();
	}
	if (m_pRenderer != NULL)
	{// レンダリングクラスの更新
		m_pRenderer->Update();
		if (m_pInputKeyboard->GetTrigger(DIK_F2))
		{// ワイヤーフレーム
			bWireFrame = bWireFrame ? false : true;
			if (bWireFrame) { m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); }
			else { m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); }
		}
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3))
	{// ライン非表示
		CLine::SetDisp(CLine::GetDisp() ? false : true);// ライン
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
	{// データの更新
		CObjCreate::SetScan();		// ファイル読み込み[ オブジェクト ]
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
	{// モデルの判定データの更新
		CSceneX::ScanCollision();
	}
	// 各種終了処理
	switch (m_mode)
	{
	case MODE_TITLE:	// タイトル
		if (m_pTitle != NULL)
		{// タイトルの更新
			m_pTitle->Update();
		}
		break;

	case MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// ゲームの更新
			m_pGame->Update();
		}
		break;
	case MODE_CREATE:	// クリエイト
		if (m_pCreate != NULL)
		{// クリエイトの更新
			m_pCreate->Update();
		}
		break;
	case MODE_RESULT:	// リザルト
		if (m_pResult != NULL)
		{// リザルトの更新
			m_pResult->Update();
		}
		break;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// レンダリングクラスの描画
		m_pRenderer->Draw();
	}

}
//=============================================================================
// モード設定
//=============================================================================
HRESULT CManager::SetMode(CManager::MODE mode)
{
	// 各種終了処理
	switch (m_mode)
	{
	case MODE_TITLE:	// タイトル
		if (m_pTitle != NULL)
		{// タイトルの破棄
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		if (m_pSound != NULL)
		{// サウンドの停止
			m_pSound->Stop();
		}
		break;

	case MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// ゲームの破棄
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		if (m_pSound != NULL)
		{// サウンドの停止
			m_pSound->Stop();
		}
		break;
	case MODE_CREATE:	// クリエイト
		if (m_pCreate != NULL)
		{// クリエイトの破棄
			m_pCreate->Uninit();
			delete m_pCreate;
			m_pCreate = NULL;
		}
		break;

	case MODE_RESULT:	// リザルト
		if (m_pResult != NULL)
		{// リザルトの破棄
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		if (m_pSound != NULL)
		{// サウンドの停止
			m_pSound->Stop();
		}
		break;
	}

	m_mode = mode;

	// 各種初期化処理
	switch (mode)
	{
	case MODE_TITLE:	// タイトル
		if (m_pTitle == NULL)
		{// タイトルの生成
			m_pTitle = CTitle::Create();
		}
		if (m_pSound != NULL)
		{// サウンド開始
			m_pSound->PlaySound(0,CSound::LABEL_BGM_TITLE);
		}
		break;

	case MODE_GAME:		// ゲーム
		if (m_pGame == NULL)
		{// ゲームの生成
			m_pGame = CGame::Create();
		}
		if (m_pSound != NULL)
		{// サウンド開始
			m_pSound->PlaySound(0, CSound::LABEL_BGM_GAME);

		}
		break;
	case MODE_CREATE:	// クリエイト
		if (m_pCreate == NULL)
		{// クリエイトの更新
			m_pCreate=CCreate::Create();
		}
		break;
	case MODE_RESULT:	// リザルト
		if (m_pResult == NULL)
		{// リザルトの生成
			m_pResult = CResult::Create();
		}
		if (m_pSound != NULL)
		{// サウンド開始
			m_pSound->PlaySound(0, CSound::LABEL_BGM_RESULT);

		}
		break;
	}
	return S_OK;
}
//=============================================================================
// モード取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// 角度セーブ
//=============================================================================
void CManager::SaveAngle(float *pAngle)
{
	if (*pAngle > D3DX_PI)
	{// 差分が3.14を超えるなら
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		*pAngle += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// 回転量セーブ
//=============================================================================
void CManager::SaveRot(D3DXVECTOR3 *rot)
{
	if (rot->x > D3DX_PI)
	{// 差分が3.14を超えるなら
		rot->x -= D3DX_PI * 2.0f;
	}
	if (rot->x < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		rot->x += D3DX_PI * 2.0f;
	}
	if (rot->y > D3DX_PI)
	{// 差分が3.14を超えるなら
		rot->y -= D3DX_PI * 2.0f;
	}
	if (rot->y < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		rot->y += D3DX_PI * 2.0f;
	}
	if (rot->z > D3DX_PI)
	{// 差分が3.14を超えるなら
		rot->z -= D3DX_PI * 2.0f;
	}
	if (rot->z < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		rot->z += D3DX_PI * 2.0f;
	}
}
//=============================================================================
// int型の累乗計算
//=============================================================================
int CManager::PowInt(const int nNum, const int nCnt)
{
	int nAnswer = 1;

	for (int nCntPow = 0; nCntPow < nCnt; nCntPow++)
	{
		nAnswer *= nNum;
	}

	return nAnswer;
}

//=============================================================================
// ランダムな角度取得
//=============================================================================
float CManager::GetRandomPI(void)
{
	float fRot = 0;

	fRot = (rand() % 628 -314)/100.0f;

	return fRot;
}
//=============================================================================
// ランダム数取得[float]
//=============================================================================
float CManager::GetRandBetWeen(const float fMin, const float fMax)
{
	float fRange = 0;
	fRange = rand() % (int(fMax * 100) - int(fMin * 100) + 1) + (fMin * 100);
	fRange = fRange / 100.0f;
	return fRange;
}
//=============================================================================
// ランダム数取得[int]
//=============================================================================
int CManager::GetIntRandBetWeen(const int nMin, const int nMax)
{
	int nRange = 0;
	nRange = rand() % (nMax - nMin + 1) + nMin;
	return nRange;
}
//=============================================================================
// 最大値最小値を取る
//=============================================================================
void CManager::SaveMinMax(float *pSave, const float Min, const float Max)
{
	if (*pSave <= Min)
	{// 最小
		*pSave = Min;
	}
	if (*pSave >= Max)
	{// 最大
		*pSave = Max;
	}

 }

//=============================================================================
// カメラモード取得
//=============================================================================
CCamera *CManager::GetCameraMode(void)
{
	CCamera *pCamera = NULL;
	// 各種終了処理
	switch (m_mode)
	{
	case MODE_TITLE:	// タイトル
		pCamera = CTitle::GetCamera();

		break;
	case MODE_GAME:		// ゲーム
		pCamera = CGame::GetCamera();
		break;
	case MODE_CREATE:		// クリエイト
		pCamera = CCreate::GetCamera();
		break;
	case MODE_RESULT:	// リザルト
		pCamera = CResult::GetCamera();
		break;
	}
	return pCamera;
}
//=============================================================================
// レンダリングクラスの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// キーボードの取得
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//=============================================================================
// ゲームパッドの取得
//=============================================================================
CInputGamePad *CManager::GetInputGamePad(void)
{
	return m_pInputGamePad;
}
//=============================================================================
// マウスの取得
//=============================================================================
CInputMouse *CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}
//=============================================================================
// サウンドの取得
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// テクスチャの取得
//=============================================================================
CTexture *CManager::GetTexture(void)
{
	return m_pTexture;
}

