//=============================================================================
//
// 2Dオブジェクト処理 [scene.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
//*********************************************************************************
// 静的メンバ変数宣言
//*********************************************************************************
LPDIRECT3DTEXTURE9	CScene2D::m_pTextureThis = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority):CScene(nPriority)
{
	m_pTexture = NULL;								// テクスチャへのポインタ
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);		// 位置
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_fWidth = 0;							// 幅
	m_fHeight = 0;							// 高さ
	m_fAngle[0] = 0;						// 角度
	m_fLength[0] = 0;						// 距離
	m_fAngle[1] = 0;						// 角度
	m_fLength[1] = 0;						// 距離
	m_fRot = 0;								// 回転量
	m_VtxType = VTXTYPE_CENTER;				// 中心点
	m_bUse = true;							// 使用されているかどうか
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CScene2D::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/CatFox.png",
		&m_pTextureThis);

	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CScene2D::UnLoad(void)
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
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// ポリゴンの位置を設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_fWidth = 0;
	m_fHeight = 0;
	m_fAngle[0] = 0;						// 角度
	m_fLength[0] = 0;						// 距離
	m_fAngle[1] = 0;						// 角度
	m_fLength[1] = 0;						// 距離
	m_fRot = 0;
	m_VtxType = VTXTYPE_CENTER;				// 中心点
	m_bUse = true;							// 使用されているかどうか

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(-(m_fWidth / 2), -(m_fHeight / 2), 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3( (m_fWidth / 2), -(m_fHeight / 2), 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-(m_fWidth / 2),  (m_fHeight / 2), 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3( (m_fWidth / 2),  (m_fHeight / 2), 0.0f) + m_pos;
	// rhwの設定
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
	}
	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CScene2D::Uninit(void)
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

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	if (!m_bUse)return;
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}
//=============================================================================
// 生成
//=============================================================================
CScene2D *CScene2D::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CScene2D *pScene2D = NULL;
	if (!CScene::OverData())
	{// ２Dポリゴン生成
		pScene2D = new CScene2D;
	}
	if (pScene2D != NULL)
	{
		// 初期化処理
		if (FAILED(pScene2D->Init()))
		{
			return NULL;
		}
		pScene2D->SetPos(pos);
		pScene2D->SetSize(fWidth, fHeight);
		pScene2D->BindTexture(m_pTextureThis);
		pScene2D->SetVtx();
	}
	return pScene2D;
}
//=============================================================================
// 座標設定
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_posOld = m_pos;
	m_pos = pos;
}
//=============================================================================
// 初期座標設定
//=============================================================================
void CScene2D::SetInitPos(D3DXVECTOR3 pos)
{
	m_Initpos = pos;
}
//=============================================================================
// 大きさ設定
//=============================================================================
void CScene2D::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;		// 幅
	m_fHeight = fHeight;	// 高さ
	switch (m_VtxType)
	{// 中心点の位置ごとに計算を変える
	case VTXTYPE_CENTER:
		m_fAngle[0] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));
		m_fAngle[1] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[1] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));
		break;
	case VTXTYPE_BASE:
		// 01 頂点の長さ回転
		m_fAngle[0] = atan2f((m_fWidth / 2), (-m_fHeight));
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight)*(m_fHeight));
		// 23 頂点の長さ回転
		m_fAngle[1] = atan2f((m_fWidth / 2), (0));
		m_fLength[1] = m_fWidth / 2;
		break;
	case VTXTYPE_LEFT:
		break;
	case VTXTYPE_RIGHT:
		break;
	}
}
//=============================================================================
// 回転設定
//=============================================================================
void CScene2D::SetRot(float fRot)
{
  	m_fRot = fRot;

}
//=============================================================================
// 色設定
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// テクスチャ座標設定[パターン]
//=============================================================================
void CScene2D::SetPattern(const D3DXVECTOR2 uv, const int nPatternX, const int nPatternY, const bool bTurn)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 1.0f : 0.0f)), uv.y * nPatternY);
	pVtx[1].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 0.0f : 1.0f)), uv.y * nPatternY);
	pVtx[2].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 1.0f : 0.0f)), uv.y * (nPatternY + 1));
	pVtx[3].tex = D3DXVECTOR2(uv.x * (nPatternX + (bTurn ? 0.0f : 1.0f)), uv.y * (nPatternY + 1));

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// テクスチャ座標設定
//=============================================================================
void CScene2D::SetTex(D3DXVECTOR2 tex)
{
	m_tex = tex;
	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f) + m_tex;
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f) + m_tex;
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f) + m_tex;
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f) + m_tex;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


}
//=============================================================================
// 頂点座標
//=============================================================================
void CScene2D::SetVtx(void)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_VtxType)
	{// 中心点の位置ごとに計算を変える
	case VTXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;

		break;
	case VTXTYPE_BASE:
		pVtx[0].pos = D3DXVECTOR3(sinf( - m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(-m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		break;

	case VTXTYPE_LEFT:
		pVtx[0].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		break;
	case VTXTYPE_RIGHT:
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;

		break;
	}

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 頂点設定
//=============================================================================
void CScene2D::SetVtxType(VTXTYPE VtxType)
{
	m_VtxType = VtxType;
}
//=============================================================================
// 使用されているかどうか
//=============================================================================
void CScene2D::SetUse(const bool bUse)
{
	m_bUse = bUse;
}
void CScene2D::SetStamp(const float fWidth, const float fHeight)
{
	m_fWidth += (fWidth - m_fWidth)*0.3f;
	m_fHeight += (fHeight - m_fHeight)*0.3f;
	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	switch (m_VtxType)
	{// 中心点の位置ごとに計算を変える
	case VTXTYPE_CENTER:
		m_fAngle[0] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));
		m_fAngle[1] = atan2f(m_fWidth / 2, m_fHeight / 2);
		m_fLength[1] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight / 2)* (m_fHeight / 2));

		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI + m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(D3DX_PI - m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;

		break;
	case VTXTYPE_BASE:
		// 01 頂点の長さ回転
		m_fAngle[0] = atan2f((m_fWidth / 2), (-m_fHeight));
		m_fLength[0] = sqrtf((m_fWidth / 2)*(m_fWidth / 2) + (m_fHeight)*(m_fHeight));
		// 23 頂点の長さ回転
		m_fAngle[1] = atan2f((m_fWidth / 2), (0));
		m_fLength[1] = m_fWidth / 2;

		pVtx[0].pos = D3DXVECTOR3(sinf(-m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(-m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle[0] + m_fRot)*m_fLength[0],
			cosf(m_fAngle[0] + m_fRot)*m_fLength[0], 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(sinf(-m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(-m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle[1] + m_fRot)*m_fLength[1],
			cosf(m_fAngle[1] + m_fRot)*m_fLength[1], 0.0f) + m_pos;
		break;

	case VTXTYPE_LEFT:
		pVtx[0].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		break;
	case VTXTYPE_RIGHT:
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[1].pos = D3DXVECTOR3(0.0f, -m_fHeight / 2, 0.0f) + m_pos;
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, m_fHeight / 2, 0.0f) + m_pos;
		pVtx[3].pos = D3DXVECTOR3(0.0f, m_fHeight / 2, 0.0f) + m_pos;

		break;
	}
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}