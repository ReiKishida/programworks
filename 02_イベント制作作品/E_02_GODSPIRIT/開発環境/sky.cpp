////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��i���́j���� [sky.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sky.h"
#include "camera.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_SKY	(1)
#define BLOCK_X	(50)
#define BLOCK_Y	(30)
#define VERTEX_WIDTH	(2+2*BLOCK_X)
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ���S���W
	D3DXVECTOR3 rot;			// ����
	SKYTYPE nType;		// ���
	float fWidth;				// ��
	float fHeight;				// ����
	float fDepth;				// ���s��
	float fLength;				// ���a
} SkyInfo;
typedef struct
{
	char *pFileName;
}SkyTexture;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexSky(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSky = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_apTextureSky[SKYTYPE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSky = NULL;						// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Sky g_aSky[(BLOCK_X + 1) * (BLOCK_Y + 1)];

int g_nNumVertexSky;	// ���_��
int g_nNumIndexSky;	// �C���f�b�N�X��
int g_nNumPolygonSky;	// �|���S����
float g_fTexmove;
SkyInfo g_aSkyInfo[] =
{
	{ D3DXVECTOR3(0.0f, -300.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SKYTYPE_0, 200.0f,200.0f, 0.0f,20000.0f },
};
SkyTexture g_aSkyTexInfo[SKYTYPE_MAX] =
{
	{ "data/TEXTURE/sky001.jpg" },
};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	
	for (int nCntSkyTex = 0; nCntSkyTex < SKYTYPE_MAX; nCntSkyTex++)
	{// �e�N�X�`���̐ݒ�
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			g_aSkyTexInfo[nCntSkyTex].pFileName,	// �t�@�C���̖��O
			&g_apTextureSky[nCntSkyTex]);			// �e�N�X�`���ւ̃|�C���^
	}
	g_nNumVertexSky = (BLOCK_X + 1) * (BLOCK_Y + 1);					// ���_��
	g_nNumIndexSky = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1) + 2;	// �C���f�b�N�X��
	g_nNumPolygonSky = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1);	// �|���S����

	for (int nCntSky = 0; nCntSky < g_nNumVertexSky; nCntSky++)
	{
		g_aSky[nCntSky].pos = g_aSkyInfo[nCntSky].pos;			// �ʒu
		g_aSky[nCntSky].rot = g_aSkyInfo[nCntSky].rot;			// ����
		g_aSky[nCntSky].fWidth = g_aSkyInfo[nCntSky].fWidth;	// ��
		g_aSky[nCntSky].fHeight = g_aSkyInfo[nCntSky].fHeight;	// ����
		g_aSky[nCntSky].fDepth = g_aSkyInfo[nCntSky].fDepth;	// ���s��
		g_aSky[nCntSky].fLength = g_aSkyInfo[nCntSky].fLength;	// ���a
		g_aSky[nCntSky].nType = g_aSkyInfo[nCntSky].nType;		// ���
		g_aSky[nCntSky].bUse = true;										// �g�p����Ă��邩�ǂ���

	}
	g_fTexmove = 0;
	MakeVertexSky(pDevice);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitSky(void)
{
	// �e�N�X�`���̔j��
	for (int nCntSky = 0; nCntSky < SKYTYPE_MAX; nCntSky++)
	{
		if (g_apTextureSky[nCntSky] != NULL)
		{
			g_apTextureSky[nCntSky]->Release();
			g_apTextureSky[nCntSky] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffSky != NULL)
	{
		g_pVtxBuffSky->Release();
		g_pVtxBuffSky = NULL;
	}
	// �C���f�b�N�X�o�b�t�@�o�b�t�@�̔j��
	if (g_pIdxBuffSky != NULL)
	{
		g_pIdxBuffSky->Release();
		g_pIdxBuffSky = NULL;
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateSky(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSky->Lock(0, 0, (void**)&pVtx, 0);
	g_fTexmove += 0.0003f;
	if (g_fTexmove >= 0.5f)
	{
		g_fTexmove = 0.0f;
	}
	for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
	{// �e�N�X�`���̐ݒ�
		for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
		{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
		 // �e�N�X�`���̐ݒ�
			pVtx[nCntH + (BLOCK_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / BLOCK_X) / 100.0f + g_fTexmove, float(nCntV * 100 / BLOCK_Y) / 100.0f);
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pIdxBuffSky->Unlock();
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawSky(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxProjection;				// �v�Z�p�}�g���b�N�X
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	for (int nCntSky = 0; nCntSky < MAX_SKY; nCntSky++)
	{
		if (g_aSky[nCntSky].bUse == true) 
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSky[nCntSky].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSky[nCntSky].rot.y, g_aSky[nCntSky].rot.x, g_aSky[nCntSky].rot.z);
			D3DXMatrixMultiply(&g_aSky[nCntSky].mtxWorld, &g_aSky[nCntSky].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aSky[nCntSky].pos.x, g_aSky[nCntSky].pos.y, g_aSky[nCntSky].pos.z);
			D3DXMatrixMultiply(&g_aSky[nCntSky].mtxWorld, &g_aSky[nCntSky].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSky[nCntSky].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffSky, 0, sizeof(VERTEX_3D));
			
			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffSky);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureSky[g_aSky[nCntSky].nType]);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexSky, 0, g_nNumPolygonSky);

		}
	}
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexSky(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexSky * MAX_SKY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSky,
		NULL);
	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD)*g_nNumIndexSky * MAX_SKY,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSky,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffSky->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntSky = 0; nCntSky < MAX_SKY; nCntSky++, pVtx += g_nNumVertexSky)
	{
		
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				// ���_���W�̐ݒ�
				pVtx[nCntH + (BLOCK_X+1)*nCntV].pos = D3DXVECTOR3(sinf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*(sinf(0.1f + D3DX_PI/2/BLOCK_Y*nCntV)*g_aSky[nCntSky].fLength),
					 (cosf(0 + D3DX_PI / 2 / BLOCK_Y*nCntV)*g_aSky[nCntSky].fHeight*BLOCK_Y),
					cosf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*(sinf(0.1f + D3DX_PI/2 / BLOCK_Y*nCntV)*g_aSky[nCntSky].fLength));
			}
		}
		
		for (int nCnt = 0; nCnt < g_nNumVertexSky; nCnt++)
		{
			// �@���̐ݒ�
			pVtx[nCnt].nor = pVtx[nCnt].pos / g_aSky[nCntSky].fLength;
		}
		for (int nCnt = 0; nCnt < g_nNumVertexSky; nCnt++)
		{
			// ���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{// �e�N�X�`���̐ݒ�
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
			// �e�N�X�`���̐ݒ�
				pVtx[nCntH + (BLOCK_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH*100/BLOCK_X)/100.0f, float(nCntV*100 / BLOCK_Y)/100.0f);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSky->Unlock();

	WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffSky->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIdxSky = 0; nCntIdxSky < MAX_SKY; nCntIdxSky++, pIdx += g_nNumIndexSky)
	{// �C���f�b�N�X�̐ݒ�
		int nCnt = 0;
		// ���_���W�̐ݒ�
		for (int nCntX = 0; nCntX < g_nNumIndexSky; nCntX += 2)
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
	g_pIdxBuffSky->Unlock();
}
