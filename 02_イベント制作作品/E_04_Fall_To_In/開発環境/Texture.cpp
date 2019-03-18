//=============================================================================
//
// �e�N�X�`���̏��� [Texture.cpp]
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
// �R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{
}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CTexture::Load(void)
{
	// �e�N�X�`���ǂݍ���
	CManager::Load();
	CShader::Load();
	CScene2D::Load();
	CSceneX::Load();
	CMeshField::Load();		// ���b�V���t�B�[���h
	CMeshWall::Load();		// ���b�V���E�H�[��
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
// �e�N�X�`���j��
//=============================================================================
void CTexture::UnLoad(void)
{
	// �e�N�X�`���j��
	CScene2D::UnLoad();
	CShader::UnLoad();
	CSceneX::UnLoad();
	CMeshField::UnLoad();
	CMeshWall::UnLoad();		// ���b�V���E�H�[��
	CFade::UnLoad();
	CButtonMotion::UnLoad();
	CEnvironmentBillboard::UnLoad();
	CSky::UnLoad();
	CInfoUI::UnLoad();

	CPanelHole::UnLoad();
	CPlayer::UnLoad();

}
//=============================================================================
// ����
//=============================================================================
CTexture *CTexture::Create(void)
{
	CTexture *pCTexture = NULL;
	if (pCTexture == NULL)
	{// �e����
		pCTexture = new CTexture;
	}

	return pCTexture;
}
