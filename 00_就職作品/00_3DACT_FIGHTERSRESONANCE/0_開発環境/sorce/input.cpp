//=============================================================================
//
// ���͏��� [input.cpp]
// Author :�ݓc��(Kishida Rei)
//
//=============================================================================
#include "input.h"

//*****************************************************************************
//�ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;	//���͂̃|�C���^

//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CInput::CInput()
{//�N���A�Ȓl����
	m_pDevice = NULL;
}


//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CInput::~CInput()
{

}
//=============================================================================
// �֐����F���͂̏���������
// �֐��̊T�v�FDirectInput�̐���
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �֐����F���͂̏I������
// �֐��̊T�v�F�f�o�C�X�̔j���A���͂̔j��
//=============================================================================
void CInput::Uninit(void)
{
	//�f�o�C�X�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//���͂̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}




//=============================================================================
//
//
//								CInputKeyboard
//							  �`�L�[�{�[�h�̏����`
//
//=============================================================================
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;
		m_aKeyTrigger[nCntKey] = 0;
		m_aKeyRelease[nCntKey] = 0;
	}
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}
//=============================================================================
// �֐����F�L�[�{�[�h�̏���������
// �֐��̊T�v�F�L�[�{�[�h�̐���
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
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

//=============================================================================
// �֐����F�L�[�{�[�h�̏I������
// �֐��̊T�v�F���͂̔j��
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �֐����F�L�[�{�[�h�̍X�V����
// �֐��̊T�v�FPress, Trigger, Release�̏���
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//�g���K�[
			m_aKeyRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];	//�����[�X
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	//�L�[�{�[�h�̓��͏��ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=============================================================================
// �֐����F�L�[�{�[�h�̓��͏��(�v���X���)���擾
// �֐��̊T�v�FPress�����擾
//=============================================================================

bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �֐����F�L�[�{�[�h�̓��͏��(�g���K�[���)���擾
// �֐��̊T�v�FTrigger�����擾
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �֐����F�L�[�{�[�h�̓��͏��(�����[�X)���擾
// �֐��̊T�v�FTrigger�����擾
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
//
//
//								CInputGamePad
//							  �`�Q�[���p�b�h�̏����`
//
//=============================================================================

CInputGamePad::GamePad CInputGamePad::m_aGamePad[MAX_GAMEPAD];
int		   	   CInputGamePad::m_JoyconNumber = 0;				// joystick�p�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
int		   	   CInputGamePad::m_JoyconAxesNumber = 0;			// joystick�̋@�\�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
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
		m_aGamePad[nCnt].pDevGamePad = NULL;
	}
	m_JoyconNumber = 0;
	m_JoyconAxesNumber = 0;

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
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{// �R���g���[���[���q�����Ă��Ȃ��Ƃ��������Ȃ�
		 // �f�o�C�X�Ƀf�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(hr = m_aGamePad[nCnt].pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
			{
				return FALSE;
			}

			// �������[�h��ݒ�
			if (FAILED(hr = m_aGamePad[nCnt].pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			{
				return FALSE;
			}

			// �Q�[���p�b�h�̋@�\�̏������W
			m_aGamePad[nCnt].GamepadCaps.dwSize = sizeof(DIDEVCAPS);

			if (FAILED(hr = m_aGamePad[nCnt].pDevGamePad->GetCapabilities(&m_aGamePad[nCnt].GamepadCaps)))
			{
				m_aGamePad[nCnt].pDevGamePad->Release();
				return FALSE;
			}

			m_NumForceFeedbackAxis = 0;

			// �e���̃��[�h�ݒ�
			hr = m_aGamePad[nCnt].pDevGamePad->EnumObjects(EnumAxesCallback, (void*)m_aGamePad[nCnt].pDevGamePad, DIDFT_AXIS);

			m_JoyconAxesNumber++;	// �ԍ���i�߂�

			if (m_NumForceFeedbackAxis > 2) m_NumForceFeedbackAxis = 2;

			// �G�t�F�N�g�I�u�W�F�N�g�̐���
			if (!(CreateEffect(hWnd, nCnt)))
			{
				m_aGamePad[nCnt].pEffect = NULL;
			}

			// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
			hr = m_aGamePad[nCnt].pDevGamePad->Poll();

			if (FAILED(hr))
			{
				hr = m_aGamePad[nCnt].pDevGamePad->Acquire();

				while (hr == DIERR_INPUTLOST)
				{
					hr = m_aGamePad[nCnt].pDevGamePad->Acquire();
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
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{
			m_aGamePad[nCnt].pDevGamePad->Unacquire();
			m_aGamePad[nCnt].pDevGamePad->Release();
			m_aGamePad[nCnt].pDevGamePad = NULL;
		}

		// �G�t�F�N�g�I�u�W�F�N�g�̊J��
		if (m_aGamePad[nCnt].pEffect != NULL)
		{
			m_aGamePad[nCnt].pEffect->Release();
			m_aGamePad[nCnt].pEffect = NULL;
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
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{
			DIJOYSTATE2 aGamePadKeyState;								// �R���g���[���[�̓��͏��
			BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// �X�e�B�b�N�̓��͏��
			BYTE aPov[POV_MAX] = { 0,0,0,0 };							// �\���L�[�̓��͏��

			HRESULT hr;
			hr = m_aGamePad[nCnt].pDevGamePad->Poll();

			// ���̓f�o�C�X����f�[�^���擾
			if (SUCCEEDED(hr = m_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
			{
				// �{�^��
				for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
				{
					// �R���g���[���[�̓��͏��̕ۑ�(�g���K�[)
					m_aGamePad[nCnt].aGamePadKeyStateTrigger[nCntKey] = (m_aGamePad[nCnt].aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
					// �R���g���[���[�̓��͏��̕ۑ�(�����[�X)
					m_aGamePad[nCnt].aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ m_aGamePad[nCnt].aGamePadKeyState[nCntKey]) & m_aGamePad[nCnt].aGamePadKeyState[nCntKey];
					// �R���g���[���[�̓��͏��̕ۑ�(�v���X)
					m_aGamePad[nCnt].aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
				}

				//--------------------------------------------------------
				// ���A�i���O�X�e�B�b�N�̓��͏��̎擾
				//--------------------------------------------------------
				if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
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
					m_aGamePad[nCnt].aLeftAnalogStickTrigger[nCntAnalogStick] = (m_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
																																																				// ���X�e�B�b�N�̃v���X���
					m_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// ���A�i���O�X�e�B�b�N�̃v���X���
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
					m_aGamePad[nCnt].aPovTrigger[nCntPov] = (m_aGamePad[nCnt].aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// �\���L�[�̃g���K�[���

																																// �\���L�[�̃v���X���
					m_aGamePad[nCnt].aPov[nCntPov] = aPov[nCntPov];		// �\���L�[�̃v���X���
				}
				m_aGamePad[nCnt].aGamePad = aGamePadKeyState;			// ���݂̏�Ԃ�ۑ�
			}
			else
			{
				m_aGamePad[nCnt].pDevGamePad->Acquire();				// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��
			}

			if (m_aGamePad[nCnt].nCounterVibration > 0)
			{
				m_aGamePad[nCnt].nCounterVibration--;
				if (m_aGamePad[nCnt].nCounterVibration <= 0)
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
	if (m_aGamePad[nID].pEffect == NULL) return;

	if (nPower > 100)
	{// 100%�𒴂��Ă���ꍇ
		nPower = 100;
	}
	else if (nPower < 0)
	{// 0%�����̏ꍇ
		nPower = 0;
	}

	if (m_aGamePad[nID].pDevGamePad != NULL)
	{// �R���g���[���[���q�����Ă��Ȃ��Ƃ��������Ȃ�
		if (nPower > 0)
		{
			DIEFFECT diEffect;

			ZeroMemory(&diEffect, sizeof(diEffect));
			diEffect.dwSize = sizeof(DIEFFECT);
			diEffect.dwGain = DI_FFNOMINALMAX / 100 * nPower;
			m_aGamePad[nID].pEffect->SetParameters(&diEffect, DIEP_GAIN);

			m_aGamePad[nID].pEffect->Start(1, 0);
		}
		else
		{
			m_aGamePad[nID].pEffect->Stop();
		}
		m_aGamePad[nID].nCounterVibration = nTime;
	}
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�X�e�B�b�N)���擾
//==============================================================================================================================
CInputGamePad::GamePad *CInputGamePad::GetgamePadStick(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{// �f�o�C�X��NULL����Ȃ�������
		 // �W���C�X�e�B�b�N�̏����擾
			m_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &m_aGamePad[nCnt].aGamePad);
		}
	}
	return &m_aGamePad[0];			// �W���C�X�e�B�b�N�̏���Ԃ�
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�v���X���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadPress(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(m_aGamePad[nIndex].aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadTrigger(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(m_aGamePad[nIndex].aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�����[�X���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadRelease(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(m_aGamePad[nIndex].aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�v���X���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickPress(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad[nIndex].aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickTrigger(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad[nIndex].aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�v���X�[���)���擾
//=============================================================================
bool CInputGamePad::GetPovTpress(int nIndex, int nAngle)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad[nIndex].aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�g���K�[���)���擾
//=============================================================================
bool CInputGamePad::GetPovTtigger(int nIndex, int nAngle)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad[nIndex].aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick�p�R�[���o�b�N�֐�
//*****************************************************************************
BOOL CALLBACK CInputGamePad::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = m_pInput->CreateDevice(pdidinstance->guidInstance, &m_aGamePad[m_JoyconNumber].pDevGamePad, NULL);

	// �f�o�C�X�����o�o���Ȃ������ꍇ
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	m_JoyconNumber++;		// �ԍ���i�߂�

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
	hr = m_aGamePad[m_JoyconAxesNumber].pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

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

	hr = m_aGamePad[nCnt].pDevGamePad->CreateEffect(GUID_ConstantForce, &eff, &m_aGamePad[nCnt].pEffect, NULL);
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
	return m_JoyconNumber;
}