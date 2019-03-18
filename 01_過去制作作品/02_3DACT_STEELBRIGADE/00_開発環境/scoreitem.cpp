//=============================================================================
//
// �X�R�A�A�C�e������ [ScoreItem.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "scoreitem.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TYPE (1)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshScoreItem = NULL;	//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatScoreItem = NULL;	//�}�e���A�����ւ̃|�C���^

DWORD g_nNumMatScoreItem;	//�}�e���A���̏��̐�


SCOREITMEM g_aScoreItem[MAX_SCOREITEM];
//=============================================================================
// ����������
//=============================================================================
void InitScoreItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		// �ʒu�E�����̏����ݒ�
		g_aScoreItem[nCntScoreItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aScoreItem[nCntScoreItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aScoreItem[nCntScoreItem].VtxMinScoreItem = D3DXVECTOR3(10000, 10000, 10000);
		g_aScoreItem[nCntScoreItem].VtxMaxScoreItem = D3DXVECTOR3(-10000, -10000, -10000);
		g_aScoreItem[nCntScoreItem].bUse = false;

	}


	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/haguruma/gear.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatScoreItem,
		NULL,
		&g_nNumMatScoreItem,
		&g_pMeshScoreItem);


	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		//���_�����擾
		nNumVtx = g_pMeshScoreItem->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshScoreItem->GetFVF());

		//���_�o�b�t�@�����b�N
		g_pMeshScoreItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W

			if (Vtx.x < g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x)
			{
				g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x = Vtx.x;
			}

 			if (Vtx.y < g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y)
			{
				g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y = Vtx.y;
			}

			if (Vtx.z < g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z)
			{
				g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z = Vtx.x;
			}


			if (Vtx.x > g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x)
			{
				g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x = Vtx.x;
			}

			if (Vtx.y > g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y)
			{
				g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y = Vtx.y;
			}

			if (Vtx.z > g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z)
			{
				g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_pMeshScoreItem->UnlockVertexBuffer();


	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitScoreItem(void)
{

	// ���b�V���̊J��
	if (g_pMeshScoreItem != NULL)
	{
		g_pMeshScoreItem->Release();
		g_pMeshScoreItem = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatScoreItem != NULL)
	{
		g_pBuffMatScoreItem->Release();
		g_pBuffMatScoreItem = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScoreItem(void)
{
	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == true)
		{
			g_aScoreItem[nCntScoreItem].rot.y += 0.02f;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScoreItem(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^


	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aScoreItem[nCntScoreItem].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aScoreItem[nCntScoreItem].rot.y, g_aScoreItem[nCntScoreItem].rot.x, g_aScoreItem[nCntScoreItem].rot.z);

			D3DXMatrixMultiply(&g_aScoreItem[nCntScoreItem].mtxWorld, &g_aScoreItem[nCntScoreItem].mtxWorld, &mtxrot);


			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxtrans,
				g_aScoreItem[nCntScoreItem].pos.x, g_aScoreItem[nCntScoreItem].pos.y, g_aScoreItem[nCntScoreItem].pos.z);

			D3DXMatrixMultiply(&g_aScoreItem[nCntScoreItem].mtxWorld, &g_aScoreItem[nCntScoreItem].mtxWorld, &mtxtrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aScoreItem[nCntScoreItem].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatScoreItem->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatScoreItem; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, 0);

				// ���f��(�p�[�c)�̕`��
				g_pMeshScoreItem->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionScoreItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bHit = false; //���n������

	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x && pPos->x - VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x
				&&pPos->y + VtxMax->y >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y && pPos->y - VtxMax->y <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y
				&&pPos->z + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z && pPos->z - VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z)
			{
				if (pPosold->x + VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x  && pPos->x + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x)
				{//�����炫���Ƃ�
					//pPos->x = g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x - VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x  && pPos->x - VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x)
				{//�E���炫���Ƃ�
					//pPos->x = g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x + VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;



				}
				else if (pPosold->z + VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z && pPos->z + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z)
				{//��O���痈����
					//pPos->z = g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z - VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;

				}
				else if (pPosold->z + VtxMin->z >= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z && pPos->z + VtxMin->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z)
				{//�����痈����
					//pPos->z = g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z + VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;

				}
				if (pPosold->y - VtxMax->y >= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y && pPos->y - VtxMax->y <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y)
				{//�ォ�痈����
					//pMove->y = 0.0f;

					bHit = true;
					g_aScoreItem[nCntScoreItem].bUse = false;


				}
				if (pPosold->y + VtxMax->y <= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y && pPos->y + VtxMax->y >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y)
				{//�����痈����
					//pPos->y = g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y + VtxMin->y;

					bHit = true;
					g_aScoreItem[nCntScoreItem].bUse = false;

				}

			}
		}
	}
	return bHit;
}


void SetScoreItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pMeshScoreItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == false)	//�u���b�N���g�p����Ă��Ȃ�
		{
			g_aScoreItem[nCntScoreItem].pos = pos;

			g_aScoreItem[nCntScoreItem].rot = rot;

			//g_aScoreItem[nCntScoreItem].nType = nType;

			//�e�̐ݒ�
			//g_aScoreItem[nCntScoreItem].nIndexShadow = SetShadow(g_aScoreItem[nCntScoreItem].pos, g_aScoreItem[nCntScoreItem].rot, 10.0f, 10.0f);

			g_aScoreItem[nCntScoreItem].bUse = true;	//�g�p���Ă����Ԃɂ���

			break;
		}

	}
	//���_�o�b�t�@���A�����b�N
	g_pMeshScoreItem->UnlockVertexBuffer();
}





SCOREITMEM *GetScoreItem(void)
{
	return &g_aScoreItem[0];
}