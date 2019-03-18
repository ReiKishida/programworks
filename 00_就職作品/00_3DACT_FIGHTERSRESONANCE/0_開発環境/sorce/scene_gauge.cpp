//=============================================================================
//
// シーン処理[2Dポリゴン] [scene_gauge.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene_gauge.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DIRE_WIDTH (10.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// 関数名：ゲージの生成
// 関数の概要：ゲージを生成する
//=============================================================================
CSceneGauge *CSceneGauge::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CSceneGauge  *pSceneGauge;
	pSceneGauge = new CSceneGauge;//シーンクラスの生成
	if (pSceneGauge != NULL)
	{
		pSceneGauge->m_posGauge = pos;//位置情報の代入
		pSceneGauge->m_fGaugeWidth = fWidth;
		pSceneGauge->m_fGaugeHeight = fHeight;
		pSceneGauge->m_fCurrentValue = fWidth;
		pSceneGauge->m_fMaxValue = fWidth;
		pSceneGauge->m_bDamage = false;
		pSceneGauge->Init();
	}
	return pSceneGauge;
}

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CSceneGauge::CSceneGauge(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CSceneGauge::~CSceneGauge()
{

}

//=============================================================================
// 関数名：ゲージの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CSceneGauge::Init(void)
{


	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	VERTEX_2D *pVtx; // 頂点情報の作成
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_posGauge.x + DIRE_WIDTH,
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[1].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) + DIRE_WIDTH,
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[2].pos = D3DXVECTOR3(m_posGauge.x - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);

	pVtx[3].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);


	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


	return S_OK;
}

//=============================================================================
// 関数名：ゲージの終了処理
// 関数の概要：テクスチャ、頂点バッファの破棄、自身の破棄
//=============================================================================
void CSceneGauge::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクト(自分自身)の破棄
	Release();

}

//=============================================================================
// 関数名：ゲージの更新処理
// 関数の概要：--
//=============================================================================
void CSceneGauge::Update(void)
{

}

//=============================================================================
// 関数名：ゲージの描画処理
// 関数の概要：--
//=============================================================================
void CSceneGauge::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わなくても書く！)
	pDevice->SetTexture(0, m_pTexture);


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}


//=============================================================================
// 関数名：ポリゴンの位置を取得
// 関数の概要：ポリゴンの位置を返す
//=============================================================================
D3DXVECTOR3 CSceneGauge::GetPosGauge(void)
{
	return m_posGauge;
}

//=============================================================================
// 関数名：ポリゴンの位置を設定
// 関数の概要：ポリゴンの位置を設定する
//=============================================================================
void CSceneGauge::SetPosGauge(D3DXVECTOR3 pos)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//位置情報を代入
	m_posGauge = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_posGauge.x + (DIRE_WIDTH +10.0f),
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[1].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) + (DIRE_WIDTH + 10.0f),
		m_posGauge.y - m_fGaugeHeight,
		0.0f);

	pVtx[2].pos = D3DXVECTOR3(m_posGauge.x - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);

	pVtx[3].pos = D3DXVECTOR3(m_posGauge.x + ((m_fCurrentValue / m_fMaxValue) * m_fGaugeWidth) - DIRE_WIDTH,
		m_posGauge.y + m_fGaugeHeight,
		0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// 関数名：ポリゴンの幅を設定
// 関数の概要：ポリゴンの幅を設定する
//=============================================================================
void CSceneGauge::SetGaugeWidth(float fWidth)
{
	m_fGaugeWidth = fWidth;
}
//=============================================================================
// 関数名：ポリゴンの高さを設定
// 関数の概要：ポリゴンの高さを設定する
//=============================================================================
void CSceneGauge::SetGaugeHeight(float fHeight)
{
	m_fGaugeHeight = fHeight;

}
//=============================================================================
// 関数名：テクスチャの設定
// 関数の概要：テクスチャを割り当てる
//=============================================================================
void CSceneGauge::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// 関数名：テクスチャの座標設定
// 関数の概要：テクスチャの座標を設定する
//=============================================================================
void CSceneGauge::SetTexture(float fTex_X, float fTex_Y)
{

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

					 // 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
// 関数名：テクスチャアニメーション
// 関数の概要：アニメーションを動かす
//=============================================================================
void CSceneGauge::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
{
	//デバイスの取得
	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


					 // 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// 関数名：テクスチャアニメーション(Y軸)
// 関数の概要：アニメーションを動かす
//=============================================================================
void CSceneGauge::SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y)
{

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 関数名：ポリゴンの色設定
// 関数の概要：ポリゴンの色を設定する
//=============================================================================
void CSceneGauge::SetColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// 関数名：ポリゴンの幅を取得
// 関数の概要：ポリゴンの幅を返す
//=============================================================================
float CSceneGauge::GetWidthGauge(void)
{
	return m_fGaugeWidth;
}
//=============================================================================
// 関数名：ポリゴンのを高さ取得
// 関数の概要：ポリゴンの幅を返す
//=============================================================================
float CSceneGauge::GetHeightGauge(void)
{
	return m_fGaugeHeight;

}
//=============================================================================
// 関数名：現在の値の代入
// 関数の概要：現在の値の代入する
//=============================================================================
void CSceneGauge::SetCurrentValue(float fCurrentValue)
{
	m_fCurrentValue = fCurrentValue;
}

//=============================================================================
// 関数名：最大値の代入
// 関数の概要：最大値の代入する
//=============================================================================
void CSceneGauge::SetMaxValue(float fMaxValue)
{
	m_fMaxValue = fMaxValue;
}



//=============================================================================
// 関数名：ゲージの増減
// 関数の概要：ポリゴンの幅を返す
//=============================================================================
void CSceneGauge::SlipGauge(void)
{



}


