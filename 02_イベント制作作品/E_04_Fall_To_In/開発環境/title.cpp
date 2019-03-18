//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "scene2D.h"
#include "light.h"
#include "camera.h"
#include "meshField.h"
#include "sound.h"
#include "sky.h"
#include "player.h"
#include "InfoUI.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define NEXTCNT (50)
//=============================================================================
// 静的メンバ変数
//=============================================================================
CLight		*CTitle::m_pLight = NULL;
CCamera		*CTitle::m_pCamera = NULL;
CPlayer		*CTitle::m_pPlayer = NULL;		// プレイヤー
CPlayer		*CTitle::m_pPlayer2 = NULL;	// プレイヤー

int			CTitle::m_nCntNext = 0;
bool        CTitle::m_bPush = false;
bool		CTitle::m_bSkip = false;
int			CTitle::m_nCntAppear = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_pPlayer = NULL;		// プレイヤー
	m_pPlayer2 = NULL;	// プレイヤー
}
//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
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

	if (m_pPlayer == NULL)
	{// プレイヤーの生成
		m_pPlayer = CPlayer::Create(0);

		m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

		m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

		// モーションタイプ設定
		m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_LOOP);
	}
	if (m_pPlayer2 == NULL)
	{// プレイヤーの生成
		m_pPlayer2 = CPlayer::Create(1);

		m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

		m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
		// モーションタイプ設定
		m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_LOOP);
	}

	CSky::Create(D3DXVECTOR3(450, -300, 0), D3DXVECTOR3(0, 0, 0), 400, 400,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_MOON, 0);


	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(0, 0, 0), 20000, 20000,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_UTHU, 1);

	CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_TITLE_LOGO);
	m_nCntNext = 0;
	m_nCntAppear = 0;
	m_bPush = false;
	m_bSkip = false;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
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
void CTitle::Update(void)
{
	if (m_pCamera != NULL)
	{// カメラの更新
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ライトの更新
		m_pLight->Update();
	}

	m_nCntAppear++;	// カウンターの加算

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_START)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_A)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_B)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_X)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_Y))
	{// 何か押されたら
		m_bPush = true;	// スイッチの切り替え
		if (m_bSkip == true)
		{
		//	CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_GAMESTART);
			m_bPush = true;	// スイッチの切り替え
		}
		else if (m_bSkip == false)
		{
			m_bSkip = true;
			if (m_pPlayer != NULL)
			{// プレイヤーの生成
			 // モーションタイプ設定
				m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_JUMP);
				m_pPlayer->SetMove(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
			}
			if (m_pPlayer2 != NULL)
			{// プレイヤーの生成
			 // モーションタイプ設定
				m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_JUMP);
				m_pPlayer2->SetMove(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
			}
		}
	}
	if (m_bPush == true)
	{// スイッチがオンの時
		m_nCntNext++;		// カウンター加算
		if (m_nCntNext >= NEXTCNT)
		{// 任意の数字で更新
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
			//	CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_START);
				CFade::SetFade(CManager::MODE_GAME);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
}
//=============================================================================
// 生成
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pCTitle = NULL;
	if (pCTitle == NULL)
	{// タイトル生成
		pCTitle = new CTitle;
	}
	if (pCTitle != NULL)
	{
		if (FAILED(pCTitle->Init()))
		{// 初期化処理
			return NULL;
		}
	}
	return pCTitle;
}
//=======
//
//=======
void CTitle::SetSkip(bool skip)
{
	m_bSkip = skip;
}