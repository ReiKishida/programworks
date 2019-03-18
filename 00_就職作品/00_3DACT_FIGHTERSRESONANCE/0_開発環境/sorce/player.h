//=============================================================================
//
// プレイヤー処理[2D ポリゴン] [player.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER_PARTS (16)	//プレイヤーのパーツ数
#define MAX_PLAYER_UI (6)		//プレイヤーのパーツ数
#define MAX_KEY (20)			//最大キー数
#define MAX_ATTACKPARTS (4)		//攻撃を行うパーツ数
//*****************************************************************************
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
class CModel;
class CMeshOrbit;
class CUiGauge;
class CComboGauge;
class CSceneGauge;
class CCollision;
//*****************************************************************************
//プレイヤーのクラス宣言
//*****************************************************************************
class CPlayer : public CScene
{
public:
	//キー要素
	typedef struct
	{//キー情報の構造体
		int nFrame;								//フレーム数
		D3DXVECTOR3 pos[MAX_PLAYER_PARTS];		//位置情報
		D3DXVECTOR3 rot[MAX_PLAYER_PARTS];		//角度情報
	}KEY;

	typedef struct
	{//モーションの構造体
		int nLoop;				//ループするか
		int nNumKey;			//キー数
		KEY aKey[MAX_KEY];		//キー
	}KEY_INFO;

	typedef struct
	{// 攻撃の当たり判定のオフセットの構造体
		D3DXVECTOR3			pos;				// 位置
		D3DXMATRIX			mtxWorld;			// ワールドマトリックス
		int					nIdxParent;			// 親モデルのインデックス
		float				fRadius;			// 半径
		short int			nCollisionStart;	// 当たり判定の始まりのフレーム数
		short int			nCollisionEnd;		// 当たり判定の終わりのフレーム数
	}CollisionAttackPlayer;


	typedef enum
	{//プレイヤーの状態
		PLAYERSTATE_NORMAL = 0,		//通常状態
		PLAYERSTATE_STEEL,			//硬直状態
		PLAYERSTATE_DAMAGE,			//ダメージ状態
		PLAYERSTATE_INVISIBLE,		//無敵状態
		PLAYERSTATE_APPEAR,			//出現時状態
		PLAYERSTATE_DEATH,			//死亡状態
		PLAYERSTATE_MAX				//状態の最大数
	}PLAYERSTATE;

	typedef enum
	{// モーションの種類
		MOTION_NEUTRAL = 0,			// ニュートラル時
		MOTION_MOVE,				// 移動時
		MOTION_SLIDING,				// スライディング
		MOTION_JUMP,				// ジャンプ時
		MOTION_LANDING,				// 着地時
		MOTION_LIGHTPUNCH,			// 弱パンチ
		MOTION_MIDDLEPUNCH,			// 中パンチ
		MOTION_SHORYUKEN,			// 昇竜拳
		MOTION_HEAVYPUNCH,			// 強パンチ
		MOTION_LIGHTKICK,			// 弱キック
		MOTION_MIDDLEKICK,			// 中キック
		MOTION_SPINKICK,			// 回転キック
		MOTION_HEAVYKICK,			// 強キック
		MOTION_SMALLDAMAGE,			// ダメージ(小)
		MOTION_LARGEDAMAGE,			// ダメージ(大)
		MOTION_SKILL,				// スタン
		MOTION_DEATH,				// 死亡時
		MOTION_MAX					// モーションの総数
	}PLAYER_MOTIONSTATE;

	typedef enum
	{// フラグの種類
		PLAYERBOOLTYPE_JUMP = 0,	// ジャンプ
		PLAYERBOOLTYPE_SLIDING,		// スライディング
		PLAYERBOOLTYPE_LANDING,		// 着地
		PLAYERBOOLTYPE_LIGHTPUNCH,	// 弱パンチ
		PLAYERBOOLTYPE_MIDDLEPUNCH,	// 中パンチ
		PLAYERBOOLTYPE_SHORYUKEN,	// 昇竜拳
		PLAYERBOOLTYPE_HEAVYPUNCH,	// 強パンチ
		PLAYERBOOLTYPE_LIGHTKICK,	// 弱キック
		PLAYERBOOLTYPE_MIDDLEKICK,	// 中キック
		PLAYERBOOLTYPE_SPINKICK,	// 回転キック
		PLAYERBOOLTYPE_HEAVYKICK,	// 強キック
		PLAYERBOOLTYPE_SMALLDAMAGE,	// ダメージ(小)
		PLAYERBOOLTYPE_MEDIUMDAMAGE,// ダメージ(中)
		PLAYERBOOLTYPE_LARGEDAMAGE,	// ダメージ(大)
		PLAYERBOOLTYPE_SKILL,		// 超必殺技
		PLAYERBOOLTYPE_DEATH,		// 死亡
		PLAYERBOOLTYPE_MAX			// 総数
	}PLAYERBOOLTYPE;

	typedef enum
	{// フラグの種類
		ATTACKBOOLTYPE_PUNCH_0,			// 1段目
		ATTACKBOOLTYPE_PUNCH_1,			// 2段目
		ATTACKBOOLTYPE_PUNCH_2,			// 3段目
		ATTACKBOOLTYPE_PUNCH_HEAVY,		// 強攻撃
		ATTACKBOOLTYPE_PUNCH_MAX		// 総数
	}ATTACKBOOLTYPE_PUNCH;

	typedef enum
	{// フラグの種類
		ATTACKBOOLTYPE_KICK_0,			// 1段目
		ATTACKBOOLTYPE_KICK_1,			// 2段目
		ATTACKBOOLTYPE_KICK_2,			// 3段目
		ATTACKBOOLTYPE_KICK_HEAVY,		// 強攻撃
		ATTACKBOOLTYPE_KICK_MAX			// 総数
	}ATTACKBOOLTYPEKICK;

	typedef enum
	{// フラグの種類
		HITTYPE_LIGHTATTACK = 9,	//敵の攻撃(弱)
		HITTYPE_HEAVYATTACK,		//敵の攻撃(強)
		HITTYPE_BOSS_LIGHTATTACK,	//ボスの攻撃(弱攻撃)
		HITTYPE_BOSS_HEAVYATTACK,	//ボスの攻撃(強攻撃)
		HITTYPE_BOSS_SKILLATTACK_S,	//ボスの攻撃(超必殺技_小)
		HITTYPE_BOSS_SKILLATTACK_L,	//ボスの攻撃(超必殺技_大)

		HITTYPE_MAX
	}HITTYPE;

	CPlayer(int nPriority = 1, OBJTYPE type = OBJTYPE_PLAYER);	// コンストラクタ
	~CPlayer();													// デストラクタ
	static HRESULT Load(void);									// テクスチャの読み込み
	static void Unload(void);									// テクスチャの破棄
	static CPlayer *Create(D3DXVECTOR3 pos);					// プレイヤーの生成
	HRESULT Init(void);											// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	void DrawShadow(void);										// プレイヤーの影
	void DrawParts(void);										// プレイヤーのパーツの描画
	void PlayerMove(void);										// プレイヤーの移動
	void PlayerAction(void);									// プレイヤーのアクション
	void PlayerFallen(void);									// プレイヤーの落下
	void PlayerMotion(void);									// プレイヤーのモーション
	void PlayerState(void);										// プレイヤーの状態変化
	void PlayerSetMove_Interia(void);							// プレイヤーの移動量の代入と慣性移動
	void PlayerMotionState(bool *bAction);						// モーションの状態設定
	void PlayerRotFixes(D3DXVECTOR3 rot);						// 角度修正
	void PlayerRotFixes(float rot);								// 角度修正
	void LoadPlayerText(void);									// プレイヤー情報の読み込みとパーツの生成
	void InitPlayer(void);										// プレイヤー情報の初期化
	void InitMotion(void);										// モーション情報の初期化
	void CreateGauge(void);										// ゲージの生成
	void DefaultParts(void);									// パーツの初期設定
	void CollisionAttack(void);									//攻撃との当たり判定
	void CollisionObjectPlayer(void);							// オブジェクトとの当たり判定
	void CollisionWallPlayer(void);								// 壁との当たり判定
	void CollisionCylinderPlayer(void);							// オブジェクトとの当たり判定
	void CollisionEnemy(void);									// 敵との当たり判定
	void PlayerDebugLog(void);									// デバッグログ
	void PlayerLifeCheck(void);									//プレイヤーの体力のチェック
	void PlayerSkillCheck(void);								//プレイヤーのスキルのチェック
	void CreateParticleFlame_SpinKick(D3DMATRIX *mtx);								//炎のパーティクル生成
	void CreateParticleFlame_Shoryu(D3DMATRIX *mtx);								//炎のパーティクル生成
	void CreateParticleFlame_Skill(D3DMATRIX *mtx);								//炎のパーティクル生成

	void CreateParticleDust(void);								//土煙のパーティクル生成
	void CreateParticleDust_Shoryu(void);								//土煙のパーティクル生成

	static float GetDamage(void);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius);	//敵との当たり判定
	D3DXVECTOR3 GetPos(void);									// 位置情報の取得
	D3DXVECTOR3 GetRot(void);									// 角度情報の取得
	D3DXVECTOR3 GetMove(void);									// 速度情報の取得
	static CUiGauge *GetUiGauge(int nIndex);									//ゲージUIを返す
	static void SetSkill(float fSkill);	//スキルの代入
private:
	static D3DXVECTOR3 m_Partspos[MAX_PLAYER_PARTS];			// パーツの位置
	static D3DXVECTOR3 m_Partsrot[MAX_PLAYER_PARTS];			// パーツの角度
	static D3DXVECTOR3 m_PartsDefaultpos[MAX_PLAYER_PARTS];		// パーツの位置
	static D3DXVECTOR3 m_PartsDefaultrot[MAX_PLAYER_PARTS];		// パーツの角度
	static D3DXVECTOR3 m_Motionpos[MAX_PLAYER_PARTS];			// モーション差分代入用位置
	static D3DXVECTOR3 m_Motionrot[MAX_PLAYER_PARTS];			// モーション差分代入用角度

	static int m_aParent[MAX_PLAYER_PARTS];						// パーツの親
	static int m_aIndex[MAX_PLAYER_PARTS];						// パーツの番号
	//プレイヤーの基本情報//
	//静的メンバ変数//
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	static char						 m_acFileName[MAX_PLAYER_PARTS][256];	// 読み込む
	static LPD3DXMESH				 m_apMesh[MAX_PLAYER_PARTS];			// メッシュ情報へのポインタ
	static LPD3DXBUFFER				 m_apBuffMat[MAX_PLAYER_PARTS];			// マテリアル情報へのポインタ
	static DWORD					 m_aNumMat[MAX_PLAYER_PARTS];			// マテリアルの情報の数
	static CMeshOrbit *m_pMeshOrbit;										// 軌跡のポインタ
	static CUiGauge *m_apUi[MAX_PLAYER_UI];									// Uiのポインタ
	static CSceneGauge *m_apGauge[MAX_PLAYER_UI];		 					// Uiのポインタ
	static CCollision *m_pCollisionAttack;									// 攻撃時の当たり判定のポインタ
	static float m_fLife;													// プレイヤーのライフ
	static float m_fSkill;													// スタン
	static float m_fDamage;													// プレイヤーのライフ
	//通常の変数//

	float m_fPlayerMove;					// プレイヤーの移動量
	float m_fDestAngle;						// 目的の角度
	float m_fAngle;							// 角度
	float m_fComboValue;					//コンボゲージ量
	bool m_bUseDraw;						// 描画するか
	bool m_bJump;							// ジャンプしているか
	bool m_bEffect;							// 残像の有無
	int m_StateCnt;							// 状態変化のカウンタ
	int m_Jumptime;							// 滞空している時間
	int	m_nId;								// 判定用のID
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 現在の位置
	D3DXVECTOR3 m_posold;					// 以前の位置
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_jumppower;				// ジャンプ量
	PLAYERSTATE m_state;					// プレイヤーの状態
	CModel *m_apModel[MAX_PLAYER_PARTS];	// キャラクターのパーツ


	/////モーション関係の変数/////
	int m_nNumKey;									// キーの総数
	KEY_INFO *m_pKeyInfo;							// キー情報のポインタ
	KEY_INFO m_aKeyInfo[MOTION_MAX];				// キー情報のポインタ
	PLAYER_MOTIONSTATE m_Motionstate;				// モーションの状態
	PLAYER_MOTIONSTATE m_MotionstateOld;			// 以前のモーションの状態
	bool m_bAction[PLAYERBOOLTYPE_MAX];				// 行動中のフラグ
	bool m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_MAX];	// 攻撃中のフラグ[パンチ]
	bool m_bAttackKick[ATTACKBOOLTYPE_KICK_MAX];	// 攻撃中のフラグ[キック]
	bool m_bMotionBlend;							// ブレンドするか
	int m_nKey;										// 現在のキーナンバー
	int m_nAllFrame;								// 現在のキーナンバー
	int m_nCountMotion;								// モーションカウンター
	int m_nCounterFrame;							// フレームのカウンター
	int m_nCounterAllFrame;							// モーションカウンター
	int m_nCounterKey;								// キーのカウンター
	int m_nFrame;									// 今のフレーム
	float m_fRadius;								// 当たり判定の半径
	float m_fHeight;								// 高さ
};


#endif