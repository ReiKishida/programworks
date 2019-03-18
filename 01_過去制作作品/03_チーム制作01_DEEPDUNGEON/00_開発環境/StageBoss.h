//=============================================================================
//
// ポリゴン処理 [MeshFild.h]
// Author : 後閑茜
//
//=============================================================================
#ifndef STAGEBOSS_H_
#define STAGEBOSS_H_

#include "main.h"
#include "Game.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	int VertexStage;			//バーテックス
	int IndexStage;				//インデックス
	int PolygonStage;			//ポリゴン数
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int Width;					//横
	int Heidth;					//縦
	bool bUse;					//使用状況

}StageBoss;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitStageBoss(void);
void UninitStageBoss(void);
void UpdateStageBoss(void);
void DrawStageBoss(void);

StageBoss*GetStageBoss(void);
void SetStageMapBoss(int Width, int Heidth, D3DXVECTOR3 pos);
#endif
