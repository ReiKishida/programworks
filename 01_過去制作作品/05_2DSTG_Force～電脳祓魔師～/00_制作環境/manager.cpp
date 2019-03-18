//=============================================================================
//
// �}�l�[�W������ [manager.cpp]
// Author :�ݓc��(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "ui.h"
#include "effect.h"
#include "sound.h"
#include "score.h"
#include"number.h"
#include "polygon.h"
#include "force.h"
#include "timer.h"
#include "followforce.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "menu.h"
#include "ranking.h"
#include "menuranking.h"
#include "boss.h"
#include "tutorial.h"
#include "inputx.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;			//�����_���[
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	//�L�[�{�[�h
CInputX *CManager::m_pInputX = NULL;	//XInput
CPlayer *CManager::m_pPlayer = NULL;	//�v���C���[
CEnemy *CManager::m_pEnemy = NULL;		//�G
CBoss *CManager::m_pBoss = NULL;		//�{�X
CSound *CManager::m_pSound = NULL;		//�T�E���h
CScore *CManager::m_pScore = NULL;		//�X�R�A
CMenu *CManager::m_pMenu = NULL;		//���j���[
CTitle *CManager::m_pTitle  = NULL;//�^�C�g��
CGame *CManager::m_pGame = NULL;//�Q�[��
CTutorial *CManager::m_pTutorial = NULL;//�`���[�g���A��
CResult *CManager::m_pResult = NULL;//���U���g
CRanking *CManager::m_pRanking = NULL;//�����L���O
CMenuRanking *CManager::m_pMenuRanking = NULL;//�����L���O
CFade *CManager::m_pFade = NULL;//�t�F�[�h
CManager::MODE CManager::m_mode = MODE_TITLE;//���[�h
int CManager::m_nScore = 0;	//�X�R�A
int CManager::m_nHighScore = 0;	//�n�C�X�R�A
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}



//=============================================================================
//����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//�n�C�X�R�A
	m_nHighScore = 40000;
	//�X�R�A
	m_nScore = 0;

	//�����_���[�̐���
	m_pRenderer = new CRenderer;
	if (m_pRenderer != NULL)
	{
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{
			return -1;
		}
	}


	//�L�[�{�[�h�̐���
	m_pInputKeyboard = new CInputKeyboard;
	if (m_pInputKeyboard != NULL)
	{
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}


	//�Q�[���p�b�h�̏�����
	InitJoyPad();

	//�T�E���h�̍쐬
	m_pSound = new CSound;
	if (m_pSound != NULL)
	{
		if (FAILED(m_pSound->InitSound(hWnd)))
		{
			return -1;
		}
	}

	//���[�h�̐ݒ�
	SetMode(MODE_TITLE);



	return S_OK;

}
//=============================================================================
//�I������
//=============================================================================
void CManager::Uninit()
{
	CScene::ReleaseSceneAll();

	//�^�C�g����ʂ̏I������
	m_pTitle->Uninit();
	//�^�C�g����ʂ̔j��
	delete m_pTitle;
	m_pTitle = NULL;

	//���j���[��ʂ̏I��
	m_pMenu->Uninit();
	//���j���[��ʂ̔j��
	delete m_pMenu;
	m_pMenu = NULL;

	//�Q�[����ʂ̏I��
	m_pGame->Uninit();
	//�Q�[����ʂ̔j��
	delete m_pGame;
	m_pGame = NULL;

	//���U���g��ʂ̏I��
	m_pResult->Uninit();
	//���U���g��ʂ̔j��
	delete m_pResult;
	m_pResult = NULL;

	//�����L���O��ʂ̏I��
	m_pRanking->Uninit();
	//�����L���O��ʂ̔j��
	delete m_pRanking;
	m_pRanking = NULL;

	//�`���[�g���A����ʂ̏I��
	m_pTutorial->Uninit();
	//�`���[�g���A����ʂ̔j��
	delete m_pRanking;
	m_pTutorial = NULL;



	//���j���[�����L���O��ʂ̏I��
	m_pMenuRanking->Uninit();
	//�����L���O��ʂ̔j��
	delete m_pMenuRanking;
	m_pMenuRanking = NULL;

	//�Q�[���p�b�h�I��
	UninitJoyPad();

	//�T�E���h�̏I������
	if (m_pSound != NULL)
	{
		//�I������
		m_pSound->UninitSound();
		//�T�E���h�̔j��
		delete m_pSound;
		m_pSound = NULL;

	}

	//�T�E���h�̏I������
	if (m_pFade != NULL)
	{
		//�I������
		m_pFade->Uninit();

		//�T�E���h�̔j��
		delete m_pFade;
		m_pFade = NULL;

	}


	//�L�[�{�[�h�̏I������
	if (m_pInputKeyboard != NULL)
	{
		// �I������
		m_pInputKeyboard->Uninit();

		//�L�[�{�[�h�̔j��
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}


	//�����_���[�̏I������
	if (m_pRenderer != NULL)
	{
		// �I������
		m_pRenderer->Uninit();

		//�����_���[�̔j��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}


}
//=============================================================================
//�X�V����
//=============================================================================
void CManager::Update()
{
	//�L�[�{�[�h�̍X�V
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//�����_���[�̍X�V
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}


	switch (m_mode)
	{//���[�h
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_MENU:
		m_pMenu->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Update();
		break;

	case MODE_RESULT:
		m_pResult->Update();
		break;

	case MODE_RANKING:
		m_pRanking->Update();
		break;

	case MODE_MENURANKING:
		m_pMenuRanking->Update();
		break;
	}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CManager::Draw()
{
	//�����_���[�̍X�V
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}

	switch (m_mode)
	{//���[�h
	case MODE_TITLE:
		m_pTitle->Draw();
		break;

	case MODE_MENU:
		m_pMenu->Draw();
		break;

	case MODE_GAME:
		m_pGame->Draw();
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Draw();
		break;

	case MODE_RESULT:
		m_pResult->Draw();
		break;

	case MODE_RANKING:
		m_pRanking->Draw();
		break;

	case MODE_MENURANKING:
		m_pMenuRanking->Draw();
		break;
	}

}

//=============================================================================
// �����_�����O�N���X�̎擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
// �C���v�b�g�N���X�̎擾(�L�[�{�[�h)
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// �C���v�b�g�N���X�̎擾(XInput)
//=============================================================================
CInputX * CManager::GetInputX(void)
{
	return m_pInputX;
}

//=============================================================================
// �v���C���[�N���X�̎擾
//=============================================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �v���C���[�N���X�̎擾
//=============================================================================
CEnemy *CManager::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// �{�X�N���X�̎擾
//=============================================================================
CBoss *CManager::GetBoss(void)
{
	return m_pBoss;
}

//=============================================================================
// �T�E���h�N���X�̎擾
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// �X�R�A�N���X�̎擾
//=============================================================================
CScore * CManager::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		 m_pTitle->Uninit();
		 delete m_pTitle;
		 m_pTitle = NULL;
		 //�T�E���h�̍Đ�
		 m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE);
		break;

	case MODE_MENU:
		m_pMenu->Uninit();
		delete m_pMenu;
		m_pMenu = NULL;
		//�T�E���h�̍Đ�
		 m_pSound->StopSound(CSound::SOUND_LABEL_BGM_MENU);

		break;

	case MODE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
		//�T�E���h�̍Đ�
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		break;


	case MODE_TUTORIAL:
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
		//�T�E���h�̍Đ�
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TUTORIAL);
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = NULL;
		//�T�E���h�̍Đ�
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_RESULT);
		break;

	case MODE_RANKING:
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
		break;

	case MODE_MENURANKING:
		m_pMenuRanking->Uninit();
		delete m_pMenuRanking;
		m_pMenuRanking = NULL;
		break;
	}

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = new CTitle;
		m_pTitle->Init();
		//�T�E���h�̍Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
		break;

	case MODE_MENU:
		m_pMenu = new CMenu;
		m_pMenu->Init();
		//�T�E���h�̍Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_MENU);
		break;

	case MODE_GAME:
		m_pGame = new CGame;
		m_pGame->Init();
		//�T�E���h�̍Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
		break;

	case MODE_TUTORIAL:
		m_pTutorial = new CTutorial;
		m_pTutorial->Init();
		//�T�E���h�̍Đ�
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TUTORIAL);
		break;

	case MODE_RESULT:
		m_pResult = new CResult;
		m_pResult->Init();
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
		break;

	case MODE_RANKING:
		m_pRanking = new CRanking;
		m_pRanking->Init();
		break;

	case MODE_MENURANKING:
		m_pMenuRanking = new CMenuRanking;
		m_pMenuRanking->Init();
		break;
	}

	m_mode = mode;
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//�n�C�X�R�A�̐ݒ�
//=============================================================================
void CManager::SetHighScore(int nValue)
{
	if (nValue > m_nHighScore)
	{
		m_nHighScore = nValue;
	}
}

//=============================================================================
//�n�C�X�R�A�̎擾
//=============================================================================
int CManager::GetHighScoreNum(void)
{
	return m_nHighScore;
}

//=============================================================================
//�X�R�A�̐ݒ�
//=============================================================================
void CManager::SetScore(int nValue)
{
	m_nScore = nValue;

}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CManager::GetScoreNum(void)
{
	return m_nScore;
}
