////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ３Dポリゴン処理 [buttonMotion.h]
// Author : 圷　卍　太郎
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "buttonMotion.h"
#include "renderer.h"
#include "manager.h"
#include "scene3D.h"
//*********************************************************************************
// 静的メンバ変数宣言
//*********************************************************************************
LPDIRECT3DTEXTURE9 CButtonMotion::m_apTextureThis[TEX_TYPE_MAX] = {};	// テクスチャの情報
char *CButtonMotion::m_aTexInfo[TEX_TYPE_MAX] =
{
	// アドレス
	"data//TEXTURE//UI//A.png",		// A
	"data//TEXTURE//UI//B.png",		// B
	"data//TEXTURE//UI//X.png",		// X
	"data//TEXTURE//UI//Y.png",		// Y
};

//=============================================================================
// コンストラクタ
//=============================================================================
CButtonMotion::CButtonMotion(int nPriority ):CScene3D(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CButtonMotion::~CButtonMotion()
{

}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CButtonMotion::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	for (int nCnt = 0; nCnt < TEX_TYPE_MAX; nCnt++)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice,
			m_aTexInfo[nCnt],
			&m_apTextureThis[nCnt]);
	}

	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CButtonMotion::UnLoad(void)
{
	for (int nCnt = 0; nCnt < TEX_TYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTextureThis[nCnt] != NULL)
		{
			m_apTextureThis[nCnt]->Release();
			m_apTextureThis[nCnt] = NULL;
		}
	}
}


//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
HRESULT CButtonMotion::Init(void)
{
	CScene3D::Init();

	m_nCounter = 0;
	m_bDisp = true;

	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CButtonMotion::Uninit(void)
{
	CScene3D::Uninit();
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void CButtonMotion::Update(void)
{
	m_nCounter++;

	if (m_nCounter < 50 * 2 + 25)
	{
		if (m_nCounter % 25 == 0)
		{
			m_bDisp = m_bDisp ? false : true;
		}
	}

	CScene3D::Update();
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void CButtonMotion::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (m_bDisp == true)
	{
		CScene3D::Draw();
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 生成
//=============================================================================
CButtonMotion *CButtonMotion::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const TEX_TYPE texType)
{
	CButtonMotion *pButtonMotion = NULL;
	if (!CScene::OverData())
	{// ２Dポリゴン生成
		pButtonMotion = new CButtonMotion;
	}
	if (pButtonMotion != NULL)
	{
		// 初期化処理
		if (FAILED(pButtonMotion->Init()))
		{
			return NULL;
		}
		pButtonMotion->SetPos(pos);
		pButtonMotion->SetSize(fWidth, fHeight, fDepth);
		pButtonMotion->BindTexture(m_apTextureThis[texType]);
		pButtonMotion->SetVtx();
	}
	return pButtonMotion;
}
