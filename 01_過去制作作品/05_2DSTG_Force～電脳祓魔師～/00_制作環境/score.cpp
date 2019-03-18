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
#include "score.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
#define	SCORE_GAME_WIDTH (15.0f)
#define	SCORE_GAME_HEIGHT (25.0f)
#define	SCORE_GAME_INTERVAL (25.0f)

#define	SCORE_RESULT_WIDTH (70.0f)
#define	SCORE_RESULT_HEIGHT (90.0f)
#define	SCORE_RESULT_INTERVAL (90.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CScore::m_apNumber[SCORE_NUMBER] = {};
int CScore::m_nScore = 0;
//=============================================================================
//コンストラクタ
//=============================================================================
CScore::CScore() :CScene(6)
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
		pScore->m_type = type;
		pScore->Init(pos);
	}

	return pScore;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
	{
		//数字の生成
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{
			switch (m_type)
			{
			case SCORETYPE_GAME:
				//初期化処理
				m_apNumber[nCntNumber]->SetNumberWidth(SCORE_GAME_WIDTH);
				m_apNumber[nCntNumber]->SetNumberHeight(SCORE_GAME_HEIGHT);
				m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x + (SCORE_GAME_INTERVAL * nCntNumber), pos.y, pos.z));
				break;

			case SCORETYPE_RESULT:
				//初期化処理
				m_apNumber[nCntNumber]->SetNumberWidth(SCORE_RESULT_WIDTH);
				m_apNumber[nCntNumber]->SetNumberHeight(SCORE_RESULT_HEIGHT);
				m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x + (SCORE_RESULT_INTERVAL * nCntNumber), pos.y, pos.z));
				break;
			}


			m_nScore = 0;

			//スコアのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_SCORE);
		}
	}


	//モードがリザルトのときスコアを
	if (m_type == SCORETYPE_RESULT)
	{
		SetScoreNum(CManager::GetScoreNum());
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
void CScore::Update(void)
{
	CManager::SetHighScore(m_nScore);
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

