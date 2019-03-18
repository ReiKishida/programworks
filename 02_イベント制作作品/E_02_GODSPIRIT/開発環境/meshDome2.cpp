//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// ���b�V���V�h�[������ [meshDome2.cpp]
// Author : ���a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "meshDome2.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_MESHDOME2	(1)

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �\���̒�`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXMATRIX	mtxWorld;				// ���[���h�}�g���b�N�X
	float fRadius;						// ���a
	int nHBlock;						// X�����̃u���b�N��
	int nVBlock;						// Y�����̃u���b�N��
	int nNumVertex;						// ���_��
	int nNumIndex;						// �C���f�b�N�X��
	int nNumPolygon;					// �|���S����
	int nTexType;						// �e�N�X�`���[���
	bool bInside;						// �������ǂ���
	bool bUse;							// �g�p���Ă��邩�ǂ���
}MeshDome2;

typedef enum
{
	POLYGON_TEX_000 = 0,	// �|���S���e�N�X�`��000
	POLYGON_TEX_MAX,		// ����
}MeshDome2TexName;

typedef struct
{
	char *pFilename;	// �t�@�C����
} MeshDome2TexInfo;
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �v���g�^�C�v�錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexMeshDome2(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexMeshDome2(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���[�o���ϐ�
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9			g_apTextureMeshDome2[POLYGON_TEX_MAX] = {};	// �e�N�X�`���ւ̃|�C���^

MeshDome2 g_aMeshDome2[MAX_MESHDOME2];				// ���b�V���h�[���̏��

// �ǂݍ��ރe�N�X�`�����̐ݒ�
MeshDome2TexInfo g_apTextureMeshDome2Info[POLYGON_TEX_MAX] =
{
	{ "data//TEXTURE//sky001.jpg" },		// �e�N�X�`��0
};

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitMeshDome2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	for (int nCntMeshDome2Tex = 0; nCntMeshDome2Tex < POLYGON_TEX_MAX; nCntMeshDome2Tex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureMeshDome2Info[nCntMeshDome2Tex].pFilename,
			&g_apTextureMeshDome2[nCntMeshDome2Tex]);
	}
	
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		g_aMeshDome2[nCntMeshDome2].pVtxBuff = NULL;
		g_aMeshDome2[nCntMeshDome2].pIdxBuff = NULL;
		g_aMeshDome2[nCntMeshDome2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshDome2[nCntMeshDome2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshDome2[nCntMeshDome2].nHBlock = 0;
		g_aMeshDome2[nCntMeshDome2].nVBlock = 0;
		g_aMeshDome2[nCntMeshDome2].fRadius = 0.0f;
		g_aMeshDome2[nCntMeshDome2].nNumVertex = 0;
		g_aMeshDome2[nCntMeshDome2].nNumIndex = 0;
		g_aMeshDome2[nCntMeshDome2].nNumPolygon = 0;
		g_aMeshDome2[nCntMeshDome2].nTexType = 0;
		g_aMeshDome2[nCntMeshDome2].bInside = false;
	}
	// �z�u���b�V���̏��
	g_aMeshDome2[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshDome2[0].nHBlock = 64;
	g_aMeshDome2[0].nVBlock = 16;
	g_aMeshDome2[0].fRadius = 10000.0f;
	g_aMeshDome2[0].bInside = false;

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// �C���f�b�N�X���v�Z	
		g_aMeshDome2[nCntMeshDome2].nNumIndex = 2 + g_aMeshDome2[nCntMeshDome2].nHBlock * 2;
		g_aMeshDome2[nCntMeshDome2].nNumIndex += (g_aMeshDome2[nCntMeshDome2].nNumIndex + 2) * (g_aMeshDome2[nCntMeshDome2].nVBlock - 1);

		// �|���S�����v�Z
		g_aMeshDome2[nCntMeshDome2].nNumPolygon = (g_aMeshDome2[nCntMeshDome2].nHBlock * g_aMeshDome2[nCntMeshDome2].nVBlock) * 2 + 4 * (g_aMeshDome2[nCntMeshDome2].nVBlock - 1);

		// ���_���v�Z
		g_aMeshDome2[nCntMeshDome2].nNumVertex = (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)*(g_aMeshDome2[nCntMeshDome2].nVBlock + 1);
	}

	// ���_���̍쐬
	MakeVertexMeshDome2(pDevice);

	// �C���f�b�N�X���̍쐬
	MakeIndexMeshDome2(pDevice);

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̐ݒ�	���W�̒��ڎw��͂����ł͊�{�I�ɂ͂��Ȃ�(g_pos~~���𑫂������͂��Ȃ�D3DXMatrixTranslation�ł���Ă�
		for (int nCntPos = 0; nCntPos < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntPos++, pVtx++)
		{
			pVtx[0].tex += D3DXVECTOR2(0.5f, 0.0f);
		}
		// ���_�o�b�t�@���A�����b�N����
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Unlock();
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitMeshDome2(void)
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < POLYGON_TEX_MAX; nCntTex++)
	{
		if (g_apTextureMeshDome2[nCntTex] != NULL)
		{
			g_apTextureMeshDome2[nCntTex]->Release();
			g_apTextureMeshDome2[nCntTex] = NULL;
		}
	}

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// ���_�o�b�t�@�̊J��
		if (g_aMeshDome2[nCntMeshDome2].pVtxBuff != NULL)
		{
			g_aMeshDome2[nCntMeshDome2].pVtxBuff->Release();
			g_aMeshDome2[nCntMeshDome2].pVtxBuff = NULL;
		}

		// �C���f�b�N�X�o�b�t�@�̊J��
		if (g_aMeshDome2[nCntMeshDome2].pIdxBuff != NULL)
		{
			g_aMeshDome2[nCntMeshDome2].pIdxBuff->Release();
			g_aMeshDome2[nCntMeshDome2].pIdxBuff = NULL;
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateMeshDome2(void)
{
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		g_aMeshDome2[nCntMeshDome2].rot.y += 0.001f;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawMeshDome2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot,mtxTrans;					// �v�Z�p�}�g���b�N�X����

	// ���C�g�I�t
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMeshDome2[nCntMeshDome2].mtxWorld);

		// ��]�𔽉f	Y,X,Z�̏��Ԃ����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshDome2[nCntMeshDome2].rot.y, g_aMeshDome2[nCntMeshDome2].rot.x, g_aMeshDome2[nCntMeshDome2].rot.z);
		D3DXMatrixMultiply(&g_aMeshDome2[nCntMeshDome2].mtxWorld, &g_aMeshDome2[nCntMeshDome2].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aMeshDome2[nCntMeshDome2].pos.x, g_aMeshDome2[nCntMeshDome2].pos.y, g_aMeshDome2[nCntMeshDome2].pos.z);
		D3DXMatrixMultiply(&g_aMeshDome2[nCntMeshDome2].mtxWorld, &g_aMeshDome2[nCntMeshDome2].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshDome2[nCntMeshDome2].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_aMeshDome2[nCntMeshDome2].pVtxBuff, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_aMeshDome2[nCntMeshDome2].pIdxBuff);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureMeshDome2[0]);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aMeshDome2[nCntMeshDome2].nNumVertex, 0, g_aMeshDome2[nCntMeshDome2].nNumPolygon);
	}
	// ���C�g�I��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���_���̍쐬
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexMeshDome2(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// ���_�o�b�t�@�𐶐�
		// ���_���̍쐬
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshDome2[nCntMeshDome2].nNumVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_aMeshDome2[nCntMeshDome2].pVtxBuff,
			NULL);

		VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		// ���_���̐ݒ�	���W�̒��ڎw��͂����ł͊�{�I�ɂ͂��Ȃ�(g_pos~~���𑫂������͂��Ȃ�D3DXMatrixTranslation�ł���Ă�
		for (int nCntPos = 0; nCntPos < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntPos++)
		{
			int nInside = 1;
			if (g_aMeshDome2[nCntMeshDome2].bInside == true)
			{
				nInside *= -1;
			}

			pVtx[nCntPos].pos =
				D3DXVECTOR3(float(sin(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * (nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * (cos(-D3DX_PI + ((D3DX_PI / 2.0f) / g_aMeshDome2[nCntMeshDome2].nVBlock) * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius)) * -1.0f,
					float(sin(D3DX_PI- ((D3DX_PI / 2.0f) / g_aMeshDome2[nCntMeshDome2].nVBlock) * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius),
					float(cos(float(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * float(nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)))) * (cos(-D3DX_PI + ((D3DX_PI / 2.0f) / g_aMeshDome2[nCntMeshDome2].nVBlock) * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius))) * -1.0f;

			/*pVtx[nCntPos].pos = 
				D3DXVECTOR3(float(sin(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * (nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius),
				float((g_aMeshDome2[nCntMeshDome2].nVBlock * g_aMeshDome2[nCntMeshDome2].fVertical) - g_aMeshDome2[nCntMeshDome2].fVertical * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))),
				float(cos(float(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * float(nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)))) * g_aMeshDome2[nCntMeshDome2].fRadius));*/
		}

		// �@���̐ݒ�@�@���́u1�v�ɂ��Ȃ��Ă͂Ȃ�Ȃ��H�u1.0�v�ȉ��ɂ��Ȃ���΂Ȃ�Ȃ��H
		for (int nCntNor = 0; nCntNor < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntNor++)
		{
			D3DXVec3Normalize(&pVtx[nCntNor].nor, &pVtx[nCntNor].pos);		// ���K������
		}

		// ���_�J���[�̐ݒ�
		for (int nCntCol = 0; nCntCol < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntCol++)
		{
			pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`���̐ݒ�
		for (int nCntTex = 0; nCntTex < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntTex++)
		{
			pVtx[nCntTex].tex = D3DXVECTOR2(1.0f / 2.0f / g_aMeshDome2[nCntMeshDome2].nHBlock * (nCntTex % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))*2, // �Ō��*2�͂��̏ꂵ�̂�
				1.0f / 2.0f / g_aMeshDome2[nCntMeshDome2].nVBlock * (nCntTex / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)));
		}


		// ���_�o�b�t�@���A�����b�N����
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Unlock();
	}

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �C���f�b�N�X���̍쐬
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeIndexMeshDome2(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// �C���f�b�N�X�o�b�t�@�𐶐�
		// �C���f�b�N�X���̍쐬
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome2[nCntMeshDome2].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,			// ��̐�����2�o�C�g�����g��Ȃ�
			D3DPOOL_MANAGED,
			&g_aMeshDome2[nCntMeshDome2].pIdxBuff,
			NULL);

		WORD *pIdx;				// �C���f�b�N�X�f�[�^�ւ̃|�C���^

		// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
		g_aMeshDome2[nCntMeshDome2].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// �C���f�b�N�X�̐ݒ�
		for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aMeshDome2[nCntMeshDome2].nNumIndex; nCntIdx += 2)
		{
			if ((nCntIdx / 2) % (g_aMeshDome2[nCntMeshDome2].nHBlock + 2) == g_aMeshDome2[nCntMeshDome2].nHBlock + 1)
			{
				pIdx[nCntIdx] = nCntPos - 1;
				pIdx[nCntIdx + 1] = nCntPos + g_aMeshDome2[nCntMeshDome2].nHBlock + 1;
			}
			else
			{
				pIdx[nCntIdx] = nCntPos + g_aMeshDome2[nCntMeshDome2].nHBlock + 1;
				pIdx[nCntIdx + 1] = nCntPos;

				nCntPos++;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		g_aMeshDome2[nCntMeshDome2].pIdxBuff->Unlock();
	}
}
