//=============================================================================
//
// ゲーム処理[2D ポリゴン] [game.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY (50)
#define MAX_UI (12)

#include "main.h"
class CUi;
class CForce;
class CFollowForce;
class CCutin;
class CRemaining;
class CParticle;
class CBoss;
class CForceNumber;
class CHighScore;
class CPause;
class CLog;

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
	CGame();	//コンストラクタ
	~CGame();	//デストラクタ
	//static CGame *Create(void);	//ゲームの生成
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
	static CPlayer *GetPlayer(void);
	static CBoss *GetBoss(void);
	static CEnemy *GetEnemy(int nCntEnemy);
	static void SetEnemy(void);
	static void SetGameState(GAMESTATE state);
	static GAMESTATE GetGameState(void);
	int GetFrame(void);

private:
	static CPlayer *m_pPlayer;//プレイヤーのポインタ
	static CEnemy *m_apEnemy[MAX_ENEMY];	//敵のポインタ
	static CUi *m_apUi[MAX_UI];	//UIのポインタ
	static CBg *m_pBg;	//背景のポインタ
	static CFollowForce *m_pFollowForce;	//フォースのポインタ
	static CForce *m_pForce;	//フォースのポインタ
	static CLog *m_pLog;	//フォースのポインタ
	static CBullet *m_pBullet;	//弾のポインタ
	static CEffect *m_pEffect;	//エフェクトのポインタ
	static CExplosion *m_pExplosion;	//爆発のポインタ
	static CNumber *m_pNumber;	//数字のポインタ
	static CCutin *m_pCutin;	//カットインのポインタ
	static CRemaining *m_pRemaining;	//残機数のポインタ
	static CParticle *m_pParticle;	//パーティクルのポインタ
	static CBoss *m_pBoss;	//ボスのポインタ
	static CForceNumber *m_pForceNumber;	//フォース数のポインタ
	static CHighScore *m_pHighScore;	//ハイスコアのポインタ
	static CScore *m_pScore;	//スコアのポインタ

	static CGame *m_pGame;//ゲームのポインタ
	static GAMESTATE m_GameState;//ゲームの状態
	static int m_nCntGame;//シーンの移り
	static int m_nCntFrame;//フレーム数のカウント
};

#endif