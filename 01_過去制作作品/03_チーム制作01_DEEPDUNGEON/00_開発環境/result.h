//=============================================================================
//
// 情報画面処理 [tuto.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	RESULTSTATE_NONE = 0,
	RESULTSTATE_USE,
	RESULTSTATE_MAX
}RESULTSTATE;

typedef enum
{
	RESULTPAGE_ONE = 0,
	RESULTPAGE_TWO,
	RESULTPAGE_THREE,
	RESULTPAGE_FOUR,
	RESULTPAGE_FIVE,
	RESULTPAGE_MAX
}RESULTPAGE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//カラ－
	int nType;			//種類
	RESULTSTATE state;
	RESULTPAGE page;
	bool bUse;
}RESULT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);


#endif


