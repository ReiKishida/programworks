////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�V�������� [meshField.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�

#include "meshField.h"
#include "input.h"
#include "manager.h"
#include "line.h"
#include "renderer.h"
#include "DebugProc.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_SCAN  "data/object_scan.txt"		// �t�@�C���̖��O�ǂݍ���

#define MAX_MESHFIELD		(100)
#define MAX_MESHFIELDTYPE	(24)
//*********************************************************************************
// �ÓI�����o�ϐ��錾
//*********************************************************************************
LPDIRECT3DTEXTURE9	*CMeshField::m_pTextureThis = NULL;
int CMeshField::m_nNumFile = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(int nPriority):CScene(PRIORITY_MESHFIELD)
{
	m_pTexture = NULL;								// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pNor = NULL;	// �@��
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_fWidth = 0;							// ��
	m_fHeight = 0;							// ����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]��
	m_bCullMode = false;					// �J�����O���[�h
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CMeshField::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice= CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FILE_SCAN, "r");		// �t�@�C���̖��O�̓ǂݍ���
	short int nNumTexture = 0;
	short int nCntTexture = 0;

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// �s�̓ǂݍ���
			if (strcmp(cData, "NUM_TEXTURE") == 0)		// ���f���̐�
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %hd", &cData[0], &cData[0], &nNumTexture);
				m_pTextureThis = new LPDIRECT3DTEXTURE9[nNumTexture]();
			}
			// ���f���̃t�@�C���ǂݍ���
			if (strcmp(cData, "TEXTURE_FILENAME") == 0 && nNumTexture > nCntTexture)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice,
					&cData[0],
					&m_pTextureThis[nCntTexture]);
				nCntTexture++;
				m_nNumFile++;
			}

		}
		fclose(pFile);// �I��
	}

	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CMeshField::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTextureThis != NULL)
	{
		delete[] m_pTextureThis;
		m_pTextureThis = NULL;
	}
}
//=============================================================================
// �e�N�X�`�����蓖��
//=============================================================================
void CMeshField::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==============================================================================================================================
// ����������
//==============================================================================================================================
HRESULT CMeshField::Init(void)
{
	m_pVtxBuff = NULL;									// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;									// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �F
	m_fWidth = 0;											// ��
	m_fHeight = 0;											// ����
	m_fDepth = 0;											// ���s��
	m_nBlock_X = 0;										// �u���b�NX
	m_nBlock_Z = 0;										// �u���b�NZ
	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Z + 1);// ���_��
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1) + 2;// �C���f�b�N�X��
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1);	// �|���S����
	m_bCullMode = false;						// �J�����O���[�h
	SetObjType(OBJTYPE_MESHFIELD);				// �I�u�W�F�N�g�^�C�v�ݒ�

	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CMeshField::Uninit(void)
{

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// �C���f�b�N�X�o�b�t�@�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	Release();
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void CMeshField::Update(void)
{
	//VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//CDebugProc::print("���_���F%d\n", m_nNumVertex);

	//static float fLength = 0;

	//fLength += 10;
	//static float *pSpeed = new float[m_nNumVertex];
	//for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	//{
	//	float fPosLenght = (pVtx[nCnt].pos.x*pVtx[nCnt].pos.x) + (pVtx[nCnt].pos.z*pVtx[nCnt].pos.z);				// ���ꂼ���2�悵�Ă��ׂđ����ċ������o��
	//																					// ��̓����蔻��̔��a�̋������v�Z
	//	float fCollisionRadius = (fLength);	// �v���C���[�̑̂̓����蔻��̔��a+�G�̍U���̓����蔻��̔��a�𑫂�
	//	float fRadiusLenght = fCollisionRadius * fCollisionRadius;	// ��̔��a�𑫂����l��2�悵�āA���a�̋������v�Z���v�Z

	//	if (fRadiusLenght > fPosLenght &&
	//		-fRadiusLenght < fPosLenght)
	//	{// ���W�̋��������a�̋�������������������
	//		pSpeed[nCnt] += 0.1f;
	//		pVtx[nCnt].pos.y = cosf(pSpeed[nCnt])*100;

	//	}
	//}

	//// ���_�o�b�t�@���A�����b�N����
	//m_pVtxBuff->Unlock();
	//SetVtx();
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
	//{// �f�[�^�̍X�V
	//	CMeshField::SetScan();
	//}
}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// ���e�X�g��ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���l��0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���傫��

				// Z�o�b�t�@�ւ̏�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if (m_bCullMode)
	{// �J�����O�I�t
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);
	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumIndex, 0, m_nNumPolygon);
	// Z�o�b�t�@�ւ̏�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// �J�����O���[�h�߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// ���e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���

}
//=============================================================================
// ����
//=============================================================================
CMeshField *CMeshField::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const int nBlockX, const int nBlockZ,const int type,const bool bCull)
{
	CMeshField *pMeshField = NULL;
	if (!CScene::OverData())
	{// �QD�|���S������
		pMeshField = new CMeshField;
	}
	if (pMeshField != NULL)
	{
		// ����������
		if (FAILED(pMeshField->Init()))
		{
			return NULL;
		}
		pMeshField->SetPos(pos);
		pMeshField->SetSize(fWidth, fHeight, fDepth);
		pMeshField->SetBlock(nBlockX, nBlockZ);
		pMeshField->m_type = type;
		pMeshField->m_bCullMode = bCull;
		pMeshField->BindTexture(m_pTextureThis[type]);
		pMeshField->MakeVtx();
	}
	return pMeshField;
}
//==============================================================================================================================
// ���W�ݒ�
//==============================================================================================================================
void CMeshField::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// �傫���ݒ�
//==============================================================================================================================
void CMeshField::SetSize(const float fWidth, const float fHeight, const float fDepth)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
}
//==============================================================================================================================
// ��]�ݒ�
//==============================================================================================================================
void CMeshField::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// �������̐ݒ�
//==============================================================================================================================
void CMeshField::SetBlock(const int nBlockX, const int nBlockZ)
{
	m_nBlock_X = nBlockX;
	m_nBlock_Z = nBlockZ;
}
//==============================================================================================================================
// �F�̐ݒ�
//==============================================================================================================================
void CMeshField::SetColor(const D3DXCOLOR col)
{
	if (m_col == col)return;
	m_col = col;
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].col = m_col;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//==============================================================================================================================
// ���_�ݒ�
//==============================================================================================================================
void CMeshField::SetVtx(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �e���_�̖@��
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntZ = 0; nCntZ < m_nBlock_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntZ*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntZ) + nCntX;
			int nLDOWN = (nCntZ * (m_nBlock_X + 1)) + 1 + nCntX;
			// ����
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// �E��
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
		}

	}

	int nCntCenter = 0;
	int nBlockCounter = 0;
	// �@���̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		//	pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		if (nCnt == 0)
		{// �ŏ�
			pVtx[nCnt].nor = (m_pNor[nCnt] + m_pNor[nCnt + 1]) / 2;
		}
		else if (nCnt == m_nNumVertex - 1)
		{// �Ō�
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// �E��
		 // �ԍ����{���ĂP����
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Z)
		{// ����
		 // Z�����Łu�Q���vX�����Łu�{�v
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Z - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// ���[< X < �E�[
		{// ���̏�[�P
			int nIdy = nCnt + nCnt - 1;	// �ԍ����{���ĂP����
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Z&&nCnt < m_nNumVertex - 1)
		{// ���̉��[�Q
			int nIdypolygon = (m_nBlock_Z - 1) * 2 * m_nBlock_X;		// ���[�̃|���S���ԍ�
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (���[�̔ԍ� - �ԍ� -1 )*3�Ń|���S����i�߂�
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Z&&nCnt % (m_nBlock_X + 1) == 0)
		{// �[�̒��̍�	(���̃u���b�N��+1 ���ŏ��̒��_�ł���A�{���Ŏ��̒��_�B
		 // �|���S����(���̒��_��(�u���b�N��+1))�̊������o���ė��p����B
			int nBlock = nCnt / (m_nBlock_X + 1);	// �����v�Z
			int nIdy = m_nBlock_X * 2;			// ������
			pVtx[nCnt].nor = (m_pNor[(nBlock - 1)*nIdy] + m_pNor[(nBlock)*nIdy] + m_pNor[(nBlock)*nIdy + 1]) / 3;
		}
		else if (nCnt != 0 && ((m_nNumVertex - 1) - nCnt) % (m_nBlock_X + 1) == 0)
		{// �[�̒��̉E
			int nBlock = (nCnt - m_nBlock_X) / (m_nBlock_X + 1);	// �����v�Z
			int nIdy = m_nBlock_X * 2;								// ������
			pVtx[nCnt].nor = (m_pNor[nIdy*nBlock - 1] + m_pNor[nIdy*nBlock - 2] + m_pNor[nIdy*(nBlock + 1) - 1]) / 3;
		}
		else
		{// �^��
			pVtx[nCnt].nor = (m_pNor[0 + nCntCenter] + m_pNor[1 + nCntCenter] + m_pNor[2 + nCntCenter]
				+ m_pNor[0 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[1 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[2 + m_nBlock_X * 2 + 1 + nCntCenter]) / 6;
			nCntCenter += 2;
			nBlockCounter++;
			if (nBlockCounter >= m_nBlock_X)
			{
				nBlockCounter = 0;
				nCntCenter += 2;
			}
		}
	}
	// ���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].col = m_col;
	}
	// �u���b�N���ƂɈꖇ�z�u����e�N�X�`��
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt % (m_nBlock_X + 1)), 0.0f + (nCnt / (m_nBlock_X + 1)));
	}
	for (int nCntV = 0; nCntV < m_nBlock_Z + 1; nCntV++)
	{// �e�N�X�`���̐ݒ�
		for (int nCntH = 0; nCntH < m_nBlock_X + 1; nCntH++)
		{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
		 // �e�N�X�`���̐ݒ�
			pVtx[nCntH + (m_nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / m_nBlock_X) / 100.0f, float(nCntV * 100 / m_nBlock_Z) / 100.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//==============================================================================================================================
// ���_����
//==============================================================================================================================
void CMeshField::MakeVtx(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Z + 1);// ���_��
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1) + 2;// �C���f�b�N�X��
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Z) + 4 * (m_nBlock_Z - 1);	// �|���S����

	m_pNor = new D3DXVECTOR3[m_nNumPolygon];

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntPos = 0; nCntPos < m_nNumVertex; nCntPos++)
	{
		float fHeight = 0;
		//fHeight = rand()%2 * 100.0f;
		pVtx[nCntPos].pos = D3DXVECTOR3(float(-((m_nBlock_X * m_fWidth) / 2) + m_fWidth * (nCntPos % (m_nBlock_X + 1))),
			fHeight,
			float((m_nBlock_Z * m_fDepth) / 2 - m_fDepth * (nCntPos / (m_nBlock_X + 1))));
	}

	// �e���_�̖@��
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntZ = 0; nCntZ < m_nBlock_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntZ*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntZ) + nCntX;
			int nLDOWN = (nCntZ * (m_nBlock_X + 1)) + 1 + nCntX;
			// ����
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// �E��
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntZ *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
		}

	}


	int nCntCenter = 0;
	int nBlockCounter = 0;
	// �@���̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		//	pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		if (nCnt == 0)
		{// �ŏ�
			pVtx[nCnt].nor = (m_pNor[nCnt] + m_pNor[nCnt + 1]) / 2;
		}
		else if (nCnt == m_nNumVertex - 1)
		{// �Ō�
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Z) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// �E��
		 // �ԍ����{���ĂP����
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Z)
		{// ����
		 // Z�����Łu�Q���vX�����Łu�{�v
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Z - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// ���[< X < �E�[
		{// ���̏�[�P
			int nIdy = nCnt + nCnt - 1;	// �ԍ����{���ĂP����
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Z&&nCnt < m_nNumVertex - 1)
		{// ���̉��[�Q
			int nIdypolygon = (m_nBlock_Z - 1) * 2 * m_nBlock_X;		// ���[�̃|���S���ԍ�
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (���[�̔ԍ� - �ԍ� -1 )*3�Ń|���S����i�߂�
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Z&&nCnt % (m_nBlock_X + 1) == 0)
		{// �[�̒��̍�	(���̃u���b�N��+1 ���ŏ��̒��_�ł���A�{���Ŏ��̒��_�B
		 // �|���S����(���̒��_��(�u���b�N��+1))�̊������o���ė��p����B
			int nBlock = nCnt / (m_nBlock_X + 1);	// �����v�Z
			int nIdy = m_nBlock_X * 2;			// ������
			pVtx[nCnt].nor = (m_pNor[(nBlock - 1)*nIdy] + m_pNor[(nBlock)*nIdy] + m_pNor[(nBlock)*nIdy + 1]) / 3;
		}
		else if (nCnt != 0 && ((m_nNumVertex - 1) - nCnt) % (m_nBlock_X + 1) == 0)
		{// �[�̒��̉E
			int nBlock = (nCnt - m_nBlock_X) / (m_nBlock_X + 1);	// �����v�Z
			int nIdy = m_nBlock_X * 2;								// ������
			pVtx[nCnt].nor = (m_pNor[nIdy*nBlock - 1] + m_pNor[nIdy*nBlock - 2] + m_pNor[nIdy*(nBlock + 1) - 1]) / 3;
		}
		else
		{// �^��
			pVtx[nCnt].nor = (m_pNor[0 + nCntCenter] + m_pNor[1 + nCntCenter] + m_pNor[2 + nCntCenter]
				+ m_pNor[0 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[1 + m_nBlock_X * 2 + 1 + nCntCenter] + m_pNor[2 + m_nBlock_X * 2 + 1 + nCntCenter]) / 6;
			nCntCenter += 2;
			nBlockCounter++;
			if (nBlockCounter >= m_nBlock_X)
			{
				nBlockCounter = 0;
				nCntCenter += 2;
			}
		}
	}
	// ���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].col = m_col;
	}
	// �u���b�N���ƂɈꖇ�z�u����e�N�X�`��
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt % (m_nBlock_X + 1)), 0.0f + (nCnt / (m_nBlock_X + 1)));
	}

	//for (int nCntV = 0; nCntV < m_nBlock_Z + 1; nCntV++)
	//{// �e�N�X�`���̐ݒ�
	//	for (int nCntH = 0; nCntH < m_nBlock_X + 1; nCntH++)
	//	{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
	//	 // �e�N�X�`���̐ݒ�
	//		pVtx[nCntH + (m_nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / m_nBlock_X) / 100.0f, float(nCntV * 100 / m_nBlock_Z) / 100.0f);
	//	}
	//}
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();


	WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

				// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD)*m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int nCntIdx = 0, nCntPos = 0; nCntIdx < m_nNumIndex; nCntIdx += 2)
	{
		if ((nCntIdx / 2) % (m_nBlock_X + 2) == m_nBlock_X + 1)
		{
			pIdx[nCntIdx] = nCntPos - 1;
			pIdx[nCntIdx + 1] = nCntPos + m_nBlock_X + 1;
		}
		else
		{
			pIdx[nCntIdx] = nCntPos + m_nBlock_X + 1;
			pIdx[nCntIdx + 1] = nCntPos;
			nCntPos++;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

}
//==============================================================================================================================
// ���_����
//==============================================================================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *Vec)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fHeight = -200;
	D3DXVECTOR3 VecA[4];// ���b�V���̃x�N�g��
	D3DXVECTOR3 VexC[4];// �v���C���[�ƒ��_�̃x�N�g��
	float fVecLine[4];	// ���_�x�N�g�����v���C���[�����������ǂ���

	D3DXVECTOR3 VecAL[3];// ���b�V���̃x�N�g��
	D3DXVECTOR3 VexCL[3];// �v���C���[�ƒ��_�̃x�N�g��
	D3DXVECTOR3 VecAR[3];// ���b�V���̃x�N�g��
	D3DXVECTOR3 VexCR[3];// �v���C���[�ƒ��_�̃x�N�g��
	float fVecLineL[3];	// ���_�x�N�g�����v���C���[�����������ǂ���
	float fVecLineR[3];	// ���_�x�N�g�����v���C���[�����������ǂ���

	for (int nCntZ = 0; nCntZ < m_nBlock_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			D3DXVECTOR3 meshPos[4] =
			{
				pVtx[0 + nCntX + nCntZ * (m_nBlock_X+1)].pos + m_pos,		// 0
				pVtx[1 + nCntX + nCntZ * (m_nBlock_X + 1)].pos + m_pos,		// 1
				pVtx[3 + m_nBlock_X - 1 + nCntX + nCntZ * (m_nBlock_X + 1)].pos + m_pos,		// 3
				pVtx[2 + m_nBlock_X - 1 + nCntX + nCntZ * (m_nBlock_X + 1)].pos + m_pos,		// 2
			};
			VecA[0] = meshPos[0] - meshPos[1];
			VecA[1] = meshPos[1] - meshPos[2];
			VecA[2] = meshPos[2] - meshPos[3];
			VecA[3] = meshPos[3] - meshPos[0];
			int nIn = 0;
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{// �v���C���[�ƃ��b�V���̃x�N�g��
				VexC[nCnt] = (pos - meshPos[nCnt]);

				fVecLine[nCnt] = (VexC[nCnt].z * VecA[nCnt].x) - (VexC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] >= 0)
				{// ������
					nIn++;
				}
			}
			if (nIn == 4)
			{
				// ���E����
				// ��
				VecAL[0] = meshPos[3] - meshPos[0];	// �O�p�`��`���悤�ȃx�N�g���@
				VecAL[1] = meshPos[2] - meshPos[3];	// �O�p�`��`���悤�ȃx�N�g���A
				VecAL[2] = meshPos[0] - meshPos[2];	// �O�p�`��`���悤�ȃx�N�g���B
				VexCL[0] = (pos - meshPos[3]);
				VexCL[1] = (pos - meshPos[2]);
				VexCL[2] = (pos - meshPos[0]);
				// �E
				VecAR[0] = meshPos[0] - meshPos[1];	// �O�p�`��`���悤�ȃx�N�g���@
				VecAR[1] = meshPos[1] - meshPos[2];	// �O�p�`��`���悤�ȃx�N�g���A
				VecAR[2] = meshPos[2] - meshPos[0];	// �O�p�`��`���悤�ȃx�N�g���B

				VexCR[0] = (pos - meshPos[0]);
				VexCR[1] = (pos - meshPos[1]);
				VexCR[2] = (pos - meshPos[2]);

				bool bIn = false;
				bool bInR = false;
				for (int nCnt = 0; nCnt < 3; nCnt++)
				{// �v���C���[�ƃ��b�V���̃x�N�g��

					fVecLineL[nCnt] = (VexCL[nCnt].z * VecAL[nCnt].x) - (VexCL[nCnt].x * VecAL[nCnt].z);
					fVecLineR[nCnt] = (VexCR[nCnt].z * VecAR[nCnt].x) - (VexCR[nCnt].x * VecAR[nCnt].z);
					if (fVecLineL[nCnt] < 0)
					{// ������
						bIn = true;
					}
					if (fVecLineR[nCnt] < 0)
					{// ������
						bInR = true;
					}
				}

				if (!bIn)
				{// ��
					D3DXVECTOR3 vv1, vv2, nor;
					vv1 = meshPos[3] - meshPos[2];
					vv2 = meshPos[3] - meshPos[0];
					D3DXVec3Cross(&nor, &vv1, &vv2);
					D3DXVec3Normalize(&nor, &nor);
					D3DXVECTOR3 VecA = pos - meshPos[3];
					fHeight = (-(nor.x * VecA.x) - (nor.z * VecA.z)) / nor.y + meshPos[3].y;
					*Vec = nor;
				}
				if (!bInR)
				{// �E
					D3DXVECTOR3 vv1, vv2, nor;
					vv1 = meshPos[1] - meshPos[0];
					vv2 = meshPos[1] - meshPos[2];
					D3DXVec3Cross(&nor, &vv1, &vv2);
					D3DXVec3Normalize(&nor, &nor);
					D3DXVECTOR3 VecA = pos - meshPos[1];
					fHeight = (-(nor.x * VecA.x) - (nor.z * VecA.z)) / nor.y + meshPos[1].y;
					*Vec = nor;
				}
			}
		}
	}

	return fHeight;
}

//=============================================================================
// �t�@�C���̓ǂݍ���
//=============================================================================
void CMeshField::SetScan()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FILE_SCAN, "r");		// �t�@�C���̖��O�̓ǂݍ���
	short int nNumModel = 0;

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// �s�̓ǂݍ���
			if (strcmp(cData, "SETFIELD") == 0)
			{// OBJECTSET�������ꍇ
				D3DXVECTOR3 pos;		// �ʒu
				float sizeX = 0;		// �傫��[ �� ]
				float sizeZ = 0;		// �傫��[ �� ]
				int Width =0;				// ����
				int Depth=0;				// ����
				int type=0;				// ���
				int nCull = 0;
				bool bCull = false;		// �J�����O
				while (strcmp(cData, "ENDSETFIELD") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);

					if (strcmp(cData, "FIELD") == 0)
					{// �ʒu�̓ǂݍ���
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %f %d %d %d", &cData[0], &cData[0], &type, &pos.x, &pos.y, &pos.z, &sizeX, &sizeZ, &Width, &Depth,&nCull);
						if (nCull == 1)
						{
							bCull = true;
						}
						CMeshField::Create(pos, sizeX, 0.0f, sizeZ, Width, Depth,type, bCull);
					}
					if (strcmp(cData, "SETVERTEX") == 0)
					{// SET_FIELD�������ꍇ
						D3DXVECTOR3 aMeshPos[100] = {};
						D3DXVECTOR3 start = D3DXVECTOR3(0, 0, 0);		// �ʒu
						int nBlock = 0;						//
						int nCntMesh = 0;
						int nCntBrock = -1;
						while (strcmp(cData, "END_SETVERTEX") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "TYPE") == 0)
							{// ��ނ̓ǂݍ���
								sscanf(&cLine[0], "%s %s %d ", &cData[0], &cData[0], &type);
							}
							else if (strcmp(cData, "BLOCK") == 0)
							{// �u���b�N�̑����𐔂���
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nBlock);
							}
							else if (strcmp(cData, "CULL") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCull);
								if (nCull == 1)
								{
									bCull = true;
								}
							}
							else if (strcmp(cData, "SET_FIELDSET") != 0)
							{// SET_FIELDSET�ȊO�̎�
								sscanf(&cLine[0], "%d %s %f %f %f", &nCntMesh, &cData[0], &start.x, &start.y, &start.z);
								aMeshPos[nCntMesh] = start;
								nCntMesh++;		// ���_�J�E���^�[�̉��Z
							}

						}
						CMeshField *pMesh;
						pMesh = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 1, nBlock, type, bCull);
						pMesh->SetVtxPos(&aMeshPos[0]);
					}
				}
			}
		}
		fclose(pFile);// �I��
	}
}

//=============================================================================
// ���_�ʒu�̐ݒ�
//=============================================================================
void CMeshField::SetVtxPos(D3DXVECTOR3 *pPos)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// X���EZ�������p
	//for (int nCnt = 0; nCnt < m_nNumVertex; nCnt += m_nBlock_X + 1)
	//{
	//	for (int nCntBlock = 0; nCntBlock < m_nBlock_X + 1; nCntBlock++)
	//	{
	//		pVtx[nCnt + nCntBlock].pos = pPos[nCnt+nCntBlock];
	//	}
	//}

	// Z�����������p
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		pVtx[nCnt].pos = pPos[nCnt];
	}
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
	SetVtx();
}