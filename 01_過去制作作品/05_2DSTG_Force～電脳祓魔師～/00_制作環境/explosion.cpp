//=============================================================================
//
// 爆発処理[2Dポリゴン] [explosion.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_X (0.125f)	//テクスチャ座標_X
#define TEX_Y (1.0f)//テクスチャ座標_Y
#define PATTERN_ANIM (8)	//テクスチャパターン数
#define ANIMATION_SPEED (6)	//アニメーションスピード

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL; //共有テクスチャのポインタ


//=============================================================================
//コンストラクタ
//=============================================================================
CExplosion::CExplosion()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CExplosion::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CExplosion::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//=============================================================================
//爆発の生成
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, float fRadius)
{
	//爆発のインスタンス生成
	CExplosion *pExplosion;
	pExplosion = new CExplosion;
	if (pExplosion != NULL)
	{
		//爆発の初期化処理
		pExplosion->Init(pos, fRadius);


	}
	return pExplosion;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, float fRadius)
{
	//アニメーションカウンター
	m_nCounterAnim = 0;

	SetScene2DWidth(fRadius);
	SetScene2DHeight(fRadius);


	//テクスチャの割り当て
	BindTexture(m_pTexture);

	//2Dシーンの初期化処理
	CScene2D::Init(pos);
	SetTexture(TEX_X, TEX_Y);

	//爆発のオブジェクトの設定
	SetObjType(CScene::OBJTYPE_EXPLOSION);


	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	//2Dシーンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CExplosion::Update(void)
{
	D3DXVECTOR3 posExplosion = GetPosScene2D();	//プレイヤーの位置情報
	D3DXVECTOR3 rotExplosion = GetRotScene2D();	//プレイヤーの角度情報

	m_nCounterAnim++;
	if (m_nCounterAnim % ANIMATION_SPEED == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % PATTERN_ANIM;//パターンNo.更新
		//位置情報の設定
		CScene2D::SetPosScene2D(posExplosion);

		//テクスチャアニメーション
		CScene2D::SetTextureAnim(TEX_X, TEX_Y, m_nPatternAnim);
		if (m_nPatternAnim <= 0)
		{
			Uninit();
		}
	}


}

//=============================================================================
//描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	//2Dシーンの描画処理
	CScene2D::Draw();
}
