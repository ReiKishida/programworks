//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author : ��������
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS				// �t�@�C���ǂݍ��݂ŕK�v
#include <stdio.h>							// �t�@�C���ǂݍ��݂ŕK�v
#include <string.h>							// �t�@�C���ǂݍ��݂ŕK�v
#include <stdlib.h>							//rand�֐��ɕK�v
#include <time.h>							//rand�֐������ԂŒl�����߂�����̂ɕK�v
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "effect.h"
#include "main.h"
#include "meshField.h"
#include "meshWall.h"
#include "BIllLife.h"
#include "light.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "model.h"
#include "Item.h"
#include "hit.h"
#include "MessageWindow.h"
#include "result.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �W���֌W�̃}�N��
#define ENEMY_INERTIA				(0.15f)						// �G�̊���
#define ENEMY_AIR_INERTIA			(0.15f)						// �G��Y���̊���
#define ENEMY_DIFF_ANGLE			(0.15f)						// �p�x��ς����
#define ENEMY_GRAVITY				(-1.5f)						// �d��
#define ENEMY_JUMP_DOWN				(0.1f)						// �G�̃W�����v�͂̌�����
#define ENEMY_FLY_SIDE_DOWN			(2.0f)						// �G�̐�����ԍ����̌�����
#define ENEMY_FLY_INERTIA			(0.25f)						// �G�̐�����Ԋ���

// �G�̍s���n�̃}�N��
#define ENEMY_MOVE00_RADIUS			(1000.0f)					// �G���v���C���[�Ɍ������đ����Ă��锽��������͈�
#define ENEMY_MOVE01_RADIUS			(500.0f)					// �G����x�l�q������͈�
#define ENEMY_ATTACK_RADIUS			(100.0f)					// �G���v���C���[�ɍU�����Ă��锽��������͈�
#define ENEMY_OZIGI_RADIUS			(200.0f)					// �G�������V����͈�

#define ENEMY_ATTACK_WAIT_MAX_TIME	(120)						// �G���U��������̌��̍ő�̒���
#define ENEMY_ATTACK_WAIT_MIN_TIME	(90)						// �G���U��������̌��̍ŏ��̒���
#define ENEMY_ACT_MAX_TIME			(30)						// �G�̍s�������߂�Ԋu�̍ő�̒���
#define ENEMY_ACT_MIN_TIME			(20)						// �G�̍s�������߂�Ԋu�̍ŏ��̒���

#define ENEMY_DELETE_RADIUS			(100000000000000.0f)					// �G����������͈�

// �G��State��؂�ւ���t���[�����̃}�N��
#define ENEMY_DAMAGE_COUNTER		(15)						// �_���[�W���̖��G����
#define ENEMY_DEATH_COUNTER			(60)						// ���S���ɏ�����܂ł̎���

// ���낢��
#define MAX_CHAR					(256)						// �e�L�X�g�t�@�C������ǂݍ��ޕ����̍ő吔
#define ENEMY_MOTION_BLEND_FRAME	(10)						// ���[�V�����u�����h�̃t���[����
#define ENEMY_MOVE_NORM				(0.25f)						// �G�������Ă��̈ړ���
#define ENEMY_MOTION_MOVE_SPEED		(0.4f)						// ���[�V�������̓�����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Enemy_info	g_EnemyInfo[ENEMY_TYPE_MAX];				// �G�̎�ނ��Ƃ̏��
Enemy		g_aEnemy[MAX_ENEMY];						// �G�̏��
int			g_nNumEnemy;								// �G�̐l��
float		g_EnemyDeleteRadius;						// �G����������͈�
int			g_nEnemyCounter;							// ���݂̃t���[����
//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	// �����̏�����
	srand((unsigned int)time(0));								// <-�����rand�֐������ԂŐ��l���ς��悤�ɂ���

	//----------------------------------------------------------------------
	// �ϐ��̏�����
	//----------------------------------------------------------------------
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̑��������[�v

		// �G�̏��̏�����
		g_aEnemy[nCntEnemy].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏����ݒ�
		g_aEnemy[nCntEnemy].posOld			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu�̏����ݒ�
		g_aEnemy[nCntEnemy].InitPos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
		g_aEnemy[nCntEnemy].Move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̈ړ��ʂ̏�����
		g_aEnemy[nCntEnemy].rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̏����ݒ�
		g_aEnemy[nCntEnemy].rotDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړI�̌�����������
		g_aEnemy[nCntEnemy].nIdxLight		= -1;								// -1�ɏ�����

		// �U���ł��Ȃ����Ԃ������_���ɐݒ�
		g_aEnemy[nCntEnemy].nAttackWaitTime	= rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;				
		g_aEnemy[nCntEnemy].nAttackCounter	= 0;								// �U������܂ł̃J�E���^�[��������
		g_aEnemy[nCntEnemy].nLife			= 0;								// ���C�t��������
		g_aEnemy[nCntEnemy].nMaxLife		= 0;								// ���C�t�̍ő�l��������
		g_aEnemy[nCntEnemy].nDownCounter	= 0;								// �_�E����Ԃ̃J�E���^�[�ϐ���������

		// �s���n�̕ϐ��̏�����
		g_aEnemy[nCntEnemy].nActTime			= rand() % (ENEMY_ACT_MAX_TIME - ENEMY_ACT_MIN_TIME + 1) + ENEMY_ACT_MIN_TIME;		// �s���̎��Ԃ̕ϐ��������_���Őݒ�
		g_aEnemy[nCntEnemy].nActNowCounter	= 0;								// �s���̃J�E���^�[�ϐ���������
		g_aEnemy[nCntEnemy].bMoved			= false;							// �܂������Ă��Ȃ���Ԃɂ���

		g_aEnemy[nCntEnemy].fRadius			= 0.0f;								// �����蔻��̔��a��������
		g_aEnemy[nCntEnemy].fPosLeght		= 0.0f;								// �v���C�C���[�Ƃ̋�����������
		g_aEnemy[nCntEnemy].fJampPower		= 0.0f;								// �W�����v�͂�ݒ�
		g_aEnemy[nCntEnemy].fFlyPower		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ������ї͂�ݒ�
		g_aEnemy[nCntEnemy].bUse				= false;							// �g�p���Ă��Ȃ���Ԃɂ���
		g_aEnemy[nCntEnemy].Motionstate		= ENEMY_MOTION_NEUTRAL;				// ���[�V�������j���[�g�����ɏ�����
		g_aEnemy[nCntEnemy].MotionstateOld	= ENEMY_MOTION_MAX;					// �O��̃��[�V�������j���[�g�����ɏ�����

		// ��Ԃ̕ϐ��̏�����
		g_aEnemy[nCntEnemy].State			= ENEMY_STATE_NORMAL;				// �ʏ��Ԃɏ�����
		g_aEnemy[nCntEnemy].nCounterState	= 0;								// �X�e�[�g�̃J�E���^�[��������

		// ���[�V�����̕ϐ��̏�����
		g_aEnemy[nCntEnemy].nCounterKey		= 0;								// ���݂̃L�[�t���[����������
		g_aEnemy[nCntEnemy].nCounterFrame	= 0;								// ���݂̃t���[������������
		g_aEnemy[nCntEnemy].nCounterAllFrame = 0;								// ���݂̑S�t���[������������
		g_aEnemy[nCntEnemy].nFrame			= 0;								// ����̃t���[������������
		g_aEnemy[nCntEnemy].pMeshField		= NULL;								// ���b�V���t�B�[���h���
		g_aEnemy[nCntEnemy].pModel			 = NULL;							// ���f�����
		for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
		{// ���f���̐������[�v
			// �ϐ��̏�����
			g_aEnemy[nCntEnemy].posMotion[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ʒu�̍�����������
			g_aEnemy[nCntEnemy].rotMotion[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ��]�̍�����������
		}
		// �U����Ԃ�ݒ�
		for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
		{
			g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;			// �U��������Ȃ���Ԃɐݒ�
			g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;			// �U���̏�����Ԃ���Ȃ���Ԃɐݒ�
		}
	}

	g_nNumEnemy		= 0;				// �G�̐l����������
	g_nEnemyCounter = 0;			// ���݂̃t���[������������

	// �X�V����͈͂�������
	g_EnemyDeleteRadius = ENEMY_DELETE_RADIUS * ENEMY_DELETE_RADIUS;			// �������鋗��

	// �p�[�c�̈ʒu�̐ݒ�
	EnemyModelSet();

	// �G�̔z�u���̓ǂݍ���
	EnemySetLode();

}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// �G�̎�ޕ����[�v

		for (int nCntModel = 0; nCntModel < g_EnemyInfo[nCntEnemyType].MaxModel; nCntModel++)
		{// ���f���̐������[�v

			// ���b�V���̊J��
			if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh != NULL)
			{// ���g�������Ă���
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->Release();
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh = NULL;
			}

			// �}�e���A���̊J��
			if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat != NULL)
			{// ���g�������Ă���
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat->Release();
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat = NULL;
			}

			for (int nCntTexture = 0; nCntTexture < (int)g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat; nCntTexture++)
			{// �}�e���A���̐������[�v
				// �e�N�X�`���̊J��
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntTexture] != NULL)
				{
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntTexture]->Release();
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntTexture] = NULL;
				}
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	// �ϐ��錾
	MODE	pMode	= GetMode();			// ���݂̃��[�h���擾
	Camera *pCamera = GetCamera();			// �J�����̏����擾
	Player *pPlayer = GetPlayer();			// �v���C���[�̏��̎擾

	float fDiffAngle;						// ����

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̑��������[�v
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �g���ĂȂ����

			if (pMode == MODE_GAME ||
				pMode == MODE_TUTORIAL)
			{// �Q�[���ƃ`���[�g���A�����̂Ƃ�
			 // �G�ƃv���C���[�̋����̌v�Z
				float posX = (g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x);				// (�G�̈ʒu���W�|�v���C���[�̈ʒu���W)
				float posZ = (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z);				// (�G�̈ʒu���W�|�v���C���[�̈ʒu���W)
				g_aEnemy[nCntEnemy].fPosLeght = (posX * posX) + (posZ * posZ);			// ���ꂼ���2�悵�ċ������v�Z

				g_aEnemy[nCntEnemy].Move.y += ENEMY_GRAVITY;			// �d�͂̉��Z
				if (g_aEnemy[nCntEnemy].pos.y <= -500)
				{// �J�E���^�[��0�ȉ��ɂȂ�����
					DeletEnemy(nCntEnemy, false);							// �G���폜����
				}
				
				//-----------------------------------------------
				// �G�̑���
				//-----------------------------------------------
				if (g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_LANDING	&&
					g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_DAMAGE	&&
					g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_FLY		&&
					g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_GETUP)
				{// ����̃��[�V�������ɂ͑�����󂯕t���Ȃ�
					EnemyController(nCntEnemy);										// �G�̑���ƈʒu����]�̍X�V
				}
				if (g_aEnemy[nCntEnemy].fJampPower > 0.0f)
				{// �W�����v�͂���������
					g_aEnemy[nCntEnemy].Move.y += g_aEnemy[nCntEnemy].fJampPower;		// �㏸���Ă���
					g_aEnemy[nCntEnemy].fJampPower -= ENEMY_JUMP_DOWN;				// �W�����v�͂����炵�Ă���

				}
				else if (g_aEnemy[nCntEnemy].fJampPower < 0.0f)
				{// �W�����v�͂�0.0f��艺�������
					g_aEnemy[nCntEnemy].fJampPower = 0.0f;
				}
				if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
				{// ������ї͂��܂���������
					if (g_aEnemy[nCntEnemy].fFlyPower.x >= 0.1f ||
						g_aEnemy[nCntEnemy].fFlyPower.x <= -0.1f)
					{// ���������̐�����ї͂�����Ȃ�
						g_aEnemy[nCntEnemy].Move.x += g_aEnemy[nCntEnemy].fFlyPower.x;										// ������ї͂����Z
					}
					if (g_aEnemy[nCntEnemy].fFlyPower.z >= 0.1f ||
						g_aEnemy[nCntEnemy].fFlyPower.z <= -0.1f)
					{// ���������̐�����ї͂�����Ȃ�
						g_aEnemy[nCntEnemy].Move.z += g_aEnemy[nCntEnemy].fFlyPower.z;										// ������ї͂����Z
					}

					if (g_aEnemy[nCntEnemy].nFlyCounter > 0)
					{// ������ԃJ�E���^�[��0���傫��������
						g_aEnemy[nCntEnemy].fFlyPower.y -= ENEMY_JUMP_DOWN;			// �W�����v�͂����炵�Ă���

						if (g_aEnemy[nCntEnemy].fFlyPower.y > 0.0f)
						{// �W�����v�͂��܂���������
							g_aEnemy[nCntEnemy].Move.y += g_aEnemy[nCntEnemy].fFlyPower.y;			// �㏸���Ă���
						}
					}
				}


				if (g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_ATTACK)
				{// �U�����[�V�����̎�����Ȃ���Ό�������]������

					// �ړI�̊p�x���C������
					if (g_aEnemy[nCntEnemy].rotDest.y > D3DX_PI)
					{// +3.14���A�����Ă��܂�����
						g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
					}
					if (g_aEnemy[nCntEnemy].rotDest.y < -D3DX_PI)
					{// -3.14���A���������
						g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
					}

					// ����������
					fDiffAngle = (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y);

					// �����̊p�x���C������
					if (fDiffAngle > D3DX_PI)
					{// +3.14���A�����Ă��܂�����
						fDiffAngle -= D3DX_PI * 2.0f;
					}
					if (fDiffAngle < -D3DX_PI)
					{// -3.14���A���������
						fDiffAngle += D3DX_PI * 2.0f;
					}

					// ���������Z���Ă���
					g_aEnemy[nCntEnemy].rot.y += fDiffAngle * ENEMY_DIFF_ANGLE;

					// ���݂̊p�x���C������
					if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
					{// +3.14���A�����Ă��܂�����
						g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
					}
					if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
					{// -3.14���A���������
						g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
					}
				}

				// ����
				g_aEnemy[nCntEnemy].Move.x += (0.0f - g_aEnemy[nCntEnemy].Move.x) * ENEMY_INERTIA;
				g_aEnemy[nCntEnemy].Move.z += (0.0f - g_aEnemy[nCntEnemy].Move.z) * ENEMY_INERTIA;
				g_aEnemy[nCntEnemy].Move.y += (0.0f - g_aEnemy[nCntEnemy].Move.y) * ENEMY_AIR_INERTIA;

				// �G�̈ʒu���X�V
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].Move;

				// �e�̈ʒu�̍X�V
				SetPositionShadow(g_aEnemy[nCntEnemy].nIdyShadow, D3DXVECTOR3(g_aEnemy[nCntEnemy].aModel[0].mtxWorld._41, g_aEnemy[nCntEnemy].aModel[0].mtxWorld._42, g_aEnemy[nCntEnemy].aModel[0].mtxWorld._43), g_aEnemy[nCntEnemy].InitPos);
				// ���C�t�̈ړ�
				SetPositionBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos);

				//-----------------------------------------------
				// �����蔻��
				//-----------------------------------------------
				CollisionEnemy(&g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].fRadius);
				if (CollisionModelMinMax(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius, &g_aEnemy[nCntEnemy].pModel) != 0)
				{
					if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
					{// �W�����v���Ă�����
					 //g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = true;					// ���n��Ԃɂ���
					}
					Shadow *pShadow = GetShadow();
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].pos.y = g_aEnemy[nCntEnemy].pos.y + 0.1f;
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].col.a = 1.0f;
				}
				if (CollisionModelMinMax(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius, &g_aEnemy[nCntEnemy].pModel) != 0)
				{// �t�B�[���h�̓����蔻��
					if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
					{// �W�����v���Ă�����
					 //g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = true;					// ���n��Ԃɂ���
					}

					g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] = false;						// �W�����v�ł����Ԃɂ���

					if (g_aEnemy[nCntEnemy].Motionstate == ENEMY_MOTION_FLY)
					{// ������у��[�V���������n������
						if (g_aEnemy[nCntEnemy].State != ENEMY_STATE_DEATH)
						{// ����łȂ����
							if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
							{// ������эŒ��Ȃ�

								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = false;						// �|��Ă����Ԃ��畜��
								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = true;						// �|��Ă����Ԃ��畜��
								g_aEnemy[nCntEnemy].nDownCounter = rand() % (120 - 60 + 1) + 60;	// �N���オ�鎞�Ԃ�ݒ�
							}
						}

						// ������ї͂�0�ɂ��āA���点�Ȃ�
						g_aEnemy[nCntEnemy].fFlyPower.x = 0.0f;
						g_aEnemy[nCntEnemy].fFlyPower.z = 0.0f;
					}

				}
				// ���b�V���E�H�[��
				if (CollisionVecMeshWall(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius) == true)
				{

				}
				if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] == false)
				{// ���˕Ԃ�D��
					if (CollisionMeshWall(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius) == true)
					{// �ǂɓ���������
					}
				}

				if (CollisionMeshField(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move,
					g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius, &g_aEnemy[nCntEnemy].pMeshField) == true)
				{
					Shadow *pShadow = GetShadow();
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].pos.y = g_aEnemy[nCntEnemy].pos.y + 0.1f;
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].col.a = 1.0f;

					if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
					{// �W�����v���Ă�����
						//g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = true;					// ���n��Ԃɂ���
					}

					g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] = false;						// �W�����v�ł����Ԃɂ���

					if (g_aEnemy[nCntEnemy].Motionstate == ENEMY_MOTION_FLY)
					{// ������у��[�V���������n������
						if (g_aEnemy[nCntEnemy].State != ENEMY_STATE_DEATH)
						{// ����łȂ����
							if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
							{// ������эŒ��Ȃ�

								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = false;						// �|��Ă����Ԃ��畜��
								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = true;							// �|��Ă����Ԃ��畜��
								g_aEnemy[nCntEnemy].nDownCounter = rand() % (120 - 60 + 1) + 60;	// �N���オ�鎞�Ԃ�ݒ�
							}

						}

						// ������ї͂�0�ɂ��āA���点�Ȃ�
						g_aEnemy[nCntEnemy].fFlyPower.x = 0.0f;
						g_aEnemy[nCntEnemy].fFlyPower.z = 0.0f;
					}
				}

				//-------------------------------------------------------
				// �U���̓����蔻��
				//-------------------------------------------------------
				for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
				{// �L�����N�^�[�̍U���̓����蔻��̑��������[�v
					if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse == true)
					{// �����蔻�肪�g���Ă���Ƃ�
						if (int(g_aEnemy[nCntEnemy].Motionstate) == g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nMotionType)
						{// �����蔻��̃��[�V�������ꏏ��������
							if (g_aEnemy[nCntEnemy].nCounterAllFrame >= g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionStart &&
								g_aEnemy[nCntEnemy].nCounterAllFrame <= g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionEnd)
							{// �����蔻�肪��������t���[�������Ȃ���������

								// �v���C���[�ւ̍U������
								CollisionPlayer(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._43), g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].fRadius, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nAttackPower, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].fDamageSide, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].fDamageVertical);

								// �U�����ɃG�t�F�N�g
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._43), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, 1.0f), g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].fRadius, 10, EFFECTTYPE_EXPLOSION);
							}

						}
					}
				}

#if 0	// �̂̓����蔻��ɃG�t�F�N�g���o���Č�����悤�ɂ��邩

				//-------------------------------------------------------
				// �̂̓����蔻��
				//-------------------------------------------------------
				for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_Enemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
				{// �L�����N�^�[�̑̂̓����蔻��̑��������[�v

					if (g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
					{// �̂̓����蔻�肪�g���Ă���Ƃ�
						SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41,
							g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42,
							g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
							D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),
							g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius,
							1, EffectType_Normal);	// �����蔻�������

					}
				}
#endif

				//-----------------------------------------------
				// ���[�V�����̍X�V
				//-----------------------------------------------
				if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
				{// �_���[�W����������
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_FLY;										// ������ђ��ɐݒ�
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] == true)
				{// �_���[�W����������
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_DAMAGE;									// �_���[�W���ɐݒ�
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] == true)
				{// �_���[�W����������
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_GETUP;									// �_�E�����ɐݒ�
				}
				else if (g_aEnemy[nCntEnemy].bEnemyAttack[ENEMY_ATTACKTYPE_ATTACK] == true)
				{// �G���U�����Ȃ�
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_ATTACK;					// �U�����[�V�����ɐݒ�
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
				{// �G���W�����v���Ȃ�
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_JUMP;						// �W�����v���[�V�����ɐݒ�
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] == true)
				{// ���n���Ȃ�
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_LANDING;					// ���n���[�V������ݒ�
				}
				else if (g_aEnemy[nCntEnemy].Move.x > ENEMY_MOVE_NORM ||
					g_aEnemy[nCntEnemy].Move.x < -ENEMY_MOVE_NORM ||
					g_aEnemy[nCntEnemy].Move.z >  ENEMY_MOVE_NORM ||
					g_aEnemy[nCntEnemy].Move.z < -ENEMY_MOVE_NORM)
				{// �G�������Ă���Ƃ�
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_MOVE;				// �ړ���Ԃɂ���
				}
				else
				{// �G�������ĂȂ��Ƃ�
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_NEUTRAL;			// �j���[�g������Ԃɂ���
				}

				EnemyMotion(nCntEnemy);												// �G�̃��[�V�����̍X�V


				// �G�̑O��̏���ۑ�
				g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;				// �O��̈ʒu��ۑ�
				g_aEnemy[nCntEnemy].MotionstateOld = g_aEnemy[nCntEnemy].Motionstate;		// �O��̃��[�V�����̃X�e�[�g��ۑ�

				if (g_EnemyDeleteRadius < g_aEnemy[nCntEnemy].fPosLeght)
				{// ��������͈͂̊O�ɂ�����
					DeletEnemy(nCntEnemy, false);										// �G����������
				}
			}
			else
			{// ����ȊO�̎�

				// �G�ƃJ�����̋����̌v�Z
				float posX = (g_aEnemy[nCntEnemy].pos.x -pCamera->posV.x);				// (�G�̍��W�|�J�����̎��_�̍��W)��2�悷��
				float posZ = (g_aEnemy[nCntEnemy].pos.z -pCamera->posV.z);				// (�G�̍��W�|�J�����̎��_�̍��W)��2�悷��
				float fPosLenght = (posX * posX) + (posZ * posZ);						// ���ꂼ���2�悵�ċ������v�Z

				// �����V�����锼�a�̋����̌v�Z
				float fRadiusLenght = (ENEMY_OZIGI_RADIUS) * (ENEMY_OZIGI_RADIUS);		// (�����V�����锼�a)��2�悵�Ĕ��a�̋������v�Z

				// ���W�̋����Ɣ��a�̋������r
				if (fPosLenght <  fRadiusLenght &&
					fPosLenght > -fRadiusLenght)
				{// ���a�̋��������ʒu�̋����̂ق���0�ɋ߂�������
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_ATTACK;				// ���[�V������������

				}

				EnemyMotion(nCntEnemy);													// �G�̃��[�V�����̍X�V
				g_aEnemy[nCntEnemy].MotionstateOld = g_aEnemy[nCntEnemy].Motionstate;		// �O��̃��[�V�����̃X�e�[�g��ۑ�
			}

			//---------------------------------------------
			// �G�̏�ԕʂ̏���
			//---------------------------------------------
			switch (g_aEnemy[nCntEnemy].State)
			{// �G�̏�Ԃ�
			case ENEMY_STATE_NORMAL:		// �ʏ��ԂȂ�
				// �������Ȃ�
				break;

			case ENEMY_STATE_DAMAGE:		// �_���[�W��ԂȂ�

				g_aEnemy[nCntEnemy].nCounterState--;						// �J�E���^�[�����炵�Ă���

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// �J�E���^�[��0�ȉ��ɂȂ�����
					g_aEnemy[nCntEnemy].State = ENEMY_STATE_NORMAL;		// �ʏ��Ԃɖ߂�
				}

				break;

			case ENEMY_STATE_DEATH:		// ���S��ԂȂ�

				g_aEnemy[nCntEnemy].nCounterState--;						// �J�E���^�[�����炵�Ă���

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// �J�E���^�[��0�ȉ��ɂȂ�����
					DeletEnemy(nCntEnemy, true);							// �G���폜����
				}

				break;
			}
		}
	}

	// ���݂̃t���[���������Z
	g_nEnemyCounter++;

	if (g_nNumEnemy <= 0)
	{
		if (GetGameState() != GAMESTATE_END &&GetGameState() != GAMESTATE_END2)
		{
			SetResultType(RESULTTYPE_CLEAR);		// �Q�[���N���A�I��ݒ�
			StopSound(SOUND_LABEL_GAMEBGM1);			// �{�XBGM��~
			PlaySound(SOUND_LABEL_GAMECLEARE);		// �N���ABGM�Đ�
			SetGameState(GAMESTATE_END);			// �Q�[�����I��
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̏�����
	D3DXMATRIX mtxRot, mtxTrans;					// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;							// �K�w�\���p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;							// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;								// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̑��������[�v

		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �g���Ă����

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// ��](����)�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aEnemy[nCntEnemy].rot.y,
				g_aEnemy[nCntEnemy].rot.x,
				g_aEnemy[nCntEnemy].rot.z);	// �Q�[���𐧍삷��Ƃ��͈�Ԏg���x����������

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,			// �Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
				&g_aEnemy[nCntEnemy].mtxWorld,
				&mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aEnemy[nCntEnemy].pos.x,				// �ʒu��X���W�������Ă���
				g_aEnemy[nCntEnemy].pos.y,
				g_aEnemy[nCntEnemy].pos.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,			// �Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
				&g_aEnemy[nCntEnemy].mtxWorld,
				&mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
			{// �p�[�c�̐������[�v
				
				if (g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel == -1)
				{// �e���f�������Ȃ��Ƃ�
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;				// �G�̃}�g���b�N�X����
				}
				else
				{// �e�̃��f��������Ƃ�
					mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel].mtxWorld;		// �e�̃}�g���b�N�X����
				}

				//---------------------------
				// ���f���̕`��
				//---------------------------
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				// ��](����)�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y,
					g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x,
					g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);	// �Q�[���𐧍삷��Ƃ��͈�Ԏg���x����������

				// ��](����)��ݒ�
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,			// �Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
					&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,
					&mtxRot);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTrans,
					g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x,				// �ʒu��X���W�������Ă���
					g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y,
					g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);

				// �ړ���ݒ�
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,			// �Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
					&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,
					&mtxTrans);


				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,			// �e�̃��[���h�}�g���b�N�X�Ǝq�̃��[���h�}�g���b�N�X�������Ă���
					&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,
					&mtxParent);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].nNumMat; nCntMat++)
				{						
						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						// �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].pTexture[nCntMat]);

						// �G(�p�[�c)�̕`��
						g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}

			//---------------------------
			// �U���̓����蔻��̃��[���h�}�g���N�X�̐ݒ�
			//---------------------------
			for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
			{// �L�����N�^�[�̍U���̓����蔻��̑��������[�v

				if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse == true)
				{// �U���̓����蔻�肪�g���Ă����ԂȂ珈������
					if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent == -1)
					{// �e���f�������Ȃ��Ƃ�
						mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// �G�̃��[���h�}�g���b�N�X����
					}
					else
					{// �e���f��������Ƃ�
						mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent].mtxWorld;		// �e���f���̃}�g���b�N�X����
					}

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);

					// �ړ��𔽉f
					D3DXMatrixTranslation(&mtxTrans,
						g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.x,			// �ʒu��X���W�������Ă���
						g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.y,
						g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.z);

					// �ړ���ݒ�
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,			// �Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
						&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,
						&mtxTrans);


					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,			// �e�̃��[���h�}�g���b�N�X�Ǝq�̃��[���h�}�g���b�N�X�������Ă���
						&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,
						&mtxParent);

					// ���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);
				}
			}
				//---------------------------------
				// �̂̓����蔻��
				//---------------------------------
			for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
			{// �L�����N�^�[�̑̂̓����蔻��̑��������[�v

				if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
				{// �̂̓����蔻�肪�g���Ă����ԂȂ珈������

					if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent == -1)
					{// �e���f�������Ȃ��Ƃ�
						mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// �G�̃��[���h�}�g���b�N�X����
					}
					else
					{// �e���f��������Ƃ�
						mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent].mtxWorld;		// �e���f���̃}�g���b�N�X����
					}

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);

					// �ړ��𔽉f
					D3DXMatrixTranslation(&mtxTrans,
						g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.x,			// �ʒu��X���W�������Ă���
						g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.y,
						g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.z);

					// �ړ���ݒ�
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,			// �Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
						&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,
						&mtxTrans);


					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,			// �e�̃��[���h�}�g���b�N�X�Ǝq�̃��[���h�}�g���b�N�X�������Ă���
						&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,
						&mtxParent);

					// ���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);

				}
			}
		}
	}
}

//=============================================================================
// �G�̓���
//=============================================================================
void EnemyController(int nCntEnemy)
{
	Player *pPlayer = GetPlayer();												// �v���C���[�̏��̎擾

	// �G���������鋗���̌v�Z
	float fMoveReactionLenght00 = ENEMY_MOVE00_RADIUS * ENEMY_MOVE00_RADIUS;	// �G���v���C���[�Ɍ������đ���͈͂�2�悵�ċ������v�Z
	float fMoveReactionLenght01 = ENEMY_MOVE01_RADIUS * ENEMY_MOVE01_RADIUS;	// �G����x�l�q������͈͂�2�悵�ċ������v�Z
	float fAttackReactionLenght = ENEMY_ATTACK_RADIUS * ENEMY_ATTACK_RADIUS;	// �G���U�����Ă��鋗��

	int nActType;																// �G�̍s���̃^�C�v������
	int nMinAct;																// �����̍ŏ��l������ϐ�
	int nMaxAct;																// �����̍ő�l������ϐ�

	float Angle;																// �G����v���C���[�ւ̊p�x

	//----------------------------------------------------------------
	// �G���s�����銄����ݒ�
	//----------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].fPosLeght  >  fMoveReactionLenght00 &&
		-g_aEnemy[nCntEnemy].fPosLeght < -fMoveReactionLenght00)
	{// �G�������͈͂��������ꍇ
		switch (g_aEnemy[nCntEnemy].EnemyType)
		{// �G�̎�ނ�
		case ENEMY_TYPE_ARMOR:		// �Z�̏ꍇ
			if (g_aEnemy[nCntEnemy].bMoved == false)
			{// �����Ă��Ȃ���Ԃ̍s��

				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	 = 1.0f;		// �_�������������߂ɐݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		 = 0.0f;		// �v���C���[�����銄����ݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	 = 0.0f;		// ��������߂ɂ��āA�K�x�ɍs��������
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	 = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�
			}
			else
			{// ��������̍s��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	 = 0.5f;		// �_�����������߂ɐݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		 = 0.0f;		// �v���C���[�����銄����ݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	 = 0.5f;		// ������������߂ɐݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ���s�������m���ł�����
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	 = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�

			}

			break;

		case ENEMY_TYPE_HITOME:		// ��ڂ̏ꍇ

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	 = 0.75f;			// �_�������������߂ɐݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		 = 0.0f;			// �v���C���[�����銄����ݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	 = 0.25f;			// ��������߂ɂ��āA�K�x�ɍs��������
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	 = 0.0f;			// �U�����銄����0�ɂ��čs�������Ȃ�

			break;

		case ENEMY_TYPE_HIGECHAN:	// �Ђ������̏ꍇ

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.75f;			// �_�������������߂ɐݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;			// �v���C���[�����銄����ݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.25f;			// ��������߂ɂ��āA�K�x�ɍs��������
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// �U�����銄����0�ɂ��čs�������Ȃ�

			break;

		case ENEMY_TYPE_HONCHAN:	// �{�����̏ꍇ

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.75f;			// �_�������������߂ɐݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;			// �v���C���[�����銄����ݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.25f;			// ��������߂ɂ��āA�K�x�ɍs��������
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// �U�����銄����0�ɂ��čs�������Ȃ�

			break;

		}
	}
	else
	{// �G�������͈͂̏ꍇ
		if (g_aEnemy[nCntEnemy].fPosLeght  >  fMoveReactionLenght01 &&
			-g_aEnemy[nCntEnemy].fPosLeght < -fMoveReactionLenght01)
		{// ��x�l�q������͈͂����O��������
			switch (g_aEnemy[nCntEnemy].EnemyType)
			{// �G�̎�ނ�
			case ENEMY_TYPE_ARMOR:		// �Z�̏ꍇ
				if (g_aEnemy[nCntEnemy].bMoved == false)
				{// �����Ă��Ȃ���Ԃ̍s��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 1.0f;		// �v���C���[���U������O�͖_����
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// �����������ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ���s����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// �U�����銄����ݒ肷��
				}
				else
				{// ��������̍s��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�����������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.1f;		// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// ������������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.8f;		// �v���C���[�Ɍ������đ���s�������m���ł�����
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�

				}

				break;

			case ENEMY_TYPE_HITOME:		// ��ڂ̏ꍇ

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�����������߂ɐݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.1f;		// �v���C���[�����銄����ݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// ������������߂ɐݒ肷��
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.8f;		// �v���C���[�Ɍ������đ���s�������m���ł�����
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�

			break;

			case ENEMY_TYPE_HIGECHAN:	// �Ђ������̏ꍇ

				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�������������߂ɐݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.25f;		// �v���C���[�����銄����ݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.25f;		// ��������߂ɂ��āA�K�x�ɍs��������
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.5f;		// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�

				break;

			case ENEMY_TYPE_HONCHAN:	// �{�����̏ꍇ

				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;			// �_�������������߂ɐݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.5f;			// �v���C���[�����銄����ݒ肷��
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.5f;			// ��������߂ɂ��āA�K�x�ɍs��������
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// �U�����銄����0�ɂ��čs�������Ȃ�

				break;
			}
		}
		else
		{// �l�q������͈͓���������

			if (g_aEnemy[nCntEnemy].fPosLeght  < fAttackReactionLenght &&
				g_aEnemy[nCntEnemy].fPosLeght  > -fAttackReactionLenght)
			{// �v���C���[���G���U������͈͂ɓ�������
				switch (g_aEnemy[nCntEnemy].EnemyType)
				{// �G�̎�ނ�
				case ENEMY_TYPE_ARMOR:		// �Z�̏ꍇ
					if (g_aEnemy[nCntEnemy].bMoved == false)
					{// �����Ă��Ȃ���Ԃ̍s��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�����������߂ɐݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// �v���C���[�����銄����ݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// �����
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 1.0f;		// �U�����銄����1.0�ɂ��č��m���ōs��������
					}
					else
					{// ��������̍s��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�����������߂ɐݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.2f;		// �v���C���[�����銄����ݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// ������������߂ɐݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ���s�������m���ł�����
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// �U�����銄����0�ɂ��čs�������Ȃ�

					}

					break;

				case ENEMY_TYPE_HITOME:		// ��ڂ̏ꍇ

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�����������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.2f;		// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// �����
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// �U�����銄����1.0�ɂ��č��m���ōs��������

					break;

				case ENEMY_TYPE_HIGECHAN:	// �Ђ������̏ꍇ

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�������������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// ��������߂ɂ��āA�K�x�ɍs��������
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.2f;		// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// �U�����銄����0�ɂ��čs�������Ȃ�

					break;

				case ENEMY_TYPE_HONCHAN:	// �{�����̏ꍇ

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�������������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.3f;		// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// ��������߂ɂ��āA�K�x�ɍs��������
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// �U�����銄����0�ɂ��čs�������Ȃ�

					break;


				}
			}
			else
			{// �U���͈͓����O�̏ꍇ
				switch (g_aEnemy[nCntEnemy].EnemyType)
				{// �G�̎�ނ�
				case ENEMY_TYPE_ARMOR:		// �Z�̏ꍇ

					if (g_aEnemy[nCntEnemy].bMoved == false)
					{// �����Ă��Ȃ���Ԃ̍s��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 1.0f;		// �_�����������߂ɐݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// �v���C���[�����銄����ݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// �����
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// �v���C���[�Ɍ������đ��銄����1.0�ɂ��č��m���ōs��������
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�
					}
					else
					{// ��������̍s��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�����������߂ɐݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.35f;		// �v���C���[�����銄����ݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.35f;		// ������������߂ɐݒ肷��
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.3f;		// �v���C���[�Ɍ������đ���s�������m���ł�����
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�

					}

					break;

				case ENEMY_TYPE_HITOME:		// ��ڂ̏ꍇ

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// �_�����������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.35f;		// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.35f;		// �����
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.3f;		// �v���C���[�Ɍ������đ��銄����1.0�ɂ��č��m���ōs��������
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// �U�����銄����0�ɂ��čs�������Ȃ�

					break;

				case ENEMY_TYPE_HIGECHAN:	// �Ђ������̏ꍇ

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;			// �_�������������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.35f;			// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.35f;			// ��������߂ɂ��āA�K�x�ɍs��������
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// �U�����銄����0�ɂ��čs�������Ȃ�

					break;

				case ENEMY_TYPE_HONCHAN:	// �{�����̏ꍇ

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;			// �_�������������߂ɐݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.5f;			// �v���C���[�����銄����ݒ肷��
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;			// ��������߂ɂ��āA�K�x�ɍs��������
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.5f;			// �v���C���[�Ɍ������đ��銄����0�ɂ��čs�������Ȃ�
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// �U�����銄����0�ɂ��čs�������Ȃ�

					break;


				}
			}
		}
	}

	//---------------------------------------------------------
	// �G�̍s��������
	//---------------------------------------------------------
	if (g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_ATTACK)
	{// �U�����[�V����������Ȃ����

		if (g_aEnemy[nCntEnemy].nActNowCounter % g_aEnemy[nCntEnemy].nActTime == 0)
		{// ���t���[�����Ƃɍs����ύX

			// �G�̍s��������
			nActType = rand() % int((100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL])	 +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK])		 +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER])	 +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES]) +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK])	 )
									+ 1;

			// �����̐��l�̏�����
			nMinAct = 1;
			nMaxAct = nMinAct + int(100 * g_aEnemy[nCntEnemy].fActProportion[0]);

			//--------------------------------------------------------
			// �s���̔���
			//--------------------------------------------------------
			for (int nCntAct = 0; nCntAct < ENEMY_ACT_TYPE_MAX; nCntAct++)
			{// �s���̎�ޕ����[�v
				if (nActType >= nMinAct &&
					nActType < nMaxAct)
				{// �����̒��ɓ����Ă��邩�𔻒�
					g_aEnemy[nCntEnemy].nActType = nCntAct;								// �s��������

					if (nCntAct == ENEMY_ACT_TYPE_WANDER)
					{// ������s���Ɍ��肵����
					 // �����_���Ɋp�x��ݒ�
						g_aEnemy[nCntEnemy].fWanderAngle = float(rand() % (628) + 0);								// 0�`628(3.14 * 100 * 2�������l(�p�x�𐮐��Ƃ��Ă���킷))�������_���ŏo��
						g_aEnemy[nCntEnemy].fWanderAngle = g_aEnemy[nCntEnemy].fWanderAngle / 100 - 3.14f;	// �����ŏo�����p�x��100�Ŋ�����3.14�ň����Ɛ��m�Ȋp�x�����߂�

					}
				}

				// ���肷��͈͂����炷
				nMinAct += int(100 * g_aEnemy[nCntEnemy].fActProportion[nCntAct]);		// �O��̍ő�l���ŏ��l�ɐݒ�
				nMaxAct += int(100 * g_aEnemy[nCntEnemy].fActProportion[nCntAct + 1]);	// �O��̍ő�l�ɍ���̐��l�����Z

			}
		}

		//---------------------------------------------------------
		// �s�������s
		//---------------------------------------------------------
		switch (g_aEnemy[nCntEnemy].nActType)
		{
		case ENEMY_ACT_TYPE_NEUTRAL:		// �_�����̎�
			// �������Ȃ�
			break;

		case ENEMY_ACT_TYPE_LOOK:			// �v���C���[�����鎞
			// �v���C���[�ւ̊p�x���Z�o
			Angle = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z);

			g_aEnemy[nCntEnemy].rotDest.y = Angle - D3DX_PI;								// �L�������v���C���[�̕����Ɍ�������

			break;

		case ENEMY_ACT_TYPE_WANDER:			// �������
			// �����_���ȕ����ɓ���
			g_aEnemy[nCntEnemy].Move.x += sinf(g_aEnemy[nCntEnemy].fWanderAngle) * g_aEnemy[nCntEnemy].fMoveSpeed;
			g_aEnemy[nCntEnemy].Move.z += cosf(g_aEnemy[nCntEnemy].fWanderAngle) * g_aEnemy[nCntEnemy].fMoveSpeed;

			g_aEnemy[nCntEnemy].rotDest.y = g_aEnemy[nCntEnemy].fWanderAngle - D3DX_PI;								// �L�����𑖂��Ă�������Ɍ�������

			break;


		case ENEMY_ACT_TYPE_APPROACHES:		// �ڋ߂̎�
			// �v���C���[�ւ̊p�x���Z�o
			Angle = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z);

			// �v���C���[�Ɍ������đ����Ă���
			g_aEnemy[nCntEnemy].Move.x += sinf(Angle) * g_aEnemy[nCntEnemy].fMoveSpeed;
			g_aEnemy[nCntEnemy].Move.z += cosf(Angle) * g_aEnemy[nCntEnemy].fMoveSpeed;

			g_aEnemy[nCntEnemy].rotDest.y = Angle - D3DX_PI;								// �L�����𑖂��Ă�������Ɍ�������

			break;

		case ENEMY_ACT_TYPE_ATTACK:		// �U���̎�
			if (pPlayer->bMotionState[PLAYER_BOOL_DAMAGE] == false &&
				pPlayer->bMotionState[PLAYER_BOOL_FLY]	 == false &&
				pPlayer->bMotionState[PLAYER_BOOL_GETUP]	 == false )
			{// �v���C���[������Ă����Ԃ���Ȃ����

				if (g_aEnemy[nCntEnemy].nAttackWaitTime <= g_aEnemy[nCntEnemy].nAttackCounter)
				{// �G���U���̑ҋ@���Ԃ��I����Ă�����
					g_aEnemy[nCntEnemy].bEnemyAttack[ENEMY_ATTACKTYPE_ATTACK] = true;			// �U�����ɐݒ�
					g_aEnemy[nCntEnemy].nAttackCounter = 0;										// �U���܂ł̎��Ԃ�������
					g_aEnemy[nCntEnemy].bMoved = true;											// ��������Ԃɐݒ�

				}
			}

			break;
		}

		if (pPlayer->bMotionState[PLAYER_BOOL_DAMAGE] == false &&
			pPlayer->bMotionState[PLAYER_BOOL_FLY]	 == false &&
			pPlayer->bMotionState[PLAYER_BOOL_GETUP]	 == false )
		{// �v���C���[������Ă����Ԃ���Ȃ����

			// �U������܂ł̎��Ԃ����Z
			g_aEnemy[nCntEnemy].nAttackCounter++;

		}

		// �s���̃J�E���^�[�����Z
		g_aEnemy[nCntEnemy].nActNowCounter++;												// ���t���[�����Z����
	}
}

//=============================================================================
// �G�e���f���̐ݒ�
//=============================================================================
void EnemyModelSet(void) 
{
	// �G���f���̐ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̑��������[�v
		for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
		{// �p�[�c�̐������[�v
			g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModel = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].nIdxModel;		// �e���f���̃C���f�b�N�X
			g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].nIdxParentModel;		// �e���f���̃C���f�b�N�X
			g_aEnemy[nCntEnemy].aModel[nCntParts].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].pos;								// �ʒu�̐ݒ�
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].rot;								// �����̐ݒ�

		}
	}
}

//=============================================================================
// �G�̃��[�V����
//=============================================================================
void EnemyMotion(int nCntEnemy)
{
	//-----------------------------------------------------------------------
	// ���[�V�����u�����h�̐ݒ�
	//-----------------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].Motionstate != g_aEnemy[nCntEnemy].MotionstateOld)
	{// ���[�V�����̃X�e�[�g���ς������
		g_aEnemy[nCntEnemy].nCounterFrame = 0;						// ���݂̃L�[�t���[���̃t���[������������
		g_aEnemy[nCntEnemy].nCounterKey = 0;							// ���݂̃L�[�t���[���̏�����
		g_aEnemy[nCntEnemy].nCounterAllFrame = 0;					// ���[�V�����̑S�t���[������������
		g_aEnemy[nCntEnemy].nFrame = ENEMY_MOTION_BLEND_FRAME;		// ���[�V�����u�����h�̃t���[������ݒ�
	}

	//-----------------------------------------------
	// ���[�V�������̓���
	//-----------------------------------------------

	switch (g_aEnemy[nCntEnemy].Motionstate)
	{// �G�̃��[�V������
	case ENEMY_MOTION_ATTACK:														// �U�����[�V����00��������
		
		if (g_aEnemy[nCntEnemy].nCounterAllFrame >= 20 &&
			g_aEnemy[nCntEnemy].nCounterAllFrame <= 30)
		{// �U�����[�V�����̉���Ƃ�
			// �O�ɑO�i����
			g_aEnemy[nCntEnemy].Move.x += (sinf(g_aEnemy[nCntEnemy].rot.y - D3DX_PI) * ENEMY_MOTION_MOVE_SPEED);
			g_aEnemy[nCntEnemy].Move.z += (cosf(g_aEnemy[nCntEnemy].rot.y - D3DX_PI) * ENEMY_MOTION_MOVE_SPEED);
			break;
		}
	}

	//-----------------------------------------------
	// ���[�V�������̌��ʉ�
	//-----------------------------------------------
	switch (g_aEnemy[nCntEnemy].Motionstate)
	{// �G�̃��[�V������
	case ENEMY_MOTION_GETUP:														// �_�E�����[�V������������
		if (g_aEnemy[nCntEnemy].nCounterAllFrame == 0)
		{// �N���オ��̉�
		}
		break;
	}
	

	//-----------------------------------------------------------------------
	// �L�[�t���[���̐؂�ւ��
	//-----------------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].nCounterFrame == g_aEnemy[nCntEnemy].nFrame)
	{// ���[�V�����u�����h�̎��A�t���[�������Ō�ɂȂ�����
		g_aEnemy[nCntEnemy].nCounterFrame = 0;															// ���݂̃L�[�t���[���̃t���[������������
		g_aEnemy[nCntEnemy].nFrame = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[g_aEnemy[nCntEnemy].nCounterKey].nFrame;	// ���[�V�����̃t���[������ݒ�
		g_aEnemy[nCntEnemy].nCounterKey++;																// �L�[�t���[����i�߂�

			if (g_aEnemy[nCntEnemy].nCounterKey == g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nNumKey)
			{// �L�[�t���[�����Ō�܂ŗ�����
				if (g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nLoop == 0)
				{// ���[�v���Ȃ��ꍇ
					switch (g_aEnemy[nCntEnemy].Motionstate)
					{// �G�̃��[�V������
					case ENEMY_MOTION_ATTACK:																		// �U�����[�V������������
						g_aEnemy[nCntEnemy].bEnemyAttack[ENEMY_ATTACKTYPE_ATTACK] = false;							// �U��������Ȃ���Ԃɐݒ�
						// �U���ł��Ȃ����Ԃ������_���ɐݒ�
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;				
						g_aEnemy[nCntEnemy].nCounterKey--;															// �L�[�t���[����߂�
						break;

					case ENEMY_MOTION_JUMP:																// �W�����v���[�V������������
						g_aEnemy[nCntEnemy].nCounterKey--;															// �L�[�t���[����߂�
						break;

					case ENEMY_MOTION_LANDING:															// ���n���[�V������������
						g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = false;														// ���n��Ԃ���Ȃ���Ԃɂ���
						g_aEnemy[nCntEnemy].nCounterKey--;															// �L�[�t���[����߂�

						break;

					case ENEMY_MOTION_DAMAGE:															// �_���[�W���[�V������������
						g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] = false;												// �_���[�W��Ԃ���Ȃ���Ԃɂ���

						// �U����Ԃ̃��Z�b�g
						for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
						{// �U���̎�ޕ����[�v
							g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack]		 = false;					// �U����Ԃ����Z�b�g����
							g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;					// �U����Ԃ����Z�b�g����
						}
						// �U���ł��Ȃ����Ԃ������_���ɐݒ�
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;

						g_aEnemy[nCntEnemy].bMoved = true;												// ��������Ԃɐݒ�

						g_aEnemy[nCntEnemy].nCounterKey--;												// �L�[�t���[����߂�

						break;

					case ENEMY_MOTION_FLY:																// ������у��[�V������������

						// �U����Ԃ̃��Z�b�g
						for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
						{// �U���̎�ޕ����[�v
							g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;						// �U����Ԃ����Z�b�g����
							g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;					// �U����Ԃ����Z�b�g����
						}
						// �U���ł��Ȃ����Ԃ������_���ɐݒ�
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;

						g_aEnemy[nCntEnemy].nCounterKey--;												// �L�[�t���[����߂�

						break;

					case ENEMY_MOTION_GETUP:			// �N���オ���ԃ��[�V������������
						// �N���オ���Ԃ̃��Z�b�g
						for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
						{// �U���̎�ޕ����[�v
							g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;						// �U����Ԃ����Z�b�g����
							g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;					// �U����Ԃ����Z�b�g����
						}

						g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = false;		// �N���オ���ԉ���

						g_aEnemy[nCntEnemy].bMoved = true;											// ��������Ԃɐݒ�


						// �U���ł��Ȃ����Ԃ������_���ɐݒ�
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;

						g_aEnemy[nCntEnemy].nCounterKey--;												// �L�[�t���[����߂�

						break;
					}
				}
				else
				{// ���[�v����
					g_aEnemy[nCntEnemy].nCounterKey = (g_aEnemy[nCntEnemy].nCounterKey) % g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nNumKey;					// �L�[�t���[����i�߂�
					g_aEnemy[nCntEnemy].nCounterAllFrame = 0;																			// ���[�V�����̃t���[������������
				}
			}
	}

	//-----------------------------------------------------------------------
	// ���[�V�����̍�������
	//-----------------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].nCounterFrame == 0)
	{// �t���[�������ŏ��̎�
		for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
		{// �p�[�c�̐������[�v
			// ���[�V�����̈ʒu��������
			g_aEnemy[nCntEnemy].posMotion[nCntParts] = (g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].pos + g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[g_aEnemy[nCntEnemy].nCounterKey].pos[nCntParts]) - g_aEnemy[nCntEnemy].aModel[nCntParts].pos;
			// ���[�V�����̉�]��������
			g_aEnemy[nCntEnemy].rotMotion[nCntParts] = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[g_aEnemy[nCntEnemy].nCounterKey].rot[nCntParts] - g_aEnemy[nCntEnemy].aModel[nCntParts].rot;

			//----------------------------------------------------
			// �����̉�]���C������
			//----------------------------------------------------
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].x > D3DX_PI)
			{// +3.14���A�����Ă��܂�����
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].x -= D3DX_PI * 2.0f;
			}
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].x < -D3DX_PI)
			{// -3.14���A���������
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].x += D3DX_PI * 2.0f;
			}
			// ���f���̉�]���C������
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].y > D3DX_PI)
			{// +3.14���A�����Ă��܂�����	
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].y -= D3DX_PI * 2.0f;
			}
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].y < -D3DX_PI)
			{// -3.14���A���������		   
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].y += D3DX_PI * 2.0f;
			}
			// ���f���̉�]���C������
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].z > D3DX_PI)
			{// +3.14���A�����Ă��܂�����	   
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].z -= D3DX_PI * 2.0f;
			}
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].z < -D3DX_PI)
			{// -3.14���A���������		   
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].z += D3DX_PI * 2.0f;
			}
		}
	}

	//----------------------------------------------
	// ���[�V�����̓���
	//----------------------------------------------
	for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
	{// �p�[�c�̐������[�v
		// �ʒu�������t���[�����Ŋ������l�����Z���Ă���
		g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x += g_aEnemy[nCntEnemy].posMotion[nCntParts].x / g_aEnemy[nCntEnemy].nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���
		g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y += g_aEnemy[nCntEnemy].posMotion[nCntParts].y / g_aEnemy[nCntEnemy].nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���
		g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z += g_aEnemy[nCntEnemy].posMotion[nCntParts].z / g_aEnemy[nCntEnemy].nFrame;		// �ʒu�̍������t���[�����Ŋ������l�����Z���Ă���

		// ��]�������t���[�����Ŋ������l�����Z���Ă���
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x += g_aEnemy[nCntEnemy].rotMotion[nCntParts].x / g_aEnemy[nCntEnemy].nFrame;		// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y += g_aEnemy[nCntEnemy].rotMotion[nCntParts].y / g_aEnemy[nCntEnemy].nFrame;		// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z += g_aEnemy[nCntEnemy].rotMotion[nCntParts].z / g_aEnemy[nCntEnemy].nFrame;		// ��]�̍������t���[�����Ŋ������l�����Z���Ă���
																		
		// ���f���̉�]���C������
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x > D3DX_PI)
		{// +3.14���A�����Ă��܂�����
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x < -D3DX_PI)
		{// -3.14���A���������
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x += D3DX_PI * 2.0f;
		}
		// ���f���̉�]���C������
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y > D3DX_PI)
		{// +3.14���A�����Ă��܂�����	
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y < -D3DX_PI)
		{// -3.14���A���������		   
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y += D3DX_PI * 2.0f;
		}
		// ���f���̉�]���C������
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z > D3DX_PI)
		{// +3.14���A�����Ă��܂�����	   
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z < -D3DX_PI)
		{// -3.14���A���������		   
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z += D3DX_PI * 2.0f;
		}
	}
	g_aEnemy[nCntEnemy].nCounterFrame++;		// ���݃L�[�t���[���̃t���[���������Z
	g_aEnemy[nCntEnemy].nCounterAllFrame++;	// ���[�V�����̃t���[���������Z
}


//=============================================================================
// �G�̐ݒu
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE EnemyType,bool bMove)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̑��������[�v

		if (g_aEnemy[nCntEnemy].bUse == false)
		{// �g���ĂȂ����
			g_aEnemy[nCntEnemy].pos = pos;									// �ʒu��ݒ�
			g_aEnemy[nCntEnemy].posOld = pos;								// �O��ʒu�̏����ݒ�
			g_aEnemy[nCntEnemy].InitPos = pos;								// �����ʒu
			g_aEnemy[nCntEnemy].Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�̈ړ��ʂ̏�����
			g_aEnemy[nCntEnemy].rot = rot;									// �����̏����ݒ�
			g_aEnemy[nCntEnemy].rotDest = rot;								// �ړI�̌�����������

			// �G�̎�ށ��X�e�[�^�X��ݒ�
			g_aEnemy[nCntEnemy].EnemyType = EnemyType;												// �G�̃^�C�v��ݒ�
			g_aEnemy[nCntEnemy].bMoved = bMove;														// ��������Ԃ��ǂ���
			g_aEnemy[nCntEnemy].fMoveSpeed = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].fMoveSpeed;	// �ړ����x��ݒ�
			g_aEnemy[nCntEnemy].fJamp = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].fJamp;			// �W�����v�͂�ݒ�
			g_aEnemy[nCntEnemy].fRadius = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].fRadius;		// �����蔻��̔��a��ݒ�
			g_aEnemy[nCntEnemy].nLife = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nLife;			// ���C�t��ݒ�
			g_aEnemy[nCntEnemy].nMaxLife = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nLife;		// ���C�t��ݒ�
			g_aEnemy[nCntEnemy].fJampPower = 0.0f;													// �W�����v�͂�������
			g_aEnemy[nCntEnemy].fFlyPower = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// ������ї͂�������

			// ��Ԃ̕ϐ��̏�����
			g_aEnemy[nCntEnemy].State = ENEMY_STATE_NORMAL;				// �ʏ��Ԃɏ�����
			g_aEnemy[nCntEnemy].nCounterState = 0;						// �X�e�[�g�̃J�E���^�[��������

			// �s���n�̕ϐ��̏�����
			g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;
			g_aEnemy[nCntEnemy].nAttackCounter = 0;								// �U������܂ł̃J�E���^�[��������
			g_aEnemy[nCntEnemy].nActTime = rand() % (ENEMY_ACT_MAX_TIME - ENEMY_ACT_MIN_TIME + 1) + ENEMY_ACT_MIN_TIME;		// �s���̎��Ԃ̕ϐ��������_���Őݒ�
			g_aEnemy[nCntEnemy].nActNowCounter = 0;								// �s���̃J�E���^�[�ϐ���������

			g_aEnemy[nCntEnemy].nDownCounter = 0;								// �|��Ă��鎞�Ԃ�������
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] = false;		// �W�����v��Ԃ�������
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = false;	// ���n������Ȃ���Ԃɏ�����
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] = false;		// �_���[�W������Ȃ���Ԃɏ�����
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = false;		// ������ђ�����Ȃ���Ԃɏ�����
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = false;		// �_�E��������Ȃ���Ԃɏ�����

			g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_NEUTRAL;				// ���[�V�������j���[�g�����ɏ�����
			g_aEnemy[nCntEnemy].MotionstateOld = ENEMY_MOTION_MAX;				// �O��̃��[�V�������j���[�g�����ɏ�����

			// ���[�V�����̕ϐ��̏�����
			g_aEnemy[nCntEnemy].nCounterKey = 0;								// ���݂̃L�[�t���[����������
			g_aEnemy[nCntEnemy].nCounterFrame = 0;								// ���݂̃t���[������������
			g_aEnemy[nCntEnemy].nCounterAllFrame = 0;							// ���݂̑S�t���[������������
			g_aEnemy[nCntEnemy].nFrame = 0;										// ����̃t���[������������

			// �U����Ԃ�ݒ�
			for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
			{// �U���̎�ޕ����[�v
				g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;			// �U��������Ȃ���Ԃɐݒ�
				g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;		// �U���̏�����Ԃ���Ȃ���Ԃɐݒ�
			}

			// UI�̐ݒ�
			if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_ARMOR)
			{// �Z�̎��̐ݒ�
				//�@���C�t�ݒ�
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 120, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 120, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 120, true);

			}
			else if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_HITOME)
			{// ��ڂ̎��̐ݒ�
				//�@���C�t�ݒ�
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 100, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 100, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 100, true);


			}
			else if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_HIGECHAN)
			{// �Ђ������̎��̐ݒ�
				// �D�x�̃R�}���hUI�̐ݒ�i�I�t�Z�b�g�̈ʒu,���S����̋���,�G�̔ԍ�,�D�x�̃^�C�v�j
				//�@���C�t�ݒ�
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 50, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 50, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 50, true);
			}
			else if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_HONCHAN)
			{// �{�����̎��̐ݒ�
				//�@���C�t�ݒ�
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 80, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 80, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 80, true);
			}

			// �e��ݒ�
			g_aEnemy[nCntEnemy].nIdyShadow = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), g_aEnemy[nCntEnemy].rot,g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius);

			//----------------------------------------------
			// �U���̓����蔻��̐ݒ�
			//----------------------------------------------
			for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
			{// �L�����N�^�[�̍U���̓����蔻��̑��������[�v
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nIdxParent;		// �e�p�[�c�̔ԍ��̐ݒ�
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].pos;				// �I�t�Z�b�g�̐ݒ�
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].fRadius = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].fRadius;			// ���a�̐ݒ�
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionStart = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nCollisionStart;	// ���肪�n�܂�t���[���̐ݒ�
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionEnd = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nCollisionEnd;		// ���肪�I���t���[���̐ݒ�
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nMotionType = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nMotionType;		// ���肪���̃��[�V�����̐ݒ�
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse = true;																								// �����蔻����g�p��Ԃɐݒ�
			}

			//----------------------------------------------
			// �̂̓����蔻��̐ݒ�
			//----------------------------------------------
			for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
			{// �L�����N�^�[�̑̂̓����蔻��̑��������[�v
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionBody[nCntCollisionBody].nIdxParent;		// �e�p�[�c�̔ԍ��̐ݒ�
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionBody[nCntCollisionBody].pos;				// �I�t�Z�b�g�̐ݒ�
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionBody[nCntCollisionBody].fRadius;			// ���a�̐ݒ�
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse = true;																								// �����蔻����g�p��Ԃɐݒ�
			}

			g_aEnemy[nCntEnemy].bUse = true;		// �g���Ă����Ԃɐݒ�
			g_nNumEnemy++;							// �G�̐l���𑝂₷

			//--------------------------
			// �����|�[�Y��ݒ�
			//--------------------------
			for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
			{// �p�[�c�̐������[�v
				// ���f���̈ʒu���j���[�g�������[�V�����ɏ�����
				g_aEnemy[nCntEnemy].aModel[nCntParts].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].pos + g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[0].pos[nCntParts];
				// ���f���̉�]���j���[�g�������[�V�����ɏ�����
				g_aEnemy[nCntEnemy].aModel[nCntParts].rot = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[0].rot[nCntParts];
			}

			//------------------------------------------------------------------
			// ���[���h�}�g���N�X�̐ݒ�
			//------------------------------------------------------------------
			LPDIRECT3DDEVICE9	pDevice = GetDevice();		// �f�o�C�X�̏�����
			D3DXMATRIX			mtxParent;					// �K�w�\���p�̃}�g���b�N�X
			D3DXMATRIX mtxRot, mtxTrans;					// �v�Z�p�}�g���b�N�X

			 // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// ��](����)�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,g_aEnemy[nCntEnemy].rot.y,g_aEnemy[nCntEnemy].rot.x,g_aEnemy[nCntEnemy].rot.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,&g_aEnemy[nCntEnemy].mtxWorld,&mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y,g_aEnemy[nCntEnemy].pos.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,&g_aEnemy[nCntEnemy].mtxWorld,&mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
			{// �p�[�c�̐������[�v

				if (g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel == -1)
				{// �e���f�������Ȃ��Ƃ�
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;				// �G�̃}�g���b�N�X����
				}
				else
				{// �e�̃��f��������Ƃ�
					mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel].mtxWorld;		// �e�̃}�g���b�N�X����
				}

				//---------------------------
				// ���f���̃��[���h�}�g���N�X�̐ݒ�
				//---------------------------
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);
				// ��](����)�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y,g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x,g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);
				// ��](����)��ݒ�
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,	&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,&mtxRot);
				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x,g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y,g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);
				// �ړ���ݒ�
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,	&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,&mtxTrans);
				// �e�̃��[���h�}�g���b�N�X�𔽉f
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,	&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,&mtxParent);
				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				//---------------------------------
				// �U���̓����蔻��
				//---------------------------------
				for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
				{// �����蔻��̐������[�v

					if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse == true)
					{// �U���̓����蔻�肪�g���Ă����ԂȂ珈������

						if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent == -1)
						{// �e���f�������Ȃ��Ƃ�
							mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// �G�̃��[���h�}�g���b�N�X����
						}
						else
						{// �e���f��������Ƃ�
							mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent].mtxWorld;		// �e���f���̃}�g���b�N�X����
						}

						// ���[���h�}�g���b�N�X�̏�����
						D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);
						// �ړ��𔽉f
						D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.x,	g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.y,g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.z);
						// �ړ���ݒ�
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&mtxTrans);
						// �e�̃��[���h�}�g���b�N�X�𔽉f
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&mtxParent);
						// ���[���h�}�g���b�N�X�̐ݒ�
						pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);
					}
				}

				//---------------------------------
				// �̂̓����蔻��
				//---------------------------------
				for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
				{// �����蔻��̐������[�v

					if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
					{// �̂̓����蔻�肪�g���Ă����ԂȂ珈������
						if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent == -1)
						{// �e���f�������Ȃ��Ƃ�
							mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// �G�̃��[���h�}�g���b�N�X����
						}
						else
						{// �e���f��������Ƃ�
							mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent].mtxWorld;		// �e���f���̃}�g���b�N�X����
						}

						// ���[���h�}�g���b�N�X�̏�����
						D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);
						// �ړ��𔽉f
						D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.x,g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.y,g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.z);
						// �ړ���ݒ�A�Q�ԖڂƂR�Ԗڂ̈������|���Z���ꂽ�l����Ԗڂ̈����ɑ�����Ă���
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,&mtxTrans);
						// �e�̃��[���h�}�g���b�N�X�𔽉f
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,	&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,&mtxParent);
						// ���[���h�}�g���b�N�X�̐ݒ�
						pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);
					}
				}
			}
			break;
		}

	}
}

//=============================================================================
// �G���d�Ȃ�Ȃ����߂̓����蔻��
//=============================================================================
void CollisionEnemy(D3DXVECTOR3 *pPos, float fRadius)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̐������[�v
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �G���g���Ă���Ƃ�
			if (g_aEnemy[nCntEnemy].pos.x != pPos->x &&
				g_aEnemy[nCntEnemy].pos.y != pPos->y &&
				g_aEnemy[nCntEnemy].pos.z != pPos->z)
			{// �G�ƃ|�C���^�̈ʒu���ꏏ����Ȃ����(�����L�������m)

				// �����蔻��̍��W�̋����̌v�Z
				float posX = (g_aEnemy[nCntEnemy].pos.x - pPos->x);					// (�G�̑̂̓����蔻��̍��W�|�U���̓����蔻��̍��W)��2�悷��
				float posY = (g_aEnemy[nCntEnemy].pos.y - pPos->y);					// (�G�̑̂̓����蔻��̍��W�|�U���̓����蔻��̍��W)��2�悷��
				float posZ = (g_aEnemy[nCntEnemy].pos.z - pPos->z);					// (�G�̑̂̓����蔻��̍��W�|�U���̓����蔻��̍��W)��2�悷��
				float fPosLenght = (posX * posX) + (posY * posY) + (posZ * posZ);	// ���ꂼ���2�悵�ċ������v�Z

				// �����蔻��̔��a�̋����̌v�Z
				float fRadiusLenght = (g_aEnemy[nCntEnemy].fRadius + fRadius) * (g_aEnemy[nCntEnemy].fRadius + fRadius);			// (�{�b�N�X�̔��a�|�U���̔��a)��2�悵�ċ������v�Z

																																// ���W�̋����Ɣ��a�̋����̔�r
				if (fPosLenght  < fRadiusLenght)
				{// ���a�̋������A���W�̋����̂ق���������������
					float fPosAngle = atan2f(g_aEnemy[nCntEnemy].pos.x - pPos->x,		// �G�ƃL�����̈ʒu�̊p�x���v�Z
											 g_aEnemy[nCntEnemy].pos.z - pPos->z);

					g_aEnemy[nCntEnemy].pos.x = pPos->x + (sinf(fPosAngle)*(g_aEnemy[nCntEnemy].fRadius + fRadius));	// �L�����̈ʒu�����炷
					g_aEnemy[nCntEnemy].pos.z = pPos->z + (cosf(fPosAngle)*(g_aEnemy[nCntEnemy].fRadius + fRadius));	// �L�����̈ʒu�����炷

				}
			}
		}
	}
}

//=============================================================================
// �G�̑̂̓����蔻��
//=============================================================================
void CollisionEnemyBody(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, int nPower, float fDamageSide, float fDamageVertical)
{
	Player *pPlayer = GetPlayer();	// �v���C���[�̏����擾

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G�̐������[�v

		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �G���g���Ă���Ƃ�

			if (g_aEnemy[nCntEnemy].State == ENEMY_STATE_NORMAL)
			{// �ʏ��ԂȂ�

				for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
				{// �����蔻��̐������[�v

					if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
					{// �����蔻�肪�g���Ă���Ƃ�

						// �����蔻��̍��W�̋����̌v�Z
						float posX = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41 - mtxWorld.x);				// (�G�̑̂̓����蔻��̍��W�|�U���̓����蔻��̍��W)��2�悷��
						float posY = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42 - mtxWorld.y);				// (�G�̑̂̓����蔻��̍��W�|�U���̓����蔻��̍��W)��2�悷��
						float posZ = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43 - mtxWorld.z);				// (�G�̑̂̓����蔻��̍��W�|�U���̓����蔻��̍��W)��2�悷��
						float fPosLenght = (posX * posX) + (posY * posY) + (posZ * posZ);											// ���ꂼ���2�悵�ċ������v�Z

						// �����蔻��̔��a�̋����̌v�Z
						float fRadiusLenght = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius + fRadius) *
											  (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius + fRadius);			// (�{�b�N�X�̔��a+�U���̔��a)��2�悵�ċ������v�Z

						// ���W�̋����Ɣ��a�̋����̔�r
						if (fPosLenght  < fRadiusLenght)
						{// ���a�̋������A���W�̋����̂ق���������������
							float fAttackAngle = atan2f(pos.x - g_aEnemy[nCntEnemy].pos.x,		// ���������p�x���v�Z
														pos.z - g_aEnemy[nCntEnemy].pos.z);

							// ���C�t����
							g_aEnemy[nCntEnemy].nLife -= nPower;

							GetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].nLife);	// �A��

							AddHit(1);	// �q�b�g

				


							for (int nCntEffect = 0; nCntEffect < 11; nCntEffect++)
							{// �Ή�
								float nDirection = SetRandomEffect();
								float nDirectio2 = SetRandomEffect();
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
									D3DXVECTOR3(sinf(nDirection) * (float(rand() % 50 / 10.0f) + 3), cosf(nDirection) * (float(rand() % 50 / 10.0f) + 3), cosf(nDirectio2) * (float(rand() % 30 / 10.0f) + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(1.0f, float(rand() % 100 / 100.0f), 0.0f, 0.8f), float(rand() % 20) + 10, 30, EFFECTTYPE_HITEXPLOSION);
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
									D3DXVECTOR3(sinf(nDirection) *(float(rand() % 50 / 10.0f) + 3), cosf(nDirectio2) * (float(rand() % 30 / 10.0f) + 3), cosf(nDirection) * (float(rand() % 50 / 10.0f) + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(0.8f, float(rand() % 100 / 100.0f), 0.0f, 0.8f), float(rand() % 30) + 10, 30, EFFECTTYPE_HITEXPLOSION);
							}
							for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
							{// �Ռ���
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
									D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(0.8f, 0.5f, 0.0f, 0.8f), 10, 30, EFFECTTYPE_IMPACT);
							}
								// �G�����񂾂��ǂ���
								if (g_aEnemy[nCntEnemy].nLife <= 0)
								{// ���C�t��0�ȉ��ɂȂ�����

									// �G�̏�Ԃ�ݒ�
									g_aEnemy[nCntEnemy].State = ENEMY_STATE_DEATH;				// ���񂾏�Ԃɂ���
									g_aEnemy[nCntEnemy].nCounterState = ENEMY_DEATH_COUNTER;		// �J�E���^�[��ݒ肷��

									g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = true;								// ������ђ��ɐݒ�

								}
								else
								{// ���C�t���c���Ă���Ƃ�

									// �G�̏�Ԃ�ݒ�
									g_aEnemy[nCntEnemy].State = ENEMY_STATE_DAMAGE;				// �_���[�W��Ԃɂ���
									g_aEnemy[nCntEnemy].nCounterState = ENEMY_DAMAGE_COUNTER;	// �J�E���^�[��ݒ肷��

								}

								if (fDamageSide > 0.0f || fDamageVertical > 0.0f)
								{// ������ї͂�����Ƃ�

									// ���������̐�����ї͂���
									g_aEnemy[nCntEnemy].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
									g_aEnemy[nCntEnemy].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;

									// ���������̐�����ї͂���
									g_aEnemy[nCntEnemy].fFlyPower.y = fDamageVertical;

									g_aEnemy[nCntEnemy].nFlyCounter = 10;

									g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = true;												// ������ђ��ɐݒ�

									g_aEnemy[nCntEnemy].rotDest.y = fAttackAngle - D3DX_PI;								// �L�����N�^�[�̌�����ύX

									// �U�����ꂽ������ї͂���
									g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[ENEMY_MOTION_FLY].fDamageSide = fDamageSide;							// ������ї͂��㏑������
									g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[ENEMY_MOTION_FLY].fDamageVertical = fDamageVertical;					// ������ї͂��㏑������

									// ���ʉ��Đ�
									PlaySound(SOUND_LABEL_SE_HIT_0);

								}
								else
								{// ������ї͂��Ȃ��Ƃ�
									g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] = true;											// �_���[�W���ɐݒ�

									// ���ʉ��Đ�
									PlaySound(SOUND_LABEL_SE_HIT_1);
								}

								break;																					// �ق��̓����蔻��͏������Ȃ�����
						}
					}
				}
			}
		}
	}
}


//=============================================================================
// �G�̍폜
//=============================================================================
void DeletEnemy(int nCntEnemy, bool bKo)
{
	if (bKo == true)
	{// �G��|�����Ȃ�

		// ���C�t�Q�[�W�̍폜
		DeleteBillLife(nCntEnemy);
		DeleteShadow(g_aEnemy[nCntEnemy].nIdyShadow);				// �e���폜

		// �G�t�F�N�g
		for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
		{
			float fRandRadius = float(rand() % 60 + 30);
			float nDirection = SetRandomEffect();
			float nDirection2 = SetRandomEffect();
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection) * 7, cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) * 7), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 20, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 7, cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) * 7), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 20, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 3, cosf(nDirection2) * 3, cosf(nDirection) * 3), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 3, sinf(nDirection) * 3, cosf(nDirection) * 3), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 2, cosf(nDirection2) * 1, cosf(nDirection) * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 30, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 2, sinf(nDirection) * 1, cosf(nDirection) * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 30, 30, EFFECTTYPE_DEATH);
		}
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		// �A�C�e�����o��������
		SetItem(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[0].mtxWorld._41,				// �ݒu����ʒu
			g_aEnemy[nCntEnemy].CollisionBody[0].mtxWorld._42,				// �ݒu����ʒu
			g_aEnemy[nCntEnemy].CollisionBody[0].mtxWorld._43),				// �ݒu����ʒu
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),												// �ړ���
			ITEM_KOUTOKU);													// �ݒu������


		// �G�̑��������炷
		g_nNumEnemy--;

		g_aEnemy[nCntEnemy].bUse = false;							// �G���g�p���Ă��Ȃ���Ԃɂ���

		// ���ʉ��Đ�
		PlaySound(SOUND_LABEL_SE_HIT_1);
	}
	else
	{// �|�����킯����Ȃ��Ȃ�
		// ���C�t�Q�[�W�̍폜
		DeleteBillLife(nCntEnemy);

		DeleteShadow(g_aEnemy[nCntEnemy].nIdyShadow);				// �e���폜

		//----------------------------------------------
		// �U���̓����蔻��̍폜
		//----------------------------------------------
		for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
		{// �L�����N�^�[�̍U���̓����蔻��̑��������[�v
			g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse = false;	// �����蔻����g�p���Ă��Ȃ���Ԃɂ���
		}

		//----------------------------------------------
		// �̂̓����蔻��̍폜
		//----------------------------------------------
		for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
		{// �L�����N�^�[�̑̂̓����蔻��̑��������[�v
			g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse = false;				// �����蔻����g�p���Ă��Ȃ���Ԃɂ���
		}

		// �G�̑��������炷
		g_nNumEnemy--;

		g_aEnemy[nCntEnemy].bUse = false;							// �G���g�p���Ă��Ȃ���Ԃɂ���
	}
	
}

//=============================================================================
// �G�̏��̃��[�h
//=============================================================================
void EnemyLode(void)
{
	FILE *pFile = NULL;									// �t�@�C���̃|�C���^�ϐ�
	char acLine[MAX_CHAR];							// 1�s�ۂ��ƋL�����邽�߂̕ϐ�
	char acData[MAX_CHAR];							// �ꕔ�̕����L�����邽�߂̕ϐ�

	//--------------------------------------------------------------------------
	// ���f���̃A�h���X�����[�V�������̓ǂݍ���
	//--------------------------------------------------------------------------
	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// �G�̎�ޕ����[�v

		// �ϐ��錾&������
		int nCntModel		  = 0;								// �G�̃p�[�c��
		int nCntModelIdx	  = 0;								// �p�[�c�̃C���f�b�N�X
		int nCntMotion		  = 0;								// ���[�V�����̐�
		int nCntParts;		  									// �p�[�c�̃J�E���g�ϐ�
		int nCntKey			  = 0;								// �L�[�t���[���̃J�E���g�ϐ�
		int nCntCollisionAttack	  = 0;								// �U���̓����蔻��̃J�E���g�ϐ�
		int nCntCollisionBody = 0;								// �̂̓����蔻��̃J�E���g�ϐ�
		g_EnemyInfo[nCntEnemyType].nCntMaxCollisionAttack = 0;	// �U���̓����蔻��̍ő吔��������
		g_EnemyInfo[nCntEnemyType].nCntMaxCollisionBody	  = 0;	// �̂̓����蔻��̍ő吔��������

		// �t�@�C�����J��
		if (nCntEnemyType == ENEMY_TYPE_ARMOR)
		{// �Z�̓ǂݍ���
			pFile = fopen("data/TEXT/motion_armor.txt", "r");
		}
		else if (nCntEnemyType == ENEMY_TYPE_HITOME)
		{// ��ڂ̓ǂݍ���
			pFile = fopen("data/TEXT/motion_hitome.txt", "r");
		}
		else if (nCntEnemyType == ENEMY_TYPE_HIGECHAN)
		{// �Ђ������̓ǂݍ���
			pFile = fopen("data/TEXT/motion_higechan.txt", "r");
		}
		else if (nCntEnemyType == ENEMY_TYPE_HONCHAN)
		{// �{�����̓ǂݍ���
			pFile = fopen("data/TEXT/motion_honchan.txt", "r");
		}


		//-----------------------------------------------------------------------------
		// �G�̃��f���E���[�V�����̓ǂݍ���
		//-----------------------------------------------------------------------------
		if (pFile != NULL)
		{// �k���`�F�b�N

			while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
			{
				sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[

				if (strcmp(acData, "NUM_MODEL") == 0)
				{// ���f���̍ő吔����������
					sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].MaxModel);		// ���f���̍ő吔���擾
				}
				//----------------------------------------
				// ���f���̃A�h���X�̎擾
				//----------------------------------------
				if (strcmp(acData, "MODEL_FILENAME") == 0)
				{// �t�@�C���̃A�h���X����������
					sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].cIP[0]);		// ���f���̃A�h���X���擾				
					nCntModel++;
				}

				//----------------------------------------
				// �L�����̃I�t�Z�b�g�̎擾
				//----------------------------------------
				if (strcmp(acData, "CHARACTERSET") == 0)
				{// �L�����Z�b�g�̕����񂪂�������
					while (strcmp(acData, "END_CHARACTERSET") != 0)
					{// END_CHARACTERSET��������܂Ń��[�v����
						fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
						sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

						if (strcmp(acData, "NUM_PARTS") == 0)
						{// NUM_PARTS�̕����񂪂�������
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].MaxParts);		// �p�[�c�����擾
						}
						else if (strcmp(acData, "MOVE") == 0)
						{// MOVE�̕����񂪂�������
							sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].fMoveSpeed);		// �ړ����x���擾
						}
						else if (strcmp(acData, "JUMP") == 0)
						{// JUMP�̕����񂪂�������
							sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].fJamp);		// �W�����v�͂��擾
						}
						else if (strcmp(acData, "RADIUS") == 0)
						{// RADIUS�̕����񂪂�������
							sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].fRadius);		// ���a���擾
						}
						else if (strcmp(acData, "LIFE") == 0)
						{// LIFE�̕����񂪂�������
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].nLife);			// �̗͂��擾
						}
						else if (strcmp(acData, "PARTSSET") == 0)
						{// PARTSSET�̕����񂪂�������
							while (strcmp(acData, "END_PARTSSET") != 0)
							{// END_PARTSSET��������܂Ń��[�v����
								fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
								sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

								if (strcmp(acData, "INDEX") == 0)
								{// INDEX����������
									sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].nIdxModel);	// �C���f�b�N�X���擾
								}
								else if (strcmp(acData, "PARENT") == 0)
								{// PARENT����������
									sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].nIdxParentModel);	// ���f���̐e�̃C���f�b�N�X���擾
								}
								else if (strcmp(acData, "POS") == 0)
								{// POS����������
									sscanf(&acLine[0],
										"%s %s %f %f %f",
										&acData[0],
										&acData[0],
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].pos.x,	// �I�t�Z�b�g�̈ʒu���擾
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].pos.y,	// �I�t�Z�b�g�̈ʒu���擾
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].pos.z);	// �I�t�Z�b�g�̈ʒu���擾
								}
								else if (strcmp(acData, "ROT") == 0)
								{// ROT����������
									sscanf(&acLine[0],
										"%s %s %f %f %f",
										&acData[0],
										&acData[0],
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].rot.x,	// �I�t�Z�b�g�̈ʒu���擾
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].rot.y,	// �I�t�Z�b�g�̈ʒu���擾
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].rot.z);	// �I�t�Z�b�g�̈ʒu���擾
								}


							}
							nCntModelIdx++;			// ���f���̃J�E���g��i�߂�
						}
						if (strcmp(acData, "BODY_COLLISION") == 0)
						{// COLLISION����������
							sscanf(&acLine[0],
								"%s %s %hhd %f %f %f %f",
								&acData[0],
								&acData[0],
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].nIdxParent,			// �I�t�Z�b�g�̐e�p�[�c�̃C���f�b�N�X���擾
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].pos.x,					// �I�t�Z�b�g�̈ʒuX���W
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].pos.y,					// �I�t�Z�b�g�̈ʒuY���W
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].pos.z,					// �I�t�Z�b�g�̈ʒuZ���W
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].fRadius); 				// ���a
							g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].bUse = true;				// �����蔻���true�ɂ���
							g_EnemyInfo[nCntEnemyType].nCntMaxCollisionBody++;						// �̂̓����蔻��̑������J�E���g����
							nCntCollisionBody++;														// �J�E���g�ϐ���i�߂�
						}

					}
				}

				//----------------------------------------
				// �L�����̃��[�V�����̎擾
				//----------------------------------------
				if (nCntMotion < ENEMY_MOTION_MAX)
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
								sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].nLoop);	// ���[�v���邩�ǂ������擾

							}
							else if (strcmp(acData, "NUM_KEY") == 0)
							{// NUM_KEY����������
								sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].nNumKey);	// �L�[�����擾
							}
							else if (strcmp(acData, "COLLISION") == 0)
							{// COLLISION����������
								sscanf(&acLine[0],
									"%s %s %hhd %f %f %f %f %hd %hd",
									&acData[0],
									&acData[0],
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nIdxParent,				// �I�t�Z�b�g�̐e�p�[�c�̃C���f�b�N�X���擾
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].pos.x,					// �I�t�Z�b�g�̈ʒuX���W
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].pos.y,					// �I�t�Z�b�g�̈ʒuY���W
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].pos.z,					// �I�t�Z�b�g�̈ʒuZ���W
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].fRadius, 				// ���a
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nCollisionStart, 		// ����J�n�̃t���[����
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nCollisionEnd);			// ����I���̃t���[����
								g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nMotionType = nCntMotion;		// ���肪�����[�V������ݒ�
								g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].bUse = true;					// �����蔻���true�ɂ���

								g_EnemyInfo[nCntEnemyType].nCntMaxCollisionAttack++;									// �U���̓����蔻��̑������J�E���g����
								nCntCollisionAttack++;																		// �J�E���g�ϐ���i�߂�
							}
							else if (strcmp(acData, "COLLISION_DAMAGE") == 0)
							{// COLLISION����������
								sscanf(&acLine[0],
									"%s %s %f %f %hhd",
									&acData[0],
									&acData[0],
									&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].fDamageSide,
									&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].fDamageVertical,
									&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].nAttackPower);								// ���[�V�����̍U���͂�ݒ�
							}

							else if (strcmp(acData, "KEYSET") == 0)
							{// KEYSET����������
								nCntParts = 0;						// �p�[�c�̃J�E���g�ϐ���������
								while (strcmp(acData, "END_KEYSET") != 0)
								{// END_KEYSET������܂Ń��[�v
									fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
									sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

									if (strcmp(acData, "FRAME") == 0)
									{// FRAME����������
										sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].nFrame);	// �L�[�̃t���[�������擾
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
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].pos[nCntParts].x,		// �e�p�[�c�̃��[�V�����̈ʒu���擾
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].pos[nCntParts].y,		// �e�p�[�c�̃��[�V�����̈ʒu���擾
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].pos[nCntParts].z);		// �e�p�[�c�̃��[�V�����̈ʒu���擾
											}
											else if (strcmp(acData, "ROT") == 0)
											{// ROT����������
												sscanf(&acLine[0],
													"%s %s %f %f %f",
													&acData[0],
													&acData[0],
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].rot[nCntParts].x,		// �e�p�[�c�̃��[�V�����̉�]���擾
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].rot[nCntParts].y,		// �e�p�[�c�̃��[�V�����̉�]���擾
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].rot[nCntParts].z);		// �e�p�[�c�̃��[�V�����̉�]���擾
											}

										}
										nCntParts++;			// �p�[�c�̃J�E���g��i�߂�
									}
								}
								nCntKey++;						// �L�[�t���[����i�߂�
							}
						}
						nCntMotion++;								// ���[�V������ς���
					}
				}
			}
			fclose(pFile);
		}
	}


	//-------------------------------------------------------------------------------
	// X�t�@�C���̓ǂݍ���
	//-------------------------------------------------------------------------------

	LPDIRECT3DDEVICE9 pDevice = GetDevice();					// �f�o�C�X�̎擾
	D3DXMATERIAL *pMat;											// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// �G�̎�ޕ����[�v
		for (int nCntModel = 0; nCntModel < g_EnemyInfo[nCntEnemyType].MaxModel; nCntModel++)
		{// �G�̃��f���̐������[�v
			D3DXLoadMeshFromX(g_EnemyInfo[nCntEnemyType].ModelAddress[g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModel].nIdxModel].cIP,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat,
				NULL,
				&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat,
				&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh);


			//-------------------------------------------------------------------------------
			// �e�N�X�`���̓ǂݍ���
			//-------------------------------------------------------------------------------

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat->GetBufferPointer();

			// �}�e���A�����e�ʂ𑝂₷
			g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat];

			for (int nCntMat = 0; nCntMat < (int)g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat; nCntMat++)
			{// �}�e���A���̐������[�v
				if (pMat[nCntMat].pTextureFilename != NULL)
				{// �e�N�X�`���̃A�h���X�����݂��Ă�����
					D3DXCreateTextureFromFile(pDevice,												// 3D�f�o�C�X�̃A�h���X
						pMat[nCntMat].pTextureFilename,												// �ǂݍ��ރe�N�X�`���̃t�@�C���̃A�h���X
						&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntMat]);		// �e�N�X�`���̃|�C���^�ɃA�h���X�ɐݒ肷��
				}
				else
				{// �e�N�X�`���̃A�h���X���Ȃ��Ȃ�
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntMat] = NULL;	// NULL�ɐݒ肷��
				}
			}
		}
	}

	//-------------------------------------------------------------------------------
	// ���f����Min&Max�̎擾
	//-------------------------------------------------------------------------------

	// �ϐ��錾
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// �G�̎�ޕ����[�v

		for (int nCntModel = 0; nCntModel < g_EnemyInfo[nCntEnemyType].MaxModel; nCntModel++)
		{// ���f���̐������[�v

			// ���_�����擾
			nNumVtx = g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g���擾
			sizeFVF = D3DXGetFVFVertexSize(g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			// �G�̍ŏ��l�ƍő�l���r
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// ���_�������[�v
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// ���_���W�̑��

																// �ŏ��l�Ɣ�r
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.x > vtx.x)
				{// �������Ă���ŏ��l��x�����������l��������
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.x = vtx.x;						// �ŏ��l��x����
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.y > vtx.y)
				{// �������Ă���ŏ��l��y�����������l��������
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.y = vtx.y;						// �ŏ��l��y����
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.z > vtx.z)
				{// �������Ă���ŏ��l��y�����������l��������
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.z = vtx.z;						// �ŏ��l��z����
				}

				// �ő�l�Ɣ�r
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.x < vtx.x)
				{// �������Ă���ő�l��x�����傫���l��������
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.x = vtx.x;						// �ő�l��x����
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.y < vtx.y)
				{// �������Ă���ő�l��x�����傫���l��������
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.y = vtx.y;						// �ő�l��y����
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.z < vtx.z)
				{// �������Ă���ő�l��x�����傫���l��������
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.z = vtx.z;						// �ő�l��z����
				}

				pVtxBuff += sizeFVF;							// �|�C���^���T�C�Y���i�߂�
			}

			// ���_�o�b�t�@���A�����b�N
			g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->UnlockVertexBuffer();
		}
	}

}

//=============================================================================
// �G�̔z�u���̓ǂݍ���
//=============================================================================
void EnemySetLode(void)
{
	// �ϐ��錾
	FILE *pFile = NULL;				// �t�@�C���̃|�C���^�ϐ�
	char acLine[MAX_CHAR];			// 1�s�ۂ��ƋL�����邽�߂̕ϐ�
	char acData[MAX_CHAR];			// �ꕔ�̕����L�����邽�߂̕ϐ�

	D3DXVECTOR3	pos;				// �ʒu������ϐ�
	D3DXVECTOR3 rot;				// ����������ϐ�
	int			nType;				// �G�̎�ނ�����ϐ�
	ENEMY_TYPE	EnemyType;			// �G�̎�ނ�����ϐ� 

	// �t�@�C�����J��
	if (GetMode() == MODE_GAME)
	{// �Q�[���̎��ɊJ���t�@�C��
		pFile = fopen("data/TEXT/Game_SetEnemy.txt", "r");
	}
	else if (GetMode() == MODE_TITLE)
	{// �^�C�g���̎��ɊJ���t�@�C��
		pFile = fopen("data/TEXT/Title_SetEnemy.txt", "r");
	}

	if (pFile != NULL)
	{// �k���`�F�b�N

		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{// �e�L�X�g�t�@�C���̈�ԉ��̍s������܂Ń��[�v
			sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[

			if (strcmp(acData, "ENEMYSET") == 0)
			{// KEYSET����������
				EnemyType = ENEMY_TYPE_ARMOR;				// �G�̃^�C�v��������

				while (strcmp(acData, "END_ENEMYSET") != 0)
				{// END_KEYSET������܂Ń��[�v
					fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
					sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

					if (strcmp(acData, "POS") == 0)
					{// POS����������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// �ʒu���擾
					}
					else if (strcmp(acData, "ROT") == 0)
					{// ROT����������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// �������擾
						rot.x = (D3DX_PI * 2 / 360 * rot.x);													// 3.14��2�{����360�x�Ŋ�����1�x���̒l���o���āA�e�L�X�g�̊p�x�ɂ���
						rot.y = (D3DX_PI * 2 / 360 * rot.y);													// 3.14��2�{����360�x�Ŋ�����1�x���̒l���o���āA�e�L�X�g�̊p�x�ɂ���
						rot.z = (D3DX_PI * 2 / 360 * rot.z);													// 3.14��2�{����360�x�Ŋ�����1�x���̒l���o���āA�e�L�X�g�̊p�x�ɂ���
					}
					else if (strcmp(acData, "TYPE") == 0)
					{// ROT����������
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nType);		// ��ނ��擾
						if (nType == 0)
						{// �^�C�v���Z��������
							EnemyType = ENEMY_TYPE_ARMOR;
						}
						else if (nType == 1)
						{// �^�C�v����ڂ�������
							EnemyType = ENEMY_TYPE_HITOME;
						}
						else if (nType == 2)
						{// �^�C�v���Ђ�����񂾂�����
							EnemyType = ENEMY_TYPE_HIGECHAN;
						}
						else if (nType == 3)
						{// �^�C�v���{�`������������
							EnemyType = ENEMY_TYPE_HONCHAN;
						}

					}

				}

				// �G�̐ݒu
				SetEnemy(pos, rot, EnemyType, false);

			}
		}
		fclose(pFile);
	}

}

//=============================================================================
// �G���̎擾
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// �G�̐l�����擾
//=============================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}