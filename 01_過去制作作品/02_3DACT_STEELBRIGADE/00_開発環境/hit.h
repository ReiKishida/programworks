//=============================================================================
//
// アイテム処理 [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#define MAX_ITEM (4)
#define MAX_TYPEITEM (4)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	HITSTATE_NONE = 0,
	HITSTATE_USE,
	HITSTATE_MAX
}HIT_STATE;
//state
typedef enum
{
	HITSTATUS_NONE = 0,
	HITSTATUS_NOW,
	HITSTATUS_NEXT
}HIT_STATUS;
//status

typedef enum
{
	ITEMTYPE_0 = 0,
	ITEMTYPE_1,
	ITEMTYPE_2,
	ITEMTYPE_3,
	ITEMTYPE_MAX
}ITEMTYPE;

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
	ITEMTYPE Type;
	HIT_STATE state;
	int nType;
	bool bUse;
	float fpos;
	int nCntAngle;
	int nIdxModelParent = -1;
}ITEM_POLYGON;

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
	ITEMTYPE type;
	HIT_STATUS status;
	int nIdxModelParent = -1;
	HIT_STATE state;
	ITEM_POLYGON aPolygon[MAX_ITEM];
}ITEM;

typedef struct
{
	D3DXVECTOR3 pos;	//位置

	D3DXVECTOR3 rot;	//向き

	float fDestAngle;		//目的の角度
	float fAngle;			//差分

}POLYGONROT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitItem(void);		//アイテム初期化
void UninitItem(void);	//アイテム終了
void UpdateItem(void);	//アイテム更新
void DrawItem(void);		//アイテム描画

void HitCall(void);
//void SeItem(D3DXVECTOR3 pos, int nType);



#endif


