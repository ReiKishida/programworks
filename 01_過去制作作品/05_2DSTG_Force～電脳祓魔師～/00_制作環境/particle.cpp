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
#include "particle.h"
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
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CParticle::CParticle() : CScene2D(5)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}
//=============================================================================
//�e�N�X�`���̃��[�h
//=============================================================================
HRESULT CParticle::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;

}
//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CParticle::Unload(void)
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
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nLife)
{
	//�G�t�F�N�g�̐���
	CParticle *pParticle;
	pParticle = new CParticle;


	if (pParticle != NULL)
	{//pParticle���󂶂�Ȃ�������
		//�F���̐ݒ�
		pParticle->m_col = col;
		//��ނ̐ݒ�
		pParticle->m_type = type;
		switch (pParticle->m_type)
		{
		case PARITCLETYPE_HIT:
			pParticle->m_move = D3DXVECTOR3(sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f,
											cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f,
											0.0f);
			break;
		case PARITCLETYPE_HIT_R:
			pParticle->m_move = D3DXVECTOR3((sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f) * -1.0f,
											(cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 2 + 2) * 1.0f) * -1.0f,
											0.0f);
			break;
		case PARITCLETYPE_EXPLOSION:
			pParticle->m_move = D3DXVECTOR3(sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f,
											cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f,
											0.0f);
			break;
		case PARITCLETYPE_EXPLOSION_R:
			pParticle->m_move = D3DXVECTOR3((sinf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f) * -1.0f,
											(cosf(D3DX_PI * ((float)rand() / 32767.0f))  * (rand() % 8 + 2) * 1.0f) * -1.0f,
											0.0f);
			break;
		}
		//���C�t�̐ݒ�
		pParticle->m_nLife = nLife;
		//���a�̐ݒ�
		pParticle->m_fRadius = (rand()% 40 + 1) * 1.0f;
		//�G�t�F�N�g�̏�����
		pParticle->Init(pos);

	}

	return pParticle;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos)
{

	//�e�N�X�`���̊��蓖��
	CScene2D::BindTexture(m_pTexture);

	//�|���S���̕��A�����ݒ�
	SetScene2DWidth(m_fRadius);
	SetScene2DHeight(m_fRadius);



	//2D�V�[���̏���������
	CScene2D::Init(pos);
	//�F���ݒ�
	SetColor(m_col);
	//���C�t�ݒ�
	SetLife(m_nLife);
	//�G�t�F�N�g�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CParticle::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CParticle::Update(void)
{


	//�G�t�F�N�g�̏��
	D3DXVECTOR3 posParticle = GetPosScene2D();	//�G�t�F�N�g�̈ʒu���
	D3DXVECTOR3 rotEffect = GetRotScene2D();	//�G�t�F�N�g�̊p�x���
	float fLengthEffect = GetLengthScene2D();	//�G�t�F�N�g�̒����擾



	//�|���S���̏k��
	fLengthEffect--;

	//�ʒu�������Z
	posParticle += m_move;

	//�ʒu���̐ݒ�
	SetPosScene2D(posParticle);

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
void CParticle::Draw(void)
{

	//2D�V�[���̕`�揈��
	DrawEffect();


}
