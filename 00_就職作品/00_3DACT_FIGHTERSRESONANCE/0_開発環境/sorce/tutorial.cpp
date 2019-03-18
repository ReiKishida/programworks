//=======================================================================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author :�ݓc��(Kishida Rei)
//
//=======================================================================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "title.h"
#include "tutorial.h"
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
#include "sound.h"
#include "cutin.h"
//********************************************************************************************
// �ÓI�����o�ϐ�
//********************************************************************************************
CTutoLogo	*CTutorial::m_pTutoLogo = NULL;
CComboGauge *CTutorial::m_apComboGauge[2] = {};		//�R���{�Q�[�W
CPlayer *CTutorial::m_pPlayer = NULL;				//�v���C���[

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CTutorial::~CTutorial()
{

}




//=============================================================================
// �֐����F�`���[�g���A���̏���������
// �֐��̊T�v�F�g�p����e�N�X�`���⃂�f���̓ǂݍ���
//=============================================================================
HRESULT CTutorial::Init(void)
{
	//�v���C���[�̃��f���̓ǂݍ���
	CPlayer::Load();

	//�G�̃��f���̓ǂݍ���
	CEnemy::Load();

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

	CCutin::Load();

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-100.0f, 0.0f, -350.0f));
	CEnemy::Create(D3DXVECTOR3(250.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, CEnemy::ENEMYTYPE_SPEED);

	CSceneMeshWall::LoadWallText_Tuto();

	//���f���̐���
	CObject::LoadModelTextTuto();

	//���b�V���t�B�[���h
	CSceneMeshField::Create(D3DXVECTOR3(1850.0f, -0.5f, -200.0f), 45, 4, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);		//�ŏ��̏�
	CSceneMeshField::Create(D3DXVECTOR3(2000.0f, -0.4f, -110.0f), 200, 1, 30.0f, 30.0f, CSceneMeshField::FIELDTYPE_YELLOW);

	//�X�R�A�̐���
	CScore::Create(D3DXVECTOR3(970.0f, 40.0f, 0.0f), CScore::SCORETYPE_GAME);

	//�^�C�}�[
	CTimer::Create(D3DXVECTOR3(1060.0f, 120.0f, 0.0f), CTimer::TIMEMODE_GAME);

	//UI�̐���(�X�R�A)
	CUi::Create(CUi::UIMODE_TUTORIAL);


	//�R���{�̐���
	CCombo::Create();

	//�R���{�Q�[�W�̐���
	m_apComboGauge[0] = CComboGauge::Create(D3DXVECTOR3(1040.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 210.0f, 10.0f, 100.0f, 100.0f, CComboGauge::GAUGETYPE_UNDER);		//�R���{�Q�[�W�̉��n����
	m_apComboGauge[1] = CComboGauge::Create(D3DXVECTOR3(1065.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 170.0f, 5.5f, 0.0f, 100.0f, CComboGauge::GAUGETYPE_COMBO);					//�R���{�Q�[�W�̐���



	return S_OK;
}
//=============================================================================
// �֐����F�`���[�g���A���̏I������
// �֐��̊T�v�F�g�p����e�N�X�`���⃂�f���̔j���A�V�[���̔j��
//=============================================================================
void CTutorial::Uninit()
{
	CCutin::Unload();


	//�v���C���[�̃��f���̔j��
	CPlayer::Unload();

	//�G�̃��f���̓ǂݍ���
	CEnemy::Unload();

	//�p�[�e�B�N���̃e�N�X�`���̔j��
	CParticle::Unload();

	//�G�t�F�N�g�̃e�N�X�`���̔j��
	CEffect::Unload();

	//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	CObject::Unload();

	//�Q�[�WUI�̃e�N�X�`���̔j��
	CUiGauge::Unload();

	//�Q�[�WUI�̃e�N�X�`���̔j��
	CUiGaugeBillBoard::Unload();

	//�R���{�Q�[�W�̃e�N�X�`���̔j��
	CComboGauge::Unload();

	//�R���{�̃e�N�X�`���̔j��
	CCombo::Unload();

	//���b�V���t�B�[���h�̃e�N�X�`���̔j��
	CSceneMeshField::Unload();

	//���b�V���E�H�[���̃e�N�X�`���̔j��
	CSceneMeshWall::Unload();

	//�����e�N�X�`���̔j��
	CNumber::Unload();

	//Ui�̃e�N�X�`���̔j��
	CUi::Unload();




	//�S�V�[���̔j��
	CScene::ReleaseSceneAll();
}

//=============================================================================
// �֐����F�`���[�g���A���̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CTutorial::Update()
{
	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
	{//R�L�[�������ꂽ�Ƃ�
		if (pFade == CFade::FADE_NONE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			CFade::SetFade(CManager::MODE_GAME);//�Q�[���ֈڍs
		}
	}
}

//=============================================================================
// �֐����F�`���[�g���A���̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CTutorial::Draw()
{
}

//=============================================================================
// �֐����F�v���C���[�N���X�̎擾
// �֐��̊T�v�F�v���C���[�N���X��Ԃ�
//=============================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �֐����F�R���{�Q�[�W�N���X�̎擾
// �֐��̊T�v�F�R���{�Q�[�W�N���X��Ԃ�
//=============================================================================
CComboGauge * CTutorial::GetComboGauge(int nCnt)
{
	return m_apComboGauge[nCnt];
}
