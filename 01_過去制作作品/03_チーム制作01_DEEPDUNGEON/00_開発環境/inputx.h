//=============================================================================
//
// inputx処理 [inputx.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _INPUTX_H_
#define _INPUTX_H_

#include "main.h"

//=============================================================================
// ジョイパッドの構造体
//=============================================================================
typedef enum
{
	INPUT_JOYSTATE_NOTPUSH = 0,
	INPUT_JOYSTATE_PUSH,
	INPUT_JOYSTATE_NONE,
}InputJoyState;

typedef struct
{
	InputJoyState nJoypadState;		//ジョイパッドの状態
	bool bConnectionJoypad;			//ジョイパッドの接続状態
}JoyState;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitJoyPad(void);
void UninitJoyPad(void);
void UpdateJoyPad(void);
JoyState GetJoystate(void);
#endif