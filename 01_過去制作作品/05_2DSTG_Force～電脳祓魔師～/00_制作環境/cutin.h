//=============================================================================
//
// カットイン処理[2D ポリゴン] [cutin.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef CUTIN_H_
#define _CUTIN_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_CUTIN (6)	//テクスチャ数
#define MAX_LIFE_RYOU (75)
#define MAX_LIFE_IKI (75)
#define MAX_LIFE_TEN (55)
#define MAX_LIFE_KAI (45)
#define MAX_LIFE_CUTIN (90)

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CCutin : public CScene2D
{
public:
	typedef enum
	{
		CUTIN_TYPE_NONE = 0,
		CUTIN_TYPE_RYOU,
		CUTIN_TYPE_IKI,
		CUTIN_TYPE_TEN,
		CUTIN_TYPE_KAI,
		CUTIN_TYPE_CUT_PLAYER,
		CUTIN_TYPE_CUT_ENEMY,
		CUTIN_TYP_MAX
	}CUTINTYPE;
	CCutin();	//コンストラクタ
	~CCutin();	//デストラクタ
	static CCutin *Create(D3DXVECTOR3 pos, CUTINTYPE type);	//背景の生成
	HRESULT Init(D3DXVECTOR3 pos, CUTINTYPE type);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄
	static void NextAnim(void);	//テクスチャアニメーションアニメーション
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_CUTIN]; //共有テクスチャのポインタ
	 CUTINTYPE m_Type;	//背景のタイプ
	D3DXCOLOR m_Color;	//色設定
	int m_nCntColor;	//カラーカウント用
	int m_nLife;	//ライフ
	float m_fWidth;	//幅
	float m_fHeight;	//高さ
	static int m_nPatternAnim;//パターンアニメーション
	static int m_nCounterAnim;//パターンアニメーション
};

#endif