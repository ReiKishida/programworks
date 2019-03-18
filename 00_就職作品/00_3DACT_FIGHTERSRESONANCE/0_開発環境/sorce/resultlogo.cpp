//=============================================================================
//
// タイトルロゴ処理[2Dポリゴン] [titlelogo.cpp]
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
#include "resultlogo.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGO_WIDTH (250.0f)	//タイトルロゴの幅
#define LOGO_HEIGHT (120.0f) //タイトルロゴのYの高さ
#define PRESS_WIDTH (200.0f) //PressEnterの幅
#define PRESS_HEIGHT (70.0f) //PressEnterの高さ

#define PATTERN_ANIM (5)	//テクスチャパターン数

#define ANIMATION_SPEED (4)	//アニメーションスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (0.25f)//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CResultLogo::m_apTexture[MAX_RESULT_TEXTURE] = {};	//共有テクスチャのポインタ
CScene2D *CResultLogo::m_apScene2D[MAX_RESULT_NUM] = {};				//シーン2Dのポインタ
D3DXCOLOR CResultLogo::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色情報
int CResultLogo::m_nJumpRankingFrame = 0;								//ランキング画面へ遷移するときのフレームカウンタ
bool CResultLogo::m_bEnable = false;
CResultLogo::RESULTSTATE CResultLogo::m_state[MAX_RESULT_NUM] = {};
CResultLogo::RESULTTYPE CResultLogo::m_type[MAX_RESULT_NUM] = {};
//=============================================================================
//コンストラクタ
//=============================================================================
CResultLogo::CResultLogo() : CScene(5)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CResultLogo::~CResultLogo()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CResultLogo::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/result.png", &m_apTexture[0]);			//タイトルロゴ[パターン1]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/press_enter2.png", &m_apTexture[1]);		//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/rankbonus.png", &m_apTexture[2]);		//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/score.png", &m_apTexture[3]);		//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/time.png", &m_apTexture[4]);		//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/combo.png", &m_apTexture[5]);		//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/damage.png", &m_apTexture[6]);		//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/total.png", &m_apTexture[7]);		//タイトルロゴ[パターン2]

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CResultLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RESULT_TEXTURE; nCntTex++)
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
CResultLogo *CResultLogo::Create(void)
{
	//タイトルロゴのインスタンス生成
	CResultLogo *pTitle;
	pTitle = new CResultLogo;
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
HRESULT CResultLogo::Init(void)
{
	//フレームの初期化
	m_nJumpRankingFrame = 0;
	m_nCntFrame = 0;

	//色情報の初期化
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntTitle = 0; nCntTitle < MAX_RESULT_NUM; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case RESULTTYPE_LOGO:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(210.0f, 65.0f, 0.0f), LOGO_WIDTH, LOGO_HEIGHT);
			//タイトルの種類の設定
			m_type[nCntTitle] = RESULTTYPE_LOGO;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[0]);
			break;
		case RESULTTYPE_PRESS:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 480.0f, SCREEN_HEIGHT / 2 + 300.0f, 0.0f), PRESS_WIDTH, PRESS_HEIGHT);
			m_type[nCntTitle] = RESULTTYPE_PRESS;
			m_state[nCntTitle] = RESULTSTATE_NORMAL;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[1]);
			break;

		case RESULTTYPE_BONUS:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 280.0f, 80.0f, 0.0f), 700.0f, 150.0f);
			m_type[nCntTitle] = RESULTTYPE_BONUS;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[2]);
			break;

		case RESULTTYPE_SCORE:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 180.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_SCORE;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[3]);
			break;
		case RESULTTYPE_TIME:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 270.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_TIME;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[4]);
			break;
		case RESULTTYPE_COMBO:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 390.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_COMBO;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[5]);
			break;
		case RESULTTYPE_DAMAGE:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 510.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_DAMAGE;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[6]);
			break;
		case RESULTTYPE_TOTAL:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(125.0f, SCREEN_HEIGHT - 90.0f, 0.0f), 120.0f, 60.0f);
			m_type[nCntTitle] = RESULTTYPE_TOTAL;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[7]);
			break;
		}

		//タイトルロゴのオブジェクトの設定
		SetObjType(CScene::OBJTYPE_TITLELOGO);
	}

	m_bEnable = false;

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CResultLogo::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < MAX_RESULT_NUM; nCntTitle++)
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
void CResultLogo::Update(void)
{




	//ランキングへジャンプするフレームのカウント
	m_nJumpRankingFrame++;

	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 posTitle[MAX_RESULT_NUM];//各種位置情報

 // コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();

	if (m_bEnable == true)
	{
		if (pFade == CFade::FADE_NONE)
		{
			if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
			{//RETURNキーが押されたとき
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				m_state[RESULTTYPE_PRESS] = RESULTSTATE_USE;//使用状態に
				CFade::SetFade(CManager::MODE_RANKING);//メニューへ移行
			}
		}
	}


	for (int nCntTitle = 0; nCntTitle < MAX_RESULT_NUM; nCntTitle++)
	{
		posTitle[nCntTitle] = m_apScene2D[nCntTitle]->GetPosPolygon();	//位置情報の取得

		if (m_type[nCntTitle] == RESULTTYPE_PRESS)
		{
			if (m_state[nCntTitle] == RESULTSTATE_NORMAL)
			{
				if (m_state[RESULTTYPE_PRESS] == RESULTSTATE_NORMAL)
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
				}
				else if (m_state[RESULTTYPE_PRESS] == RESULTSTATE_USE)
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

				}

				//色情報の反映
				m_apScene2D[nCntTitle]->SetColor(m_col);
			}
		}


		//位置情報の設定
		m_apScene2D[nCntTitle]->SetPosPolygon(posTitle[nCntTitle]);


	}
}

//=============================================================================
//描画処理
//=============================================================================
void CResultLogo::Draw(void)
{


}

void CResultLogo::SwitchEnable(void)
{
	m_bEnable = m_bEnable ? false : true;
}

