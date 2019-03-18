//=============================================================================
//
// メッシュシリンダー処理 [scene_meshcylinder.cpp]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "scene_meshcylinder.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR (125)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSceneMeshCylinder::m_pTexture = NULL; //共有テクスチャのポインタ

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：クリアな値を代入
//=============================================================================
CSceneMeshCylinder::CSceneMeshCylinder(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：処理の最後に呼び出される
//=============================================================================
CSceneMeshCylinder::~CSceneMeshCylinder()
{

}


//=============================================================================
// 関数名：メッシュシリンダーの生成処理
// 関数の概要：メッシュフィールドを生成する
//=============================================================================
CSceneMeshCylinder *CSceneMeshCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sprit_H, int sprit_V, float fRadius,float fHeight, CYLINDERTYPE type)
{
	CSceneMeshCylinder  *pSceneMeshCylinder;
	pSceneMeshCylinder = new CSceneMeshCylinder;//シーンクラスの生成
	if (pSceneMeshCylinder != NULL)
	{
		pSceneMeshCylinder->m_pos = pos;			//位置情報の代入
		pSceneMeshCylinder->m_rot = rot;			//角度情報の代入
		pSceneMeshCylinder->m_nMesh_H = sprit_H;	//水平方向の分割
		pSceneMeshCylinder->m_nMesh_V = sprit_V;	//垂直方向の分割
		pSceneMeshCylinder->m_fRadius = fRadius;	//半径の代入
		pSceneMeshCylinder->m_fHeight = fHeight;
		pSceneMeshCylinder->m_type = type;
		pSceneMeshCylinder->Init();
	}
	return pSceneMeshCylinder;
}

//=============================================================================
// 関数名：メッシュシリンダーの初期化処理
// 関数の概要：頂点数とポリゴン数に注意
//=============================================================================
HRESULT CSceneMeshCylinder::Init(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点数
	m_nVertexNum = (m_nMesh_H + 1) * (m_nMesh_V + 1);
	//インデックス数
	m_nIndexNum = (m_nMesh_H * m_nMesh_V) * 2 + (4 * (m_nMesh_V - 1)) + 2;
	//ポリゴン数
	m_nPolygonNum = (m_nMesh_H * m_nMesh_V) * 2 + (4 * (m_nMesh_V - 1));

	MakeVertex(pDevice);
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
// 関数名：メッシュシリンダーの終了処理
// 関数の概要：テクスチャ、頂点バッファの破棄
//=============================================================================
void CSceneMeshCylinder::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//自身の破棄
	Release();
}


//=============================================================================
// 関数名：メッシュシリンダーの更新処理
// 関数の概要：--
//=============================================================================
void CSceneMeshCylinder::Update(void)
{
	m_rot.y += 0.02f;

}

//=============================================================================
// 関数名：メッシュシリンダーの描画
// 関数の概要：頂点数とポリゴン数に注意
//=============================================================================
void CSceneMeshCylinder::Draw(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//マトリックス情報

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	 // メッシュシリンダーの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPolygonNum);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
}


//=============================================================================
// 関数名：頂点情報の作成
// 関数の概要：とりあえずは頂点を置くだけ
//=============================================================================
void CSceneMeshCylinder::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / m_nMesh_H;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < m_nMesh_V + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < m_nMesh_H + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * m_fRadius;
			if (nCntLife_H % 2 == 0)
			{
				pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.y = (m_fHeight -10.0f) * nCntLife_V;
			}
			else
			{
				pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.y = m_fHeight * nCntLife_V;

			}
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * m_fRadius;
			//法線の設定
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].col = D3DXCOLOR(1.0f, 0.0, 0.0f, 0.4f);

			//テクスチャ座標
			pVtx[nCntLife_V * (m_nMesh_H + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (m_nMesh_H / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();


	////頂点バッファをロック
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCntMeshCylinder_V = 0; nCntMeshCylinder_V < m_nMesh_V + 1; nCntMeshCylinder_V++)
	//{
	//	for (int nCntMeshCylinder_H = 0; nCntMeshCylinder_H < m_nMesh_H + 1; nCntMeshCylinder_H++)
	//	{
	//		pVtx[0].pos.x = sinf(D3DX_PI * (nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)))) * m_fRadius;
	//		pVtx[0].pos.y = m_fHeight * nCntMeshCylinder_V;
	//		pVtx[0].pos.z = cosf(D3DX_PI * (nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)))) * m_fRadius;
	//		//法線の設定
	//		pVtx[0].nor = D3DXVECTOR3(sinf(-D3DX_PI * (0.25f * nCntMeshCylinder_H)), 0.0f, cosf(D3DX_PI * (0.25f * nCntMeshCylinder_H)));
	//		D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	//		//頂点カラー
	//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	//		//テクスチャ座標
	//		pVtx[0].tex = D3DXVECTOR2(nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)), nCntMeshCylinder_V * -1.0f);


	//		pVtx += 1;
	//	}
	//}
	////頂点バッファをアンロック
	//m_pVtxBuff->Unlock();

}
//=============================================================================
// 関数名：インデックス情報の作成
// 関数の概要：インデックスでポリゴンを生成する
//=============================================================================
void CSceneMeshCylinder::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < m_nMesh_V; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= m_nMesh_H; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (m_nMesh_H + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == m_nMesh_H && nCntIndex_V <= m_nMesh_V)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (m_nMesh_H + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	m_pIdxBuff->Unlock();

}

//=============================================================================
// 関数名：手前面の壁の当たり判定
// 関数の概要：手前面から来たものの当たり判定を行う
//=============================================================================
bool CSceneMeshCylinder::CollisionCylinder(D3DXVECTOR3 *pos)
{
	bool bCollision = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;


	fxpos = (m_pos.x - pos->x) * (m_pos.x - pos->x);
	fzpos = (m_pos.z - pos->z) * (m_pos.z - pos->z);
	fRadius = m_fRadius * m_fRadius;

#ifdef _DEBUG

	//CDebugProc::Print("fxpos: %.1f\n", fxpos);
	//CDebugProc::Print("fzpos: %.1f\n", fzpos);

	//CDebugProc::Print("fRadius: %.1f\n", fRadius);
#endif
	if (fxpos + fzpos <= fRadius)
	{
#ifdef _DEBUG
		//CDebugProc::Print("半径内\n");
#endif

		if (m_type == CYLINDERTYPE_GOAL)
		{
			bCollision = true;
		}
	}

	return bCollision;
}

