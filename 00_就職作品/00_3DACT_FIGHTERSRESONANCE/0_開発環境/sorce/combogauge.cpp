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
#include "combogauge.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CComboGauge::m_pTexture = NULL;
int CComboGauge::m_nCombo = 0;
float  CComboGauge::m_fComboGauge = 0.0f;					//コンボ量
float  CComboGauge::m_fComboGaugeMax = 0.0f;					//コンボ量
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CComboGauge::CComboGauge(int nPriority, OBJTYPE type) : CSceneGauge(nPriority, type)
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CComboGauge::~CComboGauge()
{

}

//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CComboGauge::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/1_lifegauge.png", &m_pTexture);		// ライフゲージ
	return S_OK;

}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CComboGauge::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

}


//=============================================================================
// 関数名：ゲージの生成
// 関数の概要：ゲージを生成する
//=============================================================================
CComboGauge *CComboGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fCurrentGauge, float fMaxGauge, GAUGETYPE type)
{
	CComboGauge  *pSceneGauge;
	pSceneGauge = new CComboGauge;	//シーンクラスの生成
	if (pSceneGauge != NULL)
	{
		pSceneGauge->m_pos = pos;						//位置情報の代入
		pSceneGauge->m_fWidth = fWidth;					//幅の代入
		pSceneGauge->m_fHeight = fHeight;				//高さの代入
		pSceneGauge->m_fCurrentGauge = fCurrentGauge;	//現在の値の代入
		pSceneGauge->m_fMaxGauge = fMaxGauge;			//最大値の代入
		pSceneGauge->m_col = col;						//ゲージの色情報
		pSceneGauge->m_type = type;						//ゲージの種類の代入
		pSceneGauge->m_nCombo = 0;						//コンボ数
		pSceneGauge->m_fComboGauge = 0.0f;				//コンボ量
		pSceneGauge->Init();
	}
	return pSceneGauge;
}


//=============================================================================
// 関数名：ゲージの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CComboGauge::Init(void)
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
		BindTexture(m_pTexture);
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
void CComboGauge::Uninit(void)
{
	CSceneGauge::Uninit();
}

//=============================================================================
// 関数名：ゲージの更新処理
// 関数の概要：--
//=============================================================================
void CComboGauge::Update(void)
{
	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


	//位置情報の取得
	D3DXVECTOR3 pos = GetPosGauge();



	//フレームの加算
	m_nCntFrame++;

	if (m_type == GAUGETYPE_COMBO)
	{


		AddValueSlip(-0.10f * ((m_nCombo + 1) * 1.1f));
		//m_fCurrentGauge -= (0.09f * ((m_nCombo + 1) * 1.0f));
		//m_fComboGauge -= (0.09f * ((m_nCombo + 1) * 1.0f));

		m_nCntFrame = m_nCntFrame % 120;


		//点滅の処理
		if (m_nCntFrame < 60)
		{
			m_col.a -= 0.01f;
			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
			}
		}
		if (m_nCntFrame > 60)
		{
			m_col.a += 0.01f;
			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;
			}
		}

		if (m_fCurrentGauge < m_fDestValue)
		{//目的のゲージ
			m_fCurrentGauge += m_fValue * 0.035f;
			if (m_fCurrentGauge <= 0.0f)
			{
				if (m_nCombo > 0)
				{
					//コンボ数の加算
					m_nCombo--;
					if (m_nCombo <= 0)
					{
						m_nCombo = 0;
					}
					m_fCurrentGauge = m_fMaxGauge;
				}
			}
			if (m_fCurrentGauge >= m_fMaxGauge)
			{
				m_fCurrentGauge = 0.0f;
				m_fDestValue = 0.0f;
				//コンボ数の加算
				m_nCombo++;
				m_fComboGauge += 5.0f;
				if (m_nCombo >= 5)
				{
					m_nCombo = 5;
				}
			}
		}
		else if (m_fCurrentGauge > m_fDestValue)
		{//目的のゲージ


			m_fCurrentGauge += m_fValue * 0.035f;

			if (m_fCurrentGauge <= 0.0f)
			{
				//if (m_nCombo > 0)
				//{
					//コンボ数の加算
				m_nCombo--;
				if (m_nCombo <= 0)
				{
					m_nCombo = 0;
				}
				m_fCurrentGauge = m_fMaxGauge;
				//}
			}
		}

		if (m_fDestValue == 0.0f)
		{
			m_fDestValue = 0.0f;
			m_fCurrentGauge = 0.0f;
		}


	}



	//現在の値の加算
	SetCurrentValue(m_fCurrentGauge);




	//位置情報の設定
	SetPosGauge(m_pos);

	//色情報の設定
	SetColor(m_col);


#ifdef _DEBUG
	//if (m_type == GAUGETYPE_COMBO)
	//{//緑ゲージのとき
	//	CDebugProc::Print("GAUGETYPE_COMBO\n");
	//	CDebugProc::Print("m_fComboGauge: %.1f\n", m_fComboGauge);
	//	CDebugProc::Print("m_fComboGaugeMax: %.1f\n", m_fComboGaugeMax);

	//	CDebugProc::Print("m_fCurrentGauge: %.1f\n", m_fCurrentGauge);
	//	CDebugProc::Print("m_fDestValue: %.1f\n", m_fDestValue);
	//}



#endif
}

//=============================================================================
// 関数名：ゲージの描画処理
// 関数の概要：--
//=============================================================================
void CComboGauge::Draw(void)
{
	//ゲージの描画
	CSceneGauge::Draw();

}


//=============================================================================
// 関数名：数値の代入
// 関数の概要：ダメージや回復など
//=============================================================================
void CComboGauge::AddValueSlip(float fValue)
{

	if (m_nCombo < 5)
	{
		m_fComboGauge += fValue;
		if (m_fComboGauge <= 100.0f && m_nCombo == 1)
		{
			m_nCombo--;
		}
		if (m_fComboGauge <= 0.0f)
		{
			m_fComboGauge = 0.0f;
		}
	}
	if (m_nCombo == 5)
	{
		m_fComboGauge += fValue;
		if (m_fComboGauge <= 100.0f && m_nCombo == 1)
		{
			m_nCombo--;
		}
		if (m_fComboGauge >= 600.0f)
		{
			m_fComboGauge = 600.0f;
		}
	}


	if (m_fComboGauge >= m_fMaxGauge)
	{

		m_fReminder = m_fComboGauge - (m_fMaxGauge * m_nCombo);
		m_fDestValue = m_fReminder;
	}
	else
	{
		m_fDestValue = m_fComboGauge;
	}

	m_fValue = (m_fDestValue - m_fCurrentGauge);


	float fEmpty = m_fComboGauge;
	if (m_fComboGauge >= m_fComboGaugeMax)
	{
		m_fComboGaugeMax = m_fComboGauge;
	}
}


//=============================================================================
// 関数名：数値の代入
// 関数の概要：ダメージや回復など
//=============================================================================
void CComboGauge::SetComboGauge(float fValue)
{
	m_fComboGauge = fValue;
}





//=============================================================================
// 関数名：コンボの数値の取得
// 関数の概要：主にコンボ表記をかえるときに使用
//=============================================================================
float CComboGauge::GetComboGauge(void)
{
	return m_fComboGauge;
}

//=============================================================================
// 関数名：現在のゲージの数値の取得
// 関数の概要：主にコンボ表記をかえるときに使用
//=============================================================================
float CComboGauge::GetCurrentGauge(void)
{
	return m_fCurrentGauge;
}

//=============================================================================
// 関数名：現在のゲージの数値の代入
// 関数の概要：主にコンボ表記をかえるときに使用
//=============================================================================
void CComboGauge::SetCurrentGauge(float fValue)
{
	m_fCurrentGauge = fValue;
}

float CComboGauge::GetComboGaugeMax(void)
{
	return m_fComboGaugeMax;
}

