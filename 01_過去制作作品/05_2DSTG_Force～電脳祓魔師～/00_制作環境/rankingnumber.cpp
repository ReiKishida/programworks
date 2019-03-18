//=============================================================================
//
// ランキング数字の処理[2Dポリゴン] [rankingnumber.cpp]
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
#include "rankingnumber.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
#define	RANKING_WIDTH (50.0f)	//数字の幅
#define	RANKING_HEIGHT (70.0f)	//数字の高さ
#define	RANKING_INTERVAL_WIDTH (85.0f)	//幅間隔
#define	RANKING_INTERVAL_HEIGHT (120.0f)	//高さ間隔

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CRankingScore::m_apNumber[MAX_RANKING][SCORE_NUMBER] = {};
int CRankingScore::m_nRanking[MAX_RANKING] = {40000 ,30000, 20000, 10000, 5000};
int CRankingScore::m_nRankingPlayer = 0;
bool CRankingScore::m_bPlayer = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CRankingScore::CRankingScore() :CScene(6)
{//クリアな値を代入
}
//=============================================================================
//デストラクタ
//=============================================================================
CRankingScore::~CRankingScore()
{


}



//=============================================================================
//スコアの生成
//=============================================================================
CRankingScore *CRankingScore::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CRankingScore *pScore;
	pScore = new CRankingScore;

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
HRESULT CRankingScore::Init(D3DXVECTOR3 pos)
{
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
		{
			//数字の生成
			m_apNumber[nCntRank][nCntNumber] = new CNumber;
			if (m_apNumber[nCntRank][nCntNumber] != NULL)
			{

				//初期化処理
				m_apNumber[nCntRank][nCntNumber]->SetNumberWidth(RANKING_WIDTH);
				m_apNumber[nCntRank][nCntNumber]->SetNumberHeight(RANKING_HEIGHT);
				m_apNumber[nCntRank][nCntNumber]->Init(D3DXVECTOR3(pos.x + (RANKING_INTERVAL_WIDTH * nCntNumber), pos.y + (RANKING_INTERVAL_HEIGHT * nCntRank), pos.z));


				//スコアのオブジェクトの設定
				SetObjType(CScene::OBJTYPE_RANKINGNUNBER);
			}
		}
	}


	SetRankingNum(CManager::GetScoreNum());


	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CRankingScore::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
		{
			if (m_apNumber[nCntRank][nCntNumber] != NULL)
			{
				//2Dシーンの終了処理
				m_apNumber[nCntRank][nCntNumber]->Uninit();
				m_apNumber[nCntRank][nCntNumber] = NULL;
			}
		}
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CRankingScore::Update(void)
{
	SetRankingScore();

}

//=============================================================================
//描画処理
//=============================================================================
void CRankingScore::Draw(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
		{
			if (m_apNumber[nCntRank][nCntNumber] != NULL)
			{
				m_apNumber[nCntRank][nCntNumber]->Draw();
			}
		}
	}
}


//=============================================================================
//スコアの加算
//=============================================================================
void CRankingScore::SetRankingScore(void)
{

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{


		m_apNumber[nCntRank][0]->SetNumber(m_nRanking[nCntRank] / 100000000);
		m_apNumber[nCntRank][1]->SetNumber(m_nRanking[nCntRank] % 10000000 / 1000000);
		m_apNumber[nCntRank][2]->SetNumber(m_nRanking[nCntRank] % 1000000 / 100000);
		m_apNumber[nCntRank][3]->SetNumber(m_nRanking[nCntRank] % 100000 / 10000);
		m_apNumber[nCntRank][4]->SetNumber(m_nRanking[nCntRank] % 10000 / 1000);
		m_apNumber[nCntRank][5]->SetNumber(m_nRanking[nCntRank] % 1000 / 100);
		m_apNumber[nCntRank][6]->SetNumber(m_nRanking[nCntRank] % 100 / 10);
		m_apNumber[nCntRank][7]->SetNumber(m_nRanking[nCntRank] % 10);
	}
}

//=============================================================================
//ランキングの設定
//=============================================================================
void CRankingScore::SetRankingNum(int nValue)
{
	int nRank;
	int aEmptyData[6];
	aEmptyData[5] = nValue;

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{

		aEmptyData[nCntNum] = m_nRanking[nCntNum];

	}

	for (int nCntNum = 1; nCntNum < 6; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] <= aEmptyData[nCntNum1])
			{
				nRank = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nRank;
			}
		}
	}


	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		m_nRanking[nCntNum] = aEmptyData[nCntNum];

	}
	m_bPlayer = true;

	m_nRankingPlayer = nValue;
}

