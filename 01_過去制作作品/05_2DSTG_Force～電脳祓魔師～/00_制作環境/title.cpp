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
#include "titlelogo.h"
#include "fade.h"
#include "pause.h"
#include "inputx.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CTitle *CTitle::m_pTitle = NULL;
CScene *CTitle::m_pScene = NULL;
CBg *CTitle::m_pBg = NULL;
CTitleLogo *CTitle::m_pTitleLogo = NULL;
CManager *CTitle::m_pManager = NULL;
int CTitle::m_nCntFlame = 0;

//=============================================================================
//コンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}




//=============================================================================
//初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	if (CRenderer::GetPause() == true)
	{
		CRenderer::SetPause();
		CPause::SetSelect();
	}

	//////////////////必要なテクスチャの読み込み///////////////////
	//背景のテクスチャの読み込み
	m_pBg->Load();
	m_pTitleLogo->Load();
	//////////////////必要な処理の生成///////////////////

	//背景の生成
	m_pBg->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pTitleLogo->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150.0f, 0.0f));
	return S_OK;

}
//=============================================================================
//終了処理
//=============================================================================
void CTitle::Uninit()
{

		//全シーンの破棄
		m_pScene->ReleaseSceneAll();

		//読み込んだテクスチャの破棄
		//背景のテクスチャの破棄
		m_pBg->Unload();
		m_pTitleLogo->Unload();


}
//=============================================================================
//更新処理
//=============================================================================
void CTitle::Update()
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();



	m_nCntFlame++;
	if (m_nCntFlame % 300 == 0)
	{
		if (pFade == CFade::FADE_NONE)
		{

			CFade::SetFade(CManager::MODE_RANKING);//ランキングへ移行
		}
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CTitle::Draw()
{


}
