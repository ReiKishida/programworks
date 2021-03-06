//=============================================================================
//
// シェーダー処理 [Shader.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "Shader.h"
#include "manager.h"
#include "renderer.h"
//*********************************************************************************
// 静的メンバ変数宣言
//*********************************************************************************
LPD3DXEFFECT CShader::m_pShader = NULL;					// シェーダデータ
LPDIRECT3DVERTEXDECLARATION9 CShader::m_pDecl = NULL;	// シェーダー
LPDIRECT3DTEXTURE9 CShader::m_pTexture = NULL;			// シェーダーテクスチャ
//=============================================================================
// コンストラクタ
//=============================================================================
CShader::CShader()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CShader::~CShader()
{
}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CShader::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// シェーダの生成
	D3DXCreateEffectFromFile(pDevice,	// 描画デバイス
		TEXT("data/Shader.hlsl"),			// シェーダ名の相対パス
		NULL,							// プリプロセッサを行う構造体(NULLでOK)
		NULL,							// include操作を行うインターフェース(NULLでOK)
		D3DXSHADER_DEBUG,				// 読み込み時のオプション(今回はデバッグモードで読み込んでますが、通常0にしておきます)
		0,								// シェーダ間でやり取りする変数があった場合に指定(通常0でOK)
		&m_pShader,						// シェーダのポインタを取得
		NULL);							// エラー情報(必要なければNULLでOK)
										// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		"data/TEXTURE/ramp.png",		// ファイルの名前
		&m_pTexture);					// テクスチャへのポインタ

	// ライトの設定
	m_pShader->SetVector("g_fLightDir", &D3DXVECTOR4(-0.89f, -0.1f, 0.94f, 0)); // <-ひとまず[0]番目のライトの方向コピペしました


	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CShader::UnLoad(void)
{
	if (m_pShader != NULL)
	{// シェーダー開放
		m_pShader->Release();
		m_pShader = NULL;
	}
	if (m_pDecl != NULL)
	{
		m_pDecl->Release();
		m_pDecl = NULL;
	}

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShader::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShader::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CShader::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CShader::Draw(void)
{
}
//=============================================================================
// 生成
//=============================================================================
CShader *CShader::Create(void)
{
	CShader *pShader;

	pShader = new CShader;
	if (pShader != NULL)
	{
		// 初期化処理
		if (FAILED(pShader->Init()))
		{
			return NULL;
		}
	}
	return pShader;
}
