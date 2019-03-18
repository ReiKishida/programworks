//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : �ݓc��(KishidaRei)
//=============================================================================
#include "main.h"				//���C��
#include "scene.h"				//�V�[��
#include "renderer.h"			//�����_�����O
#include "scene2D.h"			//�V�[��2D
#include "manager.h"			//�}�l�[�W���[
#include "input.h"				//����
#include "number.h"				// �i���o�[
#include "game.h"				//�Q�[��
#include "ranking.h"			//�����L���O
#include "DebugProc.h"			//�f�o�b�O�\��
#include "fade.h"				//�t�F�[�h
#include "scene_meshfield.h"
#include "rankinglogo.h"
#include "object.h"
#include "sound.h"
#include "rankingnumber.h"
//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CUiRanking *CRanking::m_pUiRanking = NULL;
CObject *CRanking::m_pObject = NULL;				//�I�u�W�F�N�g
CSceneMeshField *CRanking::m_pMeshField = NULL;			//���b�V���t�B�[���h
int CRanking::m_nJumpTitleFrame = 0;//�^�C�g����ʂ֑J�ڂ���Ƃ��̃t���[���J�E���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{	//���������Ȃ�
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	//�����̃e�N�X�`���̓ǂݍ���
	CNumber::Load();

	//�����L���O���S�̃e�N�X�`���̓ǂݍ���
	CRankingLogo::Load();


	//�����L���O�̐���
	CRankingLogo::Create();

	//�����L���O�̐����𐶐�
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 280.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRanking::Uninit(void)
{
	//�����̃e�N�X�`���̔j��
	CNumber::Unload();

	//�����L���O���S�̃e�N�X�`���̔j��
	CRankingLogo::Unload();

	//�V�[���̔j��
	CScene::ReleaseSceneAll();


}

//=============================================================================
// �X�V����
//=============================================================================
void CRanking::Update(void)
{
	m_nJumpTitleFrame++;

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	//{//�G���^�[�L�[�������ꂽ�Ƃ�

	//	if (pFade == CFade::FADE_NONE)
	//	{
	//		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	//		CFade::SetFade(CManager::MODE_TITLE);//�����L���O�ֈڍs
	//	}

	//}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{
}