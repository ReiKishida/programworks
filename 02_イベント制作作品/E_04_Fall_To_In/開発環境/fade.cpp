//=============================================================================
//
// フェードの処理 [fade.cpp]
// Author : Ryota Kinoshita
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture[TYPE_MAX] = {};			// テクスチャ
CManager::MODE CFade::m_modeNext = CManager::MODE_TITLE;
CFade::FADE CFade::m_fade = CFade::FADE_NONE;
D3DXVECTOR3 CFade::m_pos = D3DXVECTOR3(1.0f, 1.0f, 1.0);		// 座標
D3DXCOLOR CFade::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0, 0.0f);		// 色
float CFade::m_fWidth = 0.0f;//　幅
float CFade::m_fHeight = 0.0f;//　幅
D3DXVECTOR2 CFade::m_uv = D3DXVECTOR2(0.0f, 0.0f);
CFade::TYPE CFade::m_type = CFade::TYPE_NONE;
//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0, 0,0);			// 座標
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0, 0.0f);		// 色
	m_modeNext = CManager::MODE_TITLE;
	m_fade = CFade::FADE_NONE;
	m_uv = D3DXVECTOR2(0.0f, 0.0f);
	m_type = CFade::TYPE_NONE;
}
//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
}
//=============================================================================
// テクスチャ生成
//=============================================================================
HRESULT CFade::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/.png",
		&m_pTexture[TYPE_NONE]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wipe.png",
		&m_pTexture[TYPE_WIPE]);

	return S_OK;
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CFade::UnLoad(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(CManager::MODE modeNext)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_modeNext = modeNext;
	m_type = CFade::TYPE_NONE;

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

	m_pos = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0);			// 座標
	m_fWidth = SCREEN_WIDTH ;
	m_fHeight = SCREEN_HEIGHT ;
	m_uv = D3DXVECTOR2(0.0f, 0.0f);
	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth/2 , -(m_fHeight )/2, 0.0f)+ m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fWidth/2, -(m_fHeight )/2, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth /2, (m_fHeight)/2, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fWidth/2, (m_fHeight)/2, 0.0f) + m_pos;
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
	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[2].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 0.5f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	switch (m_type)
	{
	case TYPE_NONE:
		if (m_fade == FADE_IN)			// フェードイン状態に
		{
			m_col.a -= 0.02f;
			if (m_col.a <= 0.0f)
			{// 透明になったら
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)	// フェードアウト状態に
		{
			m_col.a += 0.02f;
			if (m_col.a >= 1.0f)
			{// 不透明になったら
				m_col.a = 1.0f;
				m_fade = FADE_IN;	// フェードイン状態に
				// モードの設定
				CManager::SetMode(m_modeNext);
				CGame::SetPause(false);
			}
		}
		break;
	case TYPE_WIPE:
		if (m_fade == FADE_IN)			// フェードイン状態に
		{
			m_uv.x += -0.1f;
			m_uv.y += -0.1f;
			if (m_uv.x <= 0.0f)
			{// 透明になったら
				m_col.a = 0.0f;
				m_uv.x = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)	// フェードアウト状態に
		{
			m_uv.x += 0.1f;
			m_uv.y += 0.1f;
			if (m_uv.x >= 5.0f)
			{// 不透明になったら
				m_uv.x = 5.0f;
				m_fade = FADE_IN;	// フェードイン状態に
				// モードの設定
				CManager::SetMode(m_modeNext);
				CGame::SetPause(false);
			}
		}
		break;
	case TYPE_CLOSSFADE:
		if (m_fade == FADE_IN)			// フェードイン状態に
		{
			m_col.a -= 0.01f;
			if (m_col.a <= 0.0f)
			{// 透明になったら
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)	// フェードアウト状態に
		{
			m_col.a += 0.01f;
			if (m_col.a >= 1.0f)
			{// 不透明になったら
				m_col.a = 1.0f;
				m_fade = FADE_IN;	// フェードイン状態に
			}
		}
		break;
	}

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.5f-m_uv.x, 0.5f- m_uv.y);
	pVtx[1].tex = D3DXVECTOR2(0.5f+m_uv.x, 0.5f- m_uv.y);
	pVtx[2].tex = D3DXVECTOR2(0.5f-m_uv.x, 0.5f+ m_uv.y);
	pVtx[3].tex = D3DXVECTOR2(0.5f+m_uv.x, 0.5f+ m_uv.y);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// テクスチャアドレッシング方法(V値)を設定

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[m_type]);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
}
//=============================================================================
// 生成
//=============================================================================
CFade *CFade::Create(CManager::MODE modeNext)
{
	CFade *pCFade = NULL;
	if (pCFade == NULL)
	{// フェード生成
		pCFade = new CFade;
	}
	if (pCFade != NULL)
	{
		// 初期化処理
		if (FAILED(pCFade->Init(modeNext)))
		{
			return NULL;
		}

	}
	return pCFade;
}
//=============================================================================
// フェード
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_type = TYPE_NONE;
	m_fade = CFade::FADE_OUT;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	m_modeNext = modeNext;
}
//=============================================================================
// wipe
//=============================================================================
void CFade::SetWipe(CManager::MODE modeNext)
{
	m_type = TYPE_WIPE;
	m_fade = CFade::FADE_OUT;
	m_uv = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_modeNext = modeNext;
}
//=============================================================================
// なにもない
//=============================================================================
void CFade::SetCloss(void)
{
	m_type = TYPE_CLOSSFADE;
	m_fade = CFade::FADE_OUT;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

}
//=============================================================================
// 取得
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

