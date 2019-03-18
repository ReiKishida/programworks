//=============================================================================
//
// ランキング用ポリゴン処理[2D ポリゴン] [rankinglogo.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _RANKINGLOGO_H_
#define _RANKINGLOGO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING_LOGO_TEXTURE (5)
#define MAX_RANKING_LOGO (5)

class CManager;

class CRankingLogo : public CScene
{
public:

	CRankingLogo();	//コンストラクタ
	~CRankingLogo();	//デストラクタ
	static CRankingLogo *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RANKING_LOGO_TEXTURE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_RANKING_LOGO];	//数字

};

#endif