//=============================================================================
//
// スコアの処理[2Dポリゴン] [score.cpp]
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
#include "number.h"
#include "highscore.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define HIGHSCORE (100)//ハイスコア
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CHighScore::m_apNumber[HIGHSCORE_NUMBER] = {};
int CHighScore::m_nHighScore = 0;
//=============================================================================
//コンストラクタ
//=============================================================================
CHighScore::CHighScore() :CScene(6)
{//クリアな値を代入
	m_nHighScore = 0;
}
//=============================================================================
//デストラクタ
//=============================================================================
CHighScore::~CHighScore()
{


}



//=============================================================================
//スコアの生成
//=============================================================================
CHighScore *CHighScore::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CHighScore *pScore;
	pScore = new CHighScore;

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
HRESULT CHighScore::Init(D3DXVECTOR3 pos)
{

	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
	{
		//数字の生成
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{


			//初期化処理
			m_apNumber[nCntNumber]->SetNumberWidth(15.0f);
			m_apNumber[nCntNumber]->SetNumberHeight(25.0f);
			m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x + (25.0f * nCntNumber), pos.y, pos.z));
			m_apNumber[nCntNumber]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


			//スコアのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_SCORE);
		}
	}
	SetHighScore(CManager::GetHighScoreNum());

	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CHighScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			//2Dシーンの終了処理
			m_apNumber[nCntNumber]->Uninit();
			m_apNumber[nCntNumber] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CHighScore::Update(void)
{
	SetScore(CManager::GetHighScoreNum());

	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CHighScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
//スコアの設定
//=============================================================================
void CHighScore::SetScore(int nScore)
{
	m_nHighScore = nScore;

}

//=============================================================================
//スコアの取得
//=============================================================================
int CHighScore::GetScore(void)
{
	return m_nHighScore;
}

//=============================================================================
//スコアの加算
//=============================================================================
void CHighScore::AddScore(int nValue)
{
	//スコアの加算
	m_nHighScore += nValue;

	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[1]->SetNumber(m_nHighScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nHighScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nHighScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nHighScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nHighScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nHighScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nHighScore % 10);
}

//=============================================================================
//スコアの設定
//=============================================================================
void CHighScore::SetHighScore(int nValue)
{
	//スコアの加算
	m_nHighScore = nValue;

	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[1]->SetNumber(m_nHighScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nHighScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nHighScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nHighScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nHighScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nHighScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nHighScore % 10);
}

