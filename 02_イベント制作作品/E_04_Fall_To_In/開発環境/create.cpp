//=============================================================================
//
// �N���G�C�g�̏��� [create.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�
#include "create.h"
#include "manager.h"
#include "scene.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "DebugProc.h"
#include "meshField.h"
#include "light.h"
#include "objCreate.h"
#include "object.h"
#include "line.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CLight		*CCreate::m_pLight=NULL;
CCamera		*CCreate::m_pCamera = NULL;
CScene3D	*CCreate::m_pScene3D = NULL;
CMeshField	*CCreate::m_pMeshField = NULL;
CObjCreate  *CCreate::m_pObjCreate = NULL;
CFieldCreate  *CCreate::m_pFieldCreate = NULL;

CPlayer     *CCreate::m_pPlayer = NULL;
CLine         *CCreate::m_pLine = NULL;
CCreate::CREATEMODE  CCreate::m_mode = CREATEMODE_OBJECT;

bool CCreate::m_bPause = false;					// �|�[�Y���Ă邩�ǂ���
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCreate::CCreate()
{
	m_bPause = false;// �|�[�Y���Ă邩�ǂ���
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCreate::~CCreate()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCreate::Init(void)
{
	m_bPause = false;// �|�[�Y���Ă邩�ǂ���

	if (m_pLight == NULL)
	{// ���C�g�̐���
		m_pLight = new CLight;
	}
	if (m_pLight != NULL)
	{// ���C�g�̏�����
		if (FAILED(m_pLight->Init()))
		{// ���C�g�̏�����
			return -1;
		}
	}
	if (m_pCamera == NULL)
	{// �J�����̐���
		m_pCamera = new CCamera;
	}
	if (m_pCamera != NULL)
	{// �J�����̏�����
		if (FAILED(m_pCamera->Init()))
		{// �J�����̏�����
			return -1;
		}
	}

	CObjCreate::SetScan();

	m_pObjCreate = CObjCreate::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0);
	m_pFieldCreate = CFieldCreate::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0);
	m_pLine = CLine::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCreate::Uninit(void)
{

	if (m_pLight != NULL)
	{// ���C�g�̔j��
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	if (m_pCamera != NULL)
	{// �J�����̔j��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �I�u�W�F�N�g�N���X�̔j��
	CScene::ReleaseAll();

}

//=============================================================================
// �X�V����
//=============================================================================
void CCreate::Update(void)
{
	if (m_pLight != NULL)
	{// ���C�g�̍X�V
		m_pLight->Update();
	}
	if (m_pCamera != NULL)
	{// �J�����̍X�V
		m_pCamera->Update();
	}

	// �쐬���郂�[�h�̐؂�ւ�
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F10))
	{
		m_mode = m_mode ? CREATEMODE_OBJECT : CREATEMODE_FIELD;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F5))
	{// �N���G�C�g���[�h��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{
			CFade::SetFade(CManager::MODE_GAME);
		}
	}
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();			// �}�E�X
	if (pInputKeyboard->GetTrigger(DIK_P))
	{// �|�[�Y�̏���
		SetPause(m_bPause ? false : true);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCreate::Draw(void)
{
}
//=============================================================================
// ����
//=============================================================================
CCreate *CCreate::Create(void)
{
	CCreate *pCCreate = NULL;
	if (!CScene::OverData())
	{// �Q�[������
		pCCreate = new CCreate;
	}
	if (pCCreate != NULL)
	{
		if (FAILED(pCCreate->Init()))
		{// ����������
			return NULL;
		}

	}
	return pCCreate;
}

//=============================================================================
// �|�[�Y�̎擾
//=============================================================================
bool CCreate::GetPause(void)
{
	return m_bPause;
}
//=============================================================================
// �|�[�Y�̐ݒ�
//=============================================================================
void CCreate::SetPause(bool bPause)
{
	m_bPause = bPause;
}