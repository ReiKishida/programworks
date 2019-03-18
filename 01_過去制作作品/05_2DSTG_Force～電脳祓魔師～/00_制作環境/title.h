//=============================================================================
//
// タイトル処理[2D ポリゴン] [title.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
class CTitleLogo;

class CTitle
{
public:
	CTitle();	//コンストラクタ
	~CTitle();	//デストラクタ
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
private:
	static CManager *m_pManager;//マネージャ
	static CScene *m_pScene;//シーン
	static CTitle *m_pTitle;//タイトル
	static CBg *m_pBg;//背景
	static CTitleLogo *m_pTitleLogo;//タイトルロゴ
	static int m_nCntFlame;

};

#endif