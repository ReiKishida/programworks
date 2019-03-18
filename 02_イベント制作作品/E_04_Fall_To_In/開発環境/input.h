////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 入力処理 [input.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数
#define	NUM_MOUSE_MAX			(8)	// マウスのキーの最大数

#define GAMEPAD_DEADZONE		(3)
#define MAX_ZONE				(1000)
#define	GAMEPAD_NUM_KEY_MAX		(128)		// キーの最大数
#define MAX_GAMEPAD				(4)			// コントローラーの数

#define BUTTON_X	   (CInputGamePad::ELECOM_X)		// Xボタンの番号
#define BUTTON_Y	   (CInputGamePad::ELECOM_Y)		// Yボタンの番号
#define BUTTON_A	   (CInputGamePad::ELECOM_A)		// Aボタンの番号
#define BUTTON_B	   (CInputGamePad::ELECOM_B)		// Bボタンの番号
#define BUTTON_L1	   (CInputGamePad::ELECOM_L1)		// L1ボタンの番号
#define BUTTON_R1	   (CInputGamePad::ELECOM_R1)		// R1ボタンの番号
#define BUTTON_L2	   (CInputGamePad::ELECOM_L2)		// L2ボタンの番号
#define BUTTON_R2	   (CInputGamePad::ELECOM_R2)		// R2ボタンの番号
#define BUTTON_L_STICK (CInputGamePad::ELECOM_L_STICK)	// L_STICKボタンの番号
#define BUTTON_R_STICK (CInputGamePad::ELECOM_R_STICK)	// R_STICKボタンの番号
#define BUTTON_BACK	   (CInputGamePad::ELECOM_BACK)	// BACKボタンの番号
#define BUTTON_START   (CInputGamePad::ELECOM_START)	// STARTボタンの番号
#define BUTTON_GUIDE   (CInputGamePad::ELECOM_GUIDE)	// GUIDEボタンの番号
#define BUTTON_MAX	   (CInputGamePad::ELECOM_MAX)		// ボタンの最大

//====================================
// インプットクラス
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
	static LPDIRECTINPUT8 m_pInput;		// インプットデバイス
	LPDIRECTINPUTDEVICE8 m_pDevice;		// デバイス
	static HWND m_hWnd;					// ウィンドウ
};
//====================================
// キーボードクラス（派生）
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
	BYTE m_akeyState[NUM_KEY_MAX];				// キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードの入力情報（トリガー情報）
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];		// キーボードの入力情報（リリース情報）

};
//====================================
// マウスクラス（派生）
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
	DIMOUSESTATE2 m_akeyState;				// マウスの入力情報（プレス情報）
	DIMOUSESTATE2 m_aKeyStateTrigger;		// マウスの入力情報（トリガー情報）
	DIMOUSESTATE2 m_aKeyStateRelease;		// マウスの入力情報（リリース情報）

	POINT m_Point;							// マウスの座標
};
//====================================
// ゲームパッドクラス(派生）
//====================================
class CInputGamePad :public CInput
{
public:
	typedef enum
	{// アナログスティックの方向の種類(4方向)
		ANALOG_STICK_UP = 0,
		ANALOG_STICK_RIGHT,
		ANALOG_STICK_LEFT,
		ANALOG_STICK_DOWN,
		ANALOG_STICK_MAX
	}ANALOG_STICK;
	typedef enum
	{// 十字キーの方向の種類
		POV_UP = 0,
		POV_DOWN,
		POV_RIGHT,
		POV_LEFT,
		POV_MAX
	}POV;
	typedef enum
	{// ボタンの種類
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
		LPDIRECTINPUTDEVICE8 pDevGamePad;							// 入力デバイス(kコントローラー)へのポインタ
		DIDEVCAPS GamepadCaps;										// ゲームパッドの機能情報
		LPDIRECTINPUTEFFECT	pEffect;								// エフェクトオブジェクト

		DIJOYSTATE2 aGamePad;										// コントローラーの情報

		BYTE		aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];			// ジョイスティックのプレス入力情報
		BYTE		aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];	// ジョイスティックのトリガー入力情報
		BYTE		aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];	// ジョイスティックのリリース入力情報
		BYTE		aLeftAnalogStick[ANALOG_STICK_MAX];				// 左アナログスティックのプレス情報
		BYTE		aLeftAnalogStickTrigger[ANALOG_STICK_MAX];		// 左アナログスティックのトリガー情報
		BYTE		aPov[POV_MAX];									// 十字キーのプレス情報
		BYTE		aPovTrigger[POV_MAX];							// 十字キーのトリガー情報

		int			nCounterVibration;								// 振動用のカウンター
	}GamePad;

	CInputGamePad();
	~CInputGamePad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void SetVibration(int nPower, int nTime, int nID);

	// ゲームパッドのボタン入力情報
	bool GetGamePadPress(int nIndex, int nKey);
	bool GetGamePadTrigger(int nIndex, int nKey);
	bool GetGamePadRelease(int nIndex, int nKey);

	// ゲームパッドのスティックの入力情報
	bool GetGamePadStickPress(int nIndex, int nKey);
	bool GetGamePadStickTrigger(int nIndex, int nKey);

	// ゲームパッドの十字キーの入力情報
	bool GetPovTpress(int nIndex, int nAngle);
	bool GetPovTtigger(int nIndex, int nAngle);

	GamePad *GetgamePadStick(void);
	int GetGamePadNum(void);
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
	BOOL CreateEffect(HWND hWnd, int nCnt);
private:
	static GamePad  g_aGamePad[MAX_GAMEPAD];	// ゲームパッドの情報
	static int		g_JoyconNumber;				// joystick用コールバック関数で使うジョイコンの番号
	static int		g_JoyconAxesNumber;			// joystickの機能コールバック関数で使うジョイコンの番号
	static DWORD	m_NumForceFeedbackAxis;		// フォースフィードバック
};
#endif