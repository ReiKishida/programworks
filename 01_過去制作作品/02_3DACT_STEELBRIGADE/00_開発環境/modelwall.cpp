//=============================================================================
//
// ���f������ [modelwall.cpp]
// Author :  Kishida Rei
//
//=============================================================================
#include "modelwall.h"
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
//LPDIRECT3DTEXTURE9 g_pTextureModelWall = NULL;		//�e�N�X�`���ւ̃|�C���^
//LPD3DXMESH g_pMeshModelWall = NULL;	//���b�V�����ւ̃|�C���^
//LPD3DXBUFFER g_pBuffMatModelWall = NULL;	//�}�e���A�����ւ̃|�C���^
//DWORD g_nNumMatModelWall;	//�}�e���A���̏��̐�


WALL g_aModelWall[MAX_MODELWALL];
//=============================================================================
// ����������
//=============================================================================
void InitModelWall(void)
{


	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		g_aModelWall[nCntModelWall].nCntWall = 0;

		g_aModelWall[nCntModelWall].bUse = false;

		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[0].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[1].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[2].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[3].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[4].pTexture);


		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/wall.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[0].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[0].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[0].pMesh);

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/wall02.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[1].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[1].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[1].pMesh);

		D3DXLoadMeshFromX("data/MODEL/wall03.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[2].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[2].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[2].pMesh);


		D3DXLoadMeshFromX("data/MODEL/wall04.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[3].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[3].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[3].pMesh);

		D3DXLoadMeshFromX("data/MODEL/wall05.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[4].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[4].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[4].pMesh);
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitModelWall(void)
{
	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		for (int nCntTypeModelWall = 0; nCntTypeModelWall < MAXTYPE_MODELWALL; nCntTypeModelWall++)
		{
			//�e�N�X�`���̔j��
			if (g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pTexture != NULL)
			{
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pTexture->Release();
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pTexture = NULL;
			}

			// ���b�V���̊J��
			if (g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pMesh != NULL)
			{
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pMesh->Release();
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pMesh = NULL;
			}

			// �}�e���A���̊J��
			if (g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pBuffMat != NULL)
			{
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pBuffMat->Release();
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pBuffMat = NULL;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModelWall(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModelWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^


	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		for (int nCntTypeModelWall = 0; nCntTypeModelWall < MAXTYPE_MODELWALL; nCntTypeModelWall++)
		{
			if (g_aModelWall[nCntModelWall].bUse == true)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aModelWall[nCntModelWall].mtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_aModelWall[nCntModelWall].rot.y, g_aModelWall[nCntModelWall].rot.x, g_aModelWall[nCntModelWall].rot.z);

				D3DXMatrixMultiply(&g_aModelWall[nCntModelWall].mtxWorld, &g_aModelWall[nCntModelWall].mtxWorld, &mtxrot);


				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxtrans,
					g_aModelWall[nCntModelWall].pos.x, g_aModelWall[nCntModelWall].pos.y, g_aModelWall[nCntModelWall].pos.z);

				D3DXMatrixMultiply(&g_aModelWall[nCntModelWall].mtxWorld, &g_aModelWall[nCntModelWall].mtxWorld, &mtxtrans);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aModelWall[nCntModelWall].mtxWorld);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].nNumMat; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					pDevice->SetTexture(0, g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pTexture);

					// ���f��(�p�[�c)�̕`��
					g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->DrawSubset(nCntMat);
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}


bool CollisionModelWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //���n������

	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		if (g_aModelWall[nCntModelWall].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x && pPos->x - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x
				&&pPos->y + VtxMax->y >= g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y && pPos->y - VtxMax->y <= g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y
				&&pPos->z + VtxMax->z >= g_aModelWall[nCntModelWall].VtxMinModelWall.z + g_aModelWall[nCntModelWall].pos.z && pPos->z - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x  && pPos->x + VtxMax->z >= g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x)
				{//�����炫���Ƃ�
					pPos->x = g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x - VtxMax->z;

					//bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x  && pPos->x - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x)
				{//�E���炫���Ƃ�
					pPos->x = g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x + VtxMax->z;
					//bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= (g_aModelWall[nCntModelWall].VtxMinModelWall.z + 50.0f) + g_aModelWall[nCntModelWall].pos.z && pPos->z + VtxMax->z >= (g_aModelWall[nCntModelWall].VtxMinModelWall.z + 50.0f) + g_aModelWall[nCntModelWall].pos.z)
				{//��O���痈����
					pPos->z = (g_aModelWall[nCntModelWall].VtxMinModelWall.z + 50.0f) + g_aModelWall[nCntModelWall].pos.z - VtxMax->z ;
					//bLand = true;

				}

				else if (pPosold->z - VtxMax->z >= g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z && pPos->z - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z)
				{//�����痈����
					pPos->z = g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z + VtxMax->z;
					//bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y && pPos->y - VtxMax->y <= g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y)
				{//�ォ�痈����
					pMove->y = 0.0f;

					pPos->y = g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y + VtxMax->y;
					//bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y && pPos->y + VtxMax->y >= g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y)
				{//�����痈����
					pPos->y = g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y + VtxMin->y;


					//bLand = true;

				}

			}
		}
	}
	return bLand;

}




void SetModelWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g

	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{

		if (g_aModelWall[nCntModelWall].bUse == false)	//�u���b�N���g�p����Ă��Ȃ�
		{
			g_aModelWall[nCntModelWall].pos = pos;

			g_aModelWall[nCntModelWall].rot = rot;

			g_aModelWall[nCntModelWall].nType = nType;


			g_aModelWall[nCntModelWall].VtxMinModelWall = D3DXVECTOR3(100000, 100000, 100000);
			g_aModelWall[nCntModelWall].VtxMaxModelWall = D3DXVECTOR3(-100000, -100000, -100000);

			//���_�����擾
			nNumVtx = g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->GetFVF());

			//���_�o�b�t�@�����b�N
			g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W

				if (Vtx.x < g_aModelWall[nCntModelWall].VtxMinModelWall.x)
				{
					g_aModelWall[nCntModelWall].VtxMinModelWall.x = Vtx.x;
				}

				if (Vtx.y < g_aModelWall[nCntModelWall].VtxMinModelWall.y)
				{
					g_aModelWall[nCntModelWall].VtxMinModelWall.y = Vtx.y;
				}

				if (Vtx.z < g_aModelWall[nCntModelWall].VtxMinModelWall.z)
				{
					g_aModelWall[nCntModelWall].VtxMinModelWall.z = Vtx.z - 28.0f;
				}


				if (Vtx.x > g_aModelWall[nCntModelWall].VtxMaxModelWall.x)
				{
					g_aModelWall[nCntModelWall].VtxMaxModelWall.x = Vtx.x;
				}

				if (Vtx.y > g_aModelWall[nCntModelWall].VtxMaxModelWall.y)
				{
					g_aModelWall[nCntModelWall].VtxMaxModelWall.y = Vtx.y;
				}

				if (Vtx.z > g_aModelWall[nCntModelWall].VtxMaxModelWall.z)
				{
					g_aModelWall[nCntModelWall].VtxMaxModelWall.z = Vtx.z;
				}
				pVtxBuff += sizeFVF;
			}

			g_aModelWall[nCntModelWall].bUse = true;	//�g�p���Ă����Ԃɂ���
			break;
		}
		//���_�o�b�t�@���A�����b�N
		g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->UnlockVertexBuffer();
	}
}





WALL *GetModelWall(void)
{
	return &g_aModelWall[0];
}