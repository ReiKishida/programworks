//=============================================================================
//
// ハイスコア処理[2D ポリゴン] [highscore.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HIGHSCORE_NUMBER (8)

class CHighScore : public CScene
{
public:
	CHighScore();	//コンストラクタ
	~CHighScore();	//デストラクタ
	static CHighScore *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetScore(int nScore);	//スコアの設定
	int GetScore(void);	//スコアの取得
	static void AddScore(int nValue);	//スコアの加算
	static void SetHighScore(int nValue);//スコアの設定
private:
	static CNumber *m_apNumber[HIGHSCORE_NUMBER];	//数字
	static int m_nHighScore;	//スコア
};

#endif