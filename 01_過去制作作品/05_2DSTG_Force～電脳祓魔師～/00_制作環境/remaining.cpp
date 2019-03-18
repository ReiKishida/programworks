//=============================================================================
//
// 残機の処理[2Dポリゴン] [remaining.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================5
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "number.h"
#include "remaining.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define REMAINING_WIDTH (50)	//ポリゴンの幅
#define REMAINING_HEIGHT (50)	//ポリゴンのYの高さ
#define REMAINING_NUM (2)
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		CRemaining::m_pTexture = NULL;
CScene2D *CRemaining::m_apRemaining[REMAINING_NUMBER] = {};
int CRemaining::m_nRemaining = REMAINING_NUM;
//=============================================================================
//コンストラクタ
//=============================================================================
CRemaining::CRemaining() :CScene(7)
{//クリアな値を代入
	m_nRemaining = REMAINING_NUM;
}
//=============================================================================
//デストラクタ
//=============================================================================
CRemaining::~CRemaining()
{


}



//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CRemaining::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CRemaining::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//=============================================================================
//スコアの生成
//=============================================================================
CRemaining *CRemaining::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CRemaining *pScore;
	pScore = new CRemaining;

	if (pScore != NULL)
	{
		//スコアの初期化処理
		pScore->Init(pos);
	}

	return pScore;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CRemaining::Init(D3DXVECTOR3 pos)
{
	for (int nCntRemaining = 0; nCntRemaining < REMAINING_NUMBER; nCntRemaining++)
	{
		//数字の生成
		m_apRemaining[nCntRemaining] = new CScene2D(6);
		if (m_apRemaining[nCntRemaining] != NULL)
		{


			//初期化処理
			m_apRemaining[nCntRemaining]->SetScene2DWidth(REMAINING_WIDTH);
			m_apRemaining[nCntRemaining]->SetScene2DHeight(REMAINING_HEIGHT);
			//テクスチャの割り当て
			m_apRemaining[nCntRemaining]->BindTexture(m_pTexture);

			m_apRemaining[nCntRemaining]->Init(D3DXVECTOR3(pos.x + (80.0f * nCntRemaining), pos.y, pos.z));
			m_apRemaining[nCntRemaining]->SetTexture(0.25f, 0.33f);

			m_nRemaining = REMAINING_NUM;

			//スコアのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_REMAINING);
		}
	}

	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CRemaining::Uninit(void)
{
	for (int nCntRemaining = 0; nCntRemaining < REMAINING_NUMBER; nCntRemaining++)
	{
		if (m_apRemaining[nCntRemaining] != NULL)
		{
			//2Dシーンの終了処理
			m_apRemaining[nCntRemaining]->Uninit();
			m_apRemaining[nCntRemaining] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CRemaining::Update(void)
{
	D3DXVECTOR3 pos;
	for (int nCntRemaining = 0; nCntRemaining < m_nRemaining; nCntRemaining++)
	{
		if (m_apRemaining[nCntRemaining] != NULL)
		{
			pos = m_apRemaining[nCntRemaining]->GetPosScene2D();

			m_apRemaining[nCntRemaining]->SetPosScene2D(pos);
		}
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CRemaining::Draw(void)
{
	for (int nCntRemaining = 0; nCntRemaining < m_nRemaining; nCntRemaining++)
	{
		if (m_apRemaining[nCntRemaining] != NULL)
		{
			m_apRemaining[nCntRemaining]->Draw();
		}
	}
}

//=============================================================================
//スコアの設定
//=============================================================================
void CRemaining::SetScore(int nScore)
{
	m_nRemaining = nScore;



}

//=============================================================================
//スコアの取得
//=============================================================================
int CRemaining::GetRemaining(void)
{
	return m_nRemaining;
}

//=============================================================================
//スコアの加算
//=============================================================================
void CRemaining::DecreseRemaining(int nValue)
{
	//スコアの加算
	m_nRemaining += nValue;
	if (m_nRemaining >= 0)
	{
		if (m_apRemaining[m_nRemaining] != NULL)
		{
			m_apRemaining[m_nRemaining]->Uninit();
		}
	}

}

