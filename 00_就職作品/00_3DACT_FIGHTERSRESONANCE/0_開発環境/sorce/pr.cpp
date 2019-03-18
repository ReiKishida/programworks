//=============================================================================
//
// シーン2D処理[3Dポリゴン] [scene3D.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "pr.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_WIDTH (30) //ポリゴンのXの大きさ
#define POLYGON_HEIGHT (30) //ポリゴンのYの大きさ
#define ANIMATION_SPEED (8)	//アニメーションのスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (1.0f)	//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPr::m_apTexture[MAX_TEXTURE_PR] = {};

//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CPr::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/board_0.png", &m_apTexture[0]);		//電光掲示板（流れる文字）
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/board_1.png", &m_apTexture[1]);		//電光掲示板（切り換え1）
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/board_2.png", &m_apTexture[2]);		//電光掲示板（切り換え2）
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_0.png",	 &m_apTexture[3]);		//広告（0）
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_1.png", &m_apTexture[4]);			//広告（1）
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_2.png", &m_apTexture[5]);			//広告（2）
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/advertise/pr_3.png", &m_apTexture[6]);			//広告（3）


	return S_OK;
}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CPr::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_PR; nCntTex++)
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
//コンストラクタ
//=============================================================================
CPr::CPr(int nPriority, OBJTYPE type) : CScene3D(nPriority, type)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CPr::~CPr()
{

}
//=============================================================================
//3Dシーンの生成
//=============================================================================
CPr *CPr::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, PRTYPE type)
{
	CPr  *pPr = NULL;
	pPr = new CPr;//シーンクラスの生成
	if (pPr != NULL)
	{
		pPr->m_pos = pos;
		pPr->m_rot = rot;
		pPr->m_fWidth = fWidth;
		pPr->m_fHeight = fHeight;
		pPr->m_type = type;
	//	pPr->m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pPr->Init();

	}
	return pPr;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CPr::Init(void)
{
	//幅・高さの設定
	SetWidth(m_fWidth);
	SetHeight(m_fHeight);


	//シーン3Dの初期化
	CScene3D::Init();
	//テクスチャの割り当て
	BindTexture(m_apTexture[m_type]);

	//位置情報の代入
	SetPosPolygon(m_pos);

	SetRotPolygon(m_rot);
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CPr::Uninit(void)
{
	//シーン3Dの終了
	CScene3D::Uninit();

}

//=============================================================================
//更新処理
//=============================================================================
void CPr::Update(void)
{
	//位置情報の取得
	D3DXVECTOR3 pos;
	pos = GetPosPolygon();

	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerpos;
	playerpos = pPlayer->GetPos();

	//フレーム数の加算
	m_nCntFrame++;

	if (m_type == PRTYPE_BOARD_ADVERTISE_0)
	{
		if (m_nCntFrame % 120 == 0)
		{
			m_nRand = rand() % 3;
			switch (m_nRand)
			{
			case 0:
				BindTexture(m_apTexture[3]);
				break;
			case 1:
				BindTexture(m_apTexture[4]);
				break;
			case 2:
				BindTexture(m_apTexture[5]);
				break;
			case 3:
				BindTexture(m_apTexture[6]);
				break;
			}
		}

		if (playerpos.x <= 1400.0f && playerpos.x >= 1200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else if (playerpos.x <= 2400.0f && playerpos.x >= 2200.0f &&playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		SetColor(m_col);
	}


	if (m_type == PRTYPE_BOARD_SWITCH_0)
	{
		if (m_nCntFrame % 120 == 0)
		{
			m_nRand = rand() % 2;
			switch (m_nRand)
			{
			case 0:
				BindTexture(m_apTexture[1]);
				break;
			case 1:
				BindTexture(m_apTexture[2]);
				break;
			}
		}
		if (playerpos.z > -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		SetColor(m_col);
	}

	if (m_type == PRTYPE_BOARD_STREAM)
	{
		SetTexture_All(1.0f + (m_nCntFrame *0.001f), 0.0f + (m_nCntFrame *0.001f),0.0f, 1.0f);
		if (playerpos.z > -35.0f)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		}
		else
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		SetColor(m_col);
	}




	SetRotPolygon(m_rot);

	SetPosPolygon(pos);


}

//=============================================================================
//描画処理
//=============================================================================
void CPr::Draw(void)
{
	//シーン3Dの描画
	CScene3D::Draw();
}


