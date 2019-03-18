//=============================================================================
//
// �V�[��2D����[3D�|���S��] [scene3D.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "pr.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_WIDTH (30) //�|���S����X�̑傫��
#define POLYGON_HEIGHT (30) //�|���S����Y�̑傫��
#define ANIMATION_SPEED (8)	//�A�j���[�V�����̃X�s�[�h
#define TEX_X (0.25f)	//�e�N�X�`�����W_X
#define TEX_Y (1.0f)	//�e�N�X�`�����W_Y

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPr::m_apTexture[MAX_TEXTURE_PR] = {};

//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CPr::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/board_0.png", &m_apTexture[0]);		//�d���f���i����镶���j
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/board_1.png", &m_apTexture[1]);		//�d���f���i�؂芷��1�j
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/board_2.png", &m_apTexture[2]);		//�d���f���i�؂芷��2�j
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_0.png",	 &m_apTexture[3]);		//�L���i0�j
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_1.png", &m_apTexture[4]);			//�L���i1�j
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_2.png", &m_apTexture[5]);			//�L���i2�j
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_3.png", &m_apTexture[6]);			//�L���i3�j


	return S_OK;
}

//=============================================================================
// �֐����F�g�p�����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CPr::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_PR; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CPr::CPr(int nPriority, OBJTYPE type) : CScene3D(nPriority, type)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CPr::~CPr()
{

}
//=============================================================================
//3D�V�[���̐���
//=============================================================================
CPr *CPr::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, PRTYPE type)
{
	CPr  *pPr = NULL;
	pPr = new CPr;//�V�[���N���X�̐���
	if (pPr != NULL)
	{
		pPr->m_pos = pos;
		pPr->m_rot = rot;
		pPr->m_fWidth = fWidth;
		pPr->m_fHeight = fHeight;
		pPr->m_type = type;
	//	pPr->m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pPr->Init();

	}
	return pPr;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CPr::Init(void)
{
	//���E�����̐ݒ�
	SetWidth(m_fWidth);
	SetHeight(m_fHeight);


	//�V�[��3D�̏�����
	CScene3D::Init();
	//�e�N�X�`���̊��蓖��
	BindTexture(m_apTexture[m_type]);

	//�ʒu���̑��
	SetPosPolygon(m_pos);

	SetRotPolygon(m_rot);
	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CPr::Uninit(void)
{
	//�V�[��3D�̏I��
	CScene3D::Uninit();

}

//=============================================================================
//�X�V����
//=============================================================================
void CPr::Update(void)
{
	//�ʒu���̎擾
	D3DXVECTOR3 pos;
	pos = GetPosPolygon();

	//�v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerpos;
	playerpos = pPlayer->GetPos();

	//�t���[�����̉��Z
	m_nCntFrame++;

	if (m_type == PRTYPE_BOARD_ADVERTISE_0)
	{
		if (m_nCntFrame % 120 == 0)
		{
			m_nRand = rand() % 3;
			switch (m_nRand)
			{
			case 0:
				BindTexture(m_apTexture[3]);
				break;
			case 1:
				BindTexture(m_apTexture[4]);
				break;
			case 2:
				BindTexture(m_apTexture[5]);
				break;
			case 3:
				BindTexture(m_apTexture[6]);
				break;
			}
		}

		if (playerpos.x <= 1400.0f && playerpos.x >= 1200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else if (playerpos.x <= 2400.0f && playerpos.x >= 2200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		SetColor(m_col);
	}


	if (m_type == PRTYPE_BOARD_SWITCH_0)
	{
		if (m_nCntFrame % 120 == 0)
		{
			m_nRand = rand() % 2;
			switch (m_nRand)
			{
			case 0:
				BindTexture(m_apTexture[1]);
				break;
			case 1:
				BindTexture(m_apTexture[2]);
				break;
			}
		}
		if (playerpos.z > -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		SetColor(m_col);
	}

	if (m_type == PRTYPE_BOARD_STREAM)
	{
		SetTexture_All(1.0f + (m_nCntFrame *0.001f), 0.0f + (m_nCntFrame *0.001f),0.0f, 1.0f);
		if (playerpos.z > -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		SetColor(m_col);
	}




	SetRotPolygon(m_rot);

	SetPosPolygon(pos);


}

//=============================================================================
//�`�揈��
//=============================================================================
void CPr::Draw(void)
{
	//�V�[��3D�̕`��
	CScene3D::Draw();
}


