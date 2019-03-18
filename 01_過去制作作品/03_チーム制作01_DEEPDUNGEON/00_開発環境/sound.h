//***************************************************************************//
//
// サウンド処理 [sound.h]
// Author : TEAM_Fascinator
//
//***************************************************************************//
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//***************************************************************************//
// サウンドファイル
//***************************************************************************//
typedef enum
{
	SOUND_LABEL_BGM000 = 0,				// タイトル
	SOUND_LABEL_BGM001,					// キャラクターセレクト
	SOUND_LABEL_BGM002,					// ギャラリー
	SOUND_LABEL_BGM003,					// プロローグ
	SOUND_LABEL_BGM004,					// ゲーム
	SOUND_LABEL_BGM005,					// ボスバトル
	SOUND_LABEL_BGM006,					// 残瘴気75％
	SOUND_LABEL_BGM007,					// 残瘴気50％
	SOUND_LABEL_BGM008,					// 残瘴気25％
	SOUND_LABEL_BGM009,					// 残瘴気0％
	SOUND_LABEL_SE_DECISION,			// 決定
	SOUND_LABEL_SE_PROLOGUE_DECISION,	// チュートリアル決定
	SOUND_LABEL_SE_SELECT,				// 選択
	SOUND_LABEL_SE_PAUSE_DECISION,		// ポーズ決定
	SOUND_LABEL_SE_CANCEL,				// キャンセル
	SOUND_LABEL_SE_FOOTSTEP,				// キャンセル
	SOUND_LABEL_SE_DEAD,				// 死亡
	SOUND_LABEL_SE_SLASH,				// 斬撃
	SOUND_LABEL_SE_FIRE,				// 炎
	SOUND_LABEL_SE_SLASH2,				// 斬撃2
	SOUND_LABEL_SE_HIT,					// 斬撃2
	SOUND_LABEL_SE_HIT2,					// 斬撃2
	SOUND_LABEL_SE_EXPLOSION,					// 斬撃2
	SOUND_LABEL_SE_HIT3,					// 斬撃2
	SOUND_LABEL_SE_MEKURI,					// 斬撃2
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//***************************************************************************//
// プロトタイプ宣言(Sound)
//***************************************************************************//
HRESULT InitSound(HWND hWnd);
HRESULT PlaySound(SOUND_LABEL label); // ゲームの初期化近く　
void UninitSound(void);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
