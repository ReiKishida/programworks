////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ３Dポリゴン処理 [scene3D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "line.h"
//*********************************************************************************
// 静的メンバ変数宣言
//*********************************************************************************
LPDIRECT3DTEXTURE9	CScene3D::m_pTextureThis = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority ):CScene(nPriority)
{
	m_pTexture = NULL;								// テクスチャへのポインタ
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// 色
	m_fWidth = 0;							// 幅
	m_fHeight = 0;							// 高さ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転量
	m_fAngle = 0;	// 角度
	m_fLength=0;	// 距離
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CScene3D::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg.jpg",
		&m_pTextureThis);


	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CScene3D::UnLoad(void)
{
	// テクスチャの破棄
	if (m_pTextureThis != NULL)
	{
		m_pTextureThis->Release();
		m_pTextureThis = NULL;
	}
}
//=============================================================================
// テクスチャ割り当て
//=============================================================================
void CScene3D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
HRESULT CScene3D::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// 色
	m_fWidth = 0;								// 幅
	m_fHeight = 0;								// 高さ
	m_fDepth = 0;								// 奥行き
	m_fAngle = 0;								// 角度
	m_fLength = 0;								// 距離
	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// オブジェクトの破棄
	Release();

}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void CScene3D::Update(void)
{

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void CScene3D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

												// アルファテストの処理
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//==============================================================================================================================
// 判定
//==============================================================================================================================
void CScene3D::Collision(D3DXVECTOR3 pos)
{

	D3DXVECTOR3 VecA[4];// メッシュのベクトル
	D3DXVECTOR3 VexC[4];// プレイヤーと頂点のベクトル
	float fVecLine[4];	// 頂点ベクトルをプレイヤーが超えたかどうか
	D3DXVECTOR3 meshPos[4] =
	{
		m_aVtxPos[0],
		m_aVtxPos[1],
		m_aVtxPos[3],
		m_aVtxPos[2],
	};
	VecA[0] = meshPos[0] - meshPos[1];
	VecA[1] = meshPos[1] - meshPos[2];
	VecA[2] = meshPos[2] - meshPos[3];
	VecA[3] = meshPos[3] - meshPos[0];
	int nIn = 0;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{// プレイヤーとメッシュのベクトル
		VexC[nCnt] = (pos - meshPos[nCnt]);

		fVecLine[nCnt] = (VexC[nCnt].z * VecA[nCnt].x) - (VexC[nCnt].x * VecA[nCnt].z);
		if (fVecLine[nCnt] > 0)
		{// 超えた
			nIn++;
		}
	}
	if (nIn == 4)
	{
		// 頂点カラーの設定
		SetVtx();
	}

}
//=============================================================================
// 生成
//=============================================================================
CScene3D *CScene3D::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth)
{
	CScene3D *pScene3D = NULL;
	if (!CScene::OverData())
	{// ２Dポリゴン生成
		pScene3D = new CScene3D;
	}
	if (pScene3D != NULL)
	{
		// 初期化処理
		if (FAILED(pScene3D->Init()))
		{
			return NULL;
		}
		pScene3D->SetPos(pos);
		pScene3D->SetSize(fWidth, fHeight, fDepth);
	//	pScene3D->BindTexture(m_pTextureThis);
		pScene3D->SetVtx();
	}
	return pScene3D;
}
//==============================================================================================================================
// 座標設定
//==============================================================================================================================
void CScene3D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//==============================================================================================================================
// 移動量
//==============================================================================================================================
void CScene3D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}
//==============================================================================================================================
// 大きさ設定
//==============================================================================================================================
void CScene3D::SetSize(const float fWidth, const float fHeight, const float fDepth)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	m_fAngle = atan2f(m_fWidth / 2, m_fDepth / 2);
	m_fLength = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fDepth / 2)* (m_fDepth / 2));
}
//==============================================================================================================================
// 回転設定
//==============================================================================================================================
void CScene3D::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//==============================================================================================================================
// 色設定
//==============================================================================================================================
void CScene3D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}
//==============================================================================================================================
// 頂点設定
//==============================================================================================================================
void CScene3D::SetVtx(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点の設定
	pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ), cosf(+m_fAngle + m_rot.y)*m_fLength) ;
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI - m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ) , cosf(-m_fAngle + m_rot.y)*m_fLength) ;
	pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ) , cosf(D3DX_PI - m_fAngle + m_rot.y)*m_fLength) ;
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle + m_rot.y)*m_fLength,
		(m_fHeight ), cosf(D3DX_PI + m_fAngle + m_rot.y)*m_fLength) ;
	m_aVtxPos[0] = pVtx[0].pos + m_pos;
	m_aVtxPos[1] = pVtx[1].pos + m_pos;
	m_aVtxPos[2] = pVtx[2].pos + m_pos;
	m_aVtxPos[3] = pVtx[3].pos + m_pos;
	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f,-1.0f,0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
