//=============================================================================
//
// ���͏��� [inout.h]
// Author :�ݓc��(KishidaRei)
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUM_KEY_MAX (256)

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
#define BUTTON_ARCADESTART   (9)						// �A�P�R��START�{�^���̔ԍ�
#define BUTTON_GUIDE   (CInputGamePad::ELECOM_GUIDE)	// GUIDE�{�^���̔ԍ�
#define BUTTON_MAX	   (CInputGamePad::ELECOM_MAX)		// �{�^���̍ő�

//*****************************************************************************
// ���͂̃N���X�\����
//*****************************************************************************
class CInput
{
public:
	CInput();			//�R���X�g���N�^
	virtual ~CInput();	//�f�X�g���N�^
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);//����������
	virtual void Uninit(void);							 //�I������
	virtual void Update(void) = 0;						 //�X�V����

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};
//=========================================================
//	�L�[�{�[�h����
//=========================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard::CInputKeyboard();
	CInputKeyboard::~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyboardPress(int nKey);		//�����Ă�����
	bool GetKeyboardTrigger(int nKey);		//�P����
	bool GetKeyboardRelease(int nKey);		//�������Ƃ�

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyTrigger[NUM_KEY_MAX];
	BYTE m_aKeyRelease[NUM_KEY_MAX];
};

//====================================
// �Q�[���p�b�h�N���X[DirectInput]
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
	static GamePad  m_aGamePad[MAX_GAMEPAD];	// �Q�[���p�b�h�̏��
	static int		m_JoyconNumber;				// �R�[���o�b�N�֐��Ŏg�ԍ�
	static int		m_JoyconAxesNumber;			// j�R�[���o�b�N�֐��Ŏg���ԍ�
	static DWORD	m_NumForceFeedbackAxis;		// �t�H�[�X�t�B�[�h�o�b�N
};

#endif
