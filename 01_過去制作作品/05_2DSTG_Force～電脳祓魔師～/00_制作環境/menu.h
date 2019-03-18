//=============================================================================
//
// メニュー画面処理[2D ポリゴン] [menu.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CMenu
{
public:
	typedef enum
	{
		MENUSTATE_NONE = 0,	//空の状態
		MENUSTATE_NORMAL,		//通常状態
		MENUSTATE_USE,			//エンターが押された状態
		MENUSTATE_MAX			//状態の最大数
	}MENUSTATE;

	CMenu();	//コンストラクタ
	~CMenu();	//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	static D3DXCOLOR m_col; //色情報
	D3DXVECTOR3 m_MenuRot;//角度情報
	D3DXVECTOR3 m_MenuPos;//位置情報
	int m_nPatternAnim;	//アニメーションパターン
	int m_nCounterAnim;//アニメーションカウンター
	int m_nTex_Y;//テクスチャ座標
	int m_nCntFrame;//フレーム数のカウント
	int m_nSelect;	//選択中の番号
	float m_fWidth;//幅
	float m_fHeight;//高さ
	float m_fDestAngle;//目的の角度
	float m_fAngle;//角度
	static bool m_bRotate;	//回転してるか
	MENUSTATE m_state;//状態
};

#endif