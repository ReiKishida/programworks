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
#include "scene_billboard_anim.h"
#include "camera.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager *CSceneBillBoardAnim::m_pManager = NULL;
//=============================================================================
// 関数名：ビルボードの生成
// 関数の概要：ビルボードを生成する
//=============================================================================
CSceneBillBoardAnim *CSceneBillBoardAnim::Create(D3DXVECTOR3 pos)
{
	CSceneBillBoardAnim  *pSceneBillboard;
	pSceneBillboard = new CSceneBillBoardAnim;//シーンクラスの生成
	if (pSceneBillboard != NULL)
	{
		pSceneBillboard->m_pos = pos;
		pSceneBillboard->m_fWidth = 5.0f;
		pSceneBillboard->m_fHeight = 10.0f;
		pSceneBillboard->m_fRadius = 10.0f;

		pSceneBillboard->Init();

	}
	return pSceneBillboard;
}

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CSceneBillBoardAnim::CSceneBillBoardAnim(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
										  //m_pTexture = NULL;
										  //m_pVtxBuff = NULL;
	m_nNumSceneBillBoard++;
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CSceneBillBoardAnim::~CSceneBillBoardAnim()
{

}

//=============================================================================
// 関数名：ビルボードの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CSceneBillBoardAnim::Init(void)
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
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.0f);

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
void CSceneBillBoardAnim::Uninit(void)
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
void CSceneBillBoardAnim::Update(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;


	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//=============================================================================
// 関数名：ビルボードの描画処理
// 関数の概要：行列を変えてずっとこっちを見るように
//=============================================================================
void CSceneBillBoardAnim::Draw(void)
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


	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//=============================================================================
// 関数名：エフェクト描画処理
// 関数の概要：アルファブレンドの設定
//=============================================================================
void CSceneBillBoardAnim::DrawEffect(void)
{

	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//マトリックス情報

								//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	D3DXMATRIX pMtxView;
	pMtxView = pCamera->GetmtxView();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);




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
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 関数名：ビルボードの総数を取得
// 関数の概要：ビルボードの総数を返す
//=============================================================================
int CSceneBillBoardAnim::GetNumSceneNillBoardAll(void)
{
	return m_nNumSceneBillBoard;
}

//=============================================================================
// 関数名：ビルボードの位置を取得
// 関数の概要：ビルボードの位置を返す
//=============================================================================
D3DXVECTOR3 CSceneBillBoardAnim::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 関数名：ビルボードの位置を設定
// 関数の概要：ビルボードの位置を設定する
//=============================================================================
void CSceneBillBoardAnim::SetPos(D3DXVECTOR3 pos)
{


	//位置情報を代入
	m_pos = pos;

}

//=============================================================================
// 関数名：ビルボードの角度を取得
// 関数の概要：ビルボードの角度を返す
//=============================================================================
D3DXVECTOR3 CSceneBillBoardAnim::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// 関数名：ビルボードの角度を設定
// 関数の概要：ビルボードの角度を設定する
//=============================================================================
void CSceneBillBoardAnim::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;


}

void CSceneBillBoardAnim::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
// 関数名：ビルボードにテクスチャを割り当て
// 関数の概要：m_pTextureにテクスチャを設定する
//=============================================================================
void CSceneBillBoardAnim::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// 関数名：ビルボードの色を設定
// 関数の概要：ビルボードの色を設定する
//=============================================================================
void CSceneBillBoardAnim::SetColor(D3DXCOLOR col)
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
// 関数名：ビルボードのテクスチャ設定
// 関数の概要：テクスチャ座標を設定する
//=============================================================================
void CSceneBillBoardAnim::SetTexture(float fTex_X, float fTex_Y)
{
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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
//=============================================================================
// 関数名：ビルボードのテクスチャ設定
// 関数の概要：テクスチャ座標を設定する
//=============================================================================
void CSceneBillBoardAnim::SetTextureAnim(float fTex_X, int nPatternAnim)
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
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


}