////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �~�̏��� [meshFar.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "meshFar.h"
#include "input.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_MESHFAR	(1)
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void MakeVertexMeshFar(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
MeshFar g_aMeshFar[MAX_MESHFAR];
MeshFarData g_aMeshFarData[24];
//==============================================================================================================================
// ����������
//==============================================================================================================================
void InitMeshFar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		g_aMeshFar[nCntMeshFar].pVtxBuff = NULL;									// ���_�o�b�t�@�ւ̃|�C���^
		g_aMeshFar[nCntMeshFar].pIdxBuff = NULL;									// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		g_aMeshFar[nCntMeshFar].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);					// �ʒu
		g_aMeshFar[nCntMeshFar].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
		g_aMeshFar[nCntMeshFar].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);				// �F
		g_aMeshFar[nCntMeshFar].fDepth = 0;											// ���s��
		g_aMeshFar[nCntMeshFar].nBlock_X = 0;										// �u���b�NX
		g_aMeshFar[nCntMeshFar].nBlock_Z = 0;										// �u���b�NZ
		g_aMeshFar[nCntMeshFar].nType = 0;		// ���
		g_aMeshFar[nCntMeshFar].fMoveVtx = 0;									// ���_���Ƃ̍���
		g_aMeshFar[nCntMeshFar].fTexmove = 0;									// �e�N�X�`���̈ړ����x
		g_aMeshFar[nCntMeshFar].fTexPos = 0;								// �e�N�X�`���̈ړ�
		g_aMeshFar[nCntMeshFar].nNumVertex =(g_aMeshFar[nCntMeshFar].nBlock_X + 1)*(g_aMeshFar[nCntMeshFar].nBlock_Z + 1);// ���_��
		g_aMeshFar[nCntMeshFar].nNumIndex = 2*(g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z)+4*(g_aMeshFar[nCntMeshFar].nBlock_Z - 1)+2;// �C���f�b�N�X��										// 
		g_aMeshFar[nCntMeshFar].nNumPolygon = 2*(g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z)+4*(g_aMeshFar[nCntMeshFar].nBlock_Z-1);	// �|���S����
		g_aMeshFar[nCntMeshFar].bCull = false;										// �J�����O
		g_aMeshFar[nCntMeshFar].bUse = false;										// �g�p����Ă��邩�ǂ���
	}
	g_aMeshFarData[0].pFileName = "data/TEXTURE/cloud.jpg";

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		g_aMeshFarData[0].pFileName,						// �t�@�C���̖��O
		&g_aMeshFarData[0].pTexture);						// �e�N�X�`���ւ̃|�C���^
	SetMeshFar(D3DXVECTOR3(0,5550,0), D3DXVECTOR3(0, 0, 0), 0, 30, 1,3000, true);
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void UninitMeshFar(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMeshFar = 0; nCntMeshFar < g_aMeshFarData[0].nNumTex; nCntMeshFar++)
	{
		if (g_aMeshFarData[nCntMeshFar].pTexture != NULL)
		{
			g_aMeshFarData[nCntMeshFar].pTexture->Release();
			g_aMeshFarData[nCntMeshFar].pTexture = NULL;
		}
	}
	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		// ���_�o�b�t�@�̔j��
		if (g_aMeshFar[nCntMeshFar].pVtxBuff != NULL)
		{
			g_aMeshFar[nCntMeshFar].pVtxBuff->Release();
			g_aMeshFar[nCntMeshFar].pVtxBuff = NULL;
		}
		// �C���f�b�N�X�o�b�t�@�o�b�t�@�̔j��
		if (g_aMeshFar[nCntMeshFar].pIdxBuff != NULL)
		{
			g_aMeshFar[nCntMeshFar].pIdxBuff->Release();
			g_aMeshFar[nCntMeshFar].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void UpdateMeshFar(void)
{

}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void DrawMeshFar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		if (g_aMeshFar[nCntMeshFar].bUse == true)
		{
			if (g_aMeshFar[nCntMeshFar].bCull == true)
			{// �J�����O�I�t
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshFar[nCntMeshFar].mtxWorld);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshFar[nCntMeshFar].rot.y, g_aMeshFar[nCntMeshFar].rot.x, g_aMeshFar[nCntMeshFar].rot.z);
			D3DXMatrixMultiply(&g_aMeshFar[nCntMeshFar].mtxWorld, &g_aMeshFar[nCntMeshFar].mtxWorld, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshFar[nCntMeshFar].pos.x, g_aMeshFar[nCntMeshFar].pos.y, g_aMeshFar[nCntMeshFar].pos.z);
			D3DXMatrixMultiply(&g_aMeshFar[nCntMeshFar].mtxWorld, &g_aMeshFar[nCntMeshFar].mtxWorld, &mtxTrans);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshFar[nCntMeshFar].mtxWorld);
			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_aMeshFar[nCntMeshFar].pVtxBuff, 0, sizeof(VERTEX_3D));
			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_aMeshFar[nCntMeshFar].pIdxBuff);
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aMeshFarData[g_aMeshFar[nCntMeshFar].nType].pTexture);
			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, g_aMeshFar[nCntMeshFar].nNumIndex, 0, g_aMeshFar[nCntMeshFar].nNumPolygon);
			if (g_aMeshFar[nCntMeshFar].bCull == true)
			{// �J�����O�I�t
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
		}
	}

}

//==============================================================================================================================
// ���_���̍쐬
//==============================================================================================================================
void MakeVertexMeshFar(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		if (g_aMeshFar[nCntMeshFar].bUse == false)
		{

			// ���_���̍쐬
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshFar[nCntMeshFar].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aMeshFar[nCntMeshFar].pVtxBuff,
				NULL);

			float fAngle=0, fLength=0;
			fLength = g_aMeshFar[nCntMeshFar].fDepth;
			// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aMeshFar[nCntMeshFar].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// ���_���W�̐ݒ�
			for (int nCntZ = 0; nCntZ < g_aMeshFar[nCntMeshFar].nBlock_Z + 1; nCntZ++)
			{
				
				for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nBlock_X + 1; nCnt++)
				{// ���b�V��
					fAngle = (D3DX_PI * 2 / (g_aMeshFar[nCntMeshFar].nBlock_X -1))*nCnt;

					if (nCnt == 0)
					{
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].tex = D3DXVECTOR2(1.0f, 1.0f);
					}
					else
					{
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].pos = D3DXVECTOR3(sinf(fAngle)*fLength,0.0f,cosf(fAngle)*fLength);
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].tex = D3DXVECTOR2((1.0f / (g_aMeshFar[nCntMeshFar].nBlock_X - 1))*nCnt, (1.0f / (g_aMeshFar[nCntMeshFar].nBlock_X - 1))*nCnt);

					}
				}
			}
			// �@���̐ݒ�
			for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nNumVertex; nCnt++)
			{
				pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			// ���_�J���[�̐ݒ�
			for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aMeshFar[nCntMeshFar].col;
			}
			//// �u���b�N���ƂɈꖇ�z�u����e�N�X�`��
			//for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nNumVertex; nCnt++)
			//{
			//	// �e�N�X�`���̐ݒ�
			//	pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(g_aMeshFar[nCntMeshFar].nBlock_X+1)), 0.0f + (nCnt / (g_aMeshFar[nCntMeshFar].nBlock_X + 1)));
			//}

			// �����e�N�X�`��
			//for (int nCntV = 0; nCntV < g_aMeshFar[nCntMeshFar].nBlock_Z + 1; nCntV++)
			//{// �e�N�X�`���̐ݒ�
			//	for (int nCntH = 0; nCntH < g_aMeshFar[nCntMeshFar].nBlock_X + 1; nCntH++)
			//	{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
			//	 // �e�N�X�`���̐ݒ�
			//		pVtx[nCntH + (g_aMeshFar[nCntMeshFar].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aMeshFar[nCntMeshFar].nBlock_X) / 100.0f + g_aMeshFar[nCntMeshFar].fTexPos, float(nCntV * 100 / g_aMeshFar[nCntMeshFar].nBlock_Z) / 100.0f);
			//	}
			//}
			// ���_�o�b�t�@���A�����b�N����
			g_aMeshFar[nCntMeshFar].pVtxBuff->Unlock();


			WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

			// �C���f�b�N�X�o�b�t�@�̍쐬
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aMeshFar[nCntMeshFar].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aMeshFar[nCntMeshFar].pIdxBuff,
				NULL);

			// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
			g_aMeshFar[nCntMeshFar].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	

			// �C���f�b�N�X�̐ݒ�
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aMeshFar[nCntMeshFar].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aMeshFar[nCntMeshFar].nBlock_X + 2) == g_aMeshFar[nCntMeshFar].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aMeshFar[nCntMeshFar].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aMeshFar[nCntMeshFar].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			g_aMeshFar[nCntMeshFar].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// ���b�V���̐ݒ�
//==============================================================================================================================
void SetMeshFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType, int nBlock_X, int nBlock_Z, float fDepth,bool bCull)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		if (g_aMeshFar[nCntMeshFar].bUse == false)
		{
			g_aMeshFar[nCntMeshFar].pos = pos;					// �ʒu
			g_aMeshFar[nCntMeshFar].rot = rot;				// ����
			g_aMeshFar[nCntMeshFar].fDepth = fDepth;											// ���s��
			g_aMeshFar[nCntMeshFar].nBlock_X = nBlock_X;										// �u���b�NX
			g_aMeshFar[nCntMeshFar].nBlock_Z = nBlock_Z;										// �u���b�NZ
			g_aMeshFar[nCntMeshFar].nType = nType;		// ���
			g_aMeshFar[nCntMeshFar].bCull = bCull;		// �J�����O
			g_aMeshFar[nCntMeshFar].nNumVertex = (g_aMeshFar[nCntMeshFar].nBlock_X + 1)*(g_aMeshFar[nCntMeshFar].nBlock_Z + 1);// ���_��
			g_aMeshFar[nCntMeshFar].nNumIndex = 2 * (g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z) + 4 * (g_aMeshFar[nCntMeshFar].nBlock_Z - 1) + 2;// �C���f�b�N�X��										// 
			g_aMeshFar[nCntMeshFar].nNumPolygon = 2 * (g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z) + 4 * (g_aMeshFar[nCntMeshFar].nBlock_Z - 1);	// �|���S����		


			MakeVertexMeshFar(pDevice);
			g_aMeshFar[nCntMeshFar].bUse = true;										// �g�p����Ă��邩�ǂ���

			break;
		}
	}
}
//==============================================================================================================================
// ���b�V���̊l��
//==============================================================================================================================
MeshFar *GetMeshFar(void)
{
	return &g_aMeshFar[0];
}
MeshFarData *GetMeshFarData(void)
{
	return &g_aMeshFarData[0];
}
