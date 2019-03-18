////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�V�������� [meshCylinder.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "meshCylinder.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_MESHCYLINDER	(1)
#define BLOCK_X	(100)
#define BLOCK_Y	(3)
#define VERTEX_WIDTH	(2+2*BLOCK_X)
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ���S���W
	D3DXVECTOR3 rot;			// ����
	MESHCYLINDERTYPE nType;		// ���
	float fWidth;				// ��
	float fHeight;				// ����
	float fDepth;				// ���s��
	float fLength;				// ���a
} MeshCylinderInfo;
typedef struct
{
	char *pFileName;
}MeshCylinderTexture;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_apTextureMeshCylinder[MESHCYLINDERTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;						// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MeshCylinder g_aMeshCylinder[(BLOCK_X + 1) * (BLOCK_Y + 1)];

int g_nNumVertexCylinder;	// ���_��
int g_nNumIndexMeshCylinder;	// �C���f�b�N�X��
int g_nNumPolygonMeshCylinder;	// �|���S����
MeshCylinderInfo g_aMeshCylinderInfo[] =
{
	{ D3DXVECTOR3(0.0f,-700.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MESHCYLINDERTYPE_SEA, 10.0f,800.0f, 0.0f,19000.0f },
};
MeshCylinderTexture g_aMeshCylinderTexInfo[MESHCYLINDERTYPE_MAX] =
{
	{ "data/TEXTURE/mountain000.png" },
};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	
	for (int nCntMeshCylinderTex = 0; nCntMeshCylinderTex < MESHCYLINDERTYPE_MAX; nCntMeshCylinderTex++)
	{// �e�N�X�`���̐ݒ�
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_aMeshCylinderTexInfo[nCntMeshCylinderTex].pFileName,	// �t�@�C���̖��O
			&g_apTextureMeshCylinder[nCntMeshCylinderTex]);			// �e�N�X�`���ւ̃|�C���^
	}
	g_nNumVertexCylinder = (BLOCK_X + 1) * (BLOCK_Y + 1);					// ���_��
	g_nNumIndexMeshCylinder = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1) + 2;	// �C���f�b�N�X��
	g_nNumPolygonMeshCylinder = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1);	// �|���S����

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < g_nNumVertexCylinder; nCntMeshCylinder++)
	{
		g_aMeshCylinder[nCntMeshCylinder].pos = g_aMeshCylinderInfo[nCntMeshCylinder].pos;			// �ʒu
		g_aMeshCylinder[nCntMeshCylinder].rot = g_aMeshCylinderInfo[nCntMeshCylinder].rot;			// ����
		g_aMeshCylinder[nCntMeshCylinder].fWidth = g_aMeshCylinderInfo[nCntMeshCylinder].fWidth;	// ��
		g_aMeshCylinder[nCntMeshCylinder].fHeight = g_aMeshCylinderInfo[nCntMeshCylinder].fHeight;	// ����
		g_aMeshCylinder[nCntMeshCylinder].fDepth = g_aMeshCylinderInfo[nCntMeshCylinder].fDepth;	// ���s��
		g_aMeshCylinder[nCntMeshCylinder].fLength = g_aMeshCylinderInfo[nCntMeshCylinder].fLength;	// ���a
		g_aMeshCylinder[nCntMeshCylinder].nType = g_aMeshCylinderInfo[nCntMeshCylinder].nType;		// ���
		g_aMeshCylinder[nCntMeshCylinder].bUse = true;										// �g�p����Ă��邩�ǂ���

	}

	MakeVertexMeshCylinder(pDevice);

}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitMeshCylinder(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MESHCYLINDERTYPE_MAX; nCntMeshCylinder++)
	{
		if (g_apTextureMeshCylinder[nCntMeshCylinder] != NULL)
		{
			g_apTextureMeshCylinder[nCntMeshCylinder]->Release();
			g_apTextureMeshCylinder[nCntMeshCylinder] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
	// �C���f�b�N�X�o�b�t�@�o�b�t�@�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateMeshCylinder(void)
{
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	// ���e�X�g��ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���l��0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���傫��

	// Z�o�b�t�@�ւ̏�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder[nCntMeshCylinder].rot.y, g_aMeshCylinder[nCntMeshCylinder].rot.x, g_aMeshCylinder[nCntMeshCylinder].rot.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder[nCntMeshCylinder].pos.x, g_aMeshCylinder[nCntMeshCylinder].pos.y, g_aMeshCylinder[nCntMeshCylinder].pos.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));
			
			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMeshCylinder[g_aMeshCylinder[nCntMeshCylinder].nType]);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexCylinder, 0, g_nNumPolygonMeshCylinder);
		}
	}
	// ���e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���
	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexCylinder * MAX_MESHCYLINDER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);
	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD)*g_nNumIndexMeshCylinder * MAX_MESHCYLINDER,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++, pVtx += g_nNumVertexCylinder)
	{
		
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				// ���_���W�̐ݒ�
				pVtx[nCntH + (BLOCK_X+1)*nCntV].pos = D3DXVECTOR3(sinf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*g_aMeshCylinder[nCntMeshCylinder].fLength,
					g_aMeshCylinder[nCntMeshCylinder].fHeight*BLOCK_Y - (g_aMeshCylinder[nCntMeshCylinder].fHeight)* nCntV,
					cosf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*g_aMeshCylinder[nCntMeshCylinder].fLength);
			}
		}
		
		for (int nCnt = 0; nCnt < g_nNumVertexCylinder; nCnt++)
		{
			// �@���̐ݒ�
			D3DXVec3Normalize(&pVtx[nCnt].nor, &pVtx[nCnt].pos);//	 ���K������
		}
		for (int nCnt = 0; nCnt < g_nNumVertexCylinder; nCnt++)
		{
			// ���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//for (int nCnt = 0; nCnt < g_nNumVertexCylinder; nCnt++)
		//{// 1��1��
		//	// �e�N�X�`���̐ݒ�
		//	pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(BLOCK_X+1)), 0.0f + (nCnt / (BLOCK_X + 1)));
		//}
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{// �e�N�X�`���̐ݒ�
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
			 // �e�N�X�`���̐ݒ�
				pVtx[nCntH + (BLOCK_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / BLOCK_X) / 50.0f, float(nCntV * 100 / BLOCK_Y) / 100.0f);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIdxMeshCylinder = 0; nCntIdxMeshCylinder < MAX_MESHCYLINDER; nCntIdxMeshCylinder++, pIdx += g_nNumIndexMeshCylinder)
	{// �C���f�b�N�X�̐ݒ�
		int nCnt = 0;
		// ���_���W�̐ݒ�
		for (int nCntX = 0; nCntX < g_nNumIndexMeshCylinder; nCntX += 2)
		{
			if (BLOCK_Y > 1 && nCntX % (VERTEX_WIDTH + 2) == VERTEX_WIDTH)
			{// �d�Ȃ蕔��
				pIdx[nCntX] = pIdx[nCntX - 1];
				pIdx[nCntX + 1] = (BLOCK_X + 1) + int(nCnt / 2);
			}
			else
			{// �ʏ핔��
				pIdx[nCntX] = (BLOCK_X+1) + int(nCnt / 2);
				pIdx[nCntX + 1] = int(nCnt /2);
				nCnt += 2;
			}
		}
	}	
	// ���_�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCylinder->Unlock();
}
