//=============================================================================
//
// ゲーム処理[2D ポリゴン] [game.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
class CModel;
class CMeshCylinder;
class CSceneMeshWall;
class CComboGauge;
class CNumber;
class CTimer;
class CBoss;
//*****************************************************************************
// ゲームのクラス構造体
//*****************************************************************************
class CGame
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,	//何もしていない状態
		GAMESTATE_NORMAL,	//通常状態
		GAMESTATE_CLEAR,	//クリア状態
		GAMESTATE_FAILED,	//失敗状態
		GAMESTATE_END,		//終了状態
		GAMESTATE_MAX		//最大数
	}GAMESTATE;
	CGame();										//コンストラクタ
	~CGame();										//デストラクタ
	 HRESULT Init(void);							//初期化処理
	 void Uninit(void);								//終了処理
	 void Update(void);								//更新処理
	 void Draw(void);								//描画処理
	 void CreateEnemy(void);						//敵の生成
	 static CSceneMeshField *GetSceneMeshField(void);//メッシュフィールドシーンの取得
	 static CPlayer *GetPlayer(void);				//プレイヤーの取得
	 static CBoss *GetBoss(void);				//プレイヤーの取得
	 static CEnemy *GetEnemy(int nIndex);			//敵の取得
	 static CSceneMeshWall *GetWall(int nCnt);		//壁の取得
	 static CComboGauge *GetComboGauge(int nCnt);	//コンボゲージの取得
	 static void SetGameState(GAMESTATE state);		//ゲームの状態の設定
	 static GAMESTATE GetGameState(void);			//ゲームの状態の取得
	 static void ChangeBoolCreateBoss(void);		//ブールの切り替え
	 static bool GetBool(void);						//スイッチの取得

private:
	static CGame *m_pGame;					//ゲーム
	static CSceneMeshField *m_pMeshField[4];//メッシュフィールド
	static CSceneMeshWall *m_apMeshWall[15];//メッシュウォール
	static CComboGauge *m_apComboGauge[2];  //コンボゲージ
	static CMeshCylinder *m_pMeshCylinder;	//シリンダー
	static CBoss *m_pBoss;					//ボス
	static CPlayer *m_pPlayer;				//プレイヤー
	static CEnemy *m_pEnemy[30];			//敵
	static CModel *m_pModel;				//モデル
	static CObject *m_pObject;				//オブジェクト
	static CNumber *m_pNumber;				//数字
	static CTimer *m_pTimer;				//数字
	static CLight *m_pLight;				//ライト

	static GAMESTATE m_GameState;//ゲームの状態
	static int m_nCntGame;//シーンの移り
	int m_nID;
	static bool m_bCreateBoss;
};

#endif