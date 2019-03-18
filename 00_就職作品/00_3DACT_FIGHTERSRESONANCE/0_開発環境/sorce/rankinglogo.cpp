//=============================================================================
//
// ランキングロゴ処理[2Dポリゴン] [rinkinglogo.cpp]
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
#include "rankinglogo.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGO_WIDTH (230.0f)	//タイトルロゴの幅
#define LOGO_HEIGHT (110.0f) //タイトルロゴのYの高さ
#define PRESS_WIDTH (200.0f) //PressEnterの幅
#define PRESS_HEIGHT (70.0f) //PressEnterの高さ

#define PATTERN_ANIM (5)	//テクスチャパターン数

#define ANIMATION_SPEED (4)	//アニメーションスピード
#define TEX_X (0.25f)		//テクスチャ座標_X
#define TEX_Y (0.25f)//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRankingLogo::m_apTexture[MAX_RANKING_TEXTURE] = {}; //共有テクスチャのポインタ
CScene2D *CRankingLogo::m_apScene2D[MAX_RANKING_NUM] = {};//シーン2Dのポインタ
D3DXCOLOR CRankingLogo::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色情報
int CRankingLogo::m_nJumpTitleFrame = 0;//ランキング画面へ遷移するときのフレームカウンタ

//=============================================================================
//コンストラクタ
//=============================================================================
CRankingLogo::CRankingLogo(int nPriority, OBJTYPE type) : CScene(nPriority , type)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CRankingLogo::~CRankingLogo()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CRankingLogo::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/ranking.png", &m_apTexture[0]);		//タイトルロゴ[パターン1]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/press_enter2.png", &m_apTexture[1]);	//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/flame.png", &m_apTexture[2]);			//タイトルロゴ[パターン2]

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CRankingLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RANKING_TEXTURE; nCntTex++)
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
//タイトルロゴの生成
//=============================================================================
CRankingLogo *CRankingLogo::Create(void)
{
	//タイトルロゴのインスタンス生成
	CRankingLogo *pTitle;
	pTitle = new CRankingLogo;
	if (pTitle != NULL)
	{
		//タイトルロゴの初期化処理
		pTitle->Init();
	}

	return pTitle;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CRankingLogo::Init(void)
{
	//フレームの初期化
	m_nJumpTitleFrame = 0;
	m_nCntFrame = 0;
	//色情報の初期化
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntTitle = 0; nCntTitle < MAX_RANKING_NUM; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case RANKINGTYPE_FLAME:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2, 0.0f), 955.0f, 550.0f);
			//タイトルの種類の設定
			m_type = RANKINGTYPE_FLAME;
			m_state = RANKINGSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[2]);
			break;
		case RANKINGTYPE_LOGO:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(170.0f, 65.0f, 0.0f), LOGO_WIDTH, LOGO_HEIGHT);
			//タイトルの種類の設定
			m_type = RANKINGTYPE_LOGO;
			m_state = RANKINGSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[0]);
			break;
		case RANKINGTYPE_PRESS:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 480.0f, SCREEN_HEIGHT / 2 + 300.0f, 0.0f), PRESS_WIDTH, PRESS_HEIGHT);
			m_type = RANKINGTYPE_PRESS;
			m_state = RANKINGSTATE_NORMAL;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[1]);
			break;
		}

		//タイトルロゴのオブジェクトの設定
		SetObjType(CScene::OBJTYPE_TITLELOGO);
	}
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CRankingLogo::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < MAX_RANKING_NUM; nCntTitle++)
	{
		if (m_apScene2D[nCntTitle] != NULL)
		{
			//2Dシーンの終了処理
			m_apScene2D[nCntTitle]->Uninit();
			m_apScene2D[nCntTitle] = NULL;

		}

	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CRankingLogo::Update(void)
{
	// コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();



	//ランキングへジャンプするフレームのカウント
	m_nJumpTitleFrame++;

	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 posTitle[MAX_RANKING_NUM];//各種位置情報


	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
		{//RETURNキーが押されたとき
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			m_state = RANKINGSTATE_USE;//使用状態に
			CFade::SetFade(CManager::MODE_TITLE);//メニューへ移行
		}
		if (m_nJumpTitleFrame % 500 == 0)
		{
			CManager::SetTimer(9, 9, 9, 9, 9, 9);
			CFade::SetFade(CManager::MODE_TITLE);//ランキングへ移行
		}
	}

	for (int nCntTitle = 0; nCntTitle < MAX_RANKING_NUM; nCntTitle++)
	{
		posTitle[nCntTitle] = m_apScene2D[nCntTitle]->GetPosPolygon();	//位置情報の取得

		switch (m_type)
		{
		case RANKINGTYPE_FLAME:
			break;
		case RANKINGTYPE_LOGO:
		break;
		case RANKINGTYPE_PRESS:
			if (m_state == RANKINGSTATE_NORMAL)
			{//通常状態のとき
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 280;
				if (m_nCntFrame < 140)
				{
					m_col.a -= 0.01f;
				}
				if (m_nCntFrame > 140)
				{
					m_col.a += 0.01f;
				}
				//色情報の反映
				m_apScene2D[2]->SetColor(m_col);
			}
			else if (m_state == RANKINGSTATE_USE)
			{//ボタンが押されたとき
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 6;
				if (m_nCntFrame < 3)
				{
					m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
				}
				if (m_nCntFrame > 3)
				{
					m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//色を赤に
				}
				//色情報の反映
				m_apScene2D[2]->SetColor(m_col);
			}
			break;
		}

		//位置情報の設定
		m_apScene2D[nCntTitle]->SetPosPolygon(posTitle[nCntTitle]);

	}

}

//=============================================================================
//描画処理
//=============================================================================
void CRankingLogo::Draw(void)
{


}

