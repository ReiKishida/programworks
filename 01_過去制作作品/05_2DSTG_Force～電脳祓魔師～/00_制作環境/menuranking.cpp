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
#include "menuranking.h"
#include "rankingnumber.h"
#include "rankinglogo.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CMenuRanking::CMenuRanking()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CMenuRanking::~CMenuRanking()
{

}




//=============================================================================
//����������
//=============================================================================
HRESULT CMenuRanking::Init(void)
{
	//////////////////�K�v�ȃe�N�X�`���̓ǂݍ���///////////////////
	//�w�i�̃e�N�X�`���̓ǂݍ���
	CBg::Load();
	CNumber::Load();
	CRankingLogo::Load();
	//////////////////�K�v�ȏ����̐���///////////////////

	//�w�i�̐���
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 280.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));
	CRankingLogo::Create(D3DXVECTOR3(250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));
	return S_OK;

}
//=============================================================================
//�I������
//=============================================================================
void CMenuRanking::Uninit()
{

	//�S�V�[���̔j��
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
void CMenuRanking::Update()
{

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
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
		{//R�L�[�������ꂽ�Ƃ�
			pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
			CFade::SetFade(CManager::MODE_MENU);
		}
	}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CMenuRanking::Draw()
{


}
