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
#include "titlelogo.h"
#include "fade.h"
#include "pause.h"
#include "inputx.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CTitle *CTitle::m_pTitle = NULL;
CScene *CTitle::m_pScene = NULL;
CBg *CTitle::m_pBg = NULL;
CTitleLogo *CTitle::m_pTitleLogo = NULL;
CManager *CTitle::m_pManager = NULL;
int CTitle::m_nCntFlame = 0;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}




//=============================================================================
//����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	if (CRenderer::GetPause() == true)
	{
		CRenderer::SetPause();
		CPause::SetSelect();
	}

	//////////////////�K�v�ȃe�N�X�`���̓ǂݍ���///////////////////
	//�w�i�̃e�N�X�`���̓ǂݍ���
	m_pBg->Load();
	m_pTitleLogo->Load();
	//////////////////�K�v�ȏ����̐���///////////////////

	//�w�i�̐���
	m_pBg->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pTitleLogo->Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150.0f, 0.0f));
	return S_OK;

}
//=============================================================================
//�I������
//=============================================================================
void CTitle::Uninit()
{

		//�S�V�[���̔j��
		m_pScene->ReleaseSceneAll();

		//�ǂݍ��񂾃e�N�X�`���̔j��
		//�w�i�̃e�N�X�`���̔j��
		m_pBg->Unload();
		m_pTitleLogo->Unload();


}
//=============================================================================
//�X�V����
//=============================================================================
void CTitle::Update()
{
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();



	m_nCntFlame++;
	if (m_nCntFlame % 300 == 0)
	{
		if (pFade == CFade::FADE_NONE)
		{

			CFade::SetFade(CManager::MODE_RANKING);//�����L���O�ֈڍs
		}
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CTitle::Draw()
{


}
