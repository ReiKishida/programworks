#pragma once
//=============================================================================
//
// �^�C�g���̏��� [title.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _TIITLE_H_
#define _TIITLE_H_

#include "main.h"
class CLight;
class CCamera;
class CScene3D;
class CInfoUI;
class CPlayer;
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//====================================
// �^�C�g���N���X
//====================================
class CTitle
{
public:
	CTitle();
	~CTitle();
	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLight *GetLight(void) { return m_pLight; }						// ���C�g
	static CCamera *GetCamera(void) { return m_pCamera; }					// �J����

	static int GetNext(void){ return m_nCntNext; }
	static bool GetTitlePush(void) { return m_bPush; }
	static bool GetSkip(void) { return m_bSkip; }

	static void SetSkip(bool skip);

private:
	static CLight *m_pLight;					// ���C�g
	static CCamera *m_pCamera;					// �J����
	static CPlayer *m_pPlayer;		// �v���C���[
	static CPlayer *m_pPlayer2;		// �v���C���[

	static int m_nCntNext;	// �Z���N�g�܂ōs���܂ł̃J�E���^�[
	static bool m_bPush;	// �{�^���������ꂽ���ǂ���
	static bool m_bSkip;	// �X�L�b�v����Ă邩�ǂ���

	static int m_nCntAppear;		// �^�C�g���f�ނ̏o���J�E���^�[
};
#endif