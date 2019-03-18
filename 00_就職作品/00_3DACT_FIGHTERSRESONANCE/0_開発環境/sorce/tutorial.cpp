//=======================================================================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author :岸田怜(Kishida Rei)
//
//=======================================================================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "title.h"
#include "tutorial.h"
#include "sceneX.h"
#include "model.h"
#include "player.h"
#include "number.h"
#include "scene_meshfield.h"
#include "scene_meshwall.h"
#include "scene_billboard.h"
#include "scene_meshcylinder.h"
#include "scene_gauge.h"
#include "object.h"
#include "fade.h"
#include "ui_gauge.h"
#include "combogauge.h"
#include "particle.h"
#include "score.h"
#include "timer.h"
#include "enemy.h"
#include "enemy_pow.h"
#include "enemy_trc.h"
#include "pause.h"
#include "ui.h"
#include "combo.h"
#include "ui_gauge_bill.h"
#include "effect.h"
#include "boss.h"
#include "camera.h"
#include "pr.h"
#include "sound.h"
#include "cutin.h"
//********************************************************************************************
// 静的メンバ変数
//********************************************************************************************
CTutoLogo	*CTutorial::m_pTutoLogo = NULL;
CComboGauge *CTutorial::m_apComboGauge[2] = {};		//コンボゲージ
CPlayer *CTutorial::m_pPlayer = NULL;				//プレイヤー

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CTutorial::~CTutorial()
{

}




//=============================================================================
// 関数名：チュートリアルの初期化処理
// 関数の概要：使用するテクスチャやモデルの読み込み
//=============================================================================
HRESULT CTutorial::Init(void)
{
	//プレイヤーのモデルの読み込み
	CPlayer::Load();

	//敵のモデルの読み込み
	CEnemy::Load();

	//パーティクルのテクスチャの読み込み
	CParticle::Load();

	//エフェクトのテクスチャの読み込み
	CEffect::Load();

	//オブジェクトのテクスチャの読み込み
	CObject::Load();

	//ゲージUIのテクスチャの読み込み
	CUiGauge::Load();

	//ゲージUIのテクスチャの読み込み
	CUiGaugeBillBoard::Load();

	//コンボゲージのテクスチャの読み込み
	CComboGauge::Load();

	//コンボのテクスチャの読み込み
	CCombo::Load();

	//メッシュフィールドのテクスチャの読み込み
	CSceneMeshField::Load();

	//メッシュウォールのテクスチャの読み込み
	CSceneMeshWall::Load();

	//数字テクスチャの読み込み
	CNumber::Load();

	//Uiのテクスチャの読み込み
	CUi::Load();

	CCutin::Load();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-100.0f, 0.0f, -350.0f));
	CEnemy::Create(D3DXVECTOR3(250.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, CEnemy::ENEMYTYPE_SPEED);

	CSceneMeshWall::LoadWallText_Tuto();

	//モデルの生成
	CObject::LoadModelTextTuto();

	//メッシュフィールド
	CSceneMeshField::Create(D3DXVECTOR3(1850.0f, -0.5f, -200.0f), 45, 4, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);		//最初の床
	CSceneMeshField::Create(D3DXVECTOR3(2000.0f, -0.4f, -110.0f), 200, 1, 30.0f, 30.0f, CSceneMeshField::FIELDTYPE_YELLOW);

	//スコアの生成
	CScore::Create(D3DXVECTOR3(970.0f, 40.0f, 0.0f), CScore::SCORETYPE_GAME);

	//タイマー
	CTimer::Create(D3DXVECTOR3(1060.0f, 120.0f, 0.0f), CTimer::TIMEMODE_GAME);

	//UIの生成(スコア)
	CUi::Create(CUi::UIMODE_TUTORIAL);


	//コンボの生成
	CCombo::Create();

	//コンボゲージの生成
	m_apComboGauge[0] = CComboGauge::Create(D3DXVECTOR3(1040.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 210.0f, 10.0f, 100.0f, 100.0f, CComboGauge::GAUGETYPE_UNDER);		//コンボゲージの下地生成
	m_apComboGauge[1] = CComboGauge::Create(D3DXVECTOR3(1065.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 170.0f, 5.5f, 0.0f, 100.0f, CComboGauge::GAUGETYPE_COMBO);					//コンボゲージの生成



	return S_OK;
}
//=============================================================================
// 関数名：チュートリアルの終了処理
// 関数の概要：使用するテクスチャやモデルの破棄、シーンの破棄
//=============================================================================
void CTutorial::Uninit()
{
	CCutin::Unload();


	//プレイヤーのモデルの破棄
	CPlayer::Unload();

	//敵のモデルの読み込み
	CEnemy::Unload();

	//パーティクルのテクスチャの破棄
	CParticle::Unload();

	//エフェクトのテクスチャの破棄
	CEffect::Unload();

	//オブジェクトのテクスチャの破棄
	CObject::Unload();

	//ゲージUIのテクスチャの破棄
	CUiGauge::Unload();

	//ゲージUIのテクスチャの破棄
	CUiGaugeBillBoard::Unload();

	//コンボゲージのテクスチャの破棄
	CComboGauge::Unload();

	//コンボのテクスチャの破棄
	CCombo::Unload();

	//メッシュフィールドのテクスチャの破棄
	CSceneMeshField::Unload();

	//メッシュウォールのテクスチャの破棄
	CSceneMeshWall::Unload();

	//数字テクスチャの破棄
	CNumber::Unload();

	//Uiのテクスチャの破棄
	CUi::Unload();




	//全シーンの破棄
	CScene::ReleaseSceneAll();
}

//=============================================================================
// 関数名：チュートリアルの更新処理
// 関数の概要：--
//=============================================================================
void CTutorial::Update()
{
	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
	{//Rキーが押されたとき
		if (pFade == CFade::FADE_NONE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			CFade::SetFade(CManager::MODE_GAME);//ゲームへ移行
		}
	}
}

//=============================================================================
// 関数名：チュートリアルの描画処理
// 関数の概要：--
//=============================================================================
void CTutorial::Draw()
{
}

//=============================================================================
// 関数名：プレイヤークラスの取得
// 関数の概要：プレイヤークラスを返す
//=============================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// 関数名：コンボゲージクラスの取得
// 関数の概要：コンボゲージクラスを返す
//=============================================================================
CComboGauge * CTutorial::GetComboGauge(int nCnt)
{
	return m_apComboGauge[nCnt];
}
