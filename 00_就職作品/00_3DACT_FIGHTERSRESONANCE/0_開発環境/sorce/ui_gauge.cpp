//=============================================================================
//
// ゲージ処理[2Dポリゴン] [ui_gauge.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene_gauge.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "ui_gauge.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUiGauge::m_apTexture[MAX_GAUGE_TEX] = {};
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CUiGauge::CUiGauge(int nPriority, OBJTYPE type) : CSceneGauge(nPriority, type)
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CUiGauge::~CUiGauge()
{

}

//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CUiGauge::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/1_lifegauge.png", &m_apTexture[0]);		// ライフゲージ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/face_P.png", &m_apTexture[1]);		// ライフゲージ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/face_B.png", &m_apTexture[2]);		// ライフゲージ

	return S_OK;

}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CUiGauge::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_GAUGE_TEX; nCntTex++)
	{
		//テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
// 関数名：ゲージの生成
// 関数の概要：ゲージを生成する
//=============================================================================
CUiGauge *CUiGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type)
{
	CUiGauge  *pSceneGauge;
	pSceneGauge = new CUiGauge;	//シーンクラスの生成
	if (pSceneGauge != NULL)
	{
		pSceneGauge->m_pos = pos;						//位置情報の代入
		pSceneGauge->m_fWidth = fWidth;					//幅の代入
		pSceneGauge->m_fHeight = fHeight;				//高さの代入
		pSceneGauge->m_fCurrentGauge = fCurrentGauge;	//現在の値の代入
		pSceneGauge->m_fMaxGauge = fMaxGauge;			//最大値の代入
		pSceneGauge->m_col = col;						//ゲージの色情報
		pSceneGauge->m_type = type;						//ゲージの種類の代入
		pSceneGauge->Init();
	}
	return pSceneGauge;
}


//=============================================================================
// 関数名：ゲージの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CUiGauge::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//ゲージの幅の代入
	SetGaugeWidth(m_fWidth);
	//ゲージの高さの代入
	SetGaugeHeight(m_fHeight);

	//現在の値の代入
	SetCurrentValue(m_fCurrentGauge);
	//最大値の代入
	SetMaxValue(m_fMaxGauge);

	if (m_type == GAUGETYPE_UNDER)
	{
		//テクスチャの割り当て
		BindTexture(m_apTexture[0]);
	}
	if (m_type == GAUGETYPE_FACE_PLAYER)
	{
		//テクスチャの割り当て
		BindTexture(m_apTexture[1]);
	}
	if (m_type == GAUGETYPE_FACE_BOSS)
	{
		//テクスチャの割り当て
		BindTexture(m_apTexture[2]);
	}

	//ゲージの初期化
	CSceneGauge::Init();
	//色情報の代入
	SetColor(m_col);


	//位置情報の代入
	SetPosGauge(m_pos);

	return S_OK;
}

//=============================================================================
// 関数名：ゲージの終了処理
// 関数の概要：テクスチャ、頂点バッファの破棄、自身の破棄
//=============================================================================
void CUiGauge::Uninit(void)
{
	CSceneGauge::Uninit();
}

//=============================================================================
// 関数名：ゲージの更新処理
// 関数の概要：--
//=============================================================================
void CUiGauge::Update(void)
{
	//位置情報の取得
	D3DXVECTOR3 pos = GetPosGauge();

	//フレームの加算
	m_nCntFrame++;
	if (m_type == GAUGETYPE_SKILL)
	{
		m_nCntFrame = m_nCntFrame % 80;
	}
	if (m_type == GAUGETYPE_GREEN)
	{
		m_nCntFrame = m_nCntFrame % 40;
	}

	if (m_type == GAUGETYPE_GREEN || m_type == GAUGETYPE_RED)
	{
		if (m_fCurrentGauge > m_fDestValue)
		{//目的のゲージ
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;
			}
			SetCurrentValue(m_fCurrentGauge);
		}
	}
	//スキルゲージ
	if (m_type == GAUGETYPE_SKILL)
	{
		if (m_fCurrentGauge < m_fDestValue)
		{//目的のゲージ
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;

			}
			SetCurrentValue(m_fCurrentGauge);
		}
		if (m_fCurrentGauge > m_fDestValue)
		{//目的のゲージ
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				m_fCurrentGauge = 0.0f;
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = m_fMaxGauge;

			}
			SetCurrentValue(m_fCurrentGauge);
		}
		if (m_fCurrentGauge >= m_fMaxGauge -0.2f)
		{
			if (m_nCntFrame < 40)
			{
				m_col.a -= 0.01f;
				if (m_col.a <= 0.0f)
				{
					m_col.a = 0.0f;
				}
			}
			if (m_nCntFrame > 40)
			{
				m_col.a += 0.01f;
				if (m_col.a >= 1.0f)
				{
					m_col.a = 1.0f;
				}
			}
		}
	}

	if (m_type == GAUGETYPE_GREEN)
	{
		if (m_fCurrentGauge <= 120.0f)
		{
			if (m_nCntFrame < 20)
			{
				m_col.a -= 0.01f;
				if (m_col.a <= 0.0f)
				{
					m_col.a = 0.0f;
				}
			}
			if (m_nCntFrame > 20)
			{
				m_col.a += 0.01f;
				if (m_col.a >= 1.0f)
				{
					m_col.a = 1.0f;
				}
			}
		}

	}

	//位置情報の設定
	SetPosGauge(m_pos);

	//色情報の設定
	SetColor(m_col);


#ifdef _DEBUG
	//if (m_type == GAUGETYPE_GREEN)
	//{//緑ゲージのとき
	//	CDebugProc::Print("GAUGETYPE_GREEN\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);

	//}
	//if (m_type == GAUGETYPE_RED)
	//{//赤ゲージのとき
	//	CDebugProc::Print("\n");
	//	CDebugProc::Print("GAUGETYPE_RED\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//	CDebugProc::Print("\n");
	//}

	//if (m_type == GAUGETYPE_SKILL)
	//{//赤ゲージのとき
	//	CDebugProc::Print("\n");
	//	CDebugProc::Print("GAUGETYPE_SKILL\n");
	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//	CDebugProc::Print("\n");
	//}
#endif
}

//=============================================================================
// 関数名：ゲージの描画処理
// 関数の概要：--
//=============================================================================
void CUiGauge::Draw(void)
{
	//ゲージの描画
	CSceneGauge::Draw();

}

//=============================================================================
// 関数名：数値の代入
// 関数の概要：ダメージや回復など
//=============================================================================
void CUiGauge::AddValue(float fValue)
{
	m_fCurrentGauge = fValue;
	if (m_fCurrentGauge <= 0.0f)
	{
		m_fCurrentGauge = 0.0f;
	}
	if (m_fCurrentGauge >= m_fMaxGauge)
	{
		m_fCurrentGauge = m_fMaxGauge;
	}
	SetCurrentValue(m_fCurrentGauge);
}

//=============================================================================
// 関数名：数値の代入
// 関数の概要：ダメージや回復など
//=============================================================================
void CUiGauge::AddValueSlip(float fValue)
{
	m_fDestValue = fValue;
	if (m_fDestValue <= 0.0f)
	{
		m_fDestValue = 0.0f;
	}
	if (m_fDestValue >= m_fMaxGauge)
	{
		m_fDestValue = m_fMaxGauge;
	}
	m_fValue = (m_fDestValue - m_fCurrentGauge);
}

