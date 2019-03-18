////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プレイヤー処理 [player.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "meshField.h"
#include "sound.h"
#include "model.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_PLAYER		(1)			// 最大数
#define MAX_PLAYERMODEL	(18)
#define MAX_PLAYER_KEY	(20)	// モーション1つに対するキーフレームの最大数
#define MAX_MOTION		(30)	// モーション数
#define MAX_COLLISION	(50)	// 判定最大数

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{// モーションの種類
	PLAYER_MOTION_TYPE_NEUTRAL = 0,					// ニュートラル時
	PLAYER_MOTION_TYPE_MOVE,						// 移動時
	PLAYER_MOTION_TYPE_JUMP,						// ジャンプ時
	PLAYER_MOTION_TYPE_LANDING,						// 着地時
	PLAYER_MOTION_TYPE_DAMAGE,						// ダメージ(仰け反り)時
	PLAYER_MOTION_TYPE_FLY,							// ダメージ(吹っ飛び)時
	PLAYER_MOTION_TYPE_GETUP,						// 起き上がり時
	PLAYER_MOTION_TYPE_STEP,						// ステップ(回避)時
	PLAYER_MOTION_TYPE_CHANGE,						// 変える
	PLAYER_MOTION_TYPE_ATTACK_0,						// 攻撃0
	PLAYER_MOTION_TYPE_ATTACK_1,						// 攻撃0
	PLAYER_MOTION_TYPE_ATTACK_2,						// 攻撃0
	PLAYER_MOTION_TYPE_ATTACK_BIG,						// 強攻撃
	PLAYER_MOTION_TYPE_MAX							// モーションの総数
}PlayerMotionType;

typedef enum
{// 攻撃の種類
	PLAYER_ATTACK_TYPE_ATTACK_0 = 0,				// 攻撃0
	PLAYER_ATTACK_TYPE_ATTACK_1 ,					// 攻撃1
	PLAYER_ATTACK_TYPE_ATTACK_2,					// 攻撃2
	PLAYER_ATTACK_TYPE_ATTACK_BIG ,					// 強攻撃
	PLAYER_ATTACK_TYPE_MAX						// 攻撃の種類の総数
}PlayerAttackType;
typedef enum
{// boolの種類
	PLAYER_BOOL_RUN = 0,			// 走る
	PLAYER_BOOL_JUMP,				// ジャンプ
	PLAYER_BOOL_LANDING,			// 着地
	PLAYER_BOOL_DAMAGE,				// ダメージ
	PLAYER_BOOL_FLY,				// 吹っ飛び
	PLAYER_BOOL_GETUP,				// 起き上がり
	PLAYER_BOOL_STEP,				// ステップ
	PLAYER_BOOL_CHANGE,				// カエル
	PLAYER_BOOL_MAX
}PlayerBool;
typedef enum
{
	PLAYERWEAPON_HAND = 0,
	PLAYERWEAPON_SORD,
	PLAYERWEAPON_LANCE,
	PLAYERWEAPON_HAMMER,
	PLAYERWEAPON_MAX,
}PLAYERWEAPON;
typedef enum
{// プレイヤーの状態
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_DAMAGESTART,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DEATH,
}PLAYERSTATE;
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
}PartsKeyPlayer;
typedef struct
{
	int nFrame;									// キーフレーム
	PartsKeyPlayer aPartsKey[MAX_PLAYERMODEL];	// パーツごとのキー情報
}KeyPlayer;
typedef struct
{
	bool bLoop;									// ループするかどうか
	int nNumKey;								// キー数
	KeyPlayer aKey[MAX_PLAYER_KEY];				// キー情報
	__int8 nAttackPower;						// モーションの攻撃力
	float fDamageHorizontal;					// モーションの水平方向に吹き飛ばす力
	float fDamageVertical;						// モーションの垂直方向に吹き飛ばす力
}MotionPlayer;

typedef struct
{// 攻撃の当たり判定のオフセットの構造体
	D3DXVECTOR3			ofset;				// 位置
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			mtxWorldOld;			// ワールドマトリックス
	__int8				nIdxParent;			// 親モデルのインデックス
	float				fRadius;			// 半径
	short int			nCollisionStart;	// 当たり判定の始まりのフレーム数
	short int			nCollisionEnd;		// 当たり判定の終わりのフレーム数
	PlayerMotionType	nMotionType;		// 当たり判定が付くモーションのタイプ
	bool				bUse;				// 当たり判定が使われてるかどうか
}CollisionAttackPlayer;
typedef struct
{// 体の当たり判定のオフセットの構造体
	D3DXVECTOR3			ofset;				// 位置
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			mtxWorldOld;			// ワールドマトリックス
	__int8				nIdxParent;			// 親モデルのインデックス
	float				fRadius;			// 半径
	bool				bUse;				// 当たり判定が使われてるかどうか
}CollisionBodyPlayer;

typedef struct
{
	char *pFileName;					// ファイル名
	LPD3DXMESH pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;				// マテリアル情報へのポインタ
	DWORD nNumMat;						// マテリアル情報の数
	LPDIRECT3DTEXTURE9 *pTextuer;		// テクスチャ
	D3DXVECTOR3 rot;					// 角度
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 Initpos;				// 初期オフセット位置
	D3DXVECTOR3	posAddMotion;			// 加算モーション位置
	D3DXVECTOR3	rotAddMotion;			// 加算モーション向き
	D3DXVECTOR3 DiffrotMotion;			// 差分
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	int nIdxModel;						// 自分のモデルの番号
	int nIdxModelPareant;				// 親モデルのインデックス
}PlayerModel;

typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 posOld;					// 前回の位置
	D3DXVECTOR3 Diffpos;				// 目的の位置
	D3DXVECTOR3 Initpos;				// 初期位置
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 CtrlMove;				// 操作の移動量
	float JumpSpeed;					// ジャンプ量
	float MoveSpeed;					// 移動速度
	D3DXVECTOR3 Diffrot;				// 角度の差分
	D3DXVECTOR3 Destrot;				// 目的の角度
	D3DXVECTOR3 rot;					// 角度
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	PlayerMotionType nMotionType;			// 状態
	PlayerMotionType nMotionTypeOld;		// 前の状態
	PlayerModel aModel[MAX_PLAYERMODEL][PLAYERWEAPON_MAX];// パーツ数
	MotionPlayer aMotion[MAX_MOTION][PLAYERWEAPON_MAX];	// モーションの情報
	PLAYERSTATE state;					// 状態
	int nCntState;						// 状態
	float fRadiusWidth;					// 半径横
	float fRadiusHeight;				// 半径高さ
	int nIdyShadow;						// 影の番号
	int nIdyLine;						// 判定の番号
	bool bMotionState[PLAYER_BOOL_MAX];				// モーション使用判定
	bool bMotionStateOld[PLAYER_BOOL_MAX];				// 前回のモーション使用判定
	bool bBlend;						// ブレンド中かどうか
	bool bJump;							// ジャンプしているかどうか
	int nDamage;						// ダメージ
	int nNumParts;						// パーツ数
	int nNumMotion;						// モーション番号
	int nLife;							// ライフ
	int nInitLife;						// 初期ライフ
	int nSpirit;						// スピリット
	int nInitSpirit;					// 初期スピリット
	bool bAttention;					// 注目しているかどうか
	int nIdyLockEnemy;					// ロックしている敵の番号
	bool bSound[SOUND_LABEL_MAX];		// 音
	MeshField *pMeshFieldLand;			// どの床に乗っているか
	CollisionAttackPlayer CollisionAttack[MAX_COLLISION][PLAYERWEAPON_MAX];	// 攻撃判定
	CollisionBodyPlayer CollisionBody[MAX_COLLISION];		// ダメージ判定
	D3DXVECTOR3 fFlyPower;									// 吹っ飛び速度
	int nFlyCounter;										// 飛んでいる時間
	float fRotSpeed;										// 回転の減速スピード
	bool bAttackType[PLAYER_ATTACK_TYPE_MAX];				// アタックの種類
	Model *pModel;											// 当たったモデルの情報
	int nAirCounter;										// 飛んでる時のコンボ制限
	int nLoopCounter;										// モーションのループカウンタ
	PLAYERWEAPON nWeaponState;								// 武器の状態
	bool bWeaponNow[PLAYERWEAPON_MAX];						// どの武器中か
}Player;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(int nIdxPlayer);
void DrawPlayer(int nIdxPlayer);
Player *GetPlayer(void);
void MotionInfoPlayer(int nIdxPlayer);
void SetAttackCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset,float fRadius,  int nCollisionStart, int nCollisionEnd,PlayerMotionType nMotionType, PLAYERWEAPON nWeaponState);
void SetBodyCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset, float fRadius);
void CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, int nPower, float fDamageSide, float fDamageVertical);
void PlayerController(int nCntPlayer);					// プレイヤーの操作
void PlayerMotionEffect(int nCntPlayer,int nCollision, PLAYERWEAPON nWeaponState);
void PlayerSpirit(int nCntPlayer);
void PlayerSpiritAdd(int nSpirit);
#endif
