//=============================================================================
//
// シーンゲージ処理[2D ポリゴン] [scenegauge.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENEGAUGE_H_
#define _SCENEGAUGE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン2Dのクラス構造体
//*****************************************************************************
class CSceneGauge : public CScene
{
public:
	CSceneGauge::CSceneGauge(int nPriority = 7, OBJTYPE type = OBJTYPE_SCENEGAUGE);	//コンストラクタ
	CSceneGauge::~CSceneGauge();	//デストラクタ
	static CSceneGauge *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void SetTexture(float fTex_X, float fTex_Y);//テクスチャの設定
	void SetColor(D3DXCOLOR col);	//色の設定
	void SlipGauge(void);			//ゲージ増減

	//テクスチャアニメーション
	void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);
	void SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y);

	//ポリゴンの大きさの取得、設定
	void SetGaugeWidth(float fWidth);
	void SetGaugeHeight(float fWidth);

	//ポリゴンの位置情報の取得、設定
	D3DXVECTOR3 GetPosGauge(void);
	void SetPosGauge(D3DXVECTOR3 pos);

	//ポリゴンの大きさの取得、設定
	float GetWidthGauge(void);
	float GetHeightGauge(void);

	//現在値の設定
	void SetCurrentValue(float fCurrentValue);

	//最大値の設定
	void SetMaxValue(float fMaxValue);



protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_posGauge;		// ポリゴンの位置
	D3DXVECTOR3				m_rotGauge;		// ポリゴンの回転
	float m_fLengthGauge;					//ポリゴンの長さ
	float m_fAngleGauge;					//ポリゴンの角度
	float m_fGaugeWidth;					//ポリゴンの幅
	float m_fGaugeHeight;					//ポリゴンの高さ
	int m_nCntflame;						//カウント
	float m_fCurrentValue;					//現在の値
	float m_fMaxValue;						//ゲージの最大値
	bool  m_bDamage;						//ダメージ状態
};

#endif