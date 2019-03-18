//=============================================================================
//
// 情報画面処理 [tuto.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _TUTO_H_
#define _TUTO_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	TUTOSTATE_NONE = 0,
	TUTOSTATE_USE,
	TUTOSTATE_MAX
}TUTO_STATE;

typedef enum
{
	Page_ONE = 0,
	Page_TWO,
	Page_THREE,
	Page_FOUR,
	Page_FIVE,
	Page_MAX
}Page;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//カラ－
	int nType;			//種類
	TUTO_STATE state;
	Page Page;
	bool bUse;
}TUTO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTuto(void);
void UninitTuto(void);
void UpdateTuto(void);
void DrawTuto(void);


#endif


