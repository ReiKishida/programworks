//=============================================================================
//
// ���U���g�̏��� [result.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "result.h"
#include "player.h"
#include "sky.h"
#include "InfoUI.h"

//*******************************************************1**********************
// �ÓI�����o�ϐ�
//*****************************************************************************
CCamera		*CResult::m_pCamera = NULL;
CLight		*CResult::m_pLight = NULL;

CPlayer		*CResult::m_pPlayer = NULL;		// �v���C���[
CPlayer		*CResult::m_pPlayer2 = NULL;	// �v���C���[

int CResult::m_nWin = 2;

int CResult::n1PMotion = 0;
int CResult::n2PMotion = 0;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	m_pPlayer = NULL;		// �v���C���[
	 m_pPlayer2 = NULL;	// �v���C���[

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	if (m_pCamera == NULL)
	{// �J�����̐���
		m_pCamera = new CCamera;
	}
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
	if (m_pCamera != NULL)
	{// �J�����̏�����
		if (FAILED(m_pCamera->Init()))
		{// �J�����̏�����
			return -1;
		}
	}


	CSky::Create(D3DXVECTOR3(450, -300, 0), D3DXVECTOR3(0, 0, 0), 400, 400,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_MOON, 0);


	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(0, 0, 0), 20000, 20000,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_UTHU, 1);


	if (m_nWin == 0)
	{// 1
		CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_WIN);
		if (m_pPlayer == NULL)
		{// �v���C���[�̐���
			m_pPlayer = CPlayer::Create(0);

			m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

			m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

			// ���[�V�����^�C�v�ݒ�
			m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_WIN);
		}
		if (m_pPlayer2 == NULL)
		{// �v���C���[�̐���
			m_pPlayer2 = CPlayer::Create(1);

			m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

			m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
			// ���[�V�����^�C�v�ݒ�
			m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
	}
	else if (m_nWin == 1)
	{// 2
		CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_LOOSE);
		if (m_pPlayer == NULL)
		{// �v���C���[�̐���
			m_pPlayer = CPlayer::Create(0);

			m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

			m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

			// ���[�V�����^�C�v�ݒ�
			m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
		if (m_pPlayer2 == NULL)
		{// �v���C���[�̐���
			m_pPlayer2 = CPlayer::Create(1);

			m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

			m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
			// ���[�V�����^�C�v�ݒ�
			m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_WIN);
		}
	}
	else if (m_nWin == 2)
	{// ��������
		CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_DRAW);

		if (m_pPlayer == NULL)
		{// �v���C���[�̐���
			m_pPlayer = CPlayer::Create(0);

			m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

			m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

			// ���[�V�����^�C�v�ݒ�
			m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
		if (m_pPlayer2 == NULL)
		{// �v���C���[�̐���
			m_pPlayer2 = CPlayer::Create(1);

			m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

			m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
			// ���[�V�����^�C�v�ݒ�
			m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_LOOSE);
		}
	}

	if (m_pPlayer != NULL)
	{// �v���C���[�̐���
		m_pPlayer->SetMotion(n1PMotion);
	}
	if (m_pPlayer2 != NULL)
	{// �v���C���[�̐���
		m_pPlayer2->SetMotion(n2PMotion);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	if (m_pCamera != NULL)
	{// �J�����̔j��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	if (m_pLight != NULL)
	{// ���C�g�̔j��
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	// �I�u�W�F�N�g�N���X�̔j��
	CScene::ReleaseAll();

}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{

	if (m_pCamera != NULL)
	{// �J�����̍X�V
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ���C�g�̍X�V
		m_pLight->Update();
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_START)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_A)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_B)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_X)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_Y))
	{// ���������ꂽ��

		CFade::SetFade(CManager::MODE_TITLE);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
}
//=============================================================================
// ����
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pCResult = NULL;
	if (pCResult == NULL)
	{// ���U���g����
		pCResult = new CResult;
	}
	if (pCResult != NULL)
	{
		if (FAILED(pCResult->Init()))
		{// ����������
			return NULL;
		}

	}
	return pCResult;
}
