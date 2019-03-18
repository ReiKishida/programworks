//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// お知らせUIの処理 [infoUI.cpp]
// Author : Ryota Kinoshita
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "infoUI.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "title.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNTDOWN_SIZE (150)	// カウントダウンのサイズ
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CInfoUI::m_apTexture[TYPE_MAX] = {};			// テクスチャ
//=============================================================================
// コンストラクタ
//=============================================================================
CInfoUI::CInfoUI(int nPriority):CScene2D(nPriority)
{
	m_nCounter = 0;	// カウンター
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 目的の座標
}
//=============================================================================
// デストラクタ
//=============================================================================
CInfoUI::~CInfoUI()
{
}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CInfoUI::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	char *pFileName[TYPE_MAX] =
	{// ファイル名
		"data/TEXTURE/UI/title.png",
		"data/TEXTURE/countdown.png",
		"data/TEXTURE/UI/1pwin.png",
		"data/TEXTURE/UI/2pwin.png",
		"data/TEXTURE/UI/draw.png",

	};
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice,
			pFileName[nCnt],
			&m_apTexture[nCnt]);
	}


	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CInfoUI::UnLoad(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInfoUI::Init(void)
{
	if (FAILED(CScene2D::Init()))
	{
		return -1;
	}
	SetObjType(CScene::OBJTYPE_INFOUI);
	m_nCounter = 0;	// カウンター
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 目的の座標



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInfoUI::Uninit(void)
{
	CScene2D::Uninit();
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CInfoUI::Update(void)
{

	//int nTimeOld = m_nTime;
	//m_nTime = CGame::GetTimer()->GetTimer();

}
//=============================================================================
// 描画処理
//=============================================================================
void CInfoUI::Draw(void)
{
	CScene2D::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CInfoUI *CInfoUI::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 Destpos, const float fRot, float fWidth, const float fHeight,const TYPE type)
{
	CInfoUI *pInfoUI = NULL;
	if (!CScene::OverData())
	{// 生成
		switch (type)
		{
		case TYPE_TITLE_LOGO:		// タイトルロゴ
			pInfoUI = CTitleLogo::Create(CTitleLogo::LOGOTYPE_1);
			break;

		default:
			pInfoUI = new CInfoUI;
			break;
		}
	}
	if (pInfoUI != NULL)
	{
		if (FAILED(pInfoUI->Init()))
		{// 初期化処理
			return NULL;
		}
		pInfoUI->SetPos(pos+ Destpos);
		pInfoUI->SetDestPos(Destpos);
		pInfoUI->SetRot(fRot);
		pInfoUI->SetSize(fWidth, fHeight);
		pInfoUI->BindTexture(m_apTexture[type]);
		pInfoUI->SetVtx();
		pInfoUI->SetObjType(CScene::OBJTYPE_INFOUI);
	}
	return pInfoUI;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// タイトルロゴの処理 [infoUI.cpp]
// Author : Okabe Kazuki
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo() :CInfoUI(PRIORITY_TITLEROGO)
{
}
//=============================================================================
// デストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(void)
{
	if (FAILED(CInfoUI::Init()))
	{
		return -1;
	}

	SetObjType(CScene::OBJTYPE_INFOUI);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitleLogo::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CTitleLogo::Update(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{
	CInfoUI::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CTitleLogo *CTitleLogo::Create(const LOGOTYPE type)
{
	CTitleLogo *pTitleLogo = NULL;
	// クラスの生成
	pTitleLogo = new CTitleLogo;

	if (pTitleLogo == NULL)
	{// 生成できなかった場合
		return NULL;
	}

	// 初期化処理
	if (FAILED(pTitleLogo->Init()))
	{
		return NULL;
	}
	pTitleLogo->m_type = type;

	return pTitleLogo;
}

