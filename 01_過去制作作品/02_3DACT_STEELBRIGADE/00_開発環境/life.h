//=============================================================================
//
// ライフ処理 [life.h]
// Author : 
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//****************************************************************************
#define MAX_LIFE (128)
//*****************************************************************************
// 構造体宣言
//****************************************************************************

typedef enum
{
	LIFESTATE_NONE = 0,
	LIFESTATE_USE,
	LIFESTATE_MAX
}LIFE_STATE;
typedef enum
{
	LIFETYPE_NONE = 0,
	LIFETYPE_NOW,
	LIFETYPE_NEXT
}LIFE_TYPE;



typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 ParentPos;
	D3DXVECTOR3 ParentRot;
	D3DXCOLOR col;
	LIFE_STATE state;
	bool bUse;
}LIFE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLife(void);		//ライフ初期化
void UninitLife(void);	//ライフ終了
void UpdateLife(void);	//ライフ更新
void DrawLife(void);		//ライフ描画

void SetLife(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Parentpos, D3DXVECTOR3 Parentrot, D3DXCOLOR col);
void HitDamage(int nDamage);

#endif
