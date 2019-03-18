//=============================================================================
//
// 入力処理 [inout.h]
// Author :岸田怜(KishidaRei)
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX (256)

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
#define BUTTON_ARCADESTART   (9)						// アケコンSTARTボタンの番号
#define BUTTON_GUIDE   (CInputGamePad::ELECOM_GUIDE)	// GUIDEボタンの番号
#define BUTTON_MAX	   (CInputGamePad::ELECOM_MAX)		// ボタンの最大

//*****************************************************************************
// 入力のクラス構造体
//*****************************************************************************
class CInput
{
public:
	CInput();			//コンストラクタ
	virtual ~CInput();	//デストラクタ
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);//初期化処理
	virtual void Uninit(void);							 //終了処理
	virtual void Update(void) = 0;						 //更新処理

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};
//=========================================================
//	キーボード入力
//=========================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard::CInputKeyboard();
	CInputKeyboard::~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyboardPress(int nKey);		//押している状態
	bool GetKeyboardTrigger(int nKey);		//単押し
	bool GetKeyboardRelease(int nKey);		//離したとき

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyTrigger[NUM_KEY_MAX];
	BYTE m_aKeyRelease[NUM_KEY_MAX];
};

//====================================
// ゲームパッドクラス[DirectInput]
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
	static GamePad  m_aGamePad[MAX_GAMEPAD];	// ゲームパッドの情報
	static int		m_JoyconNumber;				// コールバック関数で使番号
	static int		m_JoyconAxesNumber;			// jコールバック関数で使う番号
	static DWORD	m_NumForceFeedbackAxis;		// フォースフィードバック
};

#endif
