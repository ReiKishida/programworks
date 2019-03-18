//=============================================================================
//
// �e���ˏ��� [Bullet.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "camera.h"
#include "model.h"
#include "effect.h"
#include "bullet.h"
#include "particle.h"
#include "enemy.h"
#include "score.h"
#include "player.h"
#include "life.h"
#include "gate.h"
#include "modelwall.h"
#include "canon.h"
#include "hit.h"
#include "damage.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//�X�e�[�W�̌��E�͈�
#define STAGE_LIMIT_MAX_X (1450.0f)
#define STAGE_LIMIT_MIN_X (-40.0f)
#define STAGE_LIMIT_MAX_Z (30.0f)
#define STAGE_LIMIT_MIN_Z (-3800.0f)
#define STAGE_LIMIT_MAX_Y (1000.0f)
#define STAGE_LIMIT_MIN_Y (0.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);	//���_��񐶐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^

BULLET g_aBullet[MAX_BULLET];//�e���

int g_nIndexBulletShadow[MAX_BULLET];	//�e�̔ԍ�


//=============================================================================
// ����������
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾


	MakeVertexBullet(pDevice);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
		g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��邩
		g_aBullet[nCntBullet].type = BULLETTYPE_NONE;//�e�̏��
		g_aBullet[nCntBullet].nLife = 0;//���܂̏��
		g_aBullet[nCntBullet].fRadius = 0.0f;//�e�̔��a
	}



}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	//�J�����̎擾
	CAMERA *pCamera;
	pCamera = GetCamera();

	//�G�̎擾
	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	//���f���̎擾
	MODEL *pModel;
	pModel = GetModel();

	//�v���C���[�̎擾
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	//�Q�[�g�̎擾
	GATE *pGate;
	pGate = GetGate();

	//�ǂ̎擾
	WALL *pWall;
	pWall = GetModelWall();

	//�L���m���C�̎擾
	CANON *pCanon;
	pCanon = GetCanon();

	//�G�t�F�N�g�̈ʒu
	D3DXVECTOR3 EffectPos;


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă�����

			g_aBullet[nCntBullet].posold = g_aBullet[nCntBullet].pos;
			EffectPos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);


			//�e�̎��������炵�Ă���
			g_aBullet[nCntBullet].nLife--;


			//////////////////////////////////////////////////////////////////////
			/////////////�v���C���[�̒e//////////////////////////////////////////
			////////////////////////////////////////////////////////////////////
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				SetEffect(EffectPos, 5.0f, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f));
				g_aBullet[nCntBullet].pos.x -= sinf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += 0.1f *  g_aBullet[nCntBullet].move.y;
				g_aBullet[nCntBullet].pos.z -= cosf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.z;
				SetPositionShadow(g_nIndexBulletShadow[nCntBullet], D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z));//�e�̈ʒu

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă�����
						if (pEnemy->pos.x + pEnemy->aModel[0].VtxMinModel.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pEnemy->pos.x + pEnemy->aModel[0].VtxMaxModel.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius

							&&pEnemy->pos.y - pEnemy->aModel[0].VtxMinModel.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pEnemy->pos.y + pEnemy->aModel[0].VtxMaxModel.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius

							&&pEnemy->pos.z + pEnemy->aModel[0].VtxMinModel.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pEnemy->pos.z + pEnemy->aModel[0].VtxMaxModel.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{

							//�G�ɓ�������
							HitEnemy(-1, nCntEnemy);

							//�q�b�g�R�[���̕\��
							HitCall();

							//�X�R�A���Z
							AddScore(500);

							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//���e�_�Ƀp�[�e�B�N������
								SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							//�e���폜���A�e������
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);

						}
					}
				}


				for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++, pGate++)
				{
					if (pGate->bUse == true)
					{//�Q�[�g���g�p����Ă���Ƃ�
						if (pGate->pos.x + pGate->VtxMinGate.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.x + pGate->VtxMaxGate.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y + pGate->VtxMinGate.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y + pGate->VtxMaxGate.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMinGate.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMaxGate.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//���e�_�Ƀp�[�e�B�N������
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							//�Q�[�g�ɓ�������
							HitGate(-1, nCntGate);
							//�q�b�g�R�[�����o��
							HitCall();

							//�e���폜���A�e������
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}
				for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++, pCanon++)
				{
					if (pCanon->bUse == true)
					{
						if (pCanon->pos.x + pCanon->VtxMinCanon.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.x + pCanon->VtxMaxCanon.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.y + pCanon->VtxMinCanon.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.y + pCanon->VtxMaxCanon.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.z + pCanon->VtxMinCanon.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.z + pCanon->VtxMaxCanon.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//���e�_�Ƀp�[�e�B�N������
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							//�L���m���Ƀ_���[�W���Z
							HitCanon(-1, nCntCanon);
							//�X�R�A���Z
							AddScore(300);
							//�q�b�g�R�[��
							HitCall();

							//�e���폜���A�e������
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}

			}



			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//�G�̒e�̏ꍇ
				SetPositionShadow(g_nIndexBulletShadow[nCntBullet], D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z));//�e�̈ʒu
				SetEffect(EffectPos, 5.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));


				g_aBullet[nCntBullet].pos.x -= sinf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				g_aBullet[nCntBullet].pos.z -= cosf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.z;

				if (pPlayer.pos.x + pPlayer.aModel[0].VtxMinModel.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
					&&pPlayer.pos.x + pPlayer.aModel[0].VtxMaxModel.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius

					&&pPlayer.pos.y - pPlayer.aModel[0].VtxMinModel.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
					&&pPlayer.pos.y + pPlayer.aModel[0].VtxMaxModel.y <= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius



					&&pPlayer.pos.z + pPlayer.aModel[0].VtxMinModel.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
					&&pPlayer.pos.z + pPlayer.aModel[0].VtxMaxModel.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
					)
				{
					//�v���C���[�Ƀ_���[�W���Z
					HitPlayer(-5);
					//�Q�[�W�����炷
					HitDamage(-10);
					//�_���[�W�R�[��
					HitDamage();

					for (int nCntPerticle = 0; nCntPerticle < 12; nCntPerticle++)
					{//���e�_�Ƀp�[�e�B�N������
						SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
					}

					//�e���폜���A�e������
					g_aBullet[nCntBullet].bUse = false;
					DeleteShadow(g_nIndexBulletShadow[nCntBullet]);


				}


				for (int nCntWall = 0; nCntWall < MAX_GATE; nCntWall++, pGate++)
				{
					if (pWall->bUse == true)
					{//�ǂ��g�p����Ă���Ƃ�
						if (pGate->pos.x + pGate->VtxMinGate.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.x + pGate->VtxMaxGate.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y - pGate->VtxMinGate.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y + pGate->VtxMaxGate.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMinGate.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMaxGate.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//���e�_�Ƀp�[�e�B�N������
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}

							//�e���폜���A�e������
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}
			}



			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER || g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				if (g_aBullet[nCntBullet].pos.x > STAGE_LIMIT_MAX_X || g_aBullet[nCntBullet].pos.x < STAGE_LIMIT_MIN_X
					|| g_aBullet[nCntBullet].pos.y > STAGE_LIMIT_MAX_Y || g_aBullet[nCntBullet].pos.y < STAGE_LIMIT_MIN_Y
					|| g_aBullet[nCntBullet].pos.z > STAGE_LIMIT_MAX_Z || g_aBullet[nCntBullet].pos.z < STAGE_LIMIT_MIN_Z
					)
				{
					g_aBullet[nCntBullet].bUse = false;
					DeleteShadow(g_nIndexBulletShadow[nCntBullet]);


					for (int nCntPerticle = 0; nCntPerticle < 5; nCntPerticle++)
					{//���e�_�Ƀp�[�e�B�N������
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.5f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);

					}
				}

				if (g_aBullet[nCntBullet].nLife <= 0)
				{//�e�̎������Ȃ��Ȃ����Ƃ�
					//�e���폜���A�e������
					g_aBullet[nCntBullet].bUse = false;
					DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
					for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
					{//���e�_�Ƀp�[�e�B�N������
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.5f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);

					}
				}



				for (int nCnModeltWall = 0; nCnModeltWall < MAX_MODELWALL; nCnModeltWall++, pWall++)
				{
					if (pWall->bUse == true)
					{//�ǂ��g�p����Ă�����
						if (pWall->pos.x + pWall->VtxMinModelWall.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pWall->pos.x + pWall->VtxMaxModelWall.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pWall->pos.y - pWall->VtxMinModelWall.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pWall->pos.y + pWall->VtxMaxModelWall.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pWall->pos.z + pWall->VtxMinModelWall.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pWall->pos.z + pWall->VtxMaxModelWall.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{//�ǂ͈̔͂ɓ�������
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//���e�_�Ƀp�[�e�B�N������
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}


							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}

				for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
				{
					if (pModel->bUse == true)
					{//���f�����g�p����Ă�����
						if (pModel->pos.x + pModel->VtxMinModel.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pModel->pos.x + pModel->VtxMaxModel.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pModel->pos.y - pModel->VtxMinModel.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pModel->pos.y + pModel->VtxMaxModel.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pModel->pos.z + pModel->VtxMinModel.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pModel->pos.z + pModel->VtxMaxModel.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{//���f���͈͓̔��Ȃ�
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//���e�_�Ƀp�[�e�B�N������
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}
			}//�v���C���[�̒e�������͓G�̒e
		}//�e���g�p���ꂢ��
	}//for��(�e�S��)

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxrot, mtxtrans;

	//�J�����̎擾
	CAMERA *pCamera;
	pCamera = GetCamera();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);
			pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

			g_aBullet[nCntBullet].mtxWorld._11 = pCamera->mtxView._11;
			g_aBullet[nCntBullet].mtxWorld._12 = pCamera->mtxView._21;
			g_aBullet[nCntBullet].mtxWorld._13 = pCamera->mtxView._31;
			g_aBullet[nCntBullet].mtxWorld._21 = pCamera->mtxView._12;
			g_aBullet[nCntBullet].mtxWorld._22 = pCamera->mtxView._22;
			g_aBullet[nCntBullet].mtxWorld._23 = pCamera->mtxView._32;
			g_aBullet[nCntBullet].mtxWorld._31 = pCamera->mtxView._13;
			g_aBullet[nCntBullet].mtxWorld._32 = pCamera->mtxView._23;
			g_aBullet[nCntBullet].mtxWorld._33 = pCamera->mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxtrans,
				g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxtrans);


			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, g_pTextureBullet);


			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}


	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);


	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fRadius, -g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fRadius, -g_aBullet[nCntBullet].fRadius, 0.0f);

		//�@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, float fRadius, BULLETTYPE type)
{
	// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		//���_�o�b�t�@�����b�N
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
		{
			if (g_aBullet[nCntBullet].bUse == false)
			{

				g_aBullet[nCntBullet].pos = pos;	//�ʒu���
				g_aBullet[nCntBullet].move = move;	//�ړ���
				g_aBullet[nCntBullet].rot = rot;	//�p�x
				g_aBullet[nCntBullet].nLife = nLife;//�e�̎���
				g_aBullet[nCntBullet].type = type;//�e�̎��
				g_aBullet[nCntBullet].fRadius = fRadius;//���a

				//�e�̐���
				g_nIndexBulletShadow[nCntBullet] = SetShadow(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					g_aBullet[nCntBullet].fRadius,
					g_aBullet[nCntBullet].fRadius);

				//�g�p����
				g_aBullet[nCntBullet].bUse = true;
				break;

			}
			pVtx += 4;
 		}
		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffBullet->Unlock();
}


////=============================================================================
//// �e�̎擾
////=============================================================================
BULLET *GetBullet(void)
{

	return &g_aBullet[0];

}



bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bHit = false;
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{


		if(pPos->x + VtxMin->x < g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
		 &&pPos->x + VtxMax->x > g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
		 &&pPos->z + VtxMin->z < g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
		 &&pPos->z + VtxMax->z > g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
			)
		{
			bHit = true;
		}
	}
	return bHit;
}
