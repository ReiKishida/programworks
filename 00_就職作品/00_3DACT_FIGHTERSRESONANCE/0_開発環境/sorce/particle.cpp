//=============================================================================
//
// �G�t�F�N�g����[3D�|���S��] [particle.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "particle.h"
#include "camera.h"
#include "debugproc.h"
#include "scene_billboard.h"
#include "scene_meshorbit.h"
#include "game.h"
#include "player.h"
#include "tutorial.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticle::m_apTexture[MAXPARTICLE_TEXTURE] = {};
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CParticle::CParticle(int nPriority, OBJTYPE type) : CSceneBillBoard(nPriority, type)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�x
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CParticle::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/effect000.jpg", &m_apTexture[0]);		//��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/smoke000.jpg", &m_apTexture[1]);	// ���n�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/fire.jpg", &m_apTexture[2]);	// ���n�e�N�X�`��

	return S_OK;
}

//=============================================================================
// �֐����F�g�p����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CParticle::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAXPARTICLE_TEXTURE; nCntTexture++)
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
// �֐����F�G�t�F�N�g�̐���
// �֐��̊T�v�F�r���{�[�h�𐶐�����
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLEDRAWTYPE drawtype, PARTICLETYPE type, int nLife)
{
	CParticle  *pParticle;
	pParticle = new CParticle;//�V�[���N���X�̐���

							  //�v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	switch (mode)
	{
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;

	case CManager::MODE_TUTORIAL:
		pPlayer = CTutorial::GetPlayer();
		break;
	}
	//pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();



	if (pParticle != NULL)
	{
		pParticle->m_pos = pos;
		pParticle->m_col = col;
		pParticle->m_Drawtype = drawtype;
		pParticle->m_Type = type;
		switch (pParticle->m_Type)
		{
		case PARTICLETYPE_TEST:
			pParticle->m_move = D3DXVECTOR3((rand() % 2 + 1) * 0.5f,
				(rand() % 5 + 1) * 1.0f,
				0.0f);
			break;
		case PARTICLETYPE_RAND:

			pParticle->m_move.x = (rand() % 3 - 2) * 1.0f;
			pParticle->m_move.y = (rand() % 2 + 1) * 1.0f;
			pParticle->m_move.z = (rand() % 3 - 2) * 1.0f;


			break;
		case PARTICLETYPE_SLIDING:
			pParticle->m_move = D3DXVECTOR3((sinf(PlayerRot.y - D3DX_PI) * (rand() % 3 - 2 * 1.5f)),
				(rand() % 2 + 1) * 1.0f,
				0.0f);

			break;
		case PARTICLETYPE_ATTACK_SHORYUKEN:
			pParticle->m_move = D3DXVECTOR3((sinf(PlayerRot.y - D3DX_PI) * (rand() % 2 + 1 * 1.2f)),
				(rand() % 8 + 1) * -1.0f,
				cosf((PlayerRot.y - D3DX_PI) * (rand() % 3 - 2) * 1.0f) * -1.0f);
			break;
		case PARTICLETYPE_ATTACK_SKILL:
			pParticle->m_move = D3DXVECTOR3((rand() % 2 - 1) * 1.0f,
				(rand() % 3 + 1) * 1.0f,
				0.0f);
			break;
		case PARTICLETYPE_ATTACK_SKILL_ENEMY:
			pParticle->m_move = D3DXVECTOR3((rand() % 2 - 1) * 1.0f,
				(rand() % 3 + 1) * 1.0f,
				0.0f);
			break;
		case PARTICLETYPE_HIT_000:
			pParticle->m_move.x = (rand() % 8 - 5) * 1.3f;
			pParticle->m_move.y = (rand() % 6 - 2) * 1.3f;
			pParticle->m_move.z = (rand() % 6 - 4) * 1.3f;
			break;
		case PARTICLETYPE_HIT_001:
			pParticle->m_move.x = (rand() % 7 - 8) * 1.0f;
			pParticle->m_move.y = (rand() % 4 + 1) * 1.0f;
			pParticle->m_move.z = (rand() % 3 - 4) * 1.0f;
			break;
		case PARTICLETYPE_HIT_002:
			break;
		case PARTICLETYPE_ATTACK_SPINKICK:
			pParticle->m_move = D3DXVECTOR3((sinf(PlayerRot.y - D3DX_PI) * (rand() % 3 - 2 * 1.5f)),
				(rand() % 2 + 1) * 1.0f,
				cosf((PlayerRot.y - D3DX_PI) * (rand() % 3 - 2) * 1.0f) * -1.0f);
			break;
		}
		pParticle->m_nLife = nLife;
		if (pParticle->m_Type == PARTICLETYPE_ATTACK_SKILL)
		{
			pParticle->m_fRadius = (rand() % 10 + 1) * 1.0f;
		}
		else
		{
			pParticle->m_fRadius = (rand() % 20 + 1) * 1.0f;

		}
		pParticle->Init();
	}
	return pParticle;
}


//=============================================================================
// �֐����F�G�t�F�N�g�̏���������
// �֐��̊T�v�F�|���S���̐ݒ�
//=============================================================================
HRESULT CParticle::Init(void)
{

	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//�ʒu���̑��
	SetPos(m_pos);


	if (m_Type == PARTICLETYPE_TEST || m_Type == PARTICLETYPE_HIT_000 || m_Type == PARTICLETYPE_HIT_001 || m_Type == PARTICLETYPE_HIT_002)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[0]);

	}
	else if (m_Type == PARTICLETYPE_RAND || m_Type == PARTICLETYPE_RUN || m_Type == PARTICLETYPE_SLIDING)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[1]);
	}
	else if (m_Type == PARTICLETYPE_ATTACK_SHORYUKEN || m_Type == PARTICLETYPE_ATTACK_SKILL || m_Type == PARTICLETYPE_ATTACK_SKILL_ENEMY)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[2]);
	}
	else if (m_Type == PARTICLETYPE_ATTACK_SPINKICK)
	{
		//�e�N�X�`���̊��蓖��
		BindTexture(m_apTexture[2]);
	}

	//���̐ݒ�
	SetRadius(m_fRadius);
	//�r���{�[�h�̏�����
	CSceneBillBoard::Init();

	//�F���̑��
	SetColor(m_col);

	return S_OK;
}

//=============================================================================
// �֐����F�G�t�F�N�g�̏I������
// �֐��̊T�v�F���g��j������
//=============================================================================
void CParticle::Uninit(void)
{

	//�r���{�[�h�̏I������
	CSceneBillBoard::Uninit();
}

//=============================================================================
// �֐����F�G�t�F�N�g�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CParticle::Update(void)
{

	//�ʒu�������Z
	m_pos += m_move;

	//�ʒu���̐ݒ�
	SetPos(m_pos);

	//�F���̑��
	SetColor(m_col);

	//���C�t�̌���
	m_nLife--;

	//�����x�̌���
	m_col.a -= 0.01f;

	if (m_nLife <= 0)
	{
		//�I������
		Uninit();
	}
	if (m_col.a <= 0)
	{
		//�I������
		Uninit();
	}
}

//=============================================================================
// �֐����F�G�t�F�N�g�̕`�揈��
// �֐��̊T�v�F���Z�Ɖ��Z��ݒ肷��
//=============================================================================
void CParticle::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//�v���C���[�̎擾
	CPlayer *pPlayer = NULL;
	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();
	switch (mode)
	{
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	case CManager::MODE_TUTORIAL:
		pPlayer = CTutorial::GetPlayer();
		break;
	}


	D3DXVECTOR3 playerpos;
	playerpos = pPlayer->GetPos();
	if (playerpos.z > -30.0f && (playerpos.x >= 460.0f && playerpos.x <= 580.0f)
		|| playerpos.z > -30.0f && (playerpos.x >= 3190.0f && playerpos.x <= 3530.0f)
		|| (playerpos.x <= 1400.0f && playerpos.x >= 1200.0f) && (playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		|| (playerpos.x <= 2400.0f && playerpos.x >= 2200.0f) && (playerpos.z >= -168.0f && playerpos.z <= -35.0f))
	{
		switch (m_Drawtype)
		{
		case PARTICLEDRAWTYPE_ADD:

			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);

			// ���C�g�e���󂯂Ȃ�
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			//�r���{�[�h�̕`��
			CSceneBillBoard::Draw();

			// Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// ���C�g�e���󂯂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;

		case PARTICLEDRAWTYPE_DEST:
			// ���Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			//Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
			// ���C�g�e���󂯂Ȃ�
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			CSceneBillBoard::Draw();

			// Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			// ���C�g�e���󂯂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;
		default:
			CSceneBillBoard::Draw();
			break;
		}
	}
	else
	{

		switch (m_Drawtype)
		{
		case PARTICLEDRAWTYPE_ADD:

			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


			//Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// ���C�g�e���󂯂Ȃ�
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			//�r���{�[�h�̕`��
			CSceneBillBoard::Draw();

			// Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// ���C�g�e���󂯂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;

		case PARTICLEDRAWTYPE_DEST:
			// ���Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			//Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			// ���C�g�e���󂯂Ȃ�
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			CSceneBillBoard::Draw();

			// Z�o�b�t�@
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			// ���C�g�e���󂯂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;
		default:
			CSceneBillBoard::Draw();
			break;
		}
	}


}

