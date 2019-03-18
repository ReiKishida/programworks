//=============================================================================
//
// 背景処理[2D ポリゴン] [bg.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_COMBO (6)	//テクスチャ数

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CCombo : public CScene
{
public:
	typedef enum
	{
		COMBOTYPE_S = 0,	//S
		COMBOTYPE_A,		//A
		COMBOTYPE_B,		//B
		COMBOTYPE_C,		//C
		COMBOTYPE_D,		//D
		COMBOTYPE_NONE,	//NONE
		COMBOTYPE_MAX		//コンボの最大数
	}COMBOTYPE;//UIの種類
	CCombo(int nPriority = 7, OBJTYPE type = OBJTYPE_COMBO);	//コンストラクタ
	~CCombo();	//デストラクタ
	static CCombo *Create(void);	//背景の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_COMBO]; //共有テクスチャのポインタ
	D3DXCOLOR m_Color;
	int m_nCntColor;	//カラーカウント用
	CScene2D *m_pScene2D;
	COMBOTYPE m_Type;	//UIのタイプ
	float m_fCombo;		//コンボ量
};

#endif