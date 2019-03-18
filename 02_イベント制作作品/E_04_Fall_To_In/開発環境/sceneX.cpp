////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���f������ [model.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�

#include "input.h"
#include "camera.h"
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "line.h"
#include "DebugProc.h"
#include "game.h"
#include "camera.h"
#include "Shader.h"
#include "sound.h"
#include "shadow.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_SCAN  "data/object_scan.txt"		// �t�@�C���̖��O�ǂݍ���
#define FILE_PRINT "data/object_print.txt"		// �t�@�C���̖��O��������

//******************************************************************************************************************************
// �ÓI�����o�ϐ��錾
//******************************************************************************************************************************
CSceneX::XData CSceneX::m_pModelData[MAX_MODEL_TYPE] = {};
int CSceneX::m_nNumFile = 0;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(int nPriority) :CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = 0;
	m_aMeshPos =NULL; //�S���_���W�v�Z�p�|�C���^
	m_apLine= NULL;		// �l�p�`�����p�|�C���^
	m_nCameraID = 0;// �J�����ԍ�
	m_ShadowPos = D3DXVECTOR3(0, 0, 0);// �e�̈ʒu
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}
//=============================================================================
// �e�N�X�`������
//=============================================================================
HRESULT CSceneX::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FILE_SCAN, "r");
	short int nNumModel = 0;
	int nCntModel = 0;
	int nType = 0;
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{// ������
		m_pModelData[nCntMat].m_pMesh = NULL;
		m_pModelData[nCntMat].m_pBuffMat = NULL;
		m_pModelData[nCntMat].m_nNumMat = 0;
		m_pModelData[nCntMat].m_pTextuer = NULL;
		m_pModelData[nCntMat].m_pFileName = NULL;
		m_pModelData[nCntMat].m_nNumCollision = 0;
		m_pModelData[nCntMat].m_pCollision = NULL;
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "NUM_MODEL") == 0)		// ���f���̐�
			{// �g���ĂȂ�����ǂ�����
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_nNumFile);
			}
			// ���f���̃t�@�C���ǂݍ���
			if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);

				m_pModelData[nCntModel].m_pFileName = &cData[0];

				// X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(m_pModelData[nCntModel].m_pFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_pModelData[nCntModel].m_pBuffMat,
					NULL,
					&m_pModelData[nCntModel].m_nNumMat,
					&m_pModelData[nCntModel].m_pMesh);
				D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)m_pModelData[nCntModel].m_pBuffMat->GetBufferPointer();
				// �e�N�X�`���̔z��ݒ�
				m_pModelData[nCntModel].m_pTextuer = new LPDIRECT3DTEXTURE9[(int)m_pModelData[nCntModel].m_nNumMat];
				for (int nCntPlayerTex = 0; nCntPlayerTex < (int)m_pModelData[nCntModel].m_nNumMat; nCntPlayerTex++)
				{

					m_pModelData[nCntModel].m_pTextuer[nCntPlayerTex] = NULL;
					if (pMat[nCntPlayerTex].pTextureFilename != NULL)
					{
						// �e�N�X�`���̐ݒ�
						D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
							pMat[nCntPlayerTex].pTextureFilename,				// �t�@�C���̖��O
							&m_pModelData[nCntModel].m_pTextuer[nCntPlayerTex]);// �e�N�X�`���ւ̃|�C���^
					}
				}
				nCntModel++;

				//m_nNumFile++;		// �ǂݍ��񂾃t�@�C�����̉��Z
			}
			if (strcmp(cData, "COLLISIONSET") == 0)
			{// ����Z�b�g
				int nNumCube = 0;
				while (strcmp(cData, "END_COLLISIONSET") != 0)
				{// ����I���܂Ń��[�v
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nType);
					}
					else if (strcmp(cData, "NUMCOLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_pModelData[nType].m_nNumCollision);
						m_pModelData[nType].m_pCollision = new CollisionData[m_pModelData[nType].m_nNumCollision]();
					}
					else if (strcmp(cData, "CUBE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f %f %f %f", &cData[0], &cData[0],
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.x,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.y,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.z,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fWidth,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fHeight,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fDepth);
						// �傫������ő�l�ŏ��l���v�Z
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax = D3DXVECTOR3(m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, m_pModelData[nType].m_pCollision[nNumCube].m_fHeight, m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin = D3DXVECTOR3(-m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, 0.0f, -m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;

						// �傫������p�x�Ƌ������v�Z[ �����@�����@�E���@�E�� ]
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[0] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[0] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[1] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[1] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[2] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[2] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[3] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[3] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));

						nNumCube++;
					}
				}

			}
		}
		fclose(pFile);// �I��
	}
	return S_OK;
}
//=============================================================================
// ���f���̔j��
//=============================================================================
void CSceneX::UnLoad(void)
{
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{
		// ���b�V���̊J��
		if (m_pModelData[nCntMat].m_pMesh != NULL)
		{
			m_pModelData[nCntMat].m_pMesh->Release();
			m_pModelData[nCntMat].m_pMesh = NULL;
		}
		// �}�e���A���̊J��
		if (m_pModelData[nCntMat].m_pBuffMat != NULL)
		{
			m_pModelData[nCntMat].m_pBuffMat->Release();
			m_pModelData[nCntMat].m_pBuffMat = NULL;
		}
		// �e�N�X�`���̊J��
		if (m_pModelData[nCntMat].m_pTextuer != NULL)
		{
			// �e�N�X�`���̊J��
			for (int nCntMatTex = 0; nCntMatTex < int(m_pModelData[nCntMat].m_nNumMat); nCntMatTex++)
			{
				if (m_pModelData[nCntMat].m_pTextuer[nCntMatTex] != NULL)
				{
					m_pModelData[nCntMat].m_pTextuer[nCntMatTex]->Release();
					m_pModelData[nCntMat].m_pTextuer[nCntMatTex] = NULL;
				}
			}
			delete[] m_pModelData[nCntMat].m_pTextuer;
			m_pModelData[nCntMat].m_pTextuer = NULL;
		}
		for (int nCntCollision = 0; nCntCollision < m_pModelData[nCntMat].m_nNumCollision; nCntCollision++)
		{// ����폜
			if (m_pModelData[nCntMat].m_pCollision != NULL)
			{
				delete[] m_pModelData[nCntMat].m_pCollision;
				m_pModelData[nCntMat].m_pCollision = NULL;
			}
		}

	}
}
//=============================================================================
// ���f���̊��蓖��
//=============================================================================
void CSceneX::BindModel(const int type)
{
	m_pBuffMat = m_pModelData[type].m_pBuffMat;
	// ����쐬
	DeleteCollision();
	m_type = type;
	// ����ݒ�
	SetCollision();
}

//==============================================================================================================================
// ����������
//==============================================================================================================================
HRESULT CSceneX::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ShadowPos = D3DXVECTOR3(0, 0, 0);// �e�̈ʒu
	m_aMeshPos = NULL;
	m_apLine = NULL;
	m_nCameraID = 0;// �J�����ԍ�
	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CSceneX::Uninit(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{// ���肪����Ȃ�
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// ���蕪
			if (m_apLine != NULL)
			{// ���C�����폜
				if (m_apLine[nCntCol] != NULL)
				{// ����̌����폜
					for (int nLine = 0; nLine < MAX_VTX_LINE; nLine++)
					{// �����̂̃��C�����폜
						if (m_apLine[nCntCol][nLine] != NULL)
						{
							m_apLine[nCntCol][nLine]->Uninit();
							m_apLine[nCntCol][nLine] = NULL;
						}
					}
					delete[] m_apLine[nCntCol];
					m_apLine[nCntCol] = NULL;
				}
			}
			if (m_aMeshPos != NULL)
			{// ����p���W���폜
				if (m_aMeshPos[nCntCol] != NULL)
				{// ����̌����폜
					delete[] m_aMeshPos[nCntCol];
				}
			}
		}
		if (m_apLine != NULL)
		{// ���C�����폜
			delete m_apLine;
			m_apLine = NULL;
		}
		if (m_aMeshPos != NULL)
		{// ����p���W���폜
			delete m_aMeshPos;
			m_aMeshPos = NULL;
		}
	}
	// �I�u�W�F�N�g�̔j��
	Release();

}
//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void CSceneX::Update(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// ���蕔����
			for (int meshCnt = 0; meshCnt < MAX_VTX; meshCnt++)
			{// �S���_�v�Z
				// ���_���W���p�x�Ƌ����Ōv�Z
				m_aMeshPos[nCntCol][meshCnt] = D3DXVECTOR3(sinf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt],
					0.0f,
					cosf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt]) + m_pos;
				if (m_apLine[nCntCol][meshCnt] != NULL)
				{// ���C�����W�ݒ�u���v
					m_apLine[nCntCol][meshCnt]->SetPos(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f) ,
						m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f) );
				}
				if (m_apLine[nCntCol][4 + meshCnt] != NULL)
				{// ���C�����W�ݒ�u��v
					m_apLine[nCntCol][4 + meshCnt]->SetPos(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f) ,
						m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f) );
				}
				if (m_apLine[nCntCol][8 + meshCnt] != NULL)
				{// ���C�����W�ݒ�u���v
					m_apLine[nCntCol][8 + meshCnt]->SetPos(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f) ,
						m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f) );
				}
			}
		}
	}

}

//==============================================================================================================================
// �`�揈��
//==============================================================================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
	CCamera *pCamera = CManager::GetCameraMode();
	D3DXVECTOR3 rot = pCamera->GetRot(m_nCameraID);
	LPD3DXEFFECT pShader = CShader::GetEffect();
	LPDIRECT3DTEXTURE9 pTexture = CShader::GetTexture();

	// ���C�g�̐ݒ�
	D3DXVECTOR3 lightrot = D3DXVECTOR3((0.7f)*cosf(rot.y) - (0.8f)*sinf(rot.y), -0.5f, (0.7f)*sinf(rot.y) + (0.8f)*cosf(rot.y));
	// ���K��
	D3DXVec3Normalize(&lightrot, &lightrot);

	// ���e�X�g��ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���l��0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���傫��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ���C�gON

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

	// �e�N�j�b�N�̐ݒ�
	pShader->SetTechnique("StandardShader");
	// �V�F�[�_�J�n
	pShader->Begin(0, 0);

	// �r���[�}�g���b�N�X��ݒ�
	D3DXMATRIX matrix = m_mtxWorld *pCamera->GetMtxView(m_nCameraID) * pCamera->GetMtxProjection(m_nCameraID);
	// ���C�g�̐ݒ�
	CShader::GetEffect()->SetVector("g_fLightDir", &D3DXVECTOR4(lightrot.x, lightrot.y, lightrot.z, 0)); // ��]�s��
	pShader->SetMatrix("g_fMatrix", &matrix);
	// �F�̐ݒ�
	pShader->SetVector("g_fColor", &D3DXVECTOR4(m_col.r, m_col.g, m_col.b, m_col.a));
	// �p�X�̕`��J�n
	pShader->BeginPass(0);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat2 = 0; nCntMat2 < (int)m_pModelData[m_type].m_nNumMat; nCntMat2++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pModelData[m_type].m_pTextuer[nCntMat2]);
		// �����v�e�N�X�`���̐ݒ�(1�ԂɃ����v�e�N�X�`����ݒ�)
		pDevice->SetTexture(1, pTexture);

		// ���f��(�p�[�c)�̕`��
		m_pModelData[m_type].m_pMesh->DrawSubset(nCntMat2);
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
	// �p�X�̕`��I��
	pShader->EndPass();
	// �V�F�[�_�I��
	pShader->End();

	// ���e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ���C�gON
	CShadow::SetShadow(m_pModelData[m_type].m_pMesh, m_pModelData[m_type].m_nNumMat, m_mtxWorld, &m_ShadowPos);

}
//=============================================================================
// ����
//=============================================================================
CSceneX *CSceneX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type)
{
	CSceneX *pSceneX = NULL;
	if (!CScene::OverData())
	{// �QD�|���S������
		pSceneX = new CSceneX;
	}
	if (pSceneX != NULL)
	{
		// ����������
		if (FAILED(pSceneX->Init()))
		{
			return NULL;
		}
		pSceneX->SetPos(pos);
		pSceneX->SetType(type);
		pSceneX->BindModel(type);

	}
	return pSceneX;
}
//==============================================================================================================================
// �����蔻��
//==============================================================================================================================
bool CSceneX::Collision(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move,const float fHeight)
{
	bool bRand = false;
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{
			D3DXVECTOR3 VecA[4];// ���b�V���̃x�N�g��
			D3DXVECTOR3 VecC[4];// �v���C���[�ƒ��_�̃x�N�g��
			D3DXVECTOR3 VecCOld[4];// �v���C���[�ƒ��_�̃x�N�g��
			float fVecLine[4], fVecLineOld[4];	// ���_�x�N�g�����v���C���[�����������ǂ���
			bool bIn = true;
			bool bInOld = true;

			VecA[0] = m_aMeshPos[nCntCol][0]- m_aMeshPos[nCntCol][1];
			VecA[1] = m_aMeshPos[nCntCol][1]- m_aMeshPos[nCntCol][2];
			VecA[2] = m_aMeshPos[nCntCol][2]- m_aMeshPos[nCntCol][3];
			VecA[3] = m_aMeshPos[nCntCol][3]- m_aMeshPos[nCntCol][0];

			int nCntLine = 0;
			float fLine = 10000;

			for (int nCnt = 0; nCnt < MAX_VTX; nCnt++)
			{// �v���C���[�ƃ��b�V���̃x�N�g��
				VecC[nCnt] = (*pPos  - m_aMeshPos[nCntCol][nCnt]);

				fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] < 0)
				{// �ǂꂩ���͈͊O
					bIn = false;
				}
				VecCOld[nCnt] = (posOld  - m_aMeshPos[nCntCol][nCnt]);

				fVecLineOld[nCnt] = (VecCOld[nCnt].z * VecA[nCnt].x) - (VecCOld[nCnt].x * VecA[nCnt].z);
				if (fVecLineOld[nCnt] < 0)
				{// �ǂꂩ���͈͊O
					bInOld = false;

				}
				float fabsLine = fabsf(fVecLineOld[nCnt]);
				if (fLine >= fabsLine)
				{// ��ԋ߂�
					fLine = fabsLine;
					nCntLine = nCnt;
				}
			}

			if (bIn)
			{// ������
				if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y &&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y <= posOld.y )
				{// ��
					bRand = true;
					move->y = 0.0f;
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y ;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y < pPos->y + fHeight&&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y >= posOld.y + fHeight)
				{// ��
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y - fHeight;
					move->y = 0.0f;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y &&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y <= pPos->y + fHeight)
				{// �㉺�͈͓�
					for (int meshCnt = 0; meshCnt < MAX_VTX; meshCnt++)
					{
						// ���݂̃x�N�g���̒����{�ߋ��̃x�N�g���̒���
						float All = fabsf(fVecLine[meshCnt]) + fabsf(fVecLineOld[meshCnt]);
						float Now = fabsf(fVecLine[meshCnt]) / All + 0.001f;// 0���h�~�̂��߉��Z
						D3DXVECTOR3 diff = (*pPos - posOld);// ���W�ƑO����W�̍����𒊏o
						pPos->x += -diff.x*Now;
						pPos->z += -diff.z*Now;
						move->x = 0;
						move->z = 0;

					}
					//CDebugProc::print("����������%d\n", nCntLine);
					//CDebugProc::print("����������%d\n", (nCntLine + 1) % MAX_VTX);
					//D3DXVECTOR3 EndPos = *pPos;
					//D3DXVECTOR3 AB = m_aMeshPos[nCntCol][nCntLine] - m_aMeshPos[nCntCol][(nCntLine + 1) % MAX_VTX];
					//D3DXVec3Normalize(&AB, &AB);

					//float fLength = AB.x * VecC[nCntLine].x + AB.z * VecC[nCntLine].z;

					//pPos->x = m_aMeshPos[nCntCol][nCntLine].x + AB.x*fLength;
					//pPos->z = m_aMeshPos[nCntCol][nCntLine].z + AB.z*fLength;
					break;
				}
			}
		}
	}
	return bRand;
}
//==============================================================================================================================
// ���˂���
//==============================================================================================================================
bool CSceneX::CollisionReflection(D3DXVECTOR3 *pPos, const D3DXVECTOR3 posOld, D3DXVECTOR3 *move, const float fHeight)
{
	bool bRand = false;
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{
			D3DXVECTOR3 VecA[4];// ���b�V���̃x�N�g��
			D3DXVECTOR3 VecC[4];// �v���C���[�ƒ��_�̃x�N�g��
			D3DXVECTOR3 VecCOld[4];// �v���C���[�ƒ��_�̃x�N�g��
			float fVecLine[4], fVecLineOld[4];	// ���_�x�N�g�����v���C���[�����������ǂ���
			bool bIn = true;
			bool bInOld = true;

			VecA[0] = m_aMeshPos[nCntCol][0] - m_aMeshPos[nCntCol][1];
			VecA[1] = m_aMeshPos[nCntCol][1] - m_aMeshPos[nCntCol][2];
			VecA[2] = m_aMeshPos[nCntCol][2] - m_aMeshPos[nCntCol][3];
			VecA[3] = m_aMeshPos[nCntCol][3] - m_aMeshPos[nCntCol][0];

			int nCntLine = 0;

			for (int nCnt = 0; nCnt < MAX_VTX; nCnt++)
			{// �v���C���[�ƃ��b�V���̃x�N�g��
				VecC[nCnt] = (*pPos - m_aMeshPos[nCntCol][nCnt]);

				fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] < 0)
				{// �ǂꂩ���͈͊O
					bIn = false;
				}
				VecCOld[nCnt] = (posOld - m_aMeshPos[nCntCol][nCnt]);

				fVecLineOld[nCnt] = (VecCOld[nCnt].z * VecA[nCnt].x) - (VecCOld[nCnt].x * VecA[nCnt].z);
				if (fVecLineOld[nCnt] < 0)
				{// �ǂꂩ���͈͊O
					bInOld = false;
				}

			}

			if (bIn)
			{// ������
				if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y &&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y <= posOld.y)
				{// ��
					bRand = true;
					move->y = 0.0f;
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y < pPos->y + fHeight&&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y >= posOld.y + fHeight)
				{// ��
					pPos->y = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y - fHeight;
					move->y = 0.0f;
				}
				else if (m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y >= pPos->y + fHeight&&
					m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y + m_pos.y <= pPos->y + fHeight)
				{// �㉺�͈͓�
						// ���݂̃x�N�g���̒����{�ߋ��̃x�N�g���̒���
					float All = fabsf(fVecLine[0]) + fabsf(fVecLineOld[0]);
					float Now = fabsf(fVecLine[0]) / All + 0.001f;// 0���h�~�̂��߉��Z
					D3DXVECTOR3 diff = (*pPos - posOld);// ���W�ƑO����W�̍����𒊏o
					pPos->x += -diff.x*Now;
					pPos->z += -diff.z*Now;
					move->x = -move->x;
					move->z = -move->z;
					break;
				}
			}
		}
	}
	return bRand;
}
//==============================================================================================================================
// ���擾
//==============================================================================================================================

float CSceneX::GetHeight(D3DXVECTOR3 *pPos, bool *bIn)
{
	float fHeight = -200;
	if (m_pModelData[m_type].m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{
			D3DXVECTOR3 VecA[4];// ���b�V���̃x�N�g��
			D3DXVECTOR3 VecC[4];// �v���C���[�ƒ��_�̃x�N�g��
			float fVecLine[4];	// ���_�x�N�g�����v���C���[�����������ǂ���
			*bIn = true;

			VecA[0] = m_aMeshPos[nCntCol][0] - m_aMeshPos[nCntCol][1];
			VecA[1] = m_aMeshPos[nCntCol][1] - m_aMeshPos[nCntCol][2];
			VecA[2] = m_aMeshPos[nCntCol][2] - m_aMeshPos[nCntCol][3];
			VecA[3] = m_aMeshPos[nCntCol][3] - m_aMeshPos[nCntCol][0];

			for (int nCnt = 0; nCnt < MAX_VTX; nCnt++)
			{// �v���C���[�ƃ��b�V���̃x�N�g��
				VecC[nCnt] = (*pPos - m_aMeshPos[nCntCol][nCnt]);

				fVecLine[nCnt] = (VecC[nCnt].z * VecA[nCnt].x) - (VecC[nCnt].x * VecA[nCnt].z);
				if (fVecLine[nCnt] < 0)
				{// �ǂꂩ���͈͊O
					*bIn = false;
				}
			}
			if (*bIn)
			{// ������
				fHeight = m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y + m_pos.y;
				return fHeight;
			}
		}
	}
	return fHeight;
}
//==============================================================================================================================
// ����ݒ�
//==============================================================================================================================
void CSceneX::ScanCollision(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FILE_SCAN, "r");
	int nType = 0;

	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{// ���������
		for (int nCntCollision = 0; nCntCollision < m_pModelData[nCntMat].m_nNumCollision; nCntCollision++)
		{// ����폜
			if (m_pModelData[nCntMat].m_pCollision != NULL)
			{
				delete[] m_pModelData[nCntMat].m_pCollision;
				m_pModelData[nCntMat].m_pCollision = NULL;
			}
		}
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "COLLISIONSET") == 0)
			{// ����Z�b�g
				int nNumCube = 0;
				while (strcmp(cData, "END_COLLISIONSET") != 0)
				{// ����I���܂Ń��[�v
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nType);
					}
					else if (strcmp(cData, "NUMCOLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &m_pModelData[nType].m_nNumCollision);
						m_pModelData[nType].m_pCollision = new CollisionData[m_pModelData[nType].m_nNumCollision]();
					}
					else if (strcmp(cData, "CUBE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f %f %f %f", &cData[0], &cData[0],
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.x,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.y,
							&m_pModelData[nType].m_pCollision[nNumCube].m_ofset.z,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fWidth,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fHeight,
							&m_pModelData[nType].m_pCollision[nNumCube].m_fDepth);
						// �傫������ő�l�ŏ��l���v�Z
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax = D3DXVECTOR3(m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, m_pModelData[nType].m_pCollision[nNumCube].m_fHeight, m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;
						m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin = D3DXVECTOR3(-m_pModelData[nType].m_pCollision[nNumCube].m_fWidth / 2, 0.0f, -m_pModelData[nType].m_pCollision[nNumCube].m_fDepth / 2) + m_pModelData[nType].m_pCollision[nNumCube].m_ofset;

						// �傫������p�x�Ƌ������v�Z[ �����@�����@�E���@�E�� ]
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[0] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[0] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[1] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[1] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[2] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[2] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.z));
						m_pModelData[nType].m_pCollision[nNumCube].m_afAngle[3] = atan2f(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x, m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z);
						m_pModelData[nType].m_pCollision[nNumCube].m_afLength[3] = sqrtf((m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x)*(m_pModelData[nType].m_pCollision[nNumCube].m_vtxMax.x) + (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z)* (m_pModelData[nType].m_pCollision[nNumCube].m_vtxMin.z));

						nNumCube++;

					}
				}

			}
		}
		fclose(pFile);// �I��
	}
}
//==============================================================================================================================
// ���萶��
//==============================================================================================================================
void CSceneX::SetCollision(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{// ���肪������̂Ȃ�
		if (m_aMeshPos == NULL)
		{// �u����v��������
			m_aMeshPos = new D3DXVECTOR3*[m_pModelData[m_type].m_nNumCollision]();
		}
		if (m_apLine == NULL)
		{// �u����v��������
			m_apLine = new CLine**[m_pModelData[m_type].m_nNumCollision]();
		}
		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// ���蕔����
			if (m_aMeshPos[nCntCol] == NULL)
			{// �u���_�v��������
				m_aMeshPos[nCntCol] = new D3DXVECTOR3[MAX_VTX]();
			}
			if (m_apLine[nCntCol] == NULL)
			{// �u���_�v��������
				m_apLine[nCntCol] = new CLine*[MAX_VTX_LINE]();
			}
			for (int meshCnt = 0; meshCnt < MAX_VTX; meshCnt++)
			{// �S���_�v�Z
			 // ���_���W���p�x�Ƌ����Ōv�Z
				m_aMeshPos[nCntCol][meshCnt] = D3DXVECTOR3(sinf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt],
					0.0f,
					cosf(m_pModelData[m_type].m_pCollision[nCntCol].m_afAngle[meshCnt] + m_rot.y)*m_pModelData[m_type].m_pCollision[nCntCol].m_afLength[meshCnt]) + m_pos;

				// ���C�������@������
				m_apLine[nCntCol][meshCnt] = CLine::Create(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f), m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f));
				m_apLine[nCntCol][4 + meshCnt] = CLine::Create(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f), m_aMeshPos[nCntCol][(1 + meshCnt) % 4] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f));
				m_apLine[nCntCol][8 + meshCnt] = CLine::Create(m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMin.y, 0.0f), m_aMeshPos[nCntCol][meshCnt] + D3DXVECTOR3(0.0f, m_pModelData[m_type].m_pCollision[nCntCol].m_vtxMax.y, 0.0f));
			}
		}
	}

}
//==============================================================================================================================
// ����폜
//==============================================================================================================================
void CSceneX::DeleteCollision(void)
{
	if (m_pModelData[m_type].m_pCollision != NULL)
	{// ���肪������̂Ȃ�

		for (int nCntCol = 0; nCntCol < m_pModelData[m_type].m_nNumCollision; nCntCol++)
		{// ���蕪
			if (m_apLine != NULL)
			{// ���C�����폜
				if (m_apLine[nCntCol] != NULL)
				{// ����̌����폜
					for (int nLine = 0; nLine < MAX_VTX_LINE; nLine++)
					{// �����̂̃��C�����폜
						if (m_apLine[nCntCol][nLine] != NULL)
						{
							m_apLine[nCntCol][nLine]->Uninit();
							m_apLine[nCntCol][nLine] = NULL;
						}
					}
					delete[] m_apLine[nCntCol];
					m_apLine[nCntCol] = NULL;
				}
			}
			if (m_aMeshPos != NULL)
			{// ����p���W���폜
				if (m_aMeshPos[nCntCol] != NULL)
				{// ����̌����폜
					delete[] m_aMeshPos[nCntCol];
				}
			}
		}
		if (m_apLine != NULL)
		{// ���C�����폜
			delete m_apLine;
			m_apLine = NULL;
		}
		if (m_aMeshPos != NULL)
		{// ����p���W���폜
			delete m_aMeshPos;
			m_aMeshPos = NULL;
		}
	}
}
//==============================================================================================================================
// �ʒu�ݒ�
//==============================================================================================================================
void CSceneX::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// �ړ��ʂ̐ݒ�
//==============================================================================================================================
void CSceneX::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}
//==============================================================================================================================
// ��]�̐ݒ�
//==============================================================================================================================
void CSceneX::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// �}�g���b�N�X�̐ݒ�
//==============================================================================================================================
void CSceneX::SetMtx(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}
//==============================================================================================================================
// �F
//==============================================================================================================================
void CSceneX::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}
//==============================================================================================================================
// �^�C�v�ݒ�
//==============================================================================================================================
void CSceneX::SetType(const int type)
{
	m_type = type;
}
//==============================================================================================================================
// �J����
//==============================================================================================================================
void CSceneX::SetCameraID(const int nID)
{
	m_nCameraID = nID;
}
//==============================================================================================================================
// �e
//==============================================================================================================================
void CSceneX::SetShadowPos(const D3DXVECTOR3 pos)
{
	m_ShadowPos = pos;
}

