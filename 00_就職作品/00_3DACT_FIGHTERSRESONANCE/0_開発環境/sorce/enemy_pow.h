//=============================================================================
//
// 敵処理[2D ポリゴン] [player.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _ENEMYPOW_H_
#define _ENEMYPOW_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYPOW_PARTS (16)	//敵のパーツ数
#define MAX_KEY_POW (20)			//最大キー数
//*****************************************************************************
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
class CModel;
class CMeshOrbit;
class CSceneGauge;
class CUiGaugeBillBoard;
//*****************************************************************************
//敵のクラス宣言
//*****************************************************************************
class CEnemyPower : public CScene
{
public:
	//キー要素
	typedef struct
	{
		int nFrame;								//フレーム数
		D3DXVECTOR3 pos[MAX_ENEMYPOW_PARTS];		//位置情報
		D3DXVECTOR3 rot[MAX_ENEMYPOW_PARTS];		//角度情報
	}KEY;

	typedef struct
	{
		int nLoop;								//ループするか
		int nNumKey;							//キー数
		KEY aKey[MAX_KEY_POW];						//キー
	}KEY_INFO;

	typedef enum
	{//敵の種類
		ENEMYTYPE_HEAVY = 0,		// 重量型
		ENEMYTYPE_MAX			// 敵の種類の最大数
	}ENEMYTYPE;

	typedef enum
	{//敵の状態
		ENEMYSTATE_NORMAL = 0,	// 通常状態
		ENEMYSTATE_STEEL,		// 硬直状態
		ENEMYSTATE_BOUND,		// バウンド
		ENEMYSTATE_DEATH,		// 死亡
		ENEMYSTATE_APPEAR,		// 出現時状態
		ENEMYSTATE_MAX
	}ENEMYSTATE;
	typedef enum
	{// モーションの種類
		MOTION_NEUTRAL = 0,			// ニュートラル時
		MOTION_MOVE,				// 移動時
		MOTION_LIGHTATTACK,			// 弱攻撃
		MOTION_JUMP,				// ジャンプ時
		MOTION_LANDING,				// 着地時
		MOTION_HEAVYATTACK,			// 強攻撃
		MOTION_NORMALDAMAGE,		// ダメージ(通常)
		MOTION_FLOATDAMAGE,			// ダメージ(浮かし)
		MOTION_IMPACTDAMAGE,		// ダメージ(吹っ飛び)
		MOTION_DEATH,				// 死亡
		MOTION_MAX					// モーションの総数
	}MOTIONSTATE;

	typedef enum
	{// フラグの種類
		ENEMYBOOLTYPE_JUMP = 0,		// ジャンプ
		ENEMYBOOLTYPE_SLIDING,		// スライディング
		ENEMYBOOLTYPE_LANDING,		// 着地
		ENEMYBOOLTYPE_LIGHTATTACK,	// 弱攻撃
		ENEMYBOOLTYPE_HEAVYATTACK,	// 強パンチ
		ENEMYBOOLTYPE_NORMALDAMAGE,	// ダメージ(通常)
		ENEMYBOOLTYPE_MEDIUMDAMAGE,	// ダメージ(中)
		ENEMYBOOLTYPE_FLOATDAMAGE,	// ダメージ(浮かし)
		ENEMYBOOLTYPE_BOUNDDAMAGE,	// ダメージ(バウンド)
		ENEMYBOOLTYPE_IMPACTDAMAGE,	// ダメージ(吹っ飛び)
		ENEMYBOOLTYPE_DEATH,		// 死亡
		ENEMYBOOLTYPE_MAX			// 総数
	}ENEMYBOOLTYPE;

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
		HITTYPE_SKILL,				//超必殺技
		HITTYPE_MAX
	}HITTYPE;

	CEnemyPower(int nPriority = 1, OBJTYPE type = OBJTYPE_ENEMY_POW);	// コンストラクタ
	~CEnemyPower();													// デストラクタ
	static HRESULT Load(void);									// テクスチャの読み込み
	static void Unload(void);									// テクスチャの破棄
	static CEnemyPower *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife, ENEMYTYPE type);		// 敵の生成
	HRESULT Init(void);											// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	void DrawShadow(void);										// 敵の影
	void DrawParts(void);										// 敵のパーツの描画
	void EnemyMove(void);										// 敵の移動
	void EnemyAction(void);										// 敵のアクション
	void EnemyFallen(void);										// 敵の落下
	void EnemyMotion(void);										// 敵のモーション
	void EnemyState(void);										// 敵の状態変化
	void EnemySetMove_Interia(void);							// 敵の移動量の代入と慣性移動
	void EnemyMotionState(bool *bAction);						// モーションの状態設定
	void EnemyRotFixes(D3DXVECTOR3 rot);						// 角度修正
	void EnemyRotFixes(float rot);								// 角度修正
	void EnemyDebugLog(void);									// デバックログ
	void LoadEnemyText(void);									// 敵情報の読み込みとパーツの生成
	void InitEnemy(void);										// 敵情報の初期化
	void InitMotion(void);										// モーション情報の初期化
	void DefaultParts(void);									// パーツの初期設定
	void CollisionObjectEnemy(void);							// オブジェクトとの当たり判定
	void CollisionWallEnemy(void);								// 壁との当たり判定
	void CollisionCylinderEnemy(void);							// オブジェクトとの当たり判定
	void CollisionAttack(void);									// 攻撃の当たり判定
	void CollisionPlayer(void);									// 敵との当たり判定
	void EnemyLifeCheck(void);									//敵の体力のチェック
	void CreateGauge(void);										// ゲージの生成
	void SetGauge(void);										// ゲージ位置の代入
	void UpdateCancelCombo(void);								// キャンセルフレーム
	void UpdateFind(void);										//敵探索
	D3DXVECTOR3 GetPos(void);									// 位置情報の取得
	D3DXVECTOR3 GetRot(void);									// 角度情報の取得
	D3DXVECTOR3 GetMove(void);									// 速度情報の取得
	bool FindPlayer(D3DXVECTOR3 *pos, float fRange);			//敵探索
	bool AttackPlayer(D3DXVECTOR3 *pos, float fRange);			//敵への攻撃
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius);	//敵との当たり判定


private:
	D3DXVECTOR3 m_Partspos[MAX_ENEMYPOW_PARTS];				// パーツの位置
	D3DXVECTOR3 m_Partsrot[MAX_ENEMYPOW_PARTS];				// パーツの角度
	D3DXVECTOR3 m_PartsDefaultpos[MAX_ENEMYPOW_PARTS];		// パーツの位置
	D3DXVECTOR3 m_PartsDefaultrot[MAX_ENEMYPOW_PARTS];		// パーツの角度
	D3DXVECTOR3 m_Motionpos[MAX_ENEMYPOW_PARTS];			// モーション差分代入用位置
	D3DXVECTOR3 m_Motionrot[MAX_ENEMYPOW_PARTS];			// モーション差分代入用角度

	int m_aParent[MAX_ENEMYPOW_PARTS];						// パーツの親
	int m_aIndex[MAX_ENEMYPOW_PARTS];						// パーツの番号

														//敵の基本情報//
														//静的メンバ変数//
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	static char						 m_acFileName[MAX_ENEMYPOW_PARTS][256];	// 読み込む
	static LPD3DXMESH				 m_apMesh[MAX_ENEMYPOW_PARTS];				// メッシュ情報へのポインタ
	static LPD3DXBUFFER				 m_apBuffMat[MAX_ENEMYPOW_PARTS];			// マテリアル情報へのポインタ
	static DWORD					 m_aNumMat[MAX_ENEMYPOW_PARTS];			// マテリアルの情報の数
	static CMeshOrbit *m_pMeshOrbit;										// 軌跡のポインタ
	float m_fLife;															// 敵のライフ

																			//通常の変数//
	CUiGaugeBillBoard  *m_apLifeGauge[3];	// Uiのポインタ
	float m_fEnemyMove;						// 敵の移動量
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
	ENEMYSTATE m_state;						// 敵の状態
	ENEMYTYPE m_type;						// 敵の種類
	CModel *m_apModel[MAX_ENEMYPOW_PARTS];		// キャラクターのパーツ


											/////モーション関係の変数/////
	int m_nNumKey;							// キーの総数
	KEY_INFO *m_pKeyInfo;					// キー情報のポインタ
	KEY_INFO m_aKeyInfo[MOTION_MAX];		// キー情報のポインタ
	MOTIONSTATE m_Motionstate;				// モーションの状態
	MOTIONSTATE m_MotionstateOld;			// 以前のモーションの状態
	bool m_bAction[ENEMYBOOLTYPE_MAX];		// 行動中のフラグ
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