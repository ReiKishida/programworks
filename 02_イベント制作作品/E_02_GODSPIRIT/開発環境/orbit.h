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
#include "enemy.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_ORBIT	(50)
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
	ORBIT_ENEMY,
	ORBIT_MAX,
}ORBIT_PLAYER_ENEMY;
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff= NULL;	// 頂点バッファへのポインタ
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	D3DXMATRIX mtxStart;// ワールドマトリックス
	D3DXMATRIX mtxEnd;	// ワールドマトリックス
	ORBITTYPE nType;	// 種類
	D3DXVECTOR3 Start;	// 始点
	D3DXVECTOR3 End;	// 長さ
	D3DXVECTOR3 InitStart;	// 始点
	D3DXVECTOR3 InitEnd;	// 長さ
	PlayerMotionType nMotionType;
	ORBIT_PLAYER_ENEMY nCharacterType;
	PLAYERWEAPON nWeaponState;
	int nLength;		// 長さ
	int nPlus;			// 増やす量
	int nIdxParent;		// 親モデル
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
void SetOrbit(int nIdxParent, int nLength, D3DXVECTOR3 Start, D3DXVECTOR3 End, ORBITTYPE nType, PlayerMotionType nMotionType,PLAYERWEAPON nWeaponState);
void SetPosOrbit(PlayerMotionType nMotionType,PLAYERWEAPON nWeaponState);

#endif
