////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���C�t���� [life.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "life.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "game.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_LIFE_NUM	(7)
#define MAX_LIFE		(100.0f)
#define ANIM			(0.3333f)
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
Life g_aLife[MAX_LIFE_NUM];
LifeData g_aLifeData[LIFETEX_MAX] =
{
	{ NULL,"data/TEXTURE/soul001.png" },		// �v���C���[�̔w�i�̔w�i
	{ NULL,"data/TEXTURE/soul001.png" },		// �v���C���[�̔w�i
	{ NULL,"data/TEXTURE/.png" },				// ���C�t�̗��̍�
	{ NULL,"data/TEXTURE/.jpg" },				// ���C�t�̐ԐF
	{ NULL,"data/TEXTURE/life002.jpg" },		// ���C�t
	{ NULL,"data/TEXTURE/Life_frame02.png" },	// ���C�t�̃t���[��
};
int g_nDrawDownLife;// ���C�t�����炷�ϐ�
int g_nLife;
bool g_bDown;
bool b_col;
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		g_aLife[nCntLife].pVtxBuff = NULL;										// ���_�o�b�t�@�ւ̃|�C���^
		g_aLife[nCntLife].pIdxBuff = NULL;										// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		g_aLife[nCntLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ʒu
		g_aLife[nCntLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ����
		g_aLife[nCntLife].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// �F
		g_aLife[nCntLife].fWidth = 0;											// ��
		g_aLife[nCntLife].fHeight = 0;											// ����
		g_aLife[nCntLife].nBlock_X = 0;											// �u���b�NX
		g_aLife[nCntLife].nBlock_Y = 0;											// �u���b�NY
		g_aLife[nCntLife].nPatternAnim = 0;										// �p�^�[��
		g_aLife[nCntLife].nCounterAnim = 0;										// �J�E���^�[
		g_aLife[nCntLife].nType = 0;											// ���
		g_aLife[nCntLife].bUse = false;											// �g�p����Ă��邩�ǂ���
		g_aLife[nCntLife].bUseDown = false;										// �_�E�������ǂ���
		g_aLife[nCntLife].bDownRed = false;									// �w�i�̐Ԃ����C�t�p
		g_aLife[nCntLife].nDownRedCounter = 0;								// �w�i�̐Ԃ����C�t�p����
		g_aLife[nCntLife].nDrawDown = 0;									// �|���S�����Z�p
	}
	g_nDrawDownLife = 0;
	g_nLife = 3;
	g_bDown = false;
	b_col = false;
	for (int nCntTex = 0; nCntTex < LIFETEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			g_aLifeData[nCntTex].pFileName,						// �t�@�C���̖��O
			&g_aLifeData[nCntTex].pTexture);						// �e�N�X�`���ւ̃|�C���^
	}
	// ���𒆐S�Ƃ��Ă̏������W�E��]�E�e�N�X�`���E���C�t�������E������Ȃ��E�������E�c
	SetLife(D3DXVECTOR3(1005, 455, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), LIFETEX_ICONBG, 1, 1, 250 + 10, 250 + 10, false);
	SetLife(D3DXVECTOR3(1005, 460, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.9f, 1.0f, 1.0f, 1.0f), LIFETEX_ICON, 1, 1, 250, 250, false);

	SetLife(D3DXVECTOR3(550, 625, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), LIFETEX_BG2, 100, 1, 560, 20, false);
	SetLife(D3DXVECTOR3(550, 625, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), LIFETEX_GAGERED, 100, 1, 560, 20, true);
	SetLife(D3DXVECTOR3(550, 625, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LIFETEX_GAGE, 100, 1, 560, 20, true);
	SetLife(D3DXVECTOR3(545, 620, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LIFETEX_BG, 100, 1, 560 + 10, 30, false);

}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitLife(void)
{
	// �e�N�X�`���̔j��
	for (int nCntLife = 0; nCntLife <LIFETEX_MAX; nCntLife++)
	{
		if (g_aLifeData[nCntLife].pTexture != NULL)
		{
			g_aLifeData[nCntLife].pTexture->Release();
			g_aLifeData[nCntLife].pTexture = NULL;
		}
	}
	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		// ���_�o�b�t�@�̔j��
		if (g_aLife[nCntLife].pVtxBuff != NULL)
		{
			g_aLife[nCntLife].pVtxBuff->Release();
			g_aLife[nCntLife].pVtxBuff = NULL;
		}
		// �C���f�b�N�X�o�b�t�@�o�b�t�@�̔j��
		if (g_aLife[nCntLife].pIdxBuff != NULL)
		{
			g_aLife[nCntLife].pIdxBuff->Release();
			g_aLife[nCntLife].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// �f�o�C�X�̎擾
	VERTEX_2D *pVtx;	// ���_���̃|�C���^


	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{
			// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aLife[nCntLife].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			if (g_aLife[nCntLife].bUseDown == true)
			{
				if (g_aLife[nCntLife].nType == LIFETEX_GAGE)
				{
					if (g_bDown == true)
					{// ������
						g_aLife[nCntLife].nDrawDown++;
						if (g_aLife[nCntLife].nDrawDown >= 100 - ((float)g_nLife / MAX_LIFE) * 100)
						{// 100%�� X%�������Ƃ��܂Ō��炷
							g_bDown = false;
							g_aLife[nCntLife - 1].bDownRed = true;		// ���C�t�̑O���ԃ��C�t�Ȃ�-1
							g_aLife[nCntLife - 1].nDownRedCounter = 30; // �ԃ��C�t�����Ԋu�ۊ�
							if (g_nLife <= 0)
							{// ���񂾂�ۊǂȂ�
								g_aLife[nCntLife - 1].nDownRedCounter = 0;
							}
						}
					}
				}
				if (g_aLife[nCntLife].nType == LIFETEX_GAGERED)
				{
					if (g_aLife[nCntLife].bDownRed == true)
					{
						g_aLife[nCntLife].nDownRedCounter--;
						if (g_aLife[nCntLife].nDownRedCounter <= 0)
						{
							g_aLife[nCntLife].nDownRedCounter = 0;
							g_aLife[nCntLife].nDrawDown++;
							if (g_aLife[nCntLife].nDrawDown >= 100 - ((float)g_nLife / MAX_LIFE) * 100)
							{// 100%�� X%�������Ƃ��܂Ō��炷
								g_aLife[nCntLife].bDownRed = false;
							}
						}
					}
				}
			}
			if (g_aLife[nCntLife].nType == LIFETEX_GAGE)
			{
				if (b_col == false)
				{
					g_aLife[nCntLife].col.a -= 0.01f;
					if (g_aLife[nCntLife].col.a <= 0.8f)
					{
						b_col = true;
					}
				}
				else
				{
					g_aLife[nCntLife].col.a += 0.01f;
					if (g_aLife[nCntLife].col.a >= 1.0f)
					{
						b_col = false;
					}
				}
				for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
				{
					pVtx[nCnt].col = g_aLife[nCntLife].col;
				}
				g_aLife[nCntLife].nCounterAnim++;
				if (g_aLife[nCntLife].nCounterAnim % 5 == 0)
				{
					g_aLife[nCntLife].nPatternAnim = (g_aLife[nCntLife].nPatternAnim + 1) % 3;
					for (int nCntV = 0; nCntV < g_aLife[nCntLife].nBlock_Y + 1; nCntV++)
					{// �e�N�X�`���̐ݒ�
						for (int nCntH = 0; nCntH < g_aLife[nCntLife].nBlock_X + 1; nCntH++)
						{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
						 // �e�N�X�`���̐ݒ�
							pVtx[nCntH + (g_aLife[nCntLife].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aLife[nCntLife].nBlock_X) / 100.0f, float(nCntV * 100 / g_aLife[nCntLife].nBlock_Y) / 100.0f + (ANIM*g_aLife[nCntLife].nPatternAnim));
						}
					}
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aLife[nCntLife].pVtxBuff->Unlock();
		}
	}
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

												// ���C���[�t���[��
												//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aLife[nCntLife].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLife[nCntLife].rot.y, g_aLife[nCntLife].rot.x, g_aLife[nCntLife].rot.z);
			D3DXMatrixMultiply(&g_aLife[nCntLife].mtxWorld, &g_aLife[nCntLife].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aLife[nCntLife].pos.x, g_aLife[nCntLife].pos.y, g_aLife[nCntLife].pos.z);
			D3DXMatrixMultiply(&g_aLife[nCntLife].mtxWorld, &g_aLife[nCntLife].mtxWorld, &mtxTrans);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_aLife[nCntLife].pVtxBuff, 0, sizeof(VERTEX_2D));
			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_aLife[nCntLife].pIdxBuff);
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aLifeData[g_aLife[nCntLife].nType].pTexture);
			if (g_aLife[nCntLife].bUseDown == true)
			{
				// �|���S���̕`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aLife[nCntLife].nNumIndex, 0, g_aLife[nCntLife].nNumPolygon - g_aLife[nCntLife].nDrawDown * 2);
			}
			else
			{
				// �|���S���̕`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aLife[nCntLife].nNumIndex, 0, g_aLife[nCntLife].nNumPolygon);

			}
		}
	}

}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���̃|�C���^
	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == false)
		{
			// ���_���̍쐬
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * g_aLife[nCntLife].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_aLife[nCntLife].pVtxBuff,
				NULL);


			// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾

			g_aLife[nCntLife].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// ���_���W�̐ݒ�

			for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
			{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				pVtx[nCnt].pos = D3DXVECTOR3((g_aLife[nCntLife].fWidth / g_aLife[nCntLife].nBlock_X)*(nCnt % (g_aLife[nCntLife].nBlock_X + 1)),
					+(g_aLife[nCntLife].fHeight)*(nCnt / (g_aLife[nCntLife].nBlock_X + 1)), 0.0f) + g_aLife[nCntLife].pos;

			}
			// rhw�̐ݒ�
			for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].rhw = 1.0f;
			}
			// ���_�J���[�̐ݒ�
			for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aLife[nCntLife].col;
			}

			// �S�̃e�N�X�`��
			for (int nCntV = 0; nCntV < g_aLife[nCntLife].nBlock_Y + 1; nCntV++)
			{// �e�N�X�`���̐ݒ�
				for (int nCntH = 0; nCntH < g_aLife[nCntLife].nBlock_X + 1; nCntH++)
				{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				 // �e�N�X�`���̐ݒ�
					pVtx[nCntH + (g_aLife[nCntLife].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aLife[nCntLife].nBlock_X) / 100.0f, float(nCntV * 100 / g_aLife[nCntLife].nBlock_Y) / 100.0f);
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aLife[nCntLife].pVtxBuff->Unlock();
			WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

						// �C���f�b�N�X�o�b�t�@�̍쐬
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aLife[nCntLife].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aLife[nCntLife].pIdxBuff,
				NULL);

			// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aLife[nCntLife].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


			// �C���f�b�N�X�̐ݒ�
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aLife[nCntLife].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aLife[nCntLife].nBlock_X + 2) == g_aLife[nCntLife].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aLife[nCntLife].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aLife[nCntLife].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aLife[nCntLife].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// ���b�V���̐ݒ�
//==============================================================================================================================
void SetLife(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bUseDown)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == false)
		{
			g_aLife[nCntLife].pos = pos;				// �ʒu
			g_aLife[nCntLife].rot = rot;				// ����
			g_aLife[nCntLife].col = col;				// �F
			g_aLife[nCntLife].fWidth = fWidth;			// ��
			g_aLife[nCntLife].fHeight = fHeight;		// ���s��
			g_aLife[nCntLife].nBlock_X = nBlock_X;		// �u���b�NX
			g_aLife[nCntLife].nBlock_Y = nBlock_Y;		// �u���b�NZ
			g_aLife[nCntLife].nType = nType;			// ���
			g_aLife[nCntLife].bUseDown = bUseDown;
			g_aLife[nCntLife].nNumVertex = (g_aLife[nCntLife].nBlock_X + 1)*(g_aLife[nCntLife].nBlock_Y + 1);											// ���_��
			g_aLife[nCntLife].nNumIndex = 2 * (g_aLife[nCntLife].nBlock_X*g_aLife[nCntLife].nBlock_Y) + 4 * (g_aLife[nCntLife].nBlock_Y - 1) + 2;	// �C���f�b�N�X��										// 
			g_aLife[nCntLife].nNumPolygon = 2 * (g_aLife[nCntLife].nBlock_X*g_aLife[nCntLife].nBlock_Y) + 4 * (g_aLife[nCntLife].nBlock_Y - 1);	// �|���S����

			MakeVertexLife(pDevice);
			g_aLife[nCntLife].bUse = true;										// �g�p����Ă��邩�ǂ���
			break;
		}
	}
}
//==============================================================================================================================
// ���C�t�̊l��
//==============================================================================================================================
Life *GetLife(void)
{
	return &g_aLife[0];
}
LifeData *GetLifeData(void)
{
	return &g_aLifeData[0];
}
//==============================================================================================================================
// ���C�t�̏����ݒ�
//==============================================================================================================================
void SetPlayerLife(int nLife)
{
	g_nLife = nLife;
}
//==============================================================================================================================
// ���C�t�̕ύX
//==============================================================================================================================
void GetPlayerLife(int nLife)
{
	g_nLife = nLife;
	g_bDown = true;
}