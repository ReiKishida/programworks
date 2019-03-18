//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : 佐藤亮太
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "model.h"
#include "meshField.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY						(200)		// 敵の総数
#define MAX_ENEMY_MODEL					(20)		// 敵の最大モデルの数
#define ENEMY_MAX_KEY					(20)		// 最大のキーフレーム数
#define ENEMY_MAX_MOTION				(20)		// 最大のモーション数
#define ENEMY_MAX_COLLISION_ATTACK		(10)		// 攻撃の当たり判定の最大数
#define ENEMY_MAX_COLLISION_BODY		(10)		// 体の当たり判定の最大数
#define ENEMY_MAX_DATUYOU_TYPE			(3)			// 奪踊コマンドの最大数

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{// 敵の種類
	ENEMY_TYPE_ARMOR = 0,		// 鎧
	ENEMY_TYPE_HITOME,			// 一つ目
	ENEMY_TYPE_HIGECHAN,		// ひげちゃん
	ENEMY_TYPE_HONCHAN,			// 本ちゃん
	ENEMY_TYPE_MAX,				// 総数
}ENEMY_TYPE;

typedef enum
{// 敵の状態の種類
	ENEMY_STATE_NORMAL = 0,		// 通常状態
	ENEMY_STATE_DAMAGE,			// ダメージ状態
	ENEMY_STATE_DEATH,			// 死亡状態
	ENEMY_STATE_MAX				// 状態の総数
}ENEMY_STATE;

typedef enum
{// モーションの種類
	ENEMY_MOTION_NEUTRAL = 0,					// ニュートラル時
	ENEMY_MOTION_MOVE,							// 移動時
	ENEMY_MOTION_ATTACK,						// 通常攻撃
	ENEMY_MOTION_JUMP,							// ジャンプ時
	ENEMY_MOTION_LANDING,						// 着地時
	ENEMY_MOTION_DAMAGE,						// 仰け反り時
	ENEMY_MOTION_FLY,							// 吹っ飛び時
	ENEMY_MOTION_GETUP,							// 起き上がり時
	ENEMY_MOTION_MAX							// モーションの総数
}ENEMY_MOTION;

typedef enum
{// 攻撃の種類
	ENEMY_ATTACKTYPE_ATTACK = 0,				// 通常攻撃
	ENEMY_ATTACKTYPE_MAX						// 攻撃の種類の総数
}ENEMY_ATTACKTYPE;

typedef enum
{// 行動パターンの種類
	ENEMY_ACT_TYPE_NEUTRAL = 0,					// 棒立ち
	ENEMY_ACT_TYPE_LOOK,						// プレイヤーを見る
	ENEMY_ACT_TYPE_WANDER,						// うろつく
	ENEMY_ACT_TYPE_APPROACHES,					// プレイヤーに近づく
	ENEMY_ACT_TYPE_ATTACK,						// 攻撃する
	ENEMY_ACT_TYPE_MAX							// 行動パターンの総数
}ENEMY_ACT_TYPE;

typedef enum
{// boolの種類
	ENEMY_BOOLTYPE_JAMP = 0,				// ジャンプ中かどうか
	ENEMY_BOOLTYPE_LANDING,					// 着地中かどうか
	ENEMY_BOOLTYPE_DAMAGE,					// ダメージ中かどうか
	ENEMY_BOOLTYPE_FLY,						// 吹っ飛び中かどうか
	ENEMY_BOOLTYPE_GETUP,					// 起き上がり中かどうか
	ENEMY_BOOLTYPE_MAX						// 起き上がり中かどうか
}ENEMY_BOOLTYPE;

typedef struct
{// キーフレームの構造体
	__int8 nFrame;							// フレーム数
	D3DXVECTOR3 pos[MAX_ENEMY_MODEL];		// 位置座標
	D3DXVECTOR3 rot[MAX_ENEMY_MODEL];		// 回転座標
}Enemy_KyeInfo;

typedef struct
{// モーションの構造体
	__int8 nLoop;								// ループするかどうか(0 : ループしない / 1 : ループする)
	__int8 nNumKey;								// キー数
	Enemy_KyeInfo Motion[ENEMY_MAX_KEY];		// モーションの情報
	__int8 nAttackPower;						// モーションの攻撃力
	float fDamageSide;							// モーションの水平方向に吹き飛ばす力
	float fDamageVertical;						// モーションの垂直方向に吹き飛ばす力
	
}Enemy_Motion;

typedef struct
{// モデル情報の構造体
	char cIP[50];
	LPD3DXMESH			pMesh;					// メッシュ(頂点情報の集まり)情報へのポインタ
	LPD3DXBUFFER		pBuffMat;				// マテリアル情報へのポインタ
	DWORD				nNumMat;				// マテリアル情報の数
	LPDIRECT3DTEXTURE9	*pTexture;				// テクスチャへのポインタ
	D3DXVECTOR3			vtxMin, vtxMax;			// モデルの最小値・最大値
}EnemyModelAddress;

typedef struct
{// 各パーツのオフセットの構造体
	D3DXVECTOR3 pos;						// 位置
	D3DXVECTOR3 rot;						// 向き
	__int8		nIdxModel;					// 使用モデルのインデックス
	__int8		nIdxParentModel;			// 親モデルのインデックス
}Enemy_ModelInfo;

typedef struct
{// 各パーツの情報
	D3DXMATRIX			mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3			pos;					// 位置
	D3DXVECTOR3			rot;					// 向き
	__int8				nIdxModel;				// 使用モデルのインデックス
	__int8				nIdxParentModel;		// 親モデルのインデックス
}Enemy_Model;

typedef struct
{// 攻撃の当たり判定のオフセットの構造体
	D3DXVECTOR3			pos;					// 位置
	D3DXVECTOR3			rot;					// 向き
	D3DXMATRIX			mtxWorld;				// ワールドマトリックス
	__int8				nIdxParent;				// 親モデルのインデックス
	float				fRadius;				// 半径
	short int			nCollisionStart;		// 当たり判定の始まりのフレーム数
	short int			nCollisionEnd;			// 当たり判定の終わりのフレーム数
	__int8				nMotionType;			// 当たり判定が付くモーションのタイプ
	bool				bUse;					// 当たり判定が使われてるかどうか
}CollisionAttackEnemy;

typedef struct
{// 体の当たり判定のオフセットの構造体
	D3DXVECTOR3			pos;								// 位置
	D3DXVECTOR3			rot;								// 向き
	D3DXMATRIX			mtxWorld;							// ワールドマトリックス
	__int8				nIdxParent;							// 親モデルのインデックス
	float				fRadius;							// 半径
	bool				bUse;								// 当たり判定が使われてるかどうか
}CollisionBodyEnemy;

typedef struct
{// 敵の種類ごとのモデル＆モーションの構造体
	EnemyModelAddress	ModelAddress[MAX_ENEMY_MODEL];					// モデルの情報
	Enemy_ModelInfo		ModelInfo[MAX_ENEMY_MODEL];						// 各パーツのオフセットの構造体
	Enemy_Motion		Motion[ENEMY_MAX_MOTION];						// モーション情報
	int					MaxModel;										// モデルの総数
	int					MaxParts;										// パーツの総数

	// キャラクター個別のステータス
	float				fMoveSpeed;										// 移動速度
	float				fJamp;											// ジャンプ力
	int					nLife;											// 体力
	float				fRadius;										// 半径

	// キャラクターの当たり判定の構造体
	CollisionAttackEnemy CollisionAttack[ENEMY_MAX_COLLISION_ATTACK];	// 攻撃の当たり判定
	int					 nCntMaxCollisionAttack;						// 攻撃の当たり判定の最大数
	CollisionBodyEnemy   CollisionBody[ENEMY_MAX_COLLISION_BODY];		// 体の当たり判定
	int					 nCntMaxCollisionBody;							// 体の当たり判定の最大数

}Enemy_info;

typedef struct
{// 敵の構造体
	D3DXVECTOR3			 pos;										// 位置
	D3DXVECTOR3			 posOld;									// 前回の位置
	D3DXVECTOR3			 InitPos;									// 初期位置
	D3DXVECTOR3			 Move;										// 位置の移動量
	D3DXVECTOR3			 rot;										// 向き
	D3DXVECTOR3			 rotDest;									// 目的の向き
	D3DXMATRIX			 mtxWorld;									// ワールドマトリックス
	
	// 使用している番号の変数
	int					 nIdyShadow;								// 影の番号
	int					 nIdxLight;									// ライトの番号
	int					 nIdxLifeGauge;								// ライフゲージの番号
	int					 nIdxDatuyou;								// 奪踊のUIの番号

	// カウンター関係
	int					 nAttackWaitTime;							// 攻撃の待機時間
	int					 nAttackCounter;							// 攻撃をするまでのカウンター変数
	int					 nFlyCounter;								// 吹っ飛ぶ時間のカウンター変数
	int					 nDownCounter;								// ダウン時のカウンター変数
	int					 nActNowCounter;							// 現在の行動のカウンター
	int					 nActTime;									// 行動するフレーム数を入れる変数
	int					 nCounterState;								// 状態のカウンター変数

	// キャラクターのステータス関係
	ENEMY_STATE			 State;											// 敵の状態
	ENEMY_TYPE			 EnemyType;										// 敵のタイプ	
	int					 nLife;											// 舞奪力
	int					 nMaxLife;										// ライフの最大値
	float				 fMoveSpeed;									// 移動速度
	float				 fJamp;											// 基本のジャンプ力
	float				 fJampPower;									// 現在のジャンプ力
	float				 fRadius;										// 敵の当たり判定の半径
	D3DXVECTOR3			 fFlyPower;										// 吹っ飛び力

	// 行動系の変数
	float				 fActProportion[ENEMY_ACT_TYPE_MAX];			// 敵の行動パターンの割合
	int					 nActType;										// 行動のタイプを入れる変数
	float				 fWanderAngle;									// うろつき時のランダムな角度
	bool				 bMoved;										// 動いたらtreu(鎧のみ)
	// いろいろ
	float				 fPosLeght;										// プレイヤーとの距離
	bool				 bActionFlag[ENEMY_BOOLTYPE_MAX];				// 行動中のフラグ

	Enemy_Model			 aModel[MAX_ENEMY_MODEL];						// モデルの数
	ENEMY_MOTION	 Motionstate;									// モーションの状態
	ENEMY_MOTION	 MotionstateOld;								// 前回のモーションの状態
	CollisionAttackEnemy CollisionAttack[ENEMY_MAX_COLLISION_ATTACK];	// 敵の攻撃の当たり判定
	CollisionBodyEnemy   CollisionBody[ENEMY_MAX_COLLISION_BODY];		// 敵の体の当たり判定
	bool				 bUse;											// 使用しているかどうか

	// モーション制御の変数
	D3DXVECTOR3			posMotion[MAX_ENEMY_MODEL];					// 位置の差分
	D3DXVECTOR3			rotMotion[MAX_ENEMY_MODEL];					// 回転の差分
	int					nCounterKey;								// 現在のキー数
	int					nCounterFrame;								// 現在のフレーム数
	int					nCounterAllFrame;							// モーションの全キーフレームのフレーム数
	int					nFrame;										// 今回のフレーム数
	bool				bEnemyAttack[ENEMY_ATTACKTYPE_MAX];			// 攻撃中かどうか
	bool				bEnemyAttackReady[ENEMY_ATTACKTYPE_MAX];	// 次の攻撃に移るかどうか

	Model				*pModel;									// モデルの情報
	MeshField			*pMeshField;								// メッシュフィールド情報
}Enemy;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void EnemyController(int nCntEnemy);							// 敵の操作
void EnemyModelSet(void);										// 敵モデルの設定
void EnemyMotion(int nCntEnemy);								// 敵のモーション
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE EnemyType, bool bMove);				// 敵の設置
void CollisionEnemy(D3DXVECTOR3 *pos, float fRadius);			// キャラクターが重ならないための当たり判定
void CollisionEnemyBody(D3DXVECTOR3 pos, 
						D3DXVECTOR3 mtxWorld, 
						float fRadius,
						int nPower,
						float fDamageSide,
						float fDamageVertical);					// 敵の体と攻撃の当たり判定
void DeletEnemy(int nCntEnemy,bool bKo);						// 敵の削除
void EnemyLode(void);											// 敵のモデル&モーション情報をテキストファイルから取得
void EnemySetLode(void);										// 敵の配置情報をテキストファイルから取得
Enemy *GetEnemy(void);											// 敵情報の取得

int GetNumEnemy(void);											// 敵の人数を取得

#endif
