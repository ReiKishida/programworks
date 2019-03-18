//=============================================================================
//
// リザルトの処理 [result.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "result.h"
#include "player.h"
#include "sky.h"
#include "InfoUI.h"

//*******************************************************1**********************
// 静的メンバ変数
//*****************************************************************************
CCamera		*CResult::m_pCamera = NULL;
CLight		*CResult::m_pLight = NULL;

CPlayer		*CResult::m_pPlayer = NULL;		// プレイヤー
CPlayer		*CResult::m_pPlayer2 = NULL;	// プレイヤー

int CResult::m_nWin = 2;

int CResult::n1PMotion = 0;
int CResult::n2PMotion = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
	m_pPlayer = NULL;		// プレイヤー
	 m_pPlayer2 = NULL;	// プレイヤー

}
//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	if (m_pCamera == NULL)
	{// カメラの生成
		m_pCamera = new CCamera;
	}
	if (m_pLight == NULL)
	{// ライトの生成
		m_pLight = new CLight;
	}
	if (m_pLight != NULL)
	{// ライトの初期化
		if (FAILED(m_pLight->Init()))
		{// ライトの初期化
			return -1;
		}
	}
	if (m_pCamera != NULL)
	{// カメラの初期化
		if (FAILED(m_pCamera->Init()))
		{// カメラの初期化
			return -1;
		}
	}


	CSky::Create(D3DXVECTOR3(450, -300, 0), D3DXVECTOR3(0, 0, 0), 400, 400,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_MOON, 0);


	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(0, 0, 0), 20000, 20000,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_UTHU, 1);


	if (m_nWin == 0)
	{// 1
		CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_WIN);
		if (m_pPlayer == NULL)
		{// プレイヤーの生成
			m_pPlayer = CPlayer::Create(0);

			m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

			m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

			// モーションタイプ設定
			m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_WIN);
		}
		if (m_pPlayer2 == NULL)
		{// プレイヤーの生成
			m_pPlayer2 = CPlayer::Create(1);

			m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

			m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
			// モーションタイプ設定
			m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
	}
	else if (m_nWin == 1)
	{// 2
		CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_LOOSE);
		if (m_pPlayer == NULL)
		{// プレイヤーの生成
			m_pPlayer = CPlayer::Create(0);

			m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

			m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

			// モーションタイプ設定
			m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
		if (m_pPlayer2 == NULL)
		{// プレイヤーの生成
			m_pPlayer2 = CPlayer::Create(1);

			m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

			m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
			// モーションタイプ設定
			m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_WIN);
		}
	}
	else if (m_nWin == 2)
	{// 引き分け
		CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_DRAW);

		if (m_pPlayer == NULL)
		{// プレイヤーの生成
			m_pPlayer = CPlayer::Create(0);

			m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

			m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

			// モーションタイプ設定
			m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
		if (m_pPlayer2 == NULL)
		{// プレイヤーの生成
			m_pPlayer2 = CPlayer::Create(1);

			m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

			m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
			// モーションタイプ設定
			m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
	}

	if (m_pPlayer != NULL)
	{// プレイヤーの生成
		m_pPlayer->SetMotion(n1PMotion);
	}
	if (m_pPlayer2 != NULL)
	{// プレイヤーの生成
		m_pPlayer2->SetMotion(n2PMotion);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	if (m_pCamera != NULL)
	{// カメラの破棄
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	if (m_pLight != NULL)
	{// ライトの破棄
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	// オブジェクトクラスの破棄
	CScene::ReleaseAll();

}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{

	if (m_pCamera != NULL)
	{// カメラの更新
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ライトの更新
		m_pLight->Update();
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_START)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_A)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_B)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_X)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_Y))
	{// 何か押されたら

		CFade::SetFade(CManager::MODE_TITLE);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
}
//=============================================================================
// 生成
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pCResult = NULL;
	if (pCResult == NULL)
	{// リザルト生成
		pCResult = new CResult;
	}
	if (pCResult != NULL)
	{
		if (FAILED(pCResult->Init()))
		{// 初期化処理
			return NULL;
		}

	}
	return pCResult;
}
