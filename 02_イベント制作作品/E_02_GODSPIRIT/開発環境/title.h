////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// タイトルの処理 [title.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TITLE_H_
#define _TITLE_H_

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	TITLETYPE_LOGOBG = 0,		// ロゴ背景
	TITLETYPE_LOGO,			// ロゴ
	TITLETYPE_PRESSENTER,	// PressEnter
	TITLETYPE_MAX			// 最大
}TITLETYPE;
typedef enum
{
	TITLEFADE_NONE = 0,	// 何もない
	TITLEFADE_FADEIN,	// フェードイン
	TITLEFADE_FADEOUT	// フェードアウト
}TITLEFADE;
typedef enum
{
	TITLESTATE_NONE=0,		// 通常
	TITLESTATE_END			// 終わり
}TITLESTATE;
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 Initpos;// 初期位置
	D3DXCOLOR col;		// 色
	D3DXVECTOR3 move;	// 移動 
	TITLETYPE nType;	// 種類
	int nCounterAnim;	// アニメーションカウンター
	int nPatternAnim;	// アニメーションパターン
	int nCounterFade;	// フェードカウンタ
	float fWidth;		// 幅
	float fHeight;		// 高さ
	TITLEFADE nFade;	// フェード
	bool bUse;			// 使用しているかどうか
} Title;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitle(D3DXVECTOR3 pos,TITLETYPE nType,float fWidth,float fHeight);

#endif // !_TITLE_H_

