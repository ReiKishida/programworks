//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : 岸田怜(KishidaRei)
//=============================================================================
#include "main.h"				//メイン
#include "scene.h"				//シーン
#include "renderer.h"			//レンダリング
#include "scene2D.h"			//シーン2D
#include "manager.h"			//マネージャー
#include "input.h"				//入力
#include "number.h"				// ナンバー
#include "game.h"				//ゲーム
#include "ranking.h"			//ランキング
#include "DebugProc.h"			//デバッグ表示
#include "fade.h"				//フェード
#include "scene_meshfield.h"
#include "rankinglogo.h"
#include "object.h"
#include "sound.h"
#include "rankingnumber.h"
//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CUiRanking *CRanking::m_pUiRanking = NULL;
CObject *CRanking::m_pObject = NULL;				//オブジェクト
CSceneMeshField *CRanking::m_pMeshField = NULL;			//メッシュフィールド
int CRanking::m_nJumpTitleFrame = 0;//タイトル画面へ遷移するときのフレームカウンタ

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{	//何も書かない
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	//数字のテクスチャの読み込み
	CNumber::Load();

	//ランキングロゴのテクスチャの読み込み
	CRankingLogo::Load();


	//ランキングの生成
	CRankingLogo::Create();

	//ランキングの数字を生成
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 280.0f, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	//数字のテクスチャの破棄
	CNumber::Unload();

	//ランキングロゴのテクスチャの破棄
	CRankingLogo::Unload();

	//シーンの破棄
	CScene::ReleaseSceneAll();


}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	m_nJumpTitleFrame++;

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	//{//エンターキーが押されたとき

	//	if (pFade == CFade::FADE_NONE)
	//	{
	//		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	//		CFade::SetFade(CManager::MODE_TITLE);//ランキングへ移行
	//	}

	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
}