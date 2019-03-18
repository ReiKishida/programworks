//=============================================================================
//
// マネージャー処理 [manager.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
class CFade;
class CPause;
class CBoss;
class CMenuRanking;
class CTutorial;
class CInputX;


class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,		//タイトル画面
		MODE_TUTORIAL,		//チュートリアル画面
		MODE_MENU,			//ゲーム画面
		MODE_GAME,			//ゲーム画面
		MODE_RESULT,		//リザルト画面
		MODE_MENURANKING,		//ランキング画面
		MODE_RANKING,		//ランキング画面
		MODE_MAX			//モードの最大数
	}MODE;
	CManager();	//コンストラクタ
	~CManager();//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static CInputKeyboard *GetInputKeyboard(void);	//キーボードの取得
	static CInputX *GetInputX(void);	//XInputの取得
	static CRenderer *GetRenderer(void);	//レンダラーの取得
	static CPlayer *GetPlayer(void);	//プレイヤーの取得
	static CEnemy *GetEnemy(void);	//敵の取得
	static CBoss *GetBoss(void);	//ボスの取得
	static CSound *GetSound(void);	//サウンドの取得
	static CScore *GetScore(void);	//敵の取得
	static void SetMode(MODE mode);//モードの設定
	static MODE GetMode(void);//モードの取得
	//ハイスコア
	static void SetHighScore(int nValue);
	static int GetHighScoreNum(void);
	//スコア
	static void SetScore(int nValue);
	static int GetScoreNum(void);
private:
	static CRenderer *m_pRenderer;	//レンダラのポインタ
	static CInputKeyboard *m_pInputKeyboard;	//キーボードのポインタ
	static CInputX *m_pInputX;	//キーボードのポインタ
	static CPlayer *m_pPlayer;//プレイヤーのポインタ
	static CEnemy *m_pEnemy;	//敵のポインタ
	static CBoss *m_pBoss;//ボスのポインタ
	static CSound *m_pSound; //サウンドのポインタ
	static CScore *m_pScore; //スコアのポインタ
	static CTitle *m_pTitle;//タイトル
	static CGame *m_pGame;//ゲーム
	static CTutorial *m_pTutorial;//リザルト
	static CMenu *m_pMenu;//メニュー
	static CResult *m_pResult;//リザルト
	static CRanking *m_pRanking;//ランキング
	static CMenuRanking *m_pMenuRanking;//メニューランキング
	static CFade *m_pFade;//フェード
	static CPause *m_pPause;//ポーズ
	static MODE m_mode;	//モード
	//static bool m_bPause;//ポーズ
	static int m_nScore;	//スコア
	static int m_nHighScore;	//ハイスコア

};


#endif