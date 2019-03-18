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

#include "meshWall.h"
#include "input.h"
#include "manager.h"
#include "line.h"
#include "renderer.h"
#include "DebugProc.h"
#include "input.h"
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
LPDIRECT3DTEXTURE9	*CMeshWall::m_pTextureThis = NULL;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshWall::CMeshWall(int nPriority):CScene(PRIORITY_MESHWALL)
{
	m_pTexture = NULL;								// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pNor = NULL;	// �@��
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_fWidth = 0;							// ��
	m_fHeight = 0;							// ����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_afAngle[nCnt] = 0.0f;
		m_afLength[nCnt] = 0.0f;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshWall::~CMeshWall()
{

}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CMeshWall::Load(void)
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
			if (strcmp(cData, "NUM_WALL_TEXTURE") == 0)		// ���f���̐�
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %hd", &cData[0], &cData[0], &nNumTexture);
				m_pTextureThis = new LPDIRECT3DTEXTURE9[nNumTexture]();
			}
			// ���f���̃t�@�C���ǂݍ���
			if (strcmp(cData, "WALL_FILENAME") == 0 && nNumTexture > nCntTexture)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice,
					&cData[0],
					&m_pTextureThis[nCntTexture]);
				nCntTexture++;
			}

		}
		fclose(pFile);// �I��
	}

	return S_OK;
}
//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CMeshWall::UnLoad(void)
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
void CMeshWall::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==============================================================================================================================
// ����������
//==============================================================================================================================
HRESULT CMeshWall::Init(void)
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
	m_nBlock_Y = 0;										// �u���b�NZ
	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Y + 1);// ���_��
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1) + 2;// �C���f�b�N�X��
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1);	// �|���S����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_afAngle[nCnt] = 0.0f;
		m_afLength[nCnt] = 0.0f;
	}
	SetObjType(OBJTYPE_MESHWALL);				// �I�u�W�F�N�g�^�C�v�ݒ�

	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CMeshWall::Uninit(void)
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
void CMeshWall::Update(void)
{

}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void CMeshWall::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// ���e�X�g��ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���l��0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���傫��

	// Z�o�b�t�@�ւ̏�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
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
	// ���e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���

}
//=============================================================================
// ����
//=============================================================================
CMeshWall *CMeshWall::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXCOLOR col,const float fWidth, const float fHeight, const float fDepth, const int nBlockX, const int nBlockY,int type)
{
	CMeshWall *pMeshWall = NULL;
	if (!CScene::OverData())
	{// �ǐ���
		pMeshWall = new CMeshWall;
	}
	if (pMeshWall != NULL)
	{
		// ����������
		if (FAILED(pMeshWall->Init()))
		{
			return NULL;
		}
		pMeshWall->SetPos(pos);
		pMeshWall->SetColor(col);
		pMeshWall->SetSize(fWidth, fHeight, fDepth);
		pMeshWall->SetBlock(nBlockX, nBlockY);
		pMeshWall->SetRot(rot);
		pMeshWall->m_type = type;
		pMeshWall->BindTexture(m_pTextureThis[type]);
		pMeshWall->MakeVtx();
	}
	return pMeshWall;
}
//==============================================================================================================================
// ���W�ݒ�
//==============================================================================================================================
void CMeshWall::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// �傫���ݒ�
//==============================================================================================================================
void CMeshWall::SetSize(const float fWidth, const float fHeight, const float fDepth)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;

	m_afAngle[0] = atan2f(-m_fWidth / 2, 0);
	m_afAngle[1] = atan2f(m_fWidth / 2, 0);
	m_afAngle[2] = atan2f(m_fWidth / 2, 50);
	m_afAngle[3] = atan2f(-m_fWidth / 2, 50);
	m_afLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2));
	m_afLength[1] = sqrtf((m_fWidth / 2)*(m_fWidth / 2));
	m_afLength[2] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + 50 * 50);
	m_afLength[3] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + 50 * 50);

}
//==============================================================================================================================
// ��]�ݒ�
//==============================================================================================================================
void CMeshWall::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// �������̐ݒ�
//==============================================================================================================================
void CMeshWall::SetBlock(const int nBlockX, const int nBlockY)
{
	m_nBlock_X = nBlockX;
	m_nBlock_Y = nBlockY;
}
//==============================================================================================================================
// �F�̐ݒ�
//==============================================================================================================================
void CMeshWall::SetColor(const D3DXCOLOR col)
{
	if (m_col == col)return;
	m_col = col;
}
//==============================================================================================================================
// ���_�ݒ�
//==============================================================================================================================
void CMeshWall::SetVtx(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �e���_�̖@��
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntY = 0; nCntY < m_nBlock_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntY*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntY) + nCntX;
			int nLDOWN = (nCntY * (m_nBlock_X + 1)) + 1 + nCntX;
			// ����
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// �E��
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
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
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// �E��
		 // �ԍ����{���ĂP����
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Y)
		{// ����
		 // Z�����Łu�Q���vX�����Łu�{�v
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Y - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// ���[< X < �E�[
		{// ���̏�[�P
			int nIdy = nCnt + nCnt - 1;	// �ԍ����{���ĂP����
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Y&&nCnt < m_nNumVertex - 1)
		{// ���̉��[�Q
			int nIdypolygon = (m_nBlock_Y - 1) * 2 * m_nBlock_X;		// ���[�̃|���S���ԍ�
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (���[�̔ԍ� - �ԍ� -1 )*3�Ń|���S����i�߂�
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Y&&nCnt % (m_nBlock_X + 1) == 0)
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
	for (int nCntV = 0; nCntV < m_nBlock_Y + 1; nCntV++)
	{// �e�N�X�`���̐ݒ�
		for (int nCntH = 0; nCntH < m_nBlock_X + 1; nCntH++)
		{// "X"�͌��ɖ߂����灓�@"Z"�͌����܂��͑�����������̂Ł�
		 // �e�N�X�`���̐ݒ�
			pVtx[nCntH + (m_nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / m_nBlock_X) / 100.0f, float(nCntV * 100 / m_nBlock_Y) / 100.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//==============================================================================================================================
// ���_����
//==============================================================================================================================
void CMeshWall::MakeVtx(void)
{
	VERTEX_3D *pVtx;	// ���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nNumVertex = (m_nBlock_X + 1)*(m_nBlock_Y + 1);// ���_��
	m_nNumIndex = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1) + 2;// �C���f�b�N�X��
	m_nNumPolygon = 2 * (m_nBlock_X*m_nBlock_Y) + 4 * (m_nBlock_Y - 1);	// �|���S����

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
		pVtx[nCntPos].pos = D3DXVECTOR3(float(-((m_nBlock_X * m_fWidth) / 2) + m_fWidth * (nCntPos % (m_nBlock_X + 1))),
			float((m_fHeight * m_nBlock_Y)) - (m_fHeight)*(nCntPos / (m_nBlock_X + 1)),
			0.0f);
	}



	// �e���_�̖@��
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 Nor;
	for (int nCntY = 0; nCntY < m_nBlock_Y; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_nBlock_X; nCntX++)
		{
			int nStart = nCntX + nCntY*(m_nBlock_X + 1);
			int nLUP = m_nBlock_X + 1 + ((m_nBlock_X + 1)*nCntY) + nCntX;
			int nLDOWN = (nCntY * (m_nBlock_X + 1)) + 1 + nCntX;
			// ����
			VecA = (pVtx[nLUP].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY*(m_nBlock_X * 2)) + (nCntX * 2)] = Nor;
			// �E��
			VecA = (pVtx[nLUP + 1].pos - pVtx[nStart].pos);	// ��
			VecB = (pVtx[nLDOWN].pos - pVtx[nStart].pos); // �E
			D3DXVec3Cross(&Nor, &VecB, &VecA);
			D3DXVec3Normalize(&Nor, &Nor);
			m_pNor[(nCntY *(m_nBlock_X * 2)) + (nCntX * 2) + 1] = Nor;
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
			pVtx[nCnt].nor = (m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 1] + m_pNor[2 * (m_nBlock_X*m_nBlock_Y) - 2]) / 2;
		}
		else if (nCnt == m_nBlock_X)
		{// �E��
		 // �ԍ����{���ĂP����
			pVtx[nCnt].nor = m_pNor[nCnt + nCnt - 1];
		}
		else if (nCnt == (m_nBlock_X + 1)*m_nBlock_Y)
		{// ����
		 // Z�����Łu�Q���vX�����Łu�{�v
			pVtx[nCnt].nor = m_pNor[(m_nBlock_Y - 1) * 2 * m_nBlock_X];
		}
		else if (nCnt > 0 && nCnt < m_nBlock_X)	// ���[< X < �E�[
		{// ���̏�[�P
			int nIdy = nCnt + nCnt - 1;	// �ԍ����{���ĂP����
			pVtx[nCnt].nor = (m_pNor[nIdy] + m_pNor[nIdy + 1] + m_pNor[nIdy + 2]) / 3;
		}
		else if (nCnt >(m_nBlock_X + 1)*m_nBlock_Y&&nCnt < m_nNumVertex - 1)
		{// ���̉��[�Q
			int nIdypolygon = (m_nBlock_Y - 1) * 2 * m_nBlock_X;		// ���[�̃|���S���ԍ�
			int nDiffP = ((m_nBlock_X - 1) - (m_nNumVertex - 1 - nCnt)) * 2;	// (���[�̔ԍ� - �ԍ� -1 )*3�Ń|���S����i�߂�
			pVtx[nCnt].nor = (m_pNor[nIdypolygon + nDiffP] + m_pNor[nIdypolygon + nDiffP + 1] + m_pNor[nIdypolygon + nDiffP + 2]) / 3;
		}
		else if (nCnt > 0 && nCnt < (m_nBlock_X + 1)*m_nBlock_Y&&nCnt % (m_nBlock_X + 1) == 0)
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

	//for (int nCntV = 0; nCntV < m_nBlock_Y + 1; nCntV++)
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
bool CMeshWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, const float fWidth, const float fHeight)
{
	bool bHit = false;
	if (pPos->x == posOld.x && pPos->z == posOld.z)return false;

	VERTEX_3D *pVtx;	// ���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 VecA[4];// ���b�V���̃x�N�g��
	D3DXVECTOR3 VecC[4];// �v���C���[�ƒ��_�̃x�N�g��
	D3DXVECTOR3 VecCOld[4];// �v���C���[�ƒ��_�̃x�N�g��
	float fVecLine[4];	// ���_�x�N�g�����v���C���[�����������ǂ���
	float fVecLineOld[4];	// ���_�x�N�g�����v���C���[�����������ǂ���
	D3DXVECTOR3 meshPos[4];

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		meshPos[nCnt] = D3DXVECTOR3(sinf(m_rot.y + m_afAngle[nCnt])*m_afLength[nCnt], pVtx[nCnt].pos.y, cosf(m_rot.y + m_afAngle[nCnt])*m_afLength[nCnt]) + m_pos;
	}

	VecA[0] = meshPos[0] - meshPos[1];
	VecA[1] = meshPos[1] - meshPos[2];
	VecA[2] = meshPos[2] - meshPos[3];
	VecA[3] = meshPos[3] - meshPos[0];

	bool nIn = true;
	bool bInOld = true;
	// �x�N�g���𐂒��ɉ�]�����āA�@��
	float fNor = atan2f(VecA[0].x, VecA[0].z) + D3DX_PI / 2;
	D3DXVECTOR3 Radius = D3DXVECTOR3(sinf(fNor)*fWidth, 0.0f, cosf(fNor)*fWidth);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// �v���C���[�ƃ��b�V���̃x�N�g��
		VecC[nCnt] = (*pPos + Radius - meshPos[nCnt]);
		fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
		if (fVecLine[nCnt] > 0)
		{
			nIn = false;
		}
		VecCOld[nCnt] = (posOld + Radius - meshPos[nCnt]);
		fVecLineOld[nCnt] = (VecCOld[nCnt].z * VecA[nCnt].x) - (VecCOld[nCnt].x * VecA[nCnt].z);
		if (fVecLineOld[nCnt] > 0)
		{
			bInOld = false;
		}
	}

	if (nIn&&fVecLine[0] <= 0)
	{
		if (m_fHeight + m_pos.y >= pPos->y &&
			m_pos.y <= pPos->y + fHeight)
		{// �����͈͓�

			D3DXVECTOR3 EndPos = *pPos;
			D3DXVECTOR3 AB = meshPos[0] - meshPos[1];
			D3DXVec3Normalize(&AB, &AB);

			float fLength = AB.x * VecC[0].x + AB.z * VecC[0].z;

			pPos->x = meshPos[0].x + AB.x*fLength - Radius.x;
			pPos->z = meshPos[0].z + AB.z*fLength - Radius.z;

			CDebugProc::print("�x�N�g��AB�F%.1f,%.1f\n", AB.x, AB.z);
			CDebugProc::print("�����F%.1f\n", fLength);
			CDebugProc::print("���W�F%.1f%.1f\n", meshPos[0].x + AB.x*fLength, meshPos[0].z + AB.z*fLength);

			//���݂̃x�N�g���̒����{�ߋ��̃x�N�g���̒���
			//float All = fabsf(fVecLine[0]) + fabsf(fVecLineOld[0]);
			//float Now = fabsf(fVecLine[0]) / All + 0.001f;// 0���h�~�̂��߉��Z
			//D3DXVECTOR3 diff = (*pPos - posOld);// ���W�ƑO����W�̍����𒊏o
			//pPos->x += -diff.x*Now;
			//pPos->z += -diff.z*Now;
			bHit = true;

		}
	}

	return bHit;
}

//=============================================================================
// �t�@�C���̓ǂݍ���
//=============================================================================
void CMeshWall::SetScan()
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
			if (strcmp(cData, "SETWALL") == 0)
			{// OBJECTSET�������ꍇ
				D3DXVECTOR3 pos;		// �ʒu
				D3DXVECTOR3 rot;		// �p�x
				D3DXCOLOR col=D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);			// �F
				float fWidth = 0;		// �傫��[ �� ]
				float fHeight = 0;		// �傫��[ �c ]
				int nBlockX = 0;		// ����
				int nBlockY = 0;		// ����
				int type = 0;			// ���
				while (strcmp(cData, "ENDSETWALL") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{// �^�C�v�ǂݍ���
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &type);
					}
					else if (strcmp(cData, "POS") == 0)
					{// ���W�ǂݍ���
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pos.x, &pos.y, &pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{// �p�x�ǂݍ���
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &rot.x, &rot.y, &rot.z);
						rot = D3DXVECTOR3(D3DXToRadian(rot.x), D3DXToRadian(rot.y), D3DXToRadian(rot.z));
					}
					else if (strcmp(cData, "COLOR" )== 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f %f", &cData[0], &cData[0], &col.r, &col.g, &col.b, &col.a);
					}
					else if (strcmp(cData, "SIZE") == 0)
					{// �傫���ǂݍ���
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &fWidth, &fHeight);
					}
					else if (strcmp(cData, "BLOCK") == 0)
					{// �傫���ǂݍ���
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &nBlockX, &nBlockY);
					}
				}
				CMeshWall::Create(pos, rot, col, fWidth, fHeight,0.0f, nBlockX, nBlockY, type);

			}
		}
		fclose(pFile);// �I��
	}
}
