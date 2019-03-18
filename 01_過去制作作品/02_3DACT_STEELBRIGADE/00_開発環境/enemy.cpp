//=============================================================================
//
// �G���� [enemy.cpp]
// Author :
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "model.h"
#include "game.h"
#include "fade.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "boost.h"
#include "enemytext.h"
#include "enemymotion.h"
#include "smoke.h"
#include "modelwall.h"
#include "gate.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define JUMP_POWER (14.50f)	//�W�����v��
#define GRAVITY_E (0.8f)		//�d��
#define GRAVITY_E_MAX (150.0f)	//�d�͂̍ő��

#define DASHSPEED  (7.0f)	//�_�b�V���X�s�[�h
#define ROT_SPEED (0.05f)	//���񑬓x
#define ROT_SPEED2 (0.2f)	//���񑬓x

//�X�e�[�W�̌��E�l
#define STAGE_LIMIT_MAX_X (1450.0f)
#define STAGE_LIMIT_MIN_X (-40.0f)
#define STAGE_LIMIT_MAX_Z (30.0f)
#define STAGE_LIMIT_MIN_Z (-3800.0f)
//


#define EFFECT_MAX (50)//�G�t�F�N�g�̗�

#define MOVE_BULLET_Y (25.0f)	//�e��
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY g_Enemy[MAX_ENEMY];	//�G�̏��
ENEMYSTATUS g_EnemyStatus = ENEMYSTATUS_NEUTRAL;	//�G�̏��
ENEMYSTATUS g_EnemyStatusNow = ENEMYSTATUS_NEUTRAL;//�G�̍��̏��

//STAGE g_aLimit[4];

float fJump_E;	//�W�����v��
float fGRAVITY_E_E;	//�d��

int g_nCntSound_E = 0;
int g_nCntEnemy ;

bool bLand_E; //���n����
//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	//�X���[�N������
	InitSmoke();

	EnemyText *pEnemyText;
	pEnemyText = GetEnemyText();
	g_nCntEnemy = 3;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCntIdx = 0; nCntIdx < pEnemyText->MAX_TEXTPARTS; nCntIdx++)
		{//�e��l�̑��
		 //�ǂݍ��ރ��f����
			g_Enemy[nCntEnemy].aModel[nCntIdx].FileName[0] = pEnemyText->aModelTex[nCntIdx].FileName[0];

			//�e�q�֌W
			g_Enemy[nCntEnemy].aModel[nCntIdx].nIdxModelParent = pEnemyText->aModelTex[nCntIdx].Index;

			//�ʒu
			g_Enemy[nCntEnemy].aModel[nCntIdx].pos = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].ModelPosX, pEnemyText->aModelTex[nCntIdx].ModelPosY, pEnemyText->aModelTex[nCntIdx].ModelPosZ);

			//��]
			g_Enemy[nCntEnemy].aModel[nCntIdx].rot = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].ModelRotX,
				pEnemyText->aModelTex[nCntIdx].ModelRotY,
				pEnemyText->aModelTex[nCntIdx].ModelRotZ);

			//���f���̐ݒ�
			D3DXLoadMeshFromX(&pEnemyText->aModelTex[nCntIdx].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Enemy[nCntEnemy].aModel[nCntIdx].pBuffMat,
				NULL,
				&g_Enemy[nCntEnemy].aModel[nCntIdx].nNumMat,
				&g_Enemy[nCntEnemy].aModel[nCntIdx].pMesh);
		}
		//�W�����v�̏��
		g_Enemy[nCntEnemy].bUseJump = false;
		//�v���C���[�̏��
		g_Enemy[nCntEnemy].nCoolDown = 0;

		//���n���Ă��邩
		bLand_E = false;

		//�v���C���[���g�p����Ă��邩
		g_Enemy[nCntEnemy].bUse = false;



	}









	fJump_E = JUMP_POWER;
	fGRAVITY_E_E = GRAVITY_E;


	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_�����擾
		nNumVtx = g_Enemy[nCntEnemy].aModel[0].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_Enemy[nCntEnemy].aModel[0].pMesh->GetFVF());

		//���_�o�b�t�@�����b�N
		g_Enemy[nCntEnemy].aModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W

			if (Vtx.x < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.x)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.x = Vtx.x + -20.0f;
			}

			if (Vtx.y < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y = Vtx.y + -50.0f;
			}

			if (Vtx.z < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z = Vtx.x + -20.0f;
			}


			if (Vtx.x > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x = Vtx.x + 20.0f;
			}

			if (Vtx.y > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y = Vtx.y + 50.0f;
			}

			if (Vtx.z > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.z)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.z = Vtx.z + 20.0f;
			}
			pVtxBuff += sizeFVF;
		}

		g_Enemy[nCntEnemy].VtxMinEnemy.x = g_Enemy[nCntEnemy].aModel[0].VtxMinModel.x;
		g_Enemy[nCntEnemy].VtxMaxEnemy.x = g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x;

		g_Enemy[nCntEnemy].VtxMinEnemy.y = g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y;
		g_Enemy[nCntEnemy].VtxMaxEnemy.y = g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y;

		g_Enemy[nCntEnemy].VtxMinEnemy.z = g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z;
		g_Enemy[nCntEnemy].VtxMaxEnemy.z = g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.z;

		g_Enemy[nCntEnemy].fAngle = 0.0f;			//����

	}

	CAMERA *pCamera;
	pCamera = GetCamera();

	//�v���C���[�̃��C�t
	//g_Enemy[0].nLife = 5;
	//g_Enemy[1].nLife = 5;
	//g_Enemy[2].nLife = 3;
	////g_Enemy[3].nLife = 3;

	//// �ʒu�E�����̏����ݒ�
	//g_Enemy[0].pos = D3DXVECTOR3(750.0f, 0.0f, -1400.0f);
	//g_Enemy[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//g_Enemy[1].pos = D3DXVECTOR3(750.0f, 0.0f, -500.0f);
	//g_Enemy[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//g_Enemy[2].pos = D3DXVECTOR3(750.0f, 0.0f, -2600.0f);
	//g_Enemy[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	/*g_Enemy[3].pos = D3DXVECTOR3(1150.0f, 0.0f, -2500.0f);
	g_Enemy[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/

}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	//�X���[�N�I��
	UninitSmoke();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ���b�V���̊J��
		if (g_Enemy[nCntEnemy].aModel[0].pMesh != NULL)
		{
			g_Enemy[nCntEnemy].aModel[0].pMesh->Release();
			g_Enemy[nCntEnemy].aModel[0].pMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_Enemy[nCntEnemy].aModel[0].pBuffMat != NULL)
		{
			g_Enemy[nCntEnemy].aModel[0].pBuffMat->Release();
			g_Enemy[nCntEnemy].aModel[0].pBuffMat = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
#if 1
	EnemyMotion *pMotion;
	pMotion = GetEnemyMotion();

	EnemyText *pEnemyText;
	pEnemyText = GetEnemyText();




	//nCntUpdateRogic++;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_Enemy[nCntEnemy].posold = g_Enemy[nCntEnemy].pos;
		SetPositionShadow(g_Enemy[nCntEnemy].nIndexShadow, g_Enemy[nCntEnemy].pos);//�e�̈ʒu

		CAMERA pCamera;
		pCamera = *GetCamera();
		PLAYER *pPlayer;
		//�v���C���[�̎擾
		pPlayer = GetPlayer();
		D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
		BulletMove.x = -25.0f;
		BulletMove.z = -25.0f;

		D3DXVECTOR3 BulletMove2(0.0f, 0.0f, 0.0f);
		BulletMove2.x = -7.0f;
		BulletMove2.z = -7.0f;

		D3DXVECTOR3 VecP;
		D3DXVECTOR3 VecLength;

		g_Enemy[nCntEnemy].Rmove = pPlayer->move *-1;

		VecP.x = pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x;
		VecP.y = pPlayer->pos.y - g_Enemy[nCntEnemy].pos.y + 30.0f;
		VecP.z = pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z;
		D3DXVec3Normalize(&VecP, &VecP);

		VecLength = pPlayer->pos - g_Enemy[nCntEnemy].pos;



	/*	if (g_EnemyStatusNow == ENEMYSTATUS_NEUTRAL)
		{

			g_EnemyStatus = ENEMYSTATUS_MOVE;
		}*/


		if (VecP.y < 0.0)
		{
			BulletMove.y = VecP.y;
		}
		else
		{
			BulletMove.y = VecP.y * MOVE_BULLET_Y;

		}



		if (g_Enemy[nCntEnemy].bUse == true)
		{
			if (g_Enemy[nCntEnemy].nLife <= 1)
			{
				for (int nCnt = 0; nCnt < 50; nCnt++)
				{
					SetSmoke(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 27.0f, g_Enemy[nCntEnemy].pos.z), 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				if (rand() % 20 == 0)
				{
					SetParticle(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 27.0f, g_Enemy[nCntEnemy].pos.z), 30, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
				}
			}

			static float fAngle;
			fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z);



			g_Enemy[nCntEnemy].fLength = sqrtf((VecLength.x / 2 + VecLength.x / 2) * (VecLength.z / 2 + VecLength.z / 2));

			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}

			if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}


			g_Enemy[nCntEnemy].rot.y = fAngle;

			if (rand() % 90 == 0)
			{
				g_Enemy[nCntEnemy].bUseQick = true;
				if (g_Enemy[nCntEnemy].bUseQick == true)
				{
					g_Enemy[nCntEnemy].move.x -= sinf((D3DX_PI *  fAngle)) * DASHSPEED;
					g_Enemy[nCntEnemy].move.z -= cosf((D3DX_PI *  fAngle)) * DASHSPEED;
					g_Enemy[nCntEnemy].bUseQick = false;
				}
			}
			if (rand() % 50 == 0)
			{
				if (g_Enemy[nCntEnemy].fLength <= 200.0f)
				{

					g_Enemy[nCntEnemy].move.x = g_Enemy[nCntEnemy].Rmove.x;
					g_Enemy[nCntEnemy].move.y = g_Enemy[nCntEnemy].move.y;
					g_Enemy[nCntEnemy].move.y = g_Enemy[nCntEnemy].Rmove.z;

				}
				else
				{
					g_Enemy[nCntEnemy].move.x = fAngle * 1.5f;
					g_Enemy[nCntEnemy].move.z = fAngle * 0.1f;
				}
			}

			if (rand() % 100 == 0)
			{//�W�����v
				if (g_Enemy[nCntEnemy].bUseJump == false)
				{
					g_Enemy[nCntEnemy].bUseJump = true;
					if (g_Enemy[nCntEnemy].bUseJump == true)
					{
						g_Enemy[nCntEnemy].move.y += 10.0f;
					}
				}
			}
			if (g_Enemy[nCntEnemy].fLength <= 480.0f)
			{
				if (rand() % 30 == 0)
				{
					SetBullet(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 18.0f, g_Enemy[nCntEnemy].pos.z), BulletMove, D3DXVECTOR3(g_Enemy[nCntEnemy].rot.x, fAngle, g_Enemy[nCntEnemy].rot.z), 40, 5.0f, BULLETTYPE_ENEMY);
				}
			}
			g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

			g_Enemy[nCntEnemy].move.y -= GRAVITY_E;
			if (g_Enemy[nCntEnemy].move.y > GRAVITY_E_MAX)
			{
				g_Enemy[nCntEnemy].move.y = GRAVITY_E_MAX;
			}
			if (g_Enemy[nCntEnemy].pos.y <= 0.0)
			{

				g_Enemy[nCntEnemy].pos.y = 0.0f;
				g_Enemy[nCntEnemy].move.y = 0.0f;
				g_Enemy[nCntEnemy].bUseJump = false;

			}
			else
			{
				g_Enemy[nCntEnemy].bUseJump = true;

			}

			if (g_Enemy[nCntEnemy].pos.x > STAGE_LIMIT_MAX_X)
			{
				g_Enemy[nCntEnemy].pos.x = STAGE_LIMIT_MAX_X;
			}
			if (g_Enemy[nCntEnemy].pos.x < STAGE_LIMIT_MIN_X)
			{
				g_Enemy[nCntEnemy].pos.x = STAGE_LIMIT_MIN_X;
			}
			if (g_Enemy[nCntEnemy].pos.z > STAGE_LIMIT_MAX_Z)
			{
				g_Enemy[nCntEnemy].pos.z = STAGE_LIMIT_MAX_Z;
			}

			if (g_Enemy[nCntEnemy].pos.z < STAGE_LIMIT_MIN_Z)
			{
				g_Enemy[nCntEnemy].pos.z = STAGE_LIMIT_MIN_Z;
			}

			if (CollisionModel(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].VtxMaxEnemy, &g_Enemy[nCntEnemy].VtxMaxEnemy) == true)
			{
				g_Enemy[nCntEnemy].bUseJump = false;
				g_Enemy[nCntEnemy].move.x *= -1;
				g_Enemy[nCntEnemy].move.z *= -1;

			}
		}



		CollisionModelWall(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].VtxMaxEnemy, &g_Enemy[nCntEnemy].VtxMinEnemy);
		CollisionGate(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].VtxMaxEnemy, &g_Enemy[nCntEnemy].VtxMinEnemy);

	}

#endif
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxParent2;

	//========================//
	//�v���C���[�̃C���f�b�N�X//
	//========================//
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);

		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxrot);


		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxtrans,
			g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxtrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorld);


		for (int nCntModelParts = 0; nCntModelParts < MAX_PARTS; nCntModelParts++)
		{
			if (g_Enemy[nCntEnemy].bUse == true)
			{

				if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == -1)
				{//�v���C���[�̃}�g���b�N�X��
					mtxParent = g_Enemy[nCntEnemy].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 0)
				{//�̂̃}�g���b�N�X��
					mtxParent = g_Enemy[nCntEnemy].aModel[0].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 1)
				{//���P��
					mtxParent = g_Enemy[nCntEnemy].aModel[1].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 2)
				{//���Q��
					mtxParent = g_Enemy[nCntEnemy].aModel[2].mtxWorld;
				}
				else
				{//���̑�
					mtxParent = g_Enemy[nCntEnemy].aModel[nCntModelParts - 1].mtxWorld;
				}
				//=======================================================================//
				//						    �e���f���̃C���f�b�N�X						//
				//=====================================================================//
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_Enemy[nCntEnemy].aModel[nCntModelParts].rot.y, g_Enemy[nCntEnemy].aModel[nCntModelParts].rot.x, g_Enemy[nCntEnemy].aModel[nCntModelParts].rot.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxrot);


				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxtrans,
					g_Enemy[nCntEnemy].aModel[nCntModelParts].pos.x, g_Enemy[nCntEnemy].aModel[nCntModelParts].pos.y, g_Enemy[nCntEnemy].aModel[nCntModelParts].pos.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxtrans);

				//�e�̃}�g���b�N�X���f
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxParent);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld);


				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].aModel[nCntModelParts].pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].aModel[nCntModelParts].nNumMat; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//pDevice->SetTexture(0, g_Enemy[nCntEnemy].aModel[0].pEnemyTexture);
					pDevice->SetTexture(0, 0);
					// ���f��(�p�[�c)�̕`��
					g_Enemy[nCntEnemy].aModel[nCntModelParts].pMesh->DrawSubset(nCntMat);
				}
				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}
	//=============================================================================
	// �q�b�g����
	//=============================================================================
	void HitEnemy(int nDamage, int nCntEnemy)
	{
		GAMESTATE *pGame;
		pGame = GetGameState();

			if (g_Enemy[nCntEnemy].bUse == true)
			{
				g_Enemy[nCntEnemy].nLife += nDamage;

				if (g_Enemy[nCntEnemy].nLife <= 0)
				{
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

					g_Enemy[nCntEnemy].bUse = false;
					AddScore(25000);
					DeleteShadow(g_Enemy[nCntEnemy].nIndexShadow);
					for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
					{
						SetParticle(g_Enemy[nCntEnemy].pos, 60, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
						SetParticle(g_Enemy[nCntEnemy].pos, 60, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);

					}

				}

			}
	}

	//=============================================================================
	// �����蔻�菈��
	//=============================================================================
	bool CollisionEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosold, D3DXVECTOR3 * VtxMax, D3DXVECTOR3 * VtxMin)
	{
		bool bCollision = false; //���n������

		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_Enemy[nCntEnemy].bUse == true)
			{
				if (pPos->x + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x && pPos->x - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x
					&&pPos->y + VtxMax->y >= g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y && pPos->y - VtxMax->y <= g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y
					&&pPos->z + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z && pPos->z - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z)
				{
					if (pPosold->x + VtxMax->z <= g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x  && pPos->x + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x)
					{//�����炫���Ƃ�
						pPos->x = g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x - VtxMax->z;
						bCollision = true;
					}

					else if (pPosold->x - VtxMax->z >= g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x  && pPos->x - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x)
					{//�E���炫���Ƃ�
						pPos->x = g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x + VtxMax->z;
						bCollision = true;

					}
					else if (pPosold->z + VtxMax->z <= g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z && pPos->z + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z)
					{//��O���痈����
						pPos->z = g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z - VtxMax->z;
						bCollision = true;

					}
					else if (pPosold->z - VtxMax->z >= g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z && pPos->z - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z)
					{//�����痈����
						pPos->z = g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z + VtxMax->z;
						bCollision = true;

					}

					if (pPosold->y + VtxMax->y <= g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y && pPos->y + VtxMax->y >= g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y)
					{//�ォ�痈����
						pPos->y = g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y + VtxMin->y;
						bCollision = true;

					}
					if (pPosold->y - VtxMax->y >= g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y && pPos->y - VtxMax->y <= g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y)
					{//�����痈����
						pPos->y = g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y + VtxMax->y;
						bCollision = true;

					}
				}

			}
		}
		return bCollision;
	}

	//////////////////////////////////////////////////////////////////////////
	//////				�G�̐ݒu								  ////////////
	//////////////////////////////////////////////////////////////////////////
	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_Enemy[nCntEnemy].bUse == false)
			{
				g_Enemy[nCntEnemy].pos = pos;
				g_Enemy[nCntEnemy].rot = rot;
				g_Enemy[nCntEnemy].nLife = nLife;
				g_Enemy[nCntEnemy].bUse = true;

				g_Enemy[nCntEnemy].nIndexShadow = SetShadow(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].rot, 28.0f, 28.0f);

				break;
			}
		}
	}

ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}

ENEMYSTATUS *GetEnemyStatus(void)
{
	return &g_Enemy[0].status;
}


