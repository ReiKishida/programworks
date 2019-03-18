
//=============================================================================
//
// パーティクルの処理[2D ポリゴン] [particle.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CParticle2D : public CScene2D
{
public:
	typedef enum
	{
		PARTICLETYPE_NONE = 0,	//空の種類
		PARITCLETYPE_FLAME,		//炎
		PARITCLETYPE_MAX,		//パーティクルの最大数
	}PARTICLETYPE;//パーティクルの種類

	CParticle2D(int nPriority = 7, OBJTYPE type = OBJTYPE_PARTICLE);		//コンストラクタ
	~CParticle2D();	//デストラクタ
	static CParticle2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nLife);	//弾の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄
private:
	int m_nLife;//ライフ
	float m_fRadius;//半径
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	D3DXVECTOR3 m_pos;	//位置情報
	D3DXVECTOR3 m_move;	//移動量
	PARTICLETYPE m_type;//パーティクルの種類
	D3DXCOLOR m_col;	//色情報
};


#endif