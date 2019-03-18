////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// リザルトの処理 [result.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RESULT_H_
#define _RESILT_H_

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動 
	D3DXCOLOR col;		// 色
	int nCounterAnim;	// カウンター
	int nPatternAnim;	// パターン
	int nCounterFade;	// フェードカウンター
	int nType;			// 種類
	float fWidth;		// 幅
	float fHeight;		// 縦
	bool bUse;			// 使用しているかどうか
}Result;
typedef enum
{
	RESULTTEXTYPE_SCORE_MAX=0,
	RESULTTEXTYPE_SCORE,
	RESULTTEXTYPE_TIME,
	RESULTTEXTYPE_HIT,
	RESULTTEXTYPE_TEXT_SCORE_MAX,
	RESULTTEXTYPE_TEXT_SCORE,
	RESULTTEXTYPE_TEXT_TIME,
	RESULTTEXTYPE_TEXT_HIT,
	RESULTTEXTYPE_TEXT_DOUBLEKORON,
	RESULTTEXTYPE_TEXT_DOUBLEKORON1,
	RESULTTEXTYPE_TEXT_DOUBLEKORON2,
	RESULTTEXTYPE_TEXT_DOUBLEKORON3,
	RESULTTEXTYPE_TEXT_PT,
	RESULTTEXTYPE_TEXT_BYOU,
	RESULTTEXTYPE_TEXT_HITMOJI,
	RESULTTEXTYPE_TEXT_PT_MAX,
	RESULTTEXTYPE_GAMECLEAR_MAX,
}RESULTTEXTYPE_GAMECLEAR;

typedef enum
{
	RESULTTYPE_NONE = 0,
	RESULTTYPE_CLEAR,	// クリア状態
	RESULTTYPE_GAMEOVER,		// ゲームオーバー状態
	RESULTTYPE_MAX,
}RESULTTYPE;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResultType(RESULTTYPE type);
void SetResult(D3DXVECTOR3 pos, int nType,float fWidth,float fHeight);
RESULTTYPE GetResultType(void);
void GetScore(int nScore);
void GetTime(int nTime);
void GetHit(int nHit);
#endif // !_RESULT_H_

