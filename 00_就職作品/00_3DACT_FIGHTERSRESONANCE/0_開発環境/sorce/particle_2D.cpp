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
#include "enemy.h"
#include "player.h"
#include "particle_2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticle2D::m_pTexture = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CParticle2D::CParticle2D(int nPriority , OBJTYPE type) : CScene2D(nPriority, type)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CParticle2D::~CParticle2D()
{

}
//=============================================================================
//�e�N�X�`���̃��[�h
//=============================================================================
HRESULT CParticle2D::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect001.jpg", &m_pTexture);

	return S_OK;

}
//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CParticle2D::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
//�G�t�F�N�g�̐���
//=============================================================================
CParticle2D *CParticle2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nLife)
{
	//�G�t�F�N�g�̐���
	CParticle2D *pParticle;
	pParticle = new CParticle2D;


	if (pParticle != NULL)
	{//pParticle���󂶂�Ȃ�������
		pParticle->m_pos = pos;
		//�F���̐ݒ�
		pParticle->m_col = col;
		//��ނ̐ݒ�
		pParticle->m_type = type;
		switch (pParticle->m_type)
		{
		case PARITCLETYPE_FLAME:

			pParticle->m_move.x = (rand() % 4 + 1) * 1.0f;
			pParticle->m_move.y = (rand() % 5 + 1) * 1.0f;
			pParticle->m_move.z = 0.0f;

			break;
		}
		//���C�t�̐ݒ�
		pParticle->m_nLife = nLife;
		//���a�̐ݒ�
		pParticle->m_fRadius = (rand()% 10 + 1) * 1.0f;
		//�G�t�F�N�g�̏�����
		pParticle->Init();

	}

	return pParticle;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CParticle2D::Init(void)
{

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture);

	//�|���S���̕��A�����ݒ�
	SetPolygonWidth(m_fRadius);
	SetPolygonHeight(m_fRadius);


	//2D�V�[���̏���������
	CScene2D::Init();
	//�ʒu���̑��
	SetPosPolygon(m_pos);

	//�F���ݒ�
	SetColor(m_col);

	//�G�t�F�N�g�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CParticle2D::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CParticle2D::Update(void)
{


	//�G�t�F�N�g�̏��
	D3DXVECTOR3 posParticle = GetPosPolygon();	//�G�t�F�N�g�̈ʒu���
	D3DXVECTOR3 rotEffect = GetRotPolygon();	//�G�t�F�N�g�̊p�x���
	float fLengthEffect = GetLengthPolygon();	//�G�t�F�N�g�̒����擾



	//�|���S���̏k��
	fLengthEffect--;

	//�ʒu�������Z
	posParticle += m_move;

	//�ʒu���̐ݒ�
	SetPosPolygon(posParticle);

	//�p�x���̐ݒ�
	SetRotPolygon(rotEffect);

	//�����̐ݒ�
	SetLengthPolygon(fLengthEffect);

	//���C�t�̌���
	m_nLife--;


	if (m_nLife <= 0)
	{
		//�I������
		Uninit();
	}
	else if (fLengthEffect <= 0)
	{
		fLengthEffect = 0;
		//�I������
		Uninit();

	}



}

//=============================================================================
//�`�揈��
//=============================================================================
void CParticle2D::Draw(void)
{

	//2D�V�[���̕`�揈��
	DrawEffect();


}
