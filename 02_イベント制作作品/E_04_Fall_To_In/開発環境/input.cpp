////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 入力処理 [input.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "input.h"
#include "DebugProc.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
HWND CInput::m_hWnd;								// ウィンドウ

//==============================================================================================================================
// コンストラクタ
//==============================================================================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}
//==============================================================================================================================
// デストラクタ
//==============================================================================================================================
CInput::~CInput()
{

}
//==============================================================================================================================
// 初期化
//==============================================================================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (m_pInput == NULL)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		// ウィンドウ設定
		SetWnd(hWnd);
	}
	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CInput::Uninit(void)
{
	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}
//==============================================================================================================================
// ウィンドウ設定
//==============================================================================================================================
void CInput::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
//==============================================================================================================================
// ウィンドウ取得
//==============================================================================================================================
HWND CInput::GetWnd(void)
{
	return m_hWnd;
}

//******************************************************************************************************************************************************
// キーボードクラス
//******************************************************************************************************************************************************
//==============================================================================================================================
// コンストラクタ
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
// デストラクタ
//==============================================================================================================================
CInputKeyboard::~CInputKeyboard()
{

}
//==============================================================================================================================
// 初期化
//==============================================================================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// オブジェクト生成
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

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CInputKeyboard::Uninit(void)
{
	// 終了
	CInput::Uninit();
}

//==============================================================================================================================
// キーボードの更新処理
//==============================================================================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;
	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_akeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];	// キーボードの入力情報保存
			m_aKeyStateRelease[nCntKey] = (m_akeyState[nCntKey] ^ aKeyState[nCntKey])&m_akeyState[nCntKey];	// キーボードの入力情報保存
			m_akeyState[nCntKey] = aKeyState[nCntKey];			// キーボードの入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();	// キーボードへのアクセス権を獲得
	}

}

//==============================================================================================================================
// キーボードの入力情報(プレス情報)を取得
//==============================================================================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_akeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// キーボードの入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;

}
//==============================================================================================================================
// キーボードの入力情報(リリース情報)を取得
//==============================================================================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//******************************************************************************************************************************************************
// マウスクラス
//******************************************************************************************************************************************************
//==============================================================================================================================
// コンストラクタ
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
// デストラクタ
//==============================================================================================================================
CInputMouse::~CInputMouse()
{

}
//==============================================================================================================================
// 初期化
//==============================================================================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// オブジェクト生成
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(マウス)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
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

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CInputMouse::Uninit(void)
{
	// 終了
	CInput::Uninit();
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void CInputMouse::Update(void)
{

	DIMOUSESTATE2 aMouseState;	// マウスの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aMouseState), &aMouseState)))
	{
		for (nCntKey = 0; nCntKey < NUM_MOUSE_MAX; nCntKey++)
		{
			m_aKeyStateTrigger.rgbButtons[nCntKey] =
				(m_akeyState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) & aMouseState.rgbButtons[nCntKey];	// マウスの入力情報保存(トリガー）

			m_aKeyStateRelease.rgbButtons[nCntKey] =
				(m_akeyState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) & m_akeyState.rgbButtons[nCntKey];	// マウスの入力情報保存(リリース）

			m_akeyState.rgbButtons[nCntKey] = aMouseState.rgbButtons[nCntKey];	// マウスの入力情報保存	(プレス)
		}
		// ホイール情報保存
		m_akeyState.lZ = aMouseState.lZ;
	}
	else
	{
		m_pDevice->Acquire();
	}
	// ウィンドウ取得
	SetPoint(CInput::GetWnd());

}

//==============================================================================================================================
// マウスの入力情報(プレス情報)を取得
//==============================================================================================================================
bool CInputMouse::GetPress(int nKey)
{
	return(m_akeyState.rgbButtons[nKey-1] & 0x80) ? true : false;
}

//==============================================================================================================================
// マウスの入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CInputMouse::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger.rgbButtons[nKey-1] & 0x80) ? true : false;

}
//==============================================================================================================================
// マウスの入力情報(リリース情報)を取得
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
	//マウスクリック座標算出処理
	GetCursorPos(&m_Point);//カーソル位置の絶対座標を取得
	ScreenToClient(hWnd, &m_Point);//カーソル絶対座標から、そのウィンドウ内の座標に補正
	m_Point.x = LONG((FLOAT)m_Point.x * ((FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_WIDTH));//さらにサイズ変更を考慮して、現在のサイズで補正（枠サイズ等あるので厳密ではない）
	m_Point.y = LONG((FLOAT)m_Point.y * ((FLOAT)SCREEN_HEIGHT / (FLOAT)SCREEN_HEIGHT));

}
D3DXVECTOR3 CInputMouse::GetPoint(void)
{
	return D3DXVECTOR3(float(m_Point.x), float(m_Point.y),0.0f);
}

//******************************************************************************************************************************************************
// ゲームパッドクラス
//******************************************************************************************************************************************************
CInputGamePad::GamePad CInputGamePad::g_aGamePad[MAX_GAMEPAD];
int		   	   CInputGamePad::g_JoyconNumber = 0;				// joystick用コールバック関数で使うジョイコンの番号
int		   	   CInputGamePad::g_JoyconAxesNumber=0;			// joystickの機能コールバック関数で使うジョイコンの番号
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
		g_aGamePad[nCnt].pDevGamePad = NULL;
	}
	g_JoyconNumber = 0;
	g_JoyconAxesNumber = 0;

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
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{// コントローラーが繋がっていないとき処理しない
			// デバイスにデータフォーマットを設定
			if (FAILED(hr = g_aGamePad[nCnt].pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
			{
				return FALSE;
			}

			// 協調モードを設定
			if (FAILED(hr = g_aGamePad[nCnt].pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			{
				return FALSE;
			}

			// ゲームパッドの機能の情報を収集
			g_aGamePad[nCnt].GamepadCaps.dwSize = sizeof(DIDEVCAPS);

			if (FAILED(hr = g_aGamePad[nCnt].pDevGamePad->GetCapabilities(&g_aGamePad[nCnt].GamepadCaps)))
			{
				g_aGamePad[nCnt].pDevGamePad->Release();
				return FALSE;
			}

			m_NumForceFeedbackAxis = 0;

			// 各軸のモード設定
			hr = g_aGamePad[nCnt].pDevGamePad->EnumObjects(EnumAxesCallback, (void*)g_aGamePad[nCnt].pDevGamePad, DIDFT_AXIS);

			g_JoyconAxesNumber++;	// 番号を進める

			if (m_NumForceFeedbackAxis > 2) m_NumForceFeedbackAxis = 2;

			// エフェクトオブジェクトの生成
			if (!(CreateEffect(hWnd, nCnt)))
			{
				g_aGamePad[nCnt].pEffect = NULL;
			}

			// ゲームパッドへのアクセス権を獲得(入力制御開始)
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
// ゲームパッドの終了処理
//==============================================================================================================================
void CInputGamePad::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		// 振動停止
		SetVibration(0, 0, nCnt);

		// 入力デバイスの開放
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{
			g_aGamePad[nCnt].pDevGamePad->Unacquire();
			g_aGamePad[nCnt].pDevGamePad->Release();
			g_aGamePad[nCnt].pDevGamePad = NULL;
		}

		// エフェクトオブジェクトの開放
		if (g_aGamePad[nCnt].pEffect != NULL)
		{
			g_aGamePad[nCnt].pEffect->Release();
			g_aGamePad[nCnt].pEffect = NULL;
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
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{
			DIJOYSTATE2 aGamePadKeyState;								// コントローラーの入力情報
			BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// スティックの入力情報
			BYTE aPov[POV_MAX] = { 0,0,0,0 };							// 十字キーの入力情報

			HRESULT hr;
			hr = g_aGamePad[nCnt].pDevGamePad->Poll();

			// 入力デバイスからデータを取得
			if (SUCCEEDED(hr = g_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
			{
				// ボタン
				for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
				{
					// コントローラーの入力情報の保存(トリガー)
					g_aGamePad[nCnt].aGamePadKeyStateTrigger[nCntKey] = (g_aGamePad[nCnt].aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
					// コントローラーの入力情報の保存(リリース)
					g_aGamePad[nCnt].aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ g_aGamePad[nCnt].aGamePadKeyState[nCntKey]) & g_aGamePad[nCnt].aGamePadKeyState[nCntKey];
					// コントローラーの入力情報の保存(プレス)
					g_aGamePad[nCnt].aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
				}

				//--------------------------------------------------------
				// 左アナログスティックの入力情報の取得
				//--------------------------------------------------------
				if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE*100&& aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100&& aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
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
					g_aGamePad[nCnt].aLeftAnalogStickTrigger[nCntAnalogStick] = (g_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// 左アナログスティックのトリガー情報
																																																				// 左スティックのプレス情報
					g_aGamePad[nCnt].aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// 左アナログスティックのプレス情報
				}
				CDebugProc::print("%dスティック\n", aGamePadKeyState.lX);
				CDebugProc::print("%dスティック\n", aGamePadKeyState.lY);
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
					g_aGamePad[nCnt].aPovTrigger[nCntPov] = (g_aGamePad[nCnt].aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// 十字キーのトリガー情報

																																// 十字キーのプレス情報
					g_aGamePad[nCnt].aPov[nCntPov] = aPov[nCntPov];		// 十字キーのプレス情報
				}
				g_aGamePad[nCnt].aGamePad = aGamePadKeyState;			// 現在の状態を保存
			}
			else
			{
				g_aGamePad[nCnt].pDevGamePad->Acquire();				// ゲームパッドへのアクセス権を獲得
			}

			if (g_aGamePad[nCnt].nCounterVibration > 0)
			{
				g_aGamePad[nCnt].nCounterVibration--;
				if (g_aGamePad[nCnt].nCounterVibration <= 0)
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
	if (g_aGamePad[nID].pEffect == NULL) return;

	if (nPower > 100)
	{// 100%を超えている場合
		nPower = 100;
	}
	else if (nPower < 0)
	{// 0%未満の場合
		nPower = 0;
	}

	if (g_aGamePad[nID].pDevGamePad != NULL)
	{// コントローラーが繋がっていないとき処理しない
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
// ゲームパッドの入力情報(スティック)を取得
//==============================================================================================================================
CInputGamePad::GamePad *CInputGamePad::GetgamePadStick(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{// デバイスがNULLじゃなかったら
		 // ジョイスティックの情報を取得
			g_aGamePad[nCnt].pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &g_aGamePad[nCnt].aGamePad);
		}
	}
	return &g_aGamePad[0];			// ジョイスティックの情報を返す
}

//==============================================================================================================================
// ゲームパッドの入力情報(プレス情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadPress(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(g_aGamePad[nIndex].aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadTrigger(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(g_aGamePad[nIndex].aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(リリース情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadRelease(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(g_aGamePad[nIndex].aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// ゲームパッドのスティック入力情報(プレス情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickPress(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (g_aGamePad[nIndex].aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドのスティック入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CInputGamePad::GetGamePadStickTrigger(int nIndex, int nKey)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (g_aGamePad[nIndex].aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(プレスー情報)を取得
//=============================================================================
bool CInputGamePad::GetPovTpress(int nIndex, int nAngle)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (g_aGamePad[nIndex].aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(トリガー情報)を取得
//=============================================================================
bool CInputGamePad::GetPovTtigger(int nIndex, int nAngle)
{
	if (g_aGamePad[nIndex].pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (g_aGamePad[nIndex].aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick用コールバック関数
//*****************************************************************************
BOOL CALLBACK CInputGamePad::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = m_pInput->CreateDevice(pdidinstance->guidInstance, &g_aGamePad[g_JoyconNumber].pDevGamePad, NULL);

	// デバイスを検出出来なかった場合
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	g_JoyconNumber++;		// 番号を進める

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
	hr = g_aGamePad[g_JoyconAxesNumber].pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

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

	hr = g_aGamePad[nCnt].pDevGamePad->CreateEffect(GUID_ConstantForce, &eff, &g_aGamePad[nCnt].pEffect, NULL);
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
	return g_JoyconNumber;
}