////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 判定用ラインの処理 [line.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LINE_H_
#define _LINE_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	LINETYPE_PLAYER = 0,
	LINETYPE_ENEMY ,
	LINETYPE_MODEL ,
	LINETYPE_MAX
}LINETYPE;
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 前回の位置
	D3DXVECTOR3 posStart;	// 最初の位置
	D3DXVECTOR3 posEnd;	// 最後の位置
	D3DXCOLOR col;		// 色
	D3DXVECTOR3 move;	// 速さ
	D3DXVECTOR3 rot;	// 向き
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	LINETYPE nType;		// 種類
	bool bUse;			// 使用されているかどうか
}Line;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitLine(void);
void UninitLine(void);
void UpdateLine(void);
void DrawLine(void);
void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, D3DXCOLOR col, LINETYPE nType);
int CollisionLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMin,D3DXVECTOR3 vtxMax, LINETYPE nType);
void SetPositionLine(int nIdyLine, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void DeleteLine(LINETYPE nType);
#endif
