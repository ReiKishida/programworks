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
#include "Effect.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "particle.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_LIMIT_MAX_X (1040)	//ステージの左端
#define STAGE_LIMIT_MIN_X (240)		//ステージの右端
#define STAGE_LIMIT_MAX_Y (720)		//ステージの下端
#define STAGE_LIMIT_MIN_Y (0)		//ステージの上端

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CParticle::CParticle() : CScene2D(5)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}
//=============================================================================
//テクスチャのロード
//=============================================================================
HRESULT CParticle::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;

}
//=============================================================================
//テクスチャの破棄
//=============================================================================
void CParticle::Unload(void)
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
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nLife)
{
	//エフェクトの生成
	CParticle *pParticle;
	pParticle = new CParticle;


	if (pParticle != NULL)
	{//pParticleが空じゃなかったら
		//色情報の設定
		pParticle->m_col = col;
		//種類の設定
		pParticle->m_type = type;
		switch (pParticle->m_type)
		{
		case PARITCLETYPE_HIT:
			pParticle->m_move = D3DXVECTOR3(sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f,
											cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f,
											0.0f);
			break;
		case PARITCLETYPE_HIT_R:
			pParticle->m_move = D3DXVECTOR3((sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f) * -1.0f,
											(cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f) * -1.0f,
											0.0f);
			break;
		case PARITCLETYPE_EXPLOSION:
			pParticle->m_move = D3DXVECTOR3(sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f,
											cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f,
											0.0f);
			break;
		case PARITCLETYPE_EXPLOSION_R:
			pParticle->m_move = D3DXVECTOR3((sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f) * -1.0f,
											(cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f) * -1.0f,
											0.0f);
			break;
		}
		//ライフの設定
		pParticle->m_nLife = nLife;
		//半径の設定
		pParticle->m_fRadius = (rand()% 40 + 1) * 1.0f;
		//エフェクトの初期化
		pParticle->Init(pos);

	}

	return pParticle;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos)
{

	//テクスチャの割り当て
	CScene2D::BindTexture(m_pTexture);

	//ポリゴンの幅、高さ設定
	SetScene2DWidth(m_fRadius);
	SetScene2DHeight(m_fRadius);



	//2Dシーンの初期化処理
	CScene2D::Init(pos);
	//色情報設定
	SetColor(m_col);
	//ライフ設定
	SetLife(m_nLife);
	//エフェクトのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CParticle::Update(void)
{


	//エフェクトの情報
	D3DXVECTOR3 posParticle = GetPosScene2D();	//エフェクトの位置情報
	D3DXVECTOR3 rotEffect = GetRotScene2D();	//エフェクトの角度情報
	float fLengthEffect = GetLengthScene2D();	//エフェクトの長さ取得



	//ポリゴンの縮小
	fLengthEffect--;

	//位置情報を加算
	posParticle += m_move;

	//位置情報の設定
	SetPosScene2D(posParticle);

	//角度情報の設定
	SetRotScene2D(rotEffect);

	//長さの設定
	SetLengthScene2D(fLengthEffect);

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
void CParticle::Draw(void)
{

	//2Dシーンの描画処理
	DrawEffect();


}
