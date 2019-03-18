//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"
#include "game.h"
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CScene::m_nNumAll = 0;					// ����
CScene *CScene::m_apTop[MAX_PRIORITY_NUM] = {};				// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CScene *CScene::m_apCur[MAX_PRIORITY_NUM] = {};				// ���݁i�Ō�j�̃I�u�W�F�N�g�ւ̃|�C���^
int CScene::m_nNumPriority[MAX_PRIORITY_NUM] = {};// �D�揇�ʂ��Ƃ̑���
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority)
{
	// �D��ԍ����Ƃɉ��Z
	m_nNumPriority[nPriority]++;

	if (m_apTop[nPriority] == NULL)
	{// �擪�̃I�u�W�F�N�g����
		m_apTop[nPriority] = this;
	}

	if (m_apCur[nPriority] != NULL)
	{// �O��
	 // �O��̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g������̃I�u�W�F�N�g�ɂ���
		m_apCur[nPriority]->m_apNext = this;
		// �O��̃I�u�W�F�N�g����
		m_apPrev = m_apCur[nPriority];
	}


	// ����̃I�u�W�F�N�g����
	m_apCur[nPriority] = this;
	// �I�u�W�F�N�g�^�C�v��ݒ�
	m_objType = OBJTYPE_NONE;
	m_nPriority = nPriority;
	m_nID = m_nNumAll;
	m_bDeath = false;
	m_nNumAll++;

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// �S�I������
//=============================================================================
void CScene::ReleaseAll(void)
{

	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// �D�揇�ʂ̍ő吔����
		CScene *pScene = m_apTop[nCntP];
		int nCntALL = 0;
		while (pScene != NULL)
		{
			CScene *pSceneNext = pScene->m_apNext;
			m_nNumPriority[pScene->m_nPriority]--;
			m_nNumAll--;
			// �I������
			pScene->Uninit();
			delete pScene;
			pScene = NULL;
			pScene = pSceneNext;
			nCntALL++;
		}
		m_apTop[nCntP] = NULL;
		m_apCur[nCntP] = NULL;
	}

}
//=============================================================================
// �S�X�V����
//=============================================================================
void CScene::UpdateAll(void)
{
	if (!CGame::GetPause())
	{
		for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
		{// �D�揇�ʂ̍ő吔����
			CScene *pScene = m_apTop[nCntP];

			while (pScene != NULL)
			{// �A�b�v�f�[�g
				CScene *pSceneNext = pScene->m_apNext;
				pScene->Update();
				pScene = pSceneNext;
			}
		}
	}
	// ���S�t���O
	DeathFrag();
}
//=============================================================================
// �S�`�揈��
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// �D�揇�ʂ̍ő吔����
		CScene *pScene = m_apTop[nCntP];
		while (pScene != NULL)
		{
			CScene *pSceneNext = pScene->m_apNext;
			pScene->Draw();
			pScene = pSceneNext;
		}
	}
}
//=============================================================================
// �ꕔ�`��
//=============================================================================
void CScene::DrawPart(int nIdy)
{
	CScene *pScene;
	CScene *pSceneNext;
	switch (nIdy)
	{
	case 0:// �ʏ�J����
		for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
		{// �D�揇�ʂ̍ő吔����
			CDebugProc::print("�D�揇��%d�F%d\n", nCntP, m_nNumPriority[nCntP]);
			pScene = m_apTop[nCntP];
			while (pScene != NULL)
			{
				pSceneNext = pScene->m_apNext;
				pScene->Draw();

				pScene = pSceneNext;
			}
		}
		break;
	case 1:// �~�j�}�b�v

		break;
	case 2:

		break;
	case 3:		// ���U���g
		break;
	}

}
//=============================================================================
// �I�u�W�F�N�g�̔j��
//=============================================================================
void CScene::Release(void)
{
	if (this != NULL)
	{
		m_bDeath = true;	// ���S�t���O��������
	}
}
//=============================================================================
// ���S�t���O�������I�u�W�F�N�g�̔j��
//=============================================================================
void CScene::DeathFrag(void)
{
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// �D�揇�ʂ̍ő吔����
		CScene *pScene = m_apTop[nCntP];
		while (pScene != NULL)
		{// ���S�t���O�`�F�b�N
			CScene *pSceneNext = pScene->m_apNext;
			if (pScene->m_bDeath)
			{// ���S�t���O��������
				if (pScene->m_apPrev != NULL)
				{// ���݂̎��̃I�u�W�F�N�g��O�̃I�u�W�F�N�g�ɋ�����
					pScene->m_apPrev->m_apNext = pScene->m_apNext;
				}
				if (pScene->m_apNext != NULL)
				{// ���݂̑O�̃I�u�W�F�N�g�����̃I�u�W�F�N�g�ɋ�����
					pScene->m_apNext->m_apPrev = pScene->m_apPrev;
				}
				if (pScene->m_apNext == NULL)
				{// �Ō�������ւ���
					m_apCur[nCntP] = pScene->m_apPrev;
				}
				if (pScene->m_apPrev == NULL)
				{// �擪�����ւ���
					m_apTop[nCntP] = pScene->m_apNext;
				}
				m_nNumPriority[pScene->m_nPriority]--;
				m_nNumAll--;

				delete pScene;
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

}
//=============================================================================
// �����̎擾
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;
}
//=============================================================================
// �I�u�W�F�N�g�̎�ނ̐ݒ�
//=============================================================================
void CScene::SetObjType(CScene::OBJTYPE type)
{
	m_objType = type;
}
//=============================================================================
// �I�u�W�F�N�g�̎�ނ̎擾
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}
//=============================================================================
// �I�u�W�F�N�g�̍ő吔������
//=============================================================================
bool CScene::OverData(void)
{
	bool bOver = false;
	for (int nCntP = 0; nCntP < MAX_PRIORITY_NUM; nCntP++)
	{// �D�揇�ʂ̍ő吔����
		if (m_nNumPriority[nCntP] >= MAX_DATA)
		{// �ő吔������
			bOver = true;
		}
	}
	return bOver;
}
//=============================================================================
// �V�[���̍ŏ��擾
//=============================================================================
CScene *CScene::GetScene(const int nPriority)
{
	return m_apTop[nPriority];
}