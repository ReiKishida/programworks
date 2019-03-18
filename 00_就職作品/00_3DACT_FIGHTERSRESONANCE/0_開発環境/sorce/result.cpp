//=============================================================================
//
// ���U���g�̏��� [result.h]
// Author : �ݓc��(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "result.h"
#include "DebugProc.h"
#include "number.h"
#include "fade.h"
#include "scene_meshfield.h"
#include "resultlogo.h"
#include "object.h"
#include "sound.h"
#include "timer.h"
#include "score.h"
#include "combo.h"
#include "comboscore.h"
#include "damagescore.h"
#include "finalscore.h"
#include "clearrank.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CUiResult *CResult::m_pUiResult = NULL;					//���U���g
CObject *CResult::m_pObject = NULL;						//�I�u�W�F�N�g
CSceneMeshField *CResult::m_pMeshField = NULL;			//���b�V���t�B�[���h
int CResult::m_nCntState = 0;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
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
	//���U���g��UI�e�N�X�`���̓ǂݍ���
	CResultLogo::Load();

	//�����e�N�X�`���̓ǂݍ���
	CNumber::Load();

	//�R���{�\�L�̃e�N�X�`���̓ǂݍ���
	CCombo::Load();

	//�N���A�����N�̃e�N�X�`���̓ǂݍ���
	CClearRank::Load();

	//�X�R�A�̐���
	CScore::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), CScore::SCORETYPE_RESULT);

	//�^�C�}�[
	CResultTimer::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT/2 - 90.0f,0.0f), CTimer::TIMEMODE_RESULT);

	//�R���{�X�R�A
	CComboScore::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT / 2 + 30.0f, 0.0f));

	//�_���[�W�X�R�A
	CDamageScore::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f));

	//���U���g��UI�e�N�X�`���̐���
	CResultLogo::Create();

	m_nCntState = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	//���U���g��UI�e�N�X�`���̓ǂݍ���
	CResultLogo::Unload();

	//�����e�N�X�`���̓ǂݍ���
	CNumber::Unload();

	//�R���{�\�L�̃e�N�X�`���̓ǂݍ���
	CCombo::Unload();

	//�N���A�����N�̃e�N�X�`���̓ǂݍ���
	CClearRank::Unload();


	//�V�[���̔j��
	CScene::ReleaseSceneAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	m_nCntState++;


	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();


	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (m_nCntState == 20)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);
		//�X�R�A�{�[�i�X
		CClearRank::Create(D3DXVECTOR3(520.0f, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), CClearRank::RANKMODE_SCORE);
	}
	if (m_nCntState == 40)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);
		//�^�C���{�[�i�X
		CClearRank::Create(D3DXVECTOR3(450.0f, SCREEN_HEIGHT / 2 - 90.0f, 0.0f), CClearRank::RANKMODE_TIME);
	}
	if (m_nCntState == 60)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);

		//�R���{�{�[�i�X
		CClearRank::Create(D3DXVECTOR3(360.0f, SCREEN_HEIGHT / 2 + 30.0f, 0.0f), CClearRank::RANKMODE_COMBO);
	}
	if (m_nCntState == 80)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);
		//�_���[�W�{�[�i�X
		CClearRank::Create(D3DXVECTOR3(360.0f, SCREEN_HEIGHT / 2 + 140.0f, 0.0f), CClearRank::RANKMODE_DAMAGE);
	}
	if (m_nCntState == 120)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);
		//�ŏI�X�R�A
		CFinalScore::Create(D3DXVECTOR3(280.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f));
	}
	if (m_nCntState == 180)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);
		//�ŏI�����N
		CClearRank::Create(D3DXVECTOR3(900.0f, SCREEN_HEIGHT / 2 + 270.0f, 0.0f), CClearRank::RANKMODE_FINAL);
		CResultLogo::SwitchEnable();
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
}