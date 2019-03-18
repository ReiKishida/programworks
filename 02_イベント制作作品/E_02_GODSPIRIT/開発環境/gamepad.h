//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// コントローラー入力処理 [gamepad.h]
// Author : Ryota Kinoshita
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAMEPAD_DEADZONE	(3)
#define MAX_ZONE			(100)

#define BUTTON_X	   (ELECOM_X)		// Xボタンの番号
#define BUTTON_Y	   (ELECOM_Y)		// Yボタンの番号
#define BUTTON_A	   (ELECOM_A)		// Aボタンの番号
#define BUTTON_B	   (ELECOM_B)		// Bボタンの番号
#define BUTTON_L1	   (ELECOM_L1)		// L1ボタンの番号
#define BUTTON_R1	   (ELECOM_R1)		// R1ボタンの番号
#define BUTTON_L2	   (ELECOM_L2)		// L2ボタンの番号
#define BUTTON_R2	   (ELECOM_R2)		// R2ボタンの番号
#define BUTTON_L_STICK (ELECOM_L_STICK)	// L_STICKボタンの番号
#define BUTTON_R_STICK (ELECOM_R_STICK)	// R_STICKボタンの番号
#define BUTTON_BACK	   (ELECOM_BACK)	// BACKボタンの番号
#define BUTTON_START   (ELECOM_START)	// STARTボタンの番号
#define BUTTON_GUIDE   (ELECOM_GUIDE)	// GUIDEボタンの番号
#define BUTTON_MAX	   (ELECOM_MAX)		// ボタンの最大

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
//typedef enum
//{
//	STICK_NONE = 0,
//	STICK_UP ,
//	STICK_RIGHT ,
//	STICK_LEFT ,
//	STICK_DOWN ,
//	STICK_MAX,
//}STICK_CTRL;

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
{
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

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd);
void UninitGamePad(void);
void UpdateGamePad(void);

// ゲームパッドのボタン入力情報
bool GetGamePadPress(int nKey);
bool GetGamePadTrigger(int nKey);
bool GetGamePadRelease(int nKey);

// ゲームパッドのスティックの入力情報
bool GetGamePadStickPress(int nKey);
bool GetGamePadStickTrigger(int nKey);

// ゲームパッドの十字キーの入力情報
bool GetPovTrigger(int nAngle);

DIJOYSTATE2 *GetgamePadStick(void);
#endif
