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
#include "menu.h"
#include "menuscene.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
//コンストラクタ
//=============================================================================
CMenu::CMenu()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CMenu::~CMenu()
{

}




//=============================================================================
//初期化処理
//=============================================================================
HRESULT CMenu::Init(void)
{
	//////////////////必要なテクスチャの読み込み///////////////////
	//背景のテクスチャの読み込み
	CBg::Load();
	CMenuScene::Load();
	//////////////////必要な処理の生成///////////////////

	//背景の生成
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CMenuScene::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	return S_OK;

}
//=============================================================================
//終了処理
//=============================================================================
void CMenu::Uninit()
{


	CScene::ReleaseSceneAll();

	//読み込んだテクスチャの破棄
	//背景のテクスチャの破棄
	CMenuScene::Unload();
	CBg::Unload();



}
//=============================================================================
//更新処理
//=============================================================================
void CMenu::Update()
{

	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

}

//=============================================================================
//描画処理
//=============================================================================
void CMenu::Draw()
{


}
