//=============================================================================
//
// �A���̏��� [environment.cpp]
// Author : �؉� �̑�
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�

#include "environment.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "meshWall.h"
#include "camera.h"
//*********************************************************************************
// �}�N����`
//*********************************************************************************
#define FILE_SCAN  "data/object_scan.txt"		// �t�@�C���̖��O�ǂݍ���
#define MAX_CHAR (1024)

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9	*CEnvironmentBillboard::m_apTexture = NULL;
int					CEnvironmentBillboard::m_nNumTexture = 0;
CEnvironmentBillboard *CEnvironment::m_apEnviroment[MAX_ENVIROMENT] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnvironmentBillboard::CEnvironmentBillboard(int nPriority):CBillboard(PRIORITY_ENVIROMENT)
{
	m_type = 0;
	m_bUse = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnvironmentBillboard::~CEnvironmentBillboard()
{
}

//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CEnvironmentBillboard::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FILE_SCAN, "r");		// �t�@�C���̖��O�̓ǂݍ���
	short int nCntTexture = 0;

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// �s�̓ǂݍ���
			if (strcmp(cData, "NUM_BILLBOARD_TEXTURE") == 0)		// ���f���̐�
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_nNumTexture);
				m_apTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture]();
			}
			// ���f���̃t�@�C���ǂݍ���
			if (strcmp(cData, "BILLBOARD_FILENAME") == 0 && m_nNumTexture > nCntTexture)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice,
					&cData[0],
					&m_apTexture[nCntTexture]);
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
void CEnvironmentBillboard::UnLoad(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnvironmentBillboard::Init()
{
	if (FAILED(CBillboard::Init()))
	{
		return E_FAIL;
	}

	// ���̏�����
	m_type = 0;
	m_bUse = false;
	SetObjType(OBJTYPE_ENVIROMENT);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnvironmentBillboard::Uninit(void)
{
	CBillboard::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnvironmentBillboard::Update(void)
{
	if (!m_bUse) return;

}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnvironmentBillboard::Draw(void)
{
	if (!m_bUse) return;

	CBillboard::Draw();
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
void CEnvironmentBillboard::Set(const int nIdy,const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int type)
{
	m_bUse = true;
	m_type = type;
	BindTexture(m_apTexture[m_type]);

	SetPos(pos);
	SetRot(0.0f);
	SetVtxType(CBillboard::VTXTYPE_BASE);
	SetSize(fWidth, fHeight);
	SetVtx();
}

//=============================================================================
// �g�p�����̐ݒ�
//=============================================================================
void CEnvironmentBillboard::SetUse(const bool bUse)
{
	m_bUse = bUse;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnvironment::CEnvironment() : CScene(PRIORITY_ENVIROMENT)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnvironment::~CEnvironment()
{
}

//=============================================================================
// ��������
//=============================================================================
CEnvironment *CEnvironment::Create(void)
{
	CEnvironment *pBullet = NULL;

	// �N���X�̐���
	pBullet = new CEnvironment;

	if (pBullet == NULL)
	{// �����ł��Ȃ������ꍇ
		return NULL;
	}

	// ����������
	if (FAILED(pBullet->Init()))
	{
		return NULL;
	}

	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnvironment::Init(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_ENVIROMENT; nCntBullet++)
	{
		if (m_apEnviroment[nCntBullet] != NULL) continue;

		m_apEnviroment[nCntBullet] = new CEnvironmentBillboard;

		if (m_apEnviroment[nCntBullet] == NULL)
		{// �����ł��Ȃ������ꍇ
			return E_FAIL;
		}

		// ����������
		if (FAILED(m_apEnviroment[nCntBullet]->Init()))
		{
			return E_FAIL;
		}
		m_apEnviroment[nCntBullet]->SetVtx();
	}

	SetObjType(OBJTYPE_ENVIROMENT);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnvironment::Uninit(void)
{
	// �N���X�̏I������
	for (int nCntBullet = 0; nCntBullet < MAX_ENVIROMENT; nCntBullet++)
	{
		if (m_apEnviroment[nCntBullet] == NULL) continue;

		m_apEnviroment[nCntBullet]->Uninit();
		m_apEnviroment[nCntBullet] = NULL;
	}

	// ���̃N���X�̊J������
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnvironment::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnvironment::Draw(void)
{
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int CEnvironment::Set(const int nIdy,const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int type)
{
	for (int nCntCall = 0; nCntCall < MAX_ENVIROMENT; nCntCall++)
	{
		if (m_apEnviroment[nCntCall] == NULL) continue;

		if (!m_apEnviroment[nCntCall]->GetUse())
		{// ���g�p�̏ꍇ
			m_apEnviroment[nCntCall]->Set(nIdy, pos, fWidth, fHeight, type);
			return nCntCall;
		}
	}
	return 0;
}
//=============================================================================
// �폜
//=============================================================================
void CEnvironment::Delete(const int nID)
{
	if (m_apEnviroment[nID] != NULL)
	{
		m_apEnviroment[nID]->SetUse(false);
	}
}
//=============================================================================
// �ǂݍ���
//=============================================================================
void CEnvironment::SetScan(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FILE_SCAN, "r");		// �t�@�C���̖��O�̓ǂݍ���
	int nNumAll = 0;
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// �s�̓ǂݍ���

			if (strcmp(cData, "SETBILLBOARD") == 0)
			{// SET_FIELD�������ꍇ
				int nType = 0;
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
				D3DXVECTOR2 size = D3DXVECTOR2(0.0f,0.0f);
				while (strcmp(cData, "END_SETBILLBOARD") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{// ��ނ̓ǂݍ���
						sscanf(&cLine[0], "%s %s %d ", &cData[0], &cData[0], &nType);
					}
					if (strcmp(cData, "POS") == 0)
					{// ��ނ̓ǂݍ���
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(cData, "SIZE") == 0)
					{// ��ނ̓ǂݍ���
						sscanf(&cLine[0], "%s %s %f %f ", &cData[0], &cData[0], &size.x, &size.y);
					}

				}
				CEnvironment::Set(nNumAll, pos, size.x, size.y, nType);
				nNumAll++;
			}
		}
		fclose(pFile);// �I��
	}
}