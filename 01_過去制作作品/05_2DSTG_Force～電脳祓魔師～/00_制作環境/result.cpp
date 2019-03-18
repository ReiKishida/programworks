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
#include "inputx.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CResult *CResult::m_pResult = NULL;
CManager *CResult::m_pManager = NULL;
JoyState g_JoyStateResult;

//=============================================================================
//コンストラクタ
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CResult::~CResult()
{

}




//=============================================================================
//初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	//ゲームパッドの状態
	g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//////////////////必要なテクスチャの読み込み///////////////////
	//背景のテクスチャの読み込み
	CBg::Load();
	//数字のテクスチャの読み込み
	CNumber::Load();
	//////////////////必要な処理の生成///////////////////

	//背景の生成
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -300.0f, SCREEN_HEIGHT / 2- 100.0f, 0.0f), CScore::SCORETYPE_RESULT);

	return S_OK;

}
//=============================================================================
//終了処理
//=============================================================================
void CResult::Uninit()
{



	//読み込んだテクスチャの破棄
	//背景のテクスチャの破棄
	CBg::Unload();
	CNumber::Unload();


	CScene::ReleaseSceneAll();

}
//=============================================================================
//更新処理
//=============================================================================
void CResult::Update()
{

	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//Rキーが押されたとき
			if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
				//CManager::SetMode(CManager::MODE_TITLE);
				CFade::SetFade(CManager::MODE_RANKING);
			}
			g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

}

//=============================================================================
//描画処理
//=============================================================================
void CResult::Draw()
{


}
