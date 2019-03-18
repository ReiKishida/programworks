//=============================================================================
//
// ランキング処理[2D ポリゴン] [ranking.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MENURANKING_H_
#define _MENURANKING_H_

#include "main.h"

class CMenuRanking
{
public:
	CMenuRanking();	//コンストラクタ
	~CMenuRanking();	//デストラクタ
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
private:


};

#endif