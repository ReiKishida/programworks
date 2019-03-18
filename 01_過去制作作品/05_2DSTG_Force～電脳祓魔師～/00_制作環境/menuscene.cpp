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
#include "bullet.h"
#include "enemy.h"
#include "titlelogo.h"
#include "fade.h"
#include "sound.h"
#include "menu.h"
#include "bg.h"
#include "polygon.h"
#include "menuscene.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MENU_WIDTH (130.0f)		//各種メニューの幅
#define MENU_HEIGHT (130.0f)		//各種メニューの高さ

#define WINDOW_WIDTH (1000.0f)	//ウィンドウの幅
#define WINDOW_HEIGHT (280.0f)	//ウィンドウの高さ

#define HUMAN_WIDTH (320.0f)	//人の幅
#define  HUMAN_HEIGHT (520.0f)	//人の高さ
#define MAX_MENU_NUM (3)	//メニューの項目数
//#define MAX_MENU_NUM (2)	//ウィンドウとヒト
#define MAX_STATE_NUM (2)	//状態の数
#define MENU_RADIUS (130.0f)  //項目の距離
#define ROT_SPEED (0.05f)
#define FLASHING_NUM (100)
#define FLASHING_NUM_HALF (50)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMenuScene::m_apTexture[MAX_MENUTEXTURE_NUM] = {}; //共有テクスチャのポインタ
CScene2D *CMenuScene::m_apScene2D[MAX_MENUPOLYGON_NUM] = {};
bool CMenuScene::m_bRotate = false;	//回転してるか
int CMenuScene::m_nSelectMenu = 0;	//選択中の番号
int CMenuScene::m_nSelectState = 0;	//選択中の番号
JoyState g_JoyStateMenuScene;

//=============================================================================
//コンストラクタ
//=============================================================================
CMenuScene::CMenuScene()
{
	//アニメーションカウンター
	m_nTex_Y = 0;
}

//=============================================================================
//デストラクタ
//=============================================================================
CMenuScene::~CMenuScene()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CMenuScene::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/game mode.png", &m_apTexture[MENUTYPE_GAMEMODE]);		//ゲームモード
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &m_apTexture[MENUTYPE_RANKING]);		//ランキング
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/quit.png", &m_apTexture[MENUTYPE_QUIT]);			//クイット
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_human.png", &m_apTexture[MENUTYPE_HUMAN]);		//人
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_game.png", &m_apTexture[MENUTYPE_WINDOW_GAME]);			//ウィンドウ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_ranking.png", &m_apTexture[MENUTYPE_WINDOW_RANKING]);			//ウィンドウ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_quit.png", &m_apTexture[MENUTYPE_WINDOW_QUIT]);			//ウィンドウ

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CMenuScene::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_MENUTEXTURE_NUM; nCntTex++)
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
//メニュー生成
//=============================================================================
CMenuScene *CMenuScene::Create(D3DXVECTOR3 pos)
{
	//メニューインスタンス生成
	CMenuScene *pTitle;
	pTitle = new CMenuScene;
	if (pTitle != NULL)
	{
		//メニュー初期化処理
		pTitle->Init(pos);
	}

	return pTitle;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CMenuScene::Init(D3DXVECTOR3 pos)
{
	//ゲームパッドの状態
	g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//色情報の初期化
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nSelectMenu = MENUTYPE_GAMEMODE;
	m_state = MENUSTATE_NONE;
	m_nCntFrame = 0;
	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		//2Dポリゴンの生成
		m_apScene2D[nCntMenu] = new CScene2D(7);
		if (m_apScene2D[nCntMenu] != NULL)
		{
			switch (nCntMenu)
			{
			case MENUTYPE_GAMEMODE:
				//タイトルロゴの幅、高さ設定
				m_apScene2D[nCntMenu]->SetScene2DWidth(MENU_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(MENU_HEIGHT);
				//テクスチャの割り当て
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_GAMEMODE]);
				//2Dシーンの初期化処理
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//通常の状態に
				//タイトルロゴのオブジェクトの設定
				SetObjType(CScene::OBJTYPE_MENU);
				break;
			case MENUTYPE_RANKING:
				//タイトルロゴの幅、高さ設定
				m_apScene2D[nCntMenu]->SetScene2DWidth(MENU_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(MENU_HEIGHT);
				//テクスチャの割り当て
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_RANKING]);
				//2Dシーンの初期化処理
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(420.0f, 300.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//通常の状態に
				//タイトルロゴのオブジェクトの設定
				SetObjType(CScene::OBJTYPE_MENU);
				break;
			case MENUTYPE_QUIT:
				//タイトルロゴの幅、高さ設定
				m_apScene2D[nCntMenu]->SetScene2DWidth(MENU_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(MENU_HEIGHT);
				//テクスチャの割り当て
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_QUIT]);
				//2Dシーンの初期化処理
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(180.0f, 300.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//通常の状態に
				//タイトルロゴのオブジェクトの設定
				SetObjType(CScene::OBJTYPE_MENU);
				break;
			case MENUTYPE_HUMAN:
				//メニュー幅、高さ設定
				m_apScene2D[nCntMenu]->SetScene2DWidth(HUMAN_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(HUMAN_HEIGHT);
				//テクスチャの割り当て
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_HUMAN]);
				//2Dシーンの初期化処理
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 320.0f, SCREEN_HEIGHT / 2, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(0.25, 1.0f);
				m_state = MENUSTATE_NORMAL;//空の状態に
				//タイトルロゴのオブジェクトの設定
				SetObjType(CScene::OBJTYPE_MENU);

				break;
			case MENUTYPE_WINDOW_GAME:
				//メニュー幅、高さ設定
				m_apScene2D[nCntMenu]->SetScene2DWidth(WINDOW_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(WINDOW_HEIGHT);
				//テクスチャの割り当て
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_WINDOW_GAME]);
				//2Dシーンの初期化処理
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 + 200.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//空の状態に
				//タイトルロゴのオブジェクトの設定
				SetObjType(CScene::OBJTYPE_MENU);

				break;
			}
		}
	}
	return S_OK;
}
//=============================================================================
//終了処理
//=============================================================================
void CMenuScene::Uninit(void)
{
	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		if (m_apScene2D[nCntMenu] != NULL)
		{
			//2Dシーンの終了処理
			m_apScene2D[nCntMenu]->Uninit();
			m_apScene2D[nCntMenu] = NULL;
		}
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CMenuScene::Update(void)
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
	m_MenuPos = D3DXVECTOR3(300.0f, 230.0f, 0.0f);

	D3DXVECTOR3 posMenu[MAX_MENUPOLYGON_NUM];//各種位置情報
	D3DXCOLOR colMenu[MAX_MENUPOLYGON_NUM];//各種位置情報

	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		posMenu[nCntMenu] = m_apScene2D[nCntMenu]->GetPosScene2D();	//位置情報の取得
		colMenu[nCntMenu] = m_apScene2D[nCntMenu]->GetColor();	//位置情報の取得
	}

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
		{//RETURNキーが押されたとき
			m_state = MENUSTATE_USE;//使用状態に
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//決定音

			switch (m_nSelectMenu)
			{//現在選択中の状態によって処理を分岐
			case MENUTYPE_GAMEMODE://ゲーム選択中なら
				CFade::SetFade(CManager::MODE_TUTORIAL);//ゲームへ移行
				break;
			case MENUTYPE_QUIT://設定選択中なら
				CFade::SetFade(CManager::MODE_TITLE);//タイトルへ移行
				break;
			case MENUTYPE_RANKING://設定選択中なら
				CFade::SetFade(CManager::MODE_MENURANKING);//ランキングへ移行
				break;
			}
		}
	}

	switch (m_nSelectMenu)
	{//現在選択中の状態によって処理を分岐
	case MENUTYPE_GAMEMODE://ゲーム選択中なら
		m_apScene2D[4]->BindTexture(m_apTexture[MENUTYPE_WINDOW_GAME]);//ウィンドウテクスチャ座標の設定
		m_apScene2D[MENUTYPE_HUMAN]->SetTexture_All(0.25f, 0.0f, 0.0f, 1.0f);//ウィンドウテクスチャ座標の設定
		break;
	case MENUTYPE_RANKING://設定選択中なら
		m_apScene2D[4]->BindTexture(m_apTexture[MENUTYPE_WINDOW_RANKING]);//ウィンドウテクスチャ座標
		m_apScene2D[MENUTYPE_HUMAN]->SetTexture_All(0.5f, 0.25f, 0.0f, 1.0f);//ウィンドウテクスチャ座標の設定
		break;
	case MENUTYPE_QUIT://設定選択中なら
		m_apScene2D[4]-> BindTexture(m_apTexture[MENUTYPE_WINDOW_QUIT]);//ウィンドウテクスチャ
		m_apScene2D[MENUTYPE_HUMAN]->SetTexture_All(0.75f, 0.5f, 0.0f, 1.0f);//ウィンドウテクスチャ座標の設定
		break;
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_W) == true || pInputKeyboard->GetKeyboardTrigger(DIK_A) == true
		 || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true)
		 || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//Wキーが押されたとき
		if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CURSOR);//カーソル
		 //m_bRotate = true;
		 //色情報の反映
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
			m_nSelectMenu = (m_nSelectMenu + (MAX_MENU_NUM - 1)) % MAX_MENU_NUM;//選択状態を一つ上げる
		}
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_S) == true || pInputKeyboard->GetKeyboardTrigger(DIK_D) == true
		|| (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true)
		|| (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//Sキーが押されたとき
		if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CURSOR);//カーソル
		 //m_bRotate= true;
		 //色情報の反映
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
			m_nSelectMenu = (m_nSelectMenu + 1) % MAX_MENU_NUM;//選択状態を一つ下げる
		}
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_PUSH;

	}


	m_nCntFrame++;
	m_nCntFrame = m_nCntFrame % FLASHING_NUM;
	if (m_nCntFrame < FLASHING_NUM_HALF)
	{
		colMenu[m_nSelectMenu].a -= 0.02f;
	}
	if (m_nCntFrame > FLASHING_NUM_HALF)
	{
		colMenu[m_nSelectMenu].a += 0.02f;
	}

	if (m_state == MENUSTATE_USE)
	{//ボタンが押されたとき
		m_nCntFrame++;
		m_nCntFrame = m_nCntFrame % 6;
		if (m_nCntFrame < 3)
		{
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
		}
		if (m_nCntFrame > 3)
		{
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//色を赤に
		}
	}


	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		m_apScene2D[nCntMenu]->SetColor(colMenu[nCntMenu]);//色情報の設定
		m_apScene2D[nCntMenu]->SetPosScene2D(posMenu[nCntMenu]);//位置情報の設定
	}

	if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

}

//=============================================================================
//描画処理
//=============================================================================
void CMenuScene::Draw(void)
{


}

