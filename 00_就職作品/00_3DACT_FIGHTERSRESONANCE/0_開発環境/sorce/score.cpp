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
#include "score.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
#define	SCORE_GAME_WIDTH (30.0f)
#define	SCORE_GAME_HEIGHT (40.0f)
#define	SCORE_GAME_INTERVAL (40.0f)

#define	SCORE_RESULT_WIDTH (50.0f)
#define	SCORE_RESULT_HEIGHT (70.0f)
#define	SCORE_RESULT_INTERVAL (70.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CScore::m_apNumber[SCORE_NUMBER] = {};
int CScore::m_nScore = 0;
int CScore::m_nDestScore = 0;
int CScore::m_nNowScore = 0;

D3DXVECTOR3 CScore::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//=============================================================================
//コンストラクタ
//=============================================================================
CScore::CScore(int nPriority, OBJTYPE type) :CScene(nPriority, type)
{//クリアな値を代入
	m_nScore = 0;
}
//=============================================================================
//デストラクタ
//=============================================================================
CScore::~CScore()
{


}



//=============================================================================
//スコアの生成
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, SCORETYPE type)
{
	//スコアの生成
	CScore *pScore;
	pScore = new CScore;

	if (pScore != NULL)
	{
		//スコアの初期化処理
		m_pos = pos;
		pScore->m_type = type;
		pScore->m_nDestScore = 0;
		pScore->Init();
	}

	return pScore;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CScore::Init()
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
	{

			switch (m_type)
			{
			case SCORETYPE_GAME:
				//数字の生成
				m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (SCORE_GAME_INTERVAL * nCntNumber), m_pos.y, m_pos.z), SCORE_GAME_WIDTH, SCORE_GAME_HEIGHT);
				break;

			case SCORETYPE_RESULT:
				//数字の生成
				m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (SCORE_GAME_INTERVAL * nCntNumber), m_pos.y, m_pos.z), SCORE_GAME_WIDTH, SCORE_GAME_WIDTH);
				break;

			case SCORETYPE_FINAL:
				//数字の生成
				m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (SCORE_RESULT_INTERVAL * nCntNumber), m_pos.y, m_pos.z), SCORE_RESULT_WIDTH, SCORE_RESULT_HEIGHT);
				break;
			}


			m_nScore = 0;

			//スコアのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_SCORE);

	}


	//モードがリザルトのときスコアを
	if (m_type == SCORETYPE_RESULT)
	{
		SetScoreNum(CManager::GetScore());
	}
	//モードがリザルトのときスコアを
	if (m_type == SCORETYPE_FINAL)
	{
		SetScoreNum(CManager::GetFinalScore());
	}
	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			//数字の終了処理
			m_apNumber[nCntNumber]->Uninit();
			delete m_apNumber[nCntNumber];
			m_apNumber[nCntNumber] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CScore::Update(void)
{
	//CManager::SetHighScore(m_nScore);
	AddScoreFix();
}

//描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
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
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;



}

//=============================================================================
//スコアの取得
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
//スコアの加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	//スコアの加算
	m_nScore += nValue;

	m_apNumber[0]->SetNumber(m_nScore / 100000000);
	m_apNumber[1]->SetNumber(m_nScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nScore % 10);

}

void CScore::AddScoreFix()
{


}

void CScore::SetScoreNum(int nValue)
{
	//スコアの加算
	m_nScore = nValue;

	m_apNumber[0]->SetNumber(m_nScore / 100000000);
	m_apNumber[1]->SetNumber(m_nScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nScore % 10);
}

