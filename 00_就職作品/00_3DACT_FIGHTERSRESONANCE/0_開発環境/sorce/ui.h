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
#define MAX_TEXTURE_UI (8)	//テクスチャ数
#define MAX_UI (4)			//UI数
#define MAX_UI_TUTORIAL (8)			//UI数

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CUi : public CScene2D
{
public:
	typedef enum
	{
		UIMODE_NONE = 0,
		UIMODE_TUTORIAL,
		UIMODE_GAME,
		UIMODE_MAX
	}UIMODE;
	typedef enum
	{
		UITYPE_SCORE = 0,			//スコア
		UITYPE_TIME,				//タイムの文字
		UITYPE_COLON,				//コロン
		UITYPE_DOT,					//ドット
		UITYPE_CONTROLL_ARCADE,		//操作
		UITYPE_CONTROLL_KEYBOARD,	//操作
		UITYPE_SKILL,
		UITYPE_END,
		UI_TYP_MAX				//UIの最大数
	}UITYPE;//UIの種類
	CUi(int nPriority = 7, OBJTYPE type = OBJTYPE_UI);	//コンストラクタ
	~CUi();	//デストラクタ
	static CUi *Create(UIMODE mode);	//背景の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_UI]; //共有テクスチャのポインタ
	D3DXCOLOR m_Color;
	int m_nCntColor;	//カラーカウント用
	CScene2D *m_pScene2D[MAX_UI];
	CScene2D *m_pScene2D_Tuto[MAX_UI_TUTORIAL];
	UIMODE m_mode;	//UIのモード
	UITYPE m_Type;	//UIのタイプ
};

#endif