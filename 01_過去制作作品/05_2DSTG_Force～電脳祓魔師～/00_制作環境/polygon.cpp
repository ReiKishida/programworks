//=============================================================================
//
// シーン処理[2Dポリゴン] [scene2D.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "explosion.h"
#include "number.h"
#include "polygon.h"
#include "force.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIMATION_SPEED (8)	//アニメーションのスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (1.0f)	//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************


//=============================================================================
//コンストラクタ
//=============================================================================
CPolygon::CPolygon()
{
	m_nType = CForce::FORCETYPE_ATTACK;
	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_fPolygonWidth = 0.0f;	//ポリゴンの幅
	m_fPolygonHeight = 0.0f;	//ポリゴンの高さ
	m_pVtxBuff = NULL;

}

//=============================================================================
//デストラクタ
//=============================================================================
CPolygon::~CPolygon()
{

}

//=============================================================================
//2Dシーンの生成
//=============================================================================
CPolygon *CPolygon::Create(D3DXVECTOR3 pos)
{
	CPolygon  *pPolygon;
	pPolygon = new CPolygon;//シーンクラスの生成
	if (pPolygon != NULL)
	{
		pPolygon->Init(pos);

	}
	return pPolygon;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos)
{
	m_nType = CForce::FORCETYPE_ATTACK;

	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// ポリゴンの位置を設定
	m_posPolygon = pos;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

					 ////ポリゴンの長さ
	m_fLengthPolygon = sqrtf((m_fPolygonWidth / 2 + m_fPolygonWidth / 2) * (m_fPolygonHeight / 2 + m_fPolygonHeight / 2));

	//ポリゴンの角度
	m_fAnglePolygon = atan2f(m_fPolygonWidth / 2, m_fPolygonHeight / 2);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth, m_posPolygon.y - m_fPolygonHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth, m_posPolygon.y - m_fPolygonHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth, m_posPolygon.y + m_fPolygonHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth, m_posPolygon.y + m_fPolygonHeight, 0.0f);


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
//終了処理
//=============================================================================
void CPolygon::Uninit(void)
{


	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
//更新処理
//=============================================================================
void CPolygon::Update(void)
{



}

//=============================================================================
//描画処理
//=============================================================================
void CPolygon::Draw(void)
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
//ポリゴンの位置の取得
//=============================================================================
D3DXVECTOR3 CPolygon::GetPosPolygon(void)
{
	return m_posPolygon;
}

//=============================================================================
//ポリゴンの位置の設定
//=============================================================================
void CPolygon::SetPosPolygon(D3DXVECTOR3 pos)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//位置情報を代入
	m_posPolygon = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth / 2, m_posPolygon.y - m_fPolygonHeight / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth / 2, m_posPolygon.y - m_fPolygonHeight / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x - m_fPolygonWidth / 2, m_posPolygon.y + m_fPolygonHeight / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + m_fPolygonWidth / 2, m_posPolygon.y + m_fPolygonHeight / 2, 0.0f);



	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンの角度の取得
//=============================================================================
D3DXVECTOR3 CPolygon::GetRotPolygon(void)
{
	return m_rotPolygon;
}

//=============================================================================
//ポリゴンの角度の設定
//=============================================================================
void CPolygon::SetRotPolygon(D3DXVECTOR3 rot)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	m_rotPolygon = rot;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + sinf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + sinf(m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンの長さの取得
//=============================================================================
float CPolygon::GetLengthPolygon(void)
{
	return m_fLengthPolygon;
}

//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CPolygon::SetLengthPolygon(float fLength)
{
	m_fLengthPolygon = fLength;
}

//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CPolygon::SetPolygonWidth(float fWidth)
{
	m_fPolygonWidth = fWidth;
}
//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CPolygon::SetPolygonHeight(float fHeight)
{
	m_fPolygonHeight = fHeight;

}

//=============================================================================
//テクスチャの設定
//=============================================================================
void CPolygon::SetTexture(float fTex_X, float fTex_Y)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

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

//============================================================================================
//色の設定
//============================================================================================
void CPolygon::SetPolygonColor(D3DXCOLOR col)
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

//============================================================================================
//色情報の取得
//============================================================================================
D3DXCOLOR CPolygon::GetPolygonColor(void)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

					 // 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	return pVtx[0].col;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//============================================================================================
//幅の取得
//============================================================================================
float CPolygon::GetWidthPolygon(void)
{
	return m_fPolygonWidth;
}
//============================================================================================
//幅の取得
//============================================================================================
float CPolygon::GetHeightPolygon(void)
{
	return m_fPolygonHeight;

}




//============================================================================================
//ライフの設定
//============================================================================================
void CPolygon::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================================
//フォースのテクスチャ設定
//============================================================================================
void CPolygon::SetPolygonTexture(int nType)
{
	m_nType = nType;

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 1.0f);
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================================================
//テクスチャ設定
//============================================================================================
void CPolygon::SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y)
{
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

					 // 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(fTex_X_0, fTex_Y_0);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, fTex_Y_0);
	pVtx[2].tex = D3DXVECTOR2(fTex_X_0, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}



//============================================================================================
//状態の設定
//============================================================================================
void CPolygon::SetPolygon(int nType)
{
	m_nType = nType;


}

//============================================================================================
//状態の取得
//============================================================================================
int CPolygon::GetPolygon()
{
	return m_nType;
}

//=============================================================================
//テクスチャ設定
//============================================================================
void CPolygon::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}


//=============================================================================
//テクスチャアニメーションの設定
//=============================================================================
void CPolygon::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

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
