//=============================================================================
//
// ���f������ [model.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "model.h"
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
#define MAX_TYPE (4)

#define MOVE_SPEED (1.5f)
#define ROT_SPEED (0.1f)

#define STAGE_LIMIT_MAX_X (150.0f)
#define STAGE_LIMIT_MIN_X (-50.0f)
#define STAGE_LIMIT_MAX_Z (50.0f)
#define STAGE_LIMIT_MIN_Z (-150.0f)

#define MODEL_TEXTURENAME_0	"data/TEXTURE/rockandtekkotu.jpg"		//���f���̃e�N�X�`��1
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureModel = NULL;		//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH g_pMeshModel = NULL;	//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;	//�}�e���A�����ւ̃|�C���^

DWORD g_nNumMatModel;	//�}�e���A���̏��̐�


MODEL g_aModel[MAX_MODEL];
//=============================================================================
// ����������
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MODEL_TEXTURENAME_0, &g_pTextureModel);


	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		// �ʒu�E�����̏����ݒ�
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);
		g_aModel[nCntModel].VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);
		g_aModel[nCntModel].bUse = false;
	}


	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/rock.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel);

	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		//���_�����擾
		nNumVtx = g_pMeshModel->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

		//���_�o�b�t�@�����b�N
		g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W

			if (Vtx.x < g_aModel[nCntModel].VtxMinModel.x)
			{
				g_aModel[nCntModel].VtxMinModel.x = Vtx.x;
			}

			if (Vtx.y < g_aModel[nCntModel].VtxMinModel.y)
			{
				g_aModel[nCntModel].VtxMinModel.y = Vtx.y;
			}

			if (Vtx.z < g_aModel[nCntModel].VtxMinModel.z)
			{
				g_aModel[nCntModel].VtxMinModel.z = Vtx.x;
			}


			if (Vtx.x > g_aModel[nCntModel].VtxMaxModel.x)
			{
				g_aModel[nCntModel].VtxMaxModel.x = Vtx.x;
			}

			if (Vtx.y > g_aModel[nCntModel].VtxMaxModel.y)
			{
				g_aModel[nCntModel].VtxMaxModel.y = Vtx.y;
			}

			if (Vtx.z > g_aModel[nCntModel].VtxMaxModel.z)
			{
				g_aModel[nCntModel].VtxMaxModel.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_pMeshModel->UnlockVertexBuffer();
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{

		//�e�N�X�`���̔j��
	if (g_pTextureModel != NULL)
	{
		g_pTextureModel->Release();
		g_pTextureModel = NULL;
	}

	// ���b�V���̊J��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^


	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);

			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxrot);


			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxtrans,
				g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);

			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxtrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, g_pTextureModel);

				// ���f��(�p�[�c)�̕`��
				g_pMeshModel->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //���n������

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			if (  pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x
				&&pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y
				&&pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x  && pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x)
				{//�����炫���Ƃ�
					pPos->x = g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x  && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x)
				{//�E���炫���Ƃ�
					pPos->x = g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z)
				{//��O���痈����
					pPos->z = g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z<= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
				{//�����痈����
					pPos->z = g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z + VtxMax->z ;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y)
				{//�ォ�痈����
					pMove->y = 0.0f;

					pPos->y = g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y)
				{//�����痈����
					pPos->y = g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}


bool CollisionModelEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //���n������

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x
				&&pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y
				&&pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x  && pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x)
				{//�����炫���Ƃ�
					pPos->x = g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x  && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x)
				{//�E���炫���Ƃ�
					pPos->x = g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z)
				{//��O���痈����
					pPos->z = g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
				{//�����痈����
					pPos->z = g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z + VtxMax->z;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y)
				{//�ォ�痈����
					pMove->y = 0.0f;

					pPos->y = g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y)
				{//�����痈����
					pPos->y = g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}


void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

					//���_�o�b�t�@�����b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)	//�u���b�N���g�p����Ă��Ȃ�
		{
			g_aModel[nCntModel].pos = pos;

			g_aModel[nCntModel].rot = rot;

			g_aModel[nCntModel].nType = nType;

			//�e�̐ݒ�
			g_aModel[nCntModel].g_nIndexShadow = SetShadow(g_aModel[nCntModel].pos, g_aModel[nCntModel].rot, 60.0f, 60.0f);

			g_aModel[nCntModel].bUse = true;	//�g�p���Ă����Ԃɂ���

			break;
		}

	}
	//���_�o�b�t�@���A�����b�N
	g_pMeshModel->UnlockVertexBuffer();
}





MODEL *GetModel(void)
{
	return &g_aModel[0];
}