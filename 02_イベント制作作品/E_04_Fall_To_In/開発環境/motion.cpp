//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// ���[�V�������� [motion.cpp]
// Author : �� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�

#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "sceneX.h"
#include "calculation.h"
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_LOAD_LINE (2560)
#define BLEND_FRAME		(5)		// ���[�V�����̃u�����h�ɕK�v�ȃt���[����

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �ÓI�����o�ϐ�
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
//CMotion::MotionData CMotion::m_apMotion[TYPE_MAX];				// ���[�V�����ւ̃|�C���^
//
//char *CMotion::m_apMotionInfo[TYPE_MAX] =
//{
//	// �A�h���X
//	"data//TEXT//motion.txt",		// �T���v��00
//};

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �R���X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotion::CMotion(int nPriority) : CScene(nPriority)
{
	// �l���N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pMotion = NULL;
	m_pCollision = NULL;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �f�X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotion::~CMotion()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ����������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotion::Init(void)
{
	// �I�u�W�F�N�g�^�C�v��ݒ�
	//CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	// �����l��ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = m_pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fSpeed = 0.5f;

	m_nMotionType = 0;
	m_nMotionTypeOld = m_nMotionType;

	// ���[�V���������擾
	CMotionData::Motion *pMotion = m_pMotionData->GetMotion(m_nMotionType);
	// �p�[�c�����擾
	int nNumParts = m_pMotionData->GetNumParts();

	for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
	{
		// ���f�������擾
		CMotionData::Model *pModel = m_pMotionData->GetModel(nCntParts);

		m_pModel[nCntParts] = CModel::Create(pModel->pos);
		m_pModel[nCntParts]->SetRot(pModel->rot);

		m_pModel[nCntParts]->SetMesh(pModel->pMesh);
		m_pModel[nCntParts]->SetBuffMat(pModel->pBuffMat);
		m_pModel[nCntParts]->SetNumMat(pModel->nNumMat);

		D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

		for (int nCntTex = 0; nCntTex < (int)pModel->nNumMat; nCntTex++)
		{
			m_pModel[nCntParts]->SetTexture(pModel->pTexture[nCntTex], nCntTex);
		}

		// �e�q�֌W�ݒ�
		int nInxParent = pModel->nIdxParent;
		if (nInxParent != -1)
		{
			m_pModel[nCntParts]->SetParent(m_pModel[nInxParent]);
		}
		else
		{
			m_pModel[nCntParts]->SetParent(NULL);
		}

	}

	// ���[�V�������[�u�N���X�𐶐�
	m_pMotion = CMotionMove::Create();

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotion::Uninit(void)
{
	Release();

}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotion::Update(void)
{
	m_nMotionTypeOld = m_nMotionType;
	m_posOld = m_pos;

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// �`�揈��
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotion::Draw(void)
{
	// �f�o�C�X�擾����
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

										// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �p�[�c�̕`��
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Draw(m_mtxWorld, m_pMotion->GetPos(nCntModel), m_pMotion->GetRot(nCntModel), *m_pMotionData->GetModel(nCntModel));
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�������[�u�̃R���X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionMove::CMotionMove()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�������[�u�̃f�X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionMove::~CMotionMove()
{

}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�������[�u�̏���������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotionMove::Init(void)
{
	m_nKeyCnt = 0;
	m_nFrameCounter = 0;
	m_nAllFrameCounter = 0;
	m_bBlend = false;

	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		m_pos[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_posAdd[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rotAdd[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�������[�u�̏I������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionMove::Uninit(void)
{


}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�������[�u�̃��[�V�����X�V����
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionMove::MotionUpdate(CMotionData *motionData, int nMotionType, int nMotionTypeOld)
{
	int nFrame = 0; // ���[�V�����̕����t���[�����L�^

	// ���[�V���������擾
	CMotionData::Motion *pMotion = motionData->GetMotion(nMotionType);
	// �p�[�c�����擾
	int nNumParts = motionData->GetNumParts();

	if (nMotionType != nMotionTypeOld)
	{ // ���݂̃��[�V�����ƑO��̃��[�V�������������
		m_nFrameCounter = 0;	// �t���[�������Z�b�g
		m_nKeyCnt = 0;			// �L�[�����Z�b�g
		m_nAllFrameCounter = 0;	// �S�Ẵt���[�������Z�b�g
		m_bBlend = true;		// �u�����h������
		nFrame = BLEND_FRAME;	// �u�����h�̃t���[����
	}
	else
	{ // �ʏ펞�̃��[�V�����̕����t���[��
		nFrame = pMotion->aKey[m_nKeyCnt].nFrame;
	}

	if (m_nFrameCounter == 0)
	{ // 1�t���[�����Ƃ̒ǉ��ʂ��v�Z
		for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
		{
			m_posAdd[nCntPartsKey] =
				(pMotion->aKey[m_nKeyCnt].aPos[nCntPartsKey] - m_pos[nCntPartsKey]) / float(nFrame);

			// �������v�Z
			D3DXVECTOR3 DiffRot = pMotion->aKey[m_nKeyCnt].aRot[nCntPartsKey] - m_rot[nCntPartsKey];

			// �~�������ߊm�F
			DiffRot = CCalculation::PiCheck(DiffRot);

			// 1�t���[���̒ǉ��ʂ��v�Z
			m_rotAdd[nCntPartsKey] = DiffRot / float(nFrame);

			// �~�������ߊm�F
			m_rotAdd[nCntPartsKey] = CCalculation::PiCheck(m_rotAdd[nCntPartsKey]);

		}
	}

	// ���[�V�������Z
	for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
	{
		m_pos[nCntPartsKey] += m_posAdd[nCntPartsKey];
		m_rot[nCntPartsKey] += m_rotAdd[nCntPartsKey];

		// �~�������ߊm�F
		m_rot[nCntPartsKey] = CCalculation::PiCheck(m_rot[nCntPartsKey]);
	}


}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�������[�u�̃t���[���J�E���^�[��������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionMove::FrameUpdate(CMotionData *motionData, int nMotionType)
{
	// ���[�V���������擾
	CMotionData::Motion *pMotion = motionData->GetMotion(nMotionType);

	// �t���[���J�E���^�[����
	m_nFrameCounter++;
	m_nAllFrameCounter++;

	if (m_bBlend == true && m_nFrameCounter == BLEND_FRAME)
	{ // �u�����h�����Z�b�g
		m_nFrameCounter = 0;
		m_bBlend = false;
		m_nKeyCnt = 1;
	}
	else if (m_nFrameCounter == pMotion->aKey[m_nKeyCnt].nFrame)
	{ // �t���[���J�E���^�[���Z�b�g
		m_nFrameCounter = 0;
		if (m_nKeyCnt == pMotion->nNumKey - 1
			&& pMotion->bLoop == false)
		{ //���[�v�������ő�L�[���𒴂���

		}
		else
		{
			m_nKeyCnt = (m_nKeyCnt + 1) % pMotion->nNumKey;
		}
	}

	if (m_nAllFrameCounter == pMotion->nAllFrame)
	{
		m_nAllFrameCounter = 0;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�������[�u�̐�������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionMove *CMotionMove::Create(void)
{
	CMotionMove *pMotionMove = NULL;

	pMotionMove = new CMotionMove;	// CMotionMove�^�̃C���X�^���X����
	if (pMotionMove != NULL)
	{
		// ����������
		pMotionMove->Init();
	}
	else
	{ // �������m�ێ��s
		MessageBox(NULL, "���[�V�����̃������m�ۂɎ��s���܂���", "�G���[", (MB_OK));
	}

	return pMotionMove;
}


//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�����f�[�^�̃R���X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionData::CMotionData()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�����f�[�^�̃f�X�g���N�^
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionData::~CMotionData()
{

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�����f�[�^�̏���������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotionData::Init(void)
{
	// ������
	// ���f�����
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		m_aModel[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aModel[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aModel[nCntParts].pMesh = NULL;
		m_aModel[nCntParts].pBuffMat = NULL;
		m_aModel[nCntParts].nNumMat = 0;
		for (int nCntTex = 0; nCntTex < MAX_MOTION_TEX; nCntTex++)
		{
			m_aModel[nCntParts].pTexture[nCntTex] = NULL;
		}
		m_aModel[nCntParts].nIdxParent = 0;
	}

	// ���[�V�������
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		m_aMotion[nCntMotion].bLoop = false;
		m_aMotion[nCntMotion].nNumKey = 0;
		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			m_aMotion[nCntMotion].aKey[nCntKey].nFrame = 0;
		}
		m_aMotion[nCntMotion].nAllFrame = 0;
	}

	// �p�[�c��
	m_nNumParts = 0;


	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V����txt�f�[�^�̓ǂݍ���
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
HRESULT CMotionData::Load(char *cTextName)
{

	// ���[�J���ϐ��錾
	FILE *pFile;
	char cLine[MAX_LOAD_LINE];			// 1�s�̈ꎞ�I�ǂݍ���
	char cData[MAX_LOAD_LINE];			// �ꎞ�I�ǂݍ���
										//char cData[MAX_LOAD_LINE];			// �ꎞ�I�ǂݍ���

										// �f�o�C�X�擾
	CRenderer *pRenderer = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	int nCntMotion = 0;			// ���[�V������
	int nCntModel = 0;

	D3DXMATERIAL *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	pFile = fopen(cTextName, "r"); // �t�@�C�����J��

	if (pFile != NULL) // pFile��NULL(�󔒁j�ł͂Ȃ��Ƃ���^
	{ // �t�@�C�����J�����ۂ̏���ss
		while (fgets(cLine, MAX_LOAD_LINE, pFile) != NULL)
		{
			sscanf(cLine, "%s", cData);
			if (strcmp(cData, "NUM_MODEL") == 0)
			{
				sscanf(cLine, "%s %s %d", cData, cData, &m_nNumParts);
			}
			else if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				sscanf(cLine, "%s %s %s", cData, cData, cData);

				D3DXLoadMeshFromX(cData,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_aModel[nCntModel].pBuffMat,
					NULL,
					&m_aModel[nCntModel].nNumMat,
					&m_aModel[nCntModel].pMesh);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)m_aModel[nCntModel].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_aModel[nCntModel].nNumMat; nCntMat++)
				{
					if (pMat[nCntMat].pTextureFilename != NULL)
					{
						D3DXCreateTextureFromFile(pDevice,
							pMat[nCntMat].pTextureFilename,
							&m_aModel[nCntModel].pTexture[nCntMat]);
					}
				}

				nCntModel++;
			}
			else if (strcmp(cData, "CHARACTERSET") == 0)
			{
				while (strcmp(cData, "END_CHARACTERSET") != 0)
				{
					fgets(cLine, MAX_LOAD_LINE, pFile);

					sscanf(cLine, "%s", cData);

					if (strcmp(cData, "PARTSSET") == 0)
					{
						int nIndex = 0;
						while (strcmp(cData, "END_PARTSSET") != 0)
						{
							fgets(cLine, MAX_LOAD_LINE, pFile);
							sscanf(cLine, "%s", cData);

							if (strcmp(cData, "INDEX") == 0)
							{
								sscanf(cLine, "%s %s %d", cData, cData, &nIndex);
							}
							else if (strcmp(cData, "PARENT") == 0)
							{
								sscanf(cLine, "%s %s %d", cData, cData, &m_aModel[nIndex].nIdxParent);
							}
							else if (strcmp(cData, "POS") == 0)
							{
								sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aModel[nIndex].pos.x,
									&m_aModel[nIndex].pos.y,
									&m_aModel[nIndex].pos.z);

								int nDAta = 0;
							}
							else if (strcmp(cData, "ROT") == 0)
							{
								sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aModel[nIndex].rot.x,
									&m_aModel[nIndex].rot.y,
									&m_aModel[nIndex].rot.z);
							}
						}
					}
				}
			}
			else if (strcmp(cData, "MOTIONSET") == 0)
			{
				int nCntKey = 0;
				while (strcmp(cData, "END_MOTIONSET") != 0)
				{
					fgets(cLine, MAX_LOAD_LINE, pFile);
					sscanf(cLine, "%s", cData);

					if (strcmp(cData, "LOOP") == 0)
					{
						int nLoop;
						sscanf(cLine, "%s %s %d", cData, cData, &nLoop);

						if (nLoop == 0)
						{
							m_aMotion[nCntMotion].bLoop = false;
						}
						else
						{
							m_aMotion[nCntMotion].bLoop = true;
						}

					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(cLine, "%s %s %d", cData, cData, &m_aMotion[nCntMotion].nNumKey);
					}
					else if (strcmp(cData, "KEYSET") == 0)
					{
						int nCntParts = 0;
						while (strcmp(cData, "END_KEYSET") != 0)
						{
							fgets(cLine, MAX_LOAD_LINE, pFile);
							sscanf(cLine, "%s", cData);

							if (strcmp(cData, "FRAME") == 0)
							{
								sscanf(cLine, "%s %s %d", cData, cData, &m_aMotion[nCntMotion].aKey[nCntKey].nFrame);

								// ���[�V�����S�̂ɂ�����t���[�������v�Z���邽�߂ɃL�[���Ƃ̃t���[���𑫂�
								m_aMotion[nCntMotion].nAllFrame += m_aMotion[nCntMotion].aKey[nCntKey].nFrame;
							}
							else if (strcmp(cData, "KEY") == 0)
							{
								while (strcmp(cData, "END_KEY") != 0)
								{
									fgets(cLine, MAX_LOAD_LINE, pFile);
									sscanf(cLine, "%s", cData);
									if (strcmp(cData, "POS") == 0)
									{
										sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts].x,
											&m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts].y,
											&m_aMotion[nCntMotion].aKey[nCntKey].aPos[nCntParts].z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(cLine, "%s %s %f %f %f", cData, cData, &m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts].x,
											&m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts].y,
											&m_aMotion[nCntMotion].aKey[nCntKey].aRot[nCntParts].z);
									}
								}
								nCntParts++;
							}
						}
						nCntKey++;
					}
				}
				nCntMotion++;
			}
		}
		fclose(pFile); // �t�@�C�������
	}
	else
	{ // �t�@�C�����J���Ȃ������ۂ̏���
	}


	return S_OK;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V����txt�f�[�^�̊J������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void CMotionData::UnLoad(void)
{
	//for (int nCntModel = 0; nCntModel < sizeof m_aModel / sizeof(Model); nCntModel++)
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		// �e�N�X�`���̊J��
		for (int nCntTex = 0; nCntTex < MAX_MOTION_TEX; nCntTex++)
		{
			if (m_aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				m_aModel[nCntModel].pTexture[nCntTex]->Release();
				m_aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		// ���b�V���̊J��
		if (m_aModel[nCntModel].pMesh != NULL)
		{
			m_aModel[nCntModel].pMesh->Release();
			m_aModel[nCntModel].pMesh = NULL;
		}
		// �}�e���A���̊J��
		if (m_aModel[nCntModel].pBuffMat != NULL)
		{
			m_aModel[nCntModel].pBuffMat->Release();
			m_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ���[�V�����f�[�^�̐�������
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
CMotionData *CMotionData::Create(void)
{
	CMotionData *pMotionData = NULL;

	pMotionData = new CMotionData;	// CMotion�^�̃C���X�^���X����
	if (pMotionData != NULL)
	{
		// ����������
		pMotionData->Init();
	}
	else
	{ // �������m�ێ��s
		MessageBox(NULL, "���[�V�����̃������m�ۂɎ��s���܂���", "�G���[", (MB_OK));
	}

	return pMotionData;
}