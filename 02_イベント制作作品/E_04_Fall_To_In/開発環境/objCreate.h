#pragma once
//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : Okabe Kazuki
//
//=============================================================================
#ifndef _OBJCREATE_H_
#define _OBJCREATE_H_

#include "main.h"
#include "scene.h"
#include "sceneX.h"
//====================================
// �I�u�W�F�N�g�N���X
//====================================
class CObjCreate : public CSceneX
{
public:
	CObjCreate();
	~CObjCreate();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//int GetNumObj(void) { return m_nNumObject; }

	static void SetScan(void);			// X�t�@�C���ǂݍ���
	static void SetPrint(void);			// X�t�@�C����������
	static CObjCreate *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type);
protected:
private:
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// �p�x

	int m_type;				// ���
	int m_nNumObject;		// �z�u��
	bool m_bCamera;

};
//====================================
// �t�B�[���h�쐬�N���X
//====================================

#define MAX_FIELD (100)
class CFieldCreate : public CScene
{
public:
	CFieldCreate();
	~CFieldCreate();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScan(void);			// X�t�@�C���ǂݍ���
	static void SetPrint(void);			// X�t�@�C����������
	static CFieldCreate *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type);
protected:
private:
	static D3DXVECTOR3 m_aMeshPos[MAX_FIELD][MAX_FIELD];	// ���_�̕ۑ��ꏊ
	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_rot;				// �p�x
	float m_fLength;				// ����
	static int m_type;						// ���

	static int m_nNumAll;					// ���b�V���u���b�N�S�Ă̌�
	static int m_nNumBlock[MAX_FIELD];		// ���b�V���̃u���b�N
	static int m_nCntMesh;					// ���b�V���̒��_��
	bool m_bCamera;
};

#endif#
