//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author :岸田怜(Kishida Rei)
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
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "ui.h"
#include "effect.h"
#include "sound.h"
#include "score.h"
#include"number.h"
#include "polygon.h"
#include "force.h"
#include "timer.h"
#include "followforce.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "ranking.h"
#include "rankingnumber.h"
#include "rankinglogo.h"
#include "inputx.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CRanking::m_nCntFlame = 0;//フレームのカウント
JoyState g_JoyStateRanking;

//=============================================================================
//コンストラクタ
//=============================================================================
CRanking::CRanking()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}




//=============================================================================
//初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	//ゲームパッドの状態
	g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	//////////////////必要なテクスチャの読み込み///////////////////
	//背景のテクスチャの読み込み
	CBg::Load();
	CNumber::Load();
	CRankingLogo::Load();
	//////////////////必要な処理の生成///////////////////

	//背景の生成
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -280.0f, SCREEN_HEIGHT / 2 -250.0f, 0.0f));
	CRankingLogo::Create(D3DXVECTOR3(250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));
	return S_OK;

}
//=============================================================================
//終了処理
//=============================================================================
void CRanking::Uninit()
{


	CScene::ReleaseSceneAll();

	//読み込んだテクスチャの破棄
	//背景のテクスチャの破棄
	CNumber::Unload();
	CBg::Unload();
	CRankingLogo::Unload();




}
//=============================================================================
//更新処理
//=============================================================================
void CRanking::Update()
{

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//Rキーが押されたとき
			if (g_JoyStateRanking.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
				//CManager::SetMode(CManager::MODE_TITLE);
				CFade::SetFade(CManager::MODE_TITLE);
			}
			g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	m_nCntFlame++;
	if (m_nCntFlame % 300 == 0)
	{
		if (pFade == CFade::FADE_NONE)
		{

			CFade::SetFade(CManager::MODE_TITLE);//ランキングへ移行
		}
	}

	if (g_JoyStateRanking.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateRanking.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CRanking::Draw()
{


}
