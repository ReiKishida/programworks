//=============================================================================
//
// モデル処理 [model.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//回転
	D3DXCOLOR col;				//色
	D3DXVECTOR3 vtxMin;			//モデルの最小値
	D3DXVECTOR3 vtxMax;			//モデルの最大値
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int nIdxShadow;				//影番号
	bool bUse;					//使用状況
}Model;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

Model*GetModel(void);
bool CollisionModel(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);
#endif
