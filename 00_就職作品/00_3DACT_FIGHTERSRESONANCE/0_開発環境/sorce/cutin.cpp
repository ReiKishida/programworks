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
#include "cutin.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_WIDTH (50.0f)
#define POLYGON_HEIGHT (50.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		CCutin::m_apTexture[MAX_TEXTURE_CUTIN] = {};				// テクスチャへのポインタ
CScene2D *CCutin::m_apScene2D[MAX_CUTIN] = {};
//int CCutin::m_nCntFlame = 0;
CCutin::CUTINMODE CCutin::m_mode = CUTINMODE_NONE;
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CCutin::CCutin(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_fWidth = 0.0f;	//ポリゴンの幅
	m_fHeight = 0.0f;	//ポリゴンの高さ
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CCutin::~CCutin()
{

}


//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CCutin::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/0_flame1.png",	&m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/1_flame1_E.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/2_flame0.png",   &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/3_flame0_E.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/4_cutin_0.png", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/5_cutin_1.png", &m_apTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/6_skill_0.png", &m_apTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/7_skill_0.png", &m_apTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cutin/name.png", &m_apTexture[8]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CCutin::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_CUTIN; nCntTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
// 関数名：2Dシーンの生成
// 関数の概要：2Dシーンを生成する
//=============================================================================
CCutin *CCutin::Create(CUTINMODE mode)
{
	CCutin  *pCutin;
	pCutin = new CCutin;//シーンクラスの生成
	if (pCutin != NULL)
	{

		pCutin->m_mode = mode;
		pCutin->Init();
	}
	return pCutin;
}

//=============================================================================
// 関数名：2Dシーンの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CCutin::Init(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_CUTIN; nCntScene2D++)
	{
		switch (nCntScene2D)
		{
		case CUTCOUNT_FRAME:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH /2 -50.0f, SCREEN_HEIGHT / 2 -50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME_PLAYER]);
				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME_ENEMY]);

				break;
			}
			break;
		case CUTCOUNT_FRAME2:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 -50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME2_PLAYER]);

				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME2_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_FRAME2_ENEMY]);

				break;
			}
			break;
		case CUTCOUNT_SILHOUETTE:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2 , 0.0f), 320.0f, 600.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SILHOUETTE_PLAYER]);

				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f), 320.0f, 600.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SILHOUETTE_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f), 320.0f, 600.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SILHOUETTE_ENEMY]);

				break;
			}
			break;
		case CUTCOUNT_SKILL:
			switch (m_mode)
			{
			case CUTINMODE_PLAYER:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, 620.0f, 0.0f), 400.0f, 200.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SKILL_PLAYER]);
				break;
			case CUTINMODE_ENEMY:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, 620.0f, 0.0f), 400.0f, 200.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_SKILL_ENEMY]);
				break;
			case CUTINMODE_ENEMYAPPEAR:
				m_apScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(0.0f, 620.0f, 0.0f), 400.0f, 200.0f);
				m_apScene2D[nCntScene2D]->BindTexture(m_apTexture[CUTINTYPE_NAME_ENEMY]);

				break;
			}
			break;
		}
	}
	switch (m_mode)
	{
	case CUTINMODE_PLAYER:
		m_nCntflame = 115;

		break;

	case CUTINMODE_ENEMY:
		m_nCntflame = 60;

		break;
	case CUTINMODE_ENEMYAPPEAR:
		m_nCntflame = 170;

		break;
	}
	SetObjType(OBJTYPE_CUTIN);

	return S_OK;
}

//=============================================================================
// 関数名：2Dシーンの終了処理
// 関数の概要：テクスチャ、頂点バッファの破棄、自身の破棄
//=============================================================================
void CCutin::Uninit(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_CUTIN; nCntScene2D++)
	{
		if (m_apScene2D[nCntScene2D] != NULL)
		{
			//終了処理
			m_apScene2D[nCntScene2D]->Uninit();
			//delete m_apScene2D[nCntScene2D];
			m_apScene2D[nCntScene2D] = NULL;
		}
	}

	Release();
}

//=============================================================================
// 関数名：2Dシーンの更新処理
// 関数の概要：--
//=============================================================================
void CCutin::Update(void)
{
	m_nCntflame--;
	for (int nCntScene2D = 0; nCntScene2D < MAX_CUTIN; nCntScene2D++)
	{
		D3DXVECTOR3 pos[MAX_CUTIN];
		//位置情報の取得
		if (m_apScene2D[nCntScene2D] != NULL)
		{

			pos[nCntScene2D] = m_apScene2D[nCntScene2D]->GetPosPolygon();

			pos[CUTCOUNT_FRAME].x += 10.0f;
			pos[CUTCOUNT_FRAME2].x += 3.0f;
			pos[CUTCOUNT_SILHOUETTE].x += 8.0f;
			pos[CUTCOUNT_SKILL].x += 12.0f;

			if (pos[CUTCOUNT_FRAME].x >= SCREEN_WIDTH / 2 + 100.0f)
			{//カットインフレーム
				pos[CUTCOUNT_FRAME].x = SCREEN_WIDTH / 2 + 100.0f;
			}

			if (pos[CUTCOUNT_FRAME2].x >= SCREEN_WIDTH / 2 + 100.0f)
			{//カットインフレーム2
				pos[CUTCOUNT_FRAME2].x = SCREEN_WIDTH / 2 + 100.0f;
			}

			if (pos[CUTCOUNT_SILHOUETTE].x >= 200.0f)
			{//キャラクターのシルエット
				pos[CUTCOUNT_SILHOUETTE].x = 200.0f;
			}
			if (pos[CUTCOUNT_SKILL].x >= 280.0f)
			{//スキル名
				pos[CUTCOUNT_SKILL].x = 280.0f;
			}

			//位置情報の代入
			m_apScene2D[nCntScene2D]->SetPosPolygon(pos[nCntScene2D]);


			if (m_nCntflame <= 0)
			{
				if (m_apScene2D[nCntScene2D] != NULL)
				{
					//終了処理
					m_apScene2D[nCntScene2D]->Uninit();
					//delete m_apScene2D[nCntScene2D];
					m_apScene2D[nCntScene2D] = NULL;
					Release();
				}
				m_nCntflame = 0;
			}

		}
	}



}

//=============================================================================
// 関数名：2Dシーンの描画処理
// 関数の概要：--
//=============================================================================
void CCutin::Draw(void)
{

}
