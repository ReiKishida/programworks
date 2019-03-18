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
#include "menuscene.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CMenu::CMenu()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CMenu::~CMenu()
{

}




//=============================================================================
//����������
//=============================================================================
HRESULT CMenu::Init(void)
{
	//////////////////�K�v�ȃe�N�X�`���̓ǂݍ���///////////////////
	//�w�i�̃e�N�X�`���̓ǂݍ���
	CBg::Load();
	CMenuScene::Load();
	//////////////////�K�v�ȏ����̐���///////////////////

	//�w�i�̐���
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CMenuScene::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	return S_OK;

}
//=============================================================================
//�I������
//=============================================================================
void CMenu::Uninit()
{


	CScene::ReleaseSceneAll();

	//�ǂݍ��񂾃e�N�X�`���̔j��
	//�w�i�̃e�N�X�`���̔j��
	CMenuScene::Unload();
	CBg::Unload();



}
//=============================================================================
//�X�V����
//=============================================================================
void CMenu::Update()
{

	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

}

//=============================================================================
//�`�揈��
//=============================================================================
void CMenu::Draw()
{


}
