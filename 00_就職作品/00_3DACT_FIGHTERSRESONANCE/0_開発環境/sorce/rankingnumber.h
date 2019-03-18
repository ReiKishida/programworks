//=============================================================================
//
// ランキング数字処理[2D ポリゴン] [rankingnumber.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _RANKINGNUMBER_H_
#define _RANKINGNUMBER_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_NUMBER (8)
#define MAX_RANKING (5)

class CRankingScore : public CScene
{
public:

	CRankingScore();	//コンストラクタ
	~CRankingScore();	//デストラクタ
	static CRankingScore *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init();	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static void SetRankingScore(void);	//スコアの加算
	static void SetRankingNum(int nValue);	//スコアの加算
private:
	static CNumber *m_apNumber[MAX_RANKING][SCORE_NUMBER];	//数字
	static int m_nRanking[MAX_RANKING];	//ランキング
	D3DXVECTOR3 m_pos;		//位置情報
	static int m_nRankingPlayer;//プレイヤーのスコア
	static int m_nCntRanking;
	static bool m_bPlayer;//点滅用
};

#endif