//=============================================================================
//
// �G�t�F�N�g����[3D�|���S��] [effect.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "effect.h"
#include "camera.h"
#include "debugproc.h"
#include "scene_billboard.h"
#include "effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PATTERN_ANIM (5)	//�e�N�X�`���p�^�[����
#define PATTERN_X (0.2f)
#define ANIMATION_SPEED (120)	//�A�j���[�V�����X�s�[�h
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[3] = {}; //���L�e�N�X�`���̃|�C���^

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CEffect::CEffect(int nPriority, OBJTYPE type) : CSceneBillBoardAnim(nPriority, type)
{
	//m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	//m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CEffect::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/hit0.png", &m_apTexture[0]);		//��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/hit1.png", &m_apTexture[1]);		//��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/hit2.png", &m_apTexture[2]);		//��

	return S_OK;
}

//=============================================================================
// �֐����F�g�p����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CEffect::Unload(void)
{
	for (int nCntTex = 0; nCntTex < 3; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
// �֐����F�G�t�F�N�g�̐���
// �֐��̊T�v�F�G�t�F�N�g�𐶐�����
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, float fRadius, int nLife, EFFECTTYPE type, EFFECTMODE mode)
{
	CEffect  *pEffect;
	pEffect = new CEffect;//�V�[���N���X�̐���
	if (pEffect != NULL)
	{
		pEffect->m_pos = pos;
		pEffect->m_fRadius = fRadius;
		pEffect->m_nLife = nLife;
		pEffect->m_type = type;
		pEffect->m_mode = mode;
		pEffect->Init();

	}
	return pEffect;
}


//=============================================================================
// �֐����F�G�t�F�N�g�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CEffect::Init(void)
{

	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();



	switch (m_type)
	{
	case EFFECTTYPE_SMALL:
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[0]);
		break;
	case EFFECTTYPE_MEDIUM:
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[1]);
		break;
	case EFFECTTYPE_LARGE:
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[2]);
		break;
	}

	//���a�̐ݒ�
	SetRadius(m_fRadius);

	//�G�t�F�N�g�̏�����
	CSceneBillBoardAnim::Init();

	//�e�N�X�`���̍��W�̐ݒ�
	SetTexture(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �֐����F�G�t�F�N�g�̏I������
// �֐��̊T�v�F���g��j������
//=============================================================================
void CEffect::Uninit(void)
{
	//�G�t�F�N�g�̏I������
	CSceneBillBoardAnim::Uninit();
}

//=============================================================================
// �֐����F�G�t�F�N�g�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CEffect::Update(void)
{


	//�ʒu���̐ݒ�
	SetPos(m_pos);

	SetRadius(m_fRadius);

	CSceneBillBoardAnim::Update();
	m_nLife--;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		Uninit();
	}

	if (m_mode == EFFECTMODE_SPREAD)
	{
		switch (m_type)
		{
		case EFFECTTYPE_SMALL:
			m_fRadius += 15.0f;
			break;
		case EFFECTTYPE_MEDIUM:
			m_fRadius += 20.0f;
			break;
		case EFFECTTYPE_LARGE:
			m_fRadius += 25.0f;
			break;
		}
	}

	if (m_mode == EFFECTMODE_SHRINKING)
	{
		switch (m_type)
		{
		case EFFECTTYPE_SMALL:
			m_fRadius -= 15.0f;
			break;
		case EFFECTTYPE_MEDIUM:
			m_fRadius -= 20.0f;
			break;
		case EFFECTTYPE_LARGE:
			m_fRadius -= 5.0f;
			break;
		}
	}

	//CDebugProc::Print("m_nLife : %d\n", m_nLife);
	//CDebugProc::Print("m_fRadius : %.1f\n", m_fRadius);

}

//=============================================================================
// �֐����F�G�t�F�N�g�̕`�揈��
// �֐��̊T�v�F�s���ς��ăJ��������������悤��
//=============================================================================
void CEffect::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	CSceneBillBoardAnim::Draw();


}

