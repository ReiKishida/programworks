//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���m�点UI�̏��� [infoUI.cpp]
// Author : Ryota Kinoshita
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "infoUI.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "title.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COUNTDOWN_SIZE (150)	// �J�E���g�_�E���̃T�C�Y
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CInfoUI::m_apTexture[TYPE_MAX] = {};			// �e�N�X�`��
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInfoUI::CInfoUI(int nPriority):CScene2D(nPriority)
{
	m_nCounter = 0;	// �J�E���^�[
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړI�̍��W
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInfoUI::~CInfoUI()
{
}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CInfoUI::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	char *pFileName[TYPE_MAX] =
	{// �t�@�C����
		"data/TEXTURE/UI/title.png",
		"data/TEXTURE/countdown.png",
		"data/TEXTURE/UI/1pwin.png",
		"data/TEXTURE/UI/2pwin.png",
		"data/TEXTURE/UI/draw.png",

	};
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice,
			pFileName[nCnt],
			&m_apTexture[nCnt]);
	}


	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CInfoUI::UnLoad(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInfoUI::Init(void)
{
	if (FAILED(CScene2D::Init()))
	{
		return -1;
	}
	SetObjType(CScene::OBJTYPE_INFOUI);
	m_nCounter = 0;	// �J�E���^�[
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړI�̍��W



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInfoUI::Uninit(void)
{
	CScene2D::Uninit();
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CInfoUI::Update(void)
{

	//int nTimeOld = m_nTime;
	//m_nTime = CGame::GetTimer()->GetTimer();

}
//=============================================================================
// �`�揈��
//=============================================================================
void CInfoUI::Draw(void)
{
	CScene2D::Draw();
}
//=============================================================================
// ����
//=============================================================================
CInfoUI *CInfoUI::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 Destpos, const float fRot, float fWidth, const float fHeight,const TYPE type)
{
	CInfoUI *pInfoUI = NULL;
	if (!CScene::OverData())
	{// ����
		switch (type)
		{
		case TYPE_TITLE_LOGO:		// �^�C�g�����S
			pInfoUI = CTitleLogo::Create(CTitleLogo::LOGOTYPE_1);
			break;

		default:
			pInfoUI = new CInfoUI;
			break;
		}
	}
	if (pInfoUI != NULL)
	{
		if (FAILED(pInfoUI->Init()))
		{// ����������
			return NULL;
		}
		pInfoUI->SetPos(pos+ Destpos);
		pInfoUI->SetDestPos(Destpos);
		pInfoUI->SetRot(fRot);
		pInfoUI->SetSize(fWidth, fHeight);
		pInfoUI->BindTexture(m_apTexture[type]);
		pInfoUI->SetVtx();
		pInfoUI->SetObjType(CScene::OBJTYPE_INFOUI);
	}
	return pInfoUI;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �^�C�g�����S�̏��� [infoUI.cpp]
// Author : Okabe Kazuki
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitleLogo::CTitleLogo() :CInfoUI(PRIORITY_TITLEROGO)
{
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitleLogo::~CTitleLogo()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitleLogo::Init(void)
{
	if (FAILED(CInfoUI::Init()))
	{
		return -1;
	}

	SetObjType(CScene::OBJTYPE_INFOUI);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitleLogo::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CTitleLogo::Update(void)
{

}
//=============================================================================
// �`�揈��
//=============================================================================
void CTitleLogo::Draw(void)
{
	CInfoUI::Draw();
}
//=============================================================================
// ����
//=============================================================================
CTitleLogo *CTitleLogo::Create(const LOGOTYPE type)
{
	CTitleLogo *pTitleLogo = NULL;
	// �N���X�̐���
	pTitleLogo = new CTitleLogo;

	if (pTitleLogo == NULL)
	{// �����ł��Ȃ������ꍇ
		return NULL;
	}

	// ����������
	if (FAILED(pTitleLogo->Init()))
	{
		return NULL;
	}
	pTitleLogo->m_type = type;

	return pTitleLogo;
}

