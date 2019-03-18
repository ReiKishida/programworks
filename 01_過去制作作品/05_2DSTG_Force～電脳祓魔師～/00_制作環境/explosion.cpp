//=============================================================================
//
// ��������[2D�|���S��] [explosion.cpp]
// Author :�ݓc��(KishidaRei)
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
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_X (0.125f)	//�e�N�X�`�����W_X
#define TEX_Y (1.0f)//�e�N�X�`�����W_Y
#define PATTERN_ANIM (8)	//�e�N�X�`���p�^�[����
#define ANIMATION_SPEED (6)	//�A�j���[�V�����X�s�[�h

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL; //���L�e�N�X�`���̃|�C���^


//=============================================================================
//�R���X�g���N�^
//=============================================================================
CExplosion::CExplosion()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CExplosion::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CExplosion::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//=============================================================================
//�����̐���
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, float fRadius)
{
	//�����̃C���X�^���X����
	CExplosion *pExplosion;
	pExplosion = new CExplosion;
	if (pExplosion != NULL)
	{
		//�����̏���������
		pExplosion->Init(pos, fRadius);


	}
	return pExplosion;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, float fRadius)
{
	//�A�j���[�V�����J�E���^�[
	m_nCounterAnim = 0;

	SetScene2DWidth(fRadius);
	SetScene2DHeight(fRadius);


	//�e�N�X�`���̊��蓖��
	BindTexture(m_pTexture);

	//2D�V�[���̏���������
	CScene2D::Init(pos);
	SetTexture(TEX_X, TEX_Y);

	//�����̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_EXPLOSION);


	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CExplosion::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CExplosion::Update(void)
{
	D3DXVECTOR3 posExplosion = GetPosScene2D();	//�v���C���[�̈ʒu���
	D3DXVECTOR3 rotExplosion = GetRotScene2D();	//�v���C���[�̊p�x���

	m_nCounterAnim++;
	if (m_nCounterAnim % ANIMATION_SPEED == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % PATTERN_ANIM;//�p�^�[��No.�X�V
		//�ʒu���̐ݒ�
		CScene2D::SetPosScene2D(posExplosion);

		//�e�N�X�`���A�j���[�V����
		CScene2D::SetTextureAnim(TEX_X, TEX_Y, m_nPatternAnim);
		if (m_nPatternAnim <= 0)
		{
			Uninit();
		}
	}


}

//=============================================================================
//�`�揈��
//=============================================================================
void CExplosion::Draw(void)
{
	//2D�V�[���̕`�揈��
	CScene2D::Draw();
}
