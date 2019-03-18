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
#define MAX_MODEL		(350)		// 最大数
#define MAX_MODEL_TYPE	(40)		// モデルのタイプの最大数
#define MAX_MODEL_COLLISION	(50)		// モデルの判定個数

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	MODELTYPE_BOOK = 0,
	MODELTYPE_BOOKSHELF,
	MODELTYPE_CANDLE,
	MODELTYPE_CHAIR,
	MODELTYPE_HANDRAIL,
	MODELTYPE_LARGEDOOR1,
	MODELTYPE_LARGEDOOR2,
	MODELTYPE_PLANT,
	MODELTYPE_SMALLDOOR,
	MODELTYPE_STONE,
	MODELTYPE_TABLE,
	MODELTYPE_WINDOW,
	MODELTYPE_CAP,
	MODELTYPE_GAISOU,
	MODELTYPE_GAITOU,
	MODELTYPE_HASHIRA1,
	MODELTYPE_HASHIRA2,
	MODELTYPE_HUNSUI,
	MODELTYPE_HURUDOKEI,
	MODELTYPE_KAIGA,
	MODELTYPE_KANSETUSYOUMEI,
	MODELTYPE_DISH,
	MODELTYPE_SHANDERIA,
	MODELTYPE_TUKUE,
	MODELTYPE_TUBO,
	MODELTYPE_TREE0,
	MODELTYPE_TREE1,
	MODELTYPE_SMALLDOOR_YOKO,
	MODELTYPE_YOKANDOOR1,
	MODELTYPE_YOKANDOOR2,
	MODELTYPE_ARMOR_OBJ,
	MODELTYPE_BOOK_V,
	MODELTYPE_HASIRA3,
	MODELTYPE_SHANDERIA_BIG,
	MODELTYPE_HANDRAIL_H,
	MODELTYPE_BOSSSAKU,
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
	int nIdyLine[MAX_MODEL_COLLISION];							// 線の番号
	int nIdy;						// 番号
	bool bCollision;					// 当たり判定
	bool bUse;
	bool bDraw;
}Model;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
int CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 rot, float fRadius);
int CollisionModelMinMax(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight,Model **pModel);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE nType);	
void SetCollisionModel(D3DXVECTOR3 pos, float fRadiusCollision,float fWidthCollision, float fDepthCollision,float fHeightCollision, MODELTYPE nType);
Model *GetModel(void);
ModelData *GetModelData(void);
#endif
