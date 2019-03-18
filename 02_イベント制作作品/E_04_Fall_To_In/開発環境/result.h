#pragma once
//=============================================================================
//
// ���U���g�̏��� [result.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
class CCamera;
class CLight;
class CPlayer;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//====================================
// ���U���g�N���X
//====================================
class CResult
{
public:
	CResult();
	~CResult();
	static CResult *Create(void);
	static CLight *GetLight(void) { return m_pLight; }						// ���C�g
	static CCamera *GetCamera(void) { return m_pCamera; }						// �J����

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void WinReset(void) { m_nWin = 2; }
	static void SetWin(int nId) { m_nWin = nId; }
	static void Set1P(int nId) { n1PMotion = nId; }
	static void Set2P(int nId) { n2PMotion = nId; }

private:
	static CLight *m_pLight;					// ���C�g
	static CCamera *m_pCamera;					// �J����

	static CPlayer *m_pPlayer;		// �v���C���[
	static CPlayer *m_pPlayer2;		// �v���C���[

	static int m_nWin;

	static int n1PMotion;
	static int n2PMotion;
};
#endif