//=============================================================================
//
// �V�[������[2D�|���S��] [scene2D.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "cutin.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_WIDTH (50.0f)
#define POLYGON_HEIGHT (50.0f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		CCutin::m_apTexture[MAX_TEXTURE_CUTIN] = {};				// �e�N�X�`���ւ̃|�C���^
CScene2D *CCutin::m_apScene2D[MAX_CUTIN] = {};
//int CCutin::m_nCntFlame = 0;
CCutin::CUTINMODE CCutin::m_mode = CUTINMODE_NONE;
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CCutin::CCutin(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	m_fWidth = 0.0f;	//�|���S���̕�
	m_fHeight = 0.0f;	//�|���S���̍���
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CCutin::~CCutin()
{

}


//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CCutin::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/0_flame1.png",	&m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/1_flame1_E.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/2_flame0.png",   &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/3_flame0_E.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/4_cutin_0.png", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/5_cutin_1.png", &m_apTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/6_skill_0.png", &m_apTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/7_skill_0.png", &m_apTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/name.png", &m_apTexture[8]);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CCutin::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_CUTIN; nCntTex++)
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
// �֐����F2D�V�[���̐���
// �֐��̊T�v�F2D�V�[���𐶐�����
//=============================================================================
CCutin *CCutin::Create(CUTINMODE mode)
{
	CCutin  *pCutin;
	pCutin = new CCutin;//�V�[���N���X�̐���
	if (pCutin != NULL)
	{

		pCutin->m_mode = mode;
		pCutin->Init();
	}
	return pCutin;
}

//=============================================================================
// �֐����F2D�V�[���̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CCutin::Init(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_CUTIN; nCntScene2D++)
	{
		switch (nCntScene2D)
		{
		case CUTCOUNT_FRAME:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH /2 -50.0f, SCREEN_HEIGHT / 2 -50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME_PLAYER]);
				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME_ENEMY]);

				break;
			}
			break;
		case CUTCOUNT_FRAME2:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 -50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME2_PLAYER]);

				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME2_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME2_ENEMY]);

				break;
			}
			break;
		case CUTCOUNT_SILHOUETTE:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 , 0.0f), 320.0f, 600.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SILHOUETTE_PLAYER]);

				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f), 320.0f, 600.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SILHOUETTE_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f), 320.0f, 600.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SILHOUETTE_ENEMY]);

				break;
			}
			break;
		case CUTCOUNT_SKILL:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, 620.0f, 0.0f), 400.0f, 200.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SKILL_PLAYER]);
				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, 620.0f, 0.0f), 400.0f, 200.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SKILL_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, 620.0f, 0.0f), 400.0f, 200.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_NAME_ENEMY]);

				break;
			}
			break;
		}
	}
	switch (m_mode)
	{
	case CUTINMODE_PLAYER:
		m_nCntflame = 115;

		break;

	case CUTINMODE_ENEMY:
		m_nCntflame = 60;

		break;
	case CUTINMODE_ENEMYAPPEAR:
		m_nCntflame = 170;

		break;
	}
	SetObjType(OBJTYPE_CUTIN);

	return S_OK;
}

//=============================================================================
// �֐����F2D�V�[���̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�̔j���A���g�̔j��
//=============================================================================
void CCutin::Uninit(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_CUTIN; nCntScene2D++)
	{
		if (m_apScene2D[nCntScene2D] != NULL)
		{
			//�I������
			m_apScene2D[nCntScene2D]->Uninit();
			//delete m_apScene2D[nCntScene2D];
			m_apScene2D[nCntScene2D] = NULL;
		}
	}

	Release();
}

//=============================================================================
// �֐����F2D�V�[���̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CCutin::Update(void)
{
	m_nCntflame--;
	for (int nCntScene2D = 0; nCntScene2D < MAX_CUTIN; nCntScene2D++)
	{
		D3DXVECTOR3 pos[MAX_CUTIN];
		//�ʒu���̎擾
		if (m_apScene2D[nCntScene2D] != NULL)
		{

			pos[nCntScene2D] = m_apScene2D[nCntScene2D]->GetPosPolygon();

			pos[CUTCOUNT_FRAME].x += 10.0f;
			pos[CUTCOUNT_FRAME2].x += 3.0f;
			pos[CUTCOUNT_SILHOUETTE].x += 8.0f;
			pos[CUTCOUNT_SKILL].x += 12.0f;

			if (pos[CUTCOUNT_FRAME].x >= SCREEN_WIDTH / 2 + 100.0f)
			{//�J�b�g�C���t���[��
				pos[CUTCOUNT_FRAME].x = SCREEN_WIDTH / 2 + 100.0f;
			}

			if (pos[CUTCOUNT_FRAME2].x >= SCREEN_WIDTH / 2 + 100.0f)
			{//�J�b�g�C���t���[��2
				pos[CUTCOUNT_FRAME2].x = SCREEN_WIDTH / 2 + 100.0f;
			}

			if (pos[CUTCOUNT_SILHOUETTE].x >= 200.0f)
			{//�L�����N�^�[�̃V���G�b�g
				pos[CUTCOUNT_SILHOUETTE].x = 200.0f;
			}
			if (pos[CUTCOUNT_SKILL].x >= 280.0f)
			{//�X�L����
				pos[CUTCOUNT_SKILL].x = 280.0f;
			}

			//�ʒu���̑��
			m_apScene2D[nCntScene2D]->SetPosPolygon(pos[nCntScene2D]);


			if (m_nCntflame <= 0)
			{
				if (m_apScene2D[nCntScene2D] != NULL)
				{
					//�I������
					m_apScene2D[nCntScene2D]->Uninit();
					//delete m_apScene2D[nCntScene2D];
					m_apScene2D[nCntScene2D] = NULL;
					Release();
				}
				m_nCntflame = 0;
			}

		}
	}



}

//=============================================================================
// �֐����F2D�V�[���̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CCutin::Draw(void)
{

}
