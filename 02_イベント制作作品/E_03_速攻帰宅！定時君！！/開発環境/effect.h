////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクトの処理 [effect.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "model.h"


//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	EFFECTTYPE_SMOKE = 0,		// 煙
	EFFECTTYPE_IMPACT,			// 衝撃は
	EFFECTTYPE_HITEXPLOSION,	// ヒット
	EFFECTTYPE_WATERFALL_0,		// 滝
	EFFECTTYPE_WATERFALL_1,		// 滝１
	EFFECTTYPE_STATE_UP,		// ステータスアップ
	EFFECTTYPE_FIRE,			// 炎
	EFFECTTYPE_HIBANA,			// 火花
	EFFECTTYPE_WATER,			// 水
	EFFECTTYPE_WALK,		// 歩く
	EFFECTTYPE_LASER,		// レーザー
	EFFECTTYPE_BOMB,		// ボム
	EFFECTTYPE_DIRTY,		// 土
	EFFECTTYPE_SUNDER,		// 雷
	EFFECTTYPE_CRASH,		// 混乱
	EFFECTTYPE_CARTOON_SLICE_0,		// ザッ
	EFFECTTYPE_CARTOON_FIRE,		// 炎
	EFFECTTYPE_CARTOON_WIND,		// 風
	EFFECTTYPE_CARTOON_DON,		// ドン
	EFFECTTYPE_CARTOON_TA,		// たっ
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
	int nRandAnim;	// ランダムカウンター
	bool bUse;			// 使用されているかどうか
	int nIdy;			// 番号
	float fAttackPower; // 攻撃力
	float fDamageH;		// 横吹っ飛び
	float fDamageV;		// 縦吹っ飛び
	Model *pModel;
}Effect;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE nType);
void SetEffectDamage(int nIdy, float fAttackPower, float fDamageH, float fDamageV,D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE nType);
float SetRandomEffect(void);
Effect *GetEffect(void);

void HitEffect(D3DXVECTOR3 pos);
#endif
