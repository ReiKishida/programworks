//=============================================================================
//
// ランキングロゴの処理[2D ポリゴン] [rankinglogo.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _RANKINGLOGO_H_
#define _RANKINGLOGO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING_NUM (3)
#define MAX_RANKING_TEXTURE (3)

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CRankingLogo : public CScene
{
public:
	typedef enum
	{
		RANKINGSTATE_NONE = 0,	//空の状態
		RANKINGSTATE_NORMAL,		//通常状態
		RANKINGSTATE_USE,			//エンターが押された状態
		RANKINGSTATE_MAX			//状態の最大数
	}RANKINGSTATE;
	typedef enum
	{
		RANKINGTYPE_FLAME = 0,		//プレスエンター
		RANKINGTYPE_LOGO,		//タイトルロゴ
		RANKINGTYPE_PRESS,		//プレスエンター
		RANKINGTYPE_MAX			//タイトルの種類の最大数
	}RANKINGTYPE;
	CRankingLogo(int nPriority = 6, OBJTYPE type = OBJTYPE_RANKINGLOGO);				//コンストラクタ
	~CRankingLogo();				//デストラクタ
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの破棄
	static CRankingLogo *Create(void);	//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RANKING_TEXTURE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_RANKING_NUM];//シーン2D
	static D3DXCOLOR m_col; //色情報
	int m_nCntFrame;//フレーム数のカウント
	float m_fWidth;//幅
	float m_fHeight;//高さ
	RANKINGSTATE m_state;//状態
	RANKINGTYPE m_type;//タイトルの種類
	int m_TexNum;	//テクスチャの番号
	static int m_nJumpTitleFrame;//タイトル画面へ遷移するときのフレームカウンタ
};

#endif