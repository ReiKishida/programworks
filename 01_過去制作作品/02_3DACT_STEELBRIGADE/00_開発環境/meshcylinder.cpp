//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "meshcylinder.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CYLINDER (128)
#define MAX_TEXTURE (2)

#define MESHCYLINDER_VERTEX (  (g_MeshCylinder_h + 1) * (g_MeshCylinder_v + 1)   )	//���_��
#define MESHCYLINDER_INDEX (   (g_MeshCylinder_h * g_MeshCylinder_v )* 2 + (4 * (g_MeshCylinder_v - 1)) + 2   ) //�C���f�b�N�X��
#define MESHCYLINDER_POLYGON ( (g_MeshCylinder_h * g_MeshCylinder_v) * 2 + (4 * (g_MeshCylinder_v - 1)) )	//�|���S����

#define MESHCYLINDER_RADIUS (50.0f)
#define MESHCYLINDER_HEIGHT (6.0f)

#define MESH_H (20) //���b�V���̕�����(H)
#define MESH_V (1) //���b�V���̕�����(V)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder[MAX_TEXTURE] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL; //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

CYLINDER g_aCylinder[MAX_CYLINDER];

int g_MeshCylinder_h, g_MeshCylinder_v;
//=============================================================================
// ����������
//=============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	g_MeshCylinder_h = MESH_H;
	g_MeshCylinder_v = MESH_V;


	MakeVertexMeshCylinder(pDevice);
	MakeIndexMeshCylinder(pDevice);

	for (int nCntCylinder = 0; nCntCylinder < MAX_CYLINDER; nCntCylinder++)
	{
		g_aCylinder[nCntCylinder].bUse = false;
	}




}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshCylinder(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMeshCylinder[nCntTexture] != NULL)
		{
			g_pTextureMeshCylinder[nCntTexture]->Release();
			g_pTextureMeshCylinder[nCntTexture] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshCylinder(void)
{
	for (int nCntCylinder = 0; nCntCylinder < MAX_CYLINDER; nCntCylinder++)
	{
		if (g_aCylinder[nCntCylinder].bUse == true)
		{
			g_aCylinder[0].rot.y += 0.001f;
			g_aCylinder[1].rot.y -= 0.0011f;
			g_aCylinder[2].rot.y -= 0.0014f;
			g_aCylinder[3].rot.y += 0.0016f;
			g_aCylinder[4].rot.y -= 0.0011f;
			g_aCylinder[5].rot.y += 0.001f;
			g_aCylinder[6].rot.y += 0.0012f;
			g_aCylinder[7].rot.y -= 0.001f;
			g_aCylinder[8].rot.y += 0.001f;

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxrot, mtxtrans;


	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntCylinder = 0; nCntCylinder < MAX_CYLINDER; nCntCylinder++)
	{
		if (g_aCylinder[nCntCylinder].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aCylinder[nCntCylinder].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aCylinder[nCntCylinder].rot.y, g_aCylinder[nCntCylinder].rot.x, g_aCylinder[nCntCylinder].rot.z);

			D3DXMatrixMultiply(&g_aCylinder[nCntCylinder].mtxWorld, &g_aCylinder[nCntCylinder].mtxWorld, &mtxrot);


			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxtrans,
				g_aCylinder[nCntCylinder].pos.x, g_aCylinder[nCntCylinder].pos.y, g_aCylinder[nCntCylinder].pos.z);

			D3DXMatrixMultiply(&g_aCylinder[nCntCylinder].mtxWorld, &g_aCylinder[nCntCylinder].mtxWorld, &mtxtrans);


			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCylinder[nCntCylinder].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, g_pTextureMeshCylinder[g_aCylinder[nCntCylinder].nType]);


			// ���b�V���V�����_�[�̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHCYLINDER_VERTEX, 0, MESHCYLINDER_POLYGON);

		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//
////=============================================================================
//// ���_���̍쐬
////=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/obi.png", &g_pTextureMeshCylinder[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/city.png", &g_pTextureMeshCylinder[1]);


	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHCYLINDER_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);



	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshCylinder_V = 0; nCntMeshCylinder_V < g_MeshCylinder_v + 1; nCntMeshCylinder_V++)
	{
		for (int nCntMeshCylinder_H = 0; nCntMeshCylinder_H < g_MeshCylinder_h + 1; nCntMeshCylinder_H++)
		{
			pVtx[0].pos.x = sinf(-D3DX_PI * (nCntMeshCylinder_H * (1.0f / (g_MeshCylinder_h / 2)))) * MESHCYLINDER_RADIUS;
			pVtx[0].pos.y = MESHCYLINDER_HEIGHT * nCntMeshCylinder_V;
			pVtx[0].pos.z = cosf(-D3DX_PI * (nCntMeshCylinder_H * (1.0f / (g_MeshCylinder_h / 2)))) * MESHCYLINDER_RADIUS;
			//�@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(sinf(-D3DX_PI * (0.25f * nCntMeshCylinder_H)), 0.0f, cosf(D3DX_PI * (0.25f * nCntMeshCylinder_H)));
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntMeshCylinder_H * (1.0f / (g_MeshCylinder_h / 2)), nCntMeshCylinder_V * -1.0f);


			pVtx += 1;
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshCylinder->Unlock();



}

////=============================================================================
//// �C���f�b�N�X���쐬
////=============================================================================
void MakeIndexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHCYLINDER_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//�C���f�b�N�X�̃J�E���g

						//�C���f�b�N�X�o�b�t�@�����b�N
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_MeshCylinder_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_MeshCylinder_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_MeshCylinder_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_MeshCylinder_h && nCntIndex_V <= g_MeshCylinder_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_MeshCylinder_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pIdxBuffMeshCylinder->Unlock();

}


//=============================================================================
// �V�����_�[�쐬
//=============================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntCylinder = 0; nCntCylinder < MAX_CYLINDER; nCntCylinder++)
	{
		if (g_aCylinder[nCntCylinder].bUse == false)
		{
			g_aCylinder[nCntCylinder].pos = pos;
			g_aCylinder[nCntCylinder].rot = rot;
			g_aCylinder[nCntCylinder].bUse = true;

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshCylinder->Unlock();
}




