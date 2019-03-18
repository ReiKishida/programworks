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
#include "clearrank.h"
#include "timer.h"
#include "combogauge.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COMBO_WIDTH (70.0f)				//コンボポリゴンの幅
#define COMBO_HEIGHT (70.0f)			//コンボポリゴンのYの高さ

//各種ランクの上限下限
//============================================
// スコア
//============================================
//S
#define SCORE_S_MAX (999999)
#define SCORE_S_MIN (60001)
//A
#define SCORE_A_MAX (60000)
#define SCORE_A_MIN (45001)
//B
#define SCORE_B_MAX (45000)
#define SCORE_B_MIN (20001)
//C
#define SCORE_C_MAX (20000)
#define SCORE_C_MIN (5001)
//D
#define SCORE_D_MAX (5000)
#define SCORE_D_MIN (0)


//============================================
// タイム
//============================================
//S
#define TIME_S_MAX (0)
#define TIME_S_MIN (32900)
//A
#define TIME_A_MAX (33000)
#define TIME_A_MIN (42900)
//B
#define TIME_B_MAX (43000)
#define TIME_B_MIN (52900)
//C
#define TIME_C_MAX (53000)
#define TIME_C_MIN (62900)
//D
#define TIME_D_MAX (63000)
#define TIME_D_MIN (999999)

//============================================
// コンボ
//============================================
//S
#define COMBO_S_MAX (600)
#define COMBO_S_MIN (501)
//A
#define COMBO_A_MAX (500)
#define COMBO_A_MIN (401)
//B
#define COMBO_B_MAX (400)
#define COMBO_B_MIN (301)
//C
#define COMBO_C_MAX (300)
#define COMBO_C_MIN (201)
//D
#define COMBO_D_MAX (200)
#define COMBO_D_MIN (0)


//============================================
// ダメージ
//============================================
//S
#define DAMAGE_S_MAX (0)
#define DAMAGE_S_MIN (150)
//A
#define DAMAGE_A_MAX (151)
#define DAMAGE_A_MIN (250)
//B
#define DAMAGE_B_MAX (251)
#define DAMAGE_B_MIN (350)
//C
#define DAMAGE_C_MAX (351)
#define DAMAGE_C_MIN (400)
//D
#define DAMAGE_D_MAX (401)
#define DAMAGE_D_MIN (500)


//============================================
//最終スコア
//============================================
//S
#define FINALSCORE_S_MAX (999999)
#define FINALSCORE_S_MIN (55000)
//A
#define FINALSCORE_A_MAX (54999)
#define FINALSCORE_A_MIN (45000)
//B
#define FINALSCORE_B_MAX (44999)
#define FINALSCORE_B_MIN (35000)
//C
#define FINALSCORE_C_MAX (34999)
#define FINALSCORE_C_MIN (25000)
//D
#define FINALSCORE_D_MAX (24999)
#define FINALSCORE_D_MIN (0)


//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CClearRank::m_apTexture[MAX_TEXTURE_RANK] = {}; //共有テクスチャのポインタ
//=============================================================================
//コンストラクタ
//=============================================================================
CClearRank::CClearRank(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{//クリアな値を代入

}
//=============================================================================
//デストラクタ
//=============================================================================
CClearRank::~CClearRank()
{


}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CClearRank::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/0_S.png", &m_apTexture[0]);	//S
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/1_A.png", &m_apTexture[1]);	//A
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/2_B.png", &m_apTexture[2]);	//B
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/3_C.png", &m_apTexture[3]);	//C
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/combo/4_D.png", &m_apTexture[4]);	//D


	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CClearRank::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_RANK; nCntTex++)
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
//背景の生成
//=============================================================================
CClearRank *CClearRank::Create(D3DXVECTOR3 pos, RANKMODE mode)
{
	//背景の生成
	CClearRank *pRank;
	pRank = new CClearRank;

	if (pRank != NULL)
	{
		pRank->m_mode = mode;
		pRank->m_pos = pos;
		pRank->m_Type = RANKTYPE_D;
		//背景の初期化処理
		pRank->Init();
	}

	return pRank;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CClearRank::Init(void)
{
	//色情報設定
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCntColor = 0;

	//各種スコア
	int nScore = 0;
	int nTime = 0;
	int nComboGauge = 0;
	int nDamage = 0;

	//各種ボーナススコア
	int nScoreBonus = 0;
	int nTimeBonus = 0;
	int nComboGaugeBonus = 0;
	int nDamageBonus = 0;


	//2Dポリゴンの生成
	if (m_mode == RANKMODE_SCORE || m_mode == RANKMODE_TIME || m_mode == RANKMODE_COMBO || m_mode == RANKMODE_DAMAGE)
	{
		m_pScene2D = CScene2D::Create(m_pos, COMBO_WIDTH, COMBO_HEIGHT);
	}
	else
	{
		m_pScene2D = CScene2D::Create(m_pos, 140.0f, 140.0f);

	}

	switch (m_mode)
	{
	case RANKMODE_SCORE:
		nScore  = CManager::GetScore();
		if ((nScore >= SCORE_S_MIN && nScore <= SCORE_S_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_S]);
			nScoreBonus = 5000;
		}
		else if ((nScore >= SCORE_A_MIN && nScore <= SCORE_A_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_A]);
			nScoreBonus = 4000;

		}
		else if ((nScore >= SCORE_B_MIN && nScore <= SCORE_B_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_B]);
			nScoreBonus = 3000;

		}
		else if ((nScore >= SCORE_C_MIN && nScore <= SCORE_C_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_C]);
			nScoreBonus = 2000;

		}
		else if ((nScore >= SCORE_D_MIN && nScore <= SCORE_D_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_D]);
			nScoreBonus = 1000;
		}
		//スコアボーナスを代入
		CManager::SetScoreBonus(nScoreBonus);
		break;
	case RANKMODE_TIME:
		nTime = CManager::GetTimer();
		if ((nTime <= TIME_S_MIN && nTime >= TIME_S_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_S]);
			nTimeBonus = 5000;
		}
		else if ((nTime <= TIME_A_MIN && nTime >= TIME_A_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_A]);
			nTimeBonus = 4000;

		}
		else if ((nTime <= TIME_B_MIN && nTime >= TIME_B_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_B]);
			nTimeBonus = 3000;

		}
		else if ((nTime <= TIME_C_MIN && nTime >= TIME_C_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_C]);
			nTimeBonus = 2000;
		}
		else if ((nTime <= TIME_D_MIN && nTime >= TIME_D_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_D]);
			nTimeBonus = 1000;

		}

		//タイムボーナスの代入
		CManager::SetTimeBonus(nTimeBonus);

		break;
	case RANKMODE_COMBO:
		nComboGauge = (int)CManager::GetComboGaugeMax();
		if ((nComboGauge >= COMBO_S_MIN && nComboGauge <= COMBO_S_MAX))
		{
			if (m_apTexture[RANKTYPE_S] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_S]);
			}			nComboGauge = 5000;
		}
		else if ((nComboGauge >= COMBO_A_MIN && nComboGauge <= COMBO_A_MAX))
		{
			if (m_apTexture[RANKTYPE_A] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_A]);
			}
			nComboGauge = 4000;

		}
		else if ((nComboGauge >= COMBO_B_MIN && nComboGauge <= COMBO_B_MAX))
		{
			if (m_apTexture[RANKTYPE_B] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_B]);
			}
			nComboGauge = 3000;

		}
		else if ((nComboGauge >= COMBO_C_MIN && nComboGauge <= COMBO_C_MAX))
		{
			if (m_apTexture[RANKTYPE_C] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_C]);
			}
			nComboGauge = 2000;
		}
		else if ((nComboGauge >= COMBO_D_MIN && nComboGauge <= COMBO_D_MAX))
		{
			if (m_apTexture[RANKTYPE_D] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_D]);
			}
			nComboGauge = 1000;
		}
		//コンボボーナスの代入
		CManager::SetComboGaugeMaxBonus(nComboGauge);
		break;
	case RANKMODE_DAMAGE:
		nDamage = (int)CManager::GetDamage();
		if ((nDamage <= DAMAGE_S_MIN && nDamage >= DAMAGE_S_MAX))
		{
			if (m_apTexture[RANKTYPE_S] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_S]);
			}
			nDamageBonus = 5000;
		}
		else if ((nDamage <= DAMAGE_A_MIN && nDamage >= DAMAGE_A_MAX))
		{
			if (m_apTexture[RANKTYPE_A] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_A]);
			}
			nDamageBonus = 4000;

		}
		else if ((nDamage <= DAMAGE_B_MIN && nDamage >= DAMAGE_B_MAX))
		{
			if (m_apTexture[RANKTYPE_B] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_B]);
			}
			nDamageBonus = 3000;

		}
		else if ((nDamage <= DAMAGE_C_MIN && nDamage >= DAMAGE_C_MAX))
		{
			if (m_apTexture[RANKTYPE_C] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_C]);
			}
			nDamageBonus = 2000;
		}
		else if ((nDamage <= DAMAGE_D_MIN && nDamage >= DAMAGE_D_MAX))
		{
			if (m_apTexture[RANKTYPE_D] != NULL)
			{
				m_pScene2D->BindTexture(m_apTexture[RANKTYPE_D]);
			}
			nDamageBonus = 1000;

		}
		//コンボボーナスの代入
		CManager::SetDamageBonus(nDamageBonus);
		break;
	case RANKMODE_FINAL:
		m_nFinalScore = CManager::GetFinalScore();
		m_nFinalScore += nScoreBonus + nTimeBonus + nComboGauge + nDamageBonus;
		if ((m_nFinalScore >= FINALSCORE_S_MIN && m_nFinalScore <= FINALSCORE_S_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_S]);
		}
		else if ((m_nFinalScore >= FINALSCORE_A_MIN && m_nFinalScore <= FINALSCORE_A_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_A]);
		}
		else if ((m_nFinalScore >= FINALSCORE_B_MIN && m_nFinalScore <= FINALSCORE_B_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_B]);
		}
		else if ((m_nFinalScore >= FINALSCORE_C_MIN && m_nFinalScore <= FINALSCORE_C_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_C]);
		}
		else if ((m_nFinalScore >= FINALSCORE_D_MIN && m_nFinalScore <= FINALSCORE_D_MAX))
		{
			m_pScene2D->BindTexture(m_apTexture[RANKTYPE_D]);
		}
		break;
	}



	m_pScene2D->SetObjType(CScene::OBJTYPE_COMBO);





	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CClearRank::Uninit(void)
{
	if (m_pScene2D != NULL)
	{//シーン2Dが空じゃないとき
	 //2Dシーンの終了処理
		m_pScene2D->Uninit();
		//delete m_pScene2D;
		m_pScene2D = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CClearRank::Update(void)
{

		//位置情報の設定
		m_pScene2D->SetPosPolygon(m_pos);

}

//=============================================================================
//描画処理
//=============================================================================
void CClearRank::Draw(void)
{

}

