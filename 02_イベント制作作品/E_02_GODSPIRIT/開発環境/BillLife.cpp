////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �r���{�[�h���C�t���� [BillLife.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "BIllLife.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_BILLLIFE	(520)
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexBillLife(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
BillLife g_aBillLife[MAX_BILLLIFE];
BillLifeData g_aBillLifeData[BILLLIFETYPE_MAX] = 
{
	{ NULL,"data/TEXTURE/.jpeg" },
	{ NULL,"data/TEXTURE/.jpeg" },
	{ NULL,"data/TEXTURE/life.jpeg" },
};
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitBillLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		g_aBillLife[nCntBillLife].pVtxBuff = NULL;									// ���_�o�b�t�@�ւ̃|�C���^
		g_aBillLife[nCntBillLife].pIdxBuff = NULL;									// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		g_aBillLife[nCntBillLife].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);				// �ʒu
		g_aBillLife[nCntBillLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
		g_aBillLife[nCntBillLife].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �F
		g_aBillLife[nCntBillLife].fWidth = 0;										// ��
		g_aBillLife[nCntBillLife].fHeight = 0;										// ����
		g_aBillLife[nCntBillLife].nBlock_X = 1;										// �u���b�NX
		g_aBillLife[nCntBillLife].nBlock_Y = 1;										// �u���b�NY
		g_aBillLife[nCntBillLife].nType = BILLLIFETYPE_ENEMY;						// ���
		g_aBillLife[nCntBillLife].fMoveVtx = 0;										// ���_���Ƃ̍���
		g_aBillLife[nCntBillLife].fTexmoveMesh = 0;									// �e�N�X�`���̈ړ����x
		g_aBillLife[nCntBillLife].nLife = 0;										// ���C�t
		g_aBillLife[nCntBillLife].nInitLife = 0;									// �������C�t
		g_aBillLife[nCntBillLife].bDown = false;									// �_�E�������ǂ���
		g_aBillLife[nCntBillLife].bUseDown = false;									// �_�E�����邩�ǂ���
		g_aBillLife[nCntBillLife].bDownRed = false;									// �w�i�̐Ԃ����C�t�p
		g_aBillLife[nCntBillLife].nDownRedCounter = 0;								// �w�i�̐Ԃ����C�t�p����
		g_aBillLife[nCntBillLife].nDrawDown = 0;									// �|���S�����Z�p
		g_aBillLife[nCntBillLife].nIdy = 0;											// �ԍ�
		g_aBillLife[nCntBillLife].fOffset = 0;										// �I�t�Z�b�g
		g_aBillLife[nCntBillLife].nCntTime = 0;										// ���S�����Ƃ����C�t��������^�C�~���O
		g_aBillLife[nCntBillLife].bUse = false;										// �g�p����Ă��邩�ǂ���
	}
	for (int nCntTex = 0; nCntTex < BILLLIFETYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			g_aBillLifeData[nCntTex].pFileName,					// �t�@�C���̖��O
			&g_aBillLifeData[nCntTex].pTexture);				// �e�N�X�`���ւ̃|�C���^
	}
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitBillLife(void)
{
	// �e�N�X�`���̔j��
	for (int nCntBillLife = 0; nCntBillLife < BILLLIFETYPE_MAX; nCntBillLife++)
	{
		if (g_aBillLifeData[nCntBillLife].pTexture!= NULL)
		{
			g_aBillLifeData[nCntBillLife].pTexture->Release();
			g_aBillLifeData[nCntBillLife].pTexture = NULL;
		}
	}
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		// ���_�o�b�t�@�̔j��
		if (g_aBillLife[nCntBillLife].pVtxBuff != NULL)
		{
			g_aBillLife[nCntBillLife].pVtxBuff->Release();
			g_aBillLife[nCntBillLife].pVtxBuff = NULL;
		}
		// �C���f�b�N�X�o�b�t�@�o�b�t�@�̔j��
		if (g_aBillLife[nCntBillLife].pIdxBuff != NULL)
		{
			g_aBillLife[nCntBillLife].pIdxBuff->Release();
			g_aBillLife[nCntBillLife].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateBillLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// �f�o�C�X�̎擾

	for (int nCntLife = 0; nCntLife < MAX_BILLLIFE; nCntLife++)
	{
		if (g_aBillLife[nCntLife].bUse == true)
		{
			if (g_aBillLife[nCntLife].bUseDown == true)
			{
				if (g_aBillLife[nCntLife].nType == BILLLIFETYPE_ENEMY)
				{
					if (g_aBillLife[nCntLife].bDown == true)
					{// ������
						if (g_aBillLife[nCntLife].nInitLife <= 5)
						{// ���C�t���T�ȉ��̏ꍇ���x�𑬂߂�			
							g_aBillLife[nCntLife].nDrawDown += 3;
						}
						else
						{
							g_aBillLife[nCntLife].nDrawDown++;
						}
						if (g_aBillLife[nCntLife].nDrawDown >= 100 - ((float)g_aBillLife[nCntLife].nLife / g_aBillLife[nCntLife].nInitLife) * 100)
						{// 100%�� X%�������Ƃ��܂Ō��炷
							g_aBillLife[nCntLife].bDown = false;
							g_aBillLife[nCntLife - 1].bDownRed = true;		// ���C�t�̑O���ԃ��C�t�Ȃ�-1
							g_aBillLife[nCntLife - 1].nDownRedCounter = 30; // �ԃ��C�t�����Ԋu�ۊ�
							if (g_aBillLife[nCntLife].nLife <= 0)
							{// ���񂾂�ۊǂȂ�
								g_aBillLife[nCntLife - 1].nDownRedCounter = 0;
							}
						}
					}
				}
				if (g_aBillLife[nCntLife].nType == BILLLIFETYPE_ENEMYBGRED)
				{
					if (g_aBillLife[nCntLife].bDownRed == true)
					{
						g_aBillLife[nCntLife].nDownRedCounter--;
						if (g_aBillLife[nCntLife].nDownRedCounter <= 0)
						{
							g_aBillLife[nCntLife].nDownRedCounter = 0;
							if (g_aBillLife[nCntLife].nInitLife <= 5)
							{// ���C�t���T�ȉ��̏ꍇ���x�𑬂߂�
								g_aBillLife[nCntLife].nDrawDown += 3;
							}
							else
							{
								g_aBillLife[nCntLife].nDrawDown++;
							}
							if (g_aBillLife[nCntLife].nDrawDown >= 100 - ((float)g_aBillLife[nCntLife].nLife / g_aBillLife[nCntLife].nInitLife) * 100)
							{// 100%�� X%�������Ƃ��܂Ō��炷
								g_aBillLife[nCntLife].bDown = false;
								g_aBillLife[nCntLife].bDownRed = false;
							}
						}
					}
				}
			}
			if (g_aBillLife[nCntLife].nLife <= 0)
			{
				g_aBillLife[nCntLife].nCntTime++;
				if (g_aBillLife[nCntLife].nCntTime >= 30)
				{
					g_aBillLife[nCntLife].bUse = false;
					g_aBillLife[nCntLife].bUse = false;
				}
			}
		}
	}
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawBillLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���{�[�h�p�}�g���b�N�X
	// ���e�X�g��ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���l��0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���傫��
	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Z�o�b�t�@�ւ̏�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);				// ���u�����h���s��

	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nLife != g_aBillLife[nCntBillLife].nInitLife)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aBillLife[nCntBillLife].mtxWorld);
				// �r���{�[�h�̐ݒ�
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);
				// �t�s��̐ݒ�
				g_aBillLife[nCntBillLife].mtxWorld._11 = mtxView._11;
				g_aBillLife[nCntBillLife].mtxWorld._12 = mtxView._21;
				g_aBillLife[nCntBillLife].mtxWorld._13 = mtxView._31;
				g_aBillLife[nCntBillLife].mtxWorld._21 = mtxView._12;
				g_aBillLife[nCntBillLife].mtxWorld._22 = mtxView._22;
				g_aBillLife[nCntBillLife].mtxWorld._23 = mtxView._32;
				g_aBillLife[nCntBillLife].mtxWorld._31 = mtxView._13;
				g_aBillLife[nCntBillLife].mtxWorld._32 = mtxView._23;
				g_aBillLife[nCntBillLife].mtxWorld._33 = mtxView._33;
				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aBillLife[nCntBillLife].pos.x, g_aBillLife[nCntBillLife].pos.y, g_aBillLife[nCntBillLife].pos.z);
				D3DXMatrixMultiply(&g_aBillLife[nCntBillLife].mtxWorld, &g_aBillLife[nCntBillLife].mtxWorld, &mtxTrans);
				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aBillLife[nCntBillLife].mtxWorld);
				// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_aBillLife[nCntBillLife].pVtxBuff, 0, sizeof(VERTEX_3D));
				// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetIndices(g_aBillLife[nCntBillLife].pIdxBuff);
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aBillLifeData[g_aBillLife[nCntBillLife].nType].pTexture);
				// �|���S���̕`��
				if (g_aBillLife[nCntBillLife].bUseDown == true)
				{
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aBillLife[nCntBillLife].nNumIndex, 0, g_aBillLife[nCntBillLife].nNumPolygon - g_aBillLife[nCntBillLife].nDrawDown * 2);
				}
				else
				{
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aBillLife[nCntBillLife].nNumIndex, 0, g_aBillLife[nCntBillLife].nNumPolygon);
				}
			}
		}
	}
	// ���e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���
																// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Z�o�b�t�@�ւ̏�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��

}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexBillLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == false)
		{
			// ���_���̍쐬
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aBillLife[nCntBillLife].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aBillLife[nCntBillLife].pVtxBuff,
				NULL);

			// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aBillLife[nCntBillLife].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// ���_���W�̐ݒ�
			//for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			//{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
			//	pVtx[nCnt].pos = D3DXVECTOR3(-(g_aBillLife[nCntBillLife].fWidth  * g_aBillLife[nCntBillLife].nBlock_X / 2) + (g_aBillLife[nCntBillLife].fWidth )*(nCnt % (g_aBillLife[nCntBillLife].nBlock_X + 1)),
			//		(g_aBillLife[nCntBillLife].fHeight * g_aBillLife[nCntBillLife].nBlock_Y) - (g_aBillLife[nCntBillLife].fHeight)*(nCnt / (g_aBillLife[nCntBillLife].nBlock_X + 1)), 0.0f);
			//}

			for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				pVtx[nCnt].pos = D3DXVECTOR3(-(g_aBillLife[nCntBillLife].fWidth/2) + (g_aBillLife[nCntBillLife].fWidth / g_aBillLife[nCntBillLife].nBlock_X)*(nCnt % (g_aBillLife[nCntBillLife].nBlock_X + 1)),
					(g_aBillLife[nCntBillLife].fHeight / 2) -(g_aBillLife[nCntBillLife].fHeight)*(nCnt / (g_aBillLife[nCntBillLife].nBlock_X + 1)), 0.0f) ;
			}
			// �@���̐ݒ�
			for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			}
			// ���_�J���[�̐ݒ�
			for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aBillLife[nCntBillLife].col;
			}
			// �u���b�N���ƂɈꖇ�z�u����e�N�X�`��
			//for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			//{
			//	// �e�N�X�`���̐ݒ�
			//	pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(BLOCK_X+1)), 0.0f + (nCnt / (BLOCK_X + 1)));
			//}

			// �����e�N�X�`��
			for (int nCntV = 0; nCntV < g_aBillLife[nCntBillLife].nBlock_Y + 1; nCntV++)
			{// �e�N�X�`���̐ݒ�
				for (int nCntH = 0; nCntH < g_aBillLife[nCntBillLife].nBlock_X + 1; nCntH++)
				{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
				 // �e�N�X�`���̐ݒ�
					pVtx[nCntH + (g_aBillLife[nCntBillLife].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aBillLife[nCntBillLife].nBlock_X) / 100.0f + g_aBillLife[nCntBillLife].fTexmoveMesh, float(nCntV * 100 / g_aBillLife[nCntBillLife].nBlock_Y) / 100.0f);
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aBillLife[nCntBillLife].pVtxBuff->Unlock();

			WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

				// �C���f�b�N�X�o�b�t�@�̍쐬
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aBillLife[nCntBillLife].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aBillLife[nCntBillLife].pIdxBuff,
				NULL);

			// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aBillLife[nCntBillLife].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

			// �C���f�b�N�X�̐ݒ�
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aBillLife[nCntBillLife].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aBillLife[nCntBillLife].nBlock_X + 2) == g_aBillLife[nCntBillLife].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aBillLife[nCntBillLife].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aBillLife[nCntBillLife].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aBillLife[nCntBillLife].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// ���b�V���̐ݒ�
//==============================================================================================================================
void SetBillLife(int nIdy, D3DXVECTOR3 pos, int nLife, D3DXCOLOR col,BILLLIFETYPE nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, float fOffset, bool bUseDown)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == false)
		{
			g_aBillLife[nCntBillLife].fOffset = fOffset;			// �I�t�Z�b�g
			g_aBillLife[nCntBillLife].pos = pos;					// �ʒu
			g_aBillLife[nCntBillLife].pos.y += g_aBillLife[nCntBillLife].fOffset;	// �ʒu
			g_aBillLife[nCntBillLife].col = col;					// �F
			g_aBillLife[nCntBillLife].nLife = nLife;				// ���C�t
			g_aBillLife[nCntBillLife].nInitLife = nLife;			// �������C�t
			g_aBillLife[nCntBillLife].fWidth = fWidth;				// ��
			g_aBillLife[nCntBillLife].fHeight = fHeight;			// ���s��
			g_aBillLife[nCntBillLife].nBlock_X = nBlock_X;			// �u���b�NX
			g_aBillLife[nCntBillLife].nBlock_Y = nBlock_Y;			// �u���b�NZ
			g_aBillLife[nCntBillLife].nType = nType;				// ���
			g_aBillLife[nCntBillLife].nNumVertex = (g_aBillLife[nCntBillLife].nBlock_X + 1)*(g_aBillLife[nCntBillLife].nBlock_Y + 1);											// ���_��
			g_aBillLife[nCntBillLife].nNumIndex = 2 * (g_aBillLife[nCntBillLife].nBlock_X*g_aBillLife[nCntBillLife].nBlock_Y) + 4 * (g_aBillLife[nCntBillLife].nBlock_Y - 1) + 2;	// �C���f�b�N�X��										// 
			g_aBillLife[nCntBillLife].nNumPolygon = 2 * (g_aBillLife[nCntBillLife].nBlock_X*g_aBillLife[nCntBillLife].nBlock_Y) + 4 * (g_aBillLife[nCntBillLife].nBlock_Y - 1);	// �|���S����
			g_aBillLife[nCntBillLife].bUseDown = bUseDown;
			g_aBillLife[nCntBillLife].nIdy = nIdy;
			g_aBillLife[nCntBillLife].nCntTime = 0;
			g_aBillLife[nCntBillLife].nDrawDown = 0;
			g_aBillLife[nCntBillLife].bDown = false;				// �_�E�������ǂ���
			g_aBillLife[nCntBillLife].bDownRed = false;				// �Ԃ����C�t�_�E�������ǂ���
			g_aBillLife[nCntBillLife].nDownRedCounter = 0;			// �Ԃ����C�t�����Ԋu�ۊǃJ�E���^�[
			MakeVertexBillLife(pDevice);
			g_aBillLife[nCntBillLife].bUse = true;										// �g�p����Ă��邩�ǂ���
			break;
		}
	}
}
//==============================================================================================================================
// ���C�t�̊l��
//==============================================================================================================================
BillLife *GetBillLife(void)
{
	return &g_aBillLife[0];
}
BillLifeData *GetBillLifeData(void)
{
	return &g_aBillLifeData[0];
}

//==============================================================================================================================
// ���C�t�̕ύX
//==============================================================================================================================
void GetBillLife(int nIdy,int nLife)
{
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nIdy == nIdy)
			{
				g_aBillLife[nCntBillLife].nLife = nLife;
				g_aBillLife[nCntBillLife].bDown = true;
			}
		}
	}
}
//==============================================================================================================================
// �|�W�V�����ړ�
//==============================================================================================================================
void SetPositionBillLife(int nIdy, D3DXVECTOR3 pos)
{
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nIdy == nIdy)
			{
				g_aBillLife[nCntBillLife].pos = D3DXVECTOR3(pos.x,pos.y + g_aBillLife[nCntBillLife].fOffset,pos.z);
			}
		}
	}
}
//==============================================================================================================================
// �폜
//==============================================================================================================================
void DeleteBillLife(int nIdy)
{
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nIdy == nIdy)
			{
				g_aBillLife[nCntBillLife].bUse = false;
			}
		}
	}
}
