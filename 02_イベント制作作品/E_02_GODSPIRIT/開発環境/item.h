//=============================================================================
//
// アイテムの処理 [item.h]
// Author : 佐藤亮太
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "model.h"
#include "meshField.h"
//*****************************************************************************
// アイテムの構造体
//*****************************************************************************
typedef enum
{// アイテムの種類
	ITEM_KOUTOKU = 0,			// 幸徳
	MAX_ITEM
}ItemType;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posOld;		// 前回の位置
	D3DXVECTOR3 InitPos;	// 初期位置
	D3DXVECTOR3 move;		// 移動量
	D3DXMATRIX	mtxWorld;	// ワールドマトリクス
	float fJampPower;		// ジャンプ力
	float fPosLenght;		// プレイヤーとの距離
	__int8 nPattanAnime;	// アニメーションのパターン
	float fMaxPattanAnime;	// アニメーションのパターンの最大数
	int nCounterAnime;		// アニメーションのカウンター
	int nIdyShadow;			// 使っている壁の番号
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bUse;				// 使用しているかどうか
	ItemType ItemType;		// アイテムの種類
	Model				*pModel;									// モデルの情報
	MeshField			*pMeshField;								// メッシュフィールド情報

}Item;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ItemType type);	// アイテムの設置
void CollisionItem(D3DXVECTOR3 mtxWorld, float fRadius);		// アイテムの当たり判定
void DeleteItem(int nCntItem, ItemType type);					// アイテムの効果
#endif
