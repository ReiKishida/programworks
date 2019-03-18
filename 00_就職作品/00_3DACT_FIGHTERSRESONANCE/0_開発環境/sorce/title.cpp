//=============================================================================
//
// �^�C�g������ [title.cpp]
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
#include "fade.h"
#include "object.h"
#include "scene_meshfield.h"
#include "titlelogo.h"
//****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CTitleLogo *CTitle::m_pTitleLogo = NULL;
CObject *CTitle::m_pObject = NULL;				//�I�u�W�F�N�g
CSceneMeshField *CTitle::m_pMeshField = NULL;			//���b�V���t�B�[���h


//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CTitle::~CTitle()
{

}




//=============================================================================
// �֐����F�^�C�g���̏���������
// �֐��̊T�v�F�g�p����e�N�X�`���⃂�f���̓ǂݍ���
//=============================================================================
HRESULT CTitle::Init(void)
{
	//�^�C�g�����S�̃e�N�X�`���̓ǂݍ���
	CTitleLogo::Load();

	//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	CObject::Load();

	//���b�V���t�B�[���h�̃e�N�X�`���̓ǂݍ���
	CSceneMeshField::Load();


	//�^�C�g�����S�̐���
	CTitleLogo::Create();

	//���f���̐���
	CObject::LoadModelText();

	//���b�V���t�B�[���h�̐���
	CSceneMeshField::Create(D3DXVECTOR3(1850.0f, -0.5f, -200.0f), 45, 4, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);		//�ŏ��̏�
	CSceneMeshField::Create(D3DXVECTOR3(3900.0f, -0.5f, -1900.0f), 10, 30, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);	//���̏�
	CSceneMeshField::Create(D3DXVECTOR3(4100.0f, -0.5f, -3800.0f), 9, 10, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);	//�{�X�̏�
	CSceneMeshField::Create(D3DXVECTOR3(2000.0f, -0.4f, -80.0f), 200, 1, 30.0f, 30.0f, CSceneMeshField::FIELDTYPE_YELLOW);

	return S_OK;

}
//=============================================================================
// �֐����F�^�C�g���̏I������
// �֐��̊T�v�F�g�p����e�N�X�`���⃂�f���̔j���A�V�[���̔j��
//=============================================================================
void CTitle::Uninit()
{
	//�^�C�g�����S�̃e�N�X�`���̔j��
	CTitleLogo::Unload();

	//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	CObject::Unload();

	//���b�V���t�B�[���h�̃e�N�X�`���̓ǂݍ���
	CSceneMeshField::Unload();

	//�V�[���̔j��
	CScene::ReleaseSceneAll();


}
//=============================================================================
// �֐����F�^�C�g���̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CTitle::Update()
{
	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();





}

//=============================================================================
// �֐����F�^�C�g���̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CTitle::Draw()
{


}
