////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��̏��� [sky.cpp]
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
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSky::m_pTexture[CSky::TYPE_MAX] = {};


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSky::CSky() :CScene3DSphere(PRIORITY_SKY)
{
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSky::~CSky()
{
}

//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CSky::Load(void)
{
	// �f�o�C�X�̎擾
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
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice,
			pTexture[nCnt],
			&m_pTexture[nCnt]);
	}
	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CSky::UnLoad(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSky::Init(void)
{
	CScene3DSphere::Init();

	SetObjType(CScene::OBJTYPE_SKY);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSky::Uninit(void)
{
	CScene3DSphere::Uninit();

}

//=============================================================================
// �X�V����
//=============================================================================
void CSky::Update(void)
{


}
//=============================================================================
// �`�揈��
//=============================================================================
void CSky::Draw(void)
{
	// �f�o�C�X�̎擾
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
// ����
//=============================================================================
CSky *CSky::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight,
	const int nCutH, const int nCutV, const D3DXCOLOR col, CSky::TYPE type, const int nDirection )
{
	CSky *pPowerCut = NULL;

	if (pPowerCut == NULL)
	{// �C�x���g����������
		pPowerCut = new CSky;
	}
	if (pPowerCut != NULL)
	{
		if (FAILED(pPowerCut->Init()))
		{// ����������
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
