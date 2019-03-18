//=============================================================================
//
// ポリゴン処理 [MeshFild.h]
// Author : 後閑茜
//
//=============================================================================
#ifndef STAGE2_H_
#define STAGE2_H_

#include "main.h"
#include "Game.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int Width;					//横
	int Heidth;					//縦
	bool bUse;					//使用状況

}Stage2;
typedef struct
{
	int VertexStage;			//バーテックス
	int IndexStage;				//インデックス
	int PolygonStage;			//ポリゴン数

}StageVtx2;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitStage2(void);
void UninitStage2(void);
void UpdateStage2(void);
void DrawStage2(void);

Stage2*GetStage2(void);
void SetStageMap2(int Width, int Heidth, D3DXVECTOR3 pos);
#endif
