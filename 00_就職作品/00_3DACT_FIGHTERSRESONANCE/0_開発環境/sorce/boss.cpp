//=============================================================================
//
// �G����[���f��] [player.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "sceneX.h"
#include "scene2D.h"
#include "debugproc.h"
#include "boss.h"
#include "camera.h"
#include "model.h"
#include "object.h"
#include "scene_meshfield.h"
#include "scene_meshwall.h"
#include "scene_meshorbit.h"
#include "scene_meshcylinder.h"
#include "scene_gauge.h"
#include "game.h"
#include "sound.h"
#include "particle.h"
#include "collision.h"
#include "score.h"
#include "player.h"
#include "ui_gauge.h"
#include "combogauge.h"
#include "combo.h"
#include "effect.h"
#include "cutin.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//=====================================
// �G���
//=====================================
#define ENEMY_00_MOVE (0.5f)					//�ړ���

#define ENEMY_MOVE_DEFAULT		(1.0f)			// �G�������Ă��̈ړ���
#define MAX_CHAR (256)							//�����̍ő吔
#define GRAVITY_NUM (0.8f)						//�d�͂̒l
#define ENEMY_MOTION_BLEND_FRAME	(5)			// ���[�V�����u�����h�̃t���[����
#define JUMP_POWER (25.0f)						//�W�����v��
#define ENEMY_COLLISION_RADIUS		(20.0f)		// �G�̓����蔻��̔��a

#define SCREEN_LIMIT_MAX_X (5000.0f )			//��ʒ[(�E�[)
#define SCREEN_LIMIT_MIN_X (-120.0f)			//���(���[)

#define SCREEN_LIMIT_MIN_Z (-370.0f)			//���(���[)
#define HITPARTICLE (12)
//=====================================
// �p�[�e�B�N���̐F
//=====================================
#define PARTICLE_FIRE (D3DXCOLOR(0.7f, 0.2f, 0.9f, 1.0f))

//=====================================
// �D�揇��
//=====================================
#define ENEMY_PRIORITY (2)						//�G�̗D�揇��
#define COLLISION_PRIORITY (6)					//�����蔻��̗D�揇��
#define WALL_PRIORITY (5)						//�ǂ̗D�揇��
#define OBJECT_PRIORITY (4)						//�I�u�W�F�N�g�̗D�揇��
#define CYLINDER_PRIORITY (3)					//�V�����_�[�̗D�揇��

//=====================================
// �l���X�R�A
//=====================================
#define LIGHT_ATTACK_SCORE (100)		//��U��
#define MEDIUM_ATACK_SCORE (150)		//���U��
#define FINISH_ATACK_SCORE (500)	//�ŏI�i�U��
#define LARGE_ATACK_SCORE (100)		//���U��
#define SKILL_ATACK_SCORE (2000)	//�X�L���U��



//=====================================
// �_���[�W��
//=====================================
#define LIGHT_ATACK (12.0f)		//��U��
#define MEDIUM_ATACK (15.0f)		//���U��
#define FINISH_ATACK (20.0f)	//�ŏI�i�U��
#define LARGE_ATACK (13.0f)		//���U��
#define SKILL_ATACK (200.0f)		//���U��

//=====================================
// �R���{�Q�[�W������
//=====================================
#define LIGHT_ATACK_GAUGE (13.0f)		//��U��
#define MEDIUM_ATACK_GAUGE (15.0f)		//���U��
#define FINISH_ATACK_GAUGE (50.0f)		//�ŏI�i�U��
#define LARGE_ATACK_GAUGE (12.0f)		//���U��

//=====================================
// �X�L���Q�[�W������
//=====================================
#define LIGHT_ATACK_SKILL (8.0f)		//��U��
#define MEDIUM_ATACK_SKILL (9.0f)		//���U��
#define FINISH_ATACK_SKILL (12.0f)		//�ŏI�i�U��
#define LARGE_ATACK_SKILL (5.0f)		//���U��



//=====================================
// �ǂݍ��ރe�L�X�g�t�@�C��
//=====================================
#define MODEL_FILENAME_00 ("data/TEXT/motion_boss_0.txt")			// �{�X�̃e�L�X�g


//**************************************************************************************************
// �ÓI�����o�ϐ�
//**************************************************************************************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;							// ���L�e�N�X�`���̃|�C���^
char			   CBoss::m_acFileName[MAX_BOSS_PARTS][256] = {};		// �ǂݍ��ރt�@�C����
LPD3DXMESH		   CBoss::m_apMesh[MAX_BOSS_PARTS] = {};				// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER	   CBoss::m_apBuffMat[MAX_BOSS_PARTS] = {};				// �}�e���A�����ւ̃|�C���^
DWORD			   CBoss::m_aNumMat[MAX_BOSS_PARTS] = {};				// �}�e���A���̏��̐�
CMeshOrbit		  *CBoss::m_pMeshOrbit = NULL;							// �O�Ղ̃|�C���^
//========================================================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//========================================================================================================
CBoss::CBoss(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_bUseDraw = true;
}

//============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//============================================================================
CBoss::~CBoss()
{
}


//=============================================================================
// �֐����F�g�p����e�N�X�`���ƃ��f���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CBoss::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/character/boss_ex.png", &m_pTexture);



	FILE *pFile = fopen(MODEL_FILENAME_00, "r");	// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

													// �ǂݍ��݂ɕK�v�ȕϐ��錾
	char acLine[MAX_CHAR];							// 1�s���ǂݍ��ޕϐ�
	char acData[MAX_CHAR];							// 1����ǂݍ��ޕϐ�
	int nCntModel = 0;								// �ǂݍ��ރ��f���̐�
	int nCntTexture = 0;							// �ǂݍ��ރe�N�X�`����
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
			if (nCntModel < nMaxModel)
			{// ���[�V�����̑����ȏ�͐ݒ肵�Ȃ�
				if (strcmp(acData, "MODEL_FILENAME") == 0)
				{// "MODEL_FILENAME"�̕����񂪂�������
					sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &m_acFileName[nCntModel][0]);	// ���f���̐����擾
																											// X�t�@�C���̓ǂݍ���
					D3DXLoadMeshFromX(m_acFileName[nCntModel],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&m_apBuffMat[nCntModel],
						NULL,
						&m_aNumMat[nCntModel],
						&m_apMesh[nCntModel]);
					nCntModel++;		// ���f���������Z

				}
			}
		}
		fclose(pFile);	// �J�����t�@�C�������
	}

	return S_OK;

}

//=============================================================================
// �֐����F�g�p�����e�N�X�`���̔j��
// �֐��̊T�v�F--
//=============================================================================
void CBoss::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//�}�e���A���̐��̔j��
	for (int nCntModel = 0; nCntModel < MAX_BOSS_PARTS; nCntModel++)
	{
		if (m_aNumMat[nCntModel] != NULL)
		{
			m_aNumMat[nCntModel] = NULL;

		}
	}

	//���b�V�����̔j��
	for (int nCntModel = 0; nCntModel < MAX_BOSS_PARTS; nCntModel++)
	{
		if (m_apMesh[nCntModel] != NULL)
		{
			m_apMesh[nCntModel]->Release();
			m_apMesh[nCntModel] = NULL;
		}
	}

	//�}�e���A�����̔j��
	for (int nCntModel = 0; nCntModel < MAX_BOSS_PARTS; nCntModel++)
	{
		if (m_apBuffMat[nCntModel] != NULL)
		{
			m_apBuffMat[nCntModel]->Release();
			m_apBuffMat[nCntModel] = NULL;
		}
	}

}


//=============================================================================
// �֐����F�G�V�[���̐���
// �֐��̊T�v�F�G�V�[���𐶐�����
//=============================================================================
CBoss *CBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife)
{
	CBoss  *pBoss;
	pBoss = new CBoss;//�V�[���N���X�̐���
	if (pBoss != NULL)
	{
		pBoss->m_pos = pos;
		//pBoss->m_rot.y = -D3DX_PI * 0.5f;
		pBoss->m_rot = rot;
		pBoss->m_fDestAngle = pBoss->m_rot.y;
		pBoss->m_fLife = fLife;
		pBoss->m_posold = pBoss->m_pos;
		pBoss->m_bAction[BOSSBOOLTYPE_JUMP] = false;
		pBoss->Init();
	}
	return pBoss;
}

//=============================================================================
// �֐����F�G�V�[���̏���������
// �֐��̊T�v�F�G���̐ݒ�
//=============================================================================
HRESULT CBoss::Init(void)
{

	// �G���̓ǂݍ���
	LoadBossText();

	// ���[�V�������̏�����
	InitMotion();

	//�Q�[�W�̐���
	CreateGauge();


	// �G���̏�����
	InitBoss();

	// �p�[�c�̏����ݒ�
	DefaultParts();


	// 2D�V�[���̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_BOSS);

	return S_OK;
}
//=============================================================================
// �֐����F�G�V�[���̏I������
// �֐��̊T�v�F���b�V���A�}�e���A���A���g�̔j��
//=============================================================================
void CBoss::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{
		//���f���̏I������
		if (m_apModel[nCntParts] != NULL)
		{
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}
	}



	//�I�u�W�F�N�g�̔j��
	Release();

}

//=============================================================================
// �֐����F�G�V�[���̍X�V����
// �֐��̊T�v�F�G�̈ړ��A�p�[�c�̃��[�V�����X�V
//=============================================================================
void CBoss::Update(void)
{
	//�ȑO�̈ʒu�ɍ��̈ʒu����
	m_posold = m_pos;


	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	//==============================================
	//�G�̏��
	//==============================================
	BossState();

	//==============================================
	//�G�̈ړ�
	//==============================================
	BossMove();

	//==============================================
	// �����Ɛ���
	//==============================================
	BossFallen();


	//==============================================
	//�G�̃A�N�V����
	//==============================================
	BossAction();


	//=============================================
	// ���[�V�����̏�ԍX�V
	//=============================================
	BossMotionState(m_bAction);
	BossMotion();

	//==============================================
	//�ʒu�������Ɗ����ړ�
	//==============================================
	BossSetMove_Interia();

	//=============================================
	// �̗͂̃`�F�b�N
	//=============================================
	BossLifeCheck();

	//=============================================
	// �����蔻�� (�v���C���[)
	//=============================================
	CollisionPlayer();

	//=============================================
	// �����蔻�� (�U��)
	//=============================================
	CollisionAttack();

	//=============================================
	// �����蔻�� (�I�u�W�F�N�g)
	//=============================================
	CollisionObjectBoss();

	//=============================================
	// �����蔻��(��)
	//=============================================
	CollisionWallBoss();

	//=============================================
	// �����蔻�� (�V�����_�[)
	//=============================================
	CollisionCylinderBoss();

	//=============================================
	// �v���C���[�̒T��
	//=============================================
	UpdateFind();



	//=============================================
	// �R���{�I���t���[���̍X�V
	//=============================================
	//UpdateCancelCombo();

	//�ړI�̊p�x�܂�
	m_fAngle = m_fDestAngle - m_rot.y;
	//�p�x����
	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2;
	}

	if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2;
	}
	//�G�̊p�x����
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	//�p�x���
	m_rot.y += m_fAngle * 0.1f;

	if (m_move.y >= 32.0f)
	{
		m_move.y = 32.0f;
	}

	if (m_pos.y >= 220.0f)
	{
		m_pos.y = 220.0f;
	}

	m_nCooltime--;
	if (m_nCooltime <= 0)
	{
		m_nCooltime = 0;
	}


	m_fSkill += 0.7f;
	//m_fSkill += 100.0f;

	//==============================================
	// �f�o�b�O���O�֌W
	//==============================================
#ifdef _DEBUG
	//BossDebugLog();
#endif


	//�ȑO�̃��[�V��������
	m_MotionstateOld = m_Motionstate;


}

//=============================================================================
// �֐����F�G�V�[���̕`�揈��
// �֐��̊T�v�F�G�̃}�g���b�N�X�ݒ�A�e�p�[�c�̕`��
//=============================================================================
void CBoss::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);




	//======================================================================
	//	�e�̐ݒ�
	//======================================================================
	DrawShadow();

	//======================================================================
	//	�p�[�c�̐ݒ�
	//======================================================================
	DrawParts();


}

//=============================================================================
// �֐����F�G�̉e�̕`��
// �֐��̊T�v�F�G�̉e��`�悷��
//=============================================================================
void CBoss::DrawShadow(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	D3DXMATRIX mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DXPLANE planeField;		// ���e���镽��
	D3DXVECTOR4 vecLight;		// ���C�g�̃x�N�g��
	D3DXVECTOR3 pos, normal;	// �ʒu�Ɩ@��


								//���u�����f�B���O�����Z�����ɂ���
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	//���C�g�̃x�N�g���̐ݒ�
	vecLight = D3DXVECTOR4(-0.15f, 0.7f, -0.4f, 0.0f);


	pos = D3DXVECTOR3(0.0f, 0.5f, 0.0f);		// ���ʂ̈ʒu�̐ݒ�
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ���ʂ̖@���̐ݒ�

												//�e�𓊉e���镽�ʂ̍쐬
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	//���ʂɓ��e����e�̐ݒ�
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//�V���h�E�}�g���b�N�X�ƃ��[���h�}�g���b�N�X�̊|�����킹
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxShadow);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{
		//���f���̕`��
		m_apModel[nCntParts]->Draw();
	}
	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

//=============================================================================
// �֐����F�G�̃p�[�c�̕`��
// �֐��̊T�v�F�G�̃p�[�c��`�悷��
//=============================================================================
void CBoss::DrawParts(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);



	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_bUseDraw == true)
	{
		for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
		{
			//���f���̕`��
			m_apModel[nCntParts]->Draw();
		}
	}
}


//=============================================================================
// �֐����F�G�̈ړ�����
// �֐��̊T�v�F�G�̈ړ��֌W
//=============================================================================
void CBoss::BossMove(void)
{

}

//=============================================================================
// �֐����F�G�̃A�N�V����
// �֐��̊T�v�F�U���n�̏���
//=============================================================================
void CBoss::BossAction(void)
{

}

//=============================================================================
// �֐����F�G�̗���
// �֐��̊T�v�F�����ƒn�ʂւ̐���
//=============================================================================
void CBoss::BossFallen(void)
{
	//����
	m_move.y -= GRAVITY_NUM;

	if (m_pos.y <= 0.0f)
	{//���̍����ȉ���������
		m_pos.y = 0.0f;//������n�ʂ̍�����
		m_move.y = 0.0f;
		if (m_state == BOSSSTATE_NORMAL || m_state == BOSSSTATE_STEEL)
		{
			if (m_bAction[BOSSBOOLTYPE_JUMP] == true)
			{
				if (m_move.x < 0.2f && m_move.x > -0.2f && m_move.z < 0.2f && m_move.z > -0.2f)
				{
					m_state = BOSSSTATE_STEEL;
					m_bAction[BOSSBOOLTYPE_LANDING] = true;//���n��Ԃ�
					m_bAction[BOSSBOOLTYPE_JUMP] = false;	//�W�����v��Ԃ�����
				}
				m_bAction[BOSSBOOLTYPE_JUMP] = false;//�W�����v�ł����Ԃ�
			}
			//if (m_bAction[BOSSBOOLTYPE_SMALLDAMAGE] == true)
			//{// �_���[�W���ɒ��n������
			//	m_StateCnt = 120;
			//	m_state = BOSSSTATE_INVISIBLE;
			//	m_bAction[BOSSBOOLTYPE_SMALLDAMAGE] = false;				//�_���[�W��Ԃ�����
			//}
		}
	}
	if (m_pos.y <= 0.0f && m_posold.y >= 1.0f)
	{//���̍����ȉ���������
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_RAND, 20);
		}
	}
}

//=============================================================================
// �֐����F�G�̃��[�V����
// �֐��̊T�v�F�u�����h�ݒ��A���[�V����
//=============================================================================
void CBoss::BossMotion(void)
{

	//-----------------------------------------------------------------------
	// ���[�V�����u�����h�̐ݒ�
	//-----------------------------------------------------------------------
	if (m_Motionstate != m_MotionstateOld)
	{// ���[�V�����̃X�e�[�g���ς������
		m_nCounterFrame = 0;						// ���݂̃L�[�t���[���̃t���[������������
		m_nCounterKey = 0;							// �L�[�t���[���̏�����
		m_nCounterAllFrame = 0;						// ���[�V�����̃t���[������������
		m_nFrame = ENEMY_MOTION_BLEND_FRAME;		// ���[�V�����u�����h�̃t���[������ݒ�
		m_nAllFrame = 0;							//�S�Ẵt���[���̃��Z�b�g
		for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
		{//���t���[�����̌v�Z
			m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
		}
		m_bMotionBlend = true;		// ���[�V�����u�����h��true�ɂ���
	}

	//-----------------------------------------------------------------------
	// �L�[�t���[���̐؂�ւ��
	//-----------------------------------------------------------------------
	if (m_nCounterFrame == m_nFrame)
	{// ���[�V�����u�����h�̎��A�t���[�������Ō�ɂȂ�����
		m_nCounterFrame = 0; // ���݂̃L�[�t���[���̃t���[������������
		m_nFrame = m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].nFrame; // ���[�V�����̃t���[������ݒ�

		m_nCounterKey++; // �L�[�t���[����i�߂�

		if (m_nCounterKey == m_aKeyInfo[m_Motionstate].nNumKey)
		{// �L�[�t���[�����Ō�܂ŗ�����
			if (m_aKeyInfo[m_Motionstate].nLoop == 0)
			{// ���[�v���Ȃ��ꍇ
				switch (m_Motionstate)
				{// �G�̃��[�V������

				case MOTION_JUMP: // �W�����v���[�V������������
					m_nCounterKey--; // �L�[�t���[����߂�
					break;

				case MOTION_LANDING: // ���n���[�V������������
					m_bAction[BOSSBOOLTYPE_LANDING] = false;	//��Ԃ�߂�
					m_nCounterKey--; // �L�[�t���[����߂�
					break;

				case MOTION_LIGHTATTACK: //��U��
					m_bAction[BOSSBOOLTYPE_LIGHTATTACK] = false;//��Ԃ�߂�
					m_nCounterKey--; // �L�[�t���[����߂�
					break;
				case MOTION_HEAVYATTACK: //���U��
					m_bAction[BOSSBOOLTYPE_HEAVYATTACK] = false;//��Ԃ�߂�
					m_nCounterKey--;// �L�[�t���[����߂�
					break;
				case MOTION_MOVEATTACK: //���U��
					m_bAction[BOSSBOOLTYPE_MOVEATTACK] = false;//��Ԃ�߂�
					m_nCounterKey--;// �L�[�t���[����߂�
					break;
				case MOTION_SKILL:
					m_bAction[BOSSBOOLTYPE_SKILL] = false;// �_���[�W������Ȃ���Ԃɂ���
					CScene::SetBossStop(false);
					m_nCounterKey--; // �L�[�t���[����߂�
					break;

				case MOTION_NORMALDAMAGE: //�ʏ�̃_���[�W
					m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = false; //��Ԃ�߂�
					for (int nCntDamage = 0; nCntDamage < 8; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
					}
					m_nCounterKey--;// �L�[�t���[����߂�
					break;
				case MOTION_FLOATDAMAGE: //�������̃_���[�W
					m_bAction[BOSSBOOLTYPE_FLOATDAMAGE] = false; //��Ԃ�߂�
					for (int nCntDamage = 0; nCntDamage < 8; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
					}
					m_nCounterKey--;// �L�[�t���[����߂�
					break;
				case MOTION_IMPACTDAMAGE: //������΂��_���[�W
					m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = false;
					for (int nCntDamage = 0; nCntDamage < 8; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
					}
					m_nCounterKey--;// �L�[�t���[����߂�
					break;
				case MOTION_DEATH://���S
					m_bAction[BOSSBOOLTYPE_DEATH] = false;
					m_nCounterKey--;// �L�[�t���[����߂�
					break;
				}
				if (m_nCounterKey <= 0)
				{//�L�[���O�ȉ��ɂȂ�����
					m_nCounterKey = 0;
				}
				m_state = BOSSSTATE_NORMAL; //�ʏ��Ԃ�
			}
			else
			{// ���[�v����
				m_nCounterKey = m_nCounterKey % m_aKeyInfo[m_Motionstate].nNumKey; // �L�[�t���[����i�߂�
				m_nCounterAllFrame = 0; // ���[�V�����̃t���[����(ALL)��������
			}
		}
	}

	//-----------------------------------------------------------------------
	// ���[�V�����̍�������
	//-----------------------------------------------------------------------
	if (m_nCounterFrame == 0)
	{// �t���[�������ŏ��̎�
		for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
		{// �p�[�c�������[�v
			if (m_apModel[nCntParts] != NULL)
			{
				// ���[�V�����̈ʒu��������
				m_Motionpos[nCntParts] = (m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].pos[nCntParts] + m_Partspos[nCntParts]) - m_PartsDefaultpos[nCntParts];
				// ���[�V�����̉�]��������
				m_Motionrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].rot[nCntParts] - m_PartsDefaultrot[nCntParts];

				// ��]�̍������C������
				// X���̏C��
				if (m_Motionrot[nCntParts].x > D3DX_PI)
				{// +3.14���A�����Ă��܂�����
					m_Motionrot[nCntParts].x -= D3DX_PI * 2.0f;
				}
				else if (m_Motionrot[nCntParts].x < -D3DX_PI)
				{// -3.14���A���������
					m_Motionrot[nCntParts].x += D3DX_PI * 2.0f;
				}
				// Y���̏C��
				if (m_Motionrot[nCntParts].y > D3DX_PI)
				{// +3.14���A�����Ă��܂�����
					m_Motionrot[nCntParts].y -= D3DX_PI * 2.0f;
				}
				else if (m_Motionrot[nCntParts].y < -D3DX_PI)
				{// -3.14���A���������
					m_Motionrot[nCntParts].y += D3DX_PI * 2.0f;
				}
				// Z���̏C��
				if (m_Motionrot[nCntParts].z > D3DX_PI)
				{// +3.14���A�����Ă��܂�����
					m_Motionrot[nCntParts].z -= D3DX_PI * 2.0f;
				}
				else if (m_Motionrot[nCntParts].z < -D3DX_PI)
				{// -3.14���A���������
					m_Motionrot[nCntParts].z += D3DX_PI * 2.0f;
				}
			}
		}
	}

	//----------------------------------------------
	// ���[�V�����̓���
	//----------------------------------------------
	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{// �p�[�c�������[�v
		if (m_apModel[nCntParts] != NULL)
		{
			// �ʒu�������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultpos[nCntParts].x += m_Motionpos[nCntParts].x / m_nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultpos[nCntParts].y += m_Motionpos[nCntParts].y / m_nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultpos[nCntParts].z += m_Motionpos[nCntParts].z / m_nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���
																						//��]�l�̑��
			m_apModel[nCntParts]->SetPos(m_PartsDefaultpos[nCntParts]);

			// ��]�������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultrot[nCntParts].x += m_Motionrot[nCntParts].x / m_nFrame;		// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultrot[nCntParts].y += m_Motionrot[nCntParts].y / m_nFrame;		// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
			m_PartsDefaultrot[nCntParts].z += m_Motionrot[nCntParts].z / m_nFrame;		// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
																						//��]�l�̑��
			m_apModel[nCntParts]->SetRot(m_PartsDefaultrot[nCntParts]);

			// ���f���̉�]���C������
			// X���̏C��
			if (m_Partsrot[nCntParts].x > D3DX_PI)
			{// +3.14���A�����Ă��܂�����
				m_Partsrot[nCntParts].x -= D3DX_PI * 2.0f;
			}
			else if (m_Partsrot[nCntParts].x < -D3DX_PI)
			{// -3.14���A���������
				m_Partsrot[nCntParts].x += D3DX_PI * 2.0f;
			}
			// Y���̏C��
			if (m_Partsrot[nCntParts].y > D3DX_PI)
			{// +3.14���A�����Ă��܂�����
				m_Partsrot[nCntParts].y -= D3DX_PI * 2.0f;
			}
			else if (m_Partsrot[nCntParts].y < -D3DX_PI)
			{// -3.14���A���������
				m_Partsrot[nCntParts].y += D3DX_PI * 2.0f;
			}
			// Z���̏C��
			if (m_Partsrot[nCntParts].z > D3DX_PI)
			{// +3.14���A�����Ă��܂�����
				m_Partsrot[nCntParts].z -= D3DX_PI * 2.0f;
			}
			else if (m_Partsrot[nCntParts].z < -D3DX_PI)
			{// -3.14���A���������
				m_Partsrot[nCntParts].z += D3DX_PI * 2.0f;
			}

		}
	}

	m_nCounterFrame++;		// ���݃L�[�t���[���̃t���[���������Z
	m_nCounterAllFrame++;	// ���[�V�����̃t���[���������Z

}

//=============================================================================
// �֐����F�G�̏�ԕω�
// �֐��̊T�v�F�G�̏�ԕω�
//=============================================================================
void CBoss::BossState(void)
{
	switch (m_state)
	{
	case BOSSSTATE_NORMAL:	//�ʏ���
		break;


	case BOSSSTATE_APPEAR:	//�o����


		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_state = BOSSSTATE_NORMAL;
			CScene::ChangeStopBoss();
		}
		break;

	case BOSSSTATE_DEATH:
		CScore::AddScore(1000);
		Uninit();
		CGame::GAMESTATE pGameState;
		pGameState = CGame::GetGameState();
		if (pGameState == CGame::GAMESTATE_NORMAL)
		{
			CGame::SetGameState(CGame::GAMESTATE_CLEAR);
		}
		break;



		break;
	}
}

//=============================================================================
// �֐����F�G�̈ړ��ʂ̑���Ɗ����ړ�
// �֐��̊T�v�F--
//=============================================================================
void CBoss::BossSetMove_Interia(void)
{
	//�ړ��ʂɑ��
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;
	//�����ړ�
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;
	m_move.z += (0.0f - m_move.z) * 0.12f;
}

//=============================================================================
// �֐����F�G�̃��[�V�����X�e�[�g�̕ύX
// �֐��̊T�v�F�G�̏�ԑJ��
//=============================================================================
void CBoss::BossMotionState(bool *bAction)
{
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();


	//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//�J�����̊p�x�擾
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//���f���̃}�g���b�N�X���
	D3DMATRIX ModelMtx;

	//�v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerpos = pPlayer->GetPos();


	if (bAction[BOSSBOOLTYPE_LANDING] == true)
	{// �_���[�W���n
		m_Motionstate = MOTION_LANDING; // ���n���[�V�����ɐݒ�
	}
	else if (bAction[BOSSBOOLTYPE_JUMP] == true)
	{// �G���W�����v���Ȃ�
		m_Motionstate = MOTION_JUMP; // �W�����v���[�V�����ɐݒ�
	}
	else if (bAction[BOSSBOOLTYPE_LIGHTATTACK] == true)
	{// �G����U���������Ȃ�
		m_nCooltime = 130;
		if (m_nCounterAllFrame >= 2 && m_nCounterAllFrame <= 12 && m_Motionstate == MOTION_LIGHTATTACK)
		{//1�t���[������U�����[�V�����̂Ƃ�
			m_state = BOSSSTATE_INVISIBLE;	//�d����Ԃ�
		}
		if (m_nCounterAllFrame >= 11 && m_Motionstate == MOTION_LIGHTATTACK)
		{//8�t���[������U�����[�V�����̂Ƃ�
			//m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		}
		m_Motionstate = MOTION_LIGHTATTACK; // ��U�����[�V�����ɐݒ�
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_LIGHTATTACK)
		{//8�t���[������U�����[�V�����̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.2f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.2f);
		}

		if (m_nCounterAllFrame == 25 && m_Motionstate == MOTION_LIGHTATTACK)
		{//13�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[9]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 40.0f, CCollision::COLLSIONTYPE_BOSS_LIGHTATTACK, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_HEAVYATTACK] == true)
	{// �G�����U���������Ȃ�
		m_nCooltime = 130;
		//m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_HEAVYATTACK; // ���U�����[�V�����ɐݒ�
		if (m_nCounterAllFrame >= 3 && m_nCounterAllFrame <= 12 && m_Motionstate == MOTION_HEAVYATTACK)
		{//1�t���[������U�����[�V�����̂Ƃ�
			m_state = BOSSSTATE_INVISIBLE;	//�d����Ԃ�
		}
		if (m_nCounterAllFrame == 14 && m_Motionstate == MOTION_HEAVYATTACK)
		{//14�t���[�������U�����[�V�����̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.5f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.5f);
		}
		if (m_nCounterAllFrame > 13 && m_nCounterAllFrame < 25 && m_Motionstate == MOTION_HEAVYATTACK)
		{//12�t���[���ȏ�̂Ƃ���]�R��̂Ƃ�
			ModelMtx = m_apModel[15]->GetMatrix();
			for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 10);
			}
			for (int nCntParticle = 0; nCntParticle < 15; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 12);
			}
		}
		if (m_nCounterAllFrame == 36 && m_Motionstate == MOTION_HEAVYATTACK)
		{//13�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[9]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 50.0f, CCollision::COLLSIONTYPE_BOSS_HEAVYATTACK, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_MOVEATTACK] == true)
	{// �G�����U���������Ȃ�
		m_nCooltime = 120;
		m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_MOVEATTACK; // ���U�����[�V�����ɐݒ�
		if (m_nCounterAllFrame >= 7 && m_nCounterAllFrame <= 25 && m_Motionstate == MOTION_MOVEATTACK)
		{//14�t���[�������U�����[�V�����̂Ƃ�
			ModelMtx = m_apModel[9]->GetMatrix();
			for (int nCntParticle = 0; nCntParticle < 5; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 7);
			}
			for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, 9);
			}

			D3DXVECTOR3 DiffPos;
			//�v���C���[�ƓG�̃x�N�g�����Z�o
			DiffPos.x = m_pos.x - playerpos.x;
			DiffPos.y = m_pos.y - playerpos.y;
			DiffPos.z = m_pos.z - playerpos.z;
			D3DXVec3Normalize(&DiffPos, &DiffPos);

			//�x�N�g�������Ɍ������Ĉړ�
			m_move.x -= DiffPos.x * 2.0f;
			m_move.z -= DiffPos.z * 2.0f;

			//�G�̊p�x�ݒ�
			m_fDestAngle = atan2f(m_pos.x - playerpos.x, m_pos.z - playerpos.z);
		}
		if (m_nCounterAllFrame == 29 && m_Motionstate == MOTION_MOVEATTACK)
		{//28�t���[�����ړ��p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[9]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 55.0f, CCollision::COLLSIONTYPE_BOSS_HEAVYATTACK, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_NORMALDAMAGE])
	{
		m_Motionstate = MOTION_NORMALDAMAGE;
		m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		if (m_nCounterAllFrame == 3 && m_Motionstate == MOTION_NORMALDAMAGE)
		{//14�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -0.91f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -0.91f;
		}
	}
	else if (bAction[BOSSBOOLTYPE_MEDIUMDAMAGE])
	{
		m_Motionstate = MOTION_NORMALDAMAGE;
		if (m_nCounterAllFrame == 3 && m_Motionstate == MOTION_NORMALDAMAGE)
		{//14�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI)) * -7.2f;
			m_move.z += (cosf(m_rot.y - D3DX_PI)) * -7.2f;

			m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		}
	}
	else if (bAction[BOSSBOOLTYPE_FLOATDAMAGE])
	{
		m_state = BOSSSTATE_STEEL;	//�d����Ԃ�

		m_Motionstate = MOTION_FLOATDAMAGE;
		if (m_nCounterAllFrame == 3 && m_Motionstate == MOTION_FLOATDAMAGE)
		{//3�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 33.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
		}
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_FLOATDAMAGE)
		{//3�t���[���̂Ƃ�
			ModelMtx = m_apModel[1]->GetMatrix();

		}
	}
	else if (bAction[BOSSBOOLTYPE_IMPACTDAMAGE])
	{
		//�T�E���h�̎擾
		CSound *pSound;
		pSound = CManager::GetSound();

		m_Motionstate = MOTION_IMPACTDAMAGE;
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_IMPACTDAMAGE)
		{//1�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
			m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_IMPACTDAMAGE)
		{//1�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(m_pos, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
			m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		}
	}
	else if (bAction[BOSSBOOLTYPE_SKILL] == true)
	{// �_���[�W���n
		m_nCooltime = 80;
		m_Motionstate = MOTION_SKILL; // ���K�E�Z���[�V�����ɐݒ�
		ModelMtx = m_apModel[1]->GetMatrix();
		if (m_nCounterAllFrame == 4 && m_Motionstate == MOTION_SKILL)
		{//6�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FLASH);	//�����o
		}
		if (m_nCounterAllFrame == 5 && m_Motionstate == MOTION_SKILL)
		{//15�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			CEffect::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43 - 15.5f), 150.0f, 6, CEffect::EFFECTTYPE_SMALL, CEffect::EFFECTMODE_SHRINKING);
		}
		if (m_nCounterAllFrame == 20 && m_Motionstate == MOTION_SKILL)
		{//60�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE3);//����

		}
		if (m_nCounterAllFrame >= 20 && m_nCounterAllFrame <= 120 && m_Motionstate == MOTION_SKILL)
		{//60�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();

			for (int nCntParticle = 0; nCntParticle < 6; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SKILL_ENEMY, 5);
			}
			for (int nCntParticle = 0; nCntParticle < 12; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SKILL_ENEMY, 5);
			}
		}
		if (m_nCounterAllFrame == 40 && m_Motionstate == MOTION_SKILL)
		{//60�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�

			CScene::ChangeStopBoss();

		}

		if (m_nCounterAllFrame == 75 && m_Motionstate == MOTION_SKILL)
		{//20�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 60.0f, CCollision::COLLSIONTYPE_BOSS_SKILLATTACK_S, m_pos);
		}
		if (m_nCounterAllFrame == 110 && m_Motionstate == MOTION_SKILL)
		{//20�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, 40.0f, CCollision::COLLSIONTYPE_BOSS_SKILLATTACK_L, m_pos);
		}
	}
	else if (bAction[BOSSBOOLTYPE_DEATH])
	{


		m_Motionstate = MOTION_DEATH;
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_DEATH)
		{//1�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
			m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_DEATH)
		{//28�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(m_pos, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
			m_state = BOSSSTATE_STEEL;	//�d����Ԃ�
		}
		if (m_nCounterAllFrame == 78 && m_Motionstate == MOTION_DEATH)
		{//78�t���[���̂Ƃ�
			m_state = BOSSSTATE_DEATH;
		}
	}
	else if (m_move.x > ENEMY_MOVE_DEFAULT ||
		m_move.x < -ENEMY_MOVE_DEFAULT ||
		m_move.z >  ENEMY_MOVE_DEFAULT ||
		m_move.z < -ENEMY_MOVE_DEFAULT)
	{// �G�������Ă���Ƃ�
		m_Motionstate = MOTION_MOVE; // �ړ���Ԃɂ���
	}
	else
	{// �G�������ĂȂ��Ƃ�
		m_Motionstate = MOTION_NEUTRAL; // �j���[�g������Ԃɂ���
	}
}


//=============================================================================
// �֐����F�p�x�̏C��
// �֐��̊T�v�F�傫����΁A�����A��������΁A����
//=============================================================================
void CBoss::BossRotFixes(D3DXVECTOR3 rot)
{
	// X���̏C��
	if (rot.x > D3DX_PI)
	{// +3.14���A�����Ă��܂�����
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{// -3.14���A���������
		rot.x += D3DX_PI * 2.0f;
	}
	// Y���̏C��
	if (rot.y > D3DX_PI)
	{// +3.14���A�����Ă��܂�����
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{// -3.14���A���������
		rot.y += D3DX_PI * 2.0f;
	}
	// Z���̏C��
	if (rot.z > D3DX_PI)
	{// +3.14���A�����Ă��܂�����
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{// -3.14���A���������
		rot.z += D3DX_PI * 2.0f;
	}
}
//=============================================================================
// �֐����F�p�x�̏C��
// �֐��̊T�v�F�傫����΁A�����A��������΁A����
//=============================================================================
void CBoss::BossRotFixes(float rot)
{
	if (rot > D3DX_PI)
	{// +3.14���A�����Ă��܂�����
		rot -= D3DX_PI * 2.0f;
	}
	else if (rot < -D3DX_PI)
	{// -3.14���A���������
		rot += D3DX_PI * 2.0f;
	}

}
//=============================================================================
// �֐����F�G���̓ǂݍ���
// �֐��̊T�v�F�G���̓ǂݍ��݂ƃp�[�c�̓ǂݍ���
//=============================================================================
void CBoss::LoadBossText(void)
{
	FILE *pFile = fopen(MODEL_FILENAME_00, "r");	// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

	char acLine[MAX_CHAR];							// 1�s���ǂݍ��ޕϐ�
	char acData[MAX_CHAR];							// 1����ǂݍ��ޕϐ�
	int nCntMotion = 0;								// ���[�V�����̃J�E���^
	int nCntKey = 0;								// �L�[�̃J�E���^
	int nCntParts = 0;								// �p�[�c�̃J�E���^
	int nCntModel = 0;								// �ǂݍ��ރ��f���̃J�E���^
	int nCntTexture = 0;							// �ǂݍ��ރe�N�X�`���̃J�E���^
	int nIndex = 0;									// �g�p���郂�f���̃^�C�v
	int nParent = 0;								// �e�̐���
	int nMaxParts = 0;								// �p�[�c���̍ő吔
	D3DXVECTOR3 pos;								// �ʒu������ϐ�
	D3DXVECTOR3 rot;								// ����������ϐ�

	if (pFile != NULL)
	{
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[

			if (memcmp(acData, "CHARACTERSET", 12) == 0)
			{// "CHARACTERSET"�̕����񂪂�������
				nCntModel = 0;
				while (strcmp(acData, "END_CHARACTERSET") != 0)
				{// "END_CHARACTERSET"���܂ŏ����擾��������
					fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
					sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[
					if (strcmp(acData, "PARTSSET") == 0)
					{// "MODELSET"�̕����񂪂�������
						while (strcmp(acData, "END_PARTSSET") != 0)
						{// "END_PARTSSET"���܂ŏ����擾��������
							fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
							sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[
							if (strcmp(acData, "INDEX") == 0)
							{// "TYPE"�̕����񂪂�������
								sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nIndex);						// �g�p���郂�f���̔ԍ����擾
								m_aIndex[nCntModel] = nIndex;
							}
							else if (strcmp(acData, "PARENT") == 0)
							{// "TYPE"�̕����񂪂�������
								sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nParent);						// �g�p���郂�f���̔ԍ����擾
								m_aParent[nCntModel] = nParent;
							}
							else if (strcmp(acData, "POS") == 0)
							{// "POS"�̕����񂪂�������
								sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// �ʒu���擾
								m_Partspos[nCntModel] = pos;
							}
							else if (strcmp(acData, "ROT") == 0)
							{// "ROT"�̕����񂪂�������
								sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// �������擾
																														//�e�L�X�g�̊p�x���ʂ�ɓ����v�Z
								rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
								rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
								rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
								m_Partsrot[nCntModel] = rot;
							}
						}
						m_apModel[nCntModel] = CModel::Create(m_Partspos[nCntModel], m_Partsrot[nCntModel]);		//�p�[�c�̐���
						if (nCntModel == 0)
						{//�̂̃p�[�c�̂Ƃ�
							m_apModel[nCntModel]->SetParent(NULL);
						}
						else
						{//����ȊO�̃p�[�c�̂Ƃ�
							m_apModel[nCntModel]->SetParent(m_apModel[m_aParent[nCntModel]]);
						}
						m_apModel[nCntModel]->BindModel(m_apMesh[nCntModel], m_apBuffMat[nCntModel], m_aNumMat[nCntModel], m_pTexture);
						nCntModel++;
					}//PARTSSET
				}//END_CHARACTERSET
			}//CHARACTERSET
			if (nCntMotion < MOTION_MAX)
			{// ���[�V�����̑����ȏ�͐ݒ肵�Ȃ�
				if (strcmp(acData, "MOTIONSET") == 0)
				{// MOTIONSET�̕����񂪂�������
					nCntKey = 0;
					while (strcmp(acData, "END_MOTIONSET") != 0)
					{// END_MOTIONSET��������܂Ń��[�v����
						fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
						sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

						if (strcmp(acData, "LOOP") == 0)
						{// LOOP����������
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].nLoop);	// ���[�v���邩�ǂ������擾

						}
						else if (strcmp(acData, "NUM_KEY") == 0)
						{// NUM_KEY����������
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].nNumKey);	// �L�[�����擾
						}
						else if (strcmp(acData, "KEYSET") == 0)
						{// KEYSET����������

							nCntParts = 0;// �p�[�c�̃J�E���g�ϐ���������

							while (strcmp(acData, "END_KEYSET") != 0)
							{// END_KEYSET������܂Ń��[�v
								fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
								sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

								if (strcmp(acData, "FRAME") == 0)
								{// FRAME����������
									sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].aKey[nCntKey].nFrame);	// �L�[�̃t���[�������擾
								}
								else if (strcmp(acData, "KEY") == 0)
								{// KEY����������
									while (strcmp(acData, "END_KEY") != 0)
									{// END_KEY������܂Ń��[�v
										fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
										sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[
										if (strcmp(acData, "POS") == 0)
										{// POS����������
											sscanf(&acLine[0],
												"%s %s %f %f %f",
												&acData[0],
												&acData[0],
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].x,		// �e�p�[�c�̃��[�V�����̈ʒu���擾
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].y,		// �e�p�[�c�̃��[�V�����̈ʒu���擾
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].z);	// �e�p�[�c�̃��[�V�����̈ʒu���擾
										}
										else if (strcmp(acData, "ROT") == 0)
										{// ROT����������
											sscanf(&acLine[0],
												"%s %s %f %f %f",
												&acData[0],
												&acData[0],
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].x,		// �e�p�[�c�̃��[�V�����̉�]���擾
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].y,		// �e�p�[�c�̃��[�V�����̉�]���擾
												&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].z);		// �e�p�[�c�̃��[�V�����̉�]���擾
										}//ROT
									}// END_KEY
									nCntParts++;// �p�[�c�̃J�E���g��i�߂�
								}// KEY
							}// END_KEYSET
							nCntKey++;// �L�[�t���[����i�߂�
						}//KEYSET
					}// END_MOTIONSET
					nCntMotion++;// ���[�V������ς���
				}// MOTIONSET
			}//���[�V�����̑����ȏ�͐ݒ肵�Ȃ�
		}//while
		fclose(pFile);// �J�����t�@�C�������
	}//�t�@�C�����J���Ă���
}
//=============================================================================
// �֐����F�G�̏�����
// �֐��̊T�v�F--
//=============================================================================
void CBoss::InitBoss(void)
{
	//�G���
	m_fRadius = ENEMY_COLLISION_RADIUS; // �G�̓����蔻��̔��a
	m_StateCnt = 200;
	m_state = BOSSSTATE_APPEAR;
	m_fDestAngle = D3DX_PI * 0.5f;
	m_fHeight = 80.0f;
	CCutin::Create(CCutin::CUTINMODE_ENEMYAPPEAR);
	CScene::ChangeStopBoss();

	//�O�Ղ̐���
	//m_apModel[12]->CreateOrbit();
}
//=============================================================================
// �֐����F�G�̃��[�V�����̏�����
// �֐��̊T�v�F--
//=============================================================================
void CBoss::InitMotion(void)
{
	m_nNumKey = 2;
	m_pKeyInfo = m_aKeyInfo;
	m_nKey = 0;
	m_nCountMotion = 0;
	m_Motionstate = MOTION_NEUTRAL;
}
//=============================================================================
// �֐����F�p�[�c�̏����ݒ�
// �֐��̊T�v�F�p�[�c�̏����ݒ���s��
//=============================================================================
void CBoss::DefaultParts(void)
{
	for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
	{//���t���[�����̌v�Z
		m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
	}

	for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
	{// �p�[�c�������[�v
	 // ���f���̈ʒu���j���[�g�������[�V�����ɏ�����
		m_PartsDefaultpos[nCntParts] = m_Partspos[nCntParts] + m_aKeyInfo[m_Motionstate].aKey[0].pos[nCntParts];
		m_PartsDefaultrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[0].rot[nCntParts];
	}

}

//=============================================================================
// �֐����F�I�u�W�F�N�g�Ƃ̓����蔻��
// �֐��̊T�v�F�G�ƃI�u�W�F�N�g�Ƃ̓����蔻��
//=============================================================================
void CBoss::CollisionObjectBoss(void)
{
	//�I�u�W�F�N�g�̃|�C���^
	CObject *pObjectTop;

	pObjectTop = (CObject*)GetSceneTop(OBJECT_PRIORITY);
	while (pObjectTop != NULL)//NULL�ɂȂ�܂�
	{
		CObject *pObjectNext;
		pObjectNext = (CObject*)pObjectTop->GetSceneNext(OBJECT_PRIORITY);
		if (pObjectTop->GetObjType() == CScene::OBJTYPE_OBJECT)
		{
			if (pObjectTop->CollisionObject(&m_pos, &m_posold, &m_move, m_fRadius) == true)
			{
				m_bAction[BOSSBOOLTYPE_JUMP] = false;
			}
		}
		pObjectTop = pObjectNext;
	}

}
//=============================================================================
// �֐����F�ǂ̓����蔻��
// �֐��̊T�v�F�G�ƕǂƂ̓����蔻��
//=============================================================================
void CBoss::CollisionWallBoss(void)
{
	//���b�V���E�H�[���̃|�C���^
	CSceneMeshWall *pCylinder;

	pCylinder = (CSceneMeshWall*)GetSceneTop(WALL_PRIORITY);
	while (pCylinder != NULL)//NULL�ɂȂ�܂�
	{
		CSceneMeshWall *pWallNext;
		pWallNext = (CSceneMeshWall*)pCylinder->GetSceneNext(WALL_PRIORITY);
		if (pCylinder->GetObjType() == CScene::OBJTYPE_MESHWALL)
		{
			pCylinder->CollisionWall(&m_pos, &m_posold, 4.1f);

		}
		pCylinder = pWallNext;
	}

}

//=============================================================================
// �֐����F�V�����_�[�̓����蔻��
// �֐��̊T�v�F�G�ƃV�����_�[�Ƃ̓����蔻��
//=============================================================================
void CBoss::CollisionCylinderBoss(void)
{
	CGame::GAMESTATE gamestate = CGame::GetGameState();

	//���b�V���E�H�[���̃|�C���^
	CSceneMeshCylinder *pCylinder;

	pCylinder = (CSceneMeshCylinder*)GetSceneTop(CYLINDER_PRIORITY);
	while (pCylinder != NULL)//NULL�ɂȂ�܂�
	{
		CSceneMeshCylinder *pWallNext;
		pWallNext = (CSceneMeshCylinder*)pCylinder->GetSceneNext(CYLINDER_PRIORITY);
		if (pCylinder->GetObjType() == CScene::OBJTYPE_MESHCYLINDER)
		{
			if (pCylinder->CollisionCylinder(&m_pos) == true)
			{
				if (gamestate == CGame::GAMESTATE_NORMAL && gamestate != CGame::GAMESTATE_CLEAR)
				{
					CGame::SetGameState(CGame::GAMESTATE_CLEAR);
				}
			}

		}
		pCylinder = pWallNext;
	}
}

//=============================================================================
// �֐����F�U���Ƃ̓����蔻��
// �֐��̊T�v�F--
//=============================================================================
void CBoss::CollisionAttack(void)
{
	//�����蔻��̃|�C���^
	CCollision *pCollision;

	//���f���̃}�g���b�N�X���
	D3DMATRIX ModelMtx;

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�R���{�Q�[�W�̃|�C���^
	CComboGauge *pComboGauge;
	pComboGauge = CGame::GetComboGauge(1);

	//�v���C���[�̃|�C���^
	CPlayer *pPlayer;
	pPlayer = (CPlayer*)GetSceneTop(1);



	pCollision = (CCollision*)GetSceneTop(COLLISION_PRIORITY);
	if (m_state == BOSSSTATE_NORMAL || m_state == BOSSSTATE_STEEL)
	{
		while (pCollision != NULL)//NULL�ɂȂ�܂�
		{
			CCollision *pCollisionNext;
			pCollisionNext = (CCollision*)pCollision->GetSceneNext(COLLISION_PRIORITY);
			if (pCollision->GetObjType() == CScene::OBJTYPE_COLLISION)
			{
				if (pCollision->CollisionAttack(&m_pos, &m_rot, m_fHeight) == true)
				{
					for (int nCntDamage = 0; nCntDamage < 9; nCntDamage++)
					{
						m_bAction[nCntDamage] = false;
						m_state = BOSSSTATE_NORMAL; //�ʏ��Ԃ�

					}
					//�R���{�I���t���[���̐ݒ�
					m_nCancelFlame = 300;

					int nTypeHitType = pCollision->GetCollisionType();
					switch (nTypeHitType)
					{
					case HITTYPE_LIGHTPUNCH:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//���C�t�̌���
						m_fLife -= LIGHT_ATACK;
						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(LIGHT_ATACK_GAUGE);

						//�X�L���Q�[�W�̑���
						pPlayer->SetSkill(LIGHT_ATACK_SKILL);

						//�X�R�A�̉��Z
						CScore::AddScore(LIGHT_ATTACK_SCORE);
						//CComboGauge::AddValueSlip(11.0f);
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//��U��

						//m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_MEDIUMPUNCH:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//���C�t�̌���
						m_fLife -= MEDIUM_ATACK;
						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(MEDIUM_ATACK_GAUGE);

						//�X�L���Q�[�W�̑���
						pPlayer->SetSkill(MEDIUM_ATACK_SKILL);

						//�X�R�A�̉��Z
						CScore::AddScore(MEDIUM_ATACK_SCORE);
						//���U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_MEDIUM);	//���U��

						//m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_SHORYUKEN:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//���C�t�̌���
						m_fLife -= FINISH_ATACK;
						//�X�R�A�̉��Z
						CScore::AddScore(FINISH_ATACK_SCORE);
						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(FINISH_ATACK_GAUGE);

						//�X�L���Q�[�W�̑���
						pPlayer->SetSkill(FINISH_ATACK_SKILL);

						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//��U��

						m_bAction[BOSSBOOLTYPE_FLOATDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 10);
						}
						break;
					case HITTYPE_HEAVYPUNCH:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//���C�t�̌���
						m_fLife -= LARGE_ATACK;
						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(LARGE_ATACK_GAUGE);

						//�X�L���Q�[�W�̑���
						pPlayer->SetSkill(LARGE_ATACK_SKILL);

						//�X�R�A�̉��Z
						CScore::AddScore(LARGE_ATACK_SCORE);
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//��U��
						//m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_001, 10);
						}
						break;
					case HITTYPE_SKILL:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//���C�t�̌���
						m_fLife -= 250.0f;
						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(80.0f);
						//�X�R�A�̉��Z
						CScore::AddScore(SKILL_ATACK_SCORE);
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//��U��
						m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_001, 60);
						}

						break;
					case HITTYPE_LIGHTKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(LIGHT_ATACK_GAUGE);

						//�X�L���Q�[�W�̑���
						pPlayer->SetSkill(LIGHT_ATACK_SKILL);

						//���C�t�̌���
						m_fLife -= LIGHT_ATACK;
						//�X�R�A�̉��Z
						CScore::AddScore(LIGHT_ATTACK_SCORE);
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//��U��
						//m_bAction[BOSSBOOLTYPE_NORMALDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_MEDIUMKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(MEDIUM_ATACK_GAUGE);
						//�X�L���Q�[�W�̑���
						pPlayer->SetSkill(MEDIUM_ATACK_SKILL);
						//���C�t�̌���
						m_fLife -= MEDIUM_ATACK;
						//�X�R�A�̉��Z
						CScore::AddScore(MEDIUM_ATACK_SCORE);
						//���U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_MEDIUM);	//���U��
						//m_bAction[BOSSBOOLTYPE_MEDIUMDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_SPINKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(FINISH_ATACK_GAUGE);
						//�X�L���Q�[�W�̑���
						pPlayer->SetSkill(FINISH_ATACK_SKILL);
						//���C�t�̌���
						m_fLife -= FINISH_ATACK;
						//�X�R�A�̉��Z
						CScore::AddScore(FINISH_ATACK_SCORE);
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//��U��
						m_bAction[BOSSBOOLTYPE_FLOATDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_HEAVYKICK:
						m_bAction[BOSSBOOLTYPE_SKILL] = false;

						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(LARGE_ATACK_GAUGE);
						//�R���{�Q�[�W�̑���
						pComboGauge->AddValueSlip(LARGE_ATACK_SKILL);
						//���C�t�̌���
						m_fLife -= LARGE_ATACK;
						//�X�R�A�̉��Z
						CScore::AddScore(LARGE_ATACK_SCORE);
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//��U��
						//m_bAction[BOSSBOOLTYPE_IMPACTDAMAGE] = true;
						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 3.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < HITPARTICLE; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					}
				}
			}
			pCollision = pCollisionNext;
		}
	}
}
//=============================================================================
// �֐����F�v���C���[�Ƃ̓����蔻��
// �֐��̊T�v�F���a�O�֖߂�
//=============================================================================
void CBoss::CollisionPlayer(void)
{
	//�G�̃|�C���^
	CPlayer *pPlayer;

	pPlayer = (CPlayer*)GetSceneTop(1);
	while (pPlayer != NULL)//NULL�ɂȂ�܂�
	{
		CPlayer *pPlayerNext;
		pPlayerNext = (CPlayer*)pPlayer->GetSceneNext(1);
		if (pPlayer->GetObjType() == CScene::OBJTYPE_PLAYER)
		{
			pPlayer->CollisionPlayer(&m_pos, &m_posold, &m_move, &m_rot, 25.0f);

		}
		pPlayer = pPlayerNext;
	}



}

//=============================================================================
// �֐����F�Q�[�W�̐���
// �֐��̊T�v�F�̗̓Q�[�W�̐���
//=============================================================================
void CBoss::CreateGauge(void)
{
	m_apLifeGauge[0] = CUiGauge::Create(D3DXVECTOR3(SCREEN_WIDTH  /2 - 300.0f, SCREEN_HEIGHT /2 + 300.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 800.0f, 30.0f, m_fLife, m_fLife, CUiGauge::GAUGETYPE_UNDER);	//�Q�[�W�̉��n
	m_apLifeGauge[1] = CUiGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 245.0f, SCREEN_HEIGHT /2 + 300.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 700.0f, 20.0f, m_fLife, m_fLife, CUiGauge::GAUGETYPE_RED);	//�̗̓Q�[�W�̉��n
	m_apLifeGauge[2] = CUiGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 245.0f, SCREEN_HEIGHT /2 + 300.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 700.0f, 20.0f, m_fLife, m_fLife, CUiGauge::GAUGETYPE_GREEN);	//�̗̓Q�[�W

}


//=============================================================================
// �֐����F�R���{�I���t���[���̍X�V
// �֐��̊T�v�F0�ɂȂ�����R���{�I��
//=============================================================================
void CBoss::UpdateCancelCombo(void)
{
	//�R���{�Q�[�W�̃|�C���^
	CComboGauge *pComboGauge;
	pComboGauge = CGame::GetComboGauge(1);

	//�L�����Z���t���[���̌��Z
	m_nCancelFlame--;
	if (m_nCancelFlame <= 0)
	{
		m_nCancelFlame = 0;
		pComboGauge->SetComboGauge(0.0f);
		pComboGauge->SetCurrentGauge(0.0f);

	}

}
//=============================================================================
// �֐����F�v���C���[�̒T��
// �֐��̊T�v�F�v���C���[�����a�ɓ����Ă�����true��Ԃ�
//=============================================================================
bool CBoss::FindPlayer(D3DXVECTOR3 *pos, float fRange)
{
	bool bfind = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//�v���C���[�Ƃ̋���
	float fFind = powf(fRange, 2.0f);	//���a

	if (fLength < fFind)
	{//�Ώۂ̒��_���͈͓���������
		bfind = true;
	}
	else
	{
		bfind = false;
	}


	return bfind;
}

//=============================================================================
// �֐����F�v���C���[�ւ̍U��
// �֐��̊T�v�F�v���C���[�����a�ɓ����Ă�����true��Ԃ�
//=============================================================================
bool CBoss::AttackPlayer(D3DXVECTOR3 * pos, float fRange)
{
	bool bfind = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//�v���C���[�Ƃ̋���
	float fFind = powf(fRange, 2.0f);	//���a

	if (fLength < fFind)
	{//�Ώۂ̒��_���͈͓���������
		bfind = true;
	}
	else
	{
		bfind = false;
	}


	return bfind;
}
//=============================================================================
// �֐����F�G�Ƃ̓����蔻��
// �֐��̊T�v�F���a�O�֖߂�
//=============================================================================
bool CBoss::CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius)
{
	bool bRand = false;			// ���n�������ǂ���
	//----------------------------------------------------------
	// ��`�ł̓����蔻��
	//----------------------------------------------------------
	if (m_pos.x - m_fRadius < pPos->x + fRadius &&
		m_pos.x + m_fRadius > pPos->x - fRadius &&
		m_pos.z - m_fRadius < pPos->z + fRadius &&
		m_pos.z + m_fRadius > pPos->z - fRadius)
	{// �I�u�W�F�N�g�͈͓̔���������
		if (m_pos.y + m_fHeight <= pPosOld->y && m_pos.y + m_fHeight > pPos->y)
		{// �ォ��̔���
			pPos->x += (sinf(pRot->y - D3DX_PI) * 1.0f) * 30.0f;
			pPos->z += (cosf(pRot->y - D3DX_PI) * 1.0f) * 30.0f;
		}
	}
	if (m_pos.y + m_fHeight + 10.0f > pPos->y + fRadius &&
		m_pos.y + -5.0f <= pPos->y + fRadius)
	{// �v���C���[���I�u�W�F�N�g�͈͓̔��Ȃ�
		if (m_pos.x - m_fRadius < pPos->x + fRadius && m_pos.x + m_fRadius > pPos->x - fRadius)
		{// �I�u�W�F�N�g���I�u�W�F�N�g��Z���͈͓̔��ɓ�������
			if (m_pos.z - m_fRadius >= pPosOld->z + fRadius &&
				m_pos.z - m_fRadius < pPos->z + fRadius)
			{// ��O����̔���

				pPos->z = (m_pos.z - m_fRadius - fRadius);	// �I�u�W�F�N�g�̈ʒu��߂�

			}
			else if (m_pos.z + m_fRadius <= pPosOld->z - fRadius &&
				m_pos.z + m_fRadius > pPos->z - fRadius)
			{// ������̔���

				pPos->z = m_pos.z + m_fRadius + fRadius;	// �I�u�W�F�N�g�̈ʒu��߂�

			}

		}
		if (m_pos.z - m_fRadius < pPos->z + fRadius && m_pos.z + m_fRadius > pPos->z - fRadius)
		{// Z�����I�u�W�F�N�g�͈͓̔���������
			if (m_pos.x - m_fRadius >= pPosOld->x + fRadius &&
				m_pos.x - m_fRadius < pPos->x + fRadius)
			{// ������̔���

				pPos->x = m_pos.x - m_fRadius - fRadius;	// �I�u�W�F�N�g�̈ʒu��߂�

			}
			else if (m_pos.x + m_fRadius <= pPosOld->x - fRadius &&
				m_pos.x + m_fRadius > pPos->x - fRadius)
			{// �E����̔���

				pPos->x = m_pos.x + m_fRadius + fRadius;			// �I�u�W�F�N�g�̈ʒu��߂�

			}
		}
	}

	return bRand;
}

//=============================================================================
// �֐����F�G�֌W�̃f�o�b�O���O
// �֐��̊T�v�F--
//=============================================================================
void CBoss::BossDebugLog(void)
{
	CDebugProc::Print("\n");
	CDebugProc::Print("�G�ʒu(XYZ): %.1f, %.1f, %.1f�F\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("�G�p�x(XYZ): %.1f, %.1f, %.1f�F\n", m_rot.x, m_rot.y, m_rot.z);
	//CDebugProc::Print("�G�ړ���(XYZ): %.1f, %.1f, %.1f�F\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("m_fSkill: %.1f\n", m_fSkill);

	//CDebugProc::Print("��ԕω��̃J�E���^�F%d\n", m_StateCnt);

	//switch (m_state)
	//{
	//case BOSSSTATE_NORMAL://�ʏ��Ԃ̂Ƃ�
	//	CDebugProc::Print("STATE�F[0]NORMAL\n");
	//	break;
	//case BOSSSTATE_STEEL://�d����Ԃ̂Ƃ�
	//	CDebugProc::Print("STATE�F[1]STEEL\n");
	//	break;
	//case BOSSSTATE_APPEAR://�o�����̂Ƃ�
	//	CDebugProc::Print("STATE�F[4]BOSSSTATE_APPEAR\n");
	//	break;
	//}

	//switch (m_Motionstate)
	//{
	//case MOTION_NEUTRAL://�j���[�g�����̂Ƃ�
	//	CDebugProc::Print("MOTION�F[0]NEUTRAL\n");
	//	break;
	//case MOTION_MOVE://�ړ��̂Ƃ�
	//	CDebugProc::Print("MOTION�F[1]MOVE\n");
	//	break;
	//case MOTION_JUMP://�W�����v�̂Ƃ�
	//	CDebugProc::Print("MOTION�F[3]JUMP\n");
	//	break;
	//case MOTION_LANDING://���n�̂Ƃ�
	//	CDebugProc::Print("MOTION�F[4]LANDING\n");
	//	break;
	//case MOTION_SKILL://���n�̂Ƃ�
	//	CDebugProc::Print("MOTION�F[6]MOTION_SKILL\n");
	//	break;
	//}
	//switch (m_MotionstateOld)
	//{
	//case MOTION_NEUTRAL://�j���[�g�����̂Ƃ�
	//	CDebugProc::Print("MOTION�F[0]NEUTRAL\n");
	//	break;
	//case MOTION_MOVE://�ړ��̂Ƃ�
	//	CDebugProc::Print("MOTION�F[1]MOVE\n");
	//	break;
	//case MOTION_JUMP://�W�����v�̂Ƃ�
	//	CDebugProc::Print("MOTION�F[3]JUMP\n");
	//	break;
	//case MOTION_LANDING://���n�̂Ƃ�
	//	CDebugProc::Print("MOTION�F[4]LANDING\n");
	//	break;
	//}
	//CDebugProc::Print("KEY�F%d / %d\n", m_nCounterKey, m_aKeyInfo[m_Motionstate].nNumKey);
	//CDebugProc::Print("FRAME�F%d / %d (%d / %d)\n", m_nCounterFrame, m_nFrame, m_nCounterAllFrame, m_nAllFrame);
	//CDebugProc::Print("AllFrame�F%d\n", m_nCounterAllFrame);

	//CDebugProc::Print("m_nCooltime�F%d\n", m_nCooltime);


}

//=============================================================================
// �֐����F�v���C���[�̒T��
// �֐��̊T�v�F--
//=============================================================================
void CBoss::UpdateFind(void)
{
	//�v���C���[�̎擾
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerpos = pPlayer->GetPos();

	if (FindPlayer(&playerpos, 70.0f) == true)
	{//�v���C���[���͈͓��ɓ�������
		if (m_state == BOSSSTATE_NORMAL)
		{
			D3DXVECTOR3 DiffPos;
			//�v���C���[�ƓG�̃x�N�g�����Z�o
			DiffPos.x = m_pos.x - playerpos.x;
			DiffPos.y = m_pos.y - playerpos.y;
			DiffPos.z = m_pos.z - playerpos.z;
			D3DXVec3Normalize(&DiffPos, &DiffPos);

			//�x�N�g�������Ɍ������Ĉړ�
			m_move.x -= DiffPos.x * 0.35f;
			m_move.z -= DiffPos.z * 0.35f;

			//�G�̊p�x�ݒ�
			m_fDestAngle = atan2f(m_pos.x - playerpos.x, m_pos.z - playerpos.z);


		}

		if (AttackPlayer(&playerpos, 7.0f) == true)
		{
			if (m_state == BOSSSTATE_NORMAL)
			{
				m_move.x = 0.0f;
				m_move.z = 0.0f;


				for (int nCntAction = 6; nCntAction < 10; nCntAction++)
				{
					if (m_bAction[nCntAction] == false)
					{
						if (m_bAction[BOSSBOOLTYPE_SKILL] == false)
						{
							if (rand() % 60 == 0 && m_nCooltime == 0)
							{
								m_bAction[BOSSBOOLTYPE_LIGHTATTACK] = true;

							}
							else if (rand() % 80 == 0 && m_nCooltime == 0)
							{
								m_bAction[BOSSBOOLTYPE_HEAVYATTACK] = true;

							}
							else if (rand() % 90 == 0 && m_nCooltime == 0 && m_fSkill >= 480.0f)
							{
								m_state = BOSSSTATE_INVISIBLE;	//�d����Ԃ�

								if (m_bAction[BOSSBOOLTYPE_LIGHTATTACK] == false && m_bAction[BOSSBOOLTYPE_HEAVYATTACK] == false)
								{
									CGame::GAMESTATE pGameState;
									pGameState = CGame::GetGameState();
									if (pGameState == CGame::GAMESTATE_NORMAL)
									{
										m_fSkill = 0.0f;
										m_bAction[BOSSBOOLTYPE_SKILL] = true;
										CCutin::Create(CCutin::CUTINMODE_ENEMY);
										CScene::ChangeStopBoss();
									}
								}
							}
						}

					}
				}
			}
		}//���a7�ȓ���
		else
		{
			if (m_state == BOSSSTATE_NORMAL)
			{
				if (rand() % 60 == 0 && m_nCooltime == 0)
				{
					m_bAction[BOSSBOOLTYPE_MOVEATTACK] = true;

				}
			}

		}
	}//���a70�ȓ���
}

//=============================================================================
// �֐����F�ʒu���̎擾
// �֐��̊T�v�F�G�̈ʒu����Ԃ�
//=============================================================================
D3DXVECTOR3 CBoss::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �֐����F�p�x���̎擾
// �֐��̊T�v�F�G�̊p�x����Ԃ�
//=============================================================================
D3DXVECTOR3 CBoss::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �֐����F�ړ��ʏ��̎擾
// �֐��̊T�v�F�G�̈ړ��ʂ�Ԃ�
//=============================================================================
D3DXVECTOR3 CBoss::GetMove(void)
{
	return m_move;
}

//=============================================================================
// �֐����F�v���C���[�̃��C�t�̃`�F�b�N
// �֐��̊T�v�F�v���C���[�̃��C�t�̊m�F
//=============================================================================
void CBoss::BossLifeCheck(void)
{

	if (m_fLife <= 0.0f)
	{
		for (int nCntDamage = 5; nCntDamage < 9; nCntDamage++)
		{
			m_bAction[nCntDamage] = false;
		}
		m_bAction[BOSSBOOLTYPE_DEATH] = true;
		m_fLife = 0.0f;
	}

	if (m_fSkill >= 500.0f)
	{
		m_fSkill = 500.0f;
	}


	m_apLifeGauge[1]->AddValueSlip(m_fLife);
	m_apLifeGauge[2]->AddValue(m_fLife);




}

