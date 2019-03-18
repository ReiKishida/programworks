//=============================================================================
//
// カットイン処理[2Dポリゴン] [cutin.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "cutin.h"
#include "fade.h"
#include "bg.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOJI_WIDTH (80.0f)	//ポリゴンの幅
#define MOJI_HEIGHT (85.0f)	//ポリゴンのYの高さ
#define CUT_WIDTH (780.0f)	//ポリゴンの幅
#define CUT_HEIGHT (250.0f)	//ポリゴンのYの高さ

#define ENEMY_MOVE (1.5f)	//カットインの移動量

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CCutin::m_apTexture[MAX_TEXTURE_CUTIN] = {}; //共有テクスチャのポインタ
int CCutin::m_nPatternAnim = 0;	//アニメーションパターン
int CCutin::m_nCounterAnim = 0;//アニメーションカウンター
//=============================================================================
//コンストラクタ
//=============================================================================
CCutin::CCutin() : CScene2D(5)
{

}

//=============================================================================
//デストラクタ
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ryou.png", &m_apTexture[0]);//領
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/iki.png", &m_apTexture[1]);//域
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ten.png", &m_apTexture[2]);//展
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/kai.png", &m_apTexture[3]);//開
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cut_player.png", &m_apTexture[4]);//プレイヤーのカットイン
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cut_enemy.png", &m_apTexture[5]);//ボスのカットイン

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CCutin::Unload(void)
{
	for (int nCntCutinTex = 0; nCntCutinTex < MAX_TEXTURE_CUTIN; nCntCutinTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntCutinTex] != NULL)
		{
			m_apTexture[nCntCutinTex]->Release();
			m_apTexture[nCntCutinTex] = NULL;
		}
	}
}



//=============================================================================
//カットインの生成
//=============================================================================
CCutin *CCutin::Create(D3DXVECTOR3 pos, CUTINTYPE type)
{



	//カットインのインスタンス生成
	CCutin *pCutin;
	pCutin = new CCutin;
	if (pCutin != NULL)
	{
		//カットインの初期化
		pCutin->Init(pos, type);
	}
	return pCutin;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CCutin::Init(D3DXVECTOR3 pos, CUTINTYPE type)
{
	//タイプの割り当て
	m_Type = type;


	//テクスチャの割り当て
	switch (m_Type)
	{
	case CUTIN_TYPE_RYOU:
		BindTexture(m_apTexture[0]);
		//カットインの幅、高さ設定
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_RYOU;

		break;
	case CUTIN_TYPE_IKI:
		BindTexture(m_apTexture[1]);
		//カットインの幅、高さ設定
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_IKI;

		break;
	case CUTIN_TYPE_TEN:
		BindTexture(m_apTexture[2]);
		//カットインの幅、高さ設定
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_TEN;

		break;
	case CUTIN_TYPE_KAI:
		BindTexture(m_apTexture[3]);
		//カットインの幅、高さ設定
		SetScene2DWidth(MOJI_WIDTH);
		SetScene2DHeight(MOJI_HEIGHT);
		m_nLife = MAX_LIFE_KAI;

		break;
	case CUTIN_TYPE_CUT_PLAYER:
		BindTexture(m_apTexture[4]);
		//カットインの幅、高さ設定
		SetScene2DWidth(CUT_WIDTH);
		SetScene2DHeight(CUT_HEIGHT);
		m_nLife = MAX_LIFE_CUTIN;
		m_nPatternAnim = 0;
		m_nCounterAnim = 0;
		break;
	case CUTIN_TYPE_CUT_ENEMY:
		BindTexture(m_apTexture[5]);
		//カットインの幅、高さ設定
		SetScene2DWidth(CUT_WIDTH);
		SetScene2DHeight(CUT_HEIGHT);
		m_nLife = MAX_LIFE_CUTIN;
		m_nPatternAnim = 0;
		m_nCounterAnim = 0;
		break;
	}


	//2Dシーンの初期化処理
	CScene2D::Init(pos);

	if (m_Type == CUTIN_TYPE_CUT_PLAYER || m_Type == CUTIN_TYPE_CUT_ENEMY)
	{
		SetTexture(0.5f, 1.0f);
	}


	//カットインのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_CUTIN);



	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CCutin::Uninit(void)
{

	//ポリゴンの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CCutin::Update(void)
{

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	D3DXVECTOR3 moveCutin; //カットインの移動量
	D3DXVECTOR3 posCutin = GetPosScene2D();	//カットインの位置情報
	D3DXVECTOR3 rotCutin = GetRotScene2D();	//カットインの角度情報
	D3DXVECTOR3 BulletMove;
	D3DXVECTOR3 playerPos;//プレイヤーの位置
	CPlayer *pPlayer;//プレイヤー
	pPlayer = CManager::GetPlayer();//プレイヤーの取得
	playerPos = pPlayer->GetPos();//プレイヤーの位置酒取得


	if (m_Type == CUTIN_TYPE_CUT_PLAYER || m_Type == CUTIN_TYPE_CUT_ENEMY)
	{
		m_nCounterAnim++;//アニメーションカウンターの加算

		if (m_nCounterAnim % 60 == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % 2;//パターンNo.更新

			//テクスチャアニメーション
			CScene2D::SetTextureAnim(0.5, 1.0f, m_nPatternAnim);
		}
	}
	//位置情報の設定
	SetPosScene2D(posCutin);

	//ライフの減少
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CCutin::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
//描画処理
//=============================================================================
void CCutin::NextAnim(void)
{
}

