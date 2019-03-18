//=============================================================================
//
// 背景処理[2D ポリゴン] [bg.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE (9)	//テクスチャ数
#define MAX_BG (4)	//テクスチャ数
//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CBg : public CScene
{
public:
	typedef enum
	{
		BGFADE_NONE = 0,		// 何もしていない状態
		BGFADE_IN,			// フェードイン状態
		BGFADE_OUT,			// フェードアウト状態
		BGFADE_MAX			//フェードの最大数
	} BGFADE;

	typedef enum
	{
		BGTYPE_NONE = 0,		// 何もしていない状態
		BGTYPE_PLAYERNORMAL,			// プレイヤーのハック状態からノーマルに戻すとき
		BGTYPE_BOSSNORMAL,			// ボスのハック状態からノーマルに戻すとき
		BGTYPE_HACK_PLAYER,			// ハッキング状態(プレイヤー)
		BGTYPE_HACK_ENEMY,			// ハッキング状態(エネミー)
		FADE_MAX
	} BGTYPE;
	CBg(int nPriority = 7);	//コンストラクタ
	~CBg();	//デストラクタ
	static CBg *Create(D3DXVECTOR3 pos);	//背景の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄
	static void SetBG(BGTYPE type);
	static BGTYPE GetBG(void);

	static void SetFade(BGTYPE modeNext);//フェードの設定
	static BGFADE GetBgFade(void);//フェードの取得
	void SetTexture(float fTex_X_0, float fTex_X_1, float fTex_Y_0, float fTex_Y_1);//テクスチャの設定


private:
	static BGTYPE m_type;//背景の種類
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_BG];//シーン2D
	D3DXVECTOR2 m_aBg[MAX_BG];//背景の最大数
	int m_nType;	//背景のタイプ
	int m_nCntFrame;	//フレーム数カウント用
	static D3DXCOLOR m_col[MAX_BG]; //カラー代入用
	static BGTYPE m_typeNext;//次の背景の種類
	static BGFADE m_BgFade;//背景フェードの
	static int m_nPatternAnim;	//アニメーションパターン
	static int m_nCounterAnim;//アニメーションカウンター
	static int m_nTex_Y;//テクスチャの設定

};

#endif