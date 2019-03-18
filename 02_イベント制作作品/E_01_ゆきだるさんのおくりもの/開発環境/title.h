//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	TITLESTATE_NONE = 0,
	TITLESTATE_USE,
	TITLESTATE_MAX
}TITLE_STATE;


typedef enum
{
	TITLETYPE_NONE = 0,
	TITLETYPE_NOW,
	TITLETYPE_NEXT
}TITLE_TYPE;


typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	TITLE_TYPE TitleType;
	int nType;	
	float fAngle;
	float fLength;
	float fHeight;
	float fWidth;
	bool bUse;
	TITLE_STATE state;//種類
}TITLE;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);


#endif


