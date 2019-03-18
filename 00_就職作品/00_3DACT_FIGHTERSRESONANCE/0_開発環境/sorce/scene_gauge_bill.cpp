//=============================================================================
//
// ビルボード処理[3Dポリゴン] [scene_billboard.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "scene_gauge_bill.h"
#include "camera.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DIRE_WIDTH (10.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
//=============================================================================
// 関数名：ビルボードの生成
// 関数の概要：ビルボードを生成する
//=============================================================================
CSceneGaugeBillBoard *CSceneGaugeBillBoard::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CSceneGaugeBillBoard  *pSceneBillboard;
	pSceneBillboard = new CSceneGaugeBillBoard;//シーンクラスの生成
	if (pSceneBillboard != NULL)
	{
		pSceneBillboard->m_pos = pos;
		pSceneBillboard->m_fWidth = fWidth;
		pSceneBillboard->m_fHeight = fHeight;

		pSceneBillboard->Init();

	}
	return pSceneBillboard;
}

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CSceneGaugeBillBoard::CSceneGaugeBillBoard(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CSceneGaugeBillBoard::~CSceneGaugeBillBoard()
{

}

//=============================================================================
// 関数名：ビルボードの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CSceneGaugeBillBoard::Init(void)
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
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), -m_fHeight, 0.0f);

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
// 関数名：ビルボードの終了処理
// 関数の概要：自身を破棄する
//=============================================================================
void CSceneGaugeBillBoard::Uninit(void)
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
// 関数名：ビルボードの更新処理
// 関数の概要：--
//=============================================================================
void CSceneGaugeBillBoard::Update(void)
{

}

//=============================================================================
// 関数名：ビルボードの描画処理
// 関数の概要：行列を変えてずっとこっちを見るように
//=============================================================================
void CSceneGaugeBillBoard::Draw(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	D3DXMATRIX pMtxView;
	pMtxView = pCamera->GetmtxView();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//マトリックス情報

	//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	////Zバッファ
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	pDevice->GetTransform(D3DTS_VIEW, &pMtxView);

	m_mtxWorld._11 = pMtxView._11;
	m_mtxWorld._12 = pMtxView._21;
	m_mtxWorld._13 = pMtxView._31;
	m_mtxWorld._21 = pMtxView._12;
	m_mtxWorld._22 = pMtxView._22;
	m_mtxWorld._23 = pMtxView._32;
	m_mtxWorld._31 = pMtxView._13;
	m_mtxWorld._32 = pMtxView._23;
	m_mtxWorld._33 = pMtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, m_pTexture);


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Zバッファ
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);



	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}






//=============================================================================
// 関数名：ビルボードにテクスチャを割り当て
// 関数の概要：m_pTextureにテクスチャを設定する
//=============================================================================
void CSceneGaugeBillBoard::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// 関数名：ビルボードの色を設定
// 関数の概要：ビルボードの色を設定する
//=============================================================================
void CSceneGaugeBillBoard::SetColor(D3DXCOLOR col)
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

//=============================================================================
// 関数名：ポリゴンの幅を設定
// 関数の概要：ポリゴンの幅を設定する
//=============================================================================
void CSceneGaugeBillBoard::SetGaugeWidth(float fWidth)
{
	m_fWidth = fWidth;
}
//=============================================================================
// 関数名：ポリゴンの高さを設定
// 関数の概要：ポリゴンの高さを設定する
//=============================================================================
void CSceneGaugeBillBoard::SetGaugeHeight(float fHeight)
{
	m_fHeight = fHeight;

}
//=============================================================================
// 関数名：現在の値の代入
// 関数の概要：現在の値の代入する
//=============================================================================
void CSceneGaugeBillBoard::SetCurrentValue(float fCurrentValue)
{
	m_fCurrentValue = fCurrentValue;

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	if (m_pVtxBuff != NULL)
	{
		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, -m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(((m_fCurrentValue / m_fMaxValue) * m_fWidth), -m_fHeight, 0.0f);

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 関数名：最大値の代入
// 関数の概要：最大値の代入する
//=============================================================================
void CSceneGaugeBillBoard::SetMaxValue(float fMaxValue)
{
	m_fMaxValue = fMaxValue;
}

//=============================================================================
// 関数名：ポリゴンの位置を設定
// 関数の概要：ポリゴンの位置を設定する
//=============================================================================
void CSceneGaugeBillBoard::SetPosGauge(D3DXVECTOR3 pos)
{
	//位置情報を代入
	m_pos = pos;


}
