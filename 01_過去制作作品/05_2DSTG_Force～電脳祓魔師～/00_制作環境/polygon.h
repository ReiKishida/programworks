//=============================================================================
//
// ポリゴン生成処理[2D ポリゴン] [polygon.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************



class CPolygon
{
public:

	CPolygon();	//コンストラクタ
	~CPolygon();	//デストラクタ
	HRESULT Init(D3DXVECTOR3 pos);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	CPolygon *Create(D3DXVECTOR3 pos);//敵の生成
	void SetTexture(float fTex_X, float fTex_Y);//テクスチャの設定
	void SetPolygonTexture(int nType);//テクスチャの設定
	void SetPolygon(int nType);	//の数
	int GetPolygon();

	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て


	//ポリゴンの大きさの設定
	void SetPolygonWidth(float fWidth);
	void SetPolygonHeight(float fWidth);

	//ポリゴンの位置情報の取得、設定
	 D3DXVECTOR3 GetPosPolygon(void);
	void SetPosPolygon(D3DXVECTOR3 pos);

	//ポリゴンの角度情報の設定、設定
	D3DXVECTOR3 GetRotPolygon(void);
	void SetRotPolygon(D3DXVECTOR3 rot);

	//ポリゴンの長さの取得、設定
	float GetLengthPolygon(void);
	void SetLengthPolygon(float fLength);

	//ポリゴンの大きさの取得、設定
	 float GetWidthPolygon(void);
	 float GetHeightPolygon(void);

	 //ポリゴンの色情報の取得、設定
	 D3DXCOLOR GetPolygonColor(void);
	 void SetPolygonColor(D3DXCOLOR col);	//色の設定

	 //ライフの設定
	 void SetLife(int nLife);

	 //テクスチャアニメーション
	 void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);

	 //テクスチャ情報設定
	 void SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_posPolygon;			// ポリゴンの位置
	D3DXVECTOR3				m_rotPolygon;			// ポリゴンの回転
	float m_fLengthPolygon;	//ポリゴンの長さ
	float m_fAnglePolygon;	//ポリゴンの角度
	float m_fPolygonWidth;	//ポリゴンの幅
	float m_fPolygonHeight;	//ポリゴンの高さ
	int m_nCntflame;		//カウント
	int m_nNumScene2D;	//2Dポリゴン数
	static CManager *m_pManager;//マネージャ
	int m_nLife;//ライフ
	int m_nType;//種類

#ifdef _DEBUG
	LPD3DXFONT				m_pFont;			// フォントへのポインタ
#endif

};

#endif