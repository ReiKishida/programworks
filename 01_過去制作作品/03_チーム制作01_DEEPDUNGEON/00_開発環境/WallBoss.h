//=============================================================================
//
// ポリゴン処理 [MeshFild.h]
// Author : 後閑茜
//
//=============================================================================
#ifndef WALLNOSS_H_
#define WALLNOSS_H_

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
	int VertexStage;			//バーテックス
	int IndexStage;				//インデックス
	int PolygonStage;			//ポリゴン数
	D3DXVECTOR3 aPos[4];		//ベクトル
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecC;

}WallBoss;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitWallBoss(void);
void UninitWallBoss(void);
void UpdateWallBoss(void);
void DrawWallBoss(void);

WallBoss*GetWallBoss(void);
void SetWallBoss(int Width, int Heidth, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void CollisionWallBoss_WIDTHBack_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWallBoss_WIDTHThisSide_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWallBoss_RIGHT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWallBoss_LEFT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定
void CollisionWallBoss_WIDTHBack_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWallBoss_WIDTHThisSide_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWallBoss_RIGHT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWallBoss_LEFT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定


#endif
