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
#include "inputx.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CResult *CResult::m_pResult = NULL;
CManager *CResult::m_pManager = NULL;
JoyState g_JoyStateResult;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}




//=============================================================================
//����������
//=============================================================================
HRESULT CResult::Init(void)
{
	//�Q�[���p�b�h�̏��
	g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//////////////////�K�v�ȃe�N�X�`���̓ǂݍ���///////////////////
	//�w�i�̃e�N�X�`���̓ǂݍ���
	CBg::Load();
	//�����̃e�N�X�`���̓ǂݍ���
	CNumber::Load();
	//////////////////�K�v�ȏ����̐���///////////////////

	//�w�i�̐���
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -300.0f, SCREEN_HEIGHT / 2- 100.0f, 0.0f), CScore::SCORETYPE_RESULT);

	return S_OK;

}
//=============================================================================
//�I������
//=============================================================================
void CResult::Uninit()
{



	//�ǂݍ��񂾃e�N�X�`���̔j��
	//�w�i�̃e�N�X�`���̔j��
	CBg::Unload();
	CNumber::Unload();


	CScene::ReleaseSceneAll();

}
//=============================================================================
//�X�V����
//=============================================================================
void CResult::Update()
{

	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//R�L�[�������ꂽ�Ƃ�
			if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
				//CManager::SetMode(CManager::MODE_TITLE);
				CFade::SetFade(CManager::MODE_RANKING);
			}
			g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CResult::Draw()
{


}
