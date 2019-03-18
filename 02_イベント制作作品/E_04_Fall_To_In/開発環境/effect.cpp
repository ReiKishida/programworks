//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 亀田 憲尚
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	EFFECT_TEX_NAME1	"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名 1
#define	EFFECT_TEX_NAME2	"data/TEXTURE/effect001.png"	// 読み込むテクスチャファイル名 2
#define	EFFECT_TEX_NAME3	"data/TEXTURE/Explosion000.png"	// 読み込むテクスチャファイル名 2
#define	EFFECT_TEX_NAME4	"data/TEXTURE/ink.jpg"	// 読み込むテクスチャファイル名 2

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSceneEffect::m_apTexture[CSceneEffect::TYPE_MAX] = {};
CSceneEffect *CEffect::m_apEffect[MAX_EFFECT] = {};

//=============================================================================
// テクスチャの生成処理
//=============================================================================
HRESULT CSceneEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// デバイスの取得
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (pDevice == NULL)
	{// 取得失敗した場合
		return E_FAIL;

	}

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME1, &m_apTexture[TYPE_CIRCLE]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME2, &m_apTexture[TYPE_SHINY]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME3, &m_apTexture[TYPE_EXPLOSION]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEX_NAME4, &m_apTexture[TYPE_INK]);

	return S_OK;
}

//=============================================================================
// テクスチャの開放処理
//=============================================================================
void CSceneEffect::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneEffect::CSceneEffect()
{
	m_pVtxBuff = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0.0f;
	m_fRadiusInit = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nLife = 0;
	m_fRot = 0.0f;
	m_fAlpha = 0.0f;
	m_fLength = 0.0f;
	m_type = TYPE_CIRCLE;
	m_scaleType = SCALETYPE_NONE;
	m_fTrans = 0.0f;
	m_fAlpha = 0.0f;
	m_nCounter = 0;
	m_bUse = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneEffect::~CSceneEffect()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneEffect::Init(void)
{
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();

	// デバイスの取得
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (pDevice == NULL)
	{// 取得失敗した場合
		return E_FAIL;
	}

	// 情報の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0.0f;
	m_fRadiusInit = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nLife = 0;
	m_fRot = 0.0f;
	m_fAlpha = 0.0f;
	m_fLength = 0.0f;
	m_type = TYPE_CIRCLE;
	m_scaleType = SCALETYPE_NONE;
	m_fTrans = 0.0f;
	m_fAlpha = 0.0f;
	m_nCounter = 0;
	m_bUse = false;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_3D
		, D3DPOOL_MANAGED
		, &m_pVtxBuff
		, NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.0f);

	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
// 終了処理
//=============================================================================
void CSceneEffect::Uninit(void)
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
void CSceneEffect::Update(void)
{
	// 使用されていない場合処理をしない
	if (!m_bUse) return;

	// 位置更新
	m_pos += m_move;

	switch (m_scaleType)
	{// サイズ変化の種類ごとの処理
	case SCALETYPE_NONE:
		break;
	case SCALETYPE_UP:
		m_fRadius += m_fTrans / m_nLife;
		break;
	case SCALETYPE_DOWN:
		m_fRadius -= m_fTrans / m_nLife;
		break;
	case SCALETYPE_UPDOWN:
		m_nCounter++;
		m_fRadius = sinf(D3DX_PI / 15 * m_nCounter) * m_fTrans + m_fRadiusInit;
		break;
	default:
		break;
	}

	if (m_nLife > 0)
	{// 寿命がある場合
		m_col.a -= m_fAlpha;	// 徐々に透明に
		m_nLife--;
		if (m_nLife <= 0)
		{// 表示時間が無くなった場合
			m_bUse = false;
		}
	}
	switch (m_type)
	{
	case TYPE_EXPLOSION:
		if (m_move.x + m_move.z <= 0)
		{
			m_fRot += -0.1f;
		}
		else
		{
			m_fRot += 0.1f;
		}
		m_move.x *= 0.97f;
		m_move.z *= 0.97f;
		m_move.y += 0.01f;
		break;
	}

	// 動いていない場合
	if (m_scaleType == SCALETYPE_NONE && m_nLife <= 0) return;

	VERTEX_3D *pVtx;
	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(sinf(m_fRot - m_fAngle) * m_fLength, cosf(m_fRot - m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fRot + m_fAngle) * m_fLength, cosf(m_fRot + m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(sinf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, cosf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, cosf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, 0.0f);

	// 頂点カラーの更新
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
void CSceneEffect::Draw(void)
{
	if (!m_bUse) return;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();
	D3DXMATRIX mtxView, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// 逆行列を設定
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
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_apTexture[m_type]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP	// プリミティブの種類
							, 0					// 開始する頂点のインデックス
							, 2);				// 描画するプリミティブの数

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// エフェクトの設定
//=============================================================================
void CSceneEffect::SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const TYPE type, const SCALETYPE scaleType, const float fTrans)
{
	m_pos = pos;
	m_move = move;
	m_fRadius = fRadius;
	m_fRadiusInit = fRadius;
	m_col = col;
	m_nLife = nLife;
	m_fRot = fRot;
	m_fAngle = atan2f(m_fRadius / 2.0f, m_fRadius / 2.0f);
	m_fLength = sqrtf((m_fRadius / 2.0f) * (m_fRadius / 2.0f) + (m_fRadius / 2.0f) * (m_fRadius / 2.0f));
	m_type = type;
	m_scaleType = scaleType;
	m_fTrans = fTrans;
	m_fAlpha = 0.0f;
	m_nCounter = 0;

	if (m_nLife != 0)
	{// ゼロ割回避
		m_fAlpha = m_col.a / m_nLife;
	}

	VERTEX_3D *pVtx;
	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(sinf(m_fRot - m_fAngle) * m_fLength, cosf(m_fRot - m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fRot + m_fAngle) * m_fLength, cosf(m_fRot + m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(sinf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, cosf(m_fRot - D3DX_PI + m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, cosf(m_fRot + D3DX_PI - m_fAngle) * m_fLength, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	m_bUse = true;
}

//=============================================================================
// 生成処理
//=============================================================================
CEffect *CEffect::Create(void)
{
	CEffect *pEffect = NULL;

	// クラスの生成
	pEffect = new CEffect;

	if (pEffect == NULL)
	{// 生成できなかった場合
		return NULL;
	}

	// 初期化処理
	if (FAILED(pEffect->Init()))
	{
		return NULL;
	}

	return pEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect() : CScene(PRIORITY_EFFECT)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		m_apEffect[nCntEffect] =NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] != NULL) continue;

		m_apEffect[nCntEffect] = new CSceneEffect;

		if (m_apEffect[nCntEffect] == NULL)
		{// 生成できなかった場合
			return E_FAIL;
		}

		// 初期化処理
		if (FAILED(m_apEffect[nCntEffect]->Init()))
		{
			return E_FAIL;
		}
	}

	// 種類の設定
	SetObjType(OBJTYPE_EFFECT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	// エフェクトクラスの終了処理
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] != NULL)
		{
			m_apEffect[nCntEffect]->Uninit();
			delete m_apEffect[nCntEffect];
			m_apEffect[nCntEffect] = NULL;
		}
	}

	// このクラスの開放処理
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	// エフェクトクラスの更新処理
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] == NULL) continue;

		m_apEffect[nCntEffect]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	// エフェクトクラスの描画処理
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] == NULL) continue;

		m_apEffect[nCntEffect]->Draw();
	}
}

//=============================================================================
// エフェクトの設定
//=============================================================================
void CEffect::SetEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius, const D3DXCOLOR col, const int nLife, const float fRot, const CSceneEffect::TYPE type, const CSceneEffect::SCALETYPE scaleType, const float fTrans)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (m_apEffect[nCntEffect] == NULL) continue;

		if (!m_apEffect[nCntEffect]->GetUse())
		{// 未使用エフェクトの場合
			m_apEffect[nCntEffect]->SetEffect(pos, move, fRadius, col, nLife, fRot, type, scaleType, fTrans);
			break;
		}
	}
}
//=============================================================================
// 爆発
//=============================================================================
void CEffect::SetExplosion(const D3DXVECTOR3 pos, const int nPlayerID)
{
	//	CManager::GetSound()->PlaySound(0, CSound::LABEL_SE_EXOLOSION);

	for (int nCnt = 0; nCnt < 30; nCnt++)
	{
		float fMove = CManager::GetRandBetWeen(2, 8);
		float fRadius = CManager::GetRandBetWeen(30, 50);
		SetEffect(pos, D3DXVECTOR3(sinf(CManager::GetRandomPI()) * fMove, rand()%20/10.0f, cosf(CManager::GetRandomPI()) * fMove), fRadius,
			D3DXCOLOR(1.0f, rand()%100/100.0f, 0.0f, 1.0f),
			120, 0.0f, CSceneEffect::TYPE_EXPLOSION, CSceneEffect::SCALETYPE_UP, 10);
	}
}