//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
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
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_CHARGESHOT,	//チャージショット
	SOUND_LABEL_SE_CHARGE,		// チャージ
	SOUND_LABEL_SE_HIT000,		// ヒット音0
	SOUND_LABEL_SE_HIT001,		// ヒット音1
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_SELECT,		//選択音
	SOUND_LABEL_SE_DECIDE,		// 決定音
	SOUND_LABEL_SE_VALCAN,		// バルカンcall
	SOUND_LABEL_SE_SPREAD,		// スプレッドcall
	SOUND_LABEL_SE_CANON,		// キャノンcall
	SOUND_LABEL_SE_CANON2,		// キャノン
	SOUND_LABEL_SE_SPREAD2,		// スプレッド
	SOUND_LABEL_SE_OVERHEAT,	//オーバーヒート
	SOUND_LABEL_SE_WARNINGS,	//警告音
	SOUND_LABEL_SE_CHARGE2,		//
	SOUND_LABEL_SE_SLASH,		//スラッシュ音
	SOUND_LABEL_SE_SLASH2,		//スラッシュ音2
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
