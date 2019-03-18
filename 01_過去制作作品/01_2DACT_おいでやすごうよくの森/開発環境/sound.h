//=============================================================================
//
// サウンド処理 [sound.h]
// Author :Kishida Rei
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// サウンドファイル
//*************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM_RESULT,		// リザルト
	SOUND_LABEL_BGM_RANKING,	// ランキング
	SOUND_LABEL_SE_CHARGE,		// チャージ
	SOUND_LABEL_SE_HIT000,		// ヒット音0
	SOUND_LABEL_SE_HIT001,		// ヒット音1
	SOUND_LABEL_SE_SELECT,		//選択音
	SOUND_LABEL_SE_DECIDE,		// 決定音
	SOUND_LABEL_SE_COIN,		// コイン
	SOUND_LABEL_SE_JUMP,		// ジャンプ
	SOUND_LABEL_SE_DRINK,		// 飲む
	SOUND_LABEL_SE_PEOPLE,		// 歓声
	SOUND_LABEL_SE_PEOPLE2,		// 歓声
	SOUND_LABEL_SE_TRUMPET,		// トランペット
	SOUND_LABEL_SE_MEKURI,		// めくり
	SOUND_LABEL_SE_DRAMA,		// めくり
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
