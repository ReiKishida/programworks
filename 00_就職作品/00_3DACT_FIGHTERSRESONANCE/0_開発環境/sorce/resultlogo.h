//=============================================================================
//
// ロゴの処理[2D ポリゴン] [logo.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _RESULTLOGO_H_
#define _RESULTLOGO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RESULT_NUM (8)
#define MAX_RESULT_TEXTURE (8)

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CResultLogo : public CScene
{
public:
	typedef enum
	{
		RESULTSTATE_NONE = 0,	//空の状態
		RESULTSTATE_NORMAL,		//通常状態
		RESULTSTATE_USE,			//エンターが押された状態
		RESULTSTATE_MAX			//状態の最大数
	}RESULTSTATE;
	typedef enum
	{
		RESULTTYPE_LOGO = 0,		//タイトルロゴ
		RESULTTYPE_PRESS,		//プレスエンター
		RESULTTYPE_BONUS,		//クリアボーナス
		RESULTTYPE_SCORE,		//スコアボーナス
		RESULTTYPE_TIME,		//タイムボーナス
		RESULTTYPE_COMBO,		//コンボボーナス
		RESULTTYPE_DAMAGE,		//ダメージボーナス
		RESULTTYPE_TOTAL,		//TOTALボーナス
		RESULTTYPE_MAX			//タイトルの種類の最大数
	}RESULTTYPE;
	CResultLogo();				//コンストラクタ
	~CResultLogo();				//デストラクタ
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの破棄
	static CResultLogo *Create(void);	//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static void SwitchEnable(void);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RESULT_TEXTURE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_RESULT_NUM];//シーン2D
	static D3DXCOLOR m_col; //色情報
	int m_nCntFrame;//フレーム数のカウント
	float m_fWidth;//幅
	float m_fHeight;//高さ
	static RESULTSTATE m_state[MAX_RESULT_NUM]; //状態
	static RESULTTYPE m_type[MAX_RESULT_NUM];//タイトルの種類
	int m_TexNum;	//テクスチャの番号
	static int m_nJumpRankingFrame;//ランキング画面へ遷移するときのフレームカウンタ
	static bool m_bEnable;
};

#endif