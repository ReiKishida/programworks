//=============================================================================
//
// �I�u�W�F�N�g�̏��� [object.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "sceneX.h"
#include "renderer.h"
#include "manager.h"
#include "create.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "objCreate.h"
#include "object.h"
#include "meshField.h"
#include "DebugProc.h"
#include "line.h"
#include "meshWall.h"
#include "environment.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_OBJECT_PRINT "data/object_print.txt"		// �t�@�C���̖��O��������
#define FILE_FIELD_PRINT  "data/field_print.txt"		// �t�@�C���̖��O��������

#define MOVE (2.0f)								// �ړ���
#define COLLAR   (0.0f)							// �����x

//== �I�u�W�F�N�g ==//
#define MAX_OBJECT (19)							// �I�u�W�F�N�g�̍ő�l
#define COLLISION  (100)						// �͈�

//== �t�B�[���h ==//
#define LINEMOVE (300)							// ���C���̈ړ���

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//== �R���X�g���N�^ ==//
CObjCreate::CObjCreate()
{//�N���A�l�ɂ���
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumObject = 0;
	m_bCamera = false;
}

//=== �f�X�g���N�^ ===//
CObjCreate::~CObjCreate()
{

}


//=============================================================================
// ����������
//=============================================================================
HRESULT CObjCreate::Init(void)
{
	CSceneX::Init();	// ����������

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObjCreate::Uninit(void)
{
	CSceneX::Uninit();	// �I������
}

//============================================================================
// �X�V����
//=============================================================================
void CObjCreate::Update(void)
{
	CCreate::CREATEMODE mode = CCreate::GetCreateMode();		// ���[�h�擾�̎擾

																//== �I�u�W�F�N�g���[�h ==//
	if (mode == CCreate::CREATEMODE_OBJECT)
	{
		int nNum = CSceneX::GetNumFile();
		CCamera *pCamera = CCreate::GetCamera();	// �J�����̏��擾
		m_pos = pCamera->GetPosR(0);	// �J�����̒����_�̎擾
		bool Slow = pCamera->GetSlow();
		bool Grid = pCamera->GetGrid();
		int MaxObj = CSceneX::GetNumFile();

		if (m_bCamera == false)
		{// �J�������[�h���I�t�̏��
			m_rot = pCamera->GetRot(0);
		}

		CSceneX::SetPos(m_pos);		// �ʒu�̍X�V
		CSceneX::SetRot(D3DXVECTOR3(0.0f, m_rot.y, m_rot.z));		// �p�x�̍X�V

		//== ��ނ̑I�� ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_1))
		{// �^�C�v�I��
			m_type--;
			if (m_type < 0)
			{
				m_type = MaxObj - 1;
			}
			BindModel(m_type);
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2))
		{// �^�C�v�I��
			m_type++;
			if (m_type > MaxObj - 1)
			{
				m_type = 0;
			}
			BindModel(m_type);
		}


		if (CManager::GetInputKeyboard()->GetTrigger(DIK_K))
		{// �I�u�W�F�N�g�̍쐬
			CObject::Create(m_pos, D3DXVECTOR3(0.0f, m_rot.y, m_rot.z), m_type, m_pos);
			m_nNumObject++;
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_L))
		{// �I�u�W�F�N�g�̍폜
			for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
			{// �D�揇�ʂ̍ő吔����
				CScene	*pSceneTarget = CScene::GetScene(nCntP);
				while (pSceneTarget != NULL)
				{
					CScene *pSceneNext = pSceneTarget->GetNext();
					if (pSceneTarget != NULL)
					{
						if (pSceneTarget->GetObjType() == OBJTYPE_OBJECT)
						{// �I�u�W�F�N�g�̎擾
							CObject *pObject = ((CObject*)pSceneTarget);	// �I�u�W�F�N�g���M�~�b�N�^�ɐݒ�
							D3DXVECTOR3 ObjectPos = pObject->GetPos();	// �ʒu�̎擾
							if (m_pos.x > ObjectPos.x - COLLISION
								&& m_pos.x < ObjectPos.x + COLLISION
								&& m_pos.z > ObjectPos.z - COLLISION
								&& m_pos.z < ObjectPos.z + COLLISION)
							{
								pObject->Uninit();
								m_nNumObject--;
							}
						}
					}
					pSceneTarget = pSceneNext;
				}
			}
		}

		//== ���[�h�ؑ� ==//
		// �J�����؂�ւ�
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4))
		{
			m_bCamera = m_bCamera ? false : true;
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F6))
		{//�X���[���[�h�̐؂�ւ�
			Slow = Slow ? false : true;
			pCamera->SetSlow(Slow);
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7))
		{//�O���b�h���[�h�̐؂�ւ�
			Grid = Grid ? false : true;
			pCamera->SetGrid(Grid);
		}


		//== �f�[�^���� ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
		{// �f�[�^�̍X�V
			CObjCreate::SetScan();
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
		{// �f�[�^�̕ۑ�
			CObjCreate::SetPrint();
		}

		//== ����̐��� ==//
		CDebugProc::print("\n");
		CDebugProc::print(" [[ ������� ]]\n");
		CDebugProc::print(" �ړ� [ W, S, A, D ][ R, F ]\n");
		CDebugProc::print(" ��] [ Q, E ] \n");
		CDebugProc::print(" ��� [ 1, 2 ] \n");
		CDebugProc::print(" �ʒu�̃��Z�b�g [ Z ]\n");
		CDebugProc::print(" �p�x�̃��Z�b�g [ X ]\n");
		CDebugProc::print(" �I�u�W�F�N�g�z�u [ K ] \n");
		CDebugProc::print(" �I�u�W�F�N�g�폜 [ L ] \n");
		CDebugProc::print(" \n");

		if (m_bCamera == false)
		{// �J�������[�h���I�t
			CDebugProc::print(" �J�������[�h [ F4 ] [ �I�t ]\n");
		}
		else
		{// �J�������[�h���I��
			CDebugProc::print(" �J�������[�h [ F4 ] [ �I�� ]\n");
		}
		CDebugProc::print(" �Q�[�����[�h�Ɉڍs[ F5 ]\n");
		if (Slow == false)
		{// �O���b�h���[�h���I�t
			CDebugProc::print(" �X���[���[�h [ F6 ] [ �I�t ]\n");
		}
		else
		{// �O���b�h���[�h���I��
			CDebugProc::print(" �X���[���[�h [ F6 ] [ �I�� ]\n");
		}
		if (Grid == false)
		{// �O���b�h���[�h���I�t
			CDebugProc::print(" �O���b�h���[�h [ F7 ] [ �I�t ]\n");
		}
		else
		{// �O���b�h���[�h���I��
			CDebugProc::print(" �O���b�h���[�h [ F7 ] [ �I�� ]\n");
		}


		CDebugProc::print(" �f�[�^�̍X�V    [ F8 ]\n");
		CDebugProc::print(" �f�[�^�̕ۑ�    [ F9 ]\n");
		CDebugProc::print(" �t�B�[���h�ɐ؂�ւ� [ F10 ]\n");
		CDebugProc::print(" \n");
		CDebugProc::print(" �ʒu [ %.1f %.1f %.1f ]\n", m_pos.x, m_pos.y, m_pos.z);
		CDebugProc::print(" �p�x [ %.1f %.1f %.1f ]\n", 0.0f, m_rot.y, m_rot.z);
		CDebugProc::print(" ��� [ %d ]\n", m_type);
		CDebugProc::print(" �z�u���@[ %d ]\n", m_nNumObject);
		CDebugProc::print(" \n");

		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));		// �����x�����ɖ߂�
	}
	else
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, COLLAR));		// �����Ȃ���Ԃɂ���
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObjCreate::Draw(void)
{
	CSceneX::Draw();	// �`�揈��
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CObjCreate *CObjCreate::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type)
{
	CObjCreate *pObjCreate = NULL;
	if (!CObjCreate::OverData())
	{// �QD�|���S������
		pObjCreate = new CObjCreate;
	}
	if (pObjCreate != NULL)
	{
		// ����������
		if (FAILED(pObjCreate->Init()))
		{
			return NULL;
		}
		pObjCreate->SetPos(pos);
		pObjCreate->SetRot(rot);
		pObjCreate->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		pObjCreate->SetType(type);
		pObjCreate->BindModel(type);
	}
	return pObjCreate;
}

//=============================================================================
// �ǂݍ��݊֐�
//=============================================================================
void CObjCreate::SetScan(void)
{
	//== �I�u�W�F�N�g��S�č폜 ==//
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// �D�揇�ʂ̍ő吔����
		CScene	*pSceneTarget = CScene::GetScene(nCntP);
		while (pSceneTarget != NULL)
		{
			CScene *pSceneNext = pSceneTarget->GetNext();
			if (pSceneTarget != NULL)
			{
				if (pSceneTarget->GetObjType() == OBJTYPE_OBJECT)
				{// �I�u�W�F�N�g�̎擾
					CObject *pObject = ((CObject*)pSceneTarget);	// �I�u�W�F�N�g���M�~�b�N�^�ɐݒ�
					pObject->Uninit();								// �I�u�W�F�N�g�̍폜
				}
				if (pSceneTarget->GetObjType() == OBJTYPE_MESHFIELD)
				{// �I�u�W�F�N�g�̎擾
					CMeshField *pMeshField = ((CMeshField*)pSceneTarget);	// �I�u�W�F�N�g�����^�ɐݒ�
					pMeshField->Uninit();								// �I�u�W�F�N�g�̍폜
				}
				if (pSceneTarget->GetObjType() == OBJTYPE_MESHWALL)
				{// �I�u�W�F�N�g�̎擾
					CMeshWall *pMeshWall = ((CMeshWall*)pSceneTarget);	// �I�u�W�F�N�g��ǌ^�ɐݒ�
					pMeshWall->Uninit();								// �I�u�W�F�N�g�̍폜
				}

				if (pSceneTarget->GetObjType() == OBJTYPE_ENVIROMENT)
				{// �r���{�[�h�̍폜
					CEnvironment *pEnviroment = ((CEnvironment*)pSceneTarget);	// �I�u�W�F�N�g��A���^�ɐݒ�
					pEnviroment->Uninit();							// �I�u�W�F�N�g�̍폜
				}
			}
			pSceneTarget = pSceneNext;
		}
	}
	CScene::DeathFrag();
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CEnvironment::Create();
		CEnvironment::SetScan();
	}
	CMeshWall::SetScan();
	CMeshField::SetScan();
	CObject::SetScan();		// �ĂуI�u�W�F�N�g�̔z�u
	CLine::SetDisp(false);
}

//=============================================================================
// �������݊֐�
//=============================================================================
void CObjCreate::SetPrint(void)
{
	FILE *pFile;						// �t�@�C���̍쐬
	pFile = fopen(FILE_OBJECT_PRINT, "w");		// �t�@�C���̖��O�̏�������

	if (pFile != NULL)
	{
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# �w�I�u�W�F�N�g�x�I�u�W�F�N�g���X�g [object.txt]");
		fprintf(pFile, "# Author : Okabe Kazuki\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# �I�u�W�F�N�g�̎�ށE�ʒu�E�p�x\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
		{// �D�揇�ʂ̍ő吔����
			CScene	*pSceneTarget = CScene::GetScene(nCntP);
			while (pSceneTarget != NULL)
			{
				CScene *pSceneNext = pSceneTarget->GetNext();
				if (pSceneTarget != NULL)
				{
					if (pSceneTarget->GetObjType() == OBJTYPE_OBJECT)
					{// �I�u�W�F�N�g�̎擾

						CObject *pObject = ((CObject*)pSceneTarget);	// �I�u�W�F�N�g���M�~�b�N�^�ɐݒ�
						D3DXVECTOR3 pos = pObject->GetPos();	// �ʒu�̎擾
						D3DXVECTOR3 rot = pObject->GetRot();	// �p�x�̎擾
						int type = pObject->GetType();			// ��ނ̎擾

						fprintf(pFile, "OBJECTSET\n");
						fprintf(pFile, "TYPE = %d			# [ ��� ]\n", type);						// ��ނ̏�������
						fprintf(pFile, "POS  = %.1f %.1f %.1f	# [ �ʒu ]\n", pos.x, pos.y, pos.z);	// �ʒu�̏�������
						fprintf(pFile, "ROT  = %.1f %.1f %.1f	# [ �p�x ]\n", rot.x, rot.y, rot.z);	// �p�x�̏�������
						fprintf(pFile, "END_OBJECTSET\n");
						fprintf(pFile, "\n");
					}
				}
				pSceneTarget = pSceneNext;
			}
		}
		fprintf(pFile, "END_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI\n");
		fclose(pFile);// �I��
	}
}

//=============================================================================
//
// �t�B�[���h�쐬���� [object.cpp]
// Author : Okabe Kazuki
//
//=============================================================================
D3DXVECTOR3 CFieldCreate::m_aMeshPos[MAX_FIELD][MAX_FIELD] = {};
int CFieldCreate::m_type = 0;
int CFieldCreate::m_nNumAll = 0;
int CFieldCreate::m_nNumBlock[MAX_FIELD] = {};
int CFieldCreate::m_nCntMesh = 0;

//== �R���X�g���N�^ ==//
CFieldCreate::CFieldCreate()
{//�N���A�l�ɂ���
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = 0;
	m_fLength = 0.0f;
	m_nNumAll = 0;
	m_nCntMesh = 0;
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		m_nNumBlock[nCnt] = -1;
	}
}

//=== �f�X�g���N�^ ===//
CFieldCreate::~CFieldCreate()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFieldCreate::Init(void)
{
	m_fLength = 150.0f;		// ���C���̒���

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFieldCreate::Uninit(void)
{
	//CSceneX::Uninit();	// �I������
}

//============================================================================
// �X�V����
//=============================================================================
void CFieldCreate::Update(void)
{
	CCreate::CREATEMODE mode = CCreate::GetCreateMode();		// ���[�h�擾�̎擾

	//== �t�B�[���h���[�h ==//
	if (mode == CCreate::CREATEMODE_FIELD)
	{
		CLine *pLine = CCreate::GetLine();				// ���C���̏��擾
		CCamera *pCamera = CCreate::GetCamera();		// �J�����̏��擾
		m_pos = pCamera->GetPosR(0);					// �J�����̒����_�̎擾
		D3DXVECTOR3 LineStart = D3DXVECTOR3(sinf(-D3DX_PI / 2 + m_rot.y)*m_fLength, 0.0f, cosf(-D3DX_PI / 2 + m_rot.y)*m_fLength) + m_pos;
		D3DXVECTOR3 LineEnd = D3DXVECTOR3(sinf(D3DX_PI / 2 + m_rot.y) *m_fLength, 0.0f, cosf(D3DX_PI / 2 + m_rot.y)*m_fLength) + m_pos;
		bool Slow = pCamera->GetSlow();
		bool Grid = pCamera->GetGrid();
		int MaxField = CMeshField::GetNumFile();
		CMeshField *pMesh;
		if (m_bCamera == false)
		{
			m_rot = pCamera->GetRot(0);						// �J�����̊p�x�̎擾
		}


		if (m_bCamera == false)
		{
			D3DXVECTOR3 rot = pCamera->GetRot(0);		// �J�����̊p�x�̎擾
		}
		// �����̑召��ς���
		// ���Ƃ�X�̃u���b�N����ς���

		//==��ޕύX ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_1))
		{// �^�C�v�I��
			m_type--;
			if (m_type < 0)
			{
				m_type = MaxField - 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2))
		{// �^�C�v�I��
			m_type++;
			if (m_type > MaxField - 1)
			{
				m_type = 0;
			}
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_3))
		{// ���C���̏k��
			if (m_fLength > 25)
			{
				m_fLength -= 25;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{// ���C���̂�������
			m_fLength += 25;
		}

		//== �n�ʂ̔z�u���� ==//

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_K))
		{// ���C���̔z�u
			CLine::Create(LineStart, LineEnd);
			m_aMeshPos[m_nNumAll][m_nCntMesh] = D3DXVECTOR3(LineStart);		// ���_�̎n�_�擾
			m_aMeshPos[m_nNumAll][m_nCntMesh + 1] = D3DXVECTOR3(LineEnd);	// ���_�̏I�_�擾
			m_nNumBlock[m_nNumAll]++;	// �u���b�N���̉��Z
			m_nCntMesh += 2;			// �J�E���^�[�𒸓_�������Z
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_L))
		{// �n�ʂ̔z�u
			if (m_nCntMesh != 0)
			{
				pMesh = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 1, m_nCntMesh / 2, m_type,false);
				pMesh->SetVtxPos(&m_aMeshPos[m_nNumAll][0]);
				m_nNumAll++;		// ���̃u���b�N�Ɉڍs
				m_nCntMesh = 0;		// ���b�V���̒��_�����Z�b�g
			}
		}

		//== ���[�h�ؑ� ==//
		// �J�����؂�ւ�
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4))
		{
			m_bCamera = m_bCamera ? false : true;
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F6))
		{//�X���[���[�h�̐؂�ւ�
			Slow = Slow ? false : true;
			pCamera->SetSlow(Slow);
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7))
		{//�O���b�h���[�h�̐؂�ւ�
			Grid = Grid ? false : true;
			pCamera->SetGrid(Grid);
		}

		//== �f�[�^���o�� ==//
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
		{// �f�[�^�̍X�V
			CFieldCreate::SetScan();
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
		{// �f�[�^�̕ۑ�
			CFieldCreate::SetPrint();
		}

		pLine->SetPos(LineStart, LineEnd);		// �ʒu�̍X�V

		//== ����̐��� ==//
		CDebugProc::print("\n");
		CDebugProc::print(" [[ ������� ]]\n");
		CDebugProc::print(" �ړ� [ W, S, A, D ][ R, F ]\n");
		CDebugProc::print(" ��] [ Q, E ] \n");
		CDebugProc::print(" ��� [ 1, 2 ]\n");
		CDebugProc::print(" �g�k [ 3, 4 ]\n");
		CDebugProc::print(" ���C���z�u [ K ] \n");
		CDebugProc::print(" �t�B�[���h�̔z�u [ L ] \n");
		CDebugProc::print("\n");
		if (m_bCamera == false)
		{// �O���b�h���[�h���I�t
			CDebugProc::print(" �J�������[�h [ F4 ] [ �I�t ]\n");
		}
		else
		{// �O���b�h���[�h���I��
			CDebugProc::print(" �J�������[�h [ F4 ] [ �I�� ]\n");
		}
		CDebugProc::print(" �Q�[�����[�h�Ɉڍs[ F5 ]\n");
		if (Slow == false)
		{// �O���b�h���[�h���I�t
			CDebugProc::print(" �X���[���[�h [ F6 ] [ �I�t ]\n");
		}
		else
		{// �O���b�h���[�h���I��
			CDebugProc::print(" �X���[���[�h [ F6 ] [ �I�� ]\n");
		}
		if (Grid == false)
		{// �O���b�h���[�h���I�t
			CDebugProc::print(" �O���b�h���[�h [ F7 ] [ �I�t ]\n");
		}
		else
		{// �O���b�h���[�h���I��
			CDebugProc::print(" �O���b�h���[�h [ F7 ] [ �I�� ]\n");
		}

		CDebugProc::print(" �f�[�^�̍X�V [ F8 ]\n");
		CDebugProc::print(" �f�[�^�̕ۑ� [ F9 ]\n");
		CDebugProc::print(" �I�u�W�F�N�g�ɐ؂�ւ� [ F10 ]\n");
		CDebugProc::print(" \n");
		CDebugProc::print(" �ʒu [ %.1f %.1f %.1f ]\n", m_pos.x, m_pos.y, m_pos.z);
		CDebugProc::print(" �p�x [ %.1f %.1f %.1f ]\n", 0.0f, m_rot.y, m_rot.z);
		CDebugProc::print(" ���� [ %.1f ]\n", m_fLength);
		CDebugProc::print("\n");
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFieldCreate::Draw(void)
{
}

//=============================================================================
// �t�B�[���h�̐�������
//=============================================================================
CFieldCreate *CFieldCreate::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type)
{
	CFieldCreate *pFieldCreate = NULL;

	if (!CFieldCreate::OverData())
	{// �QD�|���S������
		pFieldCreate = new CFieldCreate;
	}
	if (pFieldCreate != NULL)
	{
		// ����������
		if (FAILED(pFieldCreate->Init()))
		{
			return NULL;
		}
	}
	return pFieldCreate;
}

//=============================================================================
// �ǂݍ��݊֐�
//=============================================================================
void CFieldCreate::SetScan(void)
{
	//== �I�u�W�F�N�g��S�č폜 ==//
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// �D�揇�ʂ̍ő吔����
		CScene	*pSceneTarget = CScene::GetScene(nCntP);
		while (pSceneTarget != NULL)
		{
			CScene *pSceneNext = pSceneTarget->GetNext();
			if (pSceneTarget != NULL)
			{
				if (pSceneTarget->GetObjType() == OBJTYPE_MESHFIELD)
				{// �t�B�[���h�̎擾
					CMeshField *pMeshField = ((CMeshField*)pSceneTarget);	// �t�B�[���h���M�~�b�N�^�ɐݒ�
					pMeshField->Uninit();									// �t�B�[���h�̍폜
				}
			}
			pSceneTarget = pSceneNext;
		}
	}
	CMeshField::SetScan();	// �Ăуt�B�[���h�̔z�u
}

//=============================================================================
// �������݊֐�
//=============================================================================
void CFieldCreate::SetPrint(void)
{
	FILE *pFile;								// �t�@�C���̍쐬
	pFile = fopen(FILE_FIELD_PRINT, "w");		// �t�@�C���̖��O�̏�������

	if (pFile != NULL)
	{
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# �w�t�B�[���h�x�t�B�[���h���X�g [field.txt]");
		fprintf(pFile, "# Author : Okabe Kazuki\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# �t�B�[���h�̎�ށE�ʒu�E�p�x\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		for (int nCnt1 = 0; nCnt1 < m_nNumAll; nCnt1++)
		{// �S�Ẵu���b�N����
			fprintf(pFile, "SETFIELD\n");
			fprintf(pFile, "	SETVERTEX\n");
			fprintf(pFile, "		TYPE = %d\n", m_type);
			fprintf(pFile, "		BLOCK = %d\n", m_nNumBlock[nCnt1]);
			for (int nCnt2 = 0; nCnt2 < m_nNumBlock[nCnt1]+1; nCnt2++)
			{// �u���b�N��������
				fprintf(pFile, "    %d = %.1f %.1f %.1f\n", nCnt2 * 2, m_aMeshPos[nCnt1][nCnt2 * 2].x, m_aMeshPos[nCnt1][nCnt2 * 2].y, m_aMeshPos[nCnt1][nCnt2 * 2].z);	// �ʒu�̏�������
				fprintf(pFile, "    %d = %.1f %.1f %.1f\n", nCnt2 * 2 + 1, m_aMeshPos[nCnt1][(nCnt2 * 2) + 1].x, m_aMeshPos[nCnt1][(nCnt2 * 2) + 1].y, m_aMeshPos[nCnt1][(nCnt2 * 2) + 1].z);
			}
			fprintf(pFile, "	END_SETVERTEX\n");
			fprintf(pFile, "ENDSETFIELD\n");
		}
		fprintf(pFile, "END_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI\n");
		fclose(pFile);// �I��
	}
}
