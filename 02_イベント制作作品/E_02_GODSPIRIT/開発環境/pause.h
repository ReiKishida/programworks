////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ポーズの処理 [pause.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PAUSE_H_
#define _PAUSE_H_
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	PAUSETYPE_BG = 0,
	PAUSETYPE_PAUSE,
	PAUSETYPE_CONTINUE,
	PAUSETYPE_RETRY,
	PAUSETYPE_QUIT,
	PAUSE_LOCK_PUNCH_PUNCH,				// パンチ->パンチ
	PAUSE_LOCK_PUNCH_PUNCH_PUNCH,		// パンチ->パンチ->パンチ
	PAUSE_LOCK_PUNCH_SPIN,				// パンチ->スピン
	PAUSE_LOCK_PUNCH_SPIN_JUMP,			// パンチ->スピン->ジャンプ
	PAUSE_LOCK_KICK_KICK,				// キック->キック
	PAUSE_LOCK_KICK_KICK_KICK,			// キック->キック->キック
	PAUSE_LOCK_KICK_SPIN,				// キック->スピン
	PAUSE_LOCK_KICK_JUMP,				// キック->ジャンプ
	PAUSE_LOCK_KICK_JUMP_PUNCH,			// キック->ジャンプ->パンチ	
	PAUSE_LOCK_SPIN_PUNCH,				// スピン->パンチ
	PAUSE_LOCK_SPIN_KICK,				// スピン->キック
	PAUSE_LOCK_SPIN_PUNCH_KICK,			// スピン->パンチ->キック	
	PAUSE_LOCK_JUMP_KICK,				// ジャンプ->キック
	PAUSE_LOCK_JUMP_SPIN,				// ジャンプ->スピン
	PAUSE_LOCK_JUMP_PUNCH,				// ジャンプ->パンチ
	PAUSETYPE_MAX
}PAUSETYPE;
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 Initpos;// 初期位置
	D3DXCOLOR col;		// 色
	D3DXVECTOR3 move;	// 移動 
	int nType;			// 種類
	int nCounterAnim;	// カウンター
	int nPatternAnim;	// パターン
	float fWidth;		// 幅
	float fHeight;		// 高さ
	bool bUse;			// 使用しているかどうか
} PAUSE;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, float fWidth, float fHeight);
PAUSE *GetPauseData(void);
#endif // !_PAUSE_H_

