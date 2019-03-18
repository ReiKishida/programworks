////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �R���g���[���[���͏��� [gamepad.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "gamepad.h"

//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define	GAMEPAD_NUM_KEY_MAX		(128)		// �L�[�̍ő吔

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECTINPUT8 g_pGamePad = NULL;					// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevGamePad = NULL;			// ���̓f�o�C�X(k�R���g���[���[)�ւ̃|�C���^
DIDEVCAPS g_GamepadCaps;							// �Q�[���p�b�h�̋@�\���	

DIJOYSTATE2 g_aGamePad;								// �R���g���[���[�̏��

BYTE		g_aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];			// �W���C�X�e�B�b�N�̃v���X���͏��
BYTE		g_aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];		// �W���C�X�e�B�b�N�̃g���K�[���͏��
BYTE		g_aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];		// �W���C�X�e�B�b�N�̃g���K�[���͏��
BYTE		g_aLeftAnalogStick[ANALOG_STICK_MAX];				// ���A�i���O�X�e�B�b�N�̃v���X���
BYTE		g_aLeftAnalogStickTrigger[ANALOG_STICK_MAX];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
BYTE		g_aPov[POV_MAX];									// �\���L�[�̃v���X���
BYTE		g_aPovTrigger[POV_MAX];								// �\���L�[�̃g���K�[���

bool g_bTrigger;
//==============================================================================================================================
// �Q�[���p�b�h�̏�����
//==============================================================================================================================
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	g_bTrigger = false;
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pGamePad, NULL)))
	{
		return FALSE;
	}

	// ���̓f�o�C�X(�R���g���[���[)�̐���
	if (FAILED(g_pGamePad->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// ��4���� (ATTACHEDONLY = ���ݐڑ�����Ă���W���C�X�e�B�b�N�̂݌��o����)
	{
		return FALSE;
	}

	if (g_pDevGamePad != NULL)
	{// �R���g���[���[���q�����Ă��Ȃ��Ƃ��������Ȃ�

	 // �f�o�C�X�Ƀf�[�^�t�H�[�}�b�g��ݒ�
		if (FAILED(hr = g_pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return FALSE;
		}

		// �������[�h��ݒ�
		if (FAILED(hr = g_pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return FALSE;
		}

		// �Q�[���p�b�h�̋@�\�̏������W 
		g_GamepadCaps.dwSize = sizeof(DIDEVCAPS);

		if (FAILED(hr = g_pDevGamePad->GetCapabilities(&g_GamepadCaps)))
		{
			g_pDevGamePad->Release();
			return FALSE;
		}

		// �e���̃��[�h�ݒ�
		if (FAILED(hr = g_pDevGamePad->EnumObjects(EnumAxesCallback, (void*)g_pDevGamePad, DIDFT_AXIS)))
		{
			return FALSE;
		}

		// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		hr = g_pDevGamePad->Poll();

		if (FAILED(hr))
		{
			hr = g_pDevGamePad->Acquire();

			while (hr == DIERR_INPUTLOST)
			{
				hr = g_pDevGamePad->Acquire();
			}
		}
	}

	return S_OK;
}

//==============================================================================================================================
// �Q�[���p�b�h�̏I������
//==============================================================================================================================
void UninitGamePad(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (g_pDevGamePad != NULL)
	{
		g_pDevGamePad->Unacquire();
		g_pDevGamePad->Release();
		g_pDevGamePad = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (g_pGamePad != NULL)
	{
		g_pGamePad->Release();
		g_pGamePad = NULL;
	}
}

//==============================================================================================================================
// �Q�[���p�b�h�̍X�V����
//==============================================================================================================================
void UpdateGamePad(void)
{
	if (g_pDevGamePad != NULL)
	{
		DIJOYSTATE2 aGamePadKeyState;		// �R���g���[���[�̓��͏��
		BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// �X�e�B�b�N�̓��͏��
		BYTE aPov[POV_MAX] = { 0,0,0,0 };							// �\���L�[�̓��͏��

		HRESULT hr;
		hr = g_pDevGamePad->Poll();

		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(hr = g_pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
		{
			// �{�^��
			for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
			{
				// �R���g���[���[�̓��͏��̕ۑ�(�g���K�[)
				g_aGamePadKeyStateTrigger[nCntKey] = (g_aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
				// �R���g���[���[�̓��͏��̕ۑ�(�����[�X)
				g_aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ g_aGamePadKeyState[nCntKey]) & g_aGamePadKeyState[nCntKey];
				// �R���g���[���[�̓��͏��̕ۑ�(�v���X)
				g_aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
			}

			//--------------------------------------------------------
			// ���A�i���O�X�e�B�b�N�̓��͏��̎擾
			//--------------------------------------------------------
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.lY > GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N�����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.lX > GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N���E�ɓ|����
				aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N�����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// ���͂��ꂽ
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// ���A�i���O�X�e�B�b�N�̕����̎�ޕ����[�v
			 // ���X�e�B�b�N�̃g���K�[���
				g_aLeftAnalogStickTrigger[nCntAnalogStick] = (g_aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
																																												// ���X�e�B�b�N�̃v���X���
				g_aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// ���A�i���O�X�e�B�b�N�̃v���X���
			}


			//--------------------------------------------------------
			// �\���L�[�̓��͏��̎擾
			//--------------------------------------------------------
			if (aGamePadKeyState.rgdwPOV[0] == 0.0f)
			{// �\���L�[�̏��������
				aPov[POV_UP] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 18000.0f)
			{// �\���L�[�̉���������
				aPov[POV_DOWN] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 9000.0f)
			{// �\���L�[�̉E��������
				aPov[POV_RIGHT] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 27000.0f)
			{// �\���L�[�̍���������
				aPov[POV_LEFT] = 128;			// ���͂��ꂽ
			}

			for (int nCntPov = 0; nCntPov < POV_MAX; nCntPov++)
			{// �\���L�[�̕����̎�ޕ����[�v
			 // �\���L�[�̃g���K�[���
				g_aPovTrigger[nCntPov] = (g_aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// �\���L�[�̃g���K�[���

																								// �\���L�[�̃v���X���
				g_aPov[nCntPov] = aPov[nCntPov];												// �\���L�[�̃v���X���
			}

			g_aGamePad = aGamePadKeyState;														// ���݂̏�Ԃ�ۑ�

		}
		else
		{
			g_pDevGamePad->Acquire();		// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��
		}
	}
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�X�e�B�b�N)���擾
//==============================================================================================================================
DIJOYSTATE2 *GetgamePadStick(void)
{

	if (g_pDevGamePad != NULL)
	{// �f�o�C�X��NULL����Ȃ�������

	 // �W���C�X�e�B�b�N�̏����擾
		g_pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &g_aGamePad);
	}

	return &g_aGamePad;			// �W���C�X�e�B�b�N�̏���Ԃ�
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�v���X���)���擾
//==============================================================================================================================
bool GetGamePadPress(int nKey)
{
	return(g_aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�g���K�[���)���擾
//==============================================================================================================================
bool GetGamePadTrigger(int nKey)
{
	return(g_aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�����[�X���)���擾
//==============================================================================================================================
bool GetGamePadRelease(int nKey)
{
	return(g_aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�v���X���)���擾
//==============================================================================================================================
bool GetGamePadStickPress(int nKey)
{
	return (g_aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�g���K�[���)���擾
//==============================================================================================================================
bool GetGamePadStickTrigger(int nKey)
{
	return (g_aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�g���K�[���)���擾
//=============================================================================
bool GetPovTrigger(int nAngle)
{
	return (g_aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick�p�R�[���o�b�N�֐�
//*****************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = g_pGamePad->CreateDevice(pdidinstance->guidInstance, &g_pDevGamePad, NULL);

	// �f�o�C�X�����o�o���Ȃ������ꍇ
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

//*****************************************************************************
// joystick�̋@�\�R�[���o�b�N�֐�
//*****************************************************************************
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// �\���L�[�̍ŏ��l [��] [��]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;					// �\���L�[�̍ő�l [��] [�E]

																					// �f�o�C�X�ɐݒ�
	hr = g_pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}