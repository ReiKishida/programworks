////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プレイヤー処理 [player.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sound.h"
#include "meshField.h"
#include "model.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_PLAYER		(1)			// 最大数
#define MAX_PLAYERMODEL	(18)
#define MAX_PLAYER_KEY	(20)	// モーション1つに対するキーフレームの最大数
#define MAX_MOTION		(30)	// モーション数
#define MAX_COLLISION	(50)	// 判定最大数
#define MAX_PLAYER_ORBIT (10)	// 軌跡の最大数
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef enum
{// モーションの種類
	PLAYER_MOTION_TYPE_NEUTRAL = 0,					// ニュートラル時
	PLAYER_MOTION_TYPE_KICK_R,						// R
	PLAYER_MOTION_TYPE_KICK_L ,						// L
	PLAYER_MOTION_TYPE_JUMP,						// ジャンプ
	PLAYER_MOTION_TYPE_LANDING,						// 着地
	PLAYER_MOTION_TYPE_FLY,						// 吹っ飛び
	PLAYER_MOTION_TYPE_HAPPY,						// 喜び
	PLAYER_MOTION_TYPE_MAX							// モーションの総数
}PlayerMotionType;

typedef enum
{// boolの種類
	PLAYER_BOOL_JUMP=0,				// ジャンプ
	PLAYER_BOOL_KICK_L,
	PLAYER_BOOL_KICK_R,
	PLAYER_BOOL_LANDING,			// 着地
	PLAYER_BOOL_FLY,				// 吹っ飛び
	PLAYER_BOOL_HAPPY,				// 勝利
	PLAYER_BOOL_MAX
}PlayerBool;

typedef enum
{// プレイヤーの状態
	PLAYERSTATE_NONE = 0,		// 何もなし
	PLAYERSTATE_DAMAGESTART,	// 無敵
	PLAYERSTATE_DAMAGE,			// ダメージ
	PLAYERSTATE_FALL,			// 落ちる
	PLAYERSTATE_INVINCIBLE,		// 無敵
	PLAYERSTATE_DEATH,			// 死ぬ
}PLAYERSTATE;

typedef struct
{
	int nIdyOrbit;					// 番号
	int nOrbitLength;				// 軌跡の長さ
	D3DXVECTOR3 OrbitStart;			// 軌跡の始点
	D3DXVECTOR3 OrbitEnd;			// 軌跡の終点
	int nType;						// タイプ
	PlayerMotionType motion;		// モーションタイプ
}PlayerOrbit;

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
	float fAttackPower;						// モーションの攻撃力
	float fDamageH;					// モーションの水平方向に吹き飛ばす力
	float fDamageV;						// モーションの垂直方向に吹き飛ばす力
}MotionPlayer;

typedef struct
{// 攻撃の当たり判定のオフセットの構造体
	D3DXVECTOR3			ofset;				// 位置
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			mtxWorldOld;		// ワールドマトリックス
	__int8				nIdxParent;			// 親モデルのインデックス
	float				fRadius;			// 半径
	short int			nCollisionStart;	// 当たり判定の始まりのフレーム数
	short int			nCollisionEnd;		// 当たり判定の終わりのフレーム数
	PlayerMotionType	nMotionType;		// 当たり判定が付くモーションのタイプ
	int					nStopTime;			// 停止時間
	bool				bUse;				// 当たり判定が使われてるかどうか
}CollisionAttackPlayer;
typedef struct
{// 体の当たり判定のオフセットの構造体
	D3DXVECTOR3			ofset;				// 位置
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			mtxWorldOld;		// ワールドマトリックス
	__int8				nIdxParent;			// 親モデルのインデックス
	float				fRadius;			// 半径
	bool				bUse;				// 当たり判定が使われてるかどうか
}CollisionBodyPlayer;

typedef struct
{
	char *pFileName;								// ファイル名
	LPD3DXMESH pMesh;								// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;							// マテリアル情報へのポインタ
	DWORD nNumMat;									// マテリアル情報の数
	LPDIRECT3DTEXTURE9 *pTextuer;	// テクスチャ
	D3DXVECTOR3 rot;								// 角度
	D3DXVECTOR3 pos;								// 位置
	D3DXVECTOR3 Initpos;							// 初期オフセット位置
	D3DXVECTOR3	posAddMotion;						// 加算モーション位置
	D3DXVECTOR3	rotAddMotion;						// 加算モーション向き
	D3DXVECTOR3 DiffrotMotion;						// 差分
	D3DXMATRIX mtxWorld;							// ワールドマトリックス
	D3DXMATRIX mtxWorldOld;							// ワールドマトリックス
	int nIdxModel;									// 自分のモデルの番号
	int nIdxModelPareant;							// 親モデルのインデックス
}PlayerModel;
typedef struct
{
	CollisionAttackPlayer ColAttack[MAX_COLLISION];	// 攻撃判定
	CollisionBodyPlayer ColBody[MAX_COLLISION];		// ダメージ判定
	PlayerModel aModel[MAX_PLAYERMODEL];// パーツ数
	MotionPlayer aMotion[MAX_MOTION];	// モーションの情報
	PlayerOrbit orbit[PLAYER_MOTION_TYPE_MAX][MAX_PLAYER_ORBIT];			// 軌跡の最大
	float fRadiusWidth;					// 半径横
	float fRadiusHeight;				// 半径高さ
	int nNumParts;						// パーツ数
	int nNumMotion;						// モーション番号
	float JumpSpeed;					// ジャンプ量
	float MoveSpeed;					// 移動速度
	int nInitLife;						// 初期ライフ
}PlayerData;
typedef struct
{
	CollisionAttackPlayer ColAttack[MAX_COLLISION];	// 攻撃判定
	CollisionBodyPlayer ColBody[MAX_COLLISION];		// ダメージ判定
	PlayerModel aModel[MAX_PLAYERMODEL];// パーツ数
	PlayerOrbit orbit[PLAYER_MOTION_TYPE_MAX][MAX_PLAYER_ORBIT];			// 軌跡の最大

	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 posOld;					// 前回の位置
	D3DXVECTOR3 Diffpos;				// 目的の位置
	D3DXVECTOR3 Initpos;				// 初期位置
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 moveR;					// 移動量
	D3DXVECTOR3 moveL;					// 移動量
	D3DXVECTOR3 CtrlMove;				// 操作の移動量
	D3DXCOLOR col;						// 色
	float fRadiusWidth;					// 半径横
	float fRadiusHeight;				// 半径高さ
	float JumpSpeed;					// ジャンプ量
	float MoveSpeed;					// 移動速度
	D3DXVECTOR3 Diffrot;				// 角度の差分
	D3DXVECTOR3 Destrot;				// 目的の角度
	D3DXVECTOR3 rot;					// 角度
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	PlayerMotionType nMotionType;		// 状態
	PlayerMotionType nMotionTypeOld;	// 前の状態
	PlayerMotionType nMotionGetCol;		// ダメージ与えたときの状態
	PlayerMotionType nMotionGetColOld;	// ダメージ与えたときの前の状態
	PLAYERSTATE state;					// 状態
	int nCntState;						// 状態
	int nIdyShadow;						// 影の番号
	int nIdyLine;						// 判定の番号
	bool bMotionState[PLAYER_BOOL_MAX];			// モーション使用判定
	bool bMotionStateOld[PLAYER_BOOL_MAX];		// 前回のモーション使用判定
	bool bBlend;						// ブレンド中かどうか
	bool bJump;							// ジャンプしているかどうか
	int nDamage;						// ダメージ
	float fLife;							// ライフ
	bool bSound[SOUND_LABEL_MAX];		// 音
	int nIdy;							// 番号
	MeshField *pMeshFieldLand;			// どの床に乗っているか
	D3DXVECTOR3 fFlyPower;									// 吹っ飛び速度
	int nFlyCounter;										// 飛んでいる時間
	float fRotSpeed;										// 回転の減速スピード
	Model *pModel;											// 当たったモデルの情報
	Model *pModelAtkData;									// 当たったモデルの情報
	Model *pModelAtk;										// 当たったモデルの情報
	int nAirCounter;										// 飛んでる時のコンボ制限
	int nLoopCounter;										// モーションのループカウンタ
	int nKeyCounter;										// キー数
	int nFrameCounter;										// フレーム数
	int nFrameAllCounter;									// 最大フレーム数
	int nFrame;												// フレー
	bool bUse;
	bool bDraw;
}Player;

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(int nIdy);
void DrawPlayer(int nIdy,int nCamera);
Player *GetPlayer(void);
PlayerData *GetPlayerData(void);
void MotionInfoPlayer(int nIdy);
void SetAttackCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset,float fRadius,  int nCollisionStart, int nCollisionEnd,int nStopTime,PlayerMotionType nMotionType);
void SetBodyCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset, float fRadius);
bool CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, float nPower, float fDamageSide, float fDamageV, int nIdy);
void PlayerController(int nIdy);					// プレイヤーの操作
void PlayerOverLap(int nIdy);
void PlayerColInfo(int nIdy);
void PlayerMotionEffect(int nIdy,int nCollision);
void SetPlayer(int nIdy, D3DXVECTOR3 pos);
void ReleasePlayer(int nIdy);
int CheckLife(void);
#endif
