////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �RD�|���S������ [buttonMotion.h]
// Author : ���@�@���Y
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "buttonMotion.h"
#include "renderer.h"
#include "manager.h"
#include "scene3D.h"
//*********************************************************************************
// �ÓI�����o�ϐ��錾
//*********************************************************************************
LPDIRECT3DTEXTURE9 CButtonMotion::m_apTextureThis[TEX_TYPE_MAX] = {};	// �e�N�X�`���̏��
char *CButtonMotion::m_aTexInfo[TEX_TYPE_MAX] =
{
	// �A�h���X
	"data//TEXTURE//UI//A.png",		// A
	"data//TEXTURE//UI//B.png",		// B
	"data//TEXTURE//UI//X.png",		// X
	"data//TEXTURE//UI//Y.png",		// Y
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CButtonMotion::CButtonMotion(int nPriority ):CScene3D(nPriority)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CButtonMotion::~CButtonMotion()
{

}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CButtonMotion::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	for (int nCnt = 0; nCnt < TEX_TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice,
			m_aTexInfo[nCnt],
			&m_apTextureThis[nCnt]);
	}

	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CButtonMotion::UnLoad(void)
{
	for (int nCnt = 0; nCnt < TEX_TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_apTextureThis[nCnt] != NULL)
		{
			m_apTextureThis[nCnt]->Release();
			m_apTextureThis[nCnt] = NULL;
		}
	}
}


//==============================================================================================================================
// ����������
//==============================================================================================================================
HRESULT CButtonMotion::Init(void)
{
	CScene3D::Init();

	m_nCounter = 0;
	m_bDisp = true;

	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CButtonMotion::Uninit(void)
{
	CScene3D::Uninit();
}

//==============================================================================================================================
// �X�V����
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
// �`�揈��
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
// ����
//=============================================================================
CButtonMotion *CButtonMotion::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const TEX_TYPE texType)
{
	CButtonMotion *pButtonMotion = NULL;
	if (!CScene::OverData())
	{// �QD�|���S������
		pButtonMotion = new CButtonMotion;
	}
	if (pButtonMotion != NULL)
	{
		// ����������
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
