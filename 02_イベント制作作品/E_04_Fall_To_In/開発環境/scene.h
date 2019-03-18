#pragma once
//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DATA (10000)
#define MAX_PRIORITY_NUM (6)
//====================================
// �V�[���N���X
//====================================
class CScene
{
public:
	typedef enum
	{// �I�u�W�F�N�g�̎��
		OBJTYPE_NONE = 0,		// �����Ȃ�
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_MESHFIELD,		// ���b�V���t�B�[���h
		OBJTYPE_MESHWALL,		// ���b�V���E�H�[��
		OBJTYPE_MAP,			// �}�b�v
		OBJTYPE_DEBUG_SPHERE,	// �f�o�b�O�X�t�B�A
		OBJTYPE_PAINT,			// �y�C���g
		OBJTYPE_MODEL,			// ���f��
		OBJTYPE_SPEEDMETER,		// �X�s�[�h���[�^�[
		OBJTYPE_TIMER,
		OBJTYPE_OBJMACHINE,
		OBJTYPE_OBJECT,			// �I�u�W�F�N�g
		OBJTYPE_FUELGAUGE,
		OBJTYPE_NUMBER,			// ����
		OBJTYPE_BULLET,			// �e
		OBJTYPE_GAUGE,			// �Q�[�W
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_BG,				// �w�i
		OBJTYPE_FRAME,			// �t���[��
		OBJTYPE_INFOUI,			// ���m�点UI
		OBJTYPE_CALLOUT,		// �����o��
		OBJTYPE_POWERCUT,		// ��d
		OBJTYPE_LEGEND,			// �`���}�V��
		OBJTYPE_EFFECT_2D,		// �G�t�F�N�g�QD
		OBJTYPE_ENVIROMENT,		// �A��
		OBJTYPE_PANEL,
		OBJTYPE_SKY,
		OBJTYPE_MAX				// �ő�
	}OBJTYPE;
	typedef enum
	{
		PRIORITY_DEBUG_SPHERE = 4,	//
		PRIORITY_OBJMODEL = 3,	// �I�u�W�F���f��
		PRIORITY_PLAYER = 3,	// �v���C���[
		PRIORITY_MESHFIELD = 1,	// ���b�V���t�B�[���h
		PRIORITY_MESHWALL = 4,	// ���b�V����
		PRIORITY_PAINT = 4,		// �y�C���g
		PRIORITY_MAP = 5,		// �}�b�v
		PRIORITY_TIMER = 5,		// ����
		PRIORITY_LINE = 4,		// ��
		PRIORITY_OBJECT = 1,
		PRIORITY_BULLET = 2,	// �e
		PRIORITY_GAUGE = 5,		// �Q�[�W
		PRIORITY_EFFECT = 5,	// �G�t�F�N�g
		PRIORITY_BG = 0,		// �w�i
		PRIORITY_FRAME = 3,		// �t���[��
		PRIORITY_INFOUI = 1,	// ���m�点UI
		PRIORITY_CALLOUT = 5,	// �����o��
		PRIORITY_POWERCUT = 4,	// ��d
		PRIPRITY_LEGEND = 3,	// �`���}�V��
		PRIORITY_EFFECT_2D = 5,	// �G�t�F�N�g�QD
		PRIORITY_TITLEROGO = 5,	// ���S
		PRIORITY_ENVIROMENT = 3,// �A��
		PRIORITY_PANEL = 5,
		PRIORITY_SKY = 4,
		PRIORITY_INIT = 3,	// �����p
	}PRIORITY;

	CScene(int nPriority = PRIORITY_INIT);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void DeathFrag(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void DrawPart(int nIdy);
	void SetObjType(OBJTYPE type);
	OBJTYPE GetObjType(void);
	static int GetNumAll(void);
	static bool OverData(void);
	static CScene *GetScene(const int nPriority);
	CScene *GetNext(void) { return m_apNext; };
	static int GetNumPriority(const int nPriority) { return m_nNumPriority[nPriority]; };
protected:
	void Release(void);
private:
	static CScene *m_apTop[MAX_PRIORITY_NUM];	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_apCur[MAX_PRIORITY_NUM];	// ���݁i�Ō�j�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_apPrev;			// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_apNext;			// ���̃I�u�W�F�N�g�ւ̃|�C���^
	static int m_nNumAll;						// ����
	static int m_nNumPriority[MAX_PRIORITY_NUM];// �D�揇�ʂ��Ƃ̑���

	int m_nID;									// �������g�̔ԍ�
	OBJTYPE m_objType;							// �I�u�W�F�N�g�̎��
	bool m_bDeath;								// ���S�t���O
	int m_nPriority;					// �D�揇��

};
#endif