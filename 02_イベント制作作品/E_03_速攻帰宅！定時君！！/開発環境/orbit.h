////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 軌道の処理 [orbit.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "main.h"
#include "player.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_ORBIT	(1000)
#define MAX_ORBIT_LENGTH	(100)
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	ORBITTYPE_0 = 0,
	ORBITTYPE_1,
	ORBITTYPE_2,
	ORBITTYPE_PLAYER,
	ORBITTYPE_MAX,
}ORBITTYPE;
typedef enum
{
	ORBIT_PLAYER = 0,
	ORBIT_MAX,
}ORBIT_PLAYER_ENEMY;
typedef struct
{
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	D3DXMATRIX mtxStart;// ワールドマトリックス
	D3DXMATRIX mtxEnd;	// ワールドマトリックス
	D3DXMATRIX mtxStartOld;// ワールドマトリックス
	D3DXMATRIX mtxEndOld;	// ワールドマトリックス
	D3DXMATRIX playerWorld;// プレイヤー
	D3DXMATRIX playerWorldOld;// プレイヤー
	ORBITTYPE nType;	// 種類
	D3DXCOLOR col;		// 色
	D3DXVECTOR3 Start;	// 始点
	D3DXVECTOR3 End;	// 長さ
	D3DXVECTOR3 InitStart;	// 始点
	D3DXVECTOR3 InitEnd;	// 長さ
	PlayerMotionType nMotionType;
	int nLength;		// 長さ
	int nCntTrueTime;	// 表示時間
	bool bUse;			// 使用されているかどうか
}Orbit;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitOrbit(void);
void UninitOrbit(void);
void UpdateOrbit(void);
void DrawOrbit(void);
void SetOrbit(D3DXMATRIX nIdxmtx, D3DXMATRIX nIdxmtxOld, int nLength, D3DXVECTOR3 Start, D3DXVECTOR3 End, ORBITTYPE nType, PlayerMotionType nMotionType);
void SetPosOrbit(PlayerMotionType nMotionType);

#endif
