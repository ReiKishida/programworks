//=============================================================================
//
// 背景処理[2D ポリゴン] [bg.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_UI (12)	//テクスチャ数

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CUi : public CScene
{
public:
	typedef enum
	{
		UI_TYPE_BG_L = 0,		//下地(L)
		UI_TYPE_BG_R,		//下地(R)
		UI_TYPE_HIGHSCORE,	//ハイスコア
		UI_TYPE_SCORE,		//スコア
		UI_TYPE_ATK,		//攻のフォース数
		UI_TYPE_SPR,		//拡のフォース数
		UI_TYPE_SPD,		//速のフォース数
		UI_TYPE_DEF,		//護のフォース数
		UI_TYPE_TIME,		//タイムの文字
		UI_TYPE_COLON,		//コロン
		UI_TYPE_DOT,		//ドット
		UI_TYPE_LOG,		//ログの文字
		UI_TYP_MAX			//UIの最大数
	}UITYPE;//UIの種類
	CUi(int nPriority = 6);	//コンストラクタ
	~CUi();	//デストラクタ
	static CUi *Create(D3DXVECTOR3 pos, UITYPE type);	//背景の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_UI]; //共有テクスチャのポインタ
	D3DXCOLOR m_Color;
	int m_nCntColor;	//カラーカウント用
	CScene2D *m_pScene2D;
	UITYPE m_Type;	//背景のタイプ
};

#endif