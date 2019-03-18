//=============================================================================
//
// スコア処理[2D ポリゴン] [score.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _COMBOSCORE_H_
#define _COMBOSCORE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COMBO_SCORE_NUMBER (4)

class CNumber;
class CComboScore : public CScene
{
public:

	CComboScore(int nPriority = 7, OBJTYPE type = OBJTYPE_SCORE);	//コンストラクタ
	~CComboScore();	//デストラクタ
	static CComboScore *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init();	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetScore(int nScore);	//スコアの設定
	static int GetScore(void);	//スコアの取得
	static void AddScoreFix();			//目的のスコアへ加算
	static void SetScoreNum(int nValue);	//スコアの加算
private:
	static CNumber *m_apNumber[COMBO_SCORE_NUMBER];	//数字
	static int m_nScore;		//計算用スコア
	static int m_nDestScore;	//目的のスコア
	static int m_nNowScore;		//現在のスコア
	static D3DXVECTOR3 m_pos;	//位置情報
};

#endif