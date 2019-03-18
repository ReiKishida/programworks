//=============================================================================
//
// キー色変え処理 [pausemenu.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PAUSEMENU	(5)	// 背景の最大数



//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PAUSEMENUSTATE_NORMAL = 0,	//通常状態
	PAUSEMENUSTATE_SELECT,			//選択状態
	PAUSEMENUSTATE_MAX			//種類の総数
}PAUSEMENUSTATE;


typedef enum
{
	PAUSEMENUSTATE_NONE = 0,	//通常状態
	PAUSEMENUSTATE_USE,			//選択状態
	PAUSEMENU_STATE_MAX			//種類の総数
}PAUSESTATE;


typedef enum
{
	CONTINUE = 0,	//コンティニュー
	RETRY,			//リトライ
	QUIT,			//クウィット
	MAX_MENU		//総数
}MENU;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//カラー
	MENU menu;
	int nType;			//種類
	PAUSEMENUSTATE state;	//キーの状態
	PAUSESTATE uState;
	int nCntState;
	
	bool bUse;			//使用しているかどうか
}PAUSEMENU;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPauseMenu(void);
void UnInitPauseMenu(void);
void UpdatePauseMenu(void);
void DrawPauseMenu(void);




//void PushPauseMenu(int nNowSelect);
void PushPauseMenu(void);

void SetPauseMenu(D3DXVECTOR3 pos, int nType);
#endif