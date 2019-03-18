//=============================================================================
//
// エフェクト処理[3Dポリゴン] [particle.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "particle.h"
#include "camera.h"
#include "debugproc.h"
#include "scene_billboard.h"
#include "scene_meshorbit.h"
#include "game.h"
#include "player.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticle::m_apTexture[MAXPARTICLE_TEXTURE] = {};
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CParticle::CParticle(int nPriority, OBJTYPE type) : CSceneBillBoard(nPriority, type)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CParticle::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/effect000.jpg", &m_apTexture[0]);		//光
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/smoke000.jpg", &m_apTexture[1]);	// 煙系テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect/fire.jpg", &m_apTexture[2]);	// 炎系テクスチャ

	return S_OK;
}

//=============================================================================
// 関数名：使用するテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CParticle::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAXPARTICLE_TEXTURE; nCntTexture++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// 関数名：エフェクトの生成
// 関数の概要：ビルボードを生成する
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLEDRAWTYPE drawtype, PARTICLETYPE type, int nLife)
{
	CParticle  *pParticle;
	pParticle = new CParticle;//シーンクラスの生成

							  //プレイヤーの取得
	CPlayer *pPlayer = NULL;
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	switch (mode)
	{
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;

	case CManager::MODE_TUTORIAL:
		pPlayer = CTutorial::GetPlayer();
		break;
	}
	//pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();



	if (pParticle != NULL)
	{
		pParticle->m_pos = pos;
		pParticle->m_col = col;
		pParticle->m_Drawtype = drawtype;
		pParticle->m_Type = type;
		switch (pParticle->m_Type)
		{
		case PARTICLETYPE_TEST:
			pParticle->m_move = D3DXVECTOR3((rand() % 2 + 1) * 0.5f,
				(rand() % 5 + 1) * 1.0f,
				0.0f);
			break;
		case PARTICLETYPE_RAND:

			pParticle->m_move.x = (rand() % 3 - 2) * 1.0f;
			pParticle->m_move.y = (rand() % 2 + 1) * 1.0f;
			pParticle->m_move.z = (rand() % 3 - 2) * 1.0f;


			break;
		case PARTICLETYPE_SLIDING:
			pParticle->m_move = D3DXVECTOR3((sinf(PlayerRot.y - D3DX_PI) * (rand() % 3 - 2 * 1.5f)),
				(rand() % 2 + 1) * 1.0f,
				0.0f);

			break;
		case PARTICLETYPE_ATTACK_SHORYUKEN:
			pParticle->m_move = D3DXVECTOR3((sinf(PlayerRot.y - D3DX_PI) * (rand() % 2 + 1 * 1.2f)),
				(rand() % 8 + 1) * -1.0f,
				cosf((PlayerRot.y - D3DX_PI) * (rand() % 3 - 2) * 1.0f) * -1.0f);
			break;
		case PARTICLETYPE_ATTACK_SKILL:
			pParticle->m_move = D3DXVECTOR3((rand() % 2 - 1) * 1.0f,
				(rand() % 3 + 1) * 1.0f,
				0.0f);
			break;
		case PARTICLETYPE_ATTACK_SKILL_ENEMY:
			pParticle->m_move = D3DXVECTOR3((rand() % 2 - 1) * 1.0f,
				(rand() % 3 + 1) * 1.0f,
				0.0f);
			break;
		case PARTICLETYPE_HIT_000:
			pParticle->m_move.x = (rand() % 8 - 5) * 1.3f;
			pParticle->m_move.y = (rand() % 6 - 2) * 1.3f;
			pParticle->m_move.z = (rand() % 6 - 4) * 1.3f;
			break;
		case PARTICLETYPE_HIT_001:
			pParticle->m_move.x = (rand() % 7 - 8) * 1.0f;
			pParticle->m_move.y = (rand() % 4 + 1) * 1.0f;
			pParticle->m_move.z = (rand() % 3 - 4) * 1.0f;
			break;
		case PARTICLETYPE_HIT_002:
			break;
		case PARTICLETYPE_ATTACK_SPINKICK:
			pParticle->m_move = D3DXVECTOR3((sinf(PlayerRot.y - D3DX_PI) * (rand() % 3 - 2 * 1.5f)),
				(rand() % 2 + 1) * 1.0f,
				cosf((PlayerRot.y - D3DX_PI) * (rand() % 3 - 2) * 1.0f) * -1.0f);
			break;
		}
		pParticle->m_nLife = nLife;
		if (pParticle->m_Type == PARTICLETYPE_ATTACK_SKILL)
		{
			pParticle->m_fRadius = (rand() % 10 + 1) * 1.0f;
		}
		else
		{
			pParticle->m_fRadius = (rand() % 20 + 1) * 1.0f;

		}
		pParticle->Init();
	}
	return pParticle;
}


//=============================================================================
// 関数名：エフェクトの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CParticle::Init(void)
{

	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//位置情報の代入
	SetPos(m_pos);


	if (m_Type == PARTICLETYPE_TEST || m_Type == PARTICLETYPE_HIT_000 || m_Type == PARTICLETYPE_HIT_001 || m_Type == PARTICLETYPE_HIT_002)
	{
		//テクスチャの割り当て
		BindTexture(m_apTexture[0]);

	}
	else if (m_Type == PARTICLETYPE_RAND || m_Type == PARTICLETYPE_RUN || m_Type == PARTICLETYPE_SLIDING)
	{
		//テクスチャの割り当て
		BindTexture(m_apTexture[1]);
	}
	else if (m_Type == PARTICLETYPE_ATTACK_SHORYUKEN || m_Type == PARTICLETYPE_ATTACK_SKILL || m_Type == PARTICLETYPE_ATTACK_SKILL_ENEMY)
	{
		//テクスチャの割り当て
		BindTexture(m_apTexture[2]);
	}
	else if (m_Type == PARTICLETYPE_ATTACK_SPINKICK)
	{
		//テクスチャの割り当て
		BindTexture(m_apTexture[2]);
	}

	//幅の設定
	SetRadius(m_fRadius);
	//ビルボードの初期化
	CSceneBillBoard::Init();

	//色情報の代入
	SetColor(m_col);

	return S_OK;
}

//=============================================================================
// 関数名：エフェクトの終了処理
// 関数の概要：自身を破棄する
//=============================================================================
void CParticle::Uninit(void)
{

	//ビルボードの終了処理
	CSceneBillBoard::Uninit();
}

//=============================================================================
// 関数名：エフェクトの更新処理
// 関数の概要：--
//=============================================================================
void CParticle::Update(void)
{

	//位置情報を加算
	m_pos += m_move;

	//位置情報の設定
	SetPos(m_pos);

	//色情報の代入
	SetColor(m_col);

	//ライフの減少
	m_nLife--;

	//透明度の減少
	m_col.a -= 0.01f;

	if (m_nLife <= 0)
	{
		//終了処理
		Uninit();
	}
	if (m_col.a <= 0)
	{
		//終了処理
		Uninit();
	}
}

//=============================================================================
// 関数名：エフェクトの描画処理
// 関数の概要：減算と加算を設定する
//=============================================================================
void CParticle::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//プレイヤーの取得
	CPlayer *pPlayer = NULL;
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();
	switch (mode)
	{
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	case CManager::MODE_TUTORIAL:
		pPlayer = CTutorial::GetPlayer();
		break;
	}


	D3DXVECTOR3 playerpos;
	playerpos = pPlayer->GetPos();
	if (playerpos.z > -30.0f && (playerpos.x >= 460.0f && playerpos.x <= 580.0f)
		|| playerpos.z > -30.0f && (playerpos.x >= 3190.0f && playerpos.x <= 3530.0f)
		|| (playerpos.x <= 1400.0f && playerpos.x >= 1200.0f) && (playerpos.z >= -168.0f && playerpos.z <= -35.0f)
		|| (playerpos.x <= 2400.0f && playerpos.x >= 2200.0f) && (playerpos.z >= -168.0f && playerpos.z <= -35.0f))
	{
		switch (m_Drawtype)
		{
		case PARTICLEDRAWTYPE_ADD:

			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);

			// ライト影響受けない
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			//ビルボードの描画
			CSceneBillBoard::Draw();

			// Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// ライト影響受ける
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;

		case PARTICLEDRAWTYPE_DEST:
			// 減算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			//Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
			// ライト影響受けない
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			CSceneBillBoard::Draw();

			// Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			// ライト影響受ける
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;
		default:
			CSceneBillBoard::Draw();
			break;
		}
	}
	else
	{

		switch (m_Drawtype)
		{
		case PARTICLEDRAWTYPE_ADD:

			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


			//Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// ライト影響受けない
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			//ビルボードの描画
			CSceneBillBoard::Draw();

			// Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// ライト影響受ける
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;

		case PARTICLEDRAWTYPE_DEST:
			// 減算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			//Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			// ライト影響受けない
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


			CSceneBillBoard::Draw();

			// Zバッファ
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			// ライト影響受ける
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			break;
		default:
			CSceneBillBoard::Draw();
			break;
		}
	}


}

