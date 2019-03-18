//=============================================================================
//
// 情報画面処理 [tips.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _WORLD_H_
#define _WORLD_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	WORLD_NORMAL = 0,
	WORLD_READING,
	WORLD_DECIDE,
	WORLD_MAX
}WORLD_STATE;

typedef struct
{
	bool bUpdate;
}WORLDSTATE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//カラ－
	int nType;			//種類
	WORLD_STATE state;
}WORLD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitWorld(void);
void UnInitWorld(void);
void UpdateWorld(void);
void DrawWorld(void);

#endif


