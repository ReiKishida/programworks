//=============================================================================
//
// ���b�V���t�B�[���h���� [scene_meshfield.cpp]
// Author : �ݓc��(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "scene_meshfield.h"
#include "debugproc.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSceneMeshField::m_apTexture[FIELD_TEXTURE] = {}; //���L�e�N�X�`���̃|�C���^
int CSceneMeshField::m_nSquareNum = 0;	//�l�p�ʂ̐�
int CSceneMeshField::m_nVecNum = 0;	//�l�p�ʂ̐�
D3DXVECTOR3 CSceneMeshField::Vertexpos[9] = {};
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�N���A�Ȓl����
//=============================================================================
CSceneMeshField::CSceneMeshField(int nPriority, OBJTYPE type) :CScene(nPriority, type)
{

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�����̍Ō�ɌĂяo�����
//=============================================================================
CSceneMeshField::~CSceneMeshField()
{
}
//=============================================================================
// �֐����F�e�N�X�`���̓ǂݍ���
// �֐��̊T�v�F�e�N�X�`����ǂݍ���
//=============================================================================
HRESULT CSceneMeshField::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field_wall/field001.jpg", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field_wall/field002.jpg", &m_apTexture[1]);

	return S_OK;
}
//=============================================================================
// �֐����F�e�N�X�`���̔j��
// �֐��̊T�v�F�e�N�X�`����j������
//=============================================================================
void CSceneMeshField::Unload(void)
{
	for (int nCntTex = 0; nCntTex < FIELD_TEXTURE; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �֐����F���b�V���t�B�[���h�̐�������
// �֐��̊T�v�F���b�V���t�B�[���h�𐶐�����
//=============================================================================
CSceneMeshField * CSceneMeshField::Create(D3DXVECTOR3 pos, int sprit_X, int sprit_Z, float fWidth, float fHeight, FIELDTYPE type)
{
	CSceneMeshField  *pSceneMeshfield;
	pSceneMeshfield = new CSceneMeshField;//�V�[���N���X�̐���
	if (pSceneMeshfield != NULL)
	{
		pSceneMeshfield->m_pos = pos;
		pSceneMeshfield->m_nMesh_X = sprit_X;
		pSceneMeshfield->m_nMesh_Z = sprit_Z;
		pSceneMeshfield->m_fMeshWidth = fWidth;
		pSceneMeshfield->m_fMeshDepth = fHeight;
		pSceneMeshfield->m_type = type;
		pSceneMeshfield->Init();

	}
	return pSceneMeshfield;
}

//=============================================================================
// �֐����F���b�V���t�B�[���h�̏���������
// �֐��̊T�v�F���_���ƃ|���S�����ɒ���
//=============================================================================
HRESULT CSceneMeshField::Init(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//���_��
	m_nVertexNum = (m_nMesh_X + 1) * (m_nMesh_Z + 1);
	//�C���f�b�N�X��
	m_nIndexNum = (m_nMesh_X * m_nMesh_Z) * 2 + (4 * (m_nMesh_Z - 1)) + 2;
	//�|���S����
	m_nPolygonNum = (m_nMesh_X * m_nMesh_Z) * 2 + (4 * (m_nMesh_Z - 1));
	//�l�p�̖ʂ̐�
	m_nSquareNum = (m_nMesh_X * m_nMesh_Z);
	//�͂��x�N�g���̐�
	m_nVecNum = 6 * m_nSquareNum;

	MakeVertex(pDevice);
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
// �֐����F���b�V���t�B�[���h�̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�̔j��
//=============================================================================
void CSceneMeshField::Uninit(void)
{
	for (int nCntTex = 0; nCntTex < FIELD_TEXTURE; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
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
// �֐����F���b�V���t�B�[���h�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CSceneMeshField::Update(void)
{

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;




	//CDebugProc::Print("���_�� : %d\n", m_nVertexNum);
	//CDebugProc::Print("�|���S���̐� : %d\n", m_nPolygonNum);
	//CDebugProc::Print("�C���f�b�N�X�̐� : %d\n", m_nVertexNum);
	//CDebugProc::Print("�l�p�̖ʂ̐� : %d\n", m_nSquareNum);


	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCntVertex = 0; nCntVertex < m_nVertexNum; nCntVertex++)
	//{
	//	CDebugProc::Print("pVtx[%d].pos : %.1f ,%.1f, %.1f \n", nCntVertex, pVtx[nCntVertex].pos.x, pVtx[nCntVertex].pos.y, pVtx[nCntVertex].pos.z);
	//}

	//CDebugProc::Print("\n");

	//for (int nCntVertex = 0; nCntVertex < m_nVertexNum; nCntVertex++)
	//{
	//	CDebugProc::Print("pVtx[%d].nor : %.1f ,%.1f, %.1f \n", nCntVertex, pVtx[nCntVertex].nor.x, pVtx[nCntVertex].nor.y, pVtx[nCntVertex].nor.z);
	//}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �֐����F���b�V���t�B�[���h�̕`��
// �֐��̊T�v�F���_���ƃ|���S�����ɒ���
//=============================================================================
void CSceneMeshField::Draw(void)
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
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, m_apTexture[m_type]);


	// ���b�V���t�B�[���h�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nIndexNum, 0, m_nPolygonNum);


}


//=============================================================================
// �֐����F���_���̍쐬
// �֐��̊T�v�F�Ƃ肠�����͒��_��u������
//=============================================================================
void CSceneMeshField::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{



	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	//�@���̑���p
	D3DXVECTOR3 anor[2000];
	int nCntnor = 0;
	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMesh_Z = 0; nCntMesh_Z <= m_nMesh_Z; nCntMesh_Z++)
	{
		for (int nCntMesh_X = 0; nCntMesh_X <= m_nMesh_X; nCntMesh_X++)
		{
			int nNum;
			nNum = (nCntMesh_Z * (m_nMesh_X + 1)) + nCntMesh_X;

			if (nNum == 0 || nNum == 5)
			{
				pVtx[(nCntMesh_Z * (m_nMesh_X + 1)) + nCntMesh_X].pos = D3DXVECTOR3(-((m_fMeshWidth * m_nMesh_X) / 2) + (m_fMeshWidth * nCntMesh_X),
					0.0f,
					(m_nMesh_Z * m_fMeshDepth) / 2 - (m_fMeshDepth * nCntMesh_Z));
			}
			else
			{
				pVtx[(nCntMesh_Z * (m_nMesh_X + 1)) + nCntMesh_X].pos = D3DXVECTOR3(-((m_fMeshWidth * m_nMesh_X) / 2) + (m_fMeshWidth * nCntMesh_X),
					0.0f,
					(m_nMesh_Z * m_fMeshDepth) / 2 - (m_fMeshDepth * nCntMesh_Z));
			}

		}
	}

	for (int nCntMesh_Z = 0; nCntMesh_Z < m_nMesh_Z; nCntMesh_Z++)
	{
		for (int nCntMesh_X = 0; nCntMesh_X < m_nMesh_X; nCntMesh_X++)
		{

			int nNum0, nNum1, nNum2, nNum3;
			nNum0 = (nCntMesh_Z * (m_nMesh_X + 1)) + nCntMesh_X;
			nNum1 = ((nCntMesh_Z + 1) * (m_nMesh_X + 1)) + nCntMesh_X;
			nNum2 = ((nCntMesh_Z + 1) * (m_nMesh_X + 1)) + (nCntMesh_X + 1);
			nNum3 = (nCntMesh_Z * (m_nMesh_X + 1)) + (nCntMesh_X + 1);
			D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;

			D3DXVECTOR3 Vec0, vec1, vec2;
			D3DXVECTOR3 nor;


			//���_���̑��
			pPos0 = &pVtx[nNum0].pos;
			pPos1 = &pVtx[nNum1].pos;
			pPos2 = &pVtx[nNum2].pos;
			pPos3 = &pVtx[nNum3].pos;

			//Vec0�̎Z�o
			Vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			D3DXVec3Cross(&nor, &vec1, &Vec0);//Vec1��Vec0�̊O��
			D3DXVec3Normalize(&nor, &nor);//nor�̐��K��
			anor[nCntnor] = nor;
			nCntnor++;

			D3DXVec3Cross(&nor, &vec2, &vec1);//Vec2��Vec1�̊O��
			D3DXVec3Normalize(&nor, &nor);//nor�̐��K��
			anor[nCntnor] = nor;
			nCntnor++;




		}
	}

	//�e���_�̖@���Z�o
	for (int nCntMesh_Z = 0; nCntMesh_Z <= m_nMesh_Z; nCntMesh_Z++)
	{
		for (int nCntMesh_X = 0; nCntMesh_X <= m_nMesh_X; nCntMesh_X++)
		{
			int nNum = (nCntMesh_Z * (m_nMesh_X + 1)) + nCntMesh_X;

			if (nCntMesh_Z == 0)
			{//��[
				if (nCntMesh_X == 0)
				{//���[
					pVtx[nNum].nor = (anor[0] + anor[1]) / 2;
				}
				else if (nCntMesh_X == m_nMesh_X)
				{//�E�[
					pVtx[nNum].nor = anor[(m_nMesh_X * 2) - 1];
				}
				else
				{//����ȊO
					pVtx[nNum].nor = (anor[(nCntMesh_X * 2) - 1] + anor[nCntMesh_X * 2] + anor[(nCntMesh_X * 2) + 1]) / 3;
				}
			}

			else if (nCntMesh_Z == m_nMesh_Z)
			{//���[
				if (nCntMesh_X == 0)
				{//���[
					pVtx[nNum].nor = anor[((m_nMesh_Z - 1) * (m_nMesh_X * 2))];
				}
				else if (nCntMesh_X == m_nMesh_X)
				{//�E�[
					pVtx[nNum].nor = (anor[((m_nMesh_X * 2)* m_nMesh_Z) - 1] + anor[((m_nMesh_X * 2)* m_nMesh_Z) - 2]) / 2;
				}
				else
				{//����ȊO
					pVtx[nNum].nor = (anor[((m_nMesh_Z - 1) * (m_nMesh_X * 2)) + ((nCntMesh_X - 1) * 2)] + anor[(((m_nMesh_Z - 1) * (m_nMesh_X * 2)) + ((nCntMesh_X - 1) * 2)) + 1] + anor[(((m_nMesh_Z - 1) * (m_nMesh_X * 2)) + ((nCntMesh_X - 1) * 2)) + 2]) / 3;

				}
			}

			else
			{//����ȊO
				if (nCntMesh_X == 0)
				{//���[
					pVtx[nNum].nor = (anor[((nCntMesh_Z - 1) * (m_nMesh_X * 2))] + anor[((nCntMesh_Z) * (m_nMesh_X * 2)) + 1] + anor[(nCntMesh_Z  * (m_nMesh_X * 2)) + 1]) / 3;
				}
				else if (nCntMesh_X == m_nMesh_X)
				{//�E�[
					pVtx[nNum].nor = (anor[((m_nMesh_X * 2)* nCntMesh_Z) - 1] + anor[((m_nMesh_X * 2)* nCntMesh_Z) - 2] + anor[((m_nMesh_X * 2)* (nCntMesh_Z + 1)) - 1]) / 3;
				}
				else
				{//����ȊO
					pVtx[nNum].nor = (anor[((nCntMesh_Z - 1) * (nCntMesh_X * 2)) + ((nCntMesh_X - 1) * 2)] + anor[(((nCntMesh_Z - 1) * (nCntMesh_X * 2)) + ((nCntMesh_X - 1) * 2)) + 1] + anor[(((nCntMesh_Z - 1) * (nCntMesh_X * 2)) + ((nCntMesh_X - 1) * 2)) + 2] + anor[(nCntMesh_Z * (m_nMesh_X * 2)) + 1] + anor[(nCntMesh_Z * (m_nMesh_X * 2)) + 2] + anor[(nCntMesh_Z * (m_nMesh_X * 2)) + 3]) / 6;
				}
			}


			//���_�J���[
			pVtx[nNum].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (m_type == FIELDTYPE_YELLOW)
			{
				//�e�N�X�`�����W
				pVtx[nNum].tex = D3DXVECTOR2(nCntMesh_X * 1.0f, nCntMesh_Z * 1.0f);

			}
			else
			{
			//�e�N�X�`�����W
			pVtx[nNum].tex = D3DXVECTOR2(nCntMesh_X * 1.0f, nCntMesh_Z * 1.0f);

			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();



}
//=============================================================================
// �֐����F�C���f�b�N�X���̍쐬
// �֐��̊T�v�F�C���f�b�N�X�Ń|���S���𐶐�����
//=============================================================================
void CSceneMeshField::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	WORD *pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

				// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIndex = 0;	//�C���f�b�N�X�̃J�E���g

	for (int nCntIndex_Z = 0; nCntIndex_Z < m_nMesh_Z; nCntIndex_Z++)
	{
		for (int nCntIndex_X = 0; nCntIndex_X <= m_nMesh_X; nCntIndex_X++, nCntIndex++)
		{
			pIdx[0] = (m_nMesh_X + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;//2���i�߂�

			if (nCntIndex_X == m_nMesh_X && nCntIndex_Z < m_nMesh_Z - 1)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (m_nMesh_X + 1) + nCntIndex + 1;

				pIdx += 2;//2��
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}


//=============================================================================
// �֐����F�����̔���
// �֐��̊T�v�F�v���C���[�̂���ׂ�������Ԃ�
//=============================================================================
float CSceneMeshField::GetHeight(D3DXVECTOR3 pos)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	int nNum = 0;

	float fHeight = 0.0f;
	float fEmptyHeight = 0.0f;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMesh_Z = 0; nCntMesh_Z < m_nMesh_Z; nCntMesh_Z++)
	{
		for (int nCntMesh_X = 0; nCntMesh_X < m_nMesh_X; nCntMesh_X++)
		{
			int nNum0, nNum1, nNum2, nNum3;

			nNum0 = (nCntMesh_Z * (m_nMesh_X + 1)) + nCntMesh_X;
			nNum1 = ((nCntMesh_Z + 1) * (m_nMesh_X + 1)) + nCntMesh_X;
			nNum2 = ((nCntMesh_Z + 1) * (m_nMesh_X + 1)) + (nCntMesh_X + 1);
			nNum3 = (nCntMesh_Z * (m_nMesh_X + 1)) + (nCntMesh_X + 1);

			D3DXVECTOR3 Pos[4];
			D3DXVECTOR3 VecA[6];
			D3DXVECTOR3 Vec_0, Vec_1;
			D3DXVECTOR3 nor;
			D3DXVECTOR3 VecC[4];
			D3DXVECTOR3 VecP;
			D3DXVECTOR3 nor0, nor1, nor2, nor3;
			float fVecLine[6];

			//�ʒu���̑��
			Pos[0] = pVtx[nNum0].pos;
			Pos[1] = pVtx[nNum1].pos;
			Pos[2] = pVtx[nNum2].pos;
			Pos[3] = pVtx[nNum3].pos;

			//�O�ϗp�̃x�N�g��
			VecA[0] = Pos[3] - Pos[0];
			VecA[1] = Pos[2] - Pos[3];
			VecA[2] = Pos[1] - Pos[2];
			VecA[3] = Pos[0] - Pos[1];
			VecA[4] = Pos[2] - Pos[0];
			VecA[5] = Pos[0] - Pos[2];

			//�v���C���[���璸�_�ւ̃x�N�g��
			VecC[0] = pos - Pos[0];
			VecC[1] = pos - Pos[3];
			VecC[2] = pos - Pos[2];
			VecC[3] = pos - Pos[1];

			//����p�̊O��
			fVecLine[0] = (VecA[0].z * VecC[0].x) - (VecA[0].x * VecC[0].z);
			fVecLine[1] = (VecA[1].z * VecC[1].x) - (VecA[1].x * VecC[1].z);
			fVecLine[2] = (VecA[2].z * VecC[2].x) - (VecA[2].x * VecC[2].z);
			fVecLine[3] = (VecA[3].z * VecC[3].x) - (VecA[3].x * VecC[3].z);
			fVecLine[4] = (VecA[4].z * VecC[0].x) - (VecA[4].x * VecC[0].z);
			fVecLine[5] = (VecA[5].z * VecC[2].x) - (VecA[5].x * VecC[2].z);

			//���_���i�[�p
			int nVertex[3];


			if (fVecLine[5] >= 0 && fVecLine[0] >= 0 && fVecLine[1] >= 0)
			{


				nVertex[0] = nNum3;
				nVertex[1] = nNum2;
				nVertex[2] = nNum0;

				fEmptyHeight = pVtx[nVertex[0]].pos.y;
				Pos[3].y -= fEmptyHeight;
				Pos[2].y -= fEmptyHeight;
				Pos[0].y -= fEmptyHeight;

				Vec_0 = pVtx[nVertex[1]].pos - pVtx[nVertex[0]].pos;
				Vec_1 = pVtx[nVertex[2]].pos - pVtx[nVertex[0]].pos;

				//CDebugProc::Print("Vec_0:%.1f, %.1f, %.1f\n", Vec_0.x, Vec_0.y, Vec_0.z);
				//CDebugProc::Print("Vec_1:%.1f, %.1f, %.1f\n", Vec_1.x, Vec_1.y, Vec_1.z);


				D3DXVec3Cross(&nor0, &Vec_0, &Vec_1);//Vec0��Vec1�̊O��
				D3DXVec3Normalize(&nor0, &nor0);//nor0�̐��K��
												//CDebugProc::Print("nor0:%.1f, %.1f, %.1f\n", nor0.x, nor0.y, nor0.z);

				VecP = pos - pVtx[nVertex[0]].pos;
				//CDebugProc::Print("VecC(%d->player) :%.1f, %.1f, %.1f\n", nVertex[0], VecP.x, VecP.y, VecP.z);

				VecP.y = (-(nor0.x * VecP.z) - (nor0.z * VecP.x) / nor0.y + fEmptyHeight);
				fHeight = VecP.y;


			}
			else if (fVecLine[4] >= 0 && fVecLine[2] >= 0 && fVecLine[3] >= 0)
			{
				//CDebugProc::Print("nNum0:%d\n", nNum0);
				//CDebugProc::Print("nNum1:%d\n", nNum1);
				//CDebugProc::Print("nNum2:%d\n", nNum2);

				nVertex[0] = nNum1;
				nVertex[1] = nNum0;
				nVertex[2] = nNum2;

				fEmptyHeight = pVtx[nVertex[0]].pos.y;
				Pos[1].y -= fEmptyHeight;
				Pos[0].y -= fEmptyHeight;
				Pos[2].y -= fEmptyHeight;

				Vec_0 = pVtx[nVertex[1]].pos - pVtx[nVertex[0]].pos;
				Vec_1 = pVtx[nVertex[2]].pos - pVtx[nVertex[0]].pos;

				//	CDebugProc::Print("Vec_0:%.1f, %.1f, %.1f\n", Vec_0.x, Vec_0.y, Vec_0.z);
				//CDebugProc::Print("Vec_1:%.1f, %.1f, %.1f\n", Vec_1.x, Vec_1.y, Vec_1.z);



				D3DXVec3Cross(&nor0, &Vec_0, &Vec_1);//Vec0��Vec1�̊O��
				D3DXVec3Normalize(&nor0, &nor0);//nor0�̐��K��
												//	CDebugProc::Print("nor0:%.1f, %.1f, %.1f\n", nor0.x, nor0.y, nor0.z);

				VecP = pos - pVtx[nVertex[0]].pos;
				//	CDebugProc::Print("VecC(%d ->player):%.1f, %.1f, %.1f\n", nVertex[0], VecP.x, VecP.y, VecP.z);

				VecP.y = (-(nor0.x * VecP.z) - (nor0.z * VecP.x) / nor0.y + fEmptyHeight);
				fHeight = VecP.y;
			}
		}
	}

	CDebugProc::Print("�v���C���[�̍���:%.1f\n", fHeight);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return fHeight;
}

