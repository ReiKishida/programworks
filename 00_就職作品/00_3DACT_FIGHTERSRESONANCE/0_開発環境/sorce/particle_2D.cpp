//=============================================================================
//
// エフェクトの処理[2Dポリゴン] [effect.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "particle_2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticle2D::m_pTexture = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CParticle2D::CParticle2D(int nPriority , OBJTYPE type) : CScene2D(nPriority, type)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CParticle2D::~CParticle2D()
{

}
//=============================================================================
//テクスチャのロード
//=============================================================================
HRESULT CParticle2D::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect001.jpg", &m_pTexture);

	return S_OK;

}
//=============================================================================
//テクスチャの破棄
//=============================================================================
void CParticle2D::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
//エフェクトの生成
//=============================================================================
CParticle2D *CParticle2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nLife)
{
	//エフェクトの生成
	CParticle2D *pParticle;
	pParticle = new CParticle2D;


	if (pParticle != NULL)
	{//pParticleが空じゃなかったら
		pParticle->m_pos = pos;
		//色情報の設定
		pParticle->m_col = col;
		//種類の設定
		pParticle->m_type = type;
		switch (pParticle->m_type)
		{
		case PARITCLETYPE_FLAME:

			pParticle->m_move.x = (rand() % 4 + 1) * 1.0f;
			pParticle->m_move.y = (rand() % 5 + 1) * 1.0f;
			pParticle->m_move.z = 0.0f;

			break;
		}
		//ライフの設定
		pParticle->m_nLife = nLife;
		//半径の設定
		pParticle->m_fRadius = (rand()% 10 + 1) * 1.0f;
		//エフェクトの初期化
		pParticle->Init();

	}

	return pParticle;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CParticle2D::Init(void)
{

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture);

	//ポリゴンの幅、高さ設定
	SetPolygonWidth(m_fRadius);
	SetPolygonHeight(m_fRadius);


	//2Dシーンの初期化処理
	CScene2D::Init();
	//位置情報の代入
	SetPosPolygon(m_pos);

	//色情報設定
	SetColor(m_col);

	//エフェクトのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CParticle2D::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CParticle2D::Update(void)
{


	//エフェクトの情報
	D3DXVECTOR3 posParticle = GetPosPolygon();	//エフェクトの位置情報
	D3DXVECTOR3 rotEffect = GetRotPolygon();	//エフェクトの角度情報
	float fLengthEffect = GetLengthPolygon();	//エフェクトの長さ取得



	//ポリゴンの縮小
	fLengthEffect--;

	//位置情報を加算
	posParticle += m_move;

	//位置情報の設定
	SetPosPolygon(posParticle);

	//角度情報の設定
	SetRotPolygon(rotEffect);

	//長さの設定
	SetLengthPolygon(fLengthEffect);

	//ライフの減少
	m_nLife--;


	if (m_nLife <= 0)
	{
		//終了処理
		Uninit();
	}
	else if (fLengthEffect <= 0)
	{
		fLengthEffect = 0;
		//終了処理
		Uninit();

	}



}

//=============================================================================
//描画処理
//=============================================================================
void CParticle2D::Draw(void)
{

	//2Dシーンの描画処理
	DrawEffect();


}
