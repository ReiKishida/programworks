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
#include "ui.h"
#include "timer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_WIDTH (80.0f)				//スコアポリゴンの幅
#define SCORE_HEIGHT (40.0f)				//スコアポリゴンのYの高さ


#define TIME_WIDTH (70.0f)			//タイムポリゴンの幅
#define TIME_HEIGHT (30.0f)			//タイムポリゴンのYの高さ


#define COLON_WIDTH (11.5f)			//コロンポリゴンの幅
#define COLON_HEIGHT (35.0f)			//コロンポリゴンのYの高さ

#define DOT_WIDTH (9.0f)			//操作ポリゴンの幅
#define DOT_HEIGHT (9.0f)			//操作ポリゴンのYの高さ

#define CONTROLL_WIDTH (350.0f)				//操作ポリゴンの幅
#define CONTROLL_HEIGHT (200.0f)				//操作ポリゴンのYの高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUi::m_pTexture[MAX_TEXTURE_UI] = {}; //共有テクスチャのポインタ
//=============================================================================
//コンストラクタ
//=============================================================================
CUi::CUi(int nPriority, OBJTYPE type) : CScene2D(nPriority, type)
{//クリアな値を代入

}
//=============================================================================
//デストラクタ
//=============================================================================
CUi::~CUi()
{


}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CUi::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/score.png", &m_pTexture[0]);		//スコア
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/time.png", &m_pTexture[1]);			//タイムの文字
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/colon.png", &m_pTexture[2]);		//コロン
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/dot.png", &m_pTexture[3]);			//ドット
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/mastertuto.png", &m_pTexture[4]);		//ドット
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/tuto_key.png", &m_pTexture[5]);			//ドット
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/gaugege.png", &m_pTexture[6]);			//ドット
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/endtuto.png", &m_pTexture[7]);			//ドット


	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CUi::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_UI; nCntTex++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
//背景の生成
//=============================================================================
CUi *CUi::Create(CUi::UIMODE mode)
{
	//背景の生成
	CUi *pUi;
	pUi = new CUi;

	if (pUi != NULL)
	{
		pUi->m_mode = mode;

		//背景の初期化処理
		pUi->Init();
	}

	return pUi;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CUi::Init(void)
{
	//色情報設定
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCntColor = 0;

	switch (m_mode)
	{
	case CUi::UIMODE_TUTORIAL:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI_TUTORIAL; nCntScene2D++)
		{
			switch (nCntScene2D)
			{
			case UITYPE_SCORE:// タイム
							  //2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(880.0f, 30.0f, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_SCORE]);
				break;
			case UITYPE_TIME:// タイム
							 //2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(980.0f, 120.0f, 0.0f), TIME_WIDTH, TIME_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_TIME]);
				break;
			case UITYPE_COLON:// コロン
							  //2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1125.0f, 125.0f, 0.0f), COLON_WIDTH, COLON_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_COLON]);
				break;

			case UITYPE_DOT://ドット
							//2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1205.0f, 140.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_DOT]);
				break;

			case UITYPE_CONTROLL_ARCADE://操作
				//2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(210.0f, 600.0f, 0.0f), CONTROLL_WIDTH, CONTROLL_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_CONTROLL_ARCADE]);
				break;
			case UITYPE_CONTROLL_KEYBOARD://操作
										//2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(210.0f, 350.0f, 0.0f), CONTROLL_WIDTH, CONTROLL_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_CONTROLL_KEYBOARD]);
				break;
			case UITYPE_SKILL://操作
				//2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(300.0f, 140.0f, 0.0f), 450.0f, 60.0f);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_SKILL]);
				break;
			case UITYPE_END://操作
				//2Dポリゴンの生成
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1150.0f, 650.0f, 0.0f), 150.0f, 100.0f);
				//テクスチャの割り当て
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_END]);
				break;
			}
			m_pScene2D_Tuto[nCntScene2D]->SetObjType(CScene::OBJTYPE_UI);
		}

		break;
	case CUi::UIMODE_GAME:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI; nCntScene2D++)
		{
			switch (nCntScene2D)
			{
			case UITYPE_SCORE:// タイム
							  //2Dポリゴンの生成
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(880.0f, 30.0f, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_SCORE]);
				break;
			case UITYPE_TIME:// タイム
							 //2Dポリゴンの生成
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(980.0f, 120.0f, 0.0f), TIME_WIDTH, TIME_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_TIME]);
				break;
			case UITYPE_COLON:// コロン
							  //2Dポリゴンの生成
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1125.0f, 125.0f, 0.0f), COLON_WIDTH, COLON_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_COLON]);
				break;

			case UITYPE_DOT://ドット
							//2Dポリゴンの生成
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1205.0f, 140.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
				//テクスチャの割り当て
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_DOT]);
				break;

			}
			m_pScene2D[nCntScene2D]->SetObjType(CScene::OBJTYPE_UI);
		}

		break;
	}


	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CUi::Uninit(void)
{
	switch (m_mode)
	{
	case CUi::UIMODE_TUTORIAL:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI_TUTORIAL; nCntScene2D++)
		{
			if (m_pScene2D_Tuto[nCntScene2D] != NULL)
			{//シーン2Dが空じゃないとき
			 //2Dシーンの終了処理
				m_pScene2D_Tuto[nCntScene2D]->Uninit();
				m_pScene2D_Tuto[nCntScene2D] = NULL;
			}
		}
		break;
	case CUi::UIMODE_GAME:

		for (int nCntScene2D = 0; nCntScene2D < MAX_UI; nCntScene2D++)
		{
			if (m_pScene2D[nCntScene2D] != NULL)
			{//シーン2Dが空じゃないとき
			 //2Dシーンの終了処理
				m_pScene2D[nCntScene2D]->Uninit();
				m_pScene2D[nCntScene2D] = NULL;
			}
		}
		break;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CUi::Update(void)
{
	m_nCntColor++;
	m_nCntColor = m_nCntColor % 140;
	if (m_nCntColor < 70)
	{
		m_Color.a -= 0.01f;
	}
	if (m_nCntColor > 70)
	{
		m_Color.a += 0.01f;
	}
	switch (m_mode)
	{
	case CUi::UIMODE_TUTORIAL:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI_TUTORIAL; nCntScene2D++)
		{
			if (m_pScene2D_Tuto[nCntScene2D] != NULL)
			{//シーン2Dが空じゃないとき
				D3DXVECTOR3 posBg = m_pScene2D_Tuto[nCntScene2D]->GetPosPolygon();	//プレイヤーの位置情報




				//色情報の設定
				m_pScene2D_Tuto[UITYPE_END]->SetColor(m_Color);
				m_pScene2D_Tuto[UITYPE_SKILL]->SetColor(m_Color);

				//位置情報の設定
				m_pScene2D_Tuto[nCntScene2D]->SetPosPolygon(posBg);
			}
		}
		break;
	case CUi::UIMODE_GAME:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI; nCntScene2D++)
		{
			if (m_pScene2D[nCntScene2D] != NULL)
			{//シーン2Dが空じゃないとき
				D3DXVECTOR3 posBg = m_pScene2D[nCntScene2D]->GetPosPolygon();	//プレイヤーの位置情報

				//位置情報の設定
				m_pScene2D[nCntScene2D]->SetPosPolygon(posBg);
			}
		}
		break;
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CUi::Draw(void)
{

}

