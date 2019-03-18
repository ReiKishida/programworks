//=============================================================================
//
// タイトル処理 [title.h]
// Author : ABE YUUTAROU
//
//=============================================================================
#ifndef _TITLE_H_			//	2重インクルード防止のマクロ定義
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
//構造体
//*****************************************************************************
typedef enum
{
	TITLE_NOMAL = 0,		//通常ダメージ
	TITLE_END,
	TITLE_MAX
}TITLESTATE;

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void InitTITLE(void);		//初期化処理タイトル
void UninitTITLE(void);		//終了処理タイトル
void UpdateTITLE(void);		//更新処理タイトル
void DrawTITLE(void);		//描画処理タイトル

#endif // !

