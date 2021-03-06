//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _CHARGEBAR_H_
#define _CHARGEBAR_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_CHARGEBAR	(128)	// ポーズの最大数



//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;
	bool bUse;			//使用しているかどうか
	int nType;
}CHARGEBAR;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitChargebar(void);
void UnInitChargebar(void);
void UpdateChargebar(void);
void DrawChargebar(void);


 




//void SetPause(D3DXVECTOR3 pos, int nType, float fSpeed);

#endif