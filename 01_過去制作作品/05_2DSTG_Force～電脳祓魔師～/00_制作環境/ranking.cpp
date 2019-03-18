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
#include "ranking.h"
#include "rankingnumber.h"
#include "rankinglogo.h"
#include "inputx.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CRanking::m_nCntFlame = 0;//�t���[���̃J�E���g
JoyState g_JoyStateRanking;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}




//=============================================================================
//����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	//�Q�[���p�b�h�̏��
	g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	//////////////////�K�v�ȃe�N�X�`���̓ǂݍ���///////////////////
	//�w�i�̃e�N�X�`���̓ǂݍ���
	CBg::Load();
	CNumber::Load();
	CRankingLogo::Load();
	//////////////////�K�v�ȏ����̐���///////////////////

	//�w�i�̐���
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -280.0f, SCREEN_HEIGHT / 2 -250.0f, 0.0f));
	CRankingLogo::Create(D3DXVECTOR3(250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));
	return S_OK;

}
//=============================================================================
//�I������
//=============================================================================
void CRanking::Uninit()
{


	CScene::ReleaseSceneAll();

	//�ǂݍ��񂾃e�N�X�`���̔j��
	//�w�i�̃e�N�X�`���̔j��
	CNumber::Unload();
	CBg::Unload();
	CRankingLogo::Unload();




}
//=============================================================================
//�X�V����
//=============================================================================
void CRanking::Update()
{

	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//R�L�[�������ꂽ�Ƃ�
			if (g_JoyStateRanking.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
				//CManager::SetMode(CManager::MODE_TITLE);
				CFade::SetFade(CManager::MODE_TITLE);
			}
			g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	m_nCntFlame++;
	if (m_nCntFlame % 300 == 0)
	{
		if (pFade == CFade::FADE_NONE)
		{

			CFade::SetFade(CManager::MODE_TITLE);//�����L���O�ֈڍs
		}
	}

	if (g_JoyStateRanking.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateRanking.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CRanking::Draw()
{


}
