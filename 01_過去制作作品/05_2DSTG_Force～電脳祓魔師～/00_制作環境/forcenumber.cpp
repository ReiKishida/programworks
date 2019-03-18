//=============================================================================
//
// スコアの処理[2Dポリゴン] [score.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "number.h"
#include "forcenumber.h"
#include "force.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH (900)	//ポリゴンの幅
#define BG_HEIGHT (700)	//ポリゴンのYの高さ
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CNumber *CForceNumber::m_apNumber[FORCE_NUMBER] = {};
int CForceNumber::m_nForce = 0;
//=============================================================================
//コンストラクタ
//=============================================================================
CForceNumber::CForceNumber() :CScene(7)
{//クリアな値を代入
	m_nForce = 0;
}
//=============================================================================
//デストラクタ
//=============================================================================
CForceNumber::~CForceNumber()
{


}



//=============================================================================
//スコアの生成
//=============================================================================
CForceNumber *CForceNumber::Create(D3DXVECTOR3 pos)
{
	//スコアの生成
	CForceNumber *pFoece;
	pFoece = new CForceNumber;

	if (pFoece != NULL)
	{
		//スコアの初期化処理
		pFoece->Init(pos);
	}

	return pFoece;



}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CForceNumber::Init(D3DXVECTOR3 pos)
{
	for (int nCntNumber = 0; nCntNumber < FORCE_NUMBER; nCntNumber++)
	{
		//数字の生成
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{


			//初期化処理
			m_apNumber[nCntNumber]->SetNumberWidth(30.0f);
			m_apNumber[nCntNumber]->SetNumberHeight(35.0f);
			m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x, pos.y + (60.0f * nCntNumber), pos.z));


			m_nForce = 0;

			//スコアのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_FORCENUMBER);
		}
	}

	return S_OK;
}


//=============================================================================
//終了処理
//=============================================================================
void CForceNumber::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < FORCE_NUMBER; nCntNumber++)
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
void CForceNumber::Update(void)
{
	int nNumAttack = CForce::GetNumAttack();
	int nNumSpread = CForce::GetNumSpread();
	int nNumSpeed = CForce::GetNumSpeed();
	int nNumDefense = CForce::GetNumDefense();

	//攻のフォース
	m_apNumber[0]->SetNumber(nNumAttack);
	m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f,1.0f));

	//拡のフォース
	m_apNumber[1]->SetNumber(nNumSpread);
	m_apNumber[1]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	//速のフォース
	m_apNumber[2]->SetNumber(nNumSpeed);
	m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//護
	m_apNumber[3]->SetNumber(nNumDefense);
	m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));

}

//=============================================================================
//描画処理
//=============================================================================
void CForceNumber::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < FORCE_NUMBER; nCntNumber++)
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
void CForceNumber::SetForce(int nFoece)
{
	m_nForce = nFoece;



}

//=============================================================================
//スコアの取得
//=============================================================================
int CForceNumber::GetForce(void)
{
	return m_nForce;
}

//=============================================================================
//スコアの加算
//=============================================================================
void CForceNumber::AddForce(int nValue)
{


}

