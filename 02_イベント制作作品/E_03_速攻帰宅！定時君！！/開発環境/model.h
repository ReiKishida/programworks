////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// モデル処理 [model.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_MODEL		(700)		// 最大数
#define MAX_MODEL_TYPE	(40)		// モデルのタイプの最大数
#define MAX_MODEL_COLLISION	(50)		// モデルの判定個数
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	MODELTYPE_BILL_1 = 0,
	MODELTYPE_BILL_2,
	MODELTYPE_BILL_3,
	MODELTYPE_GUARD,
	MODELTYPE_OFFICE,
	MODELTYPE_DRAM_1,
	MODELTYPE_DRAM_2,
	MODELTYPE_CAR,
	MODELTYPE_HYOUSIKI,
	MODELTYPE_HAUSE,
	MODELTYPE_STONE_4,
	MODELTYPE_STONE_5,
	MODELTYPE_POLE,
	MODELTYPE_JIMEN,
	MODELTYPE_ISH,
	MODELTYPE_TAIKO,
	MODELTYPE_CHOTIN2,
	MODELTYPE_TAKOYAKI_BALL,
	MODELTYPE_METEO,
	MODELTYPE_FISH,
	MODELTYPE_MAX,
}MODELTYPE;

typedef struct
{
	LPD3DXMESH pMesh;							// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;						// マテリアル情報へのポインタ
	DWORD nNumMat;								// マテリアル情報の数
	LPDIRECT3DTEXTURE9 *pTextuer;			// テクスチャ
	D3DXVECTOR3 CollisionPos[MAX_MODEL_COLLISION];
	float fWidthCollision[MAX_MODEL_COLLISION];
	float fDepthCollision[MAX_MODEL_COLLISION];
	float fHeightCollision[MAX_MODEL_COLLISION];
	float fRadiusCollision[MAX_MODEL_COLLISION];
	bool bCollison[MAX_MODEL_COLLISION];
	bool bCatch;
	bool bGetInfo;
	char *pFileName;
	int nNumModel;
}ModelData;
typedef struct
{
	LPD3DXBUFFER pBuffMat;				// マテリアル情報へのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 posOld;					// 前回の位置
	D3DXVECTOR3 Initpos;				// 初期位置
	D3DXVECTOR3 move;					// 移動量
	D3DXCOLOR col;						// カラー
	MODELTYPE nType;					// 種類
	D3DXVECTOR3 Diffrot;				// 角度の差分
	D3DXVECTOR3 Destrot;				// 目的の角度
	D3DXVECTOR3 rot;					// 角度
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	int nIdyShadow;						// 影の番号
	int nIdyLine[MAX_MODEL_COLLISION];	// 線の番号
	int nIdy;							// 番号
	bool bCollision;					// 当たり判定
	bool bUse;
	bool bDraw;
	bool bHit;
	bool bCatch;
	bool bFlag;
	int nCounter;
	float fCounter;
}Model;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
int CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 rot, float fRadius);
int CollisionModelGetInfo(D3DXVECTOR3 *pPos, float fRadiusWidth, float fRadiusHeight, Model **pModel);
int CollisionModelMinMax(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight, Model **pModel);
int SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE nType);
void SetCollisionModel(D3DXVECTOR3 pos, float fRadiusCollision,float fWidthCollision, float fDepthCollision,float fHeightCollision, MODELTYPE nType);
Model *GetModel(void);
ModelData *GetModelData(void);
#endif
