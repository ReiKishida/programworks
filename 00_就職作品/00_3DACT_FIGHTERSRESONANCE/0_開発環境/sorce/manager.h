//=============================================================================
//
// マネージャー処理 [manager.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
//前方宣言(名称に注意！なんでも通るから！)
class CScene2D;
class CScene;
class CSceneX;
class CSceneBillBoard;
class CSceneMeshField;
class CSound;
class CLight;
class CCamera;
class CDebugProc;
class CPlayer;
class CEnemy;
class CModel;
class CObject;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;
class CSound;
class CInputX;
class CInputGamePad;
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// マネージャーのクラス構造体
//*****************************************************************************
class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX
	}MODE;
	CManager();	//コンストラクタ
	~CManager();//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理

	//静的メンバ関数
	static CInputKeyboard *GetInputKeyboard(void);	//キーボードの取得
	static CRenderer *GetRenderer(void);			//レンダラーの取得
	static CInputGamePad *GetInputGamePad(void);	//ゲームパッドの取得
	static CCamera *GetCamera(void);				//カメラの取得
	static CSound *GetSound(void);					//サウンドの取得
	static CInputX *GetInput(void);					//ジョイパッド
	static void SetMode(MODE mode);					//モードの設定
	static MODE GetMode(void);						//モードの取得


	// タイマーで使用
	static void SetTimer(int nTenMinutes, int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma);
	static int GetTimer(void);
	static int GetTenMinutes(void) { return m_nTenMinutes; }			//10分台
	static int GetOneMinutes(void) { return m_nOneMinutes; }		  //1分台
	static int GetTenSeconds(void) { return m_nTenSeconds; }		  //10秒台
	static int GetOneSeconds(void) { return m_nOneSeconds; }		 //10秒台
	static int GetTenComma(void) { return m_nTenComma; }			 //コンマ秒
	static int GetOneComma(void) { return m_nOneComma; }		//コンマ秒

	// スコアで使用
	static void SetScore(int nScore);
	static int GetScore(void) { return m_nScore; }

	// 最終スコアで使用
	static void SetFinalScore(int nScore);
	static int GetFinalScore(void) { return m_nFinalScore; }


	//コンボ表記で使用
	static void SetComboGaugeMax(float fValue);
	static float GetComboGaugeMax(void) { return m_fComboGaugeMax; }

	//ダメージ表記で使用
	static void SetDamage(float fValue);
	static float GetDamage(void) { return m_fDamage; }




	//=======================================================================
	//	ボーナス
	//=======================================================================
	// スコアで使用
	static void SetScoreBonus(int nScore);
	static int GetScoreBonus(void) { return m_nScoreBonus; }

	// 最終スコアで使用
	static void SetFinalScoreBonus(int nScore);
	static int GetFinalScoreBonus(void) { return m_nFinalScoreBonus; }

	//タイムボーナスで使用
	static void SetTimeBonus(int nScore);
	static int GetTimeBonus(void) { return m_nTimerBonus; }


	//コンボ表記で使用
	static void SetComboGaugeMaxBonus(int nScore);
	static int GetComboGaugeMaxBonus(void) { return m_nComboGaugeMaxBonus; }

	//ダメージ表記で使用
	static void SetDamageBonus(int nScore);
	static int GetDamageBonus(void) { return m_nDamageBonus; }


private:
	static CTitle *m_pTitle;					//タイトル
	static CTutorial *m_pTutorial;				//チュートリアル
	static CGame *m_pGame;						//ゲーム
	static CResult *m_pResult;					//リザルト
	static CRanking *m_pRanking;				//ランキング
	static CSound *m_pSound;					//サウンドのポインタ
	static CRenderer *m_pRenderer;				//レンダラのポインタ
	static CInputKeyboard *m_pInputKeyboard;	//キーボードのポインタ
	static MODE m_mode;							//モード
	static CDebugProc *m_pDebugProc;			//デバッグ表示
	static CCamera *m_pCamera;					//カメラ
	static CLight *m_pLight;					//ライト
	static CInputX *m_pInputX;					//ジョイパッド
	static CInputGamePad *m_pInputGamePad;		// ゲームパッド
	bool m_bDebug;								//デバッグ用


	// タイマーの値を保存する為に使用
	static int m_nTimer;							//タイマー
	static int m_nTenMinutes;						//10分台
	static int m_nOneMinutes;						//1分台
	static int m_nTenSeconds;						//10秒台
	static int m_nOneSeconds;						//10秒台
	static int m_nTenComma;							//コンマ秒
	static int m_nOneComma;							//コンマ秒

	// スコアの値を保存する為に使用
	static int m_nScore;									// スコアに値を返す

	// スコアの値を保存する為に使用
	static int m_nFinalScore;									// スコアに値を返す


	// コンボゲージの値を保存する為に使用
	static float m_fComboGaugeMax;									// コンボゲージに値を返す

	// ダメージの値を保存する為に使用
	static float m_fDamage;									// ダメージに値を返す

//=======================================================================
//	ボーナス
//=======================================================================
// スコアの値を保存する為に使用
	static int m_nScoreBonus;									// スコアに値を返す

	//最終スコアの値を保存する為に使用
	static int m_nFinalScoreBonus;									// スコアに値を返す


	static int m_nTimerBonus;							//タイマー


	// コンボゲージの値を保存する為に使用
	static int m_nComboGaugeMaxBonus;									// コンボゲージに値を返す

	// ダメージの値を保存する為に使用
	static int m_nDamageBonus;									// ダメージに値を返す


};


#endif