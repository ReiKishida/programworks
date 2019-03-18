//=============================================================================
//
// チュートリアル処理[2D ポリゴン] [tutorial.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

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
class CTutorialLogo;
class CTutorial
{
public:
	typedef enum
	{
		TUTORIALSTATE_NONE = 0,	//何もしていない状態
		TUTORIALSTATE_NORMAL,	//通常状態
		TUTORIALSTATE_CLEAR,	//クリア状態
		TUTORIALSTATE_FAILED,	//失敗状態
		TUTORIALSTATE_END,		//終了状態
		TUTORIALSTATE_MAX		//最大数
	}TUTORIALSTATE;
	CTutorial();	//コンストラクタ
	~CTutorial();	//デストラクタ
				//static CTutorial *Create(void);	//ゲームの生成
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
	static CPlayer *GetPlayer(void);
	static CBoss *GetBoss(void);
	static CEnemy *GetEnemy(int nCntEnemy);
	static void SetEnemy(void);
	static void SetTutorialState(TUTORIALSTATE state);
	static TUTORIALSTATE GetTutorialState(void);
	int GetFrame(void);

private:
	static CPlayer *m_pPlayer;//プレイヤーのポインタ
	static CEnemy *m_apEnemy[MAX_ENEMY];	//敵のポインタ
	static CUi *m_apUi[MAX_UI];	//UIのポインタ
	static CBg *m_pBg;	//背景のポインタ
	static CFollowForce *m_pFollowForce;	//フォースのポインタ
	static CForce *m_pForce;	//フォースのポインタ
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

	static CTutorial *m_pTutorial;//チュートリアルのポインタ
	static CTutorialLogo *m_pTutorialLogo;//チュートリアルロゴのポインタ
	static TUTORIALSTATE m_TutorialState;//ゲームの状態
	static int m_nCntTutorial;//シーンの移り
	static int m_nCntFrame;//フレーム数のカウント
};

#endif