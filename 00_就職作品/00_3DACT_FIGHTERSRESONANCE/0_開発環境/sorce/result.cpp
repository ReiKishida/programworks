//=============================================================================
//
// リザルトの処理 [result.h]
// Author : 岸田怜(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "result.h"
#include "DebugProc.h"
#include "number.h"
#include "fade.h"
#include "scene_meshfield.h"
#include "resultlogo.h"
#include "object.h"
#include "sound.h"
#include "timer.h"
#include "score.h"
#include "combo.h"
#include "comboscore.h"
#include "damagescore.h"
#include "finalscore.h"
#include "clearrank.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CUiResult *CResult::m_pUiResult = NULL;					//リザルト
CObject *CResult::m_pObject = NULL;						//オブジェクト
CSceneMeshField *CResult::m_pMeshField = NULL;			//メッシュフィールド
int CResult::m_nCntState = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	//リザルトのUIテクスチャの読み込み
	CResultLogo::Load();

	//数字テクスチャの読み込み
	CNumber::Load();

	//コンボ表記のテクスチャの読み込み
	CCombo::Load();

	//クリアランクのテクスチャの読み込み
	CClearRank::Load();

	//スコアの生成
	CScore::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), CScore::SCORETYPE_RESULT);

	//タイマー
	CResultTimer::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT/2 - 90.0f,0.0f), CTimer::TIMEMODE_RESULT);

	//コンボスコア
	CComboScore::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT / 2 + 30.0f, 0.0f));

	//ダメージスコア
	CDamageScore::Create(D3DXVECTOR3(170.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f));

	//リザルトのUIテクスチャの生成
	CResultLogo::Create();

	m_nCntState = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//リザルトのUIテクスチャの読み込み
	CResultLogo::Unload();

	//数字テクスチャの読み込み
	CNumber::Unload();

	//コンボ表記のテクスチャの読み込み
	CCombo::Unload();

	//クリアランクのテクスチャの読み込み
	CClearRank::Unload();


	//シーンの破棄
	CScene::ReleaseSceneAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	m_nCntState++;


	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (m_nCntState == 20)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);
		//スコアボーナス
		CClearRank::Create(D3DXVECTOR3(520.0f, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), CClearRank::RANKMODE_SCORE);
	}
	if (m_nCntState == 40)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);
		//タイムボーナス
		CClearRank::Create(D3DXVECTOR3(450.0f, SCREEN_HEIGHT / 2 - 90.0f, 0.0f), CClearRank::RANKMODE_TIME);
	}
	if (m_nCntState == 60)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);

		//コンボボーナス
		CClearRank::Create(D3DXVECTOR3(360.0f, SCREEN_HEIGHT / 2 + 30.0f, 0.0f), CClearRank::RANKMODE_COMBO);
	}
	if (m_nCntState == 80)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);
		//ダメージボーナス
		CClearRank::Create(D3DXVECTOR3(360.0f, SCREEN_HEIGHT / 2 + 140.0f, 0.0f), CClearRank::RANKMODE_DAMAGE);
	}
	if (m_nCntState == 120)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);
		//最終スコア
		CFinalScore::Create(D3DXVECTOR3(280.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f));
	}
	if (m_nCntState == 180)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_FIRE);
		//最終ランク
		CClearRank::Create(D3DXVECTOR3(900.0f, SCREEN_HEIGHT / 2 + 270.0f, 0.0f), CClearRank::RANKMODE_FINAL);
		CResultLogo::SwitchEnable();
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
}