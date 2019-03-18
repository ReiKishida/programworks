//=============================================================================
//
// エネミー処理 [Enemy1.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY0_H_
#define _ENEMY0_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY_0 (2)

//#define MAX_PARTS (15)
#define MAX_E_PARTS_0 (17)
//*****************************************************************************
// エネミーの構造体
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//テクスチャへのポインタ
	LPD3DXMESH pMesh = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;	//マテリアル情報へのポインタ
	DWORD nNumMat;			//マテリアル情報の数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 VtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	D3DXVECTOR3 VecShot;	//ショット用ベクトル
	float fDestAngle;		//目的の角度
	D3DXVECTOR3 fDestAngle_Axis;		//目的の角度
	D3DXVECTOR3 fAngle_Axis;		//目的の角度
	float fAngle;			//差分
	float nIdxModelParent = -1;	//親モデルのインデックス
	char FileName[256];
}PARTS_E_0;

typedef enum
{
	ENEMYSTATE_NONE_0 = 0,
	ENEMYSTATE_BACKHOME_0,
	ENEMYSTATE_DISCOVERY_0,
	ENEMYSTATE_MAX_0
}ENEMYSTATE_0;

typedef enum
{
	ENEMYSTATE2_NORMAL_0 = 0,
	ENEMYSTATE2_DAMAGE_0,
	ENEMYSTATE2_MAX_0
}ENEMYSTATE2_0;


typedef enum
{
	ENEMYSTATUS_NEUTRAL_0 = 0,
	ENEMYSTATUS_MOVE_0,
	ENEMYSTATUS_ATTACK_0,
	ENEMYSTATUS_JUMP_0,
	ENEMYSTATUS_LANDING_0,
	ENEMYSTATUS_GUARD_0,
	ENEMYSTATUS_DAMAGE1_0,
	ENEMYSTATUS_MAX_0
}ENEMYSTATUS_0;

//------------------------------------------------------------------------
// 当たり判定の構造体													 |
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
}ENEMYCOLLISION_0;

typedef struct
{
	DWORD nNumMatEnemy = 0;	//マテリアルの情報の数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 move;	//向き
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 VtxMinEnemy = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxEnemy = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	D3DXVECTOR3 HomePoint;
	float DestHome;
	ENEMYSTATE_0 state;
	ENEMYSTATE2_0 state2;
	ENEMYSTATUS_0 status;
	bool bUseJump;
	float fDestAngle;					//目的の角度
	float fAngle;						//差分
	float fLength;
	int nIndexShadow;					//影の番号
	int nIndexSencer;					//センサー(当たり判定用)の番号
	int nIndexSencerEnemy;				//センサー(AI用)の番号
	int nIndexSencerEnableAttack;		//センサー(攻撃可能距離)の番号
	int nLife;
	bool bUse;
	bool bHit;
	int nCntCoolDown ;
	int nCoolDown = 200;
	ENEMYCOLLISION_0 Collision;
	PARTS_E_0 aModel[MAX_E_PARTS_0];
}ENEMY_0;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy_0(void);
void UninitEnemy_0(void);
void UpdateEnemy_0(void);
void DrawEnemy_0(void);

void HitEnemy_0(int nDamage, int nCntEnemy);

ENEMY_0 *GetEnemy_0(void);
ENEMYSTATUS_0 *GetEnemyStatus_0(void);

void HitEnemy_0(int nDamage, int nCntEnemy);

bool CollisionEnemy_0(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
int SetEnemy_0(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyStatus_0(ENEMYSTATUS_0 status, int nCntEnemy);
void DeleteEnemy_0(int nCntEnemy);
#endif
