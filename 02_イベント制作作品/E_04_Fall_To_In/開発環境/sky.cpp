////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 空の処理 [sky.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sky.h"
#include "manager.h"
#include "input.h"
#include "manager.h"
#include "player.h"
#include "game.h"
#include "renderer.h"
#include "sound.h"
#include "model.h"
#include "camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSky::m_pTexture[CSky::TYPE_MAX] = {};


//=============================================================================
// コンストラクタ
//=============================================================================
CSky::CSky() :CScene3DSphere(PRIORITY_SKY)
{
}
//=============================================================================
// デストラクタ
//=============================================================================
CSky::~CSky()
{
}

//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CSky::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	char *pTexture[TYPE_MAX]=
	{
		"data/TEXTURE/arth_001.png",
		"data/TEXTURE/utyu.jpg",
		"data/TEXTURE/arth_004.png",
		"data/TEXTURE/arth_005.png",
		"data/TEXTURE/moon.jpg",

	};
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice,
			pTexture[nCnt],
			&m_pTexture[nCnt]);
	}
	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CSky::UnLoad(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSky::Init(void)
{
	CScene3DSphere::Init();

	SetObjType(CScene::OBJTYPE_SKY);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSky::Uninit(void)
{
	CScene3DSphere::Uninit();

}

//=============================================================================
// 更新処理
//=============================================================================
void CSky::Update(void)
{


}
//=============================================================================
// 描画処理
//=============================================================================
void CSky::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (m_type == 2||m_type == 3)
	{
		CRenderer::SetAlphaBlend(true);
	}
	CScene3DSphere::Draw();
	CRenderer::SetAlphaBlend(false);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}
//=============================================================================
// 生成
//=============================================================================
CSky *CSky::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight,
	const int nCutH, const int nCutV, const D3DXCOLOR col, CSky::TYPE type, const int nDirection )
{
	CSky *pPowerCut = NULL;

	if (pPowerCut == NULL)
	{// イベントが発生生成
		pPowerCut = new CSky;
	}
	if (pPowerCut != NULL)
	{
		if (FAILED(pPowerCut->Init()))
		{// 初期化処理
			return NULL;
		}
		pPowerCut->SetAll(pos, rot, fRadius, fHeight,
			nCutH, nCutV, col, nDirection);
		pPowerCut->MakeVertex();
		pPowerCut->m_type = type;
		pPowerCut->BindTexture(m_pTexture[type]);

	}
	return pPowerCut;
}
