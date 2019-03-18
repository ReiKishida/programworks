//=============================================================================
//
// テクスチャの処理 [Texture.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "manager.h"
#include "Texture.h"
#include "scene.h"
#include "scene2D.h"
#include "meshField.h"
#include "meshWall.h"
#include "fade.h"
#include "sceneX.h"
#include "Shader.h"
#include "environment.h"
#include "player.h"
#include "sky.h"
#include "infoUI.h"
#include "PanelHole.h"
#include "buttonMotion.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CTexture::CTexture()
{
}
//=============================================================================
// デストラクタ
//=============================================================================
CTexture::~CTexture()
{
}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CTexture::Load(void)
{
	// テクスチャ読み込み
	CManager::Load();
	CShader::Load();
	CScene2D::Load();
	CSceneX::Load();
	CMeshField::Load();		// メッシュフィールド
	CMeshWall::Load();		// メッシュウォール
	CFade::Load();
	CEnvironmentBillboard::Load();
	CSky::Load();
	CButtonMotion::Load();
	CPanelHole::Load();
	CPlayer::Load();
	CInfoUI::Load();
	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CTexture::UnLoad(void)
{
	// テクスチャ破棄
	CScene2D::UnLoad();
	CShader::UnLoad();
	CSceneX::UnLoad();
	CMeshField::UnLoad();
	CMeshWall::UnLoad();		// メッシュウォール
	CFade::UnLoad();
	CButtonMotion::UnLoad();
	CEnvironmentBillboard::UnLoad();
	CSky::UnLoad();
	CInfoUI::UnLoad();

	CPanelHole::UnLoad();
	CPlayer::UnLoad();

}
//=============================================================================
// 生成
//=============================================================================
CTexture *CTexture::Create(void)
{
	CTexture *pCTexture = NULL;
	if (pCTexture == NULL)
	{// 弾生成
		pCTexture = new CTexture;
	}

	return pCTexture;
}
