////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 上向きエフェクト処理 [effectUP.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECTUP_H_
#define _EFFECTUP_H_

#include "main.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{
	EFFECTUPTYPE_LEAF = 0,
	EFFECTUPTYPE_PAPER,
	EFFECTUPTYPE_GUARD,
	EFFECTUPTYPE_MAX
}EFFECTUPTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXCOLOR col;	// 色
	D3DXVECTOR3 rot;	// 向き
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	float fRadius;		// 幅
	EFFECTUPTYPE nType;	// 種類
	int nIdy;			// 番号
	bool bUse;			// 使用されているかどうか
	float fAttackPower; // 攻撃力
	float fDamageH;		// 横吹っ飛び
	float fDamageV;		// 縦吹っ飛び

}EffectUP;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitEffectUP(void);
void UninitEffectUP(void);
void UpdateEffectUP(void);
void DrawEffectUP(void);
void SetEffectUP(int nIdy,D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, EFFECTUPTYPE nType, float fRadius);
void SetEffectUPDamage(int nIdy, float fAttackPower, float fDamageH, float fDamageV, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, EFFECTUPTYPE nType, float fRadius);

#endif
