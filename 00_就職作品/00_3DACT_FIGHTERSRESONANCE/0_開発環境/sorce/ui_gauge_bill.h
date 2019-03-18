//=============================================================================
//
// ゲージ処理[2D ポリゴン] [ui_gauge.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _UIGAUGE_H_
#define _SUIGAUGE_H_

#include "main.h"
#include "scene_gauge_bill.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン2Dのクラス構造体
//*****************************************************************************
class CUiGaugeBillBoard : public CSceneGaugeBillBoard
{
public:
	typedef enum
	{
		GAUGETYPE_UI = 0,				//プレイヤーの顔グラ
		GAUGETYPE_UNDER,				//ゲージの下地
		GAUGETYPE_GREEN,				//本体力
		GAUGETYPE_RED,					//仮体力
		GAUGETYPE_SKILL,					//スタンゲージ
		GAUGETYPE_MAX					//ゲージの種類の最大数
	}GAUGETYPE;
	CUiGaugeBillBoard::CUiGaugeBillBoard(int nPriority = 3, OBJTYPE type = OBJTYPE_UIGAUGEBILLBOARD);	//コンストラクタ
	CUiGaugeBillBoard::~CUiGaugeBillBoard();	//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み
	static void Unload(void);									//テクスチャの破棄
	static CUiGaugeBillBoard *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type);//ゲージの生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void AddValue(float fValue);
	void AddValueSlip(float fValue);


private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_rot;			// ポリゴンの回転
	D3DXCOLOR			    m_col;			//ポリゴンの色
	float m_fWidth;							//ポリゴンの幅
	float m_fHeight;						//ポリゴンの高さ
	float m_fCurrentGauge;					//現在の値
	float m_fMaxGauge;						//ゲージの最大値
	float m_fDestValue;						//目的の値
	float m_fValue;							//現在の算出した値
	float m_fValueOld;						//過去の算出した値
	GAUGETYPE m_type;						//ゲージの種類
	int m_nCntFrame;						//フレームのカウント
};

#endif