
//=============================================================================
//
// エフェクトの処理[2D ポリゴン] [effect.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EFFECTTYPE (3)

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CEffect : public CScene2D
{
public:
	typedef enum
	{
		EFFECTTYPE_BULLET = 0,		//弾のエフェクト
		EFFECTTYPE_PLAYER,		//プレイヤーのエフェクト
		EFFECTTYPE_BOSS,		//ボスのエフェクト
		EFFECTTYPE_MAX			//エフェクトの最大数
	}EFFECTTYPE;
	CEffect::CEffect();	//コンストラクタ
	CEffect::~CEffect();	//デストラクタ
	static CEffect *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight , int nLife, EFFECTTYPE type);	//弾の生成
	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);	//テクスチャのロード
	static void Unload(void);	//テクスチャの破棄
private:
	int m_nLife;
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_EFFECTTYPE]; //共有テクスチャのポインタ
	EFFECTTYPE m_type;	//エフェクトの種類
};

#endif