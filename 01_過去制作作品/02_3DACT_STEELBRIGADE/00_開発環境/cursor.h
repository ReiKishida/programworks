//=============================================================================
//
// ポリゴンの処理 [Cursor.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _Cursor_H_
#define _Cursor_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	CURSORSTATE_NORMAL = 0,	//通常状態
	CURSORSTATE_OVERLAP,		//重なり状態
	
	CursorYSTATE_MAX			//種類の総数
}CURSORSTATE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 move;	//移動量
	CURSORSTATE state;	//プレイヤーの状態
	bool bDisp;			//使用されているか
	bool bUpdate;
	int nIndex;
}CURSOR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCursor(void);
void UnInitCursor(void);
void UpdateCursor(void);
void DrawCursor(void);

CURSOR GetCursor000(void);


#endif