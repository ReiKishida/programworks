//=============================================================================
//
// UIの処理[2Dポリゴン] [ui.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================5
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "combo.h"
#include "timer.h"
#include "combogauge.h"
#include "game.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COMBO_WIDTH (120.0f)				//コンボポリゴンの幅
#define COMBO_HEIGHT (120.0f)			//コンボポリゴンのYの高さ

//各種ランクの上限下限
//S
#define COMBO_S_MAX (600.0f)
#define COMBO_S_MIN (501.0f)

//A
#define COMBO_A_MAX (500.0f)
#define COMBO_A_MIN (401.0f)

//B
#define COMBO_B_MAX (400.0f)
#define COMBO_B_MIN (301.0f)

//C
#define COMBO_C_MAX (300.0f)
#define COMBO_C_MIN (201.0f)

//D
#define COMBO_D_MAX (200.0f)
#define COMBO_D_MIN (101.0f)
//NONE
#define COMBO_NONE_MAX (100.0f)
#define COMBO_NONE_MIN (0.0f)


//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CCombo::m_apTexture[MAX_TEXTURE_COMBO] = {}; //共有テクスチャのポインタ
//=============================================================================
//コンストラクタ
//=============================================================================
CCombo::CCombo(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{//クリアな値を代入

}
//=============================================================================
//デストラクタ
//=============================================================================
CCombo::~CCombo()
{


}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CCombo::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/0_S.png", &m_apTexture[0]);	//S
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/1_A.png", &m_apTexture[1]);	//A
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/2_B.png", &m_apTexture[2]);	//B
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/3_C.png", &m_apTexture[3]);	//C
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/4_D.png", &m_apTexture[4]);	//D
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/5_N.png", &m_apTexture[5]);	//NONE


	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CCombo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_COMBO; nCntTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
//背景の生成
//=============================================================================
CCombo *CCombo::Create(void)
{
	//背景の生成
	CCombo *pCombo;
	pCombo = new CCombo;

	if (pCombo != NULL)
	{

		//背景の初期化処理
		pCombo->Init();
	}

	return pCombo;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CCombo::Init(void)
{
	//色情報設定
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCntColor = 0;



		//2Dポリゴンの生成
		m_pScene2D = CScene2D::Create(D3DXVECTOR3(1150.0f, SCREEN_HEIGHT / 2 - 90.0f, 0.0f), COMBO_WIDTH, COMBO_HEIGHT);
		//テクスチャの割り当て
		m_pScene2D->BindTexture(m_apTexture[COMBOTYPE_NONE]);

		m_pScene2D->SetObjType(CScene::OBJTYPE_COMBO);





	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CCombo::Uninit(void)
{
	if (m_pScene2D != NULL)
	{//シーン2Dが空じゃないとき
	 //2Dシーンの終了処理
		m_pScene2D->Uninit();
		//delete m_pScene2D;
		m_pScene2D = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CCombo::Update(void)
{
	//コンボゲージポインタの取得
	CComboGauge *pComboGauge = NULL;
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	switch (mode)
	{
	case CManager::MODE_GAME:
		pComboGauge = CGame::GetComboGauge(1);
		break;

	case CManager::MODE_TUTORIAL:
		pComboGauge = CTutorial::GetComboGauge(1);
		break;
	}


	//現在の数値の取得
	float fCurrentGauge;
	fCurrentGauge = pComboGauge->GetCurrentGauge();

	if (m_pScene2D != NULL)
	{//シーン2Dが空じゃないとき
		D3DXVECTOR3 posCombo = m_pScene2D->GetPosPolygon();	//プレイヤーの位置情報

		float fComboGauge;
		fComboGauge = CComboGauge::GetComboGauge();

		if ((fComboGauge >= COMBO_S_MIN && fComboGauge <= COMBO_S_MAX) && (fCurrentGauge >= 0.0f && fCurrentGauge <= 5.0f))
		{
			m_pScene2D->BindTexture(m_apTexture[COMBOTYPE_S]);
		}
		else if ((fComboGauge >= COMBO_A_MIN && fComboGauge <= COMBO_A_MAX) && (fCurrentGauge >= 0.0f && fCurrentGauge <= 5.0f))
		{
			m_pScene2D->BindTexture(m_apTexture[COMBOTYPE_A]);
		}
		else if ((fComboGauge >= COMBO_B_MIN && fComboGauge <= COMBO_B_MAX) && (fCurrentGauge >= 0.0f && fCurrentGauge <= 5.0f))
		{
			m_pScene2D->BindTexture(m_apTexture[COMBOTYPE_B]);
		}
		else if ((fComboGauge >= COMBO_C_MIN && fComboGauge <= COMBO_C_MAX) && (fCurrentGauge >= 0.0f && fCurrentGauge <= 5.0f))
		{
			m_pScene2D->BindTexture(m_apTexture[COMBOTYPE_C]);
		}
		else if ((fComboGauge >= COMBO_D_MIN && fComboGauge <= COMBO_D_MAX) && (fCurrentGauge >= 0.0f && fCurrentGauge <= 5.0f))
		{
			m_pScene2D->BindTexture(m_apTexture[COMBOTYPE_D]);
		}
		else if ((fComboGauge >= COMBO_NONE_MIN && fComboGauge <= COMBO_NONE_MAX) )
		{
			m_pScene2D->BindTexture(m_apTexture[COMBOTYPE_NONE]);
		}


		//位置情報の設定
		m_pScene2D->SetPosPolygon(posCombo);
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CCombo::Draw(void)
{

}

