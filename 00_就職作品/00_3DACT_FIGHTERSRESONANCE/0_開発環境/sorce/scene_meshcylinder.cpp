//=============================================================================
//
// ���b�V���V�����_�[���� [scene_meshcylinder.cpp]
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
#include "scene_meshcylinder.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR (125)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSceneMeshCylinder::m_pTexture = NULL; //���L�e�N�X�`���̃|�C���^

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�N���A�Ȓl����
//=============================================================================
CSceneMeshCylinder::CSceneMeshCylinder(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�����̍Ō�ɌĂяo�����
//=============================================================================
CSceneMeshCylinder::~CSceneMeshCylinder()
{

}


//=============================================================================
// �֐����F���b�V���V�����_�[�̐�������
// �֐��̊T�v�F���b�V���t�B�[���h�𐶐�����
//=============================================================================
CSceneMeshCylinder *CSceneMeshCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_H, int sprit_V, float fRadius,float fHeight, CYLINDERTYPE type)
{
	CSceneMeshCylinder  *pSceneMeshCylinder;
	pSceneMeshCylinder = new CSceneMeshCylinder;//�V�[���N���X�̐���
	if (pSceneMeshCylinder != NULL)
	{
		pSceneMeshCylinder->m_pos = pos;			//�ʒu���̑��
		pSceneMeshCylinder->m_rot = rot;			//�p�x���̑��
		pSceneMeshCylinder->m_nMesh_H = sprit_H;	//���������̕���
		pSceneMeshCylinder->m_nMesh_V = sprit_V;	//���������̕���
		pSceneMeshCylinder->m_fRadius = fRadius;	//���a�̑��
		pSceneMeshCylinder->m_fHeight = fHeight;
		pSceneMeshCylinder->m_type = type;
		pSceneMeshCylinder->Init();
	}
	return pSceneMeshCylinder;
}

//=============================================================================
// �֐����F���b�V���V�����_�[�̏���������
// �֐��̊T�v�F���_���ƃ|���S�����ɒ���
//=============================================================================
HRESULT CSceneMeshCylinder::Init(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//���_��
	m_nVertexNum = (m_nMesh_H + 1) * (m_nMesh_V + 1);
	//�C���f�b�N�X��
	m_nIndexNum = (m_nMesh_H * m_nMesh_V) * 2 + (4 * (m_nMesh_V - 1)) + 2;
	//�|���S����
	m_nPolygonNum = (m_nMesh_H * m_nMesh_V) * 2 + (4 * (m_nMesh_V - 1));

	MakeVertex(pDevice);
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
// �֐����F���b�V���V�����_�[�̏I������
// �֐��̊T�v�F�e�N�X�`���A���_�o�b�t�@�̔j��
//=============================================================================
void CSceneMeshCylinder::Uninit(void)
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
// �֐����F���b�V���V�����_�[�̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CSceneMeshCylinder::Update(void)
{
	m_rot.y += 0.02f;

}

//=============================================================================
// �֐����F���b�V���V�����_�[�̕`��
// �֐��̊T�v�F���_���ƃ|���S�����ɒ���
//=============================================================================
void CSceneMeshCylinder::Draw(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//�}�g���b�N�X���

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ���ʂ��J�����O


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

	 // ���b�V���V�����_�[�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPolygonNum);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
}


//=============================================================================
// �֐����F���_���̍쐬
// �֐��̊T�v�F�Ƃ肠�����͒��_��u������
//=============================================================================
void CSceneMeshCylinder::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / m_nMesh_H;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < m_nMesh_V + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < m_nMesh_H + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * m_fRadius;
			if (nCntLife_H % 2 == 0)
			{
				pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.y = (m_fHeight -10.0f) * nCntLife_V;
			}
			else
			{
				pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.y = m_fHeight * nCntLife_V;

			}
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * m_fRadius;
			//�@���̐ݒ�
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].col = D3DXCOLOR(1.0f, 0.0, 0.0f, 0.4f);

			//�e�N�X�`�����W
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (m_nMesh_H / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();


	////���_�o�b�t�@�����b�N
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCntMeshCylinder_V = 0; nCntMeshCylinder_V < m_nMesh_V + 1; nCntMeshCylinder_V++)
	//{
	//	for (int nCntMeshCylinder_H = 0; nCntMeshCylinder_H < m_nMesh_H + 1; nCntMeshCylinder_H++)
	//	{
	//		pVtx[0].pos.x = sinf(D3DX_PI * (nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)))) * m_fRadius;
	//		pVtx[0].pos.y = m_fHeight * nCntMeshCylinder_V;
	//		pVtx[0].pos.z = cosf(D3DX_PI * (nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)))) * m_fRadius;
	//		//�@���̐ݒ�
	//		pVtx[0].nor = D3DXVECTOR3(sinf(-D3DX_PI * (0.25f * nCntMeshCylinder_H)), 0.0f, cosf(D3DX_PI * (0.25f * nCntMeshCylinder_H)));
	//		D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	//		//���_�J���[
	//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	//		//�e�N�X�`�����W
	//		pVtx[0].tex = D3DXVECTOR2(nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)), nCntMeshCylinder_V * -1.0f);


	//		pVtx += 1;
	//	}
	//}
	////���_�o�b�t�@���A�����b�N
	//m_pVtxBuff->Unlock();

}
//=============================================================================
// �֐����F�C���f�b�N�X���̍쐬
// �֐��̊T�v�F�C���f�b�N�X�Ń|���S���𐶐�����
//=============================================================================
void CSceneMeshCylinder::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//�C���f�b�N�X�̃J�E���g

						//�C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < m_nMesh_V; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= m_nMesh_H; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (m_nMesh_H + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == m_nMesh_H && nCntIndex_V <= m_nMesh_V)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (m_nMesh_H + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

}

//=============================================================================
// �֐����F��O�ʂ̕ǂ̓����蔻��
// �֐��̊T�v�F��O�ʂ��痈�����̂̓����蔻����s��
//=============================================================================
bool CSceneMeshCylinder::CollisionCylinder(D3DXVECTOR3 *pos)
{
	bool bCollision = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;


	fxpos = (m_pos.x - pos->x) * (m_pos.x - pos->x);
	fzpos = (m_pos.z - pos->z) * (m_pos.z - pos->z);
	fRadius = m_fRadius * m_fRadius;

#ifdef _DEBUG

	//CDebugProc::Print("fxpos: %.1f\n", fxpos);
	//CDebugProc::Print("fzpos: %.1f\n", fzpos);

	//CDebugProc::Print("fRadius: %.1f\n", fRadius);
#endif
	if (fxpos + fzpos <= fRadius)
	{
#ifdef _DEBUG
		//CDebugProc::Print("���a��\n");
#endif

		if (m_type == CYLINDERTYPE_GOAL)
		{
			bCollision = true;
		}
	}

	return bCollision;
}

