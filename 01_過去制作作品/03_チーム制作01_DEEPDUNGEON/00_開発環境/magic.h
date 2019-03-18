//=============================================================================
//
// 魔法攻撃処理 [magic.h]
// Author : komatsu keisuke
//
//=============================================================================
#ifndef _MAGIC_H_
#define _MAGIC_H_

#include "main.h"

//=============================================================================
// 魔法の構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//位置
	D3DXVECTOR3 rot;			//方向
	D3DXVECTOR3 move;			//移動量
	D3DXMATRIX mtxWorldMagic;	//ワールドマトリックス
	int nLife;					//消滅までの時間
	int nIdxShadow;				//影番号
	bool bUse;					//使用状況
	int nType;
}BULLET;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);
void SetMagic(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move , int nLife, int nType);
void Set_L_Magic(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nLife , int nType);

#endif
