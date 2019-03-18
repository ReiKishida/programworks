//***************************************************************************//
//
// 死亡時処理 [died.h]
// Author : KishidaRei
//
//***************************************************************************//
#ifndef _DIED_H_
#define _DIED_H_

#include "main.h"
//***************************************************************************//
// 構造体宣言
//***************************************************************************//
typedef enum
{
	DIEDTYPE_APPER = 0,
	DIEDTYPE_NORMAL,
	DIEDTYPE_MAX
}DIEDTYPE;

typedef struct
{

	D3DXVECTOR3 pos;								// 位置
	D3DXVECTOR3 rot;								// 移動
	D3DXCOLOR Col;								//色
	bool bUse;									//使用しているか
	DIEDTYPE type;								//タイプ				
}DIED;
//***************************************************************************//
// プロトタイプ宣言
//***************************************************************************//
void InitDied(void);
void UninitDied(void);
void UpdateDied(void);
void DrawDied(void);
DIED *GetDied(void);

#endif