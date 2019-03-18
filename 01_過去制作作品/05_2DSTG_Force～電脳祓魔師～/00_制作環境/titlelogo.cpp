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
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGO_WIDTH (650.0f)	//タイトルロゴの幅
#define LOGO_HEIGHT (250.0f) //タイトルロゴのYの高さ
#define PRESS_WIDTH (350.0f) //PressEnterの幅
#define PRESS_HEIGHT (120.0f) //PressEnterの高さ

#define PATTERN_ANIM (5)	//テクスチャパターン数

#define ANIMATION_SPEED (4)	//アニメーションスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (0.25f)//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTitleLogo::m_apTexture[MAX_TITLE_TEXTURE] = {}; //共有テクスチャのポインタ
CScene2D *CTitleLogo::m_apScene2D[MAX_TITLE_NUM] = {};//シーン2Dのポインタ
D3DXCOLOR CTitleLogo::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色情報
JoyState g_JoyStateTitle;

//=============================================================================
//コンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo() : CScene(5)
{
	//アニメーションカウンター
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
}

//=============================================================================
//デストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CTitleLogo::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_0.png", &m_apTexture[0]);//タイトルロゴ[パターン1]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_1.png", &m_apTexture[1]);//タイトルロゴ[パターン2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_2.png", &m_apTexture[2]);//タイトルロゴ[パターン3]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_3.png", &m_apTexture[3]);//タイトルロゴ[パターン4]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_4.png", &m_apTexture[4]);//タイトルロゴ[パターン5]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_5.png", &m_apTexture[5]);//タイトルロゴ[パターン6]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_6.png", &m_apTexture[6]);//タイトルロゴ[パターン7]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_7.png", &m_apTexture[7]);//タイトルロゴ[パターン8]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_8.png", &m_apTexture[8]);//タイトルロゴ[パターン9]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_9.png", &m_apTexture[9]);//タイトルロゴ[パターン10]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter2.png", &m_apTexture[10]);//PressEnter

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CTitleLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TITLE_TEXTURE; nCntTex++)
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
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos)
{
	//タイトルロゴのインスタンス生成
	CTitleLogo *pTitle;
	pTitle = new CTitleLogo;
	if (pTitle != NULL)
	{
		//タイトルロゴの初期化処理
		pTitle->Init(pos);


	}

	return pTitle;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos)
{
	//アニメーションカウンター
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_nCntFrame = 0;
	//ゲームパッドの状態
	g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//色情報の初期化
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
	{
		//2Dポリゴンの生成
		m_apScene2D[nCntTitle] = new CScene2D(5);
		if (m_apScene2D[nCntTitle] != NULL)
		{
			if (nCntTitle == TITLETYPE_LOGO)
			{
				//タイトルロゴの幅、高さ設定
				m_apScene2D[nCntTitle]->SetScene2DWidth(LOGO_WIDTH);
				m_apScene2D[nCntTitle]->SetScene2DHeight(LOGO_HEIGHT);
				//テクスチャの割り当て
				m_apScene2D[nCntTitle]->BindTexture(m_apTexture[0]);

			}
			else if (nCntTitle == TITLETYPE_PRESS)
			{
				//タイトルロゴの幅、高さ設定
				m_apScene2D[nCntTitle]->SetScene2DWidth(PRESS_WIDTH);
				m_apScene2D[nCntTitle]->SetScene2DHeight(PRESS_HEIGHT);
				//テクスチャの割り当て
				m_apScene2D[nCntTitle]->BindTexture(m_apTexture[10]);
			}




			if (nCntTitle == TITLETYPE_LOGO)
			{
				//2Dシーンの初期化処理
				m_apScene2D[nCntTitle]->Init(pos);
				m_apScene2D[nCntTitle]->SetTexture(1.0f, 1.0f);
				m_state = TITLESTATE_NONE;//空の状態に

			}
			else if (nCntTitle == TITLETYPE_PRESS)
			{
				//2Dシーンの初期化処理
				m_apScene2D[nCntTitle]->Init(D3DXVECTOR3(pos.x, pos.y + 350.0f, pos.z));
				m_apScene2D[nCntTitle]->SetTexture(1.0f, 1.0f);
				m_state = TITLESTATE_NORMAL;//通常状態に
			}
		}
			//タイトルロゴのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_TITLELOGO);
	}
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CTitleLogo::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
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
void CTitleLogo::Update(void)
{
	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	D3DXVECTOR3 posTitle[MAX_TITLE_NUM];//各種位置情報


	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//RETURNキーが押されたとき
			if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				m_state = TITLESTATE_USE;//使用状態に
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//決定音
				CFade::SetFade(CManager::MODE_MENU);//メニューへ移行
				//CFade::SetFade(CManager::MODE_GAME);//ゲームへ移行
			}
			g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
	{
		posTitle[nCntTitle] = m_apScene2D[nCntTitle]->GetPosScene2D();	//位置情報の取得

		if (nCntTitle == TITLETYPE_LOGO)
		{

			if (rand() % 12 == 0)
			{


				if (m_TexNum = rand() % 10)
				{
					m_apScene2D[nCntTitle]->BindTexture(m_apTexture[m_TexNum]);
				}
			}
		}
		if (nCntTitle == TITLETYPE_PRESS)
		{
			if (m_state == TITLESTATE_NORMAL)
			{//通常状態のとき
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 160;
				if (m_nCntFrame < 80)
				{
					m_col.a -= 0.015f;
				}
				if (m_nCntFrame > 80)
				{
					m_col.a += 0.015f;
				}
				//色情報の反映
				m_apScene2D[1]->SetColor(m_col);
			}
			else if(m_state == TITLESTATE_USE)
			{//ボタンが押されたとき
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 6;
				if (m_nCntFrame < 3)
				{
					m_col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);//色を白に
				}
				if (m_nCntFrame > 3)
				{
					m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//色を赤に
				}
				//色情報の反映
				m_apScene2D[1]->SetColor(m_col);

			}
		}

		//位置情報の設定
		m_apScene2D[nCntTitle]->SetPosScene2D(posTitle[nCntTitle]);

	}


	if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}


}

//=============================================================================
//描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{


}

