//=============================================================================
//
// �^�C�g���̏��� [title.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "scene2D.h"
#include "light.h"
#include "camera.h"
#include "meshField.h"
#include "sound.h"
#include "sky.h"
#include "player.h"
#include "InfoUI.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define NEXTCNT (50)
//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CLight		*CTitle::m_pLight = NULL;
CCamera		*CTitle::m_pCamera = NULL;
CPlayer		*CTitle::m_pPlayer = NULL;		// �v���C���[
CPlayer		*CTitle::m_pPlayer2 = NULL;	// �v���C���[

int			CTitle::m_nCntNext = 0;
bool        CTitle::m_bPush = false;
bool		CTitle::m_bSkip = false;
int			CTitle::m_nCntAppear = 0;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	m_pPlayer = NULL;		// �v���C���[
	m_pPlayer2 = NULL;	// �v���C���[
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
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

	if (m_pPlayer == NULL)
	{// �v���C���[�̐���
		m_pPlayer = CPlayer::Create(0);

		m_pPlayer->SetPos(D3DXVECTOR3(250.0f, 50.0f, 0.0f));

		m_pPlayer->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));

		// ���[�V�����^�C�v�ݒ�
		m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_LOOP);
	}
	if (m_pPlayer2 == NULL)
	{// �v���C���[�̐���
		m_pPlayer2 = CPlayer::Create(1);

		m_pPlayer2->SetPos(D3DXVECTOR3(240.0f, 50.0f, -150.0f));

		m_pPlayer2->SetRot(D3DXVECTOR3(-D3DX_PI / 8.0f, D3DX_PI / 4.0f, 0.0f));
		// ���[�V�����^�C�v�ݒ�
		m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_LOOP);
	}

	CSky::Create(D3DXVECTOR3(450, -300, 0), D3DXVECTOR3(0, 0, 0), 400, 400,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_MOON, 0);


	CSky::Create(D3DXVECTOR3(0, -3000, 0), D3DXVECTOR3(0, 0, 0), 20000, 20000,
		20, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CSky::TYPE_UTHU, 1);

	CInfoUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXVECTOR3(0, 0, 0.0f), 0, 800, 500, CInfoUI::TYPE_TITLE_LOGO);
	m_nCntNext = 0;
	m_nCntAppear = 0;
	m_bPush = false;
	m_bSkip = false;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
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
void CTitle::Update(void)
{
	if (m_pCamera != NULL)
	{// �J�����̍X�V
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ���C�g�̍X�V
		m_pLight->Update();
	}

	m_nCntAppear++;	// �J�E���^�[�̉��Z

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_START)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_A)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_B)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_X)
		|| CManager::GetInputGamePad()->GetGamePadTrigger(0, BUTTON_Y))
	{// ���������ꂽ��
		m_bPush = true;	// �X�C�b�`�̐؂�ւ�
		if (m_bSkip == true)
		{
		//	CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_GAMESTART);
			m_bPush = true;	// �X�C�b�`�̐؂�ւ�
		}
		else if (m_bSkip == false)
		{
			m_bSkip = true;
			if (m_pPlayer != NULL)
			{// �v���C���[�̐���
			 // ���[�V�����^�C�v�ݒ�
				m_pPlayer->SetMotionType(CPlayer::MOTION_TYPE_JUMP);
				m_pPlayer->SetMove(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
			}
			if (m_pPlayer2 != NULL)
			{// �v���C���[�̐���
			 // ���[�V�����^�C�v�ݒ�
				m_pPlayer2->SetMotionType(CPlayer::MOTION_TYPE_JUMP);
				m_pPlayer2->SetMove(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
			}
		}
	}
	if (m_bPush == true)
	{// �X�C�b�`���I���̎�
		m_nCntNext++;		// �J�E���^�[���Z
		if (m_nCntNext >= NEXTCNT)
		{// �C�ӂ̐����ōX�V
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
			//	CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_START);
				CFade::SetFade(CManager::MODE_GAME);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
}
//=============================================================================
// ����
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pCTitle = NULL;
	if (pCTitle == NULL)
	{// �^�C�g������
		pCTitle = new CTitle;
	}
	if (pCTitle != NULL)
	{
		if (FAILED(pCTitle->Init()))
		{// ����������
			return NULL;
		}
	}
	return pCTitle;
}
//=======
//
//=======
void CTitle::SetSkip(bool skip)
{
	m_bSkip = skip;
}