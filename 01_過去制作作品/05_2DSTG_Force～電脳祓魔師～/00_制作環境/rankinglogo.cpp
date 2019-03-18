//=============================================================================
//
// ランキング用ポリゴン処理 [rankinglogo.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "number.h"
#include "force.h"
#include "polygon.h"
#include "input.h"
#include "effect.h"
#include "sound.h"
#include "rankinglogo.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define RANKINGLOGO_RADIUS (90.0f)
#define TEX_X (1.0f)
#define TEX_Y (1.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRankingLogo::m_apTexture[MAX_RANKING_LOGO_TEXTURE] = {}; //共有テクスチャのポインタ
CScene2D *CRankingLogo::m_apScene2D[MAX_RANKING_LOGO] = {};	//ポリゴンのポインタ


//=============================================================================
// コンストラクタ
//=============================================================================
CRankingLogo::CRankingLogo()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CRankingLogo::~CRankingLogo()
{

}

//=============================================================================
// ランキング用ポリゴンのテクスチャの読み込み
//=============================================================================
HRESULT CRankingLogo::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/1st.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/2nd.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/3rd.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/4th.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/5th.png", &m_apTexture[4]);

	return S_OK;
}

//=============================================================================
// ランキング用ポリゴンのテクスチャの破棄
//=============================================================================
void CRankingLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RANKING_LOGO_TEXTURE; nCntTex++)
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
// ランキング用ポリゴンの生成
//=============================================================================
CRankingLogo * CRankingLogo::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CRankingLogo *pRankingLogo;
	pRankingLogo = new CRankingLogo;

	if (pRankingLogo != NULL)
	{
		//スコアの初期化処理
		pRankingLogo->Init(pos);
	}

	return pRankingLogo;
}

//=============================================================================
// ランキング用ポリゴンの初期化
//=============================================================================
HRESULT CRankingLogo::Init(D3DXVECTOR3 pos)
{
	for (int nCntRankingLogo = 0; nCntRankingLogo < MAX_RANKING_LOGO; nCntRankingLogo++)
	{
		//数字の生成
		m_apScene2D[nCntRankingLogo] = new CScene2D(7);
		if (m_apScene2D[nCntRankingLogo] != NULL)
		{
			//初期化処理
			m_apScene2D[nCntRankingLogo]->SetScene2DWidth(RANKINGLOGO_RADIUS);
			m_apScene2D[nCntRankingLogo]->SetScene2DHeight(RANKINGLOGO_RADIUS);
			m_apScene2D[nCntRankingLogo]->Init(D3DXVECTOR3(pos.x , pos.y + (120.0f * nCntRankingLogo), pos.z));
			m_apScene2D[nCntRankingLogo]->BindTexture(m_apTexture[nCntRankingLogo]);

			//フォースのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_RANKINGLOOG);

		}
	}

	return S_OK;
}

//=============================================================================
// ランキング用ポリゴンの終了
//=============================================================================
void CRankingLogo::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_RANKING_LOGO; nCntNumber++)
	{
		if (m_apScene2D[nCntNumber] != NULL)
		{
			//2Dシーンの終了処理
			m_apScene2D[nCntNumber]->Uninit();
			m_apScene2D[nCntNumber] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// ランキング用ポリゴンの更新
//=============================================================================
void CRankingLogo::Update(void)
{
	//フォースの位置
	D3DXVECTOR3 posRankingLogo[MAX_RANKING_LOGO];


	for (int nCntForce = 0; nCntForce < MAX_RANKING_LOGO; nCntForce++)
	{
		posRankingLogo[nCntForce] = m_apScene2D[nCntForce]->GetPosScene2D();
	}
	for (int nCntForce = 0; nCntForce < MAX_RANKING_LOGO; nCntForce++)
	{
		m_apScene2D[nCntForce]->SetPosScene2D(posRankingLogo[nCntForce]);
	}
}

//=============================================================================
// ランキング用ポリゴンの描画
//=============================================================================
void CRankingLogo::Draw(void)
{
}

