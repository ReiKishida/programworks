//=============================================================================
//
// �V�[������ [scene.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_WIDTH (110) //�|���S����X�̑傫��
#define POLYGON_HEIGHT (110) //�|���S����Y�̑傫��

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CScene *CScene::m_apScene[MAX_PRIORITY][MAX_POLYGON] = {};	//�V�[���̃C���X�^���X����
int CScene::m_nNumAll = 0;	//�V�[���̑���

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority)
{

	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		m_nPriority = nPriority;	//�D�揇�ʂ̔ԍ�����
		if (m_apScene[m_nPriority][nCntScene] == NULL)
		{
			m_apScene[m_nPriority][nCntScene] = this;//���
			m_nID = nCntScene;	//�������g�̔ԍ����擾
			m_Objtype = OBJTYPE_NONE;
			m_nNumAll++;
			break;
		}
	}

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
//�����̎擾
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
//�S�ẴI�u�W�F�N�g�̍X�V
//=============================================================================
void CScene::UpdateAll(void)
{
	int nIndex;
	nIndex = m_nNumAll;
	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
//�S�ẴI�u�W�F�N�g�̕`��
//=============================================================================
void CScene::DrawAll(void)
{
	int nIndex;
	nIndex = m_nNumAll;
	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
//�I�u�W�F�N�g�̎擾
//=============================================================================
//CScene **CScene::GetScene(void)
//{
//	return m_apScene;
//}

//=============================================================================
// �S�ẴI�u�W�F�N�g�̔j��
//=============================================================================
void CScene::ReleaseSceneAll(void)
{
	int nIndex;
	nIndex = m_nNumAll;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				delete m_apScene[nCntPriority][nCntScene];
				m_apScene[nCntPriority][nCntScene] = NULL;
			}
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�̔j��
//=============================================================================
void CScene::Release(void)
{

		if (m_apScene[m_nPriority][m_nID] != NULL)
		{
			int nPriority;//�����̗D�揇��
			nPriority = m_nPriority;
			int nID;	//�����̐���
			nID = m_nID;
			delete m_apScene[nPriority][nID];
			m_apScene[nPriority][nID] = NULL;
			m_nNumAll--;
		}

}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̐ݒ�
//=============================================================================
void CScene::SetObjType(OBJTYPE objtype)
{
	//�I�u�W�F�N�g�̎�ނ̑��
	m_Objtype = objtype;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̎擾
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
 	return m_Objtype;
}


//=============================================================================
// ���̃I�u�W�F�N�g�̎擾
//=============================================================================
CScene *CScene::GetScene(int nPriority, int nIndex)
{

		return m_apScene[nPriority][nIndex];

}
