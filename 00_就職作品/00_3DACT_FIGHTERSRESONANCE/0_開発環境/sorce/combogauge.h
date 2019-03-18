//=============================================================================
//
// ゲージ処理[2D ポリゴン] [ui_gauge.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _COMBOGAUGE_H_
#define _COMBOGAUGE_H_

#include "main.h"
#include "scene_gauge.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// シーン2Dのクラス構造体
//*****************************************************************************
class CComboGauge : public CSceneGauge
{
public:
	typedef enum
	{
		GAUGETYPE_UNDER = 0,				//ゲージの下地
		GAUGETYPE_COMBO,				//本体力
		GAUGETYPE_MAX					//ゲージの種類の最大数
	}GAUGETYPE;
	CComboGauge::CComboGauge(int nPriority = 7, OBJTYPE type = OBJTYPE_COMBOGAUGE);	//コンストラクタ
	CComboGauge::~CComboGauge();	//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み
	static void Unload(void);									//テクスチャの破棄
	static CComboGauge *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type);//ゲージの生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	 void AddValueSlip(float fValue);
	 void SetComboGauge(float fValue);
	static float GetComboGauge(void);
	float GetCurrentGauge(void);
	void SetCurrentGauge(float fValue);

	static float GetComboGaugeMax(void);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_rot;			// ポリゴンの回転
	D3DXCOLOR			    m_col;			//ポリゴンの色
	static float m_fComboGauge;				//コンボ量
	static float m_fComboGaugeMax;			//以前のコンボ量

	float m_fWidth;							//ポリゴンの幅
	float m_fHeight;						//ポリゴンの高さ
	float m_fCurrentGauge;					//現在の値
	float m_fMaxGauge;						//ゲージの最大値
	float m_fDestValue;						//目的の値
	float m_fValue;							//増量
	float m_fMaxValue;						//最大数
	float m_fReminder;
	GAUGETYPE m_type;						//ゲージの種類
	static int m_nCombo;							//コンボ数
	int m_nCntFrame;						//フレームのカウント

};

#endif