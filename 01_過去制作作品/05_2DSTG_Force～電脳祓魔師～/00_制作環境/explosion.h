//=============================================================================
//
// 爆発処理[2D ポリゴン] [explosion.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

class CManager;

class CExplosion : public CScene2D
{
public:
	CExplosion::CExplosion();	//コンストラクタ
	CExplosion::~CExplosion();	//デストラクタ
	static CExplosion *Create(D3DXVECTOR3 pos, float fRadius);	//プレイヤーの生成
	HRESULT Init(D3DXVECTOR3 pos, float fRadius);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	int m_nCounterAnim;	//アニメーションカウンター
	int m_nPatternAnim;	//アニメーションパターン
};

#endif