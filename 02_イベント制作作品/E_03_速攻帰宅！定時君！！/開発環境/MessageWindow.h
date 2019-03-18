////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッセージウィンドウ処理 [message.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	MESSAGETEX_TITLE = 0,		// タイトル
	MESSAGETEX_TEIJI,		// タイトル
	MESSAGETEX_KAEROU,		// タイトル
	MESSAGETEX_SOUSA,		// タイトル
	MESSAGETEX_KAERU ,		// タイトル
	MESSAGETEX_MAX,
}MESSAGETEX;
typedef enum
{
	MESSAGESTATE_NONE = 0,
	MESSAGESTATE_SET,
	MESSAGESTATE_DELETE,
	MESSAGESTATE_MAX,
}MESSAGESTATE;
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	MESSAGETEX nType;			// 種類
	MESSAGESTATE nState;	// 状態
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fDiffWidth;		// 幅
	float fDiffHeight;		// 高さ
	bool bUse;				// 使用されているかどうが
}MessageWindow;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitMessageWindow(void);
void UninitMessageWindow(void);
void UpdateMessageWindow(void);
void DrawMessageWindow(void);
void SetMessageWindow(D3DXVECTOR3 pos, float DiffWidth, float DiffHigeht, MESSAGETEX nType);
void DeleteMessageWndow(void);
MessageWindow *GetMessageWindow(void);
#endif
