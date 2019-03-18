//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数

#include "game.h"
#include "manager.h"
#include "scene.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "DebugProc.h"
#include "meshField.h"
#include "light.h"
#include "meshWall.h"
#include "sound.h"
#include "object.h"
#include "objCreate.h"
#include "player.h"
#include "sky.h"
#include "PanelHole.h"
#include "result.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define SPACE_4 (3000)
#define SPACE_3 (4500)
#define SPACE_2 (6000)
#define SPACE_1 (7500)
#define SPACE_0 (9000)

#define EARTH_5 (-6500)
#define EARTH_4 (-5000)
#define EARTH_3 (-3500)
#define EARTH_2 (-2000)
#define EARTH_1 (-500)
#define EARTH_0 (1000)


#define HELL_5 (-16700)
#define HELL_4 (-15000)
#define HELL_3 (-13500)
#define HELL_2 (-12000)
#define HELL_1 (-10500)
#define HELL_0 (-9000)

#define EARTH_Y (2000)
#define HELL_Y (-8000)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CLight		*CGame::m_pLight=NULL;
CCamera		*CGame::m_pCamera = NULL;
CPlayer		*CGame::CGame::m_pPlayer = NULL;	// プレイヤー
CGame::STATE CGame::m_State = STATE_START;		// ステータス
int CGame::m_WinId = 0;
bool CGame::m_bPause = false;					// ポーズしてるかどうか
int CGame::m_nStateCounter = 0;					// ステータスカウンター
CGame::STAGE CGame::m_Stage = CGame::STAGE_SPACE;
bool CGame::m_bStage[STAGE_MAX];
int CGame::m_nPanelCounter = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_pPlayer = NULL;

	m_State = STATE_START;// ステータス
	m_bPause = false;// ポーズしてるかどうか
	m_nStateCounter = 0;// ステータスカウンター
	m_Stage = STAGE_SPACE;
	m_nPanelCounter = 0;
	for (int nCnt = 0;nCnt<STAGE_MAX;nCnt++)
	{
		m_bStage[nCnt] = false;;
	}
}
//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{

	m_State = STATE_START;// ステータス
	m_bPause = false;// ポーズしてるかどうか
	m_nStateCounter = 0;// ステータスカウンター
	m_Stage = STAGE_SPACE;
	m_nPanelCounter = 0;
	for (int nCnt = 0; nCnt<STAGE_MAX; nCnt++)
	{
		m_bStage[nCnt] = false;;
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
	if (m_pCamera == NULL)
	{// カメラの生成
		m_pCamera = new CCamera;
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
		m_pPlayer = CPlayer::Create(1);
	}
	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(-D3DX_PI / 2, 0, 0), 5000, 5000,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_EARTH, 0);

	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(-D3DX_PI / 2, 0, 0), 20000, 20000,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_UTHU, 1);
	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(-D3DX_PI / 2, 0, 0), 5100, 5100,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_EARTH_1, 0);
	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(-D3DX_PI / 2, 0, 0), 5200, 5200,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_EARTH_2, 0);


	CObjCreate::SetScan();

	CPlayer::RandomPose();

	CPanel::SetPanel(D3DXVECTOR3(0.0f, SPACE_0, 0.0f), 5, CPanel::STAGE_SPACE);

	CResult::WinReset();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	if (m_pLight != NULL)
	{// ライトの破棄
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	if (m_pCamera != NULL)
	{// カメラの破棄
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	// オブジェクトクラスの破棄
	CScene::ReleaseAll();

}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	switch (m_State)
	{
	case STATE_NONE:

		break;
	case STATE_START:

		break;
	case STATE_END:
		m_nStateCounter++;
		if (m_nStateCounter >= 120)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;
	}
	float fYGame = CManager::GetCameraMode()->GetPosR(0).y;
	switch (m_Stage)
	{
	case STAGE_SPACE:
		if (CManager::GetIntRandBetWeen(0, 10) == 0)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(sinf(CManager::GetRandomPI()) * 500 , 2000, cosf(CManager::GetRandomPI()) * 500 );
			if (pos.x <= 300&& pos.x >= -300)
			{
				pos.x += pos.x * 2;
			}
			if (pos.z <= 300 && pos.z >= -300)
			{
				pos.z += (pos.z + 100) * 2;
			}
			CObject::Create(pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), CManager::GetIntRandBetWeen(1, 3), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		if (fYGame <= SPACE_0)
		{
			if (m_nPanelCounter == 0)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, SPACE_1, 0.0f), 5, CPanel::STAGE_SPACE);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= SPACE_1)
		{
			if (m_nPanelCounter == 1)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, SPACE_2, 0.0f), 5, CPanel::STAGE_SPACE);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= SPACE_2)
		{
			if (m_nPanelCounter == 2)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, SPACE_3, 0.0f), 5, CPanel::STAGE_SPACE);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= SPACE_3)
		{
			if (m_nPanelCounter == 3)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, SPACE_4, 0.0f), 5, CPanel::STAGE_SPACE);
				m_nPanelCounter=0;
				m_Stage = STAGE_EARTH;
				CMeshField::Create(D3DXVECTOR3(0.0f, -8000, 0.0f), 15000, 0.0f, 15000, 1, 1, 0, false);
			}
		}

		break;
	case STAGE_EARTH:

		if (fYGame <= EARTH_0)
		{
			if (m_nPanelCounter == 0)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, EARTH_1, 0.0f), 5, CPanel::STAGE_EARTH);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= EARTH_1)
		{
			if (m_nPanelCounter == 1)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, EARTH_2, 0.0f), 5, CPanel::STAGE_EARTH);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= EARTH_2)
		{
			if (m_nPanelCounter == 2)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, EARTH_3, 0.0f), 5, CPanel::STAGE_EARTH);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= EARTH_3)
		{
			if (m_nPanelCounter == 3)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, EARTH_4, 0.0f), 5, CPanel::STAGE_EARTH);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= EARTH_4)
		{
			if (m_nPanelCounter == 4)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, EARTH_5, 0.0f), 5, CPanel::STAGE_EARTH);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= -7800)
		{
			CFade::SetCloss();
			m_nPanelCounter = 0;
			m_Stage = STAGE_HELL;

		}
		break;
	case STAGE_HELL:
		if (fYGame <= HELL_0)
		{
			if (m_nPanelCounter == 0)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, HELL_1, -200.0f), 3, CPanel::STAGE_HELL);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= HELL_1)
		{
			if (m_nPanelCounter == 1)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, HELL_2, -200.0f), 3, CPanel::STAGE_HELL);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= HELL_2)
		{
			if (m_nPanelCounter == 2)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, HELL_3, -200.0f), 3, CPanel::STAGE_HELL);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= HELL_3)
		{
			if (m_nPanelCounter == 3)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, HELL_4, -200.0f), 2, CPanel::STAGE_HELL);
				m_nPanelCounter++;
			}
		}
		if (fYGame <= HELL_4)
		{
			if (m_nPanelCounter == 4)
			{
				CPanel::SetPanel(D3DXVECTOR3(0.0f, HELL_5, -200.0f), 1, CPanel::STAGE_HELL);
				m_nPanelCounter++;
			}
		}
		break;
	}


	if (m_pLight != NULL)
	{// ライトの更新
		m_pLight->Update();
	}
	if (m_pCamera != NULL)
	{// カメラの更新
		m_pCamera->Update();
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F5))
	{// クリエイトモードへ
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			CFade::SetFade(CManager::MODE_CREATE);
		}
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P))
	{
		m_bPause = m_bPause ? false : true;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
}
//=============================================================================
// 生成
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pCGame = NULL;
	if (!CScene::OverData())
	{// ゲーム生成
		pCGame = new CGame;
	}
	if (pCGame != NULL)
	{
		if (FAILED(pCGame->Init()))
		{// 初期化処理
			return NULL;
		}

	}
	return pCGame;
}

//=============================================================================
// ポーズの取得
//=============================================================================
bool CGame::GetPause(void)
{
	return m_bPause;
}
//=============================================================================
// ポーズの設定
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;
}
//=============================================================================
// ステータスの取得
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}
//=============================================================================
// ステータスの設定
//=============================================================================
void CGame::SetState(const STATE state)
{
	m_State = state;
}
