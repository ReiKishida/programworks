////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �X�s���b�g���� [spirit.cpp]
// Author : �� �a��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "spirit.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "game.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_SPIRIT_NUM	(3)
#define MAX_SPIRIT		(100.0f)
#define SPIRIT_ANIM		(0.3333f)
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexSpirit(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
Spirit g_aSpirit[MAX_SPIRIT_NUM];
SpiritData g_aSpiritData[SPIRITTEX_MAX] =
{
	{ NULL,"data/TEXTURE/.png" },				// �X�s���b�g�̗��̍�
	{ NULL,"data/TEXTURE/spirit002.jpg" },		// �X�s���b�g
	{ NULL,"data/TEXTURE/Spirit_frame02.png" },	// �X�s���b�g�̃t���[��
};
int g_nDrawDownSpirit;// �X�s���b�g�����炷�ϐ�
int g_nSpirit;
bool g_bSpiritDown;
bool g_bSpiritUp;
bool b_SpiritCol;
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitSpirit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		g_aSpirit[nCntSpirit].pVtxBuff = NULL;										// ���_�o�b�t�@�ւ̃|�C���^
		g_aSpirit[nCntSpirit].pIdxBuff = NULL;										// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		g_aSpirit[nCntSpirit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ʒu
		g_aSpirit[nCntSpirit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ����
		g_aSpirit[nCntSpirit].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// �F
		g_aSpirit[nCntSpirit].fWidth = 0;											// ��
		g_aSpirit[nCntSpirit].fHeight = 0;											// ����
		g_aSpirit[nCntSpirit].nBlock_X = 0;											// �u���b�NX
		g_aSpirit[nCntSpirit].nBlock_Y = 0;											// �u���b�NY
		g_aSpirit[nCntSpirit].nPatternAnim = 0;										// �p�^�[��
		g_aSpirit[nCntSpirit].nCounterAnim = 0;										// �J�E���^�[
		g_aSpirit[nCntSpirit].nType = 0;											// ���
		g_aSpirit[nCntSpirit].bUse = false;											// �g�p����Ă��邩�ǂ���
		g_aSpirit[nCntSpirit].bUseDown = false;										// �_�E�������ǂ���
		g_aSpirit[nCntSpirit].nDrawDown = 0;									// �|���S�����Z�p
	}
	g_nDrawDownSpirit = 0;
	g_nSpirit = 100;
	g_bSpiritDown = false;
	g_bSpiritUp = false;
	b_SpiritCol = false;
	for (int nCntTex = 0; nCntTex < SPIRITTEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			g_aSpiritData[nCntTex].pFileName,						// �t�@�C���̖��O
			&g_aSpiritData[nCntTex].pTexture);						// �e�N�X�`���ւ̃|�C���^
	}
	// ���𒆐S�Ƃ��Ă̏������W�E��]�E�e�N�X�`���E�X�s���b�g�������E������Ȃ��E�������E�c
	SetSpirit(D3DXVECTOR3(550, 655, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SPIRITTEX_BG2, 100, 1, 560, 15, false);
	SetSpirit(D3DXVECTOR3(550, 655, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SPIRITTEX_GAGE, 100, 1, 560, 15, true);
	SetSpirit(D3DXVECTOR3(545, 650, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SPIRITTEX_BG, 100, 1, 560 + 10, 25, false);

}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitSpirit(void)
{
	// �e�N�X�`���̔j��
	for (int nCntSpirit = 0; nCntSpirit <SPIRITTEX_MAX; nCntSpirit++)
	{
		if (g_aSpiritData[nCntSpirit].pTexture != NULL)
		{
			g_aSpiritData[nCntSpirit].pTexture->Release();
			g_aSpiritData[nCntSpirit].pTexture = NULL;
		}
	}
	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		// ���_�o�b�t�@�̔j��
		if (g_aSpirit[nCntSpirit].pVtxBuff != NULL)
		{
			g_aSpirit[nCntSpirit].pVtxBuff->Release();
			g_aSpirit[nCntSpirit].pVtxBuff = NULL;
		}
		// �C���f�b�N�X�o�b�t�@�o�b�t�@�̔j��
		if (g_aSpirit[nCntSpirit].pIdxBuff != NULL)
		{
			g_aSpirit[nCntSpirit].pIdxBuff->Release();
			g_aSpirit[nCntSpirit].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateSpirit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// �f�o�C�X�̎擾
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == true)
		{
			// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aSpirit[nCntSpirit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			if (g_aSpirit[nCntSpirit].bUseDown == true)
			{
				if (g_aSpirit[nCntSpirit].nType == SPIRITTEX_GAGE)
				{
					if (g_aSpirit[nCntSpirit].nDrawDown > int(100 - ((float)g_nSpirit / MAX_SPIRIT) * 100))
					{// 100%�� X%�������Ƃ��܂Ō��炷
						g_aSpirit[nCntSpirit].nDrawDown--;
					}
					else if (g_aSpirit[nCntSpirit].nDrawDown < int(100 - ((float)g_nSpirit / MAX_SPIRIT) * 100))
					{
						g_aSpirit[nCntSpirit].nDrawDown++;
					}
				}
			}
			if (g_aSpirit[nCntSpirit].nType == SPIRITTEX_GAGE)
			{
				if (b_SpiritCol == false)
				{
					g_aSpirit[nCntSpirit].col.a -= 0.01f;
					if (g_aSpirit[nCntSpirit].col.a <= 0.8f)
					{
						b_SpiritCol = true;
					}
				}
				else
				{
					g_aSpirit[nCntSpirit].col.a += 0.01f;
					if (g_aSpirit[nCntSpirit].col.a >= 1.0f)
					{
						b_SpiritCol = false;
					}
				}
				for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
				{
					pVtx[nCnt].col = g_aSpirit[nCntSpirit].col;
				}
				g_aSpirit[nCntSpirit].nCounterAnim++;
				if (g_aSpirit[nCntSpirit].nCounterAnim % 5 == 0)
				{
					g_aSpirit[nCntSpirit].nPatternAnim = (g_aSpirit[nCntSpirit].nPatternAnim + 1) % 3;
					for (int nCntV = 0; nCntV < g_aSpirit[nCntSpirit].nBlock_Y + 1; nCntV++)
					{// �e�N�X�`���̐ݒ�
						for (int nCntH = 0; nCntH < g_aSpirit[nCntSpirit].nBlock_X + 1; nCntH++)
						{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
						 // �e�N�X�`���̐ݒ�
							pVtx[nCntH + (g_aSpirit[nCntSpirit].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aSpirit[nCntSpirit].nBlock_X) / 100.0f, float(nCntV * 100 / g_aSpirit[nCntSpirit].nBlock_Y) / 100.0f + (SPIRIT_ANIM*g_aSpirit[nCntSpirit].nPatternAnim));
						}
					}
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aSpirit[nCntSpirit].pVtxBuff->Unlock();
		}
	}
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawSpirit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

												// ���C���[�t���[��
												//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSpirit[nCntSpirit].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSpirit[nCntSpirit].rot.y, g_aSpirit[nCntSpirit].rot.x, g_aSpirit[nCntSpirit].rot.z);
			D3DXMatrixMultiply(&g_aSpirit[nCntSpirit].mtxWorld, &g_aSpirit[nCntSpirit].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aSpirit[nCntSpirit].pos.x, g_aSpirit[nCntSpirit].pos.y, g_aSpirit[nCntSpirit].pos.z);
			D3DXMatrixMultiply(&g_aSpirit[nCntSpirit].mtxWorld, &g_aSpirit[nCntSpirit].mtxWorld, &mtxTrans);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_aSpirit[nCntSpirit].pVtxBuff, 0, sizeof(VERTEX_2D));
			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_aSpirit[nCntSpirit].pIdxBuff);
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aSpiritData[g_aSpirit[nCntSpirit].nType].pTexture);
			if (g_aSpirit[nCntSpirit].bUseDown == true)
			{
				// �|���S���̕`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aSpirit[nCntSpirit].nNumIndex, 0, g_aSpirit[nCntSpirit].nNumPolygon - g_aSpirit[nCntSpirit].nDrawDown * 2);
			}
			else
			{
				// �|���S���̕`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aSpirit[nCntSpirit].nNumIndex, 0, g_aSpirit[nCntSpirit].nNumPolygon);

			}
		}
	}

}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexSpirit(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���̃|�C���^
	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == false)
		{
			// ���_���̍쐬
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * g_aSpirit[nCntSpirit].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_aSpirit[nCntSpirit].pVtxBuff,
				NULL);


			// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾

			g_aSpirit[nCntSpirit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// ���_���W�̐ݒ�

			for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
			{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				pVtx[nCnt].pos = D3DXVECTOR3((g_aSpirit[nCntSpirit].fWidth / g_aSpirit[nCntSpirit].nBlock_X)*(nCnt % (g_aSpirit[nCntSpirit].nBlock_X + 1)),
					+(g_aSpirit[nCntSpirit].fHeight)*(nCnt / (g_aSpirit[nCntSpirit].nBlock_X + 1)), 0.0f) + g_aSpirit[nCntSpirit].pos;

			}
			// rhw�̐ݒ�
			for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
			{
				pVtx[nCnt].rhw = 1.0f;
			}
			// ���_�J���[�̐ݒ�
			for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aSpirit[nCntSpirit].col;
			}

			// �S�̃e�N�X�`��
			for (int nCntV = 0; nCntV < g_aSpirit[nCntSpirit].nBlock_Y + 1; nCntV++)
			{// �e�N�X�`���̐ݒ�
				for (int nCntH = 0; nCntH < g_aSpirit[nCntSpirit].nBlock_X + 1; nCntH++)
				{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				 // �e�N�X�`���̐ݒ�
					pVtx[nCntH + (g_aSpirit[nCntSpirit].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aSpirit[nCntSpirit].nBlock_X) / 100.0f, float(nCntV * 100 / g_aSpirit[nCntSpirit].nBlock_Y) / 100.0f);
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aSpirit[nCntSpirit].pVtxBuff->Unlock();
			WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

						// �C���f�b�N�X�o�b�t�@�̍쐬
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aSpirit[nCntSpirit].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aSpirit[nCntSpirit].pIdxBuff,
				NULL);

			// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aSpirit[nCntSpirit].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


			// �C���f�b�N�X�̐ݒ�
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aSpirit[nCntSpirit].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aSpirit[nCntSpirit].nBlock_X + 2) == g_aSpirit[nCntSpirit].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aSpirit[nCntSpirit].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aSpirit[nCntSpirit].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aSpirit[nCntSpirit].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// ���b�V���̐ݒ�
//==============================================================================================================================
void SetSpirit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bUseDown)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == false)
		{
			g_aSpirit[nCntSpirit].pos = pos;				// �ʒu
			g_aSpirit[nCntSpirit].rot = rot;				// ����
			g_aSpirit[nCntSpirit].col = col;				// �F
			g_aSpirit[nCntSpirit].fWidth = fWidth;			// ��
			g_aSpirit[nCntSpirit].fHeight = fHeight;		// ���s��
			g_aSpirit[nCntSpirit].nBlock_X = nBlock_X;		// �u���b�NX
			g_aSpirit[nCntSpirit].nBlock_Y = nBlock_Y;		// �u���b�NZ
			g_aSpirit[nCntSpirit].nType = nType;			// ���
			g_aSpirit[nCntSpirit].bUseDown = bUseDown;
			g_aSpirit[nCntSpirit].nNumVertex = (g_aSpirit[nCntSpirit].nBlock_X + 1)*(g_aSpirit[nCntSpirit].nBlock_Y + 1);											// ���_��
			g_aSpirit[nCntSpirit].nNumIndex = 2 * (g_aSpirit[nCntSpirit].nBlock_X*g_aSpirit[nCntSpirit].nBlock_Y) + 4 * (g_aSpirit[nCntSpirit].nBlock_Y - 1) + 2;	// �C���f�b�N�X��										// 
			g_aSpirit[nCntSpirit].nNumPolygon = 2 * (g_aSpirit[nCntSpirit].nBlock_X*g_aSpirit[nCntSpirit].nBlock_Y) + 4 * (g_aSpirit[nCntSpirit].nBlock_Y - 1);	// �|���S����

			MakeVertexSpirit(pDevice);
			g_aSpirit[nCntSpirit].bUse = true;										// �g�p����Ă��邩�ǂ���
			break;
		}
	}
}
//==============================================================================================================================
// �X�s���b�g�̊l��
//==============================================================================================================================
Spirit *GetSpirit(void)
{
	return &g_aSpirit[0];
}
SpiritData *GetSpiritData(void)
{
	return &g_aSpiritData[0];
}
//==============================================================================================================================
// �X�s���b�g�̏����ݒ�
//==============================================================================================================================
void SetPlayerSpirit(int nSpirit)
{
	g_nSpirit = nSpirit;
}
//==============================================================================================================================
// �X�s���b�g�̕ύX
//==============================================================================================================================
void GetPlayerSpirit(int nSpirit)
{
	g_nSpirit = nSpirit;	
}