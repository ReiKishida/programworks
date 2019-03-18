//=======================================================================================================================
//
// �}�l�[�W������ [manager.cpp]
// Author :�ݓc��(Kishida Rei)
//
//=======================================================================================================================
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
#include "fade.h"
#include "remaining.h"
#include "boss.h"
#include "highscore.h"
#include "forcenumber.h"
#include "cutin.h"
#include "particle.h"
#include "pause.h"
#include "tutorial.h"
#include "tutoriallogo.h"
#include "inputx.h"
//***************************************************************************************************************************************
// �ÓI�����o�ϐ�
//***************************************************************************************************************************************
CTutorial *CTutorial::m_pTutorial = NULL;				//�`���[�g���A���̃|�C���^
CTutorialLogo *CTutorial::m_pTutorialLogo = NULL;				//�`���[�g���A�����S�̃|�C���^
CPlayer *CTutorial::m_pPlayer = NULL;			//�v���C���[�̃|�C���^
CEnemy *CTutorial::m_apEnemy[MAX_ENEMY] = {};	//�G�̃|�C���^
CUi *CTutorial::m_apUi[MAX_UI] = {};			//UI�̃|�C���^
CBg *CTutorial::m_pBg = NULL;					//�w�i�̃|�C���^
CFollowForce *CTutorial::m_pFollowForce = NULL;	//�t�H�[�X�̃|�C���^
CForce *CTutorial::m_pForce = NULL;				//�t�H�[�X�̃|�C���^
CBullet *CTutorial::m_pBullet = NULL;			//�e�̃|�C���^
CEffect *CTutorial::m_pEffect = NULL;			//�G�t�F�N�g�̃|�C���^
CExplosion *CTutorial::m_pExplosion = NULL;		//�����̃|�C���^
CNumber *CTutorial::m_pNumber = NULL;			//�����̃|�C���^
CCutin *CTutorial::m_pCutin = NULL;				//�J�b�g�C���̃|�C���^
CRemaining *CTutorial::m_pRemaining = NULL;		//�c�@���̃|�C���^
CParticle *CTutorial::m_pParticle = NULL;		//�p�[�e�B�N���̃|�C���^
CBoss *CTutorial::m_pBoss = NULL;				//�{�X�̃|�C���^
CForceNumber *CTutorial::m_pForceNumber = NULL;	//�t�H�[�X���̃|�C���^
CHighScore *CTutorial::m_pHighScore = NULL;		//�n�C�X�R�A�̃|�C���^
CScore *CTutorial::m_pScore = NULL;				//�X�R�A�̃|�C���^

CTutorial::TUTORIALSTATE CTutorial::m_TutorialState = TUTORIALSTATE_NORMAL;//�Q�[���̏��
int CTutorial::m_nCntTutorial = 0;//�V�[���̈ڂ�
int CTutorial::m_nCntFrame = 0;//�t���[�����̃J�E���g
JoyState g_JoyStateTutorial;

//======================================================================================================================================
//�R���X�g���N�^
//======================================================================================================================================
CTutorial::CTutorial()
{

}

//======================================================================================================================================
//�f�X�g���N�^
//======================================================================================================================================
CTutorial::~CTutorial()
{

}




//======================================================================================================================================
//����������
//======================================================================================================================================
HRESULT CTutorial::Init(void)
{
	//�Q�[���p�b�h�̏��
	g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	if (CRenderer::GetPause() == true)
	{
		CRenderer::SetPause();
		CPause::SetSelect();
	}

	m_nCntFrame = 0;


	// ->
#if 1
	//�w�i�̃e�N�X�`���̓ǂݍ���
	m_pBg->Load();

	//�G�̃e�N�X�`���ǂݍ���
	CEnemy::Load();


	//UI�̃e�N�X�`���̓ǂݍ���
	CUi::Load();

	//�t�H�[�X�̃e�N�X�`���ǂݍ���
	m_pFollowForce->Load();

	//�t�H�[�X�̃e�N�X�`���ǂݍ���
	m_pForce->Load();

	//�v���C���[�̃e�N�X�`���̓ǂݍ���
	m_pPlayer->Load();

	//�e�̃e�N�X�`���̓ǂݍ���
	m_pBullet->Load();

	//�G�t�F�N�g�e�N�X�`���̓ǂݍ���
	m_pEffect->Load();

	//�����̃e�N�X�`���̓ǂݍ���
	m_pExplosion->Load();

	//�����̃e�N�X�`���̓ǂݍ���
	m_pNumber->Load();

	//�J�b�g�C���̃e�N�X�`���ǂݍ���
	m_pCutin->Load();

	//�c�@���̃e�N�X�`���ǂݍ���
	m_pRemaining->Load();

	//�p�[�e�B�N���̃e�N�X�`���̓ǂݍ���
	m_pParticle->Load();

	//�`���[�g���A�����S�̃e�N�X�`���̓ǂݍ���
	m_pTutorialLogo->Load();

	//�{�X�̃e�N�X�`���ǂݍ���
	m_pBoss->Load();
#endif


	//�w�i�̐���
	m_pBg->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	////�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50.0f, 0.0f));


	////UI�̐���
	m_apUi[0]->CUi::Create(D3DXVECTOR3(100.0f, SCREEN_HEIGHT / 2, 0.0f), m_apUi[0]->UI_TYPE_BG_L);			//���n
	m_apUi[1]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT / 2, 0.0f), m_apUi[0]->UI_TYPE_BG_R);			//���n
	m_apUi[2]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[1]->UI_TYPE_HIGHSCORE);	//�n�C�X�R�A
	m_apUi[3]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[2]->UI_TYPE_SCORE);		//�X�R�A
	m_apUi[4]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[3]->UI_TYPE_ATK);			//�U�̃t�H�[�X��
	m_apUi[5]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[4]->UI_TYPE_SPR);			//�g�̃t�H�[�X��
	m_apUi[6]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[5]->UI_TYPE_SPD);			//���̃t�H�[�X��
	m_apUi[7]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[6]->UI_TYPE_DEF);			//��̃t�H�[�X��
	m_apUi[8]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[7]->UI_TYPE_TIME);			//�^�C���̕���
	m_apUi[9]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[8]->UI_TYPE_COLON);		//�R����
	m_apUi[10]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[9]->UI_TYPE_DOT);			//�h�b�g
	m_apUi[11]->CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_apUi[10]->UI_TYPE_LOG);			//���O�̕���


	//�G�̔z�u
	m_apEnemy[0]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f),CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_NONE);

	////�Ǐ]�t�H�[�X�̐���
	m_pFollowForce->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	////�`���[�g���A�����S�̐���
	m_pTutorialLogo->CTutorialLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120.0f, 0.0f));


	////�t�H�[�X���̐���
	m_pForceNumber->Create(D3DXVECTOR3(150.0f, SCREEN_HEIGHT / 2 - 25.0f, 0.0f));

	////�X�R�A�̐���
	m_pScore->Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 610.0f, SCREEN_HEIGHT / 2 - 170.0f, 0.0f), CScore::SCORETYPE_GAME);
	m_pHighScore->Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 610.0f, SCREEN_HEIGHT / 2 - 280.0f, 0.0f));

	////�t�H�[�X�̐���
	m_pForce->Create(D3DXVECTOR3(25.0f, SCREEN_HEIGHT / 2 - 100.0f, 0.0f));

	////�^�C�}�[�̐���
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));

	////�c�@���̐���
	m_pRemaining->Create(D3DXVECTOR3(80.0f, SCREEN_HEIGHT / 2 + 250.0f, 0.0f));


	m_nCntTutorial = 0;
	return S_OK;

}
//======================================================================================================================================
//�I������
//======================================================================================================================================
void CTutorial::Uninit()
{

	//�S�V�[���̔j��
	CScene::ReleaseSceneAll();


	// ->
#if 1
	//�{�X�̃e�N�X�`���j��
	m_pBoss->Unload();

	//�`���[�g���A�����S�̃e�N�X�`���̔j��
	m_pTutorialLogo->Unload();

	//�p�[�e�B�N���̃e�N�X�`���̔j��
	m_pParticle->Unload();

	//�c�@���̃e�N�X�`���̔j��
	m_pRemaining->Unload();

	//�J�b�g�C���̃e�N�X�`���j��
	m_pCutin->Unload();

	//�����̃e�N�X�`���̔j��
	m_pNumber->Unload();

	//�����̃e�N�X�`���̔j��
	m_pExplosion->Unload();

	//�G�t�F�N�g�e�N�X�`���̔j��
	m_pEffect->Unload();

	//�e�̃e�N�X�`���̔j��
	m_pBullet->Unload();

	//�v���C���[�̃e�N�X�`���̔j��
	m_pPlayer->Unload();


	//�t�H�[�X�̃e�N�X�`���j��
	m_pForce->Unload();

	//�Ǐ]�t�H�[�X�̃e�N�X�`���j��
	m_pFollowForce->Unload();

	//�G�̃e�N�X�`���̔j��
	CEnemy::Unload();


	//UI�̃e�N�X�`���̔j��
	CUi::Unload();

	//�w�i�̃e�N�X�`���̔j��
	m_pBg->Unload();
#endif

}
//======================================================================================================================================
//�X�V����
//======================================================================================================================================
void CTutorial::Update()
{
	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();



	//�G�̐ݒu
	//SetEnemy();


	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();



	switch (m_TutorialState)
	{
	case TUTORIALSTATE_NORMAL:
		break;

	case TUTORIALSTATE_CLEAR:
		m_nCntTutorial--;
		if (m_nCntTutorial <= 0)
		{
			m_TutorialState = TUTORIALSTATE_NONE;
			//CManager::SetHighScore(CScore::GetScore());
			//CManager::SetScore(CScore::GetScore());

			//���[�h�ݒ�
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;

	case TUTORIALSTATE_FAILED:
		m_nCntTutorial--;
		if (m_nCntTutorial <= 0)
		{
			m_TutorialState = TUTORIALSTATE_NONE;
			//CManager::SetHighScore(CScore::GetScore());
			//CManager::SetScore(CScore::GetScore());

			//���[�h�ݒ�
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;
	}

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//R�L�[�������ꂽ�Ƃ�
			if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
				CFade::SetFade(CManager::MODE_GAME);
			}
			g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//======================================================================================================================================
//�`�揈��
//======================================================================================================================================
void CTutorial::Draw()
{
}
//======================================================================================================================================
//�v���C���[�̎擾
//======================================================================================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

CBoss * CTutorial::GetBoss(void)
{
	return m_pBoss;
}

//======================================================================================================================================
//�G�̎擾
//======================================================================================================================================
CEnemy *CTutorial::GetEnemy(int nCntEnemy)
{
	return m_apEnemy[nCntEnemy];
}

//======================================================================================================================================
//�G�̐ݒu
//======================================================================================================================================
void CTutorial::SetEnemy(void)
{
#if 0
	bool bPause;
	bPause = CRenderer::GetPause();
	//�t���[�������Z
	if (bPause == false)
	{
		m_nCntFrame++;
	}
	switch (m_nCntFrame)
	{
		//==================================================================================================================================
		//���w(���v�F6�b)
		//==================================================================================================================================
	case 80:
		m_apEnemy[0]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
	case 120:
		m_apEnemy[1]->CEnemy::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;

		//==================================================================================================================================
		//���w(���v�F6�b)
		//==================================================================================================================================
	case 510:
		m_apEnemy[3]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;
	case 550:
		m_apEnemy[4]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;

		//==================================================================================================================================
		//��O�w(���v�F12�b)
		//==================================================================================================================================
	case 1060:
		m_apEnemy[6]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[7]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		m_apEnemy[8]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_R);
		break;
		//==================================================================================================================================
		//��l�w(���v�F12�b)
		//==================================================================================================================================
	case 1500:
		m_apEnemy[10]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		m_apEnemy[11]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -120.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		m_apEnemy[12]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, -190.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_STOP_L);
		break;
		//==================================================================================================================================
		//��ܐw(���v�F9�b)
		//==================================================================================================================================
	case 2000:
		m_apEnemy[14]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_THROUGH_UNDER);
		m_apEnemy[15]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f), CEnemy::ENEMYTYPE_SPEED, CEnemy::ENEMYMOVE_STOP);
		m_apEnemy[16]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 220.0f, -50.0f, 0.0f), CEnemy::ENEMYTYPE_NORMAL, CEnemy::ENEMYMOVE_THROUGH_UNDER);
		break;
		//==================================================================================================================================
		//��Z�w(���v�F6�b)
		//==================================================================================================================================
	case 2460:
		m_apEnemy[17]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_L_LONG);
		break;
	case 2700:
		m_apEnemy[18]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), CEnemy::ENEMYTYPE_ATTACK, CEnemy::ENEMYMOVE_FAN_R_LONG);
		break;
		//==================================================================================================================================
		//��Z�w(���v�F6�b)
		//==================================================================================================================================
	case 3160:
		m_apEnemy[19]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -250.0f, 0.0f), CEnemy::ENEMYTYPE_SPREAD, CEnemy::ENEMYMOVE_STOP);
		m_apEnemy[20]->CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, -50.0f, 0.0f), CEnemy::ENEMYTYPE_DEFENSE, CEnemy::ENEMYMOVE_THROUGH_UNDER);

		break;
	}
#endif
}

//======================================================================================================================================
//�Q�[����Ԃ̐ݒ�
//======================================================================================================================================
void CTutorial::SetTutorialState(TUTORIALSTATE state)
{
	m_TutorialState = state;
	m_nCntTutorial = 40;
}

//======================================================================================================================================
//�Q�[����Ԃ̎擾
//======================================================================================================================================
CTutorial::TUTORIALSTATE CTutorial::GetTutorialState(void)
{
	return m_TutorialState;
}

//======================================================================================================================================
//�t���[�����̎擾
//======================================================================================================================================
int CTutorial::GetFrame(void)
{
	return m_nCntFrame;


}


