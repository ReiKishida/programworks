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
#include "menuranking.h"
#include "rankingnumber.h"
#include "rankinglogo.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
//コンストラクタ
//=============================================================================
CMenuRanking::CMenuRanking()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CMenuRanking::~CMenuRanking()
{

}




//=============================================================================
//初期化処理
//=============================================================================
HRESULT CMenuRanking::Init(void)
{
	//////////////////必要なテクスチャの読み込み///////////////////
	//背景のテクスチャの読み込み
	CBg::Load();
	CNumber::Load();
	CRankingLogo::Load();
	//////////////////必要な処理の生成///////////////////

	//背景の生成
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 280.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));
	CRankingLogo::Create(D3DXVECTOR3(250.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));
	return S_OK;

}
//=============================================================================
//終了処理
//=============================================================================
void CMenuRanking::Uninit()
{

	//全シーンの破棄
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
void CMenuRanking::Update()
{

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
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
		{//Rキーが押されたとき
			pSound->PlaySoundA(pSound->SOUND_LABEL_SE_DECIDE);
			CFade::SetFade(CManager::MODE_MENU);
		}
	}

}

//=============================================================================
//描画処理
//=============================================================================
void CMenuRanking::Draw()
{


}
