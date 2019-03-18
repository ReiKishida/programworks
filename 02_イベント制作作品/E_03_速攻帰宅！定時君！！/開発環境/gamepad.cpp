////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コントローラー入力処理 [gamepad.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "gamepad.h"

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECTINPUT8 g_pGamePad = NULL;			// DirectInputオブジェクトへのポインタ
GamePad		   g_aGamePad[MAX_GAMEPAD];		// ゲームパッドの情報
int		   	   g_JoyconNumber;				// joystick用コールバック関数で使うジョイコンの番号
int		   	   g_JoyconAxesNumber;			// joystickの機能コールバック関数で使うジョイコンの番号

//==============================================================================================================================
// ゲームパッドの初期化
//==============================================================================================================================
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		g_aGamePad[nCnt].pDevGamePad = NULL;
	}
	g_JoyconNumber = 0;
	g_JoyconAxesNumber = 0;

	HRESULT hr;
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

			// 各軸のモード設定
			hr = g_aGamePad[nCnt].pDevGamePad->EnumObjects(EnumAxesCallback, (void*)g_aGamePad[nCnt].pDevGamePad, DIDFT_AXIS);

			g_JoyconAxesNumber++;		// 番号を進める

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
void UninitGamePad(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		// 入力デバイス(キーボード)の開放
		if (g_aGamePad[nCnt].pDevGamePad != NULL)
		{
			g_aGamePad[nCnt].pDevGamePad->Unacquire();
			g_aGamePad[nCnt].pDevGamePad->Release();
			g_aGamePad[nCnt].pDevGamePad = NULL;
		}
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
				if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE)
				{// 左スティックを上に倒した
					aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// 入力された
				}
				else if (aGamePadKeyState.lY > GAMEPAD_DEADZONE)
				{// 左スティックを下に倒した
					aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// 入力された
				}
				else if (aGamePadKeyState.lX > GAMEPAD_DEADZONE)
				{// 左gスティックを右に倒した
					aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// 入力された
				}
				else if (aGamePadKeyState.lX < -GAMEPAD_DEADZONE)
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
		}
	}
}

//==============================================================================================================================
// ゲームパッドの入力情報(スティック)を取得
//==============================================================================================================================
GamePad *GetgamePadStick(void)
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
bool GetGamePadPress(int nIndex, int nKey)
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
bool GetGamePadTrigger(int nIndex, int nKey)
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
bool GetGamePadRelease(int nIndex, int nKey)
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
bool GetGamePadStickPress(int nIndex, int nKey)
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
bool GetGamePadStickTrigger(int nIndex, int nKey)
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
bool GetPovTpress(int nIndex, int nAngle)
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
bool GetPovTtigger(int nIndex, int nAngle)
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
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = g_pGamePad->CreateDevice(pdidinstance->guidInstance, &g_aGamePad[g_JoyconNumber].pDevGamePad, NULL);

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
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
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

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// ゲームパッドの刺さっている数の取得
//*****************************************************************************
int GetGamePadNum(void)
{
	return g_JoyconNumber;
}