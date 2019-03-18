//=============================================================================
//
// 吹き出し処理 [callout.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _CALLOUT_H_
#define _CALLOUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	CALLOUTTYPE_NONE = 0,
	CALLOUTTYPE_CHANGE,
	CALLOUTTYPE_PRESENT1,
	CALLOUTTYPE_PRESENT2,
	CALLOUTTYPE_PRESENT3,
	CALLOUTTYPE_PRESENT4,
	CALLOUTTYPE_PRESENT5
}CALLOUTTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	CALLOUTTYPE type;
	int nType;
	bool bUse;
}CALLOUT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCallout(void);
void UninitCallout(void);
void UpdateCallout(void);
void DrawCallout(void);
int SetCallout(D3DXVECTOR3 pos, CALLOUTTYPE type);

void DeleteCallout(int nIdxCallout);

#endif
