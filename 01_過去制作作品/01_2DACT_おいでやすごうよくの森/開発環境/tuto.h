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
	PASE_ONE = 0,
	PASE_TWO,
	PASE_THREE,
	PASE_FOUR,
	PASE_FIVE,
	PASE_MAX
}PASE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//カラ－
	int nType;			//種類
	TUTO_STATE state;
	PASE pase;
	bool bUse;
}TUTO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTuto(void);
void UnInitTuto(void);
void UpdateTuto(void);
void DrawTuto(void);


#endif


