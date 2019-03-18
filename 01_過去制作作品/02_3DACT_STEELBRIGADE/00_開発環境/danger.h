//=============================================================================
//
// アイテム処理 [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _DANGER_H_
#define _DANGER_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#define MAX_DANGER (4)
#define MAX_TYPEDANGER (4)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	DANGERSTATE_NONE = 0,
	DANGERSTATE_USE,
	DANGERSTATE_MAX
}DANGER_STATE;
//state
typedef enum
{
	DANGERSTATUS_NONE = 0,
	DANGERSTATUS_NOW,
	DANGERSTATUS_NEXT
}DANGER_STATUS;
//status

typedef enum
{
	DANGERETYPE_0 = 0,
	DANGERTYPE_1,
	DANGERTYPE_2,
	DANGERTYPE_3,
	DANGERTYPE_MAX
}DANGERTYPE;

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
	DANGER_STATE state;
	DANGERTYPE Type;
	int nType;
	bool bUse;
	float fpos;
	int nCntAngle;
	int nIdxModelParent = -1;
}DANGER_POLYGON;

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
	DANGERTYPE type;
	DANGER_STATUS status;
	int nIdxModelParent = -1;
	DANGER_STATE state;
	DANGER_POLYGON aPolygon[MAX_DANGER];
}DANGER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitDanger(void);		//アイテム初期化
void UninitDanger(void);	//アイテム終了
void UpdateDanger(void);	//アイテム更新
void DrawDanger(void);		//アイテム描画

void HitDanger(void);
//void SeItem(D3DXVECTOR3 pos, int nType);



#endif


