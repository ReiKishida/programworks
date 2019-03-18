//=============================================================================
//
// メニュー画面処理[2D ポリゴン] [menu.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MENUTEXTURE_NUM (7)	//メニュー画面のテクスチャ数
#define MAX_MENUPOLYGON_NUM (5)	//メニュー画面のテクスチャ数

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CMenuScene : public CScene
{
public:
	typedef enum
	{
		MENUSTATE_NONE = 0,	//空の状態
		MENUSTATE_NORMAL,		//通常状態
		MENUSTATE_USE,			//エンターが押された状態
		MENUSTATE_MAX			//状態の最大数
	}MENUSTATE;
	typedef enum
	{
		MENUTYPE_GAMEMODE = 0,	//ゲームモード
		MENUTYPE_RANKING,		//ランキング
		MENUTYPE_QUIT,			//クイット
		MENUTYPE_HUMAN,			//人
		MENUTYPE_WINDOW_GAME,		//ウィンドウ
		MENUTYPE_WINDOW_RANKING,		//ウィンドウ
		MENUTYPE_WINDOW_QUIT,		//ウィンドウ
		MENUTYPE_MAX			//状態の最大数
	}MENUTYPE;
	CMenuScene();	//コンストラクタ
	~CMenuScene();	//デストラクタ
	static CMenuScene *Create(D3DXVECTOR3 pos);	//背景の生成
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_MENUTEXTURE_NUM]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_MENUPOLYGON_NUM];//シーン2Dのポインタ
	D3DXVECTOR3 m_MenuRot;//角度情報
	D3DXVECTOR3 m_MenuPos;//位置情報
	D3DXCOLOR m_col;	//色情報
	int m_nTex_Y;//テクスチャ座標
	int m_nCntFrame;//フレーム数のカウント
	static int m_nSelectMenu;	//選択中の番号
	static int m_nSelectState;	//選択中の状態
	float m_fWidth;//幅
	float m_fHeight;//高さ
	D3DXVECTOR3 m_fDestAngle;//目的の角度
	D3DXVECTOR3 m_fAngle;//角度
	static bool m_bRotate;	//回転してるか(+)
	MENUSTATE m_state;//状態
	MENUTYPE m_type; //種類
};

#endif