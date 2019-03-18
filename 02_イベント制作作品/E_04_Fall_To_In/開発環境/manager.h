#pragma once
//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputGamePad;
class CSound;
class CTexture;
class CDebugProc;
class CTitle;
class CGame;
class CResult;
class CCamera;
class CCreate;
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//====================================
// �}�l�[�W���N���X
//====================================
class CManager
{
public:
	typedef enum
	{// ���[�h�̎��
		MODE_TITLE = 0,	// �^�C�g��
		MODE_GAME,		// �Q�[��
		MODE_CREATE,	// �N���G�C�g
		MODE_RESULT,	// ���U���g
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();
	static void Load(void);
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SaveAngle(float *pAngle);
	static void SaveRot(D3DXVECTOR3 *rot);
	static int PowInt(const int nNum, const int nCnt);	// int�^�̗ݏ�v�Z
	static float GetRandomPI(void);					// �����_���֐�
	static float GetRandBetWeen(const float fMin,const float fMax);	// �����_���֐�
	static int GetIntRandBetWeen(const int nMin, const int nMax);	// �����_���֐�
	static void SaveMinMax(float *pSave,const float Min,const float Max);
	static MODE GetMode(void);						// ���[�h
	static CCamera *GetCameraMode(void);				// �J�����擾

	static CRenderer *GetRenderer(void);			// �����_���[�̎擾
	static CInputKeyboard *GetInputKeyboard(void);	// �L�[�{�[�h�̎擾
	static CInputMouse *GetInputMouse(void);		// �}�E�X�̎擾
	static CInputGamePad *GetInputGamePad(void);	// �Q�[���p�b�h�̎擾
	static CSound *GetSound(void);					// �T�E���h
	static CTexture *GetTexture(void);				// �e�N�X�`��

	static HRESULT SetMode(MODE mode);				// ���[�h
private:
	static CRenderer *m_pRenderer;				// �����_���[
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h
	static CInputMouse *m_pInputMouse;			// �}�E�X
	static CInputGamePad *m_pInputGamePad;		// �Q�[���p�b�h
	static CSound *m_pSound;					// ��
	static CTexture *m_pTexture;				// �e�N�X�`��
	static CDebugProc *m_pDebugProc;			// �f�o�b�O
	static CTitle *m_pTitle;					// �^�C�g��
	static CGame *m_pGame;						// �Q�[��
	static CCreate *m_pCreate;					// �N���G�C�g
	static CResult *m_pResult;					// ���U���g
	static MODE m_mode;							// ���[�h
};
#endif