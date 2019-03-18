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
#include "damagescore.h"
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

#define	SCORE_RESULT_WIDTH (60.0f)
#define	SCORE_RESULT_HEIGHT (80.0f)
#define	SCORE_RESULT_INTERVAL (80.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CDamageScore::m_apNumber[DAMAGE_SCORE_NUMBER] = {};
int CDamageScore::m_nScore = 0;
int CDamageScore::m_nDestScore = 0;
int CDamageScore::m_nNowScore = 0;

D3DXVECTOR3 CDamageScore::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//=============================================================================
//コンストラクタ
//=============================================================================
CDamageScore::CDamageScore(int nPriority, OBJTYPE type) :CScene(nPriority, type)
{//クリアな値を代入
	m_nScore = 0;
}
//=============================================================================
//デストラクタ
//=============================================================================
CDamageScore::~CDamageScore()
{


}



//=============================================================================
//スコアの生成
//=============================================================================
CDamageScore *CDamageScore::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CDamageScore *pScore;
	pScore = new CDamageScore;

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
HRESULT CDamageScore::Init()
{
	for (int nCntNumber = 0; nCntNumber < DAMAGE_SCORE_NUMBER; nCntNumber++)
	{



		//数字の生成
		m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (SCORE_GAME_INTERVAL * nCntNumber), m_pos.y, m_pos.z), SCORE_GAME_WIDTH, SCORE_GAME_WIDTH);
	}


	m_nScore = 0;

	//スコアのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_SCORE);



	int nEmpty;	//空のint型
	nEmpty = (int)CManager::GetDamage();

	SetScoreNum(nEmpty);


	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CDamageScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < DAMAGE_SCORE_NUMBER; nCntNumber++)
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
void CDamageScore::Update(void)
{

	AddScoreFix();
}

//描画処理
//=============================================================================
void CDamageScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < DAMAGE_SCORE_NUMBER; nCntNumber++)
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
void CDamageScore::SetScore(int nScore)
{
	m_nScore = nScore;

}

//=============================================================================
//スコアの取得
//=============================================================================
int CDamageScore::GetScore(void)
{
	return m_nScore;
}

void CDamageScore::AddScoreFix()
{


}

void CDamageScore::SetScoreNum(int nValue)
{
	//スコアの加算
	m_nScore = nValue;

	m_apNumber[0]->SetNumber(m_nScore / 10000);
	m_apNumber[1]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[2]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[3]->SetNumber(m_nScore % 10 / 1);
}


