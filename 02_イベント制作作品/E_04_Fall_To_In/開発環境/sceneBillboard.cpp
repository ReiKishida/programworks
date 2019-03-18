//=============================================================================
//
// ビルボード処理 [sceneBillboard.h]
// Author : Okabe Kazuki
//
//=============================================================================
#include "sceneBillboard.h"
#include "manager.h"
#include "renderer.h"
//******************************************************************************
// マクロ定義
//******************************************************************************
LPDIRECT3DTEXTURE9	CBillboard::m_pTextureThis = NULL;
bool				CBillboard::m_bZenable = true;
//=============================================================================
// コンストラクタ
//=============================================================================
CBillboard::CBillboard(int nPriority) :CScene(nPriority)
{
	m_pTexture = NULL;							// テクスチャへのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_rot = 0;									// 向き
	m_fWidth = 0;								// 幅
	m_fHeight = 0;								// 高さ
	m_fAngle = 0;								// 角度
	m_fLength = 0;								// 距離
	m_bZenable = true;
	m_VtxType = VTXTYPE_CENTER;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CBillboard::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		"",
		&m_pTextureThis);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CBillboard::UnLoad(void)
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
void CBillboard::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CBillboard::Init(void)
{

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ポリゴンの位置を設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_rot = 0;									// 向き
	m_fWidth = 0;								// 幅
	m_fHeight = 0;								// 高さ
	m_fAngle = 0;								// 角度
	m_fLength = 0;								// 距離
	m_VtxType = VTXTYPE_CENTER;
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CBillboard::Uninit(void)
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
void CBillboard::Update(void)
{
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CBillboard::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;				// 計算用マトリックス

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (GetObjType() == OBJTYPE_CALLOUT&&!m_bZenable)
	{// 吹き出しなら
		// Zバッファを無効に
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	}
	// アルファテストの処理
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// 逆行列(カメラと向きが入る)
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);

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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,2);

	// レンダーステートを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	if (GetObjType() == OBJTYPE_CALLOUT && !m_bZenable)
	{// 吹き出しなら
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}
//=============================================================================
// 生成
//=============================================================================
CBillboard *CBillboard::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CBillboard *pBillboard = NULL;
	if (!CBillboard::OverData())
	{// ビルボード生成
		pBillboard = new CBillboard;
	}
	if (pBillboard != NULL)
	{
		// 初期化処理
		if (FAILED(pBillboard->Init()))
		{
			return NULL;
		}
		pBillboard->SetPos(pos);
		pBillboard->SetSize(fWidth, fHeight);
		pBillboard->SetVtx();
	}
	return pBillboard;
}
//=============================================================================
// 座標設定
//=============================================================================
void CBillboard::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 大きさ設定
//=============================================================================
void CBillboard::SetSize(const float fWidth, const float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fAngle = atan2f(m_fWidth / 2, m_fHeight / 2);
	m_fLength = sqrtf((m_fWidth / 2.0f) * (m_fWidth / 2.0f) + (m_fHeight / 2.0f) * (m_fHeight / 2.0f));
}
//==============================================================================================================================
// 回転設定
//==============================================================================================================================
void CBillboard::SetRot(const float rot)
{
	m_rot = rot;

	// 限界地を超えたら元に戻す
	if (m_rot > D3DX_PI)
	{
		m_rot -= D3DX_PI * 2.0f;
	}
	else if (m_rot < -D3DX_PI)
	{
		m_rot += D3DX_PI * 2.0f;
	}

	CBillboard::SetVtx();
}

//=============================================================================
// 色設定
//=============================================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// 頂点座標
//=============================================================================
void CBillboard::SetVtx(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	switch (m_VtxType)
	{// 頂点座標の設定
	case VTXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(sinf(m_rot - m_fAngle) * m_fLength, cosf(m_rot - m_fAngle) * m_fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_rot + m_fAngle) * m_fLength, cosf(m_rot + m_fAngle) * m_fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(sinf(m_rot - D3DX_PI + m_fAngle) * m_fLength, cosf(m_rot - D3DX_PI + m_fAngle) * m_fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_rot + D3DX_PI - m_fAngle) * m_fLength, cosf(m_rot + D3DX_PI - m_fAngle) * m_fLength, 0.0f);
		break;
	case VTXTYPE_BASE:
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth/2, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth/2, m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth/2, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth/2, 0.0f, 0.0f);
		break;
	case VTXTYPE_LEFTBASE:
		pVtx[0].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);
		break;
	}

	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

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

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// Zバッファ切るかどうか座標
//=============================================================================
void CBillboard::SetZenable(const bool Zenable)
{
	m_bZenable = Zenable;
}
//=============================================================================
// 頂点タイプ
//=============================================================================
void CBillboard::SetVtxType(const VTXTYPE type)
{
	m_VtxType = type;
}