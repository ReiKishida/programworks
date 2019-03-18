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
#include "tutoriallogo.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ

#define RANKINGLOGO_WIDTH (780.0f)//チュートリアル表示の幅
#define RANKINGLOGO_HEIGHT (230.0f)//チュートリアル表示の高さ

#define END_RANKINGLOGO_WIDTH (100.0f)//チュートリアル表示の幅
#define END_RANKINGLOGO_HEIGHT (120.0f)//チュートリアル表示の高さ

#define TEX_X (1.0f)
#define TEX_Y (1.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTutorialLogo::m_apTexture[MAX_TUTORIAL_LOGO_TEXTURE] = {}; //共有テクスチャのポインタ
CScene2D *CTutorialLogo::m_apScene2D[MAX_TUTORIAL_LOGO] = {};	//ポリゴンのポインタ


//=============================================================================
// コンストラクタ
//=============================================================================
CTutorialLogo::CTutorialLogo()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorialLogo::~CTutorialLogo()
{

}

//=============================================================================
// ランキング用ポリゴンのテクスチャの読み込み
//=============================================================================
HRESULT CTutorialLogo::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/endtutorial.png", &m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// ランキング用ポリゴンのテクスチャの破棄
//=============================================================================
void CTutorialLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TUTORIAL_LOGO_TEXTURE; nCntTex++)
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
CTutorialLogo * CTutorialLogo::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CTutorialLogo *pRankingLogo;
	pRankingLogo = new CTutorialLogo;

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
HRESULT CTutorialLogo::Init(D3DXVECTOR3 pos)
{
	for (int nCntRankingLogo = 0; nCntRankingLogo < MAX_TUTORIAL_LOGO; nCntRankingLogo++)
	{
		//数字の生成
		m_apScene2D[nCntRankingLogo] = new CScene2D(7);
		if (m_apScene2D[nCntRankingLogo] != NULL)
		{
			switch (nCntRankingLogo)
			{
			case 0:
				//初期化処理
				m_apScene2D[nCntRankingLogo]->SetScene2DWidth(RANKINGLOGO_WIDTH);
				m_apScene2D[nCntRankingLogo]->SetScene2DHeight(RANKINGLOGO_HEIGHT);
				m_apScene2D[nCntRankingLogo]->Init(D3DXVECTOR3(pos.x, pos.y, pos.z));
				m_apScene2D[nCntRankingLogo]->BindTexture(m_apTexture[nCntRankingLogo]);
				break;

			case 1:
				//初期化処理
				m_apScene2D[nCntRankingLogo]->SetScene2DWidth(END_RANKINGLOGO_WIDTH);
				m_apScene2D[nCntRankingLogo]->SetScene2DHeight(END_RANKINGLOGO_HEIGHT);
				m_apScene2D[nCntRankingLogo]->Init(D3DXVECTOR3(SCREEN_WIDTH /2 + 300.0f, SCREEN_HEIGHT / 2 - 250.0f, pos.z));
				m_apScene2D[nCntRankingLogo]->BindTexture(m_apTexture[nCntRankingLogo]);
				break;
			}


			//フォースのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_TUTORIALLOGO);

		}
	}

	return S_OK;
}

//=============================================================================
// ランキング用ポリゴンの終了
//=============================================================================
void CTutorialLogo::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_TUTORIAL_LOGO; nCntNumber++)
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
void CTutorialLogo::Update(void)
{
	//フォースの位置
	D3DXVECTOR3 posRankingLogo[MAX_TUTORIAL_LOGO];


	for (int nCntForce = 0; nCntForce < MAX_TUTORIAL_LOGO; nCntForce++)
	{
		posRankingLogo[nCntForce] = m_apScene2D[nCntForce]->GetPosScene2D();
	}
	for (int nCntForce = 0; nCntForce < MAX_TUTORIAL_LOGO; nCntForce++)
	{
		m_apScene2D[nCntForce]->SetPosScene2D(posRankingLogo[nCntForce]);
	}
}

//=============================================================================
// ランキング用ポリゴンの描画
//=============================================================================
void CTutorialLogo::Draw(void)
{
}

