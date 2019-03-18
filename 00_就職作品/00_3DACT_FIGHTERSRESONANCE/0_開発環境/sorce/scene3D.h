//=============================================================================
//
// シーン3D処理[3D ポリゴン] [scene3D.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CScene3D : public CScene
{
public:

	CScene3D::CScene3D(int nPriority = 3, OBJTYPE type = OBJTYPE_SCENE3D);	//コンストラクタ
	CScene3D::~CScene3D();	//デストラクタ
	static CScene3D *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	int GetNum2DSceneAll(void);	//2Dシーン数の取得
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void SetColor(D3DXCOLOR col);	//色の設定
	void SetPriority(int nPriority);//優先順位

	//ポリゴンの位置情報の取得、設定
	D3DXVECTOR3 GetPosPolygon(void);
	void SetPosPolygon(D3DXVECTOR3 pos);

	//ポリゴンの角度情報の取得、設定
	D3DXVECTOR3 GetRotPolygon(void);
	void SetRotPolygon(D3DXVECTOR3 pos);

	void SetWidth(float fWidth);
	void SetHeight(float fHeight);

	//背景スクロール
	void ScrollPolygon(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3);
	void SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y);//テクスチャの設定


private:
	LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorldPolygon;			//ポリゴンのマトリックス情報
	D3DXVECTOR3				m_posPolygon;			// ポリゴンの位置
	D3DXVECTOR3				m_rotPolygon;			// ポリゴンの回転
	static CManager *m_pManager;//マネージャ
	int m_nCntflame;		//カウント
	int m_nNumScene2D;	//2Dポリゴン数
	int m_nPriority;//優先度
	float m_fWidth;	//ポリゴンの幅
	float m_fHeight; //ポリゴンの奥行き
	float m_fnor; //ポリゴンの法線
};

#endif