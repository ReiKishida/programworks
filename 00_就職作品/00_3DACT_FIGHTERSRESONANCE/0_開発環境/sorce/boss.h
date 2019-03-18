//=============================================================================
//
// 敵処理[2D ポリゴン] [player.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BOSS_PARTS (16)	//敵のパーツ数
#define MAX_KEY (20)			//最大キー数
//*****************************************************************************
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
class CModel;
class CMeshOrbit;
class CSceneGauge;
class CUiGauge;
//*****************************************************************************
//敵のクラス宣言
//*****************************************************************************
class CBoss : public CScene
{
public:
	//キー要素
	typedef struct
	{
		int nFrame;								//フレーム数
		D3DXVECTOR3 pos[MAX_BOSS_PARTS];		//位置情報
		D3DXVECTOR3 rot[MAX_BOSS_PARTS];		//角度情報
	}KEY;

	typedef struct
	{
		int nLoop;								//ループするか
		int nNumKey;							//キー数
		KEY aKey[MAX_KEY];						//キー
	}KEY_INFO;



	typedef enum
	{//敵の状態
		BOSSSTATE_NORMAL = 0,	// 通常状態
		BOSSSTATE_STEEL,		// 硬直状態
		BOSSSTATE_BOUND,		// バウンド
		BOSSSTATE_DEATH,		// 死亡
		BOSSSTATE_APPEAR,		// 出現時状態
		BOSSSTATE_INVISIBLE,
		BOSSSTATE_MAX
	}BOSSSTATE;
	typedef enum
	{// モーションの種類
		MOTION_NEUTRAL = 0,			// ニュートラル時
		MOTION_MOVE,				// 移動時
		MOTION_LIGHTATTACK,			// 弱攻撃
		MOTION_JUMP,				// ジャンプ時
		MOTION_LANDING,				// 着地時
		MOTION_HEAVYATTACK,			// 強攻撃
		MOTION_SKILL,				// 超必殺技
		MOTION_NORMALDAMAGE,		// ダメージ(通常)
		MOTION_FLOATDAMAGE,			// ダメージ(浮かし)
		MOTION_IMPACTDAMAGE,		// ダメージ(吹っ飛び)
		MOTION_DEATH,				// 死亡
		MOTION_MOVEATTACK,			// 移動攻撃
		MOTION_MAX					// モーションの総数
	}MOTIONSTATE;

	typedef enum
	{// フラグの種類
		BOSSBOOLTYPE_JUMP = 0,		// ジャンプ
		BOSSBOOLTYPE_SLIDING,		// スライディング
		BOSSBOOLTYPE_LANDING,		// 着地
		BOSSBOOLTYPE_LIGHTATTACK,	// 弱攻撃
		BOSSBOOLTYPE_HEAVYATTACK,	// 強パンチ
		BOSSBOOLTYPE_SKILL,			// スキル
		BOSSBOOLTYPE_NORMALDAMAGE,	// ダメージ(通常)
		BOSSBOOLTYPE_MEDIUMDAMAGE,	// ダメージ(中)
		BOSSBOOLTYPE_FLOATDAMAGE,	// ダメージ(浮かし)
		BOSSBOOLTYPE_BOUNDDAMAGE,	// ダメージ(バウンド)
		BOSSBOOLTYPE_IMPACTDAMAGE,	// ダメージ(吹っ飛び)
		BOSSBOOLTYPE_DEATH,			// 死亡
		BOSSBOOLTYPE_MOVEATTACK,	// 移動攻撃
		BOSSBOOLTYPE_MAX			// 総数
	}BOSSBOOLTYPE;

	typedef enum
	{// フラグの種類
		HITTYPE_LIGHTPUNCH = 0,		//プレイヤーの攻撃(弱パンチ)
		HITTYPE_MEDIUMPUNCH,		//プレイヤーの攻撃(中パンチ)
		HITTYPE_SHORYUKEN,			//プレイヤーの攻撃(昇竜拳)
		HITTYPE_HEAVYPUNCH,			//プレイヤーの攻撃(強パンチ)
		HITTYPE_LIGHTKICK,			//プレイヤーの攻撃(弱キック)
		HITTYPE_MEDIUMKICK,			//プレイヤーの攻撃(中キック)
		HITTYPE_SPINKICK,			//プレイヤーの攻撃(回転蹴り)
		HITTYPE_HEAVYKICK,			//プレイヤーの攻撃(強キック)
		HITTYPE_SKILL,				//プレイヤーの攻撃(必殺技)
		HITTYPE_MAX
	}HITTYPE;

	CBoss(int nPriority = 1, OBJTYPE type = OBJTYPE_BOSS);	// コンストラクタ
	~CBoss();													// デストラクタ
	static HRESULT Load(void);									// テクスチャの読み込み
	static void Unload(void);									// テクスチャの破棄
	static CBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife);		// 敵の生成
	HRESULT Init(void);											// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	void DrawShadow(void);										// 敵の影
	void DrawParts(void);										// 敵のパーツの描画
	void BossMove(void);										// 敵の移動
	void BossAction(void);										// 敵のアクション
	void BossFallen(void);										// 敵の落下
	void BossMotion(void);										// 敵のモーション
	void BossState(void);										// 敵の状態変化
	void BossSetMove_Interia(void);								// 敵の移動量の代入と慣性移動
	void BossMotionState(bool *bAction);						// モーションの状態設定
	void BossRotFixes(D3DXVECTOR3 rot);							// 角度修正
	void BossRotFixes(float rot);								// 角度修正
	void BossDebugLog(void);									// デバックログ
	void LoadBossText(void);									// 敵情報の読み込みとパーツの生成
	void InitBoss(void);										// 敵情報の初期化
	void InitMotion(void);										// モーション情報の初期化
	void DefaultParts(void);									// パーツの初期設定
	void CollisionObjectBoss(void);								// オブジェクトとの当たり判定
	void CollisionWallBoss(void);								// 壁との当たり判定
	void CollisionCylinderBoss(void);							// オブジェクトとの当たり判定
	void CollisionAttack(void);									// 攻撃の当たり判定
	void CollisionPlayer(void);									// ボスとの当たり判定
	void BossLifeCheck(void);									//ボスの体力のチェック
	void CreateGauge(void);										// ゲージの生成
	void UpdateCancelCombo(void);								// キャンセルフレーム
	void UpdateFind(void);										//ボス探索
	D3DXVECTOR3 GetPos(void);									// 位置情報の取得
	D3DXVECTOR3 GetRot(void);									// 角度情報の取得
	D3DXVECTOR3 GetMove(void);									// 速度情報の取得
	bool FindPlayer(D3DXVECTOR3 *pos, float fRange);			//ボス探索
	bool AttackPlayer(D3DXVECTOR3 *pos, float fRange);			//ボスへの攻撃
	bool CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius);	//敵との当たり判定


private:
	D3DXVECTOR3 m_Partspos[MAX_BOSS_PARTS];				// パーツの位置
	D3DXVECTOR3 m_Partsrot[MAX_BOSS_PARTS];				// パーツの角度
	D3DXVECTOR3 m_PartsDefaultpos[MAX_BOSS_PARTS];		// パーツの位置
	D3DXVECTOR3 m_PartsDefaultrot[MAX_BOSS_PARTS];		// パーツの角度
	D3DXVECTOR3 m_Motionpos[MAX_BOSS_PARTS];			// モーション差分代入用位置
	D3DXVECTOR3 m_Motionrot[MAX_BOSS_PARTS];			// モーション差分代入用角度

	int m_aParent[MAX_BOSS_PARTS];						// パーツの親
	int m_aIndex[MAX_BOSS_PARTS];						// パーツの番号

														//敵の基本情報//
														//静的メンバ変数//
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	static char						 m_acFileName[MAX_BOSS_PARTS][256];	// 読み込む
	static LPD3DXMESH				 m_apMesh[MAX_BOSS_PARTS];				// メッシュ情報へのポインタ
	static LPD3DXBUFFER				 m_apBuffMat[MAX_BOSS_PARTS];			// マテリアル情報へのポインタ
	static DWORD					 m_aNumMat[MAX_BOSS_PARTS];			// マテリアルの情報の数
	static CMeshOrbit *m_pMeshOrbit;										// 軌跡のポインタ
	float m_fLife;															// 敵のライフ
	float m_fSkill;														// 敵のライフ

																			//通常の変数//
	CUiGauge *m_apLifeGauge[4];				// Uiのポインタ
	float m_fBossMove;						// 敵の移動量
	float m_fDestAngle;						// 目的の角度
	float m_fAngle;							// 角度
	float m_fHeight;						// 高さ
	bool m_bUseDraw;						// 描画するか
	bool m_bJump;							// ジャンプしているか
	bool m_bEffect;							// 残像の有無
	int m_StateCnt;							// 状態変化のカウンタ
	int m_Jumptime;							// 滞空している時間
	int	m_nId;								// 判定用のID
	int m_nCancelFlame;						// キャンセルフレーム
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 現在の位置
	D3DXVECTOR3 m_posold;					// 以前の位置
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_jumppower;				// ジャンプ量
	BOSSSTATE m_state;						// 敵の状態
	CModel *m_apModel[MAX_BOSS_PARTS];		// キャラクターのパーツ


											/////モーション関係の変数/////
	int m_nNumKey;							// キーの総数
	KEY_INFO *m_pKeyInfo;					// キー情報のポインタ
	KEY_INFO m_aKeyInfo[MOTION_MAX];		// キー情報のポインタ
	MOTIONSTATE m_Motionstate;				// モーションの状態
	MOTIONSTATE m_MotionstateOld;			// 以前のモーションの状態
	bool m_bAction[BOSSBOOLTYPE_MAX];		// 行動中のフラグ
	bool m_bMotionBlend;					// ブレンドするか
	int m_nKey;								// 現在のキーナンバー
	int m_nAllFrame;						// 現在のキーナンバー
	int m_nCountMotion;						// モーションカウンター
	int m_nCounterFrame;					// フレームのカウンター
	int m_nCounterAllFrame;					// モーションカウンター
	int m_nCounterKey;						// キーのカウンター
	int m_nFrame;							// 今のフレーム
	int m_nCooltime;						// クールタイム
	float m_fRadius;						// 当たり判定の半径
};


#endif