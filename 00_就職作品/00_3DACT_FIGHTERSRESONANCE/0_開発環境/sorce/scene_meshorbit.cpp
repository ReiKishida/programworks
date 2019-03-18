//=============================================================================
//
// 軌跡の処理[3Dポリゴン] [scene_meshorbit.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "scene_meshorbit.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMeshOrbit::m_pTexture = NULL;
//=============================================================================
// 関数名：3Dシーンの生成
// 関数の概要：3Dシーンを生成する
//=============================================================================
CMeshOrbit *CMeshOrbit::Create()
{
	CMeshOrbit  *pOrbit;
	pOrbit = new CMeshOrbit;//シーンクラスの生成
	if (pOrbit != NULL)
	{
		pOrbit->m_fWidth = 50.0f;
		pOrbit->m_fHeight = 40.0f;
		pOrbit->Init();

	}
	return pOrbit;
}

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CMeshOrbit::CMeshOrbit(int nPriority, OBJTYPE type) : CScene(3, type)
{
	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_nNumScene3D++;
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}

//=============================================================================
// 関数名：3Dシーンの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{

	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gradation002.jpg", &m_pTexture);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{
		//頂点座標
		pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


		//法線の設定
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラー
		pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * 0.01f, nCntVtx * 0.01f);
	}
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	m_aOffSet[0] = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	m_aOffSet[1] = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 関数名：3Dシーンの終了処理
// 関数の概要：テクスチャ、頂点バッファ、自身の破棄
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

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
// 関数名：3Dシーンの更新処理
// 関数の概要：--
//=============================================================================
void CMeshOrbit::Update(void)
{


}

//=============================================================================
// 関数名：3Dシーンの描画処理
// 関数の概要：--
//=============================================================================
void CMeshOrbit::Draw(void)
{

	// 頂点情報の設定
	VERTEX_3D *pVtx;


	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxrot, mtxtrans;	//マトリックス情報

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int nCntVtx = 99; nCntVtx > 1; nCntVtx--)
	{
		m_aPosVertex[nCntVtx] = m_aPosVertex[nCntVtx - 2];
	}

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{
		D3DXVec3TransformCoord(&m_aPosVertex[nCntVtx], &m_aOffSet[nCntVtx], m_pMtxParent);
	}

	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{
		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[nCntVtx].pos = m_aPosVertex[nCntVtx];

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();

	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 98);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング


	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);





}

//=============================================================================
// 関数名：位置情報の取得
// 関数の概要：位置情報を返す
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetPosPolygon(void)
{
	return m_posPolygon;
}

//=============================================================================
// 関数名：位置情報の設定
// 関数の概要：位置情報を設定する
//=============================================================================
void CMeshOrbit::SetPosPolygon(D3DXVECTOR3 pos)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//位置情報を代入
	m_posPolygon = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 関数名：角度情報の取得
// 関数の概要：位置情報を返す
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetRotPolygon(void)
{
	return m_rotPolygon;
}

//=============================================================================
// 関数名：角度情報の設定
// 関数の概要：角度情報を設定する
//=============================================================================
void CMeshOrbit::SetRotPolygon(D3DXVECTOR3 rot)
{


	m_rotPolygon = rot;


}

//=============================================================================
// 関数名：マトリックス情報の設定
// 関数の概要：位置情報を設定する
//=============================================================================
void CMeshOrbit::SetMtxParent(D3DXMATRIX *pMtx)
{
	m_pMtxParent = pMtx;
}

//=============================================================================
// 関数名：テクスチャのわりあて
// 関数の概要：m_pTextureにテクスチャを割り当てる
//=============================================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// 関数名：色情報の設定
// 関数の概要：頂点情報に色情報を代入
//=============================================================================
void CMeshOrbit::SetColor(D3DXCOLOR col)
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

void CMeshOrbit::CreateOrbit(void)
{
}



