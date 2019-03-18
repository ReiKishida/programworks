//=============================================================================
//
// アイテム処理 [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _DAMAGE_H_
#define _DAMAGE_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#define MAX_DAMAGE (4)
#define MAX_TYPEDAMAGE (4)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	DAMAGESTATE_NONE = 0,
	DAMAGESTATE_USE,
	DAMAGESTATE_MAX
}DAMAGE_STATE;
//state
typedef enum
{
	DAMAGESTATUS_NONE = 0,
	DAMAGESTATUS_NOW,
	DAMAGESTATUS_NEXT
}DAMAGE_STATUS;
//status

typedef enum
{
	DAMAGETYPE_0 = 0,
	DAMAGETYPE_1,
	DAMAGETYPE_2,
	DAMAGETYPE_3,
	DAMAGETYPE_MAX
}DAMAGETYPE;

typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//テクスチャへのポインタ
	LPD3DXMESH pMesh = NULL;		//メッシュ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMat;			//マテリアル情報の数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXCOLOR col;
	float fDestAngle;		//目的の角度
	float fAngle;			//差分
	DAMAGETYPE Type;
	DAMAGE_STATE state;
	int nType;
	bool bUse;
	float fpos;
	int nCntAngle;
	int nIdxModelParent = -1;
}DAMAGE_POLYGON;

typedef struct
{
	DWORD nNumMatPlayer = 0;	//マテリアルの情報の数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 move;	//向き
	D3DXVECTOR3 rot;	//向き
	D3DXCOLOR col;

	bool bUseJump;
	float fDestAngle;		//目的の角度
	float fAngle;			//差分
	int nIndexShadow;	//影の番号
	int nLife;
	bool bUse;
	int nType;
	DAMAGETYPE type;
	DAMAGE_STATUS status;
	int nIdxModelParent = -1;
	DAMAGE_STATE state;
	DAMAGE_POLYGON aPolygon[MAX_DAMAGE];
}DAMAGE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitDamage(void);		//アイテム初期化
void UninitDamage(void);	//アイテム終了
void UpdateDamage(void);	//アイテム更新
void DrawDamage(void);		//アイテム描画

void HitDamage(void);
//void SeItem(D3DXVECTOR3 pos, int nType);



#endif


