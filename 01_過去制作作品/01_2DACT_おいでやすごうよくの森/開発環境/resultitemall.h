//=============================================================================
//
// リザルト画面のアイテム数字処理 [resultitemall.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _RESULTITEMALL_H_
#define _RESULTITEMALL_H_

#include "main.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	RESULTTYPE_NUMBER = 0, //数字
	RESULTTYPE_CHARACTER,
	RESULTTYPE_MAX
}RESULTTYPE;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitResultItemAll(void);
void UnInitResultItemAll(void);
void UpdateResultItemAll(void);
void DrawResultItemAll(void);

void SetResultItemAll(int nNumCoin, int nNumVodka, int nNumSatsu, int nNumArey);

//void SetResultItemAllpos(D3DXVECTOR3 pos);

#endif
