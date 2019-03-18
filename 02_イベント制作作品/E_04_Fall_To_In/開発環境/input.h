////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���͏��� [input.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔
#define	NUM_MOUSE_MAX			(8)	// �}�E�X�̃L�[�̍ő吔

#define GAMEPAD_DEADZONE		(3)
#define MAX_ZONE				(1000)
#define	GAMEPAD_NUM_KEY_MAX		(128)		// �L�[�̍ő吔
#define MAX_GAMEPAD				(4)			// �R���g���[���[�̐�

#define BUTTON_X	   (CInputGamePad::ELECOM_X)		// X�{�^���̔ԍ�
#define BUTTON_Y	   (CInputGamePad::ELECOM_Y)		// Y�{�^���̔ԍ�
#define BUTTON_A	   (CInputGamePad::ELECOM_A)		// A�{�^���̔ԍ�
#define BUTTON_B	   (CInputGamePad::ELECOM_B)		// B�{�^���̔ԍ�
#define BUTTON_L1	   (CInputGamePad::ELECOM_L1)		// L1�{�^���̔ԍ�
#define BUTTON_R1	   (CInputGamePad::ELECOM_R1)		// R1�{�^���̔ԍ�
#define BUTTON_L2	   (CInputGamePad::ELECOM_L2)		// L2�{�^���̔ԍ�
#define BUTTON_R2	   (CInputGamePad::ELECOM_R2)		// R2�{�^���̔ԍ�
#define BUTTON_L_STICK (CInputGamePad::ELECOM_L_STICK)	// L_STICK�{�^���̔ԍ�
#define BUTTON_R_STICK (CInputGamePad::ELECOM_R_STICK)	// R_STICK�{�^���̔ԍ�
#define BUTTON_BACK	   (CInputGamePad::ELECOM_BACK)	// BACK�{�^���̔ԍ�
#define BUTTON_START   (CInputGamePad::ELECOM_START)	// START�{�^���̔ԍ�
#define BUTTON_GUIDE   (CInputGamePad::ELECOM_GUIDE)	// GUIDE�{�^���̔ԍ�
#define BUTTON_MAX	   (CInputGamePad::ELECOM_MAX)		// �{�^���̍ő�

//====================================
// �C���v�b�g�N���X
//====================================
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	static void SetWnd(HWND hWnd);
	static HWND GetWnd(void);
protected:
	static LPDIRECTINPUT8 m_pInput;		// �C���v�b�g�f�o�C�X
	LPDIRECTINPUTDEVICE8 m_pDevice;		// �f�o�C�X
	static HWND m_hWnd;					// �E�B���h�E
};
//====================================
// �L�[�{�[�h�N���X�i�h���j
//====================================
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
private:
	BYTE m_akeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̓��͏��i�v���X���j
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��i�g���K�[���j
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��i�����[�X���j

};
//====================================
// �}�E�X�N���X�i�h���j
//====================================
class CInputMouse :public CInput
{
public:
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	float GetWheel(void);
	void SetPoint(HWND hWnd);
	D3DXVECTOR3 GetPoint(void);
private:
	DIMOUSESTATE2 m_akeyState;				// �}�E�X�̓��͏��i�v���X���j
	DIMOUSESTATE2 m_aKeyStateTrigger;		// �}�E�X�̓��͏��i�g���K�[���j
	DIMOUSESTATE2 m_aKeyStateRelease;		// �}�E�X�̓��͏��i�����[�X���j

	POINT m_Point;							// �}�E�X�̍��W
};
//====================================
// �Q�[���p�b�h�N���X(�h���j
//====================================
class CInputGamePad :public CInput
{
public:
	typedef enum
	{// �A�i���O�X�e�B�b�N�̕����̎��(4����)
		ANALOG_STICK_UP = 0,
		ANALOG_STICK_RIGHT,
		ANALOG_STICK_LEFT,
		ANALOG_STICK_DOWN,
		ANALOG_STICK_MAX
	}ANALOG_STICK;
	typedef enum
	{// �\���L�[�̕����̎��
		POV_UP = 0,
		POV_DOWN,
		POV_RIGHT,
		POV_LEFT,
		POV_MAX
	}POV;
	typedef enum
	{// �{�^���̎��
		ELECOM_X = 0,
		ELECOM_Y,
		ELECOM_A,
		ELECOM_B,
		ELECOM_L1,
		ELECOM_R1,
		ELECOM_L2,
		ELECOM_R2,
		ELECOM_L_STICK,
		ELECOM_R_STICK,
		ELECOM_BACK,
		ELECOM_START,
		ELECOM_GUIDE,
		ELECOM_MAX
	}ELECOM_CTRL;
	typedef struct
	{
		LPDIRECTINPUTDEVICE8 pDevGamePad;							// ���̓f�o�C�X(k�R���g���[���[)�ւ̃|�C���^
		DIDEVCAPS GamepadCaps;										// �Q�[���p�b�h�̋@�\���
		LPDIRECTINPUTEFFECT	pEffect;								// �G�t�F�N�g�I�u�W�F�N�g

		DIJOYSTATE2 aGamePad;										// �R���g���[���[�̏��

		BYTE		aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];			// �W���C�X�e�B�b�N�̃v���X���͏��
		BYTE		aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];	// �W���C�X�e�B�b�N�̃g���K�[���͏��
		BYTE		aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];	// �W���C�X�e�B�b�N�̃����[�X���͏��
		BYTE		aLeftAnalogStick[ANALOG_STICK_MAX];				// ���A�i���O�X�e�B�b�N�̃v���X���
		BYTE		aLeftAnalogStickTrigger[ANALOG_STICK_MAX];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
		BYTE		aPov[POV_MAX];									// �\���L�[�̃v���X���
		BYTE		aPovTrigger[POV_MAX];							// �\���L�[�̃g���K�[���

		int			nCounterVibration;								// �U���p�̃J�E���^�[
	}GamePad;

	CInputGamePad();
	~CInputGamePad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void SetVibration(int nPower, int nTime, int nID);

	// �Q�[���p�b�h�̃{�^�����͏��
	bool GetGamePadPress(int nIndex, int nKey);
	bool GetGamePadTrigger(int nIndex, int nKey);
	bool GetGamePadRelease(int nIndex, int nKey);

	// �Q�[���p�b�h�̃X�e�B�b�N�̓��͏��
	bool GetGamePadStickPress(int nIndex, int nKey);
	bool GetGamePadStickTrigger(int nIndex, int nKey);

	// �Q�[���p�b�h�̏\���L�[�̓��͏��
	bool GetPovTpress(int nIndex, int nAngle);
	bool GetPovTtigger(int nIndex, int nAngle);

	GamePad *GetgamePadStick(void);
	int GetGamePadNum(void);
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
	BOOL CreateEffect(HWND hWnd, int nCnt);
private:
	static GamePad  g_aGamePad[MAX_GAMEPAD];	// �Q�[���p�b�h�̏��
	static int		g_JoyconNumber;				// joystick�p�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
	static int		g_JoyconAxesNumber;			// joystick�̋@�\�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
	static DWORD	m_NumForceFeedbackAxis;		// �t�H�[�X�t�B�[�h�o�b�N
};
#endif