//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : TEAM_Fascinator
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//------------------------------------------------------------------------
// モデルの構造体														  |
//------------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshPlayerModel = NULL;
	LPD3DXBUFFER pBuffMatPlayerModel = NULL;
	DWORD nNumMatPlayerModel;
	D3DXMATRIX mtxWorldPlayerModel;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	float nIdxModelParent = -1;		// 親モデルのインデックス
	char FileName[256];
}PLAYERMODEL;

//------------------------------------------------------------------------
// 当たり判定の構造体														  |
//------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorldCollision;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	float fRadius;
	int StartFlame;
	int EndFlame;
	bool bUse;
	int nCntMotion;
	int nIdxMotion;
	int nIdxParent;
}PLAYERCOLLISION;

typedef enum
{
	PLAYERSTATUS_NORMAL = 0,			// ニュートラル
	PLAYERSTATUS_DAMAGE,					// 移動
	PLAYERSTATUS_DEAD,					// 移動
	PLAYERSTATUS_MAX
}PLAYERSTATUS;

//=============================================================================
// プレイヤーの構造体
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorldPlayer;
	D3DXVECTOR3 pos;					// 現在の位置
	D3DXVECTOR3 posOld;					// 位置
	D3DXVECTOR3 rot;					// 向き
	float rotDest;						// 目標の向き
	float fAngle;						// 差分
	float fMoveSpeed;					//スピード
	float fSaveSpeed;
	D3DXVECTOR3 move;					// 移動
	D3DXVECTOR3 vtxMinModel;			// モデルの最小値
	D3DXVECTOR3 vtxMaxModel;			// モデルの最大値
	int PlayerAnim;
	int	nNumParts = 0;
	int MAX_PARTS;						// パーツの最大数
	int nIdxShadow;						// 影の番号
	int nIdxSencer;						//センサーの番号(当たり判定用)
	int nIdxSencerLock;					//センサーの番号(ロック用)
	int nCntMotionFlame;				//モーションカウント
	int nCntCoolDown;
	int nCntCharge;
	PLAYERCOLLISION Collision;
	PLAYERMODEL aModel[256];			// パーツ
	PLAYERSTATUS status;
}PLAYER;

//------------------------------------------------------------------------
// プレイヤーの状態															  |
//------------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NEUTRAL = 0,		// ニュートラル
	PLAYERSTATE_MOVE,				// 移動
	PLAYERSTATE_ATTACK,				// 攻撃
	PLAYERSTATE_JAMP,				// ジャンプ
	PLAYERSTATE_LANDING,			// 着地
	PLAYERSTATE_DAMAGE,				// ダメージ
	PLAYERSTATE_DEATH,				// 死亡
	PLAYERSTATE_MIASMA,				// 瘴気
	PLAYERSTATE_ORIGINAL,			// オリジナル
	PLAYERSTATE_ATTACK2,			//瘴撃
	PLAYERSTATE_MAX
}PLAYERSTATE;




//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayerState(PLAYERSTATE state);
PLAYER *GetPlayer(void);
PLAYERSTATE GetPlayerStateNow(void);

#endif
