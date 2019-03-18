//=============================================================================
//
// フォース数処理[2D ポリゴン] [forcenumber.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _FORCENUMBER_H_
#define _FORCENUMBER_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FORCE_NUMBER (4)

class CForceNumber : public CScene
{
public:
	CForceNumber();	//コンストラクタ
	~CForceNumber();	//デストラクタ
	static CForceNumber *Create(D3DXVECTOR3 pos);	//数字の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetForce(int nScore);	//フォース数の設定
	int GetForce(void);	//フォース数の取得
	static void AddForce(int nValue);	//フォース数の加算
private:
	static CNumber *m_apNumber[FORCE_NUMBER];	//数字
	static int m_nForce;	//フォース数
};

#endif