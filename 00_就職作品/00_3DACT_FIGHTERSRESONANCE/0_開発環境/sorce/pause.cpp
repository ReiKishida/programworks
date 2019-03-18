//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Kishida Rei
//=============================================================================
#include "main.h"
#include "manager.h"
#include "pause.h"
#include "polygon.h"
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_apTexture[MAX_PAUSE_TEXTURE] = {}; //共有テクスチャのポインタ
CPolygon *CPause::m_apPolygon[MAX_PAUSE] = {};//シーン2D
int CPause::m_nSelectNum = PAUSEMENU_CONTINUE;
int CPause::m_nCntFrame = 0;
CPause::PAUSESTATE CPause::m_state[MAX_PAUSE] = {};//ポーズメニューの項目

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FLAME_WIDTH (500.0f)		//枠の幅
#define FLAME_HEIGHT (580.0f)		//枠の高さ

#define MENU_WIDTH (350.0f)			//各種メニューの幅
#define MENU_HEIGHT (150.0f)		//各種メニューの高さ

#define GUID_WIDTH (600.0f)			//項目説明の幅
#define GUID_HEIGHT (350.0f)		//項目説明の高さ

#define FLASHING_NUM (80)
#define FLASHING_NUM_HALF (40)

//=============================================================================
//コンストラクタ
//=============================================================================
CPause::CPause()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{

	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();


	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause00.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause01.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause02.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause03.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/mastertuto.png", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/tuto_key.png", &m_apTexture[5]);



	//選択されている数字
	m_nSelectNum = PAUSEMENU_CONTINUE;
	//フレーム数のカウント
	m_nCntFrame = 0;


	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		//ポリゴンの生成
		m_apPolygon[nCntPolygon] = new CPolygon;
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			switch (nCntPolygon)
			{
			case PAUSETYPE_BACK:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(SCREEN_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(SCREEN_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT /2, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(NULL);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.9f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;
				break;
			case PAUSETYPE_FLAME:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(FLAME_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(FLAME_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 , 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[0]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;

				break;
			case PAUSETYPE_CONTINUE:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 -80.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[1]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_RETRY:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2  + 30.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[2]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_QUIT:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[3]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_GUID_ARCADE:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(GUID_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(GUID_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 + 170.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[4]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;
				break;
			case PAUSETYPE_GUID_KEYBOARD:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(GUID_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(GUID_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 - 170.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[5]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;
				break;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CPause::Uninit(void)
{
	//テクスチャのの破棄
	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE_TEXTURE; nCntPolygon++)
	{
		if (m_apTexture[nCntPolygon] != NULL)
		{
			m_apTexture[nCntPolygon]->Release();
			m_apTexture[nCntPolygon] = NULL;
		}
	}

	//ポリゴンの破棄
	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}
}

//=============================================================================
//更新処理
//=============================================================================
void CPause::Update(void)
{
	// コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 posPause[MAX_PAUSE];	//位置情報
	D3DXCOLOR colPause[MAX_PAUSE];		//色情報



	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		posPause[nCntPolygon] = m_apPolygon[nCntPolygon]->GetPosPolygon();		//位置情報の取得
		colPause[nCntPolygon] = m_apPolygon[nCntPolygon]->GetPolygonColor();	//色情報の取得
	}



	//===============================================================================
	//	ポーズの画面遷移先
	//===============================================================================
	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_A) == true)
		{//RETURNキーが押されたとき

				switch (m_nSelectNum)
				{//現在選択中の状態によって処理を分岐
				case PAUSEMENU_CONTINUE://コンティニュー選択中なら
					CRenderer::SetPause();//ポーズ解除
					break;
				case PAUSEMENU_RETRY://リトライ選択中なら
					CRenderer::SetPause();//ポーズ解除
					pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//決定音
					CFade::SetFade(CManager::MODE_GAME);//ゲームへ移行
					break;
				case PAUSEMENU_QUIT://クイット選択中なら
					CRenderer::SetPause();//ポーズ解除
					pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//決定音
					CFade::SetFade(CManager::MODE_TITLE);//タイトルへ移行
					break;
				}

		}
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_V) == true)
	{//Wキーが押されたとき
		CCamera::SetFree();
	}

	//===============================================================================
	//	項目選択
	//===============================================================================
	if (pInputKeyboard->GetKeyboardTrigger(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
	{//Wキーが押されたとき
	 //色情報の反映

			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
			m_nSelectNum = (m_nSelectNum + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;//選択状態を一つ上げる
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
	{//Sキーが押されたとき


			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
			m_nSelectNum = (m_nSelectNum + 1) % PAUSEMENU_MAX;//選択状態を一つ下げる

	}


	//===============================================================================
	//	点滅処理
	//===============================================================================
	m_nCntFrame++;
	m_nCntFrame = m_nCntFrame % FLASHING_NUM;
	if (m_nCntFrame < FLASHING_NUM_HALF)
	{
		colPause[m_nSelectNum + 2].a -= 0.015f;
	}
	if (m_nCntFrame > FLASHING_NUM_HALF)
	{
		colPause[m_nSelectNum + 2].a += 0.015f;
	}

	if (m_state[m_nSelectNum] == PAUSESTATE_USE)
	{//ボタンが押されたとき
		m_nCntFrame = m_nCntFrame % 6;
		if (m_nCntFrame < 3)
		{
			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
		}
		if (m_nCntFrame > 3)
		{
			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//色を赤に
		}
	}

	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->SetPosPolygon(posPause[nCntPolygon]);	//位置情報を設定
		m_apPolygon[nCntPolygon]->SetPolygonColor(colPause[nCntPolygon]);//色情報の設定
	}

}

//=============================================================================
//描画処理
//=============================================================================
void CPause::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->Draw();
	}

}

//=============================================================================
//選択されている数字の設定
//=============================================================================
void CPause::SetSelect(void)
{
	m_nSelectNum = PAUSEMENU_CONTINUE;
}
