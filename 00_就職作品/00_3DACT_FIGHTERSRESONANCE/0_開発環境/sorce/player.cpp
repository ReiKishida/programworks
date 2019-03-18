//=============================================================================
//
// �v���C���[����[���f��] [player.cpp]
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
#include "player.h"
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
#include "ui_gauge.h"
#include "enemy.h"
#include "enemy_pow.h"
#include "enemy_trc.h"
#include "collision.h"
#include "combogauge.h"
#include "boss.h"
#include "effect.h"
#include "cutin.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//=====================================
// �v���C���[���
//=====================================
#define PLAYER_MOVE (0.5f)						//�ړ���
//#define PLAYER_MOVE (1.0f)						//�ړ���

#define PLAYER_MOVE_DEFAULT		(1.0f)			// �v���C���[�������Ă��̈ړ���
#define MAX_CHAR (256)							//�����̍ő吔
#define GRAVITY_NUM (0.8f)						//�d�͂̒l
#define PLAYER_MOTION_BLEND_FRAME	(5)			// ���[�V�����u�����h�̃t���[����
#define JUMP_POWER (25.0f)						//�W�����v��
#define PLAYER_COLLISION_RADIUS		(20.0f)		// �v���C���[�̓����蔻��̔��a

#define SCREEN_LIMIT_MAX_X (5000.0f )			//��ʒ[(�E�[)
#define SCREEN_LIMIT_MIN_X (-120.0f)			//���(���[)

#define SCREEN_LIMIT_MIN_Z (-370.0f)			//���(���[)

#define COLLISION_RADIUS_S (25.0f)	//�����蔻��̔��a
#define COLLISION_RADIUS_M (30.0f)	//�����蔻��̔��a
#define COLLISION_LIFE (1)

#define PARTICLE_FIRE_NUM (10)
#define PARTICLE_BLACKFIRE_NUM (5)
#define PARTICLE_FIRE_LIFE (15)
#define PARTICLE_BLACKFIRE_LIFE (8)

//=====================================
// ���C�t
//=====================================
#define PLAYER_LIFE (500.0f)
#define PLAYER_LIFE_FLAME (570.0f)

//=====================================
// �p�[�e�B�N���̐F
//=====================================
#define PARTICLE_FIRE (D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f))
#define PARTICLE_FIRE_A (D3DXCOLOR(0.8f, 0.2f, 0.0f, 0.2f))

#define PARTICLE_DEST (D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f))
#define PARTICLE_DEST_A (D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.2f))

//=====================================
// �_���[�W��
//=====================================
#define LIGHT_ATACK (9.0f)		//��U��
#define LARGE_ATACK (15.0f)		//���U��


//=====================================
// �D�揇�ʁA�G�̐�
//=====================================
#define ENEMY_PRIORITY (1)						//�G�̗D�揇��
#define COLLISION_PRIORITY (6)					//�����蔻��̗D�揇��
#define WALL_PRIORITY (5)						//�ǂ̗D�揇��
#define OBJECT_PRIORITY (4)						//�I�u�W�F�N�g�̗D�揇��
#define CYLINDER_PRIORITY (3)					//�V�����_�[�̗D�揇��
#define MAXENEMY_NUM (30)

//=====================================
// �ǂݍ��ރe�L�X�g�t�@�C��
//=====================================
#define MODEL_FILENAME ("data/TEXT/motion_player.txt")			//�ǂݍ��ރt�@�C����

//**************************************************************************************************
// �ÓI�����o�ϐ�
//**************************************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;							//���L�e�N�X�`���̃|�C���^
char			   CPlayer::m_acFileName[MAX_PLAYER_PARTS][256] = {};	// �ǂݍ���
LPD3DXMESH		   CPlayer::m_apMesh[MAX_PLAYER_PARTS] = {};			//���b�V�����ւ̃|�C���^
LPD3DXBUFFER	   CPlayer::m_apBuffMat[MAX_PLAYER_PARTS] = {};			//�}�e���A�����ւ̃|�C���^
DWORD			   CPlayer::m_aNumMat[MAX_PLAYER_PARTS] = {};			//�}�e���A���̏��̐�
D3DXVECTOR3		   CPlayer::m_Partspos[MAX_PLAYER_PARTS] = {};			//�p�[�c�̈ʒu
D3DXVECTOR3		   CPlayer::m_Partsrot[MAX_PLAYER_PARTS] = {};			//�p�[�c�̈ʒu
D3DXVECTOR3		   CPlayer::m_PartsDefaultpos[MAX_PLAYER_PARTS] = {};	//�f�t�H���g�̃p�[�c�ʒu
D3DXVECTOR3		   CPlayer::m_PartsDefaultrot[MAX_PLAYER_PARTS] = {};	//�f�t�H���g�̃p�[�c�p�x
D3DXVECTOR3		   CPlayer::m_Motionpos[MAX_PLAYER_PARTS] = {};			//���[�V������������p�ʒu
D3DXVECTOR3		   CPlayer::m_Motionrot[MAX_PLAYER_PARTS] = {};			//���[�V������������p�p�x
int				   CPlayer::m_aParent[MAX_PLAYER_PARTS] = {};			//�p�[�c�̐e
int				   CPlayer::m_aIndex[MAX_PLAYER_PARTS] = {};			//�p�[�c�ԍ�
CMeshOrbit		  *CPlayer::m_pMeshOrbit = NULL;						//�O�Ղ̃|�C���^
CUiGauge		  *CPlayer::m_apUi[MAX_PLAYER_UI] = {};					//UI�̃|�C���^
CCollision		  *CPlayer::m_pCollisionAttack = {};	// �U�����̓����蔻��̃|�C���^
float			   CPlayer::m_fLife = PLAYER_LIFE;								//�v���C���[�̃��C�t
float			   CPlayer::m_fSkill = 0.0f;								//�X�^��
float			   CPlayer::m_fDamage = 0.0f;								//�_���[�W
//========================================================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//========================================================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_bUseDraw = true;
}

//============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//============================================================================
CPlayer::~CPlayer()
{
}


//=============================================================================
// �֐����F�g�p����e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F--
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/character/player.png", &m_pTexture);



	FILE *pFile = fopen(MODEL_FILENAME, "r");		// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

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
void CPlayer::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//�}�e���A���̐��̔j��
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
	{
		if (m_aNumMat[nCntModel] != NULL)
		{
			m_aNumMat[nCntModel] = NULL;

		}
	}

	//���b�V�����̔j��
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
	{
		if (m_apMesh[nCntModel] != NULL)
		{
			m_apMesh[nCntModel]->Release();
			m_apMesh[nCntModel] = NULL;
		}
	}

	//�}�e���A�����̔j��
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_PARTS; nCntModel++)
	{
		if (m_apBuffMat[nCntModel] != NULL)
		{
			m_apBuffMat[nCntModel]->Release();
			m_apBuffMat[nCntModel] = NULL;
		}
	}
}


//=============================================================================
// �֐����F�v���C���[�V�[���̐���
// �֐��̊T�v�F�v���C���[�V�[���𐶐�����
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer  *pPlayer;
	pPlayer = new CPlayer;//�V�[���N���X�̐���
	if (pPlayer != NULL)
	{
		pPlayer->m_pos = pos;
		pPlayer->m_rot.y = -D3DX_PI * 0.5f;
		pPlayer->m_posold = pPlayer->m_pos;
		pPlayer->m_bAction[PLAYERBOOLTYPE_JUMP] = false;
		pPlayer->m_bJump = false;
		pPlayer->Init();
	}
	return pPlayer;
}

//=============================================================================
// �֐����F�v���C���[�V�[���̏���������
// �֐��̊T�v�F�v���C���[���̐ݒ�
//=============================================================================
HRESULT CPlayer::Init(void)
{

	// �v���C���[���̓ǂݍ���
	LoadPlayerText();

	 // ���[�V�������̏�����
	InitMotion();

	// �v���C���[���̏�����
	InitPlayer();

	// �Q�[�W�ނ̐���
	CreateGauge();

	// �p�[�c�̏����ݒ�
	DefaultParts();


	// �v���C���[�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}
//=============================================================================
// �֐����F�v���C���[�V�[���̏I������
// �֐��̊T�v�F���b�V���A�}�e���A���A���g�̔j��
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
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
// �֐����F�v���C���[�V�[���̍X�V����
// �֐��̊T�v�F�v���C���[�̈ړ��A�p�[�c�̃��[�V�����X�V
//=============================================================================
void CPlayer::Update(void)
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
	//�v���C���[�̏��
	//==============================================
	PlayerState();

	//==============================================
	//�v���C���[�̈ړ�
	//==============================================
	PlayerMove();

	//==============================================
	//�v���C���[�̃A�N�V����
	//==============================================
	PlayerAction();


	 //=============================================
	 // ���[�V�����̏�ԍX�V
	 //=============================================
	PlayerMotionState(m_bAction);
	PlayerMotion();

	//==============================================
	//�ʒu�������Ɗ����ړ�
	//==============================================
	PlayerSetMove_Interia();

	//=============================================
	// �����蔻�� (�G)
	//=============================================
	CollisionEnemy();

	//=============================================
	// �����蔻�� (�U��)
	//=============================================
	CollisionAttack();

	//=============================================
	// �����蔻�� (�I�u�W�F�N�g)
	//=============================================
	CollisionObjectPlayer();

	//=============================================
	// �����蔻��(��)
	//=============================================
	CollisionWallPlayer();

	//=============================================
	// �����蔻�� (�V�����_�[)
	//=============================================
	CollisionCylinderPlayer();

	//=============================================
	// �c��̗͂̃`�F�b�N
	//=============================================
	PlayerLifeCheck();


	//=============================================
	// �c��X�L���̃`�F�b�N
	//=============================================
	PlayerSkillCheck();

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
	//�v���C���[�̊p�x����
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	//�p�x���
	m_rot.y += m_fAngle * 0.35f;


	if (mode == CManager::MODE_TUTORIAL)
	{
		m_fSkill += 10.0f;
	}

	//==============================================
	// �����Ɛ���
	//==============================================
	PlayerFallen();


	//==============================================
	// �{�X�̐���
	//==============================================
	if ((m_pos.x >= 3700.0f && m_pos.x <= 4100.0f) && (m_pos.z <= -3450.0f && m_pos.z >= -3500.0f))
	{
		if (CGame::GetBool() == false)
		{
			CGame::ChangeBoolCreateBoss();
		}
	}



	//==============================================
	// �f�o�b�O���O�֌W
	//==============================================
#ifdef _DEBUG
	//PlayerDebugLog();
#endif


	//�ȑO�̃��[�V��������
	m_MotionstateOld = m_Motionstate;


}

//=============================================================================
// �֐����F�v���C���[�V�[���̕`�揈��
// �֐��̊T�v�F�v���C���[�̃}�g���b�N�X�ݒ�A�e�p�[�c�̕`��
//=============================================================================
void CPlayer::Draw(void)
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
// �֐����F�v���C���[�̉e�̕`��
// �֐��̊T�v�F�v���C���[�̉e��`�悷��
//=============================================================================
void CPlayer::DrawShadow(void)
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


	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{
		//���f���̕`��
		m_apModel[nCntParts]->Draw();
	}
	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

//=============================================================================
// �֐����F�v���C���[�̃p�[�c�̕`��
// �֐��̊T�v�F�v���C���[�̃p�[�c��`�悷��
//=============================================================================
void CPlayer::DrawParts(void)
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
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{
			//���f���̕`��
			m_apModel[nCntParts]->Draw();
		}
	}
}


//=============================================================================
// �֐����F�v���C���[�̈ړ�����
// �֐��̊T�v�F�v���C���[�̈ړ��֌W
//=============================================================================
void CPlayer::PlayerMove(void)
{
	//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//�J�����̊p�x�擾
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();



	if (m_state == PLAYERSTATE_NORMAL )
	{//�v���C���[�̏�Ԃňړ��ł��邩
	 //=========================================//
	 //�C�ӂ̃L�[(���L�[)�������ꂽ���ǂ���    //
	 //=======================================//
		if (pInputKeyboard->GetKeyboardPress(DIK_A) == true || pGamePadStick[0].aGamePad.lX < -GAMEPAD_DEADZONE)
		{//A�L�[�������ꂽ�Ƃ�
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
			{//��L�[(W�L�[)�������ꂽ
				m_move.x += sinf(-D3DX_PI * 0.25f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(-D3DX_PI * 0.25f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * 0.75f) + CameraRot.y;


			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
			{//���L�[(���L�[)�������ꂽ
				m_move.x += sinf(-D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(-D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * 0.25f) + CameraRot.y;


			}
			else
			{//���L�[(���L�[)�̂�
				m_move.x += sinf((-D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf((-D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;

				m_fDestAngle = (D3DX_PI * 0.5f + CameraRot.y);
			}
		}
		//=========================================
		//�C�ӂ̃L�[(D�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true || pGamePadStick[0].aGamePad.lX > GAMEPAD_DEADZONE)
		{
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
			{//��L�[(���L�[)�������ꂽ
				m_move.x += sinf(D3DX_PI *  0.25f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(D3DX_PI *  0.25f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * -0.75f) + CameraRot.y;

			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
			{//���L�[(���L�[)�������ꂽ
				m_move.x += sinf(D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * -0.25f) + CameraRot.y;
			}
			else
			{//�E�L�[(���L�[)�̂�
				m_move.x += sinf((D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf((D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
				m_fDestAngle = (D3DX_PI * -0.5f) + CameraRot.y;
			}
		}
		//=========================================
		//�C�ӂ̃L�[(W�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
		{//���L�[�������ꂽ�Ƃ�
			m_move.x += sinf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
			m_move.z += cosf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
			m_fDestAngle = (D3DX_PI)+CameraRot.y;
		}
		//=========================================//
		//�C�ӂ̃L�[(S�L�[)�������ꂽ���ǂ���	  //
		//=======================================//
		else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
		{//���L�[�������ꂽ�Ƃ�
			m_move.x -= sinf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
			m_move.z -= cosf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;

			m_fDestAngle = (D3DX_PI * 0.0f) + CameraRot.y;
		}


	}//(��)�v���C���[�̏�Ԃňړ��ł��邩

	 //�J�����̊p�x����
	if (CameraRot.y > D3DX_PI)
	{
		CameraRot.y -= D3DX_PI * 2;
	}

	if (CameraRot.y < -D3DX_PI)
	{
		CameraRot.y += D3DX_PI * 2;
	}
}

//=============================================================================
// �֐����F�v���C���[�̃A�N�V����
// �֐��̊T�v�F�U���n�̏���
//=============================================================================
void CPlayer::PlayerAction(void)
{
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	CComboGauge *pComboGauge;
	pComboGauge = CGame::GetComboGauge(1);

	//=======================================================//
	//�C�ӂ̃L�[(U�L�[)�������ꂽ���ǂ���(��p���`)			//
	//=====================================================//

			if (pInputKeyboard->GetKeyboardTrigger(DIK_U) == true || pGamePad->GetGamePadTrigger(0, BUTTON_X) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
			{//U�L�[�������ꂽ�Ƃ�

						if (m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_2] == false && m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_1] == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
						{
							m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_2] = true;
							m_bAction[PLAYERBOOLTYPE_SHORYUKEN] = true;
						}
				if (m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_1] == false && m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_0] == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
				{
					m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_1] = true;
					m_bAction[PLAYERBOOLTYPE_MIDDLEPUNCH] = true;
				}
				else if (m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_0] == false && m_state == PLAYERSTATE_NORMAL && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
				{
					m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_0] = true;
					m_bAction[PLAYERBOOLTYPE_LIGHTPUNCH] = true;
				}
			}

	//=======================================================//
	//�C�ӂ̃L�[(I�L�[)�������ꂽ���ǂ���(���p���`)			//
	//=====================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_I) == true || pGamePad->GetGamePadTrigger(0, BUTTON_Y) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false )
	{
		if (m_state == PLAYERSTATE_NORMAL)
		{
			for (int nCnt = 11; nCnt <= 12; nCnt++)
			{
				if (m_bAction[nCnt] == false)
				{
					m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_HEAVY] = true;
					m_bAction[PLAYERBOOLTYPE_HEAVYPUNCH] = true;
				}
			}
		}
	}




	//=======================================================//
	//�C�ӂ̃L�[(U�L�[)�������ꂽ���ǂ���(��L�b�N)			//
	//=====================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_J) == true || pGamePad->GetGamePadTrigger(0, BUTTON_A) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
	{//�G���^�[�L�[�������ꂽ�Ƃ�

		for (int nCnt = 11; nCnt <= 12; nCnt++)
		{
			if (m_bAction[nCnt] == false)
			{
				if (m_bAttackKick[ATTACKBOOLTYPE_KICK_2] == false && m_bAttackKick[ATTACKBOOLTYPE_KICK_1] == true)
				{
					m_bAttackKick[ATTACKBOOLTYPE_KICK_2] = true;
					m_bAction[PLAYERBOOLTYPE_SPINKICK] = true;
				}
			}
		}
		if (m_bAttackKick[ATTACKBOOLTYPE_KICK_1] == false && m_bAttackKick[ATTACKBOOLTYPE_KICK_0] == true)
		{
			m_bAttackKick[ATTACKBOOLTYPE_KICK_1] = true;
			m_bAction[PLAYERBOOLTYPE_MIDDLEKICK] = true;
		}
		else if (m_bAttackKick[ATTACKBOOLTYPE_KICK_0] == false && m_state == PLAYERSTATE_NORMAL)
		{
			m_bAttackKick[ATTACKBOOLTYPE_KICK_0] = true;
			m_bAction[PLAYERBOOLTYPE_LIGHTKICK] = true;
		}

	}
	//=======================================================//
	//�C�ӂ̃L�[(I�L�[)�������ꂽ���ǂ���(���L�b�N)			//
	//=====================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_K) == true || pGamePad->GetGamePadTrigger(0, BUTTON_B) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
	{
		if (m_state == PLAYERSTATE_NORMAL)
		{
			for (int nCnt = 11; nCnt <= 12; nCnt++)
			{
				if (m_bAction[nCnt] == false)
				{
					m_bAttackKick[ATTACKBOOLTYPE_KICK_HEAVY] = true;
					m_bAction[PLAYERBOOLTYPE_HEAVYKICK] = true;
				}
			}
		}
	}



	//===============================================================//
	//�C�ӂ̃L�[(O�L�[)�������ꂽ���ǂ���(���K�E�Z)					//
	//=============================================================//
	if (pInputKeyboard->GetKeyboardTrigger(DIK_O) == true || pGamePad->GetGamePadTrigger(0, BUTTON_R1) == true && m_bAction[PLAYERBOOLTYPE_JUMP] == false)
	{//�G���^�[�L�[�������ꂽ�Ƃ�
		if (m_fSkill >= PLAYER_LIFE)
		{
			for (int nCnt = 11; nCnt <= 12; nCnt++)
			{
				if (m_bAction[nCnt] == false)
				{
					if (m_state == PLAYERSTATE_NORMAL)
					{

							m_state = PLAYERSTATE_INVISIBLE;

							m_bAction[PLAYERBOOLTYPE_SKILL] = true;
							CCutin::Create(CCutin::CUTINMODE_PLAYER);
							m_fSkill = 0.0f;
							CScene::ChangeStop();

							m_StateCnt = 222;

					}
				}
			}
		}
	}


	//=============================================//
	//�C�ӂ̃L�[(�X�y�[�X�L�[)�������ꂽ���ǂ���  //
	//===========================================//u
	if ((pInputKeyboard->GetKeyboardTrigger(DIK_L) == true || pGamePad->GetGamePadTrigger(0, BUTTON_R2) == true )&& m_state == PLAYERSTATE_NORMAL)
	{//�X�y�[�X�L�[�������ꂽ�Ƃ�
		for (int nCnt = 11; nCnt <= 12; nCnt++)
		{
			if (m_bAction[nCnt] == false)
			{
				if (m_bAction[PLAYERBOOLTYPE_JUMP] == false)
				{
					m_bAction[PLAYERBOOLTYPE_JUMP] = true;
					m_bJump = true;
					m_move.y += JUMP_POWER;
				}
			}
		}
	}


	if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true)
	{
		m_fSkill += 100.0f;
	}
}

//=============================================================================
// �֐����F�v���C���[�̗���
// �֐��̊T�v�F�����ƒn�ʂւ̐���
//=============================================================================
void CPlayer::PlayerFallen(void)
{
	//����
	m_move.y -= GRAVITY_NUM;

	if (m_pos.y <= 0.0f)
	{//���̍����ȉ���������
		m_pos.y = 0.0f;//������n�ʂ̍�����
		m_move.y = 0.0f;
		if (m_bAction[PLAYERBOOLTYPE_JUMP] == true)
		{
			if (m_move.x < 0.2f && m_move.x > -0.2f && m_move.z < 0.2f && m_move.z > -0.2f)
			{
				m_state = PLAYERSTATE_STEEL;
				m_bAction[PLAYERBOOLTYPE_LANDING] = true;//���n��Ԃ�
				m_bAction[PLAYERBOOLTYPE_JUMP] = false;	//�W�����v��Ԃ�����
				m_bJump = false;//�W�����v��Ԃ�����
			}
			m_bAction[PLAYERBOOLTYPE_JUMP] = false;//�W�����v�ł����Ԃ�
			m_bJump = false;//�W�����v��Ԃ�����
		}

	}
	if (m_pos.y <= 0.0f && m_posold.y >= 1.0f)
	{//���̍����ȉ���������
		CreateParticleDust();
	}
}

//=============================================================================
// �֐����F�v���C���[�̃��[�V����
// �֐��̊T�v�F�u�����h�ݒ��A���[�V����
//=============================================================================
void CPlayer::PlayerMotion(void)
{
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//-----------------------------------------------------------------------
	// ���[�V�����u�����h�̐ݒ�
	//-----------------------------------------------------------------------
	if (m_Motionstate != m_MotionstateOld)
	{// ���[�V�����̃X�e�[�g���ς������
		m_nCounterFrame = 0;						// ���݂̃L�[�t���[���̃t���[������������
		m_nCounterKey = 0;							// �L�[�t���[���̏�����
		m_nCounterAllFrame = 0;						// ���[�V�����̃t���[������������
		m_nFrame = PLAYER_MOTION_BLEND_FRAME;		// ���[�V�����u�����h�̃t���[������ݒ�
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
				{// �v���C���[�̃��[�V������

				case MOTION_SLIDING://�X���C�f�B���O�̏ꍇ
					m_bAction[PLAYERBOOLTYPE_SLIDING] = false;
					m_nCounterKey--;

				case MOTION_JUMP: // �W�����v���[�V������������
					m_nCounterKey--; // �L�[�t���[����߂�
					break;

				case MOTION_LANDING: // ���n���[�V������������
					m_bAction[PLAYERBOOLTYPE_LANDING] = false;
					m_nCounterKey--; // �L�[�t���[����߂�
					break;

				case MOTION_LIGHTPUNCH:
					m_bAction[PLAYERBOOLTYPE_LIGHTPUNCH] = false;
					m_bAttackPunch[0] = false;

					m_nCounterKey--;
					break;
				case MOTION_MIDDLEPUNCH:
					m_bAction[PLAYERBOOLTYPE_MIDDLEPUNCH] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// �U������������
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_SHORYUKEN:
					m_bAction[PLAYERBOOLTYPE_SHORYUKEN] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// �U������������
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_HEAVYPUNCH:
					m_bAction[PLAYERBOOLTYPE_HEAVYPUNCH] = false;
					m_nCounterKey--;
					break;
				case MOTION_LIGHTKICK:
					m_bAction[PLAYERBOOLTYPE_LIGHTKICK] = false;
					m_bAttackKick[0] = false;
					m_nCounterKey--;
					break;
				case MOTION_MIDDLEKICK:
					m_bAction[PLAYERBOOLTYPE_MIDDLEKICK] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// �U������������
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_SPINKICK:
					m_bAction[PLAYERBOOLTYPE_SPINKICK] = false;
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// �U������������
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_nCounterKey--;
					break;
				case MOTION_HEAVYKICK:
					m_bAction[PLAYERBOOLTYPE_HEAVYKICK] = false;
					m_nCounterKey--;
					break;
				case MOTION_SMALLDAMAGE: // �_���[�W���[�V������������
					for (int nCntmotion = 0; nCntmotion <= 10; nCntmotion++)
					{
						m_bAction[nCntmotion] = false;
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// �U������������
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// �U������������
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = false;// �_���[�W������Ȃ���Ԃɂ���
					m_bAction[PLAYERBOOLTYPE_MEDIUMDAMAGE] = false;// �_���[�W������Ȃ���Ԃɂ���
					m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = false;// �_���[�W������Ȃ���Ԃɂ���

					m_nCounterKey--; // �L�[�t���[����߂�
					break;
				case MOTION_LARGEDAMAGE:
					for (int nCntmotion = 0; nCntmotion <= 10; nCntmotion++)
					{
						m_bAction[nCntmotion] = false;
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_PUNCH_MAX; nCntAttack++)
					{
						if (m_bAttackPunch[nCntAttack] == true)
						{// �U������������
							m_bAttackPunch[nCntAttack] = false;
						}
					}
					for (int nCntAttack = 0; nCntAttack < ATTACKBOOLTYPE_KICK_MAX; nCntAttack++)
					{
						if (m_bAttackKick[nCntAttack] == true)
						{// �U������������
							m_bAttackKick[nCntAttack] = false;
						}
					}
					m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = false;// �_���[�W������Ȃ���Ԃɂ���
					m_bAction[PLAYERBOOLTYPE_MEDIUMDAMAGE] = false;// �_���[�W������Ȃ���Ԃɂ���
					m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = false;// �_���[�W������Ȃ���Ԃɂ���

					m_nCounterKey--; // �L�[�t���[����߂�
					break;
				case MOTION_SKILL:
					m_bAction[PLAYERBOOLTYPE_SKILL] = false;// �_���[�W������Ȃ���Ԃɂ���
					CScene::SetStop(false);
					m_nCounterKey--; // �L�[�t���[����߂�
					break;
				case MOTION_DEATH://���S�����ꍇ
					break;
				}
				if (m_nCounterKey <= 0)
				{
					m_nCounterKey = 0;
				}
				m_state = PLAYERSTATE_NORMAL;
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
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{// �p�[�c�������[�v
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

	//----------------------------------------------
	// ���[�V�����̓���
	//----------------------------------------------
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{// �p�[�c�������[�v
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

	m_nCounterFrame++;		// ���݃L�[�t���[���̃t���[���������Z
	m_nCounterAllFrame++;	// ���[�V�����̃t���[���������Z

}

//=============================================================================
// �֐����F�v���C���[�̏�ԕω�
// �֐��̊T�v�F�v���C���[�̏�ԕω�
//=============================================================================
void CPlayer::PlayerState(void)
{
	switch (m_state)
	{
	case PLAYERSTATE_NORMAL:	//�ʏ���
		break;

	case PLAYERSTATE_DAMAGE:	//�_���[�W���
		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_StateCnt = 120;
			m_state = PLAYERSTATE_INVISIBLE;
		}
		break;

	case PLAYERSTATE_INVISIBLE:	//�������
		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_bUseDraw = true;
			m_state = PLAYERSTATE_NORMAL;
		}
		break;
	case PLAYERSTATE_APPEAR:	//�o����
		m_StateCnt--;
		if (m_StateCnt <= 0)
		{
			m_state = PLAYERSTATE_NORMAL;
		}
		break;
		case PLAYERSTATE_DEATH:	//�o����
			CGame::GAMESTATE pGameState;
			pGameState = CGame::GetGameState();
			if (pGameState == CGame::GAMESTATE_NORMAL)
			{
				CGame::SetGameState(CGame::GAMESTATE_CLEAR);
			}
			break;
	}

}

//=============================================================================
// �֐����F�v���C���[�̈ړ��ʂ̑���Ɗ����ړ�
// �֐��̊T�v�F--
//=============================================================================
void CPlayer::PlayerSetMove_Interia(void)
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
// �֐����F�v���C���[�̃��[�V�����X�e�[�g�̕ύX
// �֐��̊T�v�F�v���C���[�̏�ԑJ��
//=============================================================================
void CPlayer::PlayerMotionState(bool *bAction)
{
	//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//�J�����̊p�x�擾
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//���f���̃}�g���b�N�X���
	D3DMATRIX ModelMtx;


	if (bAction[PLAYERBOOLTYPE_DEATH] == true)
	{// ���񂾂Ȃ�
		m_Motionstate = MOTION_DEATH; // ���S���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_DEATH)
		{//1�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -15.0f;
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_DEATH)
		{//1�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SLIDING] == true)
	{// �X���C�f�B���O
		m_Motionstate = MOTION_SLIDING; // �U�����[�V�����ɐݒ�
		if (m_nCounterAllFrame >= 18 && m_nCounterAllFrame <= 22 &&m_Motionstate == MOTION_SLIDING)
		{
			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{//�p�[�e�B�N���̐���
				CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_SLIDING, 20);
			}
		}
		if (m_nCounterAllFrame == 20 && m_Motionstate == MOTION_SLIDING)
		{//20�t���[�����X���C�f�B���O���[�V�����̂Ƃ�
			m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
			//�ړ��ʂ̉��Z
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 7.5f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 7.5f);
		}
	}
	else if(bAction[PLAYERBOOLTYPE_LIGHTPUNCH] == true)
	{
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_LIGHTPUNCH;	//��p���`���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 7 && m_Motionstate == MOTION_LIGHTPUNCH)
		{//7�t���[������p���`���[�V�����̂Ƃ�
			//�E��̃}�g���b�N�X���̎擾
			ModelMtx = m_apModel[9]->GetMatrix();
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_0);	//�v���C���[�̍U���P
			//�v���C���[�̈ړ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.7f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.7f);
		}
		if (m_nCounterAllFrame == 13 &&m_Motionstate == MOTION_LIGHTPUNCH)
		{//7�t���[������p���`���[�V�����̂Ƃ�
		 //����̔���
			ModelMtx = m_apModel[9]->GetMatrix();
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 , ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_S, CCollision::COLLISIONTYPE_PLAYER_LIGHTPUNCH, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_MIDDLEPUNCH] == true)
	{//�v���C���[�����p���`�������Ȃ�
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_MIDDLEPUNCH;	//���p���`���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_MIDDLEPUNCH)
		{//8�t���[�������p���`���[�V�����̂Ƃ�
			 //�E��̃}�g���b�N�X���̎擾
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//�v���C���[�̍U���P
			//�v���C���[�̈ړ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.2f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.2f);
		}
		if (m_nCounterAllFrame == 13 && m_Motionstate == MOTION_MIDDLEPUNCH)
		{//13�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 , ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_S, CCollision::COLLISIONTYPE_PLAYER_MEDIUMPUNCH, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SHORYUKEN] == true)
	{//�v���C���[���������������Ȃ�
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_SHORYUKEN;	//���������[�V�����ɐݒ�
		if (m_nCounterAllFrame == 5 && m_Motionstate == MOTION_SHORYUKEN)
		{//13�t���[�����������̂Ƃ�
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_2);	//�v���C���[�̍U���P

		}
		if (m_nCounterAllFrame == 13 && m_Motionstate == MOTION_SHORYUKEN)
		{//13�t���[�����������̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 9.0f);
			m_move.y += 22.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 9.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE2);//����

		}
		if (m_nCounterAllFrame > 19 && m_nCounterAllFrame < 28 && m_Motionstate == MOTION_SHORYUKEN)
		{//19�t���[���ȏ�28�t���[���ȉ��̂Ƃ��A�������̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			CreateParticleFlame_Shoryu(&ModelMtx);
		}
		if (m_nCounterAllFrame == 12  && m_Motionstate == MOTION_SHORYUKEN)
		{//12�t���[���̂Ƃ��A�������̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 , ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_M, CCollision::COLLISIONTYPE_PLAYER_SHORYUKEN, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_HEAVYPUNCH] == true)
	{//�v���C���[�����U���������Ȃ�
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_HEAVYPUNCH;	//���p���`���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//6�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//�v���C���[�̍U���P
		}
		if (m_nCounterAllFrame == 9 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//9�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);//����
		}
		if (m_nCounterAllFrame == 13 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//13�t���[�������p���`���[�V�����̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 3.0f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 3.0f);

		}
		if (m_nCounterAllFrame > 15 && m_nCounterAllFrame <= 21 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//19�t���[���ȏ�̂Ƃ����p���`�̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			CreateParticleFlame_SpinKick(&ModelMtx);
		}
		if (m_nCounterAllFrame == 21 && m_Motionstate == MOTION_HEAVYPUNCH)
		{//31�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 35.0f, CCollision::COLLISIONTYPE_PLAYER_HEAVYPUNCH, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_LIGHTKICK] == true)
	{//��L�b�N
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_LIGHTKICK;	//��L�b�N���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 5 && m_Motionstate == MOTION_LIGHTKICK)
		{//7�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_0);	//�v���C���[�̍U���P
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.2f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.2f);
		}
		if (m_nCounterAllFrame == 15 && m_Motionstate == MOTION_LIGHTKICK)
		{//12�t���[���̂Ƃ��A�������̂Ƃ�
			ModelMtx = m_apModel[12]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, COLLISION_RADIUS_M, CCollision::COLLISIONTYPE_PLAYER_LIGHTKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_MIDDLEKICK] == true)
	{//���L�b�N
		m_state = PLAYERSTATE_STEEL;		//�d����Ԃ�
		m_Motionstate = MOTION_MIDDLEKICK;	//���L�b�N���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_MIDDLEKICK)
		{//7�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//�v���C���[�̍U���P
			m_move.x += (sinf(m_rot.y - D3DX_PI)) * 1.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI)) * 1.0f;
		}
		if (m_nCounterAllFrame == 16 && m_Motionstate == MOTION_MIDDLEKICK)
		{//12�t���[���̂Ƃ��A�������̂Ƃ�
			ModelMtx = m_apModel[11]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 35.0f, CCollision::COLLISIONTYPE_PLAYER_MEDIUMKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SPINKICK] == true)
	{//��]�L�b�N

		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_SPINKICK;	//��]�L�b�N���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 8 && m_Motionstate == MOTION_SPINKICK)
		{//8�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_2);	//�v���C���[�̍U���P
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE2);//����
		}

		if (m_nCounterAllFrame == 14 && m_Motionstate == MOTION_SPINKICK)
		{//14�t���[�������p���`���[�V�����̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 10.0f);
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 10.0f);
		}
		if (m_nCounterAllFrame > 15 && m_nCounterAllFrame < 27 && m_Motionstate == MOTION_SPINKICK)
		{//12�t���[���ȏ�̂Ƃ���]�R��̂Ƃ�
			ModelMtx = m_apModel[15]->GetMatrix();
			CreateParticleFlame_SpinKick(&ModelMtx);
		}
		if (m_nCounterAllFrame == 31 && m_Motionstate == MOTION_SPINKICK)
		{//31�t���[������]�R�胂�[�V�����̂Ƃ�
			ModelMtx = m_apModel[15]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 40.0f, CCollision::COLLISIONTYPE_PLAYER_SPINKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_HEAVYKICK] == true)
	{//��]�L�b�N
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_HEAVYKICK;	//���L�b�N���[�V�����ɐݒ�
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_HEAVYKICK)
		{//6�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_1);	//�v���C���[�̍U���P
		}
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_HEAVYKICK)
		{//12�t���[�������p���`���[�V�����̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.4f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.4f);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE3);//����

		}
		if (m_nCounterAllFrame > 13 && m_nCounterAllFrame < 19 && m_Motionstate == MOTION_HEAVYKICK)
		{//8�t���[���ȏ�̂Ƃ����L�b�N�̂Ƃ�
			ModelMtx = m_apModel[12]->GetMatrix();
			CreateParticleFlame_Shoryu(&ModelMtx);
		}
		if (m_nCounterAllFrame == 20 && m_Motionstate == MOTION_HEAVYKICK)
		{//20�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[12]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 35.0f, CCollision::COLLISIONTYPE_PLAYER_HEAVYKICK, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_SKILL] == true)
	{// �_���[�W���n
		m_Motionstate = MOTION_SKILL; // ���K�E�Z���[�V�����ɐݒ�
		ModelMtx = m_apModel[1]->GetMatrix();
		if (m_nCounterAllFrame == 6 && m_Motionstate == MOTION_SKILL)
		{//6�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FLASH);	//�����o
		}
		if (m_nCounterAllFrame == 15 && m_Motionstate == MOTION_SKILL)
		{//15�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			CEffect::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43 - 15.5f), 150.0f, 6, CEffect::EFFECTTYPE_SMALL, CEffect::EFFECTMODE_SHRINKING);
		}
		if (m_nCounterAllFrame == 60 && m_Motionstate == MOTION_SKILL)
		{//60�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE3);//����

		}
		if (m_nCounterAllFrame >= 60 && m_nCounterAllFrame <= 222 && m_Motionstate == MOTION_SKILL)
		{//60�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			CreateParticleFlame_Skill(&ModelMtx);
		}
		if (m_nCounterAllFrame == 100 && m_Motionstate == MOTION_SKILL)
		{//60�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			CScene::ChangeStop();

		}
		if (m_nCounterAllFrame == 170 && m_Motionstate == MOTION_SKILL)
		{//60�t���[���ȏ�̂Ƃ��X�L���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 6.0f);
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 6.0f);
		}
		if (m_nCounterAllFrame == 170 && m_Motionstate == MOTION_SKILL)
		{//6�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERATTACK_2);	//�v���C���[�̍U���P
		}
		if (m_nCounterAllFrame == 170 && m_Motionstate == MOTION_SKILL)
		{//9�t���[�������p���`���[�V�����̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);//����
		}
		if (m_nCounterAllFrame == 180 && m_Motionstate == MOTION_SKILL)
		{//20�t���[�������p���`���[�V�����̂Ƃ�
			ModelMtx = m_apModel[6]->GetMatrix();
			//����̔���
			CCollision::CreateAttack(D3DXVECTOR3(ModelMtx._41, ModelMtx._42, ModelMtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLLISION_LIFE, 45.0f, CCollision::COLLISIONTYPE_PLAYER_SKILL, m_pos);
		}
	}
	else if (bAction[PLAYERBOOLTYPE_LANDING] == true)
	{// �_���[�W���n
		m_state = PLAYERSTATE_STEEL;
		m_Motionstate = MOTION_LANDING; // ���n���[�V�����ɐݒ�
	}
	else if (bAction[PLAYERBOOLTYPE_JUMP] == true)
	{// �v���C���[���W�����v���Ȃ�
		m_Motionstate = MOTION_JUMP; // �W�����v���[�V�����ɐݒ�
	}
	else if (bAction[PLAYERBOOLTYPE_SMALLDAMAGE])
	{
		m_Motionstate = MOTION_SMALLDAMAGE;
		if (m_nCounterAllFrame == 4 && m_Motionstate == MOTION_SMALLDAMAGE)
		{//2�t���[���ȏ�̂Ƃ����_���[�W�̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERHIT_2);//�_���[�W�P

		}

		m_move.x += (sinf(m_rot.y - D3DX_PI) * 0.05f) * -1.0f;
		m_move.z += (cosf(m_rot.y - D3DX_PI) * 0.05f) * -1.0f;
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�

	}
	else if (bAction[PLAYERBOOLTYPE_LARGEDAMAGE])
	{
		m_state = PLAYERSTATE_STEEL;	//�d����Ԃ�
		m_Motionstate = MOTION_LARGEDAMAGE;
		if (m_nCounterAllFrame == 1 && m_Motionstate == MOTION_LARGEDAMAGE)
		{//1�t���[���̂Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYERHIT_1);//�_���[�W�P
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
			m_move.y += 15.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -20.0f;
		}
		if (m_nCounterAllFrame == 28 && m_Motionstate == MOTION_LARGEDAMAGE)
		{//1�t���[���̂Ƃ�
			m_move.x += (sinf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;
			m_move.y += 0.0f;
			m_move.z += (cosf(m_rot.y - D3DX_PI) * 1.0f) * -7.0f;

			CreateParticleDust();

		}

	}
	else if (m_move.x > PLAYER_MOVE_DEFAULT ||
			 m_move.x < -PLAYER_MOVE_DEFAULT ||
			 m_move.z >  PLAYER_MOVE_DEFAULT ||
			 m_move.z < -PLAYER_MOVE_DEFAULT)
	{// �v���C���[�������Ă���Ƃ�
		m_Motionstate = MOTION_MOVE; // �ړ���Ԃɂ���

	}
	else
	{// �v���C���[�������ĂȂ��Ƃ�
		m_Motionstate = MOTION_NEUTRAL; // �j���[�g������Ԃɂ���
	}
}


//=============================================================================
// �֐����F�p�x�̏C��
// �֐��̊T�v�F�傫����΁A�����A��������΁A����
//=============================================================================
void CPlayer::PlayerRotFixes(D3DXVECTOR3 rot)
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
void CPlayer::PlayerRotFixes(float rot)
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
// �֐����F�v���C���[���̓ǂݍ���
// �֐��̊T�v�F�v���C���[���̓ǂݍ��݂ƃp�[�c�̓ǂݍ���
//=============================================================================
void CPlayer::LoadPlayerText(void)
{
	FILE *pFile = fopen(MODEL_FILENAME, "r");		// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

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
// �֐����F�v���C���[�̏�����
// �֐��̊T�v�F--
//=============================================================================
void CPlayer::InitPlayer(void)
{
	//�v���C���[���
	m_fRadius = PLAYER_COLLISION_RADIUS; // �v���C���[�̓����蔻��̔��a
	m_StateCnt = 20;
	m_state = PLAYERSTATE_APPEAR;
	m_fDestAngle = -D3DX_PI * 0.5f;
	m_fLife = PLAYER_LIFE;
	m_fHeight = 120.0f;
	m_fSkill = 0.0f;
	m_fDamage = 0.0f;

	//�O�Ղ̐���
	//m_apModel[12]->CreateOrbit();
}
//=============================================================================
// �֐����F�v���C���[�̃��[�V�����̏�����
// �֐��̊T�v�F--
//=============================================================================
void CPlayer::InitMotion(void)
{
	m_nNumKey = 2;
	m_pKeyInfo = m_aKeyInfo;
	m_nKey = 0;
	m_nCountMotion = 0;
	m_Motionstate = MOTION_NEUTRAL;
}
//=============================================================================
// �֐����F�Q�[�W�ނ̐���
// �֐��̊T�v�F�Q�[�W�ނ𐶐�����
//=============================================================================
void CPlayer::CreateGauge()
{
	m_apUi[0] = CUiGauge::Create(D3DXVECTOR3(105.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f), 510.0f, 25.0f, PLAYER_LIFE_FLAME, PLAYER_LIFE_FLAME, CUiGauge::GAUGETYPE_UNDER);		//�̗̓Q�[�W�̉��n����
	m_apUi[1] = CUiGauge::Create(D3DXVECTOR3(138.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 450.0f, 15.0f, PLAYER_LIFE, PLAYER_LIFE, CUiGauge::GAUGETYPE_RED);		//���̗̓Q�[�W����
	m_apUi[2] = CUiGauge::Create(D3DXVECTOR3(138.0f, 40.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 450.0f, 15.0f, PLAYER_LIFE, PLAYER_LIFE, CUiGauge::GAUGETYPE_GREEN);	//�{�̗̓Q�[�W����
	m_apUi[3] = CUiGauge::Create(D3DXVECTOR3(55.0f, 90.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 455.0f, 25.0f, PLAYER_LIFE, PLAYER_LIFE, CUiGauge::GAUGETYPE_UNDER);		//�X�L���Q�[�W�̉��n����
	m_apUi[4] = CUiGauge::Create(D3DXVECTOR3(85.0f, 90.0f, 0.0f), D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f), 400.0f, 15.0f, 0.0f, PLAYER_LIFE, CUiGauge::GAUGETYPE_SKILL);		//�X�L���Q�[�W�̐���
	m_apUi[5] = CUiGauge::Create(D3DXVECTOR3(0.0f, 55.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 120.0f, 60.0f, 50.0f, 50.0f, CUiGauge::GAUGETYPE_FACE_PLAYER);		//�X�L���Q�[�W�̐���

}
//=============================================================================
// �֐����F�p�[�c�̏����ݒ�
// �֐��̊T�v�F�p�[�c�̏����ݒ���s��
//=============================================================================
void CPlayer::DefaultParts(void)
{
	for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
	{//���t���[�����̌v�Z
		m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
	}

	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{// �p�[�c�������[�v
	 // ���f���̈ʒu���j���[�g�������[�V�����ɏ�����
		m_PartsDefaultpos[nCntParts] = m_Partspos[nCntParts] + m_aKeyInfo[m_Motionstate].aKey[0].pos[nCntParts];
		m_PartsDefaultrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[0].rot[nCntParts];
	}

}

//=============================================================================
// �֐����F�U���Ƃ̓����蔻��
// �֐��̊T�v�F--
//=============================================================================
void CPlayer::CollisionAttack(void)
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

	//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	//�J�����̊p�x�擾
	D3DXVECTOR3 CameraRot = pCamera->GetRot();


	pCollision = (CCollision*)GetSceneTop(COLLISION_PRIORITY);
	if (m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_STEEL)
	{
		while (pCollision != NULL)//NULL�ɂȂ�܂�
		{
			CCollision *pCollisionNext;
			pCollisionNext = (CCollision*)pCollision->GetSceneNext(COLLISION_PRIORITY);
			if (pCollision->GetObjType() == CScene::OBJTYPE_COLLISION_ENEMY)
			{
				if (pCollision->CollisionAttack(&m_pos, &m_rot, m_fHeight) == true)
				{
					//m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

					int nTypeHitType = pCollision->GetCollisionType();
					switch (nTypeHitType)
					{
					case HITTYPE_LIGHTATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
							//���C�t�̌���
						m_fLife -= LIGHT_ATACK;
						//�X�L���̑���
						m_fSkill += 10.0f;
						m_fDamage += LIGHT_ATACK;
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//��U��

						m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_HEAVYATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
						m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

						//	m_rot.y = pCollision->GetAngle();
							//���C�t�̌���
						m_fLife -= LARGE_ATACK;
						//�X�L���̑���
						m_fSkill += 15.0f;
						m_fDamage += LARGE_ATACK;
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//���U��


						m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_LIGHTATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
						//���C�t�̌���
						m_fLife -= 20.0f;
						//�X�L���̑���
						m_fSkill += 15.0f;
						m_fDamage += 20.0f;
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//��U��

						m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_HEAVYATTACK:
						for (int nCntDamage = 0; nCntDamage < 10; nCntDamage++)
						{
							m_bAction[nCntDamage] = false;
						}
						m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

						//���C�t�̌���
						m_fLife -= 30.0f;
						//�X�L���̑���
						m_fSkill += 35.0f;
						m_fDamage += 30.0f;
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//���U��


						m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_SKILLATTACK_S:
						//m_rot.y = pCollision->GetAngle();

						//���C�t�̌���
						m_fLife -= 5.0f;
						//�X�L���̑���
						m_fSkill += 10.0f;
						m_fDamage += 5.0f;
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LIGHT);	//��U��

						m_bAction[PLAYERBOOLTYPE_SMALLDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						break;
					case HITTYPE_BOSS_SKILLATTACK_L:
						//	m_rot.y = pCollision->GetAngle();
						m_fDestAngle = (-D3DX_PI * pCollision->GetAngle()) + CameraRot.y;

							//���C�t�̌���
						m_fLife -= 150.0f;
						//�X�L���̑���
						m_fSkill += 100.0f;
						m_fDamage += 150.0f;
						//��U����
						pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT_LARGE);	//���U��


						m_bAction[PLAYERBOOLTYPE_LARGEDAMAGE] = true;

						ModelMtx = m_apModel[1]->GetMatrix();

						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 3.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
						}
						for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
						{
							CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 + 10.0f, ModelMtx._43), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_HIT_000, 15);
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
// �֐����F�I�u�W�F�N�g�Ƃ̓����蔻��
// �֐��̊T�v�F�v���C���[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
//=============================================================================
void CPlayer::CollisionObjectPlayer(void)
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
				m_bAction[PLAYERBOOLTYPE_JUMP] = false;
				m_bJump = false;
			}
		}
		pObjectTop = pObjectNext;
	}

}
//=============================================================================
// �֐����F�ǂ̓����蔻��
// �֐��̊T�v�F�v���C���[�ƕǂƂ̓����蔻��
//=============================================================================
void CPlayer::CollisionWallPlayer(void)
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
// �֐����F�v���C���[�Ƃ̓����蔻��
// �֐��̊T�v�F���a�O�֖߂�
//=============================================================================
bool CPlayer::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius)
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

				pPos->z = m_pos.z - m_fRadius - fRadius;	// �I�u�W�F�N�g�̈ʒu��߂�

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
// �֐����F�V�����_�[�̓����蔻��
// �֐��̊T�v�F�v���C���[�ƃV�����_�[�Ƃ̓����蔻��
//=============================================================================
void CPlayer::CollisionCylinderPlayer(void)
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
// �֐����F�G�Ƃ̓����蔻��
// �֐��̊T�v�F�v���C���[�ƓG�Ƃ̓����蔻��
//=============================================================================
void CPlayer::CollisionEnemy(void)
{
	//=========================================================================
	//�G�̃|�C���^(�X�s�[�h�^)
	//=========================================================================
	CEnemy *pEnemy;

	pEnemy = (CEnemy*)GetSceneTop(ENEMY_PRIORITY);
	while (pEnemy != NULL)//NULL�ɂȂ�܂�
	{
		CEnemy *pEnemyNext;
		pEnemyNext = (CEnemy*)pEnemy->GetSceneNext(ENEMY_PRIORITY);
		if (pEnemy->GetObjType() == CScene::OBJTYPE_ENEMY)
		{
			pEnemy->CollisionEnemy(&m_pos, &m_posold, &m_move, &m_rot, 25.0f);

		}
		pEnemy = pEnemyNext;
	}


	//=========================================================================
	//�G�̃|�C���^(�p���[�^)
	//=========================================================================
	CEnemyPower *pEnemyPower;

	pEnemyPower = (CEnemyPower*)GetSceneTop(ENEMY_PRIORITY);
	while (pEnemyPower != NULL)//NULL�ɂȂ�܂�
	{
		CEnemyPower *pEnemyPowerNext;
		pEnemyPowerNext = (CEnemyPower*)pEnemyPower->GetSceneNext(ENEMY_PRIORITY);
		if (pEnemyPower->GetObjType() == CScene::OBJTYPE_ENEMY_POW)
		{
			pEnemyPower->CollisionEnemy(&m_pos, &m_posold, &m_move, &m_rot, 20.0f);

		}
		pEnemyPower = pEnemyPowerNext;
	}


	//=========================================================================
	//�G�̃|�C���^(�g���b�L�[�^)
	//=========================================================================
	CEnemyTrick *pEnemyTrick;

	pEnemyTrick = (CEnemyTrick*)GetSceneTop(ENEMY_PRIORITY);
	while (pEnemyTrick != NULL)//NULL�ɂȂ�܂�
	{
		CEnemyTrick *pEnemyTrickNext;
		pEnemyTrickNext = (CEnemyTrick*)pEnemyTrick->GetSceneNext(ENEMY_PRIORITY);
		if (pEnemyTrick->GetObjType() == CScene::OBJTYPE_ENEMY_TRC)
		{
			pEnemyTrick->CollisionEnemy(&m_pos, &m_posold, &m_move, &m_rot, 20.0f);

		}
		pEnemyTrick = pEnemyTrickNext;
	}



	//=========================================================================
	//�{�X�̃|�C���^
	//=========================================================================
	CBoss *pBoss;

	pBoss = (CBoss*)GetSceneTop(ENEMY_PRIORITY);
	while (pBoss != NULL)//NULL�ɂȂ�܂�
	{
		CBoss *pBossNext;
		pBossNext = (CBoss*)pBoss->GetSceneNext(ENEMY_PRIORITY);
		if (pBoss->GetObjType() == CScene::OBJTYPE_BOSS)
		{
			pBoss->CollisionBoss(&m_pos, &m_posold, &m_move, &m_rot, 20.0f);

		}
		pBoss = pBossNext;
	}

}

//=============================================================================
// �֐����F�v���C���[�֌W�̃f�o�b�O���O
// �֐��̊T�v�F--
//=============================================================================
void CPlayer::PlayerDebugLog(void)
{
	CDebugProc::Print("\n");
	CDebugProc::Print("�v���C���[�ʒu(XYZ): %.1f, %.1f, %.1f�F\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("�v���C���[�p�x(XYZ): %.1f, %.1f, %.1f�F\n", m_rot.x, m_rot.y, m_rot.z);
	CDebugProc::Print("�v���C���[�ړ���(XYZ): %.1f, %.1f, %.1f�F\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("�v���C���[LIFE: %.1f\n", m_fLife);
	CDebugProc::Print("�v���C���[SKILL: %.1f\n", m_fSkill);

	CDebugProc::Print("��ԕω��̃J�E���^�F%d\n", m_StateCnt);

	switch (m_state)
	{
	case PLAYERSTATE_NORMAL://�ʏ��Ԃ̂Ƃ�
		CDebugProc::Print("STATE�F[0]NORMAL\n");
		break;
	case PLAYERSTATE_STEEL://�d����Ԃ̂Ƃ�
		CDebugProc::Print("STATE�F[1]STEEL\n");
		break;
	case PLAYERSTATE_DAMAGE://�_���[�W��Ԃ̂Ƃ�
		CDebugProc::Print("STATE�F[2]DAMAGE\n");
		break;
	case PLAYERSTATE_INVISIBLE://���G�̂Ƃ�
		CDebugProc::Print("STATE�F[3]INVISIBLE\n");
		break;
	case PLAYERSTATE_APPEAR://�o�����̂Ƃ�
		CDebugProc::Print("STATE�F[4]PLAYERSTATE_APPEAR\n");
		break;
	case PLAYERSTATE_DEATH://�o�����̂Ƃ�
		CDebugProc::Print("STATE�F[5]PLAYERSTATE_DEATH\n");
		break;
	}

	switch (m_Motionstate)
	{
	case MOTION_NEUTRAL://�j���[�g�����̂Ƃ�
		CDebugProc::Print("MOTION�F[0]NEUTRAL\n");
		break;
	case MOTION_MOVE://�ړ��̂Ƃ�
		CDebugProc::Print("MOTION�F[1]MOVE\n");
		break;
	case MOTION_SLIDING://�X���C�f�B���O�̂Ƃ�
		CDebugProc::Print("MOTION�F[2]SLIDING\n");
		break;
	case MOTION_JUMP://�W�����v�̂Ƃ�
		CDebugProc::Print("MOTION�F[3]JUMP\n");
		break;
	case MOTION_LANDING://���n�̂Ƃ�
		CDebugProc::Print("MOTION�F[4]LANDING\n");
		break;
	case MOTION_LIGHTPUNCH://�X���C�f�B���O�̂Ƃ�
		CDebugProc::Print("MOTION�F[5]MOTION_LIGHTPUNCH\n");
		break;
	case MOTION_MIDDLEPUNCH://�X���C�f�B���O�̂Ƃ�
		CDebugProc::Print("MOTION�F[5]MOTION_MIDDLEPUNCH\n");
		break;
	}
	switch (m_MotionstateOld)
	{
	case MOTION_NEUTRAL://�j���[�g�����̂Ƃ�
		CDebugProc::Print("MOTION�F[0]NEUTRAL\n");
		break;
	case MOTION_MOVE://�ړ��̂Ƃ�
		CDebugProc::Print("MOTION�F[1]MOVE\n");
		break;
	case MOTION_SLIDING://�X���C�f�B���O�̂Ƃ�
		CDebugProc::Print("MOTION�F[2]SLIDING\n");
		break;
	case MOTION_JUMP://�W�����v�̂Ƃ�
		CDebugProc::Print("MOTION�F[3]JUMP\n");
		break;
	case MOTION_LANDING://���n�̂Ƃ�
		CDebugProc::Print("MOTION�F[4]LANDING\n");
		break;
	case MOTION_LIGHTPUNCH://�X���C�f�B���O�̂Ƃ�
		CDebugProc::Print("MOTION�F[5]MOTION_LIGHTPUNCH\n");
		break;
	case MOTION_MIDDLEPUNCH://�X���C�f�B���O�̂Ƃ�
		CDebugProc::Print("MOTION�F[5]MOTION_MIDDLEPUNCH\n");
		break;
	}
	CDebugProc::Print("KEY�F%d / %d\n", m_nCounterKey, m_aKeyInfo[m_Motionstate].nNumKey);
	CDebugProc::Print("FRAME�F%d / %d (%d / %d)\n", m_nCounterFrame, m_nFrame, m_nCounterAllFrame, m_nAllFrame);
	CDebugProc::Print("AllFrame�F%d\n", m_nCounterAllFrame);

	CDebugProc::Print("m_bJump�F%d\n", m_bJump);



}

//=============================================================================
// �֐����F�v���C���[�̃��C�t�̃`�F�b�N
// �֐��̊T�v�F�v���C���[�̃��C�t�̊m�F
//=============================================================================
void CPlayer::PlayerLifeCheck(void)
{
	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();


	if (m_fLife <= 0.0f)
	{
		if (mode == CManager::MODE_GAME)
		{
			m_state = PLAYERSTATE_DEATH;
			for (int nCntDamage = 5; nCntDamage < 9; nCntDamage++)
			{
				m_bAction[nCntDamage] = false;
			}
			m_bAction[PLAYERBOOLTYPE_DEATH] = true;
			m_fLife = 0.0f;
		}
	}

	if (m_fDamage >= PLAYER_LIFE)
	{
		m_fDamage = PLAYER_LIFE;
	}

	m_apUi[1]->AddValueSlip(m_fLife);	//���̗̓Q�[�W
	m_apUi[2]->AddValue(m_fLife);	//�{�̗̓Q�[�W

}

//=============================================================================
// �֐����F�v���C���[�̃X�L���̃`�F�b�N
// �֐��̊T�v�F�v���C���[�̃X�L���̊m�F
//=============================================================================
void CPlayer::PlayerSkillCheck(void)
{

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();



	if (m_fSkill >= PLAYER_LIFE)
	{
		m_fSkill = PLAYER_LIFE;

	}
	if (m_fSkill <= 0.0f)
	{
		m_fSkill = 0.0f;
	}

	m_apUi[4]->AddValueSlip(m_fSkill);

}

//=============================================================================
// �֐����F���p�[�e�B�N���̐���
// �֐��̊T�v�F���p�[�e�B�N���̐���
//=============================================================================
void CPlayer::CreateParticleFlame_SpinKick(D3DMATRIX *mtx)
{
	D3DXMATRIX ModelMtx = *mtx;

	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE_A, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_FIRE_LIFE);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SPINKICK, PARTICLE_FIRE_LIFE);
		}
	}
}

//=============================================================================
// �֐����F���p�[�e�B�N���̐���
// �֐��̊T�v�F���p�[�e�B�N���̐���
//=============================================================================
void CPlayer::CreateParticleFlame_Shoryu(D3DMATRIX *mtx)
{
	D3DXMATRIX ModelMtx = *mtx;

	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE_A, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_FIRE_LIFE);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_BLACKFIRE_LIFE);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 3.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, PARTICLE_FIRE_LIFE);
		}
	}
}

void CPlayer::CreateParticleFlame_Skill(D3DMATRIX * mtx)
{
	D3DXMATRIX ModelMtx = *mtx;

	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE_A, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < PARTICLE_BLACKFIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
		for (int nCntParticle = 0; nCntParticle < PARTICLE_FIRE_NUM; nCntParticle++)
		{//�p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(ModelMtx._41, ModelMtx._42 - 1.0f, ModelMtx._43), PARTICLE_FIRE, CParticle::PARTICLEDRAWTYPE_ADD, CParticle::PARTICLETYPE_ATTACK_SKILL, 7);
		}
	}
}

//=============================================================================
// �֐����F�y���p�[�e�B�N���̐���
// �֐��̊T�v�F�y���p�[�e�B�N���̐���
//=============================================================================
void CPlayer::CreateParticleDust(void)
{
	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_RAND, 20);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_RAND, 20);
		}
	}
}

void CPlayer::CreateParticleDust_Shoryu(void)
{
	if (m_pos.z > -30.0f && (m_pos.x >= 460.0f && m_pos.x <= 580.0f)
		|| m_pos.z > -30.0f && (m_pos.x >= 3190.0f && m_pos.x <= 3530.0f)
		|| (m_pos.x <= 1400.0f && m_pos.x >= 1200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f)
		|| (m_pos.x <= 2400.0f && m_pos.x >= 2200.0f) && (m_pos.z >= -168.0f && m_pos.z <= -35.0f))
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST_A, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, 9);
		}
	}
	else
	{
		for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
		{
			CParticle::Create(m_pos, PARTICLE_DEST, CParticle::PARTICLEDRAWTYPE_DEST, CParticle::PARTICLETYPE_ATTACK_SHORYUKEN, 12);
		}
	}
}


//=============================================================================
// �֐����F�v���C���[�̃X�L���̃`�F�b�N
// �֐��̊T�v�F�v���C���[�̃X�L���̊m�F
//=============================================================================
float CPlayer::GetDamage(void)
{
	return m_fDamage;
}

//=============================================================================
// �֐����F�ʒu���̎擾
// �֐��̊T�v�F�v���C���[�̈ʒu����Ԃ�
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �֐����F�p�x���̎擾
// �֐��̊T�v�F�v���C���[�̊p�x����Ԃ�
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �֐����F�ړ��ʏ��̎擾
// �֐��̊T�v�F�v���C���[�̈ړ��ʂ�Ԃ�
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// �֐����FUI�Q�[�W�̎擾
// �֐��̊T�v�FUI�Q�[�W��Ԃ�
//=============================================================================
CUiGauge *CPlayer::GetUiGauge(int nIndex)
{
	return m_apUi[nIndex];
}


//=============================================================================
// �֐����F�X�L���Q�[�W�̎擾
// �֐��̊T�v�F�X�L���Q�[�W��Ԃ�
//=============================================================================
void CPlayer::SetSkill(float fSkill)
{
	m_fSkill += fSkill;
}


