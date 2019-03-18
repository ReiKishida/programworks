//=============================================================================
//
// クリエイトの処理 [create.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数
#include "create.h"
#include "manager.h"
#include "scene.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "DebugProc.h"
#include "meshField.h"
#include "light.h"
#include "objCreate.h"
#include "object.h"
#include "line.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CLight		*CCreate::m_pLight=NULL;
CCamera		*CCreate::m_pCamera = NULL;
CScene3D	*CCreate::m_pScene3D = NULL;
CMeshField	*CCreate::m_pMeshField = NULL;
CObjCreate  *CCreate::m_pObjCreate = NULL;
CFieldCreate  *CCreate::m_pFieldCreate = NULL;

CPlayer     *CCreate::m_pPlayer = NULL;
CLine         *CCreate::m_pLine = NULL;
CCreate::CREATEMODE  CCreate::m_mode = CREATEMODE_OBJECT;

bool CCreate::m_bPause = false;					// ポーズしてるかどうか
//=============================================================================
// コンストラクタ
//=============================================================================
CCreate::CCreate()
{
	m_bPause = false;// ポーズしてるかどうか
}
//=============================================================================
// デストラクタ
//=============================================================================
CCreate::~CCreate()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCreate::Init(void)
{
	m_bPause = false;// ポーズしてるかどうか

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

	CObjCreate::SetScan();

	m_pObjCreate = CObjCreate::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0);
	m_pFieldCreate = CFieldCreate::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0);
	m_pLine = CLine::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCreate::Uninit(void)
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
void CCreate::Update(void)
{
	if (m_pLight != NULL)
	{// ライトの更新
		m_pLight->Update();
	}
	if (m_pCamera != NULL)
	{// カメラの更新
		m_pCamera->Update();
	}

	// 作成するモードの切り替え
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F10))
	{
		m_mode = m_mode ? CREATEMODE_OBJECT : CREATEMODE_FIELD;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F5))
	{// クリエイトモードへ
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			CFade::SetFade(CManager::MODE_GAME);
		}
	}
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();			// マウス
	if (pInputKeyboard->GetTrigger(DIK_P))
	{// ポーズの処理
		SetPause(m_bPause ? false : true);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCreate::Draw(void)
{
}
//=============================================================================
// 生成
//=============================================================================
CCreate *CCreate::Create(void)
{
	CCreate *pCCreate = NULL;
	if (!CScene::OverData())
	{// ゲーム生成
		pCCreate = new CCreate;
	}
	if (pCCreate != NULL)
	{
		if (FAILED(pCCreate->Init()))
		{// 初期化処理
			return NULL;
		}

	}
	return pCCreate;
}

//=============================================================================
// ポーズの取得
//=============================================================================
bool CCreate::GetPause(void)
{
	return m_bPause;
}
//=============================================================================
// ポーズの設定
//=============================================================================
void CCreate::SetPause(bool bPause)
{
	m_bPause = bPause;
}