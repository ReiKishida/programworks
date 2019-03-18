//===========================================================================
//
// タイトル処理 [Title.h]
// Author : TEAM_Fascinator
//
//===========================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//===========================================================================
// タイトルの構造体
//===========================================================================
typedef enum
{
	TITLE_STATE_NONE = 0,
	TITLE_STATE_USE,
	TITLE_STATE_MAX
}TITLE_STATE;

typedef enum
{
	CHOICE_ONE = 0,
	CHOICE_TWO,
	CHOICE_THREE,
	CHOICE_MAX
}TITLE_CHOICE;

//===========================================================================
// プロトタイプ宣言(Title)
//===========================================================================
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
#endif