//=============================================================================
//
// ポリゴン処理 [MeshFild.h]
// Author : 後閑茜
//
//=============================================================================
#ifndef WALL1_H_
#define WALL1_H_

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
}Wall1;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitWall1(void);
void UninitWall1(void);
void UpdateWall1(void);
void DrawWall1(void);

Wall1*GetWall1(void);
void SetWall1(int Width, int Heidth, D3DXVECTOR3 pos, D3DXVECTOR3 rot);


void CollisionWall1_WIDTHBack_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定

void CollisionWall1_WIDTHBack_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定

void CollisionWall1_WIDTHBack_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定

void CollisionWall1_WIDTHBack_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定

void CollisionWall1_WIDTHBack_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定


void CollisionWall1_WIDTHBack_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定


void CollisionWall1_WIDTHBack_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定


void CollisionWall1_WIDTHBack_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定



void CollisionWall1_WIDTHBack_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定


void CollisionWall1_WIDTHBack_Arrow(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//奥面あたり判定
void CollisionWall1_WIDTHThisSide_Arrow(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//手前壁あたり判定
void CollisionWall1_RIGHT_Arrow(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//右壁あたり判定
void CollisionWall1_LEFT_Arrow(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld);//左壁あたり判定


#endif
