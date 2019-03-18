//=============================================================================
//
// �又�� [gate.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "gate.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "sound.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


#define STAGE_LIMIT_MAX_X (150.0f)
#define STAGE_LIMIT_MIN_X (-50.0f)
#define STAGE_LIMIT_MAX_Z (50.0f)
#define STAGE_LIMIT_MIN_Z (-150.0f)


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGate = NULL;		//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH g_pMeshGate = NULL;	//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatGate = NULL;	//�}�e���A�����ւ̃|�C���^

DWORD g_nNumMatGate;	//�}�e���A���̏��̐�


GATE g_aGate[MAX_GATE];
//=============================================================================
// ����������
//=============================================================================
void InitGate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		// �ʒu�E�����̏����ݒ�
		g_aGate[nCntGate].VtxMinGate = D3DXVECTOR3(10000, 10000, 10000);
		g_aGate[nCntGate].VtxMaxGate = D3DXVECTOR3(-10000, -10000, -10000);
		g_aGate[nCntGate].bUse = false;
		//g_aGate[nCntGate].nCnt = 0;

	}
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/seidou.jpeg", &g_pTextureGate);


	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/gate2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatGate,
		NULL,
		&g_nNumMatGate,
		&g_pMeshGate);

	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		//���_�����擾
		nNumVtx = g_pMeshGate->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshGate->GetFVF());

		//���_�o�b�t�@�����b�N
		g_pMeshGate->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W

			if (Vtx.x < g_aGate[nCntGate].VtxMinGate.x)
			{
				g_aGate[nCntGate].VtxMinGate.x = Vtx.x;
			}

			if (Vtx.y < g_aGate[nCntGate].VtxMinGate.y)
			{
				g_aGate[nCntGate].VtxMinGate.y = Vtx.y;
			}

			if (Vtx.z < g_aGate[nCntGate].VtxMinGate.z)
			{
				g_aGate[nCntGate].VtxMinGate.z = Vtx.z;
			}


			if (Vtx.x > g_aGate[nCntGate].VtxMaxGate.x)
			{
				g_aGate[nCntGate].VtxMaxGate.x = Vtx.x;
			}

			if (Vtx.y > g_aGate[nCntGate].VtxMaxGate.y)
			{
				g_aGate[nCntGate].VtxMaxGate.y = Vtx.y;
			}

			if (Vtx.z > g_aGate[nCntGate].VtxMaxGate.z)
			{
				g_aGate[nCntGate].VtxMaxGate.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_pMeshGate->UnlockVertexBuffer();
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitGate(void)
{

	//�e�N�X�`���̔j��
	if (g_pTextureGate != NULL)
	{
		g_pTextureGate->Release();
		g_pTextureGate = NULL;
	}

	// ���b�V���̊J��
	if (g_pMeshGate != NULL)
	{
		g_pMeshGate->Release();
		g_pMeshGate = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatGate != NULL)
	{
		g_pBuffMatGate->Release();
		g_pBuffMatGate = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGate(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGate(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^


	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aGate[nCntGate].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aGate[nCntGate].rot.y, g_aGate[nCntGate].rot.x, g_aGate[nCntGate].rot.z);

			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxrot);


			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxtrans,
				g_aGate[nCntGate].pos.x, g_aGate[nCntGate].pos.y, g_aGate[nCntGate].pos.z);

			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxtrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCntGate].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatGate->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatGate; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, g_pTextureGate);

				// ���f��(�p�[�c)�̕`��
				g_pMeshGate->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionGate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //���n������

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x && pPos->x - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x
				&&pPos->y + VtxMax->y >= g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y && pPos->y - VtxMax->y <= g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y
				&&pPos->z + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z && pPos->z - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x  && pPos->x + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x)
				{//�����炫���Ƃ�
					pPos->x = g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x  && pPos->x - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x)
				{//�E���炫���Ƃ�
					pPos->x = g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z && pPos->z + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z)
				{//��O���痈����
					pPos->z = g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z && pPos->z - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z)
				{//�����痈����
					pPos->z = g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z + VtxMax->z;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y && pPos->y - VtxMax->y <= g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y)
				{//�ォ�痈����
					pMove->y = 0.0f;

					pPos->y = g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y && pPos->y + VtxMax->y >= g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y)
				{//�����痈����
					pPos->y = g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}




void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

					//���_�o�b�t�@�����b�N
	g_pMeshGate->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == false)	//�u���b�N���g�p����Ă��Ȃ�
		{
			g_aGate[nCntGate].pos = pos;

			g_aGate[nCntGate].rot = rot;

			g_aGate[nCntGate].nLife = nLife;

			g_aGate[nCntGate].bUse = true;	//�g�p���Ă����Ԃɂ���
			g_aGate[nCntGate].nCnt++;
			break;
		}

	}
	//���_�o�b�t�@���A�����b�N
	g_pMeshGate->UnlockVertexBuffer();
}


//=============================================================================
// ��_���[�W����
//=============================================================================
void HitGate(int nDamage , int nCntGate)
{

		if (g_aGate[nCntGate].bUse == true)
		{
			g_aGate[nCntGate].nLife += nDamage;
			for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
			{
				SetParticle(g_aGate[nCntGate].pos, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
			}

			if (g_aGate[nCntGate].nLife <= 0)
			{
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
				g_aGate[nCntGate].bUse = false;
				for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
				{
					SetParticle(g_aGate[nCntGate].pos, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
					SetParticle(g_aGate[nCntGate].pos, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);

				}
				AddScore(5000);
				g_aGate[nCntGate].nCnt--;
			}
		}

}



GATE *GetGate(void)
{
	return &g_aGate[0];
}