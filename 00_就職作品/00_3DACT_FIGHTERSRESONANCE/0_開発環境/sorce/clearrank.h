//=============================================================================
//
// 背景処理[2D ポリゴン] [bg.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _CLEARRANK_H_
#define _CLEARRANK_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_RANK (5)	//テクスチャ数

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CClearRank : public CScene
{
public:
	typedef enum
	{
		RANKMODE_SCORE = 0,	//スコア
		RANKMODE_TIME,		//タイム
		RANKMODE_COMBO,		//コンボ
		RANKMODE_DAMAGE,	//ダメージ
		RANKMODE_FINAL,		//最終スコア
		RANKMODE_MAX
	}RANKMODE;
	typedef enum
	{
		RANKTYPE_S = 0,	//S
		RANKTYPE_A,		//A
		RANKTYPE_B,		//B
		RANKTYPE_C,		//C
		RANKTYPE_D,		//D
		RANKTYPE_MAX		//コンボの最大数
	}RANKTYPE;//UIの種類

	CClearRank(int nPriority = 7, OBJTYPE type = OBJTYPE_COMBO);	//コンストラクタ
	~CClearRank();	//デストラクタ
	static CClearRank *Create(D3DXVECTOR3 pos,RANKMODE mode);	//背景の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_RANK]; //共有テクスチャのポインタ
	D3DXCOLOR m_Color;
	D3DXVECTOR3 m_pos;
	int m_nCntColor;	//カラーカウント用
	CScene2D *m_pScene2D;
	RANKTYPE m_Type;	//UIのタイプ
	RANKMODE m_mode;	//モード
	float m_fCombo;		//コンボ量
	int m_nFinalScore;
};

#endif