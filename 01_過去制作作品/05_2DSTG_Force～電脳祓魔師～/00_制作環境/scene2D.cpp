//=============================================================================
//
// シーン処理[2Dポリゴン] [scene2D.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "explosion.h"
#include "score.h"
#include "timer.h"
#include "sound.h"
#include "game.h"
#include "force.h"
#include "enemy.h"
#include "bg.h"
#include "particle.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_WIDTH (30) //ポリゴンのXの大きさ
#define POLYGON_HEIGHT (30) //ポリゴンのYの大きさ
#define ANIMATION_SPEED (8)	//アニメーションのスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (1.0f)	//テクスチャ座標_Y

//*****************************************************************************
// グローバル変数
//*****************************************************************************
bool g_bDelete = false;
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager *CScene2D::m_pManager = NULL;
int CScene2D::m_nDamage = 0;

//=============================================================================
//2Dシーンの生成
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos)
{
	CScene2D  *pScene2D;
	pScene2D = new CScene2D;//シーンクラスの生成
	if (pScene2D != NULL)
	{
		pScene2D->Init(pos);

	}
	return pScene2D;
}

//=============================================================================
//コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority) :CScene(nPriority)
{
	m_rotScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_posScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色
	m_fScene2DWidth = 0.0f;	//ポリゴンの幅
	m_fScene2DHeight = 0.0f;	//ポリゴンの高さ
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_nNumScene2D++;
}

//=============================================================================
//デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	//フレーム数のカウントの初期化
	m_nCntframe = 0;
	//DPS初期化
	m_nDPS = 0;
	if (m_pManager == NULL)
	{
		m_pManager = new CManager;
	}
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	//位置、回転情報初期化
	m_rotScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posScene2D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//色情報の初期化
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ポリゴンの位置を設定
	m_posScene2D = pos;



	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	m_fLengthScene2D = sqrtf((m_fScene2DWidth / 2 + m_fScene2DWidth / 2) * (m_fScene2DHeight / 2 + m_fScene2DHeight / 2));

	m_fAngleScene2D = atan2f(m_fScene2DWidth / 2, m_fScene2DHeight / 2);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	pVtx[1].pos = D3DXVECTOR3(m_posScene2D.x + sinf(D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	pVtx[2].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	pVtx[3].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);


	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();



	return S_OK;


	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CScene2D::Uninit(void)
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
//更新処理
//=============================================================================
void CScene2D::Update(void)
{

	int m_nCntframe = CForce::GetnCnt();
	if (m_nCntframe % 60 == 0)
	{
		CheckDamage();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	if (m_pManager == NULL)
	{
		m_pManager = new CManager;
	}
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わなくても書く！)
	pDevice->SetTexture(0, m_pTexture);


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);



}
//=============================================================================
//エフェクト描画処理
//=============================================================================
void CScene2D::DrawEffect(void)
{
	if (m_pManager == NULL)
	{
		m_pManager = new CManager;
	}
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// デバイスの取得
	pDevice = pRenderer->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わなくても書く！)
	pDevice->SetTexture(0, m_pTexture);


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
//2Dシーンの総数の取得
//=============================================================================
int CScene2D::GetNum2DSceneAll(void)
{
	return m_nNumScene2D;
}

//=============================================================================
//ポリゴンの位置の取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosScene2D(void)
{
	return m_posScene2D;
}

//=============================================================================
//ポリゴンの位置の設定
//=============================================================================
void CScene2D::SetPosScene2D(D3DXVECTOR3 pos)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//位置情報を代入
	m_posScene2D = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posScene2D.x + sinf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posScene2D.x + sinf(m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンの角度の取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetRotScene2D(void)
{
	return m_rotScene2D;
}

//=============================================================================
//ポリゴンの角度の設定
//=============================================================================
void CScene2D::SetRotScene2D(D3DXVECTOR3 rot)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	m_rotScene2D = rot;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-D3DX_PI + m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posScene2D.x + sinf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(D3DX_PI - m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posScene2D.x + sinf(-m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(-m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posScene2D.x + sinf(m_fAngleScene2D + m_rotScene2D.x) * m_fLengthScene2D,
		m_posScene2D.y + cosf(m_fAngleScene2D + m_rotScene2D.y) * m_fLengthScene2D,
		0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンの長さの取得
//=============================================================================
float CScene2D::GetLengthScene2D(void)
{
	return m_fLengthScene2D;
}

//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CScene2D::SetLengthScene2D(float fLength)
{
	m_fLengthScene2D = fLength;
}

//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CScene2D::SetScene2DWidth(float fWidth)
{
	m_fScene2DWidth = fWidth;
}
//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CScene2D::SetScene2DHeight(float fHeight)
{
	m_fScene2DHeight = fHeight;

}
//=============================================================================
//テクスチャの割り当て
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}


//=============================================================================
//テクスチャの設定
//=============================================================================
void CScene2D::SetTexture(float fTex_X, float fTex_Y)
{
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

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
//テクスチャの設定(4頂点分)
//=============================================================================
void CScene2D::SetTexture_All(float fTex_X, float fTex_X_0, float fTex_Y_0, float fTex_Y)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(fTex_X_0, fTex_Y_0);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, fTex_Y_0);
	pVtx[2].tex = D3DXVECTOR2(fTex_X_0, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
//テクスチャアニメーションの設定
//=============================================================================
void CScene2D::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
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
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
//テクスチャアニメーションの設定
//=============================================================================
void CScene2D::SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y)
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
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================================================
//背景スクロール
//=============================================================================================================
void CScene2D::ScrollBg(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = m_pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = Scroll_0;
	pVtx[1].tex = Scroll_1;
	pVtx[2].tex = Scroll_2;
	pVtx[3].tex = Scroll_3;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//============================================================================================
//色の設定
//============================================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	//色情報を代入
	m_color = col;

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================================================
//色の取得
//============================================================================================
D3DXCOLOR CScene2D::GetColor(void)
{
	return m_color;
}

//============================================================================================
//優先順位の設定
//============================================================================================
void CScene2D::SetPriority(int nPriority)
{
	m_nPriority = nPriority;
}
//============================================================================================
//幅の取得
//============================================================================================
float CScene2D::GetWidthScene2D(void)
{
	return m_fScene2DWidth;
}
//============================================================================================
//幅の取得
//============================================================================================
float CScene2D::GetHeightScene2D(void)
{
	return m_fScene2DHeight;

}

//============================================================================================
//ライフの取得
//============================================================================================
int CScene2D::GetLife(void)
{
	return m_nLife;
}

//============================================================================================
//ライフの設定
//============================================================================================
void CScene2D::SetLife(int nLife)
{
	m_nLife = nLife;
}


//============================================================================================
// DPSチェック
//============================================================================================
void  CScene2D::CheckDamage()
{
	CForce::CheckDamage(m_nDamage);
	m_nDamage = 0;
}


//============================================================================================
// ダメージ量の取得
//============================================================================================
int CScene2D::GetDamage(void)
{
	return m_nDamage;
}


//============================================================================================
//ヒットライフ
//============================================================================================
void CScene2D::HitLife(int nDamage)
{

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//DPS算出用
	m_nDamage += nDamage;


	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE::MODE_GAME)
	{
		//ライフ減少
		m_nLife -= nDamage;
	}

	//パーティクルの生成
	for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
	{
		CParticle::Create(m_posScene2D, D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f), CParticle::PARITCLETYPE_HIT, 35);
	}
	for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
	{
		CParticle::Create(m_posScene2D, D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f), CParticle::PARITCLETYPE_HIT_R, 35);
	}

}


//============================================================================================
//フォースのテクスチャ設定
//============================================================================================
void CScene2D::SetForceTexture(int nType)
{
	m_nType = nType;

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	 // 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * m_nType), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * m_nType), 1.0f);
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================================================
//フォースの設定
//============================================================================================
void CScene2D::SetForce(int nType)
{
	m_nType = nType;


}


//============================================================================================
//フォースの取得
//============================================================================================
int CScene2D::GetForce()
{
	return m_nType;
}

