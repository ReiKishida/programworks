//=============================================================================
//
// シーンゲージ処理[2D ポリゴン] [scenegauge.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENEGAUGEBILL_H_
#define _SCENEGAUGEBILL_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン2Dのクラス構造体
//*****************************************************************************
class CSceneGaugeBillBoard : public CScene
{
public:
	CSceneGaugeBillBoard::CSceneGaugeBillBoard(int nPriority = 3, OBJTYPE type = OBJTYPE_SCENEGAUGE);	//コンストラクタ
	CSceneGaugeBillBoard::~CSceneGaugeBillBoard();	//デストラクタ
	static CSceneGaugeBillBoard *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void SetColor(D3DXCOLOR col);	//色の設定


	//ポリゴンの大きさの取得、設定
	void SetGaugeWidth(float fWidth);
	void SetGaugeHeight(float fWidth);


	//現在値の設定
	void SetCurrentValue(float fCurrentValue);

	//最大値の設定
	void SetMaxValue(float fMaxValue);

	//位置情報の代入
	void SetPosGauge(D3DXVECTOR3 pos);


protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;		// ポリゴンの位置
	D3DXMATRIX              m_mtxWorld;		//マトリックス情報
	float m_fLengthGauge;					//ポリゴンの長さ
	float m_fAngleGauge;					//ポリゴンの角度
	float m_fWidth;					//ポリゴンの幅
	float m_fHeight;					//ポリゴンの高さ
	int m_nCntflame;						//カウント
	float m_fCurrentValue;					//現在の値
	float m_fMaxValue;						//ゲージの最大値
	bool  m_bDamage;						//ダメージ状態
};

#endif