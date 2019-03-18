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
#include "finalscore.h"
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
CNumber *CFinalScore::m_apNumber[SCORE_NUMBER] = {};
int CFinalScore::m_nScore = 0;
int CFinalScore::m_nDestScore = 0;
int CFinalScore::m_nNowScore = 0;

D3DXVECTOR3 CFinalScore::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//=============================================================================
//コンストラクタ
//=============================================================================
CFinalScore::CFinalScore(int nPriority, OBJTYPE type) :CScene(nPriority, type)
{//クリアな値を代入
	m_nScore = 0;
}
//=============================================================================
//デストラクタ
//=============================================================================
CFinalScore::~CFinalScore()
{


}



//=============================================================================
//スコアの生成
//=============================================================================
CFinalScore *CFinalScore::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CFinalScore *pScore;
	pScore = new CFinalScore;

	if (pScore != NULL)
	{
		//スコアの初期化処理
		m_pos = pos;
		pScore->m_nDestScore = 0;
		pScore->Init();
	}

	return pScore;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CFinalScore::Init()
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
	{


		//数字の生成
		m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (SCORE_RESULT_INTERVAL * nCntNumber), m_pos.y, m_pos.z), SCORE_RESULT_WIDTH, SCORE_RESULT_HEIGHT);

		m_nScore = 0;


	}

	//スコアのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_SCORE);
	int nScore = CManager::GetScore();
	int nScorebonus = CManager::GetScoreBonus();
	int nTimerbonus = CManager::GetTimeBonus();
	int nCombobonus = CManager::GetComboGaugeMaxBonus();
	int nDamageBonus = CManager::GetDamageBonus();

	int nFinalScore = nScore +  nScorebonus + nTimerbonus + nCombobonus + nDamageBonus;
	CManager::SetFinalScore(nFinalScore);
	SetScoreNum(nFinalScore);


	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CFinalScore::Uninit(void)
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
void CFinalScore::Update(void)
{
	//CManager::SetHighScore(m_nScore);
	AddScoreFix();
}

//描画処理
//=============================================================================
void CFinalScore::Draw(void)
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
void CFinalScore::SetScore(int nScore)
{
	m_nScore = nScore;



}

//=============================================================================
//スコアの取得
//=============================================================================
int CFinalScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
//スコアの加算
//=============================================================================
void CFinalScore::AddScore(int nValue)
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

void CFinalScore::AddScoreFix()
{


}

void CFinalScore::SetScoreNum(int nValue)
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

