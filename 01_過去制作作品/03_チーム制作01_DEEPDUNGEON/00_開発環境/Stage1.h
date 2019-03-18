//=============================================================================
//
// ポリゴン処理 [MeshFild.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef STAGE1_H_
#define STAGE1_H_

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

}Stage1;
typedef struct
{
	int VertexStage;			//バーテックス
	int IndexStage;				//インデックス
	int PolygonStage;			//ポリゴン数

}StageVtx1;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitStage1(void);
void UninitStage1(void);
void UpdateStage1(void);
void DrawStage1(void);

Stage1*GetStage1(void);
void SetStageMap1(int Width, int Heidth, D3DXVECTOR3 pos);
#endif
