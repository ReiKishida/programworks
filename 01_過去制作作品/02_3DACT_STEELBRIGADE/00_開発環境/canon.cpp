//=============================================================================
//
// �C�䏈�� [canon.cpp]
// Author :Kishida Rei
//
//=============================================================================
#include "canon.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "sound.h"
#include "game.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//�X�e�[�W���E�l
#define STAGE_LIMIT_MAX_X (150.0f)
#define STAGE_LIMIT_MIN_X (-50.0f)
#define STAGE_LIMIT_MAX_Z (50.0f)
#define STAGE_LIMIT_MIN_Z (-150.0f)


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


CANON g_aCanon[MAX_CANON];//�C��
//=============================================================================
// ����������
//=============================================================================
void InitCanon(void)
{


	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++)
	{

		g_aCanon[nCntCanon].bUse = false;


		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/gato.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aCanon[nCntCanon].aCanonModel[0].pBuffMat,
			NULL,
			&g_aCanon[nCntCanon].aCanonModel[0].nNumMat,
			&g_aCanon[nCntCanon].aCanonModel[0].pMesh);

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/canon.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aCanon[nCntCanon].aCanonModel[1].pBuffMat,
			NULL,
			&g_aCanon[nCntCanon].aCanonModel[1].nNumMat,
			&g_aCanon[nCntCanon].aCanonModel[1].pMesh);


	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitCanon(void)
{
	for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++)
	{
		for (int nCntTypeCanon = 0; nCntTypeCanon < MAXTYPE_CANON; nCntTypeCanon++)
		{
			//�e�N�X�`���̔j��
			if (g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pTexture != NULL)
			{
				g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pTexture->Release();
				g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pTexture = NULL;
			}

			// ���b�V���̊J��
			if (g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pMesh != NULL)
			{
				g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pMesh->Release();
				g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pMesh = NULL;
			}

			// �}�e���A���̊J��
			if (g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pBuffMat != NULL)
			{
				g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pBuffMat->Release();
				g_aCanon[nCntCanon].aCanonModel[nCntTypeCanon].pBuffMat = NULL;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCanon(void)
{
	//�J�����̎擾
	CAMERA pCamera;
	pCamera = *GetCamera();

	//�v���C���[�̎擾
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	static float fAngle;
	D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
	BulletMove.x = -28.0f;
	BulletMove.z = -28.0f;

	for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++)
	{
		D3DXVECTOR3 VecP;
		VecP.x = pPlayer->pos.x - g_aCanon[nCntCanon].pos.x;
		VecP.y = pPlayer->pos.y - g_aCanon[nCntCanon].pos.y - 10.0f;
		VecP.z = pPlayer->pos.z - g_aCanon[nCntCanon].pos.z;

		if (g_aCanon[nCntCanon].bUse == true)
		{
			if (g_aCanon[nCntCanon].nLife <= 2)
			{//���C�t��2�ȉ��̂Ƃ�
				for (int nCnt = 0; nCnt < 50; nCnt++)
				{//���𔭐�
					SetSmoke(D3DXVECTOR3(g_aCanon[nCntCanon].pos.x, g_aCanon[nCntCanon].pos.y + 27.0f, g_aCanon[nCntCanon].pos.z), 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				if (rand() % 20 == 0)
				{//�m���Ńp�[�e�B�N���𔭐�
					SetParticle(D3DXVECTOR3(g_aCanon[nCntCanon].pos.x, g_aCanon[nCntCanon].pos.y + 27.0f, g_aCanon[nCntCanon].pos.z), 30, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
				}
			}
			switch (g_aCanon[nCntCanon].type)
			{
			case CANONTYPE_G://�K�g�����O�C�̂Ƃ�
				fAngle = atan2f(pPlayer->pos.x - g_aCanon[nCntCanon].pos.x, pPlayer->pos.z - g_aCanon[nCntCanon].pos.z);
				//������������
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}

				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				if (rand() % 120 == 0)
				{//�m���Œe����
					SetBullet(D3DXVECTOR3(g_aCanon[nCntCanon].pos.x, g_aCanon[nCntCanon].pos.y + 1.0f, g_aCanon[nCntCanon].pos.z), BulletMove, D3DXVECTOR3(g_aCanon[nCntCanon].rot.x, fAngle, g_aCanon[nCntCanon].rot.z), 30, 5.0f, BULLETTYPE_ENEMY);
				}

				g_aCanon[nCntCanon].rot.y = fAngle;
				break;

			case CANONTYPE_C://�L���m���C�̂Ƃ�
			fAngle = atan2f(pPlayer->pos.x - g_aCanon[nCntCanon].pos.x, pPlayer->pos.z - g_aCanon[nCntCanon].pos.z);
			//������������
			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}

			if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}
			if (rand() % 90 == 0)
			{//�m���Œe����
				SetBullet(D3DXVECTOR3(g_aCanon[nCntCanon].pos.x, g_aCanon[nCntCanon].pos.y + 1.0f, g_aCanon[nCntCanon].pos.z), BulletMove, D3DXVECTOR3(g_aCanon[nCntCanon].rot.x, fAngle, g_aCanon[nCntCanon].rot.z), 30, 5.0f, BULLETTYPE_ENEMY);
			}

			g_aCanon[nCntCanon].rot.y = fAngle;
			break;
			}//switch
		} //�g�p����Ă���Ȃ�
	}//for


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCanon(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^


	for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++)
	{
		for (int nCntTypeCanon = 0; nCntTypeCanon < MAXTYPE_CANON; nCntTypeCanon++)
		{
			if (g_aCanon[nCntCanon].bUse == true)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aCanon[nCntCanon].mtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_aCanon[nCntCanon].rot.y, g_aCanon[nCntCanon].rot.x, g_aCanon[nCntCanon].rot.z);

				D3DXMatrixMultiply(&g_aCanon[nCntCanon].mtxWorld, &g_aCanon[nCntCanon].mtxWorld, &mtxrot);


				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxtrans,
					g_aCanon[nCntCanon].pos.x, g_aCanon[nCntCanon].pos.y, g_aCanon[nCntCanon].pos.z);

				D3DXMatrixMultiply(&g_aCanon[nCntCanon].mtxWorld, &g_aCanon[nCntCanon].mtxWorld, &mtxtrans);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aCanon[nCntCanon].mtxWorld);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aCanon[nCntCanon].aCanonModel[g_aCanon[nCntCanon].nType].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aCanon[nCntCanon].aCanonModel[g_aCanon[nCntCanon].nType].nNumMat; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					pDevice->SetTexture(0, 0);

					// ���f��(�p�[�c)�̕`��
					g_aCanon[nCntCanon].aCanonModel[g_aCanon[nCntCanon].nType].pMesh->DrawSubset(nCntMat);
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}


//=============================================================================
// �C��Ƃ̓����蔻��
//=============================================================================
bool CollisionCanon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //���n������

	for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++)
	{
		if (g_aCanon[nCntCanon].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aCanon[nCntCanon].VtxMinCanon.x + g_aCanon[nCntCanon].pos.x && pPos->x - VtxMax->z <= g_aCanon[nCntCanon].VtxMaxCanon.x + g_aCanon[nCntCanon].pos.x
				&&pPos->y + VtxMax->y >= g_aCanon[nCntCanon].VtxMinCanon.y + g_aCanon[nCntCanon].pos.y && pPos->y - VtxMax->y <= g_aCanon[nCntCanon].VtxMaxCanon.y + g_aCanon[nCntCanon].pos.y
				&&pPos->z + VtxMax->z >= g_aCanon[nCntCanon].VtxMinCanon.z + g_aCanon[nCntCanon].pos.z && pPos->z - VtxMax->z <= g_aCanon[nCntCanon].VtxMaxCanon.z + g_aCanon[nCntCanon].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aCanon[nCntCanon].VtxMinCanon.x + g_aCanon[nCntCanon].pos.x  && pPos->x + VtxMax->z >= g_aCanon[nCntCanon].VtxMinCanon.x + g_aCanon[nCntCanon].pos.x)
				{//�����炫���Ƃ�
					pPos->x = g_aCanon[nCntCanon].VtxMinCanon.x + g_aCanon[nCntCanon].pos.x - VtxMax->z;

					//bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aCanon[nCntCanon].VtxMaxCanon.x + g_aCanon[nCntCanon].pos.x  && pPos->x - VtxMax->z <= g_aCanon[nCntCanon].VtxMaxCanon.x + g_aCanon[nCntCanon].pos.x)
				{//�E���炫���Ƃ�
					pPos->x = g_aCanon[nCntCanon].VtxMaxCanon.x + g_aCanon[nCntCanon].pos.x + VtxMax->z;
					//bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= (g_aCanon[nCntCanon].VtxMinCanon.z + 50.0f) + g_aCanon[nCntCanon].pos.z && pPos->z + VtxMax->z >= (g_aCanon[nCntCanon].VtxMinCanon.z + 50.0f) + g_aCanon[nCntCanon].pos.z)
				{//��O���痈����
					pPos->z = (g_aCanon[nCntCanon].VtxMinCanon.z + 50.0f) + g_aCanon[nCntCanon].pos.z - VtxMax->z;
					//bLand = true;

				}

				else if (pPosold->z - VtxMax->z >= g_aCanon[nCntCanon].VtxMaxCanon.z + g_aCanon[nCntCanon].pos.z && pPos->z - VtxMax->z <= g_aCanon[nCntCanon].VtxMaxCanon.z + g_aCanon[nCntCanon].pos.z)
				{//�����痈����
					pPos->z = g_aCanon[nCntCanon].VtxMaxCanon.z + g_aCanon[nCntCanon].pos.z + VtxMax->z;
					//bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aCanon[nCntCanon].VtxMaxCanon.y + g_aCanon[nCntCanon].pos.y && pPos->y - VtxMax->y <= g_aCanon[nCntCanon].VtxMaxCanon.y + g_aCanon[nCntCanon].pos.y)
				{//�ォ�痈����
					pMove->y = 0.0f;

					pPos->y = g_aCanon[nCntCanon].VtxMaxCanon.y + g_aCanon[nCntCanon].pos.y + VtxMax->y;
					//bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aCanon[nCntCanon].VtxMinCanon.y + g_aCanon[nCntCanon].pos.y && pPos->y + VtxMax->y >= g_aCanon[nCntCanon].VtxMinCanon.y + g_aCanon[nCntCanon].pos.y)
				{//�����痈����
					pPos->y = g_aCanon[nCntCanon].VtxMinCanon.y + g_aCanon[nCntCanon].pos.y + VtxMin->y;


					//bLand = true;

				}

			}
		}
	}
	return bLand;

}


//=============================================================================
// �q�b�g����
//=============================================================================
void HitCanon(int nDamage, int nCntCanon)
{
	GAMESTATE *pGame;
	pGame = GetGameState();

	if (g_aCanon[nCntCanon].bUse == true)
	{
		g_aCanon[nCntCanon].nLife += nDamage;

		if (g_aCanon[nCntCanon].nLife <= 0)
		{
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			g_aCanon[nCntCanon].bUse = false;
			if (g_aCanon[4].bUse == false)
			{
				if (*pGame == GAMESTATE_NORMAL && *pGame != GAMESTATE_END)
				{
					SetGameState(GAMESTATE_CLEAR);
				}
			}
			AddScore(15000);
			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				SetParticle(g_aCanon[nCntCanon].pos, 60, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
				SetParticle(g_aCanon[nCntCanon].pos, 60, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
			}
		}
	}
}


void SetCanon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g

	for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++)
	{

		if (g_aCanon[nCntCanon].bUse == false)	//�u���b�N���g�p����Ă��Ȃ�
		{
			g_aCanon[nCntCanon].pos = pos;

			g_aCanon[nCntCanon].rot = rot;

			g_aCanon[nCntCanon].nType = nType;

			g_aCanon[nCntCanon].nLife = nLife;

			g_aCanon[nCntCanon].VtxMinCanon = D3DXVECTOR3(100000, 100000, 100000);
			g_aCanon[nCntCanon].VtxMaxCanon = D3DXVECTOR3(-100000, -100000, -100000);


			//���_�����擾
			nNumVtx = g_aCanon[nCntCanon].aCanonModel[g_aCanon[nCntCanon].nType].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_aCanon[nCntCanon].aCanonModel[g_aCanon[nCntCanon].nType].pMesh->GetFVF());

			//���_�o�b�t�@�����b�N
			g_aCanon[nCntCanon].aCanonModel[g_aCanon[nCntCanon].nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W

				if (Vtx.x < g_aCanon[nCntCanon].VtxMinCanon.x)
				{
					g_aCanon[nCntCanon].VtxMinCanon.x = Vtx.x;
				}

				if (Vtx.y < g_aCanon[nCntCanon].VtxMinCanon.y)
				{
					g_aCanon[nCntCanon].VtxMinCanon.y = Vtx.y;
				}

				if (Vtx.z < g_aCanon[nCntCanon].VtxMinCanon.z)
				{
					g_aCanon[nCntCanon].VtxMinCanon.z = Vtx.z - 28.0f;
				}


				if (Vtx.x > g_aCanon[nCntCanon].VtxMaxCanon.x)
				{
					g_aCanon[nCntCanon].VtxMaxCanon.x = Vtx.x;
				}

				if (Vtx.y > g_aCanon[nCntCanon].VtxMaxCanon.y)
				{
					g_aCanon[nCntCanon].VtxMaxCanon.y = Vtx.y;
				}

				if (Vtx.z > g_aCanon[nCntCanon].VtxMaxCanon.z)
				{
					g_aCanon[nCntCanon].VtxMaxCanon.z = Vtx.z;
				}
				pVtxBuff += sizeFVF;
			}

			g_aCanon[nCntCanon].bUse = true;	//�g�p���Ă����Ԃɂ���
			break;
		}
		//���_�o�b�t�@���A�����b�N
		g_aCanon[nCntCanon].aCanonModel[g_aCanon[nCntCanon].nType].pMesh->UnlockVertexBuffer();
	}
}





CANON *GetCanon(void)
{
	return &g_aCanon[0];
}