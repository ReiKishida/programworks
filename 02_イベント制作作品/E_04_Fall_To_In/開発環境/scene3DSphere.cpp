//=============================================================================
//
// 3Dメッシュスフィアの処理 [scene3DSphere.cpp]
// Author : 亀田 憲尚
//
//=============================================================================
#include "scene3DSphere.h"
#include "manager.h"
#include "renderer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CScene3DSphere *CScene3DSphere::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection)
{
	CScene3DSphere *pScene3DSphere = NULL;

	// クラスの生成
	pScene3DSphere = new CScene3DSphere;

	if (pScene3DSphere == NULL)
	{// 生成できなかった場合
		return NULL;
	}

	// 初期化処理
	if (FAILED(pScene3DSphere->Init()))
	{
		return NULL;
	}
	pScene3DSphere->SetAll(pos, rot, fRadius, fHeight, nCutH, nCutV, col, nDirection);
	pScene3DSphere->MakeVertex();

	return pScene3DSphere;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3DSphere::CScene3DSphere(int nPriority):CScene(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pVtxBuffLid = NULL;
	m_pIdxBuff = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_nCutH = 0;
	m_nCutV = 0;
	m_nDirection = 0;
	m_nNumVtx = 0;
	m_nNumPolygon = 0;
	m_nNumIdx = 0;
	m_texMove = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3DSphere::~CScene3DSphere()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3DSphere::Init()
{
	// 情報を初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_nCutH = 0;
	m_nCutV = 0;
	m_nDirection = 0;
	m_nNumVtx = 0;
	m_nNumPolygon = 0;
	m_nNumIdx = 0;
	m_texMove = D3DXVECTOR2(0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene3DSphere::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 頂点バッファ[蓋]の破棄
	if (m_pVtxBuffLid != NULL)
	{
		m_pVtxBuffLid->Release();
		m_pVtxBuffLid = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 開放処理
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene3DSphere::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3DSphere::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// デバイスの取得
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}
	// アルファテストの処理
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP	// プリミティブの種類
								, 0
								, 0
								, m_nNumVtx				// 頂点数
								, 0
								, m_nNumPolygon);		// 描画するプリミティブの数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D));

	// ポリゴンの描画[蓋]
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN
						, 0						// 開始する頂点のインデックス
						, m_nCutH);				// 描画するプリミティブの数

	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN
						, m_nCutH + 2			// 開始する頂点のインデックス
						, m_nCutH);				// 描画するプリミティブの数
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化

}

//=============================================================================
// テクスチャ割り当て
//=============================================================================
void CScene3DSphere::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void CScene3DSphere::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// デバイスの取得
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_3D
		, D3DPOOL_MANAGED
		, &m_pVtxBuff
		, NULL);

	// 頂点バッファを生成[蓋]
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nCutH + 2) * 2
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_3D
		, D3DPOOL_MANAGED
		, &m_pVtxBuffLid
		, NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx
		, D3DUSAGE_WRITEONLY
		, D3DFMT_INDEX16
		, D3DPOOL_MANAGED
		, &m_pIdxBuff
		, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx, *pVtxLid;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pVtxBuffLid->Lock(0, 0, (void**)&pVtxLid, 0);

	int nCntVtx = 0, nCntVtxLid = 0;
	float fAngleV, fAngleH, fRadius, fHeight;
	for (int nCntV = 0; nCntV < m_nCutV - 1; nCntV++)
	{
		if (nCntV == 0 || nCntV == m_nCutV - 2)
		{// 頂点情報の設定[蓋]
			pVtxLid[nCntVtxLid].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
			if (nCntV == m_nCutV - 2)
			{// 底蓋の場合
				pVtxLid[nCntVtxLid].pos.y *= -1;
			}
			D3DXVec3Normalize(&pVtxLid[nCntVtxLid].nor, &pVtxLid[nCntVtxLid].pos);
			pVtxLid[nCntVtxLid].col = m_col;
			pVtxLid[nCntVtxLid].tex = D3DXVECTOR2(0.0f, 0.0f);
			nCntVtxLid++;
		}

		for (int nCntH = 0; nCntH < m_nCutH + 1; nCntH++, nCntVtx++)
		{
			fAngleH = D3DX_PI * 2.0f / m_nCutH * nCntH;
			if (fAngleH > D3DX_PI)
			{
				fAngleH -= D3DX_PI * 2.0f;
			}

			if (m_nDirection == 1)
			{// 裏向きの場合
				fAngleH *= -1;
			}

			fAngleV = D3DX_PI / m_nCutV + D3DX_PI / m_nCutV * nCntV;
			fRadius = sinf(fAngleV) * m_fRadius;
			fHeight = cosf(fAngleV) * m_fHeight;

			if (nCntV == 0 || nCntV == m_nCutV - 2)
			{// 頂点情報の設定[蓋]
				pVtxLid[nCntVtxLid].pos = D3DXVECTOR3(sinf(fAngleH) * fRadius, fHeight, cosf(fAngleH) * fRadius);
				if (nCntV == m_nCutV - 2)
				{// 底蓋の場合
					pVtxLid[nCntVtxLid].pos.z *= -1;
				}
				D3DXVec3Normalize(&pVtxLid[nCntVtxLid].nor, &pVtxLid[nCntVtxLid].pos);
				pVtxLid[nCntVtxLid].col = m_col;
				pVtxLid[nCntVtxLid].tex = D3DXVECTOR2(0.0f, 1.0f);
				nCntVtxLid++;
			}

			// 頂点情報の設定
			pVtx[nCntVtx].pos = D3DXVECTOR3(sinf(fAngleH) * fRadius, fHeight, cosf(fAngleH) * -fRadius);
			D3DXVec3Normalize(&pVtx[nCntVtx].nor, &pVtx[nCntVtx].pos);
			pVtx[nCntVtx].col = m_col;
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f / m_nCutH * nCntH, 1.0f / m_nCutV * (nCntV - 2));
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	m_pVtxBuffLid->Unlock();

	// インデックスデータの設定
	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	int nNumIdx = m_nCutH + 1;
	for (int nCntIdx = 0; nCntIdx < m_nNumIdx; nCntIdx += 2, nNumIdx++)
	{
		if (nNumIdx % (m_nCutH + 1) == 0 && nNumIdx != m_nCutH + 1)
		{// 折り返し地点の場合
			pIdx[nCntIdx] = nNumIdx - (m_nCutH + 2);
			pIdx[nCntIdx + 1] = nNumIdx;
			nCntIdx += 2;
		}
		pIdx[nCntIdx] = nNumIdx;
		pIdx[nCntIdx + 1] = nNumIdx - (m_nCutH + 1);
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
// 全ての情報の設定
//=============================================================================
void CScene3DSphere::SetAll(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const float fHeight, const int nCutH, const int nCutV, const D3DXCOLOR col, const int nDirection)
{
	// 情報を設定
	m_pos = pos;
	m_rot = rot;
	m_col = col;
	m_fRadius = fRadius;
	m_fHeight = fHeight;
	m_nCutH = nCutH;
	m_nCutV = nCutV;
	m_nDirection = nDirection;
	m_nNumVtx = (m_nCutH + 1) * (m_nCutV - 1);
	m_nNumPolygon = m_nCutH * (m_nCutV - 2) * 2 + (m_nCutV - 3) * 4;
	m_nNumIdx = m_nNumPolygon + 2;
}

//=============================================================================
// 位置の設定
//=============================================================================
void CScene3DSphere::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//=============================================================================
// テクスチャ移動
//=============================================================================
void CScene3DSphere::SetTexMove(const D3DXVECTOR2 tex)
{
	m_texMove = tex;

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

								// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;
	for (int nCntV = 0; nCntV < m_nCutV - 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < m_nCutH + 1; nCntH++, nCntVtx++)
		{
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f / m_nCutH * nCntH, 1.0f / m_nCutV * (nCntV - 2))+ m_texMove;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}