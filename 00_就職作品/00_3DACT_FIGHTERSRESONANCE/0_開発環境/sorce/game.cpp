//=============================================================================
//
// �}�l�[�W������ [game.cpp]
// Author :�ݓc��(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "sceneX.h"
#include "model.h"
#include "player.h"
#include "number.h"
#include "scene_meshfield.h"
#include "scene_meshwall.h"
#include "scene_billboard.h"
#include "scene_meshcylinder.h"
#include "scene_gauge.h"
#include "object.h"
#include "fade.h"
#include "ui_gauge.h"
#include "combogauge.h"
#include "particle.h"
#include "score.h"
#include "timer.h"
#include "enemy.h"
#include "enemy_pow.h"
#include "enemy_trc.h"
#include "pause.h"
#include "ui.h"
#include "combo.h"
#include "ui_gauge_bill.h"
#include "effect.h"
#include "boss.h"
#include "camera.h"
#include "pr.h"
#include "cutin.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPD_LIFE (100.0f)
#define POW_LIFE (200.0f)
#define TRC_LIFE (150.0f)


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CGame *CGame::m_pGame = NULL;					//�Q�[��
CBoss *CGame::m_pBoss = NULL;					//�{�X
CPlayer *CGame::m_pPlayer = NULL;				//�v���C���[
CEnemy *CGame::m_pEnemy[30] = {};				//�G
CModel *CGame::m_pModel = {};					//���f��
CObject *CGame::m_pObject = NULL;				//�I�u�W�F�N�g
CMeshCylinder *CGame::m_pMeshCylinder = NULL;	//�V�����_�[
CNumber *CGame::m_pNumber = NULL;				//����
CTimer *CGame::m_pTimer = NULL;					//�^�C�}�[
CSceneMeshField *CGame::m_pMeshField[4] = {};	//���b�V���t�B�[���h
CComboGauge *CGame::m_apComboGauge[2] = {};		//�R���{�Q�[�W
CSceneMeshWall *CGame::m_apMeshWall[15] = {};	//���b�V���E�H�[��
CGame::GAMESTATE CGame::m_GameState = CGame::GAMESTATE_NONE;//�Q�[���̏��
int  CGame::m_nCntGame = 0;					//�V�[���̈ڂ�
bool CGame::m_bCreateBoss = false;			//�{�X�𐶐����邩
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CGame::CGame()
{
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CGame::~CGame()
{

}



//=============================================================================
// �֐����F�Q�[���̏���������
// �֐��̊T�v�F�g�p����e�N�X�`���⃂�f���̓ǂݍ���
//=============================================================================
HRESULT CGame::Init(void)
{
	//�v���C���[�̃��f���̓ǂݍ���
	CPlayer::Load();

	//�G�̃��f���̓ǂݍ���
	CEnemyTrick::Load();

	//�G�̃��f���̓ǂݍ���
	CEnemyPower::Load();

	//�G�̃��f���̓ǂݍ���
	CEnemy::Load();

	//�{�X�̃��f���̓ǂݍ���
	CBoss::Load();

	//�p�[�e�B�N���̃e�N�X�`���̓ǂݍ���
	CParticle::Load();

	//�G�t�F�N�g�̃e�N�X�`���̓ǂݍ���
	CEffect::Load();

	//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	CObject::Load();

	//�Q�[�WUI�̃e�N�X�`���̓ǂݍ���
	CUiGauge::Load();

	//�Q�[�WUI�̃e�N�X�`���̓ǂݍ���
	CUiGaugeBillBoard::Load();

	//�R���{�Q�[�W�̃e�N�X�`���̓ǂݍ���
	CComboGauge::Load();

	//�R���{�̃e�N�X�`���̓ǂݍ���
	CCombo::Load();

	//���b�V���t�B�[���h�̃e�N�X�`���̓ǂݍ���
	CSceneMeshField::Load();

	//���b�V���E�H�[���̃e�N�X�`���̓ǂݍ���
	CSceneMeshWall::Load();

	//�����e�N�X�`���̓ǂݍ���
	CNumber::Load();

	//Ui�̃e�N�X�`���̓ǂݍ���
	CUi::Load();

	//�L���̃e�N�X�`���̓ǂݍ���
	CPr::Load();

	//�J�b�g�C���̃e�N�X�`���ǂݍ���
	CCutin::Load();


	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-100.0f, 0.0f, -350.0f));

	//�G�̐���
	CreateEnemy();

	//���f���̐���
	CObject::LoadModelText();

	//�d���f���̐���
	CPr::Create(D3DXVECTOR3(70.0f, 165.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 5.0f, CPr::PRTYPE_BOARD_SWITCH_0);
	CPr::Create(D3DXVECTOR3(70.0f, 154.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 10.0f, CPr::PRTYPE_BOARD_STREAM);

	CPr::Create(D3DXVECTOR3(820.0f, 165.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 5.0f, CPr::PRTYPE_BOARD_SWITCH_0);
	CPr::Create(D3DXVECTOR3(820.0f, 154.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 10.0f, CPr::PRTYPE_BOARD_STREAM);

	CPr::Create(D3DXVECTOR3(3920.0f, 195.0f, -325.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 5.0f, CPr::PRTYPE_BOARD_SWITCH_0);
	CPr::Create(D3DXVECTOR3(3920.0f, 184.0f, -325.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 10.0f, CPr::PRTYPE_BOARD_STREAM);




	//�L��
	CPr::Create(D3DXVECTOR3(1300.0f, 70.0f, -210.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);
	CPr::Create(D3DXVECTOR3(2300.0f, 70.0f, -210.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);
	CPr::Create(D3DXVECTOR3(4060.0f, 70.0f, -1700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);
	CPr::Create(D3DXVECTOR3(4060.0f, 70.0f, -2504.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);




	//���b�V���t�B�[���h�̐���
	m_pMeshField[0] = CSceneMeshField::Create(D3DXVECTOR3(1850.0f, -0.5f, -200.0f), 45,4,100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);		//�ŏ��̏�
	m_pMeshField[1] = CSceneMeshField::Create(D3DXVECTOR3(3900.0f, -0.5f, -1900.0f), 10, 30, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);	//���̏�
	m_pMeshField[2] = CSceneMeshField::Create(D3DXVECTOR3(4100.0f, -0.5f, -3800.0f), 9, 10, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);	//�{�X�̏�
	m_pMeshField[3] = CSceneMeshField::Create(D3DXVECTOR3(2000.0f, -0.4f, -80.0f), 150, 1, 30.0f, 30.0f, CSceneMeshField::FIELDTYPE_YELLOW);


	//���b�V���E�H�[���̐���(�����Ȃ���)
	CSceneMeshWall::LoadWallText();

	//�X�R�A�̐���
	CScore::Create(D3DXVECTOR3(970.0f, 40.0f, 0.0f), CScore::SCORETYPE_GAME);

	//�^�C�}�[
	CTimer::Create(D3DXVECTOR3(1060.0f, 120.0f, 0.0f),CTimer::TIMEMODE_GAME);

	//UI�̐���(�X�R�A)
	CUi::Create(CUi::UIMODE_GAME);


	//�R���{�̐���
	CCombo::Create();

	//�R���{�Q�[�W�̐���
	m_apComboGauge[0] = CComboGauge::Create(D3DXVECTOR3(1040.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 210.0f, 10.0f, 100.0f, 100.0f, CComboGauge::GAUGETYPE_UNDER);		//�R���{�Q�[�W�̉��n����
	m_apComboGauge[1] = CComboGauge::Create(D3DXVECTOR3(1065.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 170.0f, 5.5f, 0.0f, 100.0f, CComboGauge::GAUGETYPE_COMBO);					//�R���{�Q�[�W�̐���


	m_GameState = GAMESTATE_NORMAL;	//�Q�[���̏�Ԑݒ�
	m_nCntGame = 0;
	m_bCreateBoss = false;

	if (m_pBoss != NULL)
	{
		m_pBoss = NULL;
	}


	return S_OK;

}
//=============================================================================
// �֐����F�Q�[���̏I������
// �֐��̊T�v�F�g�p����e�N�X�`���⃂�f���̔j���A�V�[���̔j��
//=============================================================================
void CGame::Uninit()
{
	//�J�b�g�C���̃e�N�X�`���ǂݍ���
	CCutin::Unload();

	//�L���̃e�N�X�`���̓ǂݍ���
	CPr::Unload();

	//Ui�̃e�N�X�`���̔j��
	CUi::Unload();

	//�v���C���[�̃��f���̔j��
	CPlayer::Unload();

	//�G�̃��f���̔j��
	CEnemy::Unload();

	//�G�̃��f���̔j��
	CEnemyPower::Unload();

	//�G�̃��f���̔j��
	CEnemyTrick::Unload();

	//�{�X�̃��f���̔j��
	CBoss::Unload();

	//�p�[�e�B�N���̃e�N�X�`���̔j��
	CParticle::Unload();

	//�G�t�F�N�g�̃e�N�X�`���̔j��
	CEffect::Unload();

	//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	CObject::Unload();

	//���b�V���t�B�[���h�̃e�N�X�`���̔j��
	CSceneMeshField::Unload();

	//���b�V���E�H�[���̃e�N�X�`���̔j��
	CSceneMeshWall::Unload();

	//�R���{�̐���
	CCombo::Unload();


	//�Q�[�WUI�̃e�N�X�`���̔j��
	CUiGauge::Unload();
	//�Q�[�WUI�̃e�N�X�`���̔j��
	CUiGaugeBillBoard::Unload();


	//�R���{�Q�[�W�̃e�N�X�`���̔j��
	CComboGauge::Unload();


	//�����e�N�X�`���̔j��
	CNumber::Unload();


	//�V�[���̔j��
	CScene::ReleaseSceneAll();

}
//=============================================================================
// �֐����F�Q�[���̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CGame::Update()
{
	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


	// �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_GameState == GAMESTATE_NORMAL)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_P) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
		{//�|�[�YON/OFF

			CRenderer::SetPause();
		}
	}

	switch (m_GameState)
	{
	case GAMESTATE_NORMAL:
		break;

	case GAMESTATE_CLEAR:
		m_nCntGame--;
		if (m_nCntGame <= 0)
		{
			m_GameState = GAMESTATE_NONE;

			//���[�h�ݒ�
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;

	case GAMESTATE_FAILED:
		m_nCntGame--;
		if (m_nCntGame <= 0)
		{
			m_GameState = GAMESTATE_NONE;

			//���[�h�ݒ�
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;
	}


	if (m_bCreateBoss == true)
	{
		if (m_pBoss == NULL)
		{
			m_pBoss = CBoss::Create(D3DXVECTOR3(3900.0f, 0.0f, -3900.0f), D3DXVECTOR3(0.0f, -3.0f, 0.0f), 500.0f);
			CSceneMeshWall::Create(D3DXVECTOR3(3800.0f, 0.0f, -3390.0f), D3DXVECTOR3(0.0f, 1800.0f, 0.0f), 6, 5, 100.0f, 100.0f, CSceneMeshWall::WALLTYPE_INVISIBLE);
			CSceneMeshWall::Create(D3DXVECTOR3(3800.0f, 0.0f, -3400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6, 5, 100.0f, 100.0f, CSceneMeshWall::WALLTYPE_NORMAL);
			CSceneMeshWall::Create(D3DXVECTOR3(3800.0f, 0.0f, -3410.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 5, 100.0f, 100.0f, CSceneMeshWall::WALLTYPE_INVISIBLE);

			pSound->StopSound();
			pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_BOSS);

		}
	}


}

//=============================================================================
// �֐����F�Q�[���̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CGame::Draw()
{

}


//=============================================================================
// �֐����F�G�̐���
// �֐��̊T�v�F--
//=============================================================================
void CGame::CreateEnemy(void)
{
	//�G�̐���
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);
	CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 45.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);

	CEnemy::Create(D3DXVECTOR3(700.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);

	CEnemyPower::Create(D3DXVECTOR3(800.0f, 0.0f, -350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);

	CEnemyTrick::Create(D3DXVECTOR3(1200.0f, 0.0f, -150.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);
	CEnemy::Create(D3DXVECTOR3(1300.0f, 0.0f, -350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);


	CEnemyPower::Create(D3DXVECTOR3(1880.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemyTrick::Create(D3DXVECTOR3(1980.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);


	CEnemy::Create(D3DXVECTOR3(3080.0f, 0.0f, -350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);
	CEnemyPower::Create(D3DXVECTOR3(3010.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemyTrick::Create(D3DXVECTOR3(3010.0f, 0.0f, -230.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);

	CEnemy::Create(D3DXVECTOR3(3669.0f, 0.0f, 65.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);


	CEnemy::Create(D3DXVECTOR3(4000.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);
	CEnemyPower::Create(D3DXVECTOR3(3750.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemy::Create(D3DXVECTOR3(3650.0f, 0.0f, -230.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);



	CEnemyPower::Create(D3DXVECTOR3(3850.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemyTrick::Create(D3DXVECTOR3(3750.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);

	CEnemyTrick::Create(D3DXVECTOR3(3900.0f, 0.0f, -2200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);
	CEnemyTrick::Create(D3DXVECTOR3(3700.0f, 0.0f, -2200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);


	CEnemy::Create(D3DXVECTOR3(3850.0f, 0.0f, -2700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);

}



//=============================================================================
// �֐����F�V�[�����b�V���t�B�[���h�N���X�̎擾
// �֐��̊T�v�F�V�[�����b�V���t�B�[���h�N���X��Ԃ�
//=============================================================================
CSceneMeshField *CGame::GetSceneMeshField(void)
{
	return m_pMeshField[0];
}

//=============================================================================
// �֐����F�v���C���[�N���X�̎擾
// �֐��̊T�v�F�v���C���[�N���X��Ԃ�
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

CBoss * CGame::GetBoss(void)
{
	return m_pBoss;
}

//=============================================================================
// �֐����F�G�N���X�̎擾
// �֐��̊T�v�F�G�N���X��Ԃ�
//=============================================================================
CEnemy *CGame::GetEnemy(int nIndex)
{
	return m_pEnemy[nIndex];
}

//=============================================================================
// �֐����F�ǃN���X�̎擾
// �֐��̊T�v�F�ǃN���X��Ԃ�
//=============================================================================
CSceneMeshWall *CGame::GetWall(int nCnt)
{
	if(m_apMeshWall[nCnt] != NULL)
	{
		return m_apMeshWall[nCnt];
	}
	else
	{
		return NULL;
	}
}

//=============================================================================
// �֐����F�R���{�Q�[�W�N���X�̎擾
// �֐��̊T�v�F�R���{�Q�[�W�N���X��Ԃ�
//=============================================================================
CComboGauge * CGame::GetComboGauge(int nCnt)
{
	return m_apComboGauge[nCnt];
}

//===============================================================================
// �֐����F�Q�[����Ԃ̐ݒ�
// �֐��̊T�v�F�Q�[����Ԃ�ݒ肷��
//===============================================================================
void CGame::SetGameState(GAMESTATE state)
{
	m_GameState = state;
	m_nCntGame = 80;
	CManager::SetScore(CScore::GetScore());
	CManager::SetComboGaugeMax(CComboGauge::GetComboGaugeMax());
	CManager::SetDamage(CPlayer::GetDamage());
	CScene::SetStopBoss(false);
	CScene::SetStop(false);
}


//===============================================================================
// �֐����F�Q�[����Ԃ��擾����
// �֐��̊T�v�F�Q�[����Ԃ�Ԃ�
//===============================================================================
CGame::GAMESTATE CGame::GetGameState(void)
{
	return m_GameState;
}

//===============================================================================
// �֐����F�{�X�𐶐����邩
// �֐��̊T�v�Fbool�̐؂�ւ�
//===============================================================================
void CGame::ChangeBoolCreateBoss(void)
{
	m_bCreateBoss = m_bCreateBoss ? false : true;
}

bool CGame::GetBool(void)
{
	return m_bCreateBoss;
}

