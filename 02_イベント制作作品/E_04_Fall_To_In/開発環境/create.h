#pragma once
//=============================================================================
//
// �쐬�c�[���̏��� [create.h]
// Author : Okabe Kazuki
//
//=============================================================================
#ifndef _CREATE_H_
#define _CREATE_H_

#include "main.h"
class CLight;
class CCamera;
class CScene3D;
class CMeshField;
class CPlayer;
class CObjCreate;
class CFieldCreate;
class CLine;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//====================================
// �Q�[���N���X
//====================================
class CCreate
{
public:
	typedef enum
	{
		CREATEMODE_OBJECT = 0,
		CREATEMODE_FIELD,
		CREATEMODE_MAX
	}CREATEMODE;
	CCreate();
	~CCreate();
	static CCreate *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLight *GetLight(void) { return m_pLight; }						// ���C�g
	static CCamera *GetCamera(void) {return m_pCamera;}					// �J����
	static CScene3D *GetScene3D(void) { return m_pScene3D; }			// 3D�|���S��
	static CMeshField *GetMeshField(void) { return m_pMeshField; }			// ���b�V���t�B�[���h
	static CObjCreate *GetObjCreate(void) { return m_pObjCreate; }
	static CFieldCreate *GetFieldCreate(void) { return m_pFieldCreate; }	// �I�u�W�F�N�g�쐬
	static CLine *GetLine(void) { return m_pLine; }							// ���C��
	static int GetMainPlayerID(void) { return m_nMainPlayerID; }			// ���C���̃v���C���[�ԍ�
	static void SetMainPlayerID(int nID) { m_nMainPlayerID = nID; }


	static bool GetPause(void);					// �|�[�Y�擾
	static void SetPause(bool bPause);			// �|�[�Y�ݒ�

	static CREATEMODE  GetCreateMode(void) { return m_mode; }

private:
	static CLight *m_pLight;					// ���C�g
	static CCamera *m_pCamera;					// �J����
	static CScene3D *m_pScene3D;				// 3D�|���S��
	static CMeshField *m_pMeshField;			// ���b�V���t�B�[���h
	static int m_nMainPlayerID;					// ���C���̃v���C���[�ԍ�
	static CPlayer *m_pPlayer;					// �v���C���[
	static CObjCreate *m_pObjCreate;
	static CFieldCreate *m_pFieldCreate;
	static CLine *m_pLine;						// ���C��
	static bool m_bPause;						// �|�[�Y
	static CREATEMODE m_mode;							// �쐬������

};
#endif