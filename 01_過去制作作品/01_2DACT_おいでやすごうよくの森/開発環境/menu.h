//=============================================================================
//
// キー色変え処理 [pausemenu.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MENU	(128)	// 背景の最大数



//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	MENUSTATE_NORMAL = 0,	//通常状態
	MENUSTATE_SELECT,			//選択状態
	MENUSTATE_MAX			//種類の総数
}MENUSTATE0;


typedef enum
{
	MENUSTATE_NONE = 0,	//通常状態
	MENUSTATE_USE,			//選択状態
	MENU_STATE_MAX			//種類の総数
}MENUSTATE;


typedef enum
{
	GAMEMODE = 0,	//コンティニュー
	MENU_RANKING,			//リトライ
	QUIT,			//クウィット
	MENU_MAX		//総数
}MENU;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//カラー
	MENU menu;
	int nType;			//種類
	MENUSTATE0 state;	//キーの状態
	MENUSTATE uState;
	int nCntState;

	bool bUse;			//使用しているかどうか
}PAUSEMENU;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMenu(void);
void UnInitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);




//void PushPauseMenu(int nNowSelect);
void PushMenu(void);

void SetMenu(D3DXVECTOR3 pos, int nType);
#endif