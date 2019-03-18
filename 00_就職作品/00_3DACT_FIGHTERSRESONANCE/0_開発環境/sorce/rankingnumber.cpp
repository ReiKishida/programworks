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
#include "number.h"
#include "rankingnumber.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
#define	RANKING_WIDTH (40.0f)	//数字の幅
#define	RANKING_HEIGHT (60.0f)	//数字の高さ
#define	RANKING_INTERVAL_WIDTH (85.0f)	//幅間隔
#define	RANKING_INTERVAL_HEIGHT (110.0f)	//高さ間隔

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CRankingScore::m_apNumber[MAX_RANKING][SCORE_NUMBER] = {};
int CRankingScore::m_nRanking[MAX_RANKING] = {60000 ,50000, 40000, 30000, 20000};
int CRankingScore::m_nRankingPlayer = 0;
int CRankingScore::m_nCntRanking = 0;
bool CRankingScore::m_bPlayer = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CRankingScore::CRankingScore() :CScene(7)
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
		pScore->m_pos = pos;
		//スコアの初期化処理
		pScore->Init();
	}

	return pScore;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CRankingScore::Init(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
		{
			//数字の生成
			m_apNumber[nCntRank][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (RANKING_INTERVAL_WIDTH * nCntNumber), m_pos.y + (RANKING_INTERVAL_HEIGHT * nCntRank), m_pos.z), RANKING_WIDTH, RANKING_HEIGHT);
		}
	}
	//スコアのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_RANKINGNUNBER);

	//ランキングの数字
	SetRankingNum(CManager::GetFinalScore());

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
				delete m_apNumber[nCntRank][nCntNumber];
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


	if (m_bPlayer == true)
	{
		m_nCntRanking++;
		m_nCntRanking = m_nCntRanking % 4;

		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
			{


				if (m_nRanking[nCntRanking] == m_nRankingPlayer)
				{
					if (m_nCntRanking < 2)
					{

						m_apNumber[nCntRanking][nCntNumber]->CNumber::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					}
					else if (m_nCntRanking >= 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->CNumber::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
					}
				}
				else
				{
					m_apNumber[nCntRanking][nCntNumber]->CNumber::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}

			}

		}
	}


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

