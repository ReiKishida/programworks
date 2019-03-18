////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���͏��� [input.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "input.h"
#include "DebugProc.h"
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
HWND CInput::m_hWnd;								// �E�B���h�E

//==============================================================================================================================
// �R���X�g���N�^
//==============================================================================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}
//==============================================================================================================================
// �f�X�g���N�^
//==============================================================================================================================
CInput::~CInput()
{

}
//==============================================================================================================================
// ������
//==============================================================================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (m_pInput == NULL)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		// �E�B���h�E�ݒ�
		SetWnd(hWnd);
	}
	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CInput::Uninit(void)
{
	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}
//==============================================================================================================================
// �E�B���h�E�ݒ�
//==============================================================================================================================
void CInput::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
//==============================================================================================================================
// �E�B���h�E�擾
//==============================================================================================================================
HWND CInput::GetWnd(void)
{
	return m_hWnd;
}

//******************************************************************************************************************************************************
// �L�[�{�[�h�N���X
//******************************************************************************************************************************************************
//==============================================================================================================================
// �R���X�g���N�^
//==============================================================================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		m_akeyState[nCnt] = 0;
		m_aKeyStateTrigger[nCnt] = 0;
		m_aKeyStateRelease[nCnt] = 0;
	}
}
//==============================================================================================================================
// �f�X�g���N�^
//==============================================================================================================================
CInputKeyboard::~CInputKeyboard()
{

}
//==============================================================================================================================
// ������
//==============================================================================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �I�u�W�F�N�g����
	CInput::Init(hInstance, hWnd);
	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CInputKeyboard::Uninit(void)
{
	// �I��
	CInput::Uninit();
}

//==============================================================================================================================
// �L�[�{�[�h�̍X�V����
//==============================================================================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;
	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_akeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];	// �L�[�{�[�h�̓��͏��ۑ�
			m_aKeyStateRelease[nCntKey] = (m_akeyState[nCntKey] ^ aKeyState[nCntKey])&m_akeyState[nCntKey];	// �L�[�{�[�h�̓��͏��ۑ�
			m_akeyState[nCntKey] = aKeyState[nCntKey];			// �L�[�{�[�h�̓��͏��ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}

}

//==============================================================================================================================
// �L�[�{�[�h�̓��͏��(�v���X���)���擾
//==============================================================================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_akeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;

}
//==============================================================================================================================
// �L�[�{�[�h�̓��͏��(�����[�X���)���擾
//==============================================================================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//******************************************************************************************************************************************************
// �}�E�X�N���X
//******************************************************************************************************************************************************
//==============================================================================================================================
// �R���X�g���N�^
//==============================================================================================================================
CInputMouse::CInputMouse()
{
	//m_akeyState = 0;
	//m_aKeyStateTrigger = 0;
	//m_aKeyStateRelease = 0;
	m_Point.x = 0;
	m_Point.y = 0;
}
//==============================================================================================================================
// �f�X�g���N�^
//==============================================================================================================================
CInputMouse::~CInputMouse()
{

}
//==============================================================================================================================
// ������
//==============================================================================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �I�u�W�F�N�g����
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�}�E�X)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================================================================================
// �I������
//==============================================================================================================================
void CInputMouse::Uninit(void)
{
	// �I��
	CInput::Uninit();
}

//==============================================================================================================================
// �X�V����
//==============================================================================================================================
void CInputMouse::Update(void)
{

	DIMOUSESTATE2 aMouseState;	// �}�E�X�̓��͏��
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aMouseState), &aMouseState)))
	{
		for (nCntKey = 0; nCntKey < NUM_MOUSE_MAX; nCntKey++)
		{
			m_aKeyStateTrigger.rgbButtons[nCntKey] =
				(m_akeyState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) & aMouseState.rgbButtons[nCntKey];	// �}�E�X�̓��͏��ۑ�(�g���K�[�j

			m_aKeyStateRelease.rgbButtons[nCntKey] =
				(m_akeyState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) & m_akeyState.rgbButtons[nCntKey];	// �}�E�X�̓��͏��ۑ�(�����[�X�j

			m_akeyState.rgbButtons[nCntKey] = aMouseState.rgbButtons[nCntKey];	// �}�E�X�̓��͏��ۑ�	(�v���X)
		}
		// �z�C�[�����ۑ�
		m_akeyState.lZ = aMouseState.lZ;
	}
	else
	{
		m_pDevice->Acquire();
	}
	// �E�B���h�E�擾
	SetPoint(CInput::GetWnd());

}

//==============================================================================================================================
// �}�E�X�̓��͏��(�v���X���)���擾
//==============================================================================================================================
bool CInputMouse::GetPress(int nKey)
{
	return(m_akeyState.rgbButtons[nKey-1] & 0x80) ? true : false;
}

//==============================================================================================================================
// �}�E�X�̓��͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CInputMouse::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger.rgbButtons[nKey-1] & 0x80) ? true : false;

}
//==============================================================================================================================
// �}�E�X�̓��͏��(�����[�X���)���擾
//==============================================================================================================================
bool CInputMouse::GetRelease(int nKey)
{
	return(m_aKeyStateRelease.rgbButtons[nKey-1] & 0x80) ? true : false;
}
float CInputMouse::GetWheel(void)
{
	return float(m_akeyState.lZ);
}
void CInputMouse::SetPoint(HWND hWnd)
{
	//�}�E�X�N���b�N���W�Z�o����
	GetCursorPos(&m_Point);//�J�[�\���ʒu�̐�΍��W���擾
	ScreenToClient(hWnd, &m_Point);//�J�[�\����΍��W����A���̃E�B���h�E���̍��W�ɕ␳
	m_Point.x = LONG((FLOAT)m_Point.x * ((FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_WIDTH));//����ɃT�C�Y�ύX���l�����āA���݂̃T�C�Y�ŕ␳�i�g�T�C�Y������̂Ō����ł͂Ȃ��j
	m_Point.y = LONG((FLOAT)m_Point.y * ((FLOAT)SCREEN_HEIGHT / (FLOAT)SCREEN_HEIGHT));

}
D3DXVECTOR3 CInputMouse::GetPoint(void)
{
	return D3DXVECTOR3(float(m_Point.x), float(m_Point.y),0.0f);
}

//******************************************************************************************************************************************************
// �Q�[���p�b�h�N���X
//******************************************************************************************************************************************************
CInputGamePad::GamePad CInputGamePad::g_aGamePad[MAX_GAMEPAD];
int		   	   CInputGamePad::g_JoyconNumber = 0;				// joystick�p�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
int		   	   CInputGamePad::g_JoyconAxesNumber=0;			// joystick�̋@�\�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
DWORD			CInputGamePad::m_NumForceFeedbackAxis = 0;
//==============================================================================================================================
// �R���X�g���N�^
//==============================================================================================================================
CInputGamePad::CInputGamePad()
{
}
//==============================================================================================================================
// �f�X�g���N�^
//==============================================================================================================================
CInputGamePad::~CInputGamePad()
{

}
//==============================================================================================================================
// �Q�[���p�b�h�̏�����
//==============================================================================================================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		g_aGamePad[nCnt].pDevGamePad = NULL;
	}
	g_JoyconNumber = 0;
	g_JoyconAxesNumber = 0;

	HRESULT hr;
	// �I�u�W�F�N�g����
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�R���g���[���[)�̐���
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// ��4���� (ATTACHEDONLY = ���ݐڑ�����Ă���W���C�X�e�B�b�N�̂݌��o����)
	{
		return FALSE;
	}

	// �t�H�[�X�t�B�[�h�o�b�N�@�\�̂�����̓f�o�C�X�̐���
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{// �R���g���[���[���q�����Ă��Ȃ��Ƃ��������Ȃ�
			// �f�o�C�X�Ƀf�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(hr = g_aGamePad[nCnt].pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
			{
				return FALSE;
			}

			// �������[�h��ݒ�
			if (FAILED(hr = g_aGamePad[nCnt].pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			{
				return FALSE;
			}

			// �Q�[���p�b�h�̋@�\�̏������W
			g_aGamePad[nCnt].GamepadCaps.dwSize = sizeof(DIDEVCAPS);

			if (FAILED(hr = g_aGamePad[nCnt].pDevGamePad->GetCapabilities(&g_aGamePad[nCnt].GamepadCaps)))
			{
				g_aGamePad[nCnt].pDevGamePad->Release();
				return FALSE;
			}

			m_NumForceFeedbackAxis = 0;

			// �e���̃��[�h�ݒ�
			hr = g_aGamePad[nCnt].pDevGamePad->EnumObjects(EnumAxesCallback, (void*)g_aGamePad[nCnt].pDevGamePad, DIDFT_AXIS);

			g_JoyconAxesNumber++;	// �ԍ���i�߂�

			if (m_NumForceFeedbackAxis > 2) m_NumForceFeedbackAxis = 2;

			// �G�t�F�N�g�I�u�W�F�N�g�̐���
			if (!(CreateEffect(hWnd, nCnt)))
			{
				g_aGamePad[nCnt].pEffect = NULL;
			}

			// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
			hr = g_aGamePad[nCnt].pDevGamePad->Poll();

			if (FAILED(hr))
			{
				hr = g_aGamePad[nCnt].pDevGamePad->Acquire();

				while (hr == DIERR_INPUTLOST)
				{
					hr = g_aGamePad[nCnt].pDevGamePad->Acquire();
				}
			}
		}
	}

	return S_OK;
}

//==============================================================================================================================
// �Q�[���p�b�h�̏I������
//==============================================================================================================================
void CInputGamePad::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		// �U����~
		SetVibration(0, 0, nCnt);

		// ���̓f�o�C�X�̊J��
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{
			g_aGamePad[nCnt].pDevGamePad->Unacquire();
			g_aGamePad[nCnt].pDevGamePad->Release();
			g_aGamePad[nCnt].pDevGamePad = NULL;
		}

		// �G�t�F�N�g�I�u�W�F�N�g�̊J��
		if (g_aGamePad[nCnt].pEffect != NULL)
		{
			g_aGamePad[nCnt].pEffect->Release();
			g_aGamePad[nCnt].pEffect = NULL;
		}
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	CInput::Uninit();
}

//==============================================================================================================================
// �Q�[���p�b�h�̍X�V����
//==============================================================================================================================
void CInputGamePad::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{
			DIJOYSTATE2 aGamePadKeyState;								// �R���g���[���[�̓��͏��
			BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// �X�e�B�b�N�̓��͏��
			BYTE aPov[POV_MAX] = { 0,0,0,0 };							// �\���L�[�̓��͏��

			HRESULT hr;
			hr = g_aGamePad[nCnt].pDevGamePad->Poll();

			// ���̓f�o�C�X����f�[�^���擾
			if (SUCCEEDED(hr = g_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
			{
				// �{�^��
				for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
				{
					// �R���g���[���[�̓��͏��̕ۑ�(�g���K�[)
					g_aGamePad[nCnt].aGamePadKeyStateTrigger[nCntKey] = (g_aGamePad[nCnt].aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
					// �R���g���[���[�̓��͏��̕ۑ�(�����[�X)
					g_aGamePad[nCnt].aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ g_aGamePad[nCnt].aGamePadKeyState[nCntKey]) & g_aGamePad[nCnt].aGamePadKeyState[nCntKey];
					// �R���g���[���[�̓��͏��̕ۑ�(�v���X)
					g_aGamePad[nCnt].aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
				}

				//--------------------------------------------------------
				// ���A�i���O�X�e�B�b�N�̓��͏��̎擾
				//--------------------------------------------------------
				if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE*100&& aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100&& aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
				{// ���X�e�B�b�N����ɓ|����
					aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// ���͂��ꂽ
				}
				else if (aGamePadKeyState.lY > GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
				{// ���X�e�B�b�N�����ɓ|����
					aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// ���͂��ꂽ
				}
				else if (aGamePadKeyState.lX > GAMEPAD_DEADZONE * 100)
				{// ��g�X�e�B�b�N���E�ɓ|����
					aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// ���͂��ꂽ
				}
				else if (aGamePadKeyState.lX < -GAMEPAD_DEADZONE * 100)
				{// ���X�e�B�b�N�����ɓ|����
					aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// ���͂��ꂽ
				}

				for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
				{// ���A�i���O�X�e�B�b�N�̕����̎�ޕ����[�v
				 // ���X�e�B�b�N�̃g���K�[���
					g_aGamePad[nCnt].aLeftAnalogStickTrigger[nCntAnalogStick] = (g_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
																																																				// ���X�e�B�b�N�̃v���X���
					g_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// ���A�i���O�X�e�B�b�N�̃v���X���
				}
				CDebugProc::print("%d�X�e�B�b�N\n", aGamePadKeyState.lX);
				CDebugProc::print("%d�X�e�B�b�N\n", aGamePadKeyState.lY);
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
					g_aGamePad[nCnt].aPovTrigger[nCntPov] = (g_aGamePad[nCnt].aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// �\���L�[�̃g���K�[���

																																// �\���L�[�̃v���X���
					g_aGamePad[nCnt].aPov[nCntPov] = aPov[nCntPov];		// �\���L�[�̃v���X���
				}
				g_aGamePad[nCnt].aGamePad = aGamePadKeyState;			// ���݂̏�Ԃ�ۑ�
			}
			else
			{
				g_aGamePad[nCnt].pDevGamePad->Acquire();				// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��
			}

			if (g_aGamePad[nCnt].nCounterVibration > 0)
			{
				g_aGamePad[nCnt].nCounterVibration--;
				if (g_aGamePad[nCnt].nCounterVibration <= 0)
				{// �J�E���^�[��0�ɂȂ�����U����~
					SetVibration(0, 0, nCnt);
				}
			}
		}
	}
}

//=============================================================================
// �U���̐ݒ�
//=============================================================================
void CInputGamePad::SetVibration(int nPower, int nTime, int nID)
{
	if (g_aGamePad[nID].pEffect == NULL) return;

	if (nPower > 100)
	{// 100%�𒴂��Ă���ꍇ
		nPower = 100;
	}
	else if (nPower < 0)
	{// 0%�����̏ꍇ
		nPower = 0;
	}

	if (g_aGamePad[nID].pDevGamePad != NULL)
	{// �R���g���[���[���q�����Ă��Ȃ��Ƃ��������Ȃ�
		if (nPower > 0)
		{
			DIEFFECT diEffect;

			ZeroMemory(&diEffect, sizeof(diEffect));
			diEffect.dwSize = sizeof(DIEFFECT);
			diEffect.dwGain = DI_FFNOMINALMAX / 100 * nPower;
			g_aGamePad[nID].pEffect->SetParameters(&diEffect, DIEP_GAIN);

			g_aGamePad[nID].pEffect->Start(1, 0);
		}
		else
		{
			g_aGamePad[nID].pEffect->Stop();
		}
		g_aGamePad[nID].nCounterVibration = nTime;
	}
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�X�e�B�b�N)���擾
//==============================================================================================================================
CInputGamePad::GamePad *CInputGamePad::GetgamePadStick(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{// �f�o�C�X��NULL����Ȃ�������
		 // �W���C�X�e�B�b�N�̏����擾
			g_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &g_aGamePad[nCnt].aGamePad);
		}
	}
	return &g_aGamePad[0];			// �W���C�X�e�B�b�N�̏���Ԃ�
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�v���X���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadPress(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(g_aGamePad[nIndex].aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadTrigger(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(g_aGamePad[nIndex].aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�����[�X���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadRelease(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(g_aGamePad[nIndex].aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�v���X���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickPress(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (g_aGamePad[nIndex].aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickTrigger(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (g_aGamePad[nIndex].aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�v���X�[���)���擾
//=============================================================================
bool CInputGamePad::GetPovTpress(int nIndex, int nAngle)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (g_aGamePad[nIndex].aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�g���K�[���)���擾
//=============================================================================
bool CInputGamePad::GetPovTtigger(int nIndex, int nAngle)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (g_aGamePad[nIndex].aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick�p�R�[���o�b�N�֐�
//*****************************************************************************
BOOL CALLBACK CInputGamePad::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = m_pInput->CreateDevice(pdidinstance->guidInstance, &g_aGamePad[g_JoyconNumber].pDevGamePad, NULL);

	// �f�o�C�X�����o�o���Ȃ������ꍇ
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	g_JoyconNumber++;		// �ԍ���i�߂�

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// joystick�̋@�\�R�[���o�b�N�֐�
//*****************************************************************************
BOOL CALLBACK CInputGamePad::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// �\���L�[�̍ŏ��l [��] [��]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;				// �\���L�[�̍ő�l [��] [�E]

	// �f�o�C�X�ɐݒ�
	hr = g_aGamePad[g_JoyconAxesNumber].pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	if ((pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0)
	{// �t�H�[�X�t�B�[�h�o�b�N�̎����J�E���g
		m_NumForceFeedbackAxis++;
	}

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// �G�t�F�N�g�I�u�W�F�N�g�̐���
//*****************************************************************************
BOOL CInputGamePad::CreateEffect(HWND hWnd, int nCnt)
{
	DWORD           rgdwAxes[2] = { DIJOFS_X , DIJOFS_Y };
	LONG            rglDirection[2] = { 1 , 1 };
	DICONSTANTFORCE cf;
	DIEFFECT        eff;
	HRESULT         hr;

	cf.lMagnitude = DI_FFNOMINALMAX;

	ZeroMemory(&eff, sizeof(eff));
	eff.dwSize = sizeof(DIEFFECT);
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.dwDuration = INFINITE;
	eff.dwSamplePeriod = 0;
	eff.dwGain = DI_FFNOMINALMAX;
	eff.dwTriggerButton = DIEB_NOTRIGGER;
	eff.dwTriggerRepeatInterval = 0;
	eff.cAxes = m_NumForceFeedbackAxis;
	eff.rgdwAxes = rgdwAxes;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;

	hr = g_aGamePad[nCnt].pDevGamePad->CreateEffect(GUID_ConstantForce, &eff, &g_aGamePad[nCnt].pEffect, NULL);
	if (FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}

//*****************************************************************************
// �Q�[���p�b�h�̎h�����Ă��鐔�̎擾
//*****************************************************************************
int CInputGamePad::GetGamePadNum(void)
{
	return g_JoyconNumber;
}