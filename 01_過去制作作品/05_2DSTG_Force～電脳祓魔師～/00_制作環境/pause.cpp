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
#include "inputx.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_apTexture[MAX_PAUSE_TEXTURE] = {}; //共有テクスチャのポインタ
CPolygon *CPause::m_apPolygon[MAX_PAUSE] = {};//シーン2D
int CPause::m_nSelectNum = PAUSEMENU_CONTINUE;
int CPause::m_nCntFrame = 0;
CPause::PAUSESTATE CPause::m_state[MAX_PAUSE] = {};//ポーズメニューの項目
JoyState g_JoyStatePause;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FLAME_WIDTH (500.0f)		//枠の幅
#define FLAME_HEIGHT (580.0f)		//枠の高さ

#define MENU_WIDTH (350.0f)			//各種メニューの幅
#define MENU_HEIGHT (150.0f)		//各種メニューの高さ

#define GUID_WIDTH (900.0f)			//項目説明の幅
#define GUID_HEIGHT (200.0f)		//項目説明の高さ

#define FLASHING_NUM (100)
#define FLASHING_NUM_HALF (50)

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
	//ゲームパッドの状態
	g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();


	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wafuu.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_continue.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_retry.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pasuse_quit.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pausemenu.png", &m_apTexture[4]);



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
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH /2 , SCREEN_HEIGHT /2, 0.0f));
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;
				break;
			case PAUSETYPE_FLAME:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(FLAME_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(FLAME_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 -120.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[0]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;

				break;
			case PAUSETYPE_CONTINUE:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 -240.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[1]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_RETRY:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 120.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[2]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_QUIT:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[3]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_GUID:
				//ポーズの幅、高さ設定
				m_apPolygon[nCntPolygon]->SetPolygonWidth(GUID_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(GUID_HEIGHT);
				//2Dシーンの初期化処理
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[4]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apPolygon[nCntPolygon]->SetTexture(1.0f, 0.25f);
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
			m_apPolygon[nCntPolygon] = NULL;
		}
	}
}

//=============================================================================
//更新処理
//=============================================================================
void CPause::Update(void)
{
	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

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
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
		{//RETURNキーが押されたとき
			if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				switch (m_nSelectNum)
				{//現在選択中の状態によって処理を分岐
				case PAUSEMENU_CONTINUE://コンティニュー選択中なら
					CRenderer::SetPause();//ポーズ解除
					break;
				case PAUSEMENU_RETRY://リトライ選択中なら
					pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//決定音
					CFade::SetFade(CManager::MODE_GAME);//ゲームへ移行
					break;
				case PAUSEMENU_QUIT://クイット選択中なら
					pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//決定音
					CFade::SetFade(CManager::MODE_TITLE);//タイトルへ移行
					break;
				}
			}
			g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	//===============================================================================
	//	ポーズの説明テクスチャの設定
	//===============================================================================
	switch (m_nSelectNum)
	{//現在選択中の状態によって処理を分岐
	case PAUSEMENU_CONTINUE://ゲーム選択中なら
		m_apPolygon[PAUSETYPE_GUID]->SetTexture_All(1.0f, 0.0f, 0.0f, 0.25f);//ガイドテクスチャ座標の設定
		break;
	case PAUSEMENU_RETRY://設定選択中なら
		m_apPolygon[PAUSETYPE_GUID]->SetTexture_All(1.0f, 0.0f, 0.25f, 0.5f);//ガイドテクスチャ座標
		break;
	case PAUSEMENU_QUIT://設定選択中なら
		m_apPolygon[PAUSETYPE_GUID]->SetTexture_All(1.0f, 0.0f, 0.5f, 0.75f);//ガイドテクスチャ
		break;
	}

	if (CRenderer::GetPause() == false)
	{
		for (int nCntPolygon = PAUSETYPE_CONTINUE; nCntPolygon <= PAUSETYPE_QUIT; nCntPolygon++)
		{
			m_state[nCntPolygon] = PAUSESTATE_NORMAL;
			m_nSelectNum = PAUSEMENU_CONTINUE;
		}
	}

	//===============================================================================
	//	項目選択
	//===============================================================================
	if (pInputKeyboard->GetKeyboardTrigger(DIK_W) == true || (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//Wキーが押されたとき
	 //色情報の反映
		if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CURSOR);//カーソル
			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
			m_nSelectNum = (m_nSelectNum + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;//選択状態を一つ上げる
		}
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_S) == true || (state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//Sキーが押されたとき
		if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			//色情報の反映
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CURSOR);//カーソル
			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
			m_nSelectNum = (m_nSelectNum + 1) % PAUSEMENU_MAX;//選択状態を一つ下げる
		}
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_PUSH;

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

	if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
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
