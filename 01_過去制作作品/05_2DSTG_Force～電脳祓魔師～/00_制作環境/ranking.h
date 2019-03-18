//=============================================================================
//
// ランキング処理[2D ポリゴン] [ranking.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

class CRanking
{
public:
	CRanking();	//コンストラクタ
	~CRanking();	//デストラクタ
				//static CRanking *Create();	//リザルトの生成
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
private:
	static int m_nCntFlame;

	//static CManager *m_pManager;//マネージャ

	//static CRanking *m_pResult;

};

#endif