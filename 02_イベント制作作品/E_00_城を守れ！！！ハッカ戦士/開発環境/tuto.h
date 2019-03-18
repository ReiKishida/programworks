//=============================================================================
//
// ゲーム画面 [Tuto.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _TUTO_H_
#define _TUTO_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{
	TUTOSTATE_NONE = 0,	//何もしていない状態
	TUTOSTATE_NORMAL,	//通常状態
	TUTOSTATE_CLEAR,		//終了状態
	TUTOSTATE_FAILED,		//終了状態
	TUTOSTATE_MAX		//最大数
}TUTOSTATE;

typedef struct
{
	bool bPause;
}TUTOPAUSECHECK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTuto(void);
void UnInitTuto(void);
void UpdateTuto(void);
void DrawTuto(void);

void SetTutoState(TUTOSTATE state);
TUTOSTATE *GetTutoState(void);
TUTOSTATE *GetTutoState2(void);

//TUTOPAUSECHECK *GetPauseCheck(void);
#endif

