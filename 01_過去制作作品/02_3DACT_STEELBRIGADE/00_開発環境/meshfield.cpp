//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "meshfield.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESH_VERTEX ((g_Mesh_x + 1) * (g_Mesh_z + 1))
#define MESH_INDEX ( (g_Mesh_x * g_Mesh_z )* 2 + (4 * (g_Mesh_z - 1)) + 2 )
#define MESH_POLYGON ( (g_Mesh_x * g_Mesh_z) * 2 + (4 * (g_Mesh_z - 1)) )

#define MESH_WIDTH (200.0f)
#define MESH_DEPTH (200.0f)

#define MESH_X (10)
#define MESH_Z (21)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexMeshField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL; //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_posMeshField;		//�ʒu(���S���W)
D3DXVECTOR3 g_rotMeshField;		//����
D3DXMATRIX g_mtxWorldMeshField;	//���[���h�}�g���b�N�X

int vertex;
int polygon;
int index;

int g_Mesh_x, g_Mesh_z;

//=============================================================================
// ����������
//=============================================================================
void InitMeshField()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	g_Mesh_x = MESH_X;
	g_Mesh_z = MESH_Z;



	MakeVertexMeshField(pDevice);
	MakeIndexMeshField(pDevice);

	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);




}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if(g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxrot, mtxtrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxrot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxtrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, g_pTextureMeshField);


	// ���b�V���t�B�[���h�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0, 0, MESH_VERTEX, 0, MESH_POLYGON);



}


////=============================================================================
//// ���_���̍쐬
////=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field002.jpg", &g_pTextureMeshField);


	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESH_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);



	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntMesh_Z = 0; nCntMesh_Z <= g_Mesh_z; nCntMesh_Z++)
		{
			for (int nCntMesh_X = 0; nCntMesh_X <= g_Mesh_x; nCntMesh_X++)
			{

				pVtx[0].pos = D3DXVECTOR3(-MESH_WIDTH + (nCntMesh_X * MESH_WIDTH), 0.0f, MESH_DEPTH - (nCntMesh_Z * MESH_DEPTH));

				//�@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.9f, 0.4f, 0.3f, 1.0f);

				//�e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(nCntMesh_X * 1.0f, nCntMesh_Z * 1.0f);

				pVtx += 1;



			}
		}




	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshField->Unlock();



}

void MakeIndexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESH_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//�C���f�b�N�X�̃J�E���g

						//�C���f�b�N�X�o�b�t�@�����b�N
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_Z = 0; nCntIndex_Z < g_Mesh_z; nCntIndex_Z++)
	{
		for (int nCntIndex_X = 0; nCntIndex_X <= g_Mesh_x; nCntIndex_X++, nCntIndex++)
		{
			pIdx[0] = (g_Mesh_x + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex_X == g_Mesh_x && nCntIndex_Z < g_Mesh_z -1)
			{
				pIdx[0] = nCntIndex ;
				pIdx[1] = (g_Mesh_x + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pIdxBuffMeshField->Unlock();

}




