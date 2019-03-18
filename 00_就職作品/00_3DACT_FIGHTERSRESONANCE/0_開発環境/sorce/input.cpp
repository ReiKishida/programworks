//=============================================================================
//
// 入力処理 [input.cpp]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
#include "input.h"

//*****************************************************************************
//静的メンバ変数
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;	//入力のポインタ

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CInput::CInput()
{//クリアな値を代入
	m_pDevice = NULL;
}


//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CInput::~CInput()
{

}
//=============================================================================
// 関数名：入力の初期化処理
// 関数の概要：DirectInputの生成
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 関数名：入力の終了処理
// 関数の概要：デバイスの破棄、入力の破棄
//=============================================================================
void CInput::Uninit(void)
{
	//デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//入力の破棄
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
//							  ～キーボードの処理～
//
//=============================================================================
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
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
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}
//=============================================================================
// 関数名：キーボードの初期化処理
// 関数の概要：キーボードの生成
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 関数名：キーボードの終了処理
// 関数の概要：入力の破棄
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 関数名：キーボードの更新処理
// 関数の概要：Press, Trigger, Releaseの処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//トリガー
			m_aKeyRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];	//リリース
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボードの入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//=============================================================================
// 関数名：キーボードの入力情報(プレス情報)を取得
// 関数の概要：Press情報を取得
//=============================================================================

bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 関数名：キーボードの入力情報(トリガー情報)を取得
// 関数の概要：Trigger情報を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 関数名：キーボードの入力情報(リリース)を取得
// 関数の概要：Trigger情報を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
//
//
//								CInputGamePad
//							  ～ゲームパッドの処理～
//
//=============================================================================

CInputGamePad::GamePad CInputGamePad::m_aGamePad[MAX_GAMEPAD];
int		   	   CInputGamePad::m_JoyconNumber = 0;				// joystick用コールバック関数で使うジョイコンの番号
int		   	   CInputGamePad::m_JoyconAxesNumber = 0;			// joystickの機能コールバック関数で使うジョイコンの番号
DWORD			CInputGamePad::m_NumForceFeedbackAxis = 0;
//==============================================================================================================================
// コンストラクタ
//==============================================================================================================================
CInputGamePad::CInputGamePad()
{
}
//==============================================================================================================================
// デストラクタ
//==============================================================================================================================
CInputGamePad::~CInputGamePad()
{

}
//==============================================================================================================================
// ゲームパッドの初期化
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
	// オブジェクト生成
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(コントローラー)の生成
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// 第4引数 (ATTACHEDONLY = 現在接続されているジョイスティックのみ検出する)
	{
		return FALSE;
	}

	// フォースフィードバック機能のある入力デバイスの生成
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{// コントローラーが繋がっていないとき処理しない
		 // デバイスにデータフォーマットを設定
			if (FAILED(hr = m_aGamePad[nCnt].pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
			{
				return FALSE;
			}

			// 協調モードを設定
			if (FAILED(hr = m_aGamePad[nCnt].pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			{
				return FALSE;
			}

			// ゲームパッドの機能の情報を収集
			m_aGamePad[nCnt].GamepadCaps.dwSize = sizeof(DIDEVCAPS);

			if (FAILED(hr = m_aGamePad[nCnt].pDevGamePad->GetCapabilities(&m_aGamePad[nCnt].GamepadCaps)))
			{
				m_aGamePad[nCnt].pDevGamePad->Release();
				return FALSE;
			}

			m_NumForceFeedbackAxis = 0;

			// 各軸のモード設定
			hr = m_aGamePad[nCnt].pDevGamePad->EnumObjects(EnumAxesCallback, (void*)m_aGamePad[nCnt].pDevGamePad, DIDFT_AXIS);

			m_JoyconAxesNumber++;	// 番号を進める

			if (m_NumForceFeedbackAxis > 2) m_NumForceFeedbackAxis = 2;

			// エフェクトオブジェクトの生成
			if (!(CreateEffect(hWnd, nCnt)))
			{
				m_aGamePad[nCnt].pEffect = NULL;
			}

			// ゲームパッドへのアクセス権を獲得(入力制御開始)
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
// ゲームパッドの終了処理
//==============================================================================================================================
void CInputGamePad::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		// 振動停止
		SetVibration(0, 0, nCnt);

		// 入力デバイスの開放
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{
			m_aGamePad[nCnt].pDevGamePad->Unacquire();
			m_aGamePad[nCnt].pDevGamePad->Release();
			m_aGamePad[nCnt].pDevGamePad = NULL;
		}

		// エフェクトオブジェクトの開放
		if (m_aGamePad[nCnt].pEffect != NULL)
		{
			m_aGamePad[nCnt].pEffect->Release();
			m_aGamePad[nCnt].pEffect = NULL;
		}
	}

	// DirectInputオブジェクトの開放
	CInput::Uninit();
}

//==============================================================================================================================
// ゲームパッドの更新処理
//==============================================================================================================================
void CInputGamePad::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{
			DIJOYSTATE2 aGamePadKeyState;								// コントローラーの入力情報
			BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// スティックの入力情報
			BYTE aPov[POV_MAX] = { 0,0,0,0 };							// 十字キーの入力情報

			HRESULT hr;
			hr = m_aGamePad[nCnt].pDevGamePad->Poll();

			// 入力デバイスからデータを取得
			if (SUCCEEDED(hr = m_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
			{
				// ボタン
				for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
				{
					// コントローラーの入力情報の保存(トリガー)
					m_aGamePad[nCnt].aGamePadKeyStateTrigger[nCntKey] = (m_aGamePad[nCnt].aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
					// コントローラーの入力情報の保存(リリース)
					m_aGamePad[nCnt].aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ m_aGamePad[nCnt].aGamePadKeyState[nCntKey]) & m_aGamePad[nCnt].aGamePadKeyState[nCntKey];
					// コントローラーの入力情報の保存(プレス)
					m_aGamePad[nCnt].aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
				}

				//--------------------------------------------------------
				// 左アナログスティックの入力情報の取得
				//--------------------------------------------------------
				if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
				{// 左スティックを上に倒した
					aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// 入力された
				}
				else if (aGamePadKeyState.lY > GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
				{// 左スティックを下に倒した
					aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// 入力された
				}
				else if (aGamePadKeyState.lX > GAMEPAD_DEADZONE * 100)
				{// 左gスティックを右に倒した
					aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// 入力された
				}
				else if (aGamePadKeyState.lX < -GAMEPAD_DEADZONE * 100)
				{// 左スティックを左に倒した
					aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// 入力された
				}

				for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
				{// 左アナログスティックの方向の種類分ループ
				 // 左スティックのトリガー情報
					m_aGamePad[nCnt].aLeftAnalogStickTrigger[nCntAnalogStick] = (m_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// 左アナログスティックのトリガー情報
																																																				// 左スティックのプレス情報
					m_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// 左アナログスティックのプレス情報
				}
				//--------------------------------------------------------
				// 十字キーの入力情報の取得
				//--------------------------------------------------------
				if (aGamePadKeyState.rgdwPOV[0] == 0.0f)
				{// 十字キーの上を押した
					aPov[POV_UP] = 128;			// 入力された
				}
				if (aGamePadKeyState.rgdwPOV[0] == 18000.0f)
				{// 十字キーの下を押した
					aPov[POV_DOWN] = 128;			// 入力された
				}
				if (aGamePadKeyState.rgdwPOV[0] == 9000.0f)
				{// 十字キーの右を押した
					aPov[POV_RIGHT] = 128;			// 入力された
				}
				if (aGamePadKeyState.rgdwPOV[0] == 27000.0f)
				{// 十字キーの左を押した
					aPov[POV_LEFT] = 128;			// 入力された
				}

				for (int nCntPov = 0; nCntPov < POV_MAX; nCntPov++)
				{// 十字キーの方向の種類分ループ
				 // 十字キーのトリガー情報
					m_aGamePad[nCnt].aPovTrigger[nCntPov] = (m_aGamePad[nCnt].aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// 十字キーのトリガー情報

																																// 十字キーのプレス情報
					m_aGamePad[nCnt].aPov[nCntPov] = aPov[nCntPov];		// 十字キーのプレス情報
				}
				m_aGamePad[nCnt].aGamePad = aGamePadKeyState;			// 現在の状態を保存
			}
			else
			{
				m_aGamePad[nCnt].pDevGamePad->Acquire();				// ゲームパッドへのアクセス権を獲得
			}

			if (m_aGamePad[nCnt].nCounterVibration > 0)
			{
				m_aGamePad[nCnt].nCounterVibration--;
				if (m_aGamePad[nCnt].nCounterVibration <= 0)
				{// カウンターが0になったら振動停止
					SetVibration(0, 0, nCnt);
				}
			}
		}
	}
}

//=============================================================================
// 振動の設定
//=============================================================================
void CInputGamePad::SetVibration(int nPower, int nTime, int nID)
{
	if (m_aGamePad[nID].pEffect == NULL) return;

	if (nPower > 100)
	{// 100%を超えている場合
		nPower = 100;
	}
	else if (nPower < 0)
	{// 0%未満の場合
		nPower = 0;
	}

	if (m_aGamePad[nID].pDevGamePad != NULL)
	{// コントローラーが繋がっていないとき処理しない
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
// ゲームパッドの入力情報(スティック)を取得
//==============================================================================================================================
CInputGamePad::GamePad *CInputGamePad::GetgamePadStick(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (m_aGamePad[nCnt].pDevGamePad != NULL)
		{// デバイスがNULLじゃなかったら
		 // ジョイスティックの情報を取得
			m_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &m_aGamePad[nCnt].aGamePad);
		}
	}
	return &m_aGamePad[0];			// ジョイスティックの情報を返す
}

//==============================================================================================================================
// ゲームパッドの入力情報(プレス情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadPress(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(m_aGamePad[nIndex].aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadTrigger(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(m_aGamePad[nIndex].aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(リリース情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadRelease(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(m_aGamePad[nIndex].aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// ゲームパッドのスティック入力情報(プレス情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickPress(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad[nIndex].aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドのスティック入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickTrigger(int nIndex, int nKey)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad[nIndex].aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(プレスー情報)を取得
//=============================================================================
bool CInputGamePad::GetPovTpress(int nIndex, int nAngle)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad[nIndex].aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(トリガー情報)を取得
//=============================================================================
bool CInputGamePad::GetPovTtigger(int nIndex, int nAngle)
{
	if (m_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad[nIndex].aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick用コールバック関数
//*****************************************************************************
BOOL CALLBACK CInputGamePad::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = m_pInput->CreateDevice(pdidinstance->guidInstance, &m_aGamePad[m_JoyconNumber].pDevGamePad, NULL);

	// デバイスを検出出来なかった場合
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	m_JoyconNumber++;		// 番号を進める

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// joystickの機能コールバック関数
//*****************************************************************************
BOOL CALLBACK CInputGamePad::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// 十字キーの最小値 [上] [左]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;				// 十字キーの最大値 [下] [右]

																				// デバイスに設定
	hr = m_aGamePad[m_JoyconAxesNumber].pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	if ((pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0)
	{// フォースフィードバックの軸をカウント
		m_NumForceFeedbackAxis++;
	}

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// エフェクトオブジェクトの生成
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
// ゲームパッドの刺さっている数の取得
//*****************************************************************************
int CInputGamePad::GetGamePadNum(void)
{
	return m_JoyconNumber;
}