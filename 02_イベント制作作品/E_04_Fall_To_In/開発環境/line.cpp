////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 判定用ラインの処理 [libe.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "line.h"
#include "manager.h"
#include "renderer.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
bool CLine::m_bUse = false;
//=============================================================================
// コンストラクタ
//=============================================================================
CLine::CLine(int nPriority ):CScene(PRIORITY_LINE)
{
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
	m_posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最初の位置
	m_posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最後の位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 速さ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_bUse = false;								// 使用されているかどうか]

}

//=============================================================================
// デストラクタ
//=============================================================================
CLine::~CLine()
{

}
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
HRESULT CLine::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 色
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 速さ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_bUse = true;								// 使用されているかどうか]
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx[0].pos = m_posStart;
	pVtx[1].pos = m_posEnd;
	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void CLine::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void CLine::Update(void)
{
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void CLine::Draw(void)
{
	if (!m_bUse)return;
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビルボード用マトリックス

	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, 0);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}
//=============================================================================
// 生成
//=============================================================================
CLine *CLine::Create(const D3DXVECTOR3 posStart, const D3DXVECTOR3 posEnd)
{
	CLine *pLine = NULL;
	if (!CScene::OverData())
	{// ２Dポリゴン生成
		pLine = new CLine;
	}
	if (pLine != NULL)
	{
		pLine->m_posStart = posStart;
		pLine->m_posEnd = posEnd;

		// 初期化処理
		if (FAILED(pLine->Init()))
		{
			return NULL;
		}
	}
	return pLine;
}
//=============================================================================
// 座標設定
//=============================================================================
void CLine::SetPos(const D3DXVECTOR3 start, const D3DXVECTOR3 end)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	m_posStart = start;
	m_posEnd = end;
	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx[0].pos = m_posStart;
	pVtx[1].pos = m_posEnd;
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 表示非表示
//=============================================================================
void CLine::SetDisp(bool bDisp)
{
	m_bUse = bDisp;
}