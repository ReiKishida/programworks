//=============================================================================
//
// シーン2D処理[3Dポリゴン] [scene3D.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_WIDTH (30) //ポリゴンのXの大きさ
#define POLYGON_HEIGHT (30) //ポリゴンのYの大きさ
#define ANIMATION_SPEED (8)	//アニメーションのスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (1.0f)	//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager *CScene3D::m_pManager = NULL;
//=============================================================================
//3Dシーンの生成
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CScene3D  *pScene3D;
	pScene3D = new CScene3D;//シーンクラスの生成
	if (pScene3D != NULL)
	{
		pScene3D->m_posPolygon = pos;
		pScene3D->m_fWidth = fWidth;
		pScene3D->m_fHeight = fHeight;
		pScene3D->Init();

	}
	return pScene3D;
}

//=============================================================================
//コンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority , OBJTYPE type) :CScene(nPriority, type)
{
	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
												 //m_apTexture = NULL;
												 //m_pVtxBuff = NULL;
	m_nNumScene2D++;
}

//=============================================================================
//デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CScene3D::Init(void)
{

	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CScene3D::Uninit(void)
{

	//// テクスチャの破棄
	//if (m_pTexture != NULL)
	//{
	//	m_pTexture->Release();
	//	m_pTexture = NULL;
	//}


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
//更新処理
//=============================================================================
void CScene3D::Update(void)
{


}

//=============================================================================
//描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//マトリックス情報

								//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldPolygon);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rotPolygon.y, m_rotPolygon.x, m_rotPolygon.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_posPolygon.x, m_posPolygon.y, m_posPolygon.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldPolygon);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
//3Dシーンの総数の取得
//=============================================================================
int CScene3D::GetNum2DSceneAll(void)
{
	return m_nNumScene2D;
}

//=============================================================================
//ポリゴンの位置の取得
//=============================================================================
D3DXVECTOR3 CScene3D::GetPosPolygon(void)
{
	return m_posPolygon;
}

//=============================================================================
//ポリゴンの位置の設定
//=============================================================================
void CScene3D::SetPosPolygon(D3DXVECTOR3 pos)
{
	// 頂点情報の作成
	VERTEX_3D *pVtx;

	//位置情報を代入
	m_posPolygon = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンの角度の取得
//=============================================================================
D3DXVECTOR3 CScene3D::GetRotPolygon(void)
{
	return m_rotPolygon;
}

//=============================================================================
//ポリゴンの角度の設定
//=============================================================================
void CScene3D::SetRotPolygon(D3DXVECTOR3 rot)
{


	m_rotPolygon = rot;


}

void CScene3D::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
}

void CScene3D::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//テクスチャのスクロール
//=============================================================================
void CScene3D::ScrollPolygon(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3)
{

	// 頂点情報を設定
	VERTEX_3D *pVtx; // 頂点情報の作成


	 // 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = Scroll_0;
	pVtx[1].tex = Scroll_1;
	pVtx[2].tex = Scroll_2;
	pVtx[3].tex = Scroll_3;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CScene3D::SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点情報を設定
	VERTEX_3D *pVtx; // 頂点情報の作成

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

//=============================================================================
//テクスチャの割り当て
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//============================================================================================
//色の設定
//============================================================================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx; // 頂点情報の作成


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
//優先順位の設定
//============================================================================================
void CScene3D::SetPriority(int nPriority)
{
	m_nPriority = nPriority;
}

