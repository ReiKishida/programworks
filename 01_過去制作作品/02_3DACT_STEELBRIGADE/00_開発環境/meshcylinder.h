//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.h]
// Author : 
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"


//*****************************************************************************
// メッシュシリンダーの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;
	int nType;
}CYLINDER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshCylinder(void);		//メッシュシリンダー初期化
void UninitMeshCylinder(void);	//メッシュシリンダー終了
void UpdateMeshCylinder(void);	//メッシュシリンダー更新
void DrawMeshCylinder(void);		//メッシュシリンダー描画




void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

#endif
