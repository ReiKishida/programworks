//=============================================================================
//
// マネージャ処理 [game.cpp]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "title.h"
#include "game.h"
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
#include "cutin.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPD_LIFE (100.0f)
#define POW_LIFE (200.0f)
#define TRC_LIFE (150.0f)


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CGame *CGame::m_pGame = NULL;					//ゲーム
CBoss *CGame::m_pBoss = NULL;					//ボス
CPlayer *CGame::m_pPlayer = NULL;				//プレイヤー
CEnemy *CGame::m_pEnemy[30] = {};				//敵
CModel *CGame::m_pModel = {};					//モデル
CObject *CGame::m_pObject = NULL;				//オブジェクト
CMeshCylinder *CGame::m_pMeshCylinder = NULL;	//シリンダー
CNumber *CGame::m_pNumber = NULL;				//数字
CTimer *CGame::m_pTimer = NULL;					//タイマー
CSceneMeshField *CGame::m_pMeshField[4] = {};	//メッシュフィールド
CComboGauge *CGame::m_apComboGauge[2] = {};		//コンボゲージ
CSceneMeshWall *CGame::m_apMeshWall[15] = {};	//メッシュウォール
CGame::GAMESTATE CGame::m_GameState = CGame::GAMESTATE_NONE;//ゲームの状態
int  CGame::m_nCntGame = 0;					//シーンの移り
bool CGame::m_bCreateBoss = false;			//ボスを生成するか
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CGame::CGame()
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CGame::~CGame()
{

}



//=============================================================================
// 関数名：ゲームの初期化処理
// 関数の概要：使用するテクスチャやモデルの読み込み
//=============================================================================
HRESULT CGame::Init(void)
{
	//プレイヤーのモデルの読み込み
	CPlayer::Load();

	//敵のモデルの読み込み
	CEnemyTrick::Load();

	//敵のモデルの読み込み
	CEnemyPower::Load();

	//敵のモデルの読み込み
	CEnemy::Load();

	//ボスのモデルの読み込み
	CBoss::Load();

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

	//広告のテクスチャの読み込み
	CPr::Load();

	//カットインのテクスチャ読み込み
	CCutin::Load();


	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-100.0f, 0.0f, -350.0f));

	//敵の生成
	CreateEnemy();

	//モデルの生成
	CObject::LoadModelText();

	//電光掲示板の生成
	CPr::Create(D3DXVECTOR3(70.0f, 165.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 5.0f, CPr::PRTYPE_BOARD_SWITCH_0);
	CPr::Create(D3DXVECTOR3(70.0f, 154.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 10.0f, CPr::PRTYPE_BOARD_STREAM);

	CPr::Create(D3DXVECTOR3(820.0f, 165.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 5.0f, CPr::PRTYPE_BOARD_SWITCH_0);
	CPr::Create(D3DXVECTOR3(820.0f, 154.0f, -155.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 10.0f, CPr::PRTYPE_BOARD_STREAM);

	CPr::Create(D3DXVECTOR3(3920.0f, 195.0f, -325.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 5.0f, CPr::PRTYPE_BOARD_SWITCH_0);
	CPr::Create(D3DXVECTOR3(3920.0f, 184.0f, -325.5), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 57.0f, 10.0f, CPr::PRTYPE_BOARD_STREAM);




	//広告
	CPr::Create(D3DXVECTOR3(1300.0f, 70.0f, -210.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);
	CPr::Create(D3DXVECTOR3(2300.0f, 70.0f, -210.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);
	CPr::Create(D3DXVECTOR3(4060.0f, 70.0f, -1700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);
	CPr::Create(D3DXVECTOR3(4060.0f, 70.0f, -2504.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 90.0f, 40.0f, CPr::PRTYPE_BOARD_ADVERTISE_0);




	//メッシュフィールドの生成
	m_pMeshField[0] = CSceneMeshField::Create(D3DXVECTOR3(1850.0f, -0.5f, -200.0f), 45,4,100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);		//最初の床
	m_pMeshField[1] = CSceneMeshField::Create(D3DXVECTOR3(3900.0f, -0.5f, -1900.0f), 10, 30, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);	//次の床
	m_pMeshField[2] = CSceneMeshField::Create(D3DXVECTOR3(4100.0f, -0.5f, -3800.0f), 9, 10, 100.0f, 100.0f, CSceneMeshField::FIELDTYPE_NORMAL);	//ボスの床
	m_pMeshField[3] = CSceneMeshField::Create(D3DXVECTOR3(2000.0f, -0.4f, -80.0f), 150, 1, 30.0f, 30.0f, CSceneMeshField::FIELDTYPE_YELLOW);


	//メッシュウォールの生成(見えない壁)
	CSceneMeshWall::LoadWallText();

	//スコアの生成
	CScore::Create(D3DXVECTOR3(970.0f, 40.0f, 0.0f), CScore::SCORETYPE_GAME);

	//タイマー
	CTimer::Create(D3DXVECTOR3(1060.0f, 120.0f, 0.0f),CTimer::TIMEMODE_GAME);

	//UIの生成(スコア)
	CUi::Create(CUi::UIMODE_GAME);


	//コンボの生成
	CCombo::Create();

	//コンボゲージの生成
	m_apComboGauge[0] = CComboGauge::Create(D3DXVECTOR3(1040.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 210.0f, 10.0f, 100.0f, 100.0f, CComboGauge::GAUGETYPE_UNDER);		//コンボゲージの下地生成
	m_apComboGauge[1] = CComboGauge::Create(D3DXVECTOR3(1065.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 170.0f, 5.5f, 0.0f, 100.0f, CComboGauge::GAUGETYPE_COMBO);					//コンボゲージの生成


	m_GameState = GAMESTATE_NORMAL;	//ゲームの状態設定
	m_nCntGame = 0;
	m_bCreateBoss = false;

	if (m_pBoss != NULL)
	{
		m_pBoss = NULL;
	}


	return S_OK;

}
//=============================================================================
// 関数名：ゲームの終了処理
// 関数の概要：使用するテクスチャやモデルの破棄、シーンの破棄
//=============================================================================
void CGame::Uninit()
{
	//カットインのテクスチャ読み込み
	CCutin::Unload();

	//広告のテクスチャの読み込み
	CPr::Unload();

	//Uiのテクスチャの破棄
	CUi::Unload();

	//プレイヤーのモデルの破棄
	CPlayer::Unload();

	//敵のモデルの破棄
	CEnemy::Unload();

	//敵のモデルの破棄
	CEnemyPower::Unload();

	//敵のモデルの破棄
	CEnemyTrick::Unload();

	//ボスのモデルの破棄
	CBoss::Unload();

	//パーティクルのテクスチャの破棄
	CParticle::Unload();

	//エフェクトのテクスチャの破棄
	CEffect::Unload();

	//オブジェクトのテクスチャの破棄
	CObject::Unload();

	//メッシュフィールドのテクスチャの破棄
	CSceneMeshField::Unload();

	//メッシュウォールのテクスチャの破棄
	CSceneMeshWall::Unload();

	//コンボの生成
	CCombo::Unload();


	//ゲージUIのテクスチャの破棄
	CUiGauge::Unload();
	//ゲージUIのテクスチャの破棄
	CUiGaugeBillBoard::Unload();


	//コンボゲージのテクスチャの破棄
	CComboGauge::Unload();


	//数字テクスチャの破棄
	CNumber::Unload();


	//シーンの破棄
	CScene::ReleaseSceneAll();

}
//=============================================================================
// 関数名：ゲームの更新処理
// 関数の概要：--
//=============================================================================
void CGame::Update()
{
	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


	// コントローラー取得
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_GameState == GAMESTATE_NORMAL)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_P) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
		{//ポーズON/OFF

			CRenderer::SetPause();
		}
	}

	switch (m_GameState)
	{
	case GAMESTATE_NORMAL:
		break;

	case GAMESTATE_CLEAR:
		m_nCntGame--;
		if (m_nCntGame <= 0)
		{
			m_GameState = GAMESTATE_NONE;

			//モード設定
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;

	case GAMESTATE_FAILED:
		m_nCntGame--;
		if (m_nCntGame <= 0)
		{
			m_GameState = GAMESTATE_NONE;

			//モード設定
			if (pFade == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}
		break;
	}


	if (m_bCreateBoss == true)
	{
		if (m_pBoss == NULL)
		{
			m_pBoss = CBoss::Create(D3DXVECTOR3(3900.0f, 0.0f, -3900.0f), D3DXVECTOR3(0.0f, -3.0f, 0.0f), 500.0f);
			CSceneMeshWall::Create(D3DXVECTOR3(3800.0f, 0.0f, -3390.0f), D3DXVECTOR3(0.0f, 1800.0f, 0.0f), 6, 5, 100.0f, 100.0f, CSceneMeshWall::WALLTYPE_INVISIBLE);
			CSceneMeshWall::Create(D3DXVECTOR3(3800.0f, 0.0f, -3400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6, 5, 100.0f, 100.0f, CSceneMeshWall::WALLTYPE_NORMAL);
			CSceneMeshWall::Create(D3DXVECTOR3(3800.0f, 0.0f, -3410.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 5, 100.0f, 100.0f, CSceneMeshWall::WALLTYPE_INVISIBLE);

			pSound->StopSound();
			pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_BOSS);

		}
	}


}

//=============================================================================
// 関数名：ゲームの描画処理
// 関数の概要：--
//=============================================================================
void CGame::Draw()
{

}


//=============================================================================
// 関数名：敵の生成
// 関数の概要：--
//=============================================================================
void CGame::CreateEnemy(void)
{
	//敵の生成
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);
	CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 45.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);

	CEnemy::Create(D3DXVECTOR3(700.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);

	CEnemyPower::Create(D3DXVECTOR3(800.0f, 0.0f, -350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);

	CEnemyTrick::Create(D3DXVECTOR3(1200.0f, 0.0f, -150.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);
	CEnemy::Create(D3DXVECTOR3(1300.0f, 0.0f, -350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);


	CEnemyPower::Create(D3DXVECTOR3(1880.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemyTrick::Create(D3DXVECTOR3(1980.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);


	CEnemy::Create(D3DXVECTOR3(3080.0f, 0.0f, -350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);
	CEnemyPower::Create(D3DXVECTOR3(3010.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemyTrick::Create(D3DXVECTOR3(3010.0f, 0.0f, -230.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);

	CEnemy::Create(D3DXVECTOR3(3669.0f, 0.0f, 65.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);


	CEnemy::Create(D3DXVECTOR3(4000.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);
	CEnemyPower::Create(D3DXVECTOR3(3750.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemy::Create(D3DXVECTOR3(3650.0f, 0.0f, -230.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);



	CEnemyPower::Create(D3DXVECTOR3(3850.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), POW_LIFE, CEnemyPower::ENEMYTYPE_HEAVY);
	CEnemyTrick::Create(D3DXVECTOR3(3750.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);

	CEnemyTrick::Create(D3DXVECTOR3(3900.0f, 0.0f, -2200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);
	CEnemyTrick::Create(D3DXVECTOR3(3700.0f, 0.0f, -2200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), TRC_LIFE, CEnemyTrick::ENEMYTYPE_TRICKY);


	CEnemy::Create(D3DXVECTOR3(3850.0f, 0.0f, -2700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), SPD_LIFE, CEnemy::ENEMYTYPE_SPEED);

}



//=============================================================================
// 関数名：シーンメッシュフィールドクラスの取得
// 関数の概要：シーンメッシュフィールドクラスを返す
//=============================================================================
CSceneMeshField *CGame::GetSceneMeshField(void)
{
	return m_pMeshField[0];
}

//=============================================================================
// 関数名：プレイヤークラスの取得
// 関数の概要：プレイヤークラスを返す
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

CBoss * CGame::GetBoss(void)
{
	return m_pBoss;
}

//=============================================================================
// 関数名：敵クラスの取得
// 関数の概要：敵クラスを返す
//=============================================================================
CEnemy *CGame::GetEnemy(int nIndex)
{
	return m_pEnemy[nIndex];
}

//=============================================================================
// 関数名：壁クラスの取得
// 関数の概要：壁クラスを返す
//=============================================================================
CSceneMeshWall *CGame::GetWall(int nCnt)
{
	if(m_apMeshWall[nCnt] != NULL)
	{
		return m_apMeshWall[nCnt];
	}
	else
	{
		return NULL;
	}
}

//=============================================================================
// 関数名：コンボゲージクラスの取得
// 関数の概要：コンボゲージクラスを返す
//=============================================================================
CComboGauge * CGame::GetComboGauge(int nCnt)
{
	return m_apComboGauge[nCnt];
}

//===============================================================================
// 関数名：ゲーム状態の設定
// 関数の概要：ゲーム状態を設定する
//===============================================================================
void CGame::SetGameState(GAMESTATE state)
{
	m_GameState = state;
	m_nCntGame = 80;
	CManager::SetScore(CScore::GetScore());
	CManager::SetComboGaugeMax(CComboGauge::GetComboGaugeMax());
	CManager::SetDamage(CPlayer::GetDamage());
	CScene::SetStopBoss(false);
	CScene::SetStop(false);
}


//===============================================================================
// 関数名：ゲーム状態を取得する
// 関数の概要：ゲーム状態を返す
//===============================================================================
CGame::GAMESTATE CGame::GetGameState(void)
{
	return m_GameState;
}

//===============================================================================
// 関数名：ボスを生成するか
// 関数の概要：boolの切り替え
//===============================================================================
void CGame::ChangeBoolCreateBoss(void)
{
	m_bCreateBoss = m_bCreateBoss ? false : true;
}

bool CGame::GetBool(void)
{
	return m_bCreateBoss;
}

