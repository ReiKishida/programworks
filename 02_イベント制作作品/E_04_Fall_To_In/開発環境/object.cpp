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
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "object.h"

//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_CHAR (1024)
#define FILE_SCAN  "data/object_scan.txt"		// �t�@�C���̖��O�ǂݍ���
#define FILE_PRINT "data/object_print.txt"		// �t�@�C���̖��O��������

#define MOVE (2.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//== �R���X�g���N�^ ==//
CObject::CObject(int nPriority) : CSceneX(PRIORITY_OBJECT)
{//�N���A�l�ɂ���
}

//=== �f�X�g���N�^ ===//
CObject::~CObject()
{

}


//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	CSceneX::Init();	// ����������
	SetObjType(OBJTYPE_OBJECT);				// �I�u�W�F�N�g�^�C�v�ݒ�
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();	// �I������
}

//============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{

	CSceneX::Update();	// �X�V����
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	if (GetType() == 1 || GetType() == 2 || GetType() == 3)
	{
		SetRot(GetRot() + D3DXVECTOR3(0.01f, 0.001f, 0.001f));
		D3DXVECTOR3 pos = GetPos();
		pos.y += 20;
		SetPos(pos);
		if (pos.y >= 12000)
		{
			Uninit();
			return;
		}
	}
	CSceneX::Draw();	// �`�揈��
}

//=============================================================================
// �I�u�W�F�N�g�̐�������
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type,  const D3DXVECTOR3 ShadowPos)
{
	CObject *pObject = NULL;
	if (!CObject::OverData())
	{// �I�u�W�F�N�g�̐���
		pObject = new CObject;
	}
	if (pObject != NULL)
	{
		// ����������
		if (FAILED(pObject->Init()))
		{
			return NULL;
		}
		pObject->SetPos(pos);		// �ʒu�̊m��
		pObject->SetRot(rot);		// �p�x�̊m��
		pObject->SetType(type);		// ��ނ̊m��
		pObject->SetShadowPos(ShadowPos);// �e�̊m��
		pObject->BindModel(type);
	}
	return pObject;
}

//=============================================================================
// �t�@�C���̓ǂݍ���
//=============================================================================
void CObject::SetScan()
{
	// �f�o�C�X�̎擾
	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FILE_SCAN, "r");		// �t�@�C���̖��O�̓ǂݍ���


	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// �s�̓ǂݍ���

			if (strcmp(cData, "OBJECTSET") == 0)
			{// OBJECTSET�������ꍇ
				D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0);		// �ʒu
				D3DXVECTOR3 rot = D3DXVECTOR3(0, 0, 0);		// �p�x
				D3DXVECTOR3 shadowPos = D3DXVECTOR3(0, 0, 0);	// �e
				int type = 0;				// ���
				while (strcmp(cData, "END_OBJECTSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TYPE") == 0)
					{// ��ނ̓ǂݍ���
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &type);
					}
					else if (strcmp(cData, "POS") == 0)
					{// �ʒu�̓ǂݍ���
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pos.x, &pos.y, &pos.z);
						shadowPos = pos;
					}
					else if (strcmp(cData, "ROT") == 0)
					{// �p�x�̓ǂݍ���
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &rot.x, &rot.y, &rot.z);
					}
					else if (strcmp(cData, "SHADOWPOS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &shadowPos.x, &shadowPos.y, &shadowPos.z);

					}
				}
				if (strcmp(cData, "END_OBJECTSET") == 0)
				{// ���f���̐���

					CObject::Create(pos, rot, type, shadowPos);

				}
			}
		}
		fclose(pFile);// �I��
	}
}

