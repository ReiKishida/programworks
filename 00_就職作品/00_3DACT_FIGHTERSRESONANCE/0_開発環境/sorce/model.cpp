//=============================================================================
//
// �V�[��X����[3D�|���S��] [model.cpp]
// Author :�ݓc��(KishidaRei)
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
#include "model.h"
#include "scene2D.h"
#include "debugproc.h"
#include "scene_meshorbit.h"
#include "collision.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR (256)
#define GRAVITY_NUM (0.7f)
#define MODEL_FILENAME ("data/TEXT/model_data.txt")

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CMeshOrbit *CModel::m_pMeshOrbit = NULL;
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CModel::CModel()
{

}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CModel::~CModel()
{
}


//=============================================================================
// �֐����F�v���C���[�̃p�[�c�ǂݍ���
// �֐��̊T�v�F�p�[�c��ǂݍ���
//=============================================================================
void CModel::Load(void)
{

}

//=============================================================================
// �֐����F�v���C���[�̃p�[�c�̔j��
// �֐��̊T�v�F�p�[�c��ǂݍ���
//=============================================================================
void CModel::Unload(void)
{

}


//=============================================================================
// �֐����FX�t�@�C���V�[���̐���
// �֐��̊T�v�FX�t�@�C���V�[���𐶐�����
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel  *pModel;
	pModel = new CModel;//�V�[���N���X�̐���
	if (pModel != NULL)
	{
		pModel->Init();
		pModel->m_pos = pos;
		pModel->m_rot = rot;
	}
	return pModel;
}

//=============================================================================
// �֐����FX�t�@�C���V�[���̏���������
// �֐��̊T�v�FX�t�@�C�����̐ݒ�
//=============================================================================
HRESULT CModel::Init(void)
{



	return S_OK;
}

//=============================================================================
// �֐����FX�t�@�C���V�[���̏I������
// �֐��̊T�v�F���b�V���A�}�e���A���A���g�̔j��
//=============================================================================
void CModel::Uninit(void)
{


}

//=============================================================================
// �֐����FX�t�@�C���V�[���̍X�V����
// �֐��̊T�v�F--
//=============================================================================
void CModel::Update(void)
{
}


//=============================================================================
// �֐����FX�t�@�C���V�[���̕`�揈��
// �֐��̊T�v�F--
//=============================================================================
void CModel::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//�e�̃}�g���b�N�X
	D3DXMATRIX mtxParent;


	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	//D3DXMATERIAL *m_pMat;		//�}�e���A���ւ̃|�C���^

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


	//�e�̃}�g���b�N�X�Ɗ|�����킹��
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_pMesh != NULL)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, m_pTexture);

			// ���f��(�p�[�c)�̕`��
			m_pMesh->DrawSubset(nCntMat);
		}
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// �֐����F�ʒu���̑��
// �֐��̊T�v�F�ʒu����������
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//=============================================================================
// �֐����F�p�x���̑��
// �֐��̊T�v�F�p�x����������
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �֐����F���[���h�}�g���b�N�X�̑��
// �֐��̊T�v�F���[���h�}�g���b�N�X����������
//=============================================================================
void CModel::SetMtx(D3DXMATRIX mtx)
{
	m_mtxWorld = m_mtxWorld;
}

//=============================================================================
// �֐����F�O�Ղ̐���
// �֐��̊T�v�F�O�Ղ̐����ƃ��[���h�}�g���b�N�X����������B
//=============================================================================
void CModel::CreateOrbit(void)
{
	m_pMeshOrbit = CMeshOrbit::Create();
	m_pMeshOrbit->SetMtxParent(&m_mtxWorld);
}

//=============================================================================
// �֐����F�O�Ղ̐���
// �֐��̊T�v�F�O�Ղ̐����ƃ��[���h�}�g���b�N�X����������B
//=============================================================================
void CModel::CreateAttackCollision(void)
{
}

//=============================================================================
// �֐����F���[���h�}�g���b�N�X�̎擾
// �֐��̊T�v�F���[���h�}�g���b�N�X�����擾����
//=============================================================================
D3DXMATRIX CModel::GetMatrix(void)
{
	return m_mtxWorld;
}

//=============================================================================
// �֐����F�e�̃��f��
// �֐��̊T�v�F�e�̃��f���̐ݒ�
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}


//==========================================================================================
// �֐����F���f���̊��蓖��
// �֐��̊T�v�F���b�V�����A�}�e���A�����A�}�e���A���̐��A�e�N�X�`�������蓖�Ă�
//===========================================================================================
void CModel::BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat, LPDIRECT3DTEXTURE9 tex)
{
	m_pMesh = mesh;
	m_pBuffMat = buffmat;
	m_nNumMat = nummat;
	m_pTexture = tex;

}


//=============================================================================
// �֐����F�F�̐ݒ�
// �֐��̊T�v�F�F��ݒ肷��
//=============================================================================
void CModel::SetColor(D3DXCOLOR col)
{
	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_pMat->MatD3D.Diffuse.r = col.r;
	m_pMat->MatD3D.Diffuse.g = col.g;
	m_pMat->MatD3D.Diffuse.b = col.b;
	m_pMat->MatD3D.Diffuse.a = col.a;

	m_pMat->MatD3D.Ambient.r = col.r;
	m_pMat->MatD3D.Ambient.g = col.g;
	m_pMat->MatD3D.Ambient.b = col.b;
	m_pMat->MatD3D.Ambient.a = col.a;

}

