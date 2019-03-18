//=============================================================================
//
// 情報画面処理 [tuto.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _PROLOGUE_H_
#define _PROLOGUE_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PROLOGUESTATE_NONE = 0,
	PROLOGUESTATE_USE,
	PROLOGUESTATE_MAX
}PROLOGUESTATESTATE;

typedef enum
{
	PROLOGUESPAGE_ONE = 0,
	PROLOGUESPAGE_TWO,
	PROLOGUESPAGE_THREE,
	PROLOGUESPAGE_FOUR,
	PROLOGUESPAGE_FIVE,
	PROLOGUESPAGE_MAX
}PROLOGUESPAGE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//カラ－
	int nType;			//種類
	PROLOGUESTATESTATE state;
	PROLOGUESPAGE page;
	bool bUse;
}PROLOGUE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPrologue(void);
void UninitPrologue(void);
void UpdatePrologue(void);
void DrawPrologue(void);


#endif


