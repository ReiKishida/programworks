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
#include "effect.h"
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
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[MAX_EFFECTTYPE] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CEffect::CEffect() : CScene2D(4)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}
//=============================================================================
//テクスチャのロード
//=============================================================================
HRESULT CEffect::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_apTexture[2]);

	return S_OK;

}
//=============================================================================
//テクスチャの破棄
//=============================================================================
void CEffect::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_EFFECTTYPE; nCntTexture++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}

}
//=============================================================================
//エフェクトの生成
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife, EFFECTTYPE type)
{
	//エフェクトの生成
	CEffect *pEffect;
	pEffect = new CEffect;


	if (pEffect != NULL)
	{//pEffectが空じゃなかったら
		//エフェクトの初期化
		pEffect->m_type = type;
		pEffect->Init(pos, col, fWidth, fHeight, nLife);

	}

	return pEffect;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife)
{

	//テクスチャの割り当て
	CScene2D::BindTexture(m_apTexture[m_type]);

	//ポリゴンの幅、高さ設定
	SetScene2DWidth(fWidth);
	SetScene2DHeight(fHeight);

	//ライフの設定
	m_nLife = nLife;

	//2Dシーンの初期化処理
	CScene2D::Init(pos);
	SetColor(col);

	//エフェクトのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CEffect::Update(void)
{


	//エフェクトの情報
	D3DXVECTOR3 posEffect = GetPosScene2D();	//エフェクトの位置情報
	D3DXVECTOR3 rotEffect = GetRotScene2D();	//エフェクトの角度情報
	float fLengthEffect = GetLengthScene2D();	//エフェクトの長さ取得

	//ポリゴンの縮小
	fLengthEffect--;

	//位置情報の設定
	SetPosScene2D(posEffect);

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
	else if(fLengthEffect <= 0)
	{
		fLengthEffect = 0;
		//終了処理
		Uninit();

	}



}

//=============================================================================
//描画処理
//=============================================================================
void CEffect::Draw(void)
{

	//2Dシーンの描画処理
	DrawEffect();


}
