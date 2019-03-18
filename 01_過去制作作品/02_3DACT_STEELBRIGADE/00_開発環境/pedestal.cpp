//=============================================================================
//
// �y�䏈�� [Pedestal.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "pedestal.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "sound.h"

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
LPDIRECT3DTEXTURE9 g_pTexturePedestal = NULL;		//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH g_pMeshPedestal = NULL;	//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatPedestal = NULL;	//�}�e���A�����ւ̃|�C���^

DWORD g_nNumMatPedestal;	//�}�e���A���̏��̐�


PEDESTAL g_aPedestal[MAX_PEDESTAL];
//=============================================================================
// ����������
//=============================================================================
void InitPedestal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		// �ʒu�E�����̏����ݒ�
		/*	g_aPedestal[nCntPedestal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPedestal[nCntPedestal].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/
		g_aPedestal[nCntPedestal].VtxMinPedestal = D3DXVECTOR3(10000, 10000, 10000);
		g_aPedestal[nCntPedestal].VtxMaxPedestal = D3DXVECTOR3(-10000, -10000, -10000);
		g_aPedestal[nCntPedestal].bUse = false;
	}


	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rockandtekkotu.jpg", &g_pTexturePedestal);


	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/pedestal.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatPedestal,
		NULL,
		&g_nNumMatPedestal,
		&g_pMeshPedestal);

	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		//���_�����擾
		nNumVtx = g_pMeshPedestal->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshPedestal->GetFVF());

		//���_�o�b�t�@�����b�N
		g_pMeshPedestal->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W

			if (Vtx.x < g_aPedestal[nCntPedestal].VtxMinPedestal.x)
			{
				g_aPedestal[nCntPedestal].VtxMinPedestal.x = Vtx.x;
			}

			if (Vtx.y < g_aPedestal[nCntPedestal].VtxMinPedestal.y)
			{
				g_aPedestal[nCntPedestal].VtxMinPedestal.y = Vtx.y;
			}

			if (Vtx.z < g_aPedestal[nCntPedestal].VtxMinPedestal.z)
			{
				g_aPedestal[nCntPedestal].VtxMinPedestal.z = Vtx.z;
			}


			if (Vtx.x > g_aPedestal[nCntPedestal].VtxMaxPedestal.x)
			{
				g_aPedestal[nCntPedestal].VtxMaxPedestal.x = Vtx.x;
			}

			if (Vtx.y > g_aPedestal[nCntPedestal].VtxMaxPedestal.y)
			{
				g_aPedestal[nCntPedestal].VtxMaxPedestal.y = Vtx.y;
			}

			if (Vtx.z > g_aPedestal[nCntPedestal].VtxMaxPedestal.z)
			{
				g_aPedestal[nCntPedestal].VtxMaxPedestal.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_pMeshPedestal->UnlockVertexBuffer();
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitPedestal(void)
{

	//�e�N�X�`���̔j��
	if (g_pTexturePedestal != NULL)
	{
		g_pTexturePedestal->Release();
		g_pTexturePedestal = NULL;
	}

	// ���b�V���̊J��
	if (g_pMeshPedestal != NULL)
	{
		g_pMeshPedestal->Release();
		g_pMeshPedestal = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatPedestal != NULL)
	{
		g_pBuffMatPedestal->Release();
		g_pBuffMatPedestal = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePedestal(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPedestal(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^


	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPedestal[nCntPedestal].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aPedestal[nCntPedestal].rot.y, g_aPedestal[nCntPedestal].rot.x, g_aPedestal[nCntPedestal].rot.z);

			D3DXMatrixMultiply(&g_aPedestal[nCntPedestal].mtxWorld, &g_aPedestal[nCntPedestal].mtxWorld, &mtxrot);


			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxtrans,
				g_aPedestal[nCntPedestal].pos.x, g_aPedestal[nCntPedestal].pos.y, g_aPedestal[nCntPedestal].pos.z);

			D3DXMatrixMultiply(&g_aPedestal[nCntPedestal].mtxWorld, &g_aPedestal[nCntPedestal].mtxWorld, &mtxtrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPedestal[nCntPedestal].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatPedestal->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatPedestal; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, g_pTexturePedestal);

				// ���f��(�p�[�c)�̕`��
				g_pMeshPedestal->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionPedestal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //���n������

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x
				&&pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y
				&&pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//�����炫���Ƃ�
					pPos->x = g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x - VtxMax->z;

					//bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//�E���炫���Ƃ�
					pPos->x = g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x + VtxMax->z;
					//bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//��O���痈����
					pPos->z = g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z - VtxMax->z;
					//	bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//�����痈����
					pPos->z = g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z + VtxMax->z;
					//bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//�ォ�痈����
					pMove->y = 0.0f;

					pPos->y = g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMax->y;
					//bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//�����痈����
					pPos->y = g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMin->y;


					//bLand = true;

				}

			}
		}
	}
	return bLand;

}


bool CollisionPedestalEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //���n������

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x
				&&pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y
				&&pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//�����炫���Ƃ�
					pPos->x = g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//�E���炫���Ƃ�
					pPos->x = g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//��O���痈����
					pPos->z = g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//�����痈����
					pPos->z = g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z + VtxMax->z;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//�ォ�痈����
					pMove->y = 0.0f;

					pPos->y = g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//�����痈����
					pPos->y = g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}


void SetPedestal(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

					//���_�o�b�t�@�����b�N
	g_pMeshPedestal->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == false)	//�u���b�N���g�p����Ă��Ȃ�
		{
			g_aPedestal[nCntPedestal].pos = pos;

			g_aPedestal[nCntPedestal].rot = rot;



			g_aPedestal[nCntPedestal].bUse = true;	//�g�p���Ă����Ԃɂ���

			break;
		}

	}
	//���_�o�b�t�@���A�����b�N
	g_pMeshPedestal->UnlockVertexBuffer();
}





PEDESTAL *GetPedestal(void)
{
	return &g_aPedestal[0];
}