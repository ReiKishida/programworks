//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : komatsu keisuke
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//=============================================================================
// メッシュフィールドの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//方向
	D3DXMATRIX mtxWorldMeshField;	//ワールドマトリックス
	int nNumIndexMeshField;			//インデックス数
	int nNumPolygonMeshField;		//ポリゴン数
	float fDepth;					//奥行き
	float fWidth;					//幅
}MeshField;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif
