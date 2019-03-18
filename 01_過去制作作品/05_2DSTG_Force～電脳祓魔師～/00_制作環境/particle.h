
//=============================================================================
//
// パーティクルの処理[2D ポリゴン] [particle.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CParticle : public CScene2D
{
public:
	typedef enum
	{
		PARTICLETYPE_NONE = 0,	//空の種類
		PARITCLETYPE_HIT,		//被弾
		PARITCLETYPE_HIT_R,		//被弾
		PARITCLETYPE_EXPLOSION,	//爆発
		PARITCLETYPE_EXPLOSION_R,	//爆発
		PARITCLETYPE_MAX,		//パーティクルの最大数
	}PARTICLETYPE;//パーティクルの種類
	CParticle::CParticle();	//コンストラクタ
	CParticle::~CParticle();	//デストラクタ
	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nLife);	//弾の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄
private:
	int m_nLife;//ライフ
	float m_fRadius;//半径
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	D3DXVECTOR3 m_move;	//移動量
	PARTICLETYPE m_type;//パーティクルの種類
	D3DXCOLOR m_col;//色情報
};


#endif