//=============================================================================
//
// エフェクト処理[3Dポリゴン] [effect.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "effect.h"
#include "camera.h"
#include "debugproc.h"
#include "scene_billboard.h"
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PATTERN_ANIM (5)	//テクスチャパターン数
#define PATTERN_X (0.2f)
#define ANIMATION_SPEED (120)	//アニメーションスピード
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[3] = {}; //共有テクスチャのポインタ

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CEffect::CEffect(int nPriority, OBJTYPE type) : CSceneBillBoardAnim(nPriority, type)
{
	//m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	//m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CEffect::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/hit0.png", &m_apTexture[0]);		//光
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/hit1.png", &m_apTexture[1]);		//光
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/hit2.png", &m_apTexture[2]);		//光

	return S_OK;
}

//=============================================================================
// 関数名：使用するテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CEffect::Unload(void)
{
	for (int nCntTex = 0; nCntTex < 3; nCntTex++)
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
// 関数名：エフェクトの生成
// 関数の概要：エフェクトを生成する
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, float fRadius, int nLife, EFFECTTYPE type, EFFECTMODE mode)
{
	CEffect  *pEffect;
	pEffect = new CEffect;//シーンクラスの生成
	if (pEffect != NULL)
	{
		pEffect->m_pos = pos;
		pEffect->m_fRadius = fRadius;
		pEffect->m_nLife = nLife;
		pEffect->m_type = type;
		pEffect->m_mode = mode;
		pEffect->Init();

	}
	return pEffect;
}


//=============================================================================
// 関数名：エフェクトの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CEffect::Init(void)
{

	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();



	switch (m_type)
	{
	case EFFECTTYPE_SMALL:
		//テクスチャの割り当て
		BindTexture(m_apTexture[0]);
		break;
	case EFFECTTYPE_MEDIUM:
		//テクスチャの割り当て
		BindTexture(m_apTexture[1]);
		break;
	case EFFECTTYPE_LARGE:
		//テクスチャの割り当て
		BindTexture(m_apTexture[2]);
		break;
	}

	//半径の設定
	SetRadius(m_fRadius);

	//エフェクトの初期化
	CSceneBillBoardAnim::Init();

	//テクスチャの座標の設定
	SetTexture(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 関数名：エフェクトの終了処理
// 関数の概要：自身を破棄する
//=============================================================================
void CEffect::Uninit(void)
{
	//エフェクトの終了処理
	CSceneBillBoardAnim::Uninit();
}

//=============================================================================
// 関数名：エフェクトの更新処理
// 関数の概要：--
//=============================================================================
void CEffect::Update(void)
{


	//位置情報の設定
	SetPos(m_pos);

	SetRadius(m_fRadius);

	CSceneBillBoardAnim::Update();
	m_nLife--;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		Uninit();
	}

	if (m_mode == EFFECTMODE_SPREAD)
	{
		switch (m_type)
		{
		case EFFECTTYPE_SMALL:
			m_fRadius += 15.0f;
			break;
		case EFFECTTYPE_MEDIUM:
			m_fRadius += 20.0f;
			break;
		case EFFECTTYPE_LARGE:
			m_fRadius += 25.0f;
			break;
		}
	}

	if (m_mode == EFFECTMODE_SHRINKING)
	{
		switch (m_type)
		{
		case EFFECTTYPE_SMALL:
			m_fRadius -= 15.0f;
			break;
		case EFFECTTYPE_MEDIUM:
			m_fRadius -= 20.0f;
			break;
		case EFFECTTYPE_LARGE:
			m_fRadius -= 5.0f;
			break;
		}
	}

	//CDebugProc::Print("m_nLife : %d\n", m_nLife);
	//CDebugProc::Print("m_fRadius : %.1f\n", m_fRadius);

}

//=============================================================================
// 関数名：エフェクトの描画処理
// 関数の概要：行列を変えてカメラ方向を見るように
//=============================================================================
void CEffect::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	CSceneBillBoardAnim::Draw();


}

