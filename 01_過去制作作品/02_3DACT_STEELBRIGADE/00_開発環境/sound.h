//=============================================================================
//
// サウンド処理 [sound.h]
// Author : REI KISHIDA
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
	SOUND_LABEL_TITLE000 = 0,		// BGM0
	SOUND_LABEL_GAME000,			// BGM1
	SOUND_LABEL_RANKING,			// BGM2
	SOUND_LABEL_TUTORIAL000,		// BGM4
	SOUND_LABEL_WORLD,				// ワールド
	SOUND_LABEL_BR,				// ブリーフィング
	SOUND_LABEL_SE_SHOT,			// 射撃音
	SOUND_LABEL_SE_EXPLOSION,			// 爆発
	SOUND_LABEL_SE_QUICK,			// クイック音
	SOUND_LABEL_SE_LAND,			// 着地音
	SOUND_LABEL_SE_STEAM,			// スチーム
	SOUND_LABEL_SE_WALK,			// 歩行
	SOUND_LABEL_SE_BIRI,			//ビリビリ
	SOUND_LABEL_SE_SETUP,			//セットアップ
	SOUND_LABEL_SE_DECIDE,			// 決定音
	SOUND_LABEL_MAX,
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
