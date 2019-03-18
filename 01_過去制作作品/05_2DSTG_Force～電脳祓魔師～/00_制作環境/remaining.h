//=============================================================================
//
// 残機処理[2D ポリゴン] [remaining.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _REMAINING_H_
#define _REMAINING_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define REMAINING_NUMBER (2)

class CRemaining : public CScene
{
public:
	CRemaining();	//コンストラクタ
	~CRemaining();	//デストラクタ
	static CRemaining *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetScore(int nScore);	//スコアの設定
	static int GetRemaining(void);	//スコアの取得
	static void DecreseRemaining(int nValue);	//スコアの加算
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	static CScene2D *m_apRemaining[REMAINING_NUMBER];	//数字
	static int m_nRemaining;	//残機数
};

#endif