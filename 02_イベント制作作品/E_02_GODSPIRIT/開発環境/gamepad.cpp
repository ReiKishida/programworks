////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コントローラー入力処理 [gamepad.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "gamepad.h"

//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define	GAMEPAD_NUM_KEY_MAX		(128)		// キーの最大数

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECTINPUT8 g_pGamePad = NULL;					// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevGamePad = NULL;			// 入力デバイス(kコントローラー)へのポインタ
DIDEVCAPS g_GamepadCaps;							// ゲームパッドの機能情報	

DIJOYSTATE2 g_aGamePad;								// コントローラーの情報

BYTE		g_aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];			// ジョイスティックのプレス入力情報
BYTE		g_aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];		// ジョイスティックのトリガー入力情報
BYTE		g_aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];		// ジョイスティックのトリガー入力情報
BYTE		g_aLeftAnalogStick[ANALOG_STICK_MAX];				// 左アナログスティックのプレス情報
BYTE		g_aLeftAnalogStickTrigger[ANALOG_STICK_MAX];		// 左アナログスティックのトリガー情報
BYTE		g_aPov[POV_MAX];									// 十字キーのプレス情報
BYTE		g_aPovTrigger[POV_MAX];								// 十字キーのトリガー情報

bool g_bTrigger;
//==============================================================================================================================
// ゲームパッドの初期化
//==============================================================================================================================
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	g_bTrigger = false;
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pGamePad, NULL)))
	{
		return FALSE;
	}

	// 入力デバイス(コントローラー)の生成
	if (FAILED(g_pGamePad->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// 第4引数 (ATTACHEDONLY = 現在接続されているジョイスティックのみ検出する)
	{
		return FALSE;
	}

	if (g_pDevGamePad != NULL)
	{// コントローラーが繋がっていないとき処理しない

	 // デバイスにデータフォーマットを設定
		if (FAILED(hr = g_pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return FALSE;
		}

		// 協調モードを設定
		if (FAILED(hr = g_pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return FALSE;
		}

		// ゲームパッドの機能の情報を収集 
		g_GamepadCaps.dwSize = sizeof(DIDEVCAPS);

		if (FAILED(hr = g_pDevGamePad->GetCapabilities(&g_GamepadCaps)))
		{
			g_pDevGamePad->Release();
			return FALSE;
		}

		// 各軸のモード設定
		if (FAILED(hr = g_pDevGamePad->EnumObjects(EnumAxesCallback, (void*)g_pDevGamePad, DIDFT_AXIS)))
		{
			return FALSE;
		}

		// ゲームパッドへのアクセス権を獲得(入力制御開始)
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
// ゲームパッドの終了処理
//==============================================================================================================================
void UninitGamePad(void)
{
	// 入力デバイス(キーボード)の開放
	if (g_pDevGamePad != NULL)
	{
		g_pDevGamePad->Unacquire();
		g_pDevGamePad->Release();
		g_pDevGamePad = NULL;
	}

	// DirectInputオブジェクトの開放
	if (g_pGamePad != NULL)
	{
		g_pGamePad->Release();
		g_pGamePad = NULL;
	}
}

//==============================================================================================================================
// ゲームパッドの更新処理
//==============================================================================================================================
void UpdateGamePad(void)
{
	if (g_pDevGamePad != NULL)
	{
		DIJOYSTATE2 aGamePadKeyState;		// コントローラーの入力情報
		BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// スティックの入力情報
		BYTE aPov[POV_MAX] = { 0,0,0,0 };							// 十字キーの入力情報

		HRESULT hr;
		hr = g_pDevGamePad->Poll();

		// 入力デバイスからデータを取得
		if (SUCCEEDED(hr = g_pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
		{
			// ボタン
			for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
			{
				// コントローラーの入力情報の保存(トリガー)
				g_aGamePadKeyStateTrigger[nCntKey] = (g_aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
				// コントローラーの入力情報の保存(リリース)
				g_aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ g_aGamePadKeyState[nCntKey]) & g_aGamePadKeyState[nCntKey];
				// コントローラーの入力情報の保存(プレス)
				g_aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
			}

			//--------------------------------------------------------
			// 左アナログスティックの入力情報の取得
			//--------------------------------------------------------
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE)
			{// 左スティックを上に倒した
				aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// 入力された
			}
			if (aGamePadKeyState.lY > GAMEPAD_DEADZONE)
			{// 左スティックを下に倒した
				aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// 入力された
			}
			if (aGamePadKeyState.lX > GAMEPAD_DEADZONE)
			{// 左スティックを右に倒した
				aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// 入力された
			}
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE)
			{// 左スティックを左に倒した
				aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// 入力された
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// 左アナログスティックの方向の種類分ループ
			 // 左スティックのトリガー情報
				g_aLeftAnalogStickTrigger[nCntAnalogStick] = (g_aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// 左アナログスティックのトリガー情報
																																												// 左スティックのプレス情報
				g_aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// 左アナログスティックのプレス情報
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
				g_aPovTrigger[nCntPov] = (g_aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// 十字キーのトリガー情報

																								// 十字キーのプレス情報
				g_aPov[nCntPov] = aPov[nCntPov];												// 十字キーのプレス情報
			}

			g_aGamePad = aGamePadKeyState;														// 現在の状態を保存

		}
		else
		{
			g_pDevGamePad->Acquire();		// ゲームパッドへのアクセス権を獲得
		}
	}
}

//==============================================================================================================================
// ゲームパッドの入力情報(スティック)を取得
//==============================================================================================================================
DIJOYSTATE2 *GetgamePadStick(void)
{

	if (g_pDevGamePad != NULL)
	{// デバイスがNULLじゃなかったら

	 // ジョイスティックの情報を取得
		g_pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &g_aGamePad);
	}

	return &g_aGamePad;			// ジョイスティックの情報を返す
}

//==============================================================================================================================
// ゲームパッドの入力情報(プレス情報)を取得
//==============================================================================================================================
bool GetGamePadPress(int nKey)
{
	return(g_aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(トリガー情報)を取得
//==============================================================================================================================
bool GetGamePadTrigger(int nKey)
{
	return(g_aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(リリース情報)を取得
//==============================================================================================================================
bool GetGamePadRelease(int nKey)
{
	return(g_aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// ゲームパッドのスティック入力情報(プレス情報)を取得
//==============================================================================================================================
bool GetGamePadStickPress(int nKey)
{
	return (g_aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドのスティック入力情報(トリガー情報)を取得
//==============================================================================================================================
bool GetGamePadStickTrigger(int nKey)
{
	return (g_aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(トリガー情報)を取得
//=============================================================================
bool GetPovTrigger(int nAngle)
{
	return (g_aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick用コールバック関数
//*****************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = g_pGamePad->CreateDevice(pdidinstance->guidInstance, &g_pDevGamePad, NULL);

	// デバイスを検出出来なかった場合
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

//*****************************************************************************
// joystickの機能コールバック関数
//*****************************************************************************
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// 十字キーの最小値 [上] [左]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;					// 十字キーの最大値 [下] [右]

																					// デバイスに設定
	hr = g_pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}