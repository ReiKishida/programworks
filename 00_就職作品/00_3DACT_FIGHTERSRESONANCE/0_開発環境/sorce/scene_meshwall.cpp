//=============================================================================
//
// ���b�V���E�H�[������ [scene_meshfield.cpp]
// Author :�ݓc��(Kishida Rei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "scene_meshwall.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define WALL_FILENAME ("data/TEXT/wall_data.txt")
#define WALL_FILENAME_TUTO ("data/TEXT/wall_data_Tuto.txt")
#define MAX_CHAR (125)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSceneMeshWall::m_pTexture = NULL; //���L�e�N�X�`���̃|�C���^

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�N���A�Ȓl����
//=============================================================================
CSceneMeshWall::CSceneMeshWall(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�����̍Ō�ɌĂяo�����
//=============================================================================
CSceneMeshWall::~CSceneMeshWall()
{

}

//=============================================================================
// �֐����F�e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F�e�N�X�`����ǂݍ���
//=============================================================================
HRESULT CSceneMeshWall::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field_wall/wall000.jpg", &m_pTexture);	//�ǂ̃e�N�X�`��
	return S_OK;
}
//=============================================================================
// �֐����F�e�N�X�`���̔j��
// �֐��̊T�v�F�e�N�X�`����j������
//=============================================================================
void CSceneMeshWall::Unload(void)
{

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}

//=============================================================================
// �֐����F���b�V���E�H�[���̐�������
// �֐��̊T�v�F���b�V���t�B�[���h�𐶐�����
//=============================================================================
CSceneMeshWall *CSceneMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_X, int sprit_Y, float Width, float Height, WALLTYPE type)
{
	CSceneMeshWall  *pSceneMeshWall;
	pSceneMeshWall = new CSceneMeshWall;//�V�[���N���X�̐���
	if (pSceneMeshWall != NULL)
	{
		pSceneMeshWall->m_pos = pos;
		pSceneMeshWall->m_rot = rot;
		pSceneMeshWall->m_nMesh_X = sprit_X;
		pSceneMeshWall->m_nMesh_Y = sprit_Y;
		pSceneMeshWall->m_fMeshWidth = Width;
		pSceneMeshWall->m_fMeshHeight = Height;
		pSceneMeshWall->m_type = type;
		pSceneMeshWall->Init();
	}
	return pSceneMeshWall;
}

//=============================================================================
// �֐����F���b�V���E�H�[���̏���������
// �֐��̊T�v�F���_���ƃ|���S�����ɒ���
//=============================================================================
HRESULT CSceneMeshWall::Init(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//���_��
	m_nVertexNum = (m_nMesh_X + 1) * (m_nMesh_Y + 1);
	//�C���f�b�N�X��
	m_nIndexNum = (m_nMesh_X * m_nMesh_Y) * 2 + (4 * (m_nMesh_Y - 1)) + 2;
	//�|���S����
	m_nPolygonNum = (m_nMesh_X * m_nMesh_Y) * 2 + (4 * (m_nMesh_Y - 1));

	MakeVertex(pDevice);
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
// �֐����F���b�V���E�H�[���̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�̔j��
//=============================================================================
void CSceneMeshWall::Uninit(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//���g�̔j��
	Release();
}


//=============================================================================
// �֐����F���b�V���E�H�[���̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CSceneMeshWall::Update(void)
{


}

//=============================================================================
// �֐����F���b�V���E�H�[���̕`��
// �֐��̊T�v�F���_���ƃ|���S�����ɒ���
//=============================================================================
void CSceneMeshWall::Draw(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//�}�g���b�N�X���

								// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	if (m_type == WALLTYPE_NORMAL)
	{//�ʏ�̕ǂł͂Ȃ��Ƃ�
	 // ���b�V���E�H�[���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPolygonNum);
	}
}

//=============================================================================
// �֐����F���_���̍쐬
// �֐��̊T�v�F�Ƃ肠�����͒��_��u������
//=============================================================================
void CSceneMeshWall::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall000.jpg", &m_pTexture);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);



	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntMesh_Y = 0; nCntMesh_Y <= m_nMesh_Y; nCntMesh_Y++)
	{
		for (int nCntMesh_X = 0; nCntMesh_X <= m_nMesh_X; nCntMesh_X++)
		{
			// ���_���̐ݒ�

			pVtx[0].pos = D3DXVECTOR3(-((m_fMeshWidth * m_nMesh_X) / 2) + (m_fMeshWidth * nCntMesh_X),
				(m_nMesh_Y * m_fMeshHeight) / 2 - (m_fMeshHeight * nCntMesh_Y),
				0.0f);

			//�@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntMesh_X * 1.0f, nCntMesh_Y * 1.0f);

			pVtx += 1;
		}
	}


	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();



}
//=============================================================================
// �֐����F�C���f�b�N�X���̍쐬
// �֐��̊T�v�F�C���f�b�N�X�Ń|���S���𐶐�����
//=============================================================================
void CSceneMeshWall::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

				// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD)*m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);




	int nCntIndex = 0;	//�C���f�b�N�X�̃J�E���g

	for (int nCntIndex_Z = 0; nCntIndex_Z < m_nMesh_Y; nCntIndex_Z++)
	{
		for (int nCntIndex_X = 0; nCntIndex_X <= m_nMesh_X; nCntIndex_X++, nCntIndex++)
		{
			pIdx[0] = (m_nMesh_X + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex_X == m_nMesh_X && nCntIndex_Z < m_nMesh_Y - 1)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (m_nMesh_X + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();



}

//=============================================================================
// �֐����F��O�ʂ̕ǂ̓����蔻��
// �֐��̊T�v�F��O�ʂ��痈�����̂̓����蔻����s��
//=============================================================================
void CSceneMeshWall::CollisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posold, float fRadius)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	D3DXVECTOR3 VecA;			//�ǂ̃x�N�g��
	D3DXVECTOR3 VecC;			//�ǂ�(�v���C���[�܂��͓G��)�ʒu�̃x�N�g��
	D3DXVECTOR3 Vtxpos_Start_X;	//�n�_
	D3DXVECTOR3 Vtxpos_End_X;	//�I�_
	D3DXVECTOR3 Vtxpos_Start_Z;	//�n�_
	D3DXVECTOR3 Vtxpos_End_Z;	//�I�_

	float VecLine;				//����p�̋��E��

								// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�����̎Z�o
	Vtxpos_Start_X = pVtx[0].pos + m_pos;
	Vtxpos_End_X = pVtx[m_nVertexNum - 1].pos + m_pos;

	//�c���̎Z�o
	Vtxpos_Start_Z.x = m_pos.x;
	Vtxpos_Start_Z.y = m_pos.x;
	Vtxpos_Start_Z.z = m_pos.z + pVtx[m_nVertexNum - 1].pos.x;
	Vtxpos_End_Z.x = m_pos.x;
	Vtxpos_End_Z.y = m_pos.x;
	Vtxpos_End_Z.z = m_pos.z + pVtx[0].pos.x;


	if (m_rot.y == 0.0f)
	{//��O��
		if (pos->x >= Vtxpos_Start_X.x && pos->x <= Vtxpos_End_X.x)
		{//�ǂ͈̔�
			if (posold->z <= m_pos.z)
			{

				//VecA�̌v�Z
				VecA = Vtxpos_End_X - Vtxpos_Start_X;
				D3DXVec3Normalize(&VecA, &VecA);//nor�̐��K��
				//VecC�̌v�Z
				VecC = *pos - Vtxpos_Start_X;
				D3DXVec3Normalize(&VecC, &VecC);//nor�̐��K��


				//�x�N�g���v�Z
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->z = posold->z;
				}
			}
		}
	}
	else if (m_rot.y == D3DX_PI)
	{//��O��
		if (pos->x >= Vtxpos_Start_X.x && pos->x <= Vtxpos_End_X.x)
		{//�ǂ͈̔�
			if (posold->z >= m_pos.z)
			{

				//VecA�̌v�Z
				VecA = Vtxpos_Start_X - Vtxpos_End_X;
				D3DXVec3Normalize(&VecA, &VecA);//nor�̐��K��
												//VecC�̌v�Z
				VecC = *pos - Vtxpos_End_X;
				D3DXVec3Normalize(&VecC, &VecC);//nor�̐��K��

												//�x�N�g���v�Z
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->z = posold->z;
				}
			}
		}

	}
	else if (m_rot.y == D3DX_PI * 0.5f)
	{//��O��
		if (pos->z <= Vtxpos_Start_Z.z && pos->z >= Vtxpos_End_Z.z)
		{//�ǂ͈̔�
			if (posold->x <= m_pos.x)
			{
				//VecA�̌v�Z
				VecA = Vtxpos_End_Z - Vtxpos_Start_Z;
				D3DXVec3Normalize(&VecA, &VecA);//nor�̐��K��
				//VecC�̌v�Z
				VecC = *pos - Vtxpos_Start_Z;
				D3DXVec3Normalize(&VecC, &VecC);//nor�̐��K��

												//�x�N�g���v�Z
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->x = posold->x;
				}
			}
		}

	}
	else if (m_rot.y == -D3DX_PI * 0.5f)
	{//��O��
		if (pos->z <= Vtxpos_Start_Z.z && pos->z >= Vtxpos_End_Z.z)
		{//�ǂ͈̔�
			if (posold->x >= m_pos.x)
			{
				//VecA�̌v�Z
				VecA = Vtxpos_Start_Z - Vtxpos_End_Z;
				D3DXVec3Normalize(&VecA, &VecA);//nor�̐��K��
				//VecC�̌v�Z
				VecC = *pos - Vtxpos_End_Z;
				D3DXVec3Normalize(&VecC, &VecC);//nor�̐��K��

				//�x�N�g���v�Z
				VecLine = (VecA.z * VecC.x) - (VecA.x * VecC.z);

				if (VecLine <= 0.0f)
				{
					pos->x =posold->x;
				}
			}
		}

	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �֐����F�ǂ̏��̓ǂݍ��ݏ���
// �֐��̊T�v�F�e�L�X�g�t�@�C������ǂ̏���ǂݍ���
//=============================================================================
void CSceneMeshWall::LoadWallText(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

	FILE *pFile = fopen(WALL_FILENAME, "r");		// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

													// �ǂݍ��ނ��߂̕ϐ��錾
	char acLine[MAX_CHAR];							// 1�s���ǂݍ��ޕϐ�
	char acData[MAX_CHAR];							// 1����ǂݍ��ޕϐ�
	int nCntWall = 0;								// �ǂݍ��ރ��f���̐�
	int nSprit_X = 0;								//���̕�����
	int nSprit_Y = 0;								//�c�̕�����
	float fWidth = 0.0f;							//����
	float fHeight = 0.0f;							//�c��
	WALLTYPE TypeWall;								// �g�p���郂�f���̃^�C�v
	int nMaxWall = 0;								// ���f���̍ő吔
	D3DXVECTOR3 pos;								// �ʒu������ϐ�
	D3DXVECTOR3 rot;								// ����������ϐ�

	if (pFile != NULL)
	{// �k���`�F�b�N
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[


			if (strcmp(acData, "WALLSET") == 0)
			{// "MODELSET"�̕����񂪂�������

				while (strcmp(acData, "END_WALLSET") != 0)
				{// "END_MODELSET"���܂ŏ����擾��������
					fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
					sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

					if (strcmp(acData, "POS") == 0)
					{// "POS"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// �ʒu���擾
					}
					else if (strcmp(acData, "ROT") == 0)
					{// "ROT"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// �������擾
																												//�e�L�X�g�̊p�x���ʂ�ɓ����v�Z
						rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
						rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
						rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
					}
					else if (strcmp(acData, "SPRIT_X") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_X);	// ���̕������̎擾

					}
					else if (strcmp(acData, "SPRIT_Y") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_Y);	// �c�̕������̎擾

					}
					else if (strcmp(acData, "WALL_WIDTH") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fWidth);	// �������擾

					}
					else if (strcmp(acData, "WALL_HEIGHT") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fHeight);	// �c�����擾

					}
					else if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &TypeWall);	// �g�p���郂�f���̔ԍ����擾
					}
				}
				//���f���̐���
				Create(pos, rot, nSprit_X, nSprit_Y, fWidth, fHeight, TypeWall);
			}
		}
		fclose(pFile);	// �J�����t�@�C�������
	}
}

//=============================================================================
// �֐����F�ǂ̏��̓ǂݍ��ݏ���
// �֐��̊T�v�F�e�L�X�g�t�@�C������ǂ̏���ǂݍ���
//=============================================================================
void CSceneMeshWall::LoadWallText_Tuto(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

	FILE *pFile = fopen(WALL_FILENAME_TUTO, "r");		// �ǂݍ��ރe�L�X�g�t�@�C���̃A�h���X

													// �ǂݍ��ނ��߂̕ϐ��錾
	char acLine[MAX_CHAR];							// 1�s���ǂݍ��ޕϐ�
	char acData[MAX_CHAR];							// 1����ǂݍ��ޕϐ�
	int nCntWall = 0;								// �ǂݍ��ރ��f���̐�
	int nSprit_X = 0;								//���̕�����
	int nSprit_Y = 0;								//�c�̕�����
	float fWidth = 0.0f;							//����
	float fHeight = 0.0f;							//�c��
	WALLTYPE TypeWall;								// �g�p���郂�f���̃^�C�v
	int nMaxWall = 0;								// ���f���̍ő吔
	D3DXVECTOR3 pos;								// �ʒu������ϐ�
	D3DXVECTOR3 rot;								// ����������ϐ�

	if (pFile != NULL)
	{// �k���`�F�b�N
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[


			if (strcmp(acData, "WALLSET") == 0)
			{// "MODELSET"�̕����񂪂�������

				while (strcmp(acData, "END_WALLSET") != 0)
				{// "END_MODELSET"���܂ŏ����擾��������
					fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
					sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[

					if (strcmp(acData, "POS") == 0)
					{// "POS"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// �ʒu���擾
					}
					else if (strcmp(acData, "ROT") == 0)
					{// "ROT"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// �������擾
																												//�e�L�X�g�̊p�x���ʂ�ɓ����v�Z
						rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
						rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
						rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
					}
					else if (strcmp(acData, "SPRIT_X") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_X);	// ���̕������̎擾

					}
					else if (strcmp(acData, "SPRIT_Y") == 0)
					{
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nSprit_Y);	// �c�̕������̎擾

					}
					else if (strcmp(acData, "WALL_WIDTH") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fWidth);	// �������擾

					}
					else if (strcmp(acData, "WALL_HEIGHT") == 0)
					{
						sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &fHeight);	// �c�����擾

					}
					else if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"�̕����񂪂�������
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &TypeWall);	// �g�p���郂�f���̔ԍ����擾
					}
				}
				//���f���̐���
				Create(pos, rot, nSprit_X, nSprit_Y, fWidth, fHeight, TypeWall);
			}
		}
		fclose(pFile);	// �J�����t�@�C�������
	}

}
