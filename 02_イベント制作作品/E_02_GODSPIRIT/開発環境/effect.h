////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクトの処理 [effect.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"


//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	EFFECTTYPE_EXPLOSION = 0,	// 爆発
	EFFECTTYPE_PLAYER,			// 煙
	EFFECTTYPE_WATER,			// 水
	EFFECTTYPE_DRAIN,			// 雪
	EFFECTTYPE_AIM,				// 注目
	EFFECTTYPE_DEATH,			// 敵撃破
	EFFECTTYPE_WATERFALL,		// 滝水
	EFFECTTYPE_IMPACT,			// 衝撃は
	EFFECTTYPE_SMOKE,			// 煙
	EFFECTTYPE_SMOKEDOWN,		// 煙下がる
	EFFECTTYPE_HIT,				// 亀裂
	EFFECTTYPE_SUNDER,			// マヒ
	EFFECTTYPE_SUNDER1,			// マヒ
	EFFECTTYPE_POISON,			// 毒
	EFFECTTYPE_POISONSMOKE,		// 毒
	EFFECTTYPE_HITEXPLOSION,	// ヒット
	EFFECTTYPE_FIRE,			// 火
	EFFECTTYPE_WIND,			// 風圧
	EFFECTTYPE_FLOZE,			// 氷
	EFFECTTYPE_SUNDER_DOWN,			// マヒ
	EFFECTTYPE_WATER_DOWN,			// マヒ

	EFFECTTYPE_MAX
}EFFECTTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 Destpos;// 目的の位置
	D3DXVECTOR3 posOld;	// 前回の位置
	D3DXVECTOR3 Initpos;// 初期位置
	D3DXVECTOR3 move;	// 速さ
	D3DXVECTOR3 rot;	// 向き
	float fAngle;		// 角度
	float fLength;		// 長さ
	D3DXCOLOR col;		// 色
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	float fWidth;		// 幅
	float fHeight;		// 高さ
	float fDepth;		// 奥行き
	float fRadius;		// 長さ
	int nLife;			// 寿命
	float fSinCosMove;	// ゆれ
	EFFECTTYPE nType;// 種類
	int nIdyShadow;		// 影のID
	float fChangeRadius;// 拡大縮小
	int nCounterAnim;	// アニメーションカウンター
	int nPatternAnim;	// アニメーションパターン
	int nSpeedCounter;	// カウンター
	bool bUse;			// 使用されているかどうか
}Effect;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE nType);
float SetRandomEffect(void);
Effect *GetEffect(void);

#endif
