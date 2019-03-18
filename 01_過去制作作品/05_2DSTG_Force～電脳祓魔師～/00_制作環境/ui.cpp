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
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "ui.h"
#include "timer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (250)	//ポリゴンの幅
#define BG_HEIGHT (720)	//ポリゴンのYの高さ
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
CUi::CUi(int nPriority) : CScene(7)
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/L_70.jpg", &m_pTexture[0]);// UI下地
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/R_70.jpg", &m_pTexture[1]);// UI下地
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/highscore.png", &m_pTexture[2]);//ハイスコア
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score.png", &m_pTexture[3]);//スコア
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/atk0.png", &m_pTexture[4]);//攻のフォース数
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spr0.png", &m_pTexture[5]);//拡のフォース数
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spd0.png", &m_pTexture[6]);//速のフォース数
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/def0.png", &m_pTexture[7]);//護のフォース数
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/time.png", &m_pTexture[8]);//タイムの文字
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/colon.png", &m_pTexture[9]);//コロン
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/dot.png", &m_pTexture[10]);//ドット
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/log.png", &m_pTexture[11]);//ログの文字



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
CUi *CUi::Create(D3DXVECTOR3 pos, UITYPE type)
{
	//背景の生成
	CUi *pUi;
	pUi = new CUi;

	if (pUi != NULL)
	{
		//種類設定
		pUi->m_Type = type;
		//背景の初期化処理
		pUi->Init(pos);
	}


	return pUi;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos)
{
		//色情報設定
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
		m_nCntColor = 0;
		//2Dポリゴンの生成
		m_pScene2D = new CScene2D(6);
		if (m_pScene2D != NULL)
		{
			switch (m_Type)
			{
			case UI_TYPE_BG_L:// UIの下地
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(BG_WIDTH);
				m_pScene2D->SetScene2DHeight(BG_HEIGHT);
				//初期化処理
				m_pScene2D->Init(pos);
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_BG_L]);
				break;
			case UI_TYPE_BG_R:// UIの下地
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(BG_WIDTH);
				m_pScene2D->SetScene2DHeight(BG_HEIGHT);
				//初期化処理
				m_pScene2D->Init(pos);
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_BG_R]);
				break;
			case UI_TYPE_HIGHSCORE://ハイスコア
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(90.0f);
				m_pScene2D->SetScene2DHeight(35.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(70.0f, 25.0f,0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_HIGHSCORE]);
				break;

			case UI_TYPE_SCORE://スコア
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(70.0f);
				m_pScene2D->SetScene2DHeight(35.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(60.0f, 140.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_SCORE]);
				break;

			case UI_TYPE_ATK://攻のボーナス数
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y - 30.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_ATK]);
				break;

			case UI_TYPE_SPR://拡のボーナス数
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y + 30.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_SPR]);
				break;

			case UI_TYPE_SPD://速のボーナス数
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y + 90.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_SPD]);
				break;

			case UI_TYPE_DEF://護のボーナス数
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y + 150.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_DEF]);
				break;

			case UI_TYPE_TIME://タイムの文字
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(30.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 480.0f, pos.y - 320.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_TIME]);
				break;

			case UI_TYPE_COLON://コロン
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(15.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 480.0f, pos.y - 240.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_COLON]);
				break;

			case UI_TYPE_DOT://ドット
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(10.0f);
				m_pScene2D->SetScene2DHeight(10.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 560.0f, pos.y - 220.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_DOT]);
				break;

			case UI_TYPE_LOG://ログの文字
				//背景の幅、高さ設定
				m_pScene2D->SetScene2DWidth(70.0f);
				m_pScene2D->SetScene2DHeight(30.0f);
				//初期化処理
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 520.0f, pos.y - 180.0f, 0.0f));
				//テクスチャの割り当て
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_LOG]);
				break;
			}


			//背景のオブジェクトの設定
			m_pScene2D->SetObjType(CScene::OBJTYPE_UI);
		}



	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CUi::Uninit(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_TEXTURE_UI; nCntScene2D++)
	{
		if (m_pScene2D != NULL)
		{//シーン2Dが空じゃないとき
			//2Dシーンの終了処理
			m_pScene2D->Uninit();
			m_pScene2D = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CUi::Update(void)
{

		if (m_pScene2D != NULL)
		{//シーン2Dが空じゃないとき
			D3DXVECTOR3 posBg = m_pScene2D->GetPosScene2D();	//プレイヤーの位置情報
			if (m_Type == UI_TYPE_DOT || m_Type == UI_TYPE_COLON)
			{//時間の区切り
				int nTime = CTimer::GetTimer();
				if (nTime >= 10000)
				{//1分以上
					m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				if (nTime <= 5999)
				{//1分以下
					m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
				}
				if (nTime <= 2999)
				{//残り30秒以下
					m_nCntColor++;
					m_nCntColor = m_nCntColor % 18;
					if (m_nCntColor < 9)
					{
						m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
					if (m_nCntColor >= 9)
					{
						m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}


			//位置情報の設定
			m_pScene2D->SetPosScene2D(posBg);

			//位置情報の設定
			m_pScene2D->SetColor(m_Color);

		}

}

//=============================================================================
//描画処理
//=============================================================================
void CUi::Draw(void)
{

}

