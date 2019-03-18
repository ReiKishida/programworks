//=============================================================================
//
// 背景処理 [bg.h]
// Author : 圷 和也
//
//=============================================================================
#ifndef _BG_H_		// 2重インクルード防止のマクロ定義
#define _BG_H_

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BG_TYPE_NORMAL = 0,	// 通常背景
	BG_TYPE_SCROLL,		// スクロール背景
	BG_TYPE_MAX			// 総数
}BG_TYPE;

typedef enum
{
	BG_TEX_RESULT = 0,	// 空
	BG_TEX_RANKING,		// 空
	BG_TEX_MAX,			// 総数
}BgTexName;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void SetBg(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fWidth, float fHeight, int nTexType, BG_TYPE type);

#endif
