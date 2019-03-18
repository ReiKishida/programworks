//=============================================================================
//
// �I�u�W�F�N�g�̔z�u�̏��� [object.cpp]
// Author : �ݓc��(Kishida Rei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "sceneX.h"
#include "input.h"
#include "debugproc.h"
#include "scene_meshfield.h"
#include "game.h"
#include"tutorial.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "collision.h"
#include "particle.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_FILE_NAME			("data/TEXT/object_data.txt")		// X�t�@�C���̃A�h���X
#define MODEL_FILE_NAME_TUTO			("data/TEXT/object_data_tuto.txt")		// X�t�@�C���̃A�h���X

#define MAX_CHAR (256)	// �������̍ő吔

//=====================================
// �D�揇��
//=====================================
#define ENEMY_PRIORITY (2)						//�G�̗D�揇��
#define COLLISION_PRIORITY (6)					//�����蔻��̗D�揇��
#define WALL_PRIORITY (5)						//�ǂ̗D�揇��
#define OBJECT_PRIORITY (4)						//�I�u�W�F�N�g�̗D�揇��
#define CYLINDER_PRIORITY (3)					//�V�����_�[�̗D�揇��


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
char *CObject::m_pModelName[MAXMODEL_TYPE] = {};
int	 CObject::m_nMaxModelNum = 0;
LPDIRECT3DTEXTURE9 CObject::m_apTexture[MAX_TEXTURE_OBJ] = {}; //���L�e�N�X�`���̃|�C���^

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE type) : CSceneX(nPriority, type)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu�̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu�̈ړ��ʂ̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �����̏�����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړI�̌�����������
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CObject::~CObject()
{

}


//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CObject::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/dram.png", &m_apTexture[0]);				// �h������
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/dust.png", &m_apTexture[1]);				// �S�~��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/keepout.png", &m_apTexture[2]);				// ��~��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/telephone.png", &m_apTexture[3]);			// �d�b
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/piller.png", &m_apTexture[4]);				// ��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/stair.png", &m_apTexture[5]);				// �K�i
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/board01.png", &m_apTexture[6]);				// �Ŕ�01
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/board02.png", &m_apTexture[7]);				// �Ŕ�02
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/station_chair.png", &m_apTexture[8]);		// �֎q
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/ticket_gate.png", &m_apTexture[9]);			// ���D
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/train_chair.png", &m_apTexture[10]);		// �d�ԓ��֎q
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/tex.jpg", &m_apTexture[11]);				// �d�ԏ�
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/iron.jpg", &m_apTexture[12]);				// �S
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/train.png", &m_apTexture[13]);				// �d��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/jihanki.png", &m_apTexture[14]);			// ���̋@
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/object/denkou_000.png", &m_apTexture[15]);			// �d���f����

	return S_OK;

}

//=============================================================================
// �֐����F�g�p�����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CObject::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE_OBJ; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}


//=============================================================================
// �֐����F���f���̐���
// �֐��̊T�v�F���f���𐶐�����
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type)
{
	CObject  *pModel;
	pModel = new CObject;//�V�[���N���X�̐���
	if (pModel != NULL)
	{
		pModel->m_nTypeModel = type;
		pModel->m_pos = pos;
		pModel->m_rot = rot;
		pModel->m_fHeight = 100.0f;
		//������
		pModel->Init();
	}
	return pModel;
}

//=============================================================================
// �֐����F���f���̏���������
// �֐��̊T�v�F���f���̏�����
//=============================================================================
HRESULT CObject::Init(void)
{
	//�ʒu���̑��
	SetPos(m_pos);

	//�p�x���̑��
	SetRot(m_rot);

	//�t�@�C�����̊��蓖��
	BindFileName(m_pModelName[m_nTypeModel]);

	//����������
	CSceneX::Init();

	//�e�N�X�`���̊��蓖��
	if (m_nTypeModel == 6 || m_nTypeModel == 7)
	{
		BindTexture(m_apTexture[6]);
	}
	else if (m_nTypeModel == 8 || m_nTypeModel == 9)
	{
		BindTexture(m_apTexture[7]);
	}
	else if (m_nTypeModel == 10)
	{
		BindTexture(m_apTexture[8]);
	}
	else if (m_nTypeModel == 11)
	{
		BindTexture(m_apTexture[9]);
	}
	else if (m_nTypeModel == 12 || m_nTypeModel == 13)
	{
		BindTexture(m_apTexture[10]);
	}
	else if (m_nTypeModel == 14 || m_nTypeModel == 15)
	{
		BindTexture(m_apTexture[11]);
	}
	else if (m_nTypeModel == 16)
	{
		BindTexture(m_apTexture[12]);
	}
	else if (m_nTypeModel == 17 || m_nTypeModel == 18 || m_nTypeModel == 19 || m_nTypeModel == 20 || m_nTypeModel == 21)
	{
		BindTexture(m_apTexture[13]);
	}
	else if (m_nTypeModel == 22)
	{
		BindTexture(m_apTexture[14]);
	}
	else if (m_nTypeModel == 23 || m_nTypeModel == 24)
	{
		BindTexture(NULL);
	}
	else if (m_nTypeModel == 25 || m_nTypeModel == 26)
	{
		BindTexture(m_apTexture[15]);
	}
	else
	{
		BindTexture(m_apTexture[m_nTypeModel]);
	}
	m_vtxMax = GetVtxMax();		//�ő咸�_���W�̎擾
	m_vtxMin = GetVtxMin();		//�ŏ����_���W�̎擾

	return S_OK;
}

//=============================================================================
// �֐����F���f���̏I������
// �֐��̊T�v�F���f���̏I��
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// �֐����F���f���̍X�V
// �֐��̊T�v�F--
//=============================================================================
void CObject::Update(void)
{
	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

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
	if (mode == CManager::MODE_GAME)
	{
		playerpos = pPlayer->GetPos();
		if (playerpos.z > -35.0f)
		{
			if (m_nTypeModel == 14)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.3f));
			}
			if (m_nTypeModel == 25)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.3f));
			}
		}
		else
		{
			if (m_nTypeModel == 14)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			}
			if (m_nTypeModel == 25)
			{
				SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}

		if (playerpos.x <= 1400.0f && playerpos.x >= 1200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			if (m_nTypeModel == 23)
			{
				SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.3f));
			}
		}
		else if (playerpos.x <= 2400.0f && playerpos.x >= 2200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			if (m_nTypeModel == 23)
			{
				SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.3f));
			}
		}
		else
		{
			if (m_nTypeModel == 23)
			{
				SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
			}
		}
	}


	if (CScene::GetStop() == true)
	{//�v���C���[�ȊO���~�܂��Ă���Ƃ�
		if (m_nTypeModel == 8)
		{
			SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.3f));
		}
	}
	else
	{
		if (m_nTypeModel == 8)
		{
			SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
		}
	}

	CollisionAttack();

	//�p�x���̑��
	SetRot(m_rot);

	//�ʒu���̑��
	SetPos(m_pos);
}

//=============================================================================
// �֐����F���f���̕`�揈��
// �֐��̊T�v�F�K�w�\���ŕ`��
//=============================================================================
void CObject::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//���f���̕`��
	CSceneX::Draw();


}

//=============================================================================
// �֐����F�U���̓����蔻�菈��
// �֐��̊T�v�F�U���������������̔���
//=============================================================================
void CObject::CollisionAttack(void)
{
	//�����蔻��̃|�C���^
	CCollision *pCollision;



	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();




	pCollision = (CCollision*)GetSceneTop(COLLISION_PRIORITY);
	while (pCollision != NULL)//NULL�ɂȂ�܂�
	{
		CCollision *pCollisionNext;
		pCollisionNext = (CCollision*)pCollision->GetSceneNext(COLLISION_PRIORITY);

		if (pCollision->CollisionAttack(&m_pos, &m_rot, m_fHeight) == true)
		{

			if (m_nTypeModel == 3)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SMALLHIT);

				pSound->PlaySound(CSound::SOUND_LABEL_SE_PHONE);
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}

			}
			if (m_nTypeModel == 0)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_HITDRAM);

				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
			}
			if (m_nTypeModel == 22)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SMALLHIT);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_JIHANKI);

				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
				for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
				{
					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 9);
				}
			}
		}

		pCollision = pCollisionNext;
	}
}




//=============================================================================
// �֐����F���f���̏��̓ǂݍ��ݏ���
// �֐��̊T�v�F�e�L�X�g�t�@�C�����烂�f������ǂݍ���
//=============================================================================
void CObject::LoadModelText(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

	FILE *pFile = fopen(MODEL_FILE_NAME, "r");		// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

	// �ǂݍ��ނ��߂̕ϐ��錾
	char acLine[MAX_CHAR];							// 1�s���ǂݍ��ޕϐ�
	char acData[MAX_CHAR];							// 1����ǂݍ��ޕϐ�
	char cFileName[MAXMODEL_TYPE][MAX_CHAR];		// �ǂݍ���
	int nCntModel = 0;								// �ǂݍ��ރ��f���̐�
	int nCntTexture = 0;							// �ǂݍ��ރe�N�X�`����
	int nTypeModel;									// �g�p���郂�f���̃^�C�v
	int nMaxModel = 0;								// ���f���̍ő吔
	D3DXVECTOR3 pos;								// �ʒu������ϐ�
	D3DXVECTOR3 rot;								// ����������ϐ�

	if (pFile != NULL)
	{// �k���`�F�b�N
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// "NUM_MODEL"�̕����񂪂�������
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nMaxModel);	// ���f���̐����擾
			}
			if (strcmp(acData, "MODEL_FILENAME") == 0)
			{// "MODEL_FILENAME"�̕����񂪂�������
				sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &cFileName[nCntModel][0]);	// ���f���̐����擾
				m_pModelName[nCntModel] = &cFileName[nCntModel][0];//�t�@�C�����̑��
				nCntModel++;		// ���f���������Z
			}
			if (strcmp(acData, "MODELSET") == 0)
			{// "MODELSET"�̕����񂪂�������

				while (strcmp(acData, "END_MODELSET") != 0)
				{// "END_MODELSET"���܂ŏ����擾��������
					fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
					sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

					if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nTypeModel);					// �g�p���郂�f���̔ԍ����擾
					}
					else if (strcmp(acData, "POS") == 0)
					{// "POS"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// �ʒu���擾
					}
					else if (strcmp(acData, "ROT") == 0)
					{// "ROT"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// �������擾
						//�e�L�X�g�̊p�x���ʂ�ɓ����v�Z
						rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
						rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
						rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
					}
				}
				//���f���̐���
				Create(pos, rot, nTypeModel);
			}
		}
		fclose(pFile);	// �J�����t�@�C�������
	}
}

//=============================================================================
// �֐����F���f���̏��̓ǂݍ��ݏ���(Tuto)
// �֐��̊T�v�F�e�L�X�g�t�@�C�����烂�f������ǂݍ���
//=============================================================================
void CObject::LoadModelTextTuto(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

	FILE *pFile = fopen(MODEL_FILE_NAME_TUTO, "r");		// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

													// �ǂݍ��ނ��߂̕ϐ��錾
	char acLine[MAX_CHAR];							// 1�s���ǂݍ��ޕϐ�
	char acData[MAX_CHAR];							// 1����ǂݍ��ޕϐ�
	char cFileName[MAXMODEL_TYPE][MAX_CHAR];		// �ǂݍ���
	int nCntModel = 0;								// �ǂݍ��ރ��f���̐�
	int nCntTexture = 0;							// �ǂݍ��ރe�N�X�`����
	int nTypeModel;									// �g�p���郂�f���̃^�C�v
	int nMaxModel = 0;								// ���f���̍ő吔
	D3DXVECTOR3 pos;								// �ʒu������ϐ�
	D3DXVECTOR3 rot;								// ����������ϐ�

	if (pFile != NULL)
	{// �k���`�F�b�N
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// "NUM_MODEL"�̕����񂪂�������
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nMaxModel);	// ���f���̐����擾
			}
			if (strcmp(acData, "MODEL_FILENAME") == 0)
			{// "MODEL_FILENAME"�̕����񂪂�������
				sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &cFileName[nCntModel][0]);	// ���f���̐����擾
				m_pModelName[nCntModel] = &cFileName[nCntModel][0];//�t�@�C�����̑��
				nCntModel++;		// ���f���������Z
			}
			if (strcmp(acData, "MODELSET") == 0)
			{// "MODELSET"�̕����񂪂�������

				while (strcmp(acData, "END_MODELSET") != 0)
				{// "END_MODELSET"���܂ŏ����擾��������
					fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
					sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

					if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nTypeModel);					// �g�p���郂�f���̔ԍ����擾
					}
					else if (strcmp(acData, "POS") == 0)
					{// "POS"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// �ʒu���擾
					}
					else if (strcmp(acData, "ROT") == 0)
					{// "ROT"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// �������擾
																												//�e�L�X�g�̊p�x���ʂ�ɓ����v�Z
						rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
						rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
						rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
					}
				}
				//���f���̐���
				Create(pos, rot, nTypeModel);
			}
		}
		fclose(pFile);	// �J�����t�@�C�������
	}


}

//=============================================================================
// �֐����F���f���Ƃ̓����蔻�菈��
// �֐��̊T�v�F�߂肱�񂾂�߂�����
//=============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius)
{
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();


	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	bool bRand = false;			// ���n�������ǂ���
	if (m_nTypeModel != 5)
	{
		//----------------------------------------------------------
		// ��`�ł̓����蔻��
		//----------------------------------------------------------
		if (m_pos.x + m_vtxMin.x < pPos->x + fRadius &&
			m_pos.x + m_vtxMax.x > pPos->x - fRadius &&
			m_pos.z + m_vtxMin.z < pPos->z + fRadius &&
			m_pos.z + m_vtxMax.z > pPos->z - fRadius)
		{// �I�u�W�F�N�g�͈͓̔���������
			if (m_pos.y + m_vtxMax.y <= pPosOld->y &&
				m_pos.y + m_vtxMax.y > pPos->y)
			{// �ォ��̔���
				pPos->y = m_pos.y + m_vtxMax.y;					// �I�u�W�F�N�g�̈ʒu��߂�
				pMove->y = 0.0f;								// �������x��0.0�ɖ߂�
				bRand = true;									// ���n��������ɂ���
			}
		}
		if (m_pos.y + m_vtxMax.y + 10.0f > pPos->y + fRadius &&
			m_pos.y + m_vtxMin.y <= pPos->y + fRadius)
		{// �v���C���[���I�u�W�F�N�g�͈͓̔��Ȃ�

		 // �u���b�N�̓����蔻��
			if (m_pos.x + m_vtxMin.x < pPos->x + fRadius &&
				m_pos.x + m_vtxMax.x > pPos->x - fRadius)
			{// �I�u�W�F�N�g���I�u�W�F�N�g��Z���͈͓̔��ɓ�������
				if (m_pos.z + m_vtxMin.z >= pPosOld->z + fRadius &&
					m_pos.z + m_vtxMin.z < pPos->z + fRadius)
				{// ��O����̔���

					pPos->z = m_pos.z + m_vtxMin.z - fRadius;			// �I�u�W�F�N�g�̈ʒu��߂�

				}
				else if (m_pos.z + m_vtxMax.z <= pPosOld->z - fRadius &&
					m_pos.z + m_vtxMax.z > pPos->z - fRadius)
				{// ������̔���

					pPos->z = m_pos.z + m_vtxMax.z + fRadius;			// �I�u�W�F�N�g�̈ʒu��߂�

				}

			}
			if (m_pos.z + m_vtxMin.z < pPos->z + fRadius &&
				m_pos.z + m_vtxMax.z > pPos->z - fRadius)
			{// Z�����I�u�W�F�N�g�͈͓̔���������
				if (m_pos.x + m_vtxMin.x >= pPosOld->x + fRadius &&
					m_pos.x + m_vtxMin.x < pPos->x + fRadius)
				{// ������̔���

					pPos->x = m_pos.x + m_vtxMin.x - fRadius;			// �I�u�W�F�N�g�̈ʒu��߂�

				}
				else if (m_pos.x + m_vtxMax.x <= pPosOld->x - fRadius &&
					m_pos.x + m_vtxMax.x > pPos->x - fRadius)
				{// �E����̔���

					pPos->x = m_pos.x + m_vtxMax.x + fRadius;			// �I�u�W�F�N�g�̈ʒu��߂�

				}
			}
		}
	}
	return bRand;
}


//=============================================================================
// �֐����F�S�[���Ƃ̓����蔻��
// �֐��̊T�v�F�v���C���[�����a�ɓ����Ă�����true��Ԃ�
//=============================================================================
bool CObject::CollisionGoal(D3DXVECTOR3 *pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot)
{
	bool bCol = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//�v���C���[�Ƃ̋���
	float fRadius = powf(30.0f, 1.0f);	//���a

	if (fLength < fRadius && pos->y <= m_pos.y + m_fHeight && pos->y >= m_pos.y)
	{//�Ώۂ̒��_���͈͓���������
		bCol = true;
		float fAttackAngle = atan2f(pos->x - m_pos.x,
		pos->z - m_pos.z);// ���������p�x���v�Z
		move->x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
		move->y = 20.0f;
		move->z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
	}
	else
	{
		bCol = false;

	}
	return bCol;
}
