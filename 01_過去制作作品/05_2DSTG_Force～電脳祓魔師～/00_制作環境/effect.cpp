//=============================================================================
//
// �G�t�F�N�g�̏���[2D�|���S��] [effect.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "Effect.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGE_LIMIT_MAX_X (1040)	//�X�e�[�W�̍��[
#define STAGE_LIMIT_MIN_X (240)		//�X�e�[�W�̉E�[
#define STAGE_LIMIT_MAX_Y (720)		//�X�e�[�W�̉��[
#define STAGE_LIMIT_MIN_Y (0)		//�X�e�[�W�̏�[

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[MAX_EFFECTTYPE] = {};

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CEffect::CEffect() : CScene2D(4)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}
//=============================================================================
//�e�N�X�`���̃��[�h
//=============================================================================
HRESULT CEffect::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_apTexture[2]);

	return S_OK;

}
//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CEffect::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_EFFECTTYPE; nCntTexture++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}

}
//=============================================================================
//�G�t�F�N�g�̐���
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife, EFFECTTYPE type)
{
	//�G�t�F�N�g�̐���
	CEffect *pEffect;
	pEffect = new CEffect;


	if (pEffect != NULL)
	{//pEffect���󂶂�Ȃ�������
		//�G�t�F�N�g�̏�����
		pEffect->m_type = type;
		pEffect->Init(pos, col, fWidth, fHeight, nLife);

	}

	return pEffect;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife)
{

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_apTexture[m_type]);

	//�|���S���̕��A�����ݒ�
	SetScene2DWidth(fWidth);
	SetScene2DHeight(fHeight);

	//���C�t�̐ݒ�
	m_nLife = nLife;

	//2D�V�[���̏���������
	CScene2D::Init(pos);
	SetColor(col);

	//�G�t�F�N�g�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CEffect::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CEffect::Update(void)
{


	//�G�t�F�N�g�̏��
	D3DXVECTOR3 posEffect = GetPosScene2D();	//�G�t�F�N�g�̈ʒu���
	D3DXVECTOR3 rotEffect = GetRotScene2D();	//�G�t�F�N�g�̊p�x���
	float fLengthEffect = GetLengthScene2D();	//�G�t�F�N�g�̒����擾

	//�|���S���̏k��
	fLengthEffect--;

	//�ʒu���̐ݒ�
	SetPosScene2D(posEffect);

	//�p�x���̐ݒ�
	SetRotScene2D(rotEffect);

	//�����̐ݒ�
	SetLengthScene2D(fLengthEffect);

	//���C�t�̌���
	m_nLife--;


	if (m_nLife <= 0)
	{
		//�I������
		Uninit();
	}
	else if(fLengthEffect <= 0)
	{
		fLengthEffect = 0;
		//�I������
		Uninit();

	}



}

//=============================================================================
//�`�揈��
//=============================================================================
void CEffect::Draw(void)
{

	//2D�V�[���̕`�揈��
	DrawEffect();


}
