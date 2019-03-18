//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// ���f������ [model.cpp]
// Author:�� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "model.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "motion.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MODEL_NAME			"data//MODEL//01_head.x"		// �ǂݍ��ރe�N�X�`���̃t�@�C����
#define MODEL_MOVE_SPEED	(1.0f)
#define MOVE_INERTIA		(0.4f)

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �ÓI�����o�ϐ��錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �R���X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CModel::CModel()
{
	// �l���N���A

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �f�X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CModel::~CModel()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CModel::Init(void)
{
	/*m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;	*/
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;

	for(int nCnt = 0; nCnt < MAX_MODEL_TEX; nCnt++)
	{
		m_pTexture[nCnt] = NULL;
	}


	m_pParent = NULL;

	m_mtxWorld = {};

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Uninit(void)
{
	//UnLoad();
}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Update(void)
{
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Draw(D3DXMATRIX mtxParent)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	if (m_pParent != NULL)
	{ // �e�p�[�c��NULL����Ȃ������ꍇ�e�p�[�c�̃��[���h�}�g���b�N�X���擾����
		mtxParent = m_pParent->GetMtxWorld();
	}

	// �e�̃��[���h�}�g���b�N�X�𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//pMat->pTextureFilename[0]
	//g_pTextureModel
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// ���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);


}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�����p�`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Draw(D3DXMATRIX mtxParent, D3DXVECTOR3 posMotion, D3DXVECTOR3 rotMotion)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, (m_rot.y + rotMotion.y), (m_rot.x + rotMotion.x), (m_rot.z + rotMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, (m_pos.x + posMotion.x), (m_pos.y + posMotion.y), (m_pos.z + posMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	if (m_pParent != NULL)
	{ // �e�p�[�c��NULL����Ȃ������ꍇ�e�p�[�c�̃��[���h�}�g���b�N�X���擾����
		mtxParent = m_pParent->GetMtxWorld();
	}

	// �e�̃��[���h�}�g���b�N�X�𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//pMat->pTextureFilename[0]
	//g_pTextureModel
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// ���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�����p�`�揈��(���f����񎝂�����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::Draw(D3DXMATRIX mtxParent, D3DXVECTOR3 posMotion, D3DXVECTOR3 rotMotion, CMotionData::Model model)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, (m_rot.y + rotMotion.y), (m_rot.x + rotMotion.x), (m_rot.z + rotMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, (m_pos.x + posMotion.x), (m_pos.y + posMotion.y), (m_pos.z + posMotion.z));
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	if (m_pParent != NULL)
	{ // �e�p�[�c��NULL����Ȃ������ꍇ�e�p�[�c�̃��[���h�}�g���b�N�X���擾����
		mtxParent = m_pParent->GetMtxWorld();
	}

	// �e�̃��[���h�}�g���b�N�X�𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

	//pMat->pTextureFilename[0]
	//g_pTextureModel
	for (int nCntMat = 0; nCntMat < (int)model.nNumMat; nCntMat++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, model.pTexture[nCntMat]);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// ���f��(�p�[�c)�̕`��
		model.pMesh->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ��������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CModel *CModel::Create(const D3DXVECTOR3 pos)
{
	CModel *pModel = NULL;

	pModel = new CModel;	// CModel�^�̃C���X�^���X����
	if (pModel != NULL)
	{
		// ������
		pModel->Init();
		// ���f���ǂݍ���
		//pModel->Load();

		// �ʒu�ݒ�
		pModel->m_pos = pos;
	}
	else
	{ // �������m�ێ��s
		MessageBox(NULL, "�V�[��X�̃������m�ۂɎ��s���܂���", "�G���[", (MB_OK));
	}

	return pModel;
}
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���f���㏑������
// Author:yuki tanaka
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CModel::BindModel(const LPD3DXMESH mesh, const LPD3DXBUFFER buffMat, const DWORD numMat)
{
	//m_pMesh = mesh;
	//m_pBuffMat = buffMat;
	//m_nNumMat = numMat;

}
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//// ���f���ǂݍ��ݏ���
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//HRESULT CModel::Load(void)
//{
//	// �f�o�C�X�̎擾
//	CRenderer *pRenderer = NULL;
//	LPDIRECT3DDEVICE9 pDevice = NULL;
//	pRenderer = CManager::GetRenderer();
//
//	if (pRenderer != NULL)
//	{
//		pDevice = pRenderer->GetDevice();
//	}
//
//	D3DXMATERIAL *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
//
//	// ���f���̓ǂݍ���
//	D3DXLoadMeshFromX(MODEL_NAME,
//		D3DXMESH_SYSTEMMEM,
//		pDevice,
//		NULL,
//		&m_pBuffMat,
//		NULL,
//		&m_nNumMat,
//		&m_pMesh);
//
//	// �}�e���A���f�[�^�ւ̃|�C���^���擾
//	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
//	{
//		if (pMat[nCntMat].pTextureFilename != NULL)
//		{
//			D3DXCreateTextureFromFile(pDevice,
//				pMat[nCntMat].pTextureFilename,
//				&m_pTexture[nCntMat]);
//		}
//	}
//
//	return S_OK;
//}
//
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//// �e�N�X�`���J������
////=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//void CModel::UnLoad(void)
//{
//	// ���b�V���̊J��
//	if (m_pMesh != NULL)
//	{
//		m_pMesh->Release();
//		m_pMesh = NULL;
//	}
//	// �}�e���A���̊J��
//	if (m_pBuffMat != NULL)
//	{
//		m_pBuffMat->Release();
//		m_pBuffMat = NULL;
//	}
//}