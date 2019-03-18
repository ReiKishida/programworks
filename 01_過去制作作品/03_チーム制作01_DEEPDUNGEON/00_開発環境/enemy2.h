//=============================================================================
//
// エネミー処理 [Enemy1.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY2_H_
#define _ENEMY2_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY_2 (2)

//#define MAX_PARTS (15)
#define MAX_E_PARTS_2 (17)
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
}PARTS_E_2;

typedef enum
{
	ENEMYSTATE_NONE_2 = 0,
	ENEMYSTATE_BACKHOME_2,
	ENEMYSTATE_DISCOVERY_2,
	ENEMYSTATE_MAX_2
}ENEMYSTATE_2;

typedef enum
{
	ENEMYSTATE2_NORMAL_2 = 0,
	ENEMYSTATE2_DAMAGE_2,
	ENEMYSTATE2_MAX_2
}ENEMYSTATE2_2;


typedef enum
{
	ENEMYSTATUS_NEUTRAL_2 = 0,
	ENEMYSTATUS_MOVE_2,
	ENEMYSTATUS_ATTACK_2,
	ENEMYSTATUS_JUMP_2,
	ENEMYSTATUS_LANDING_2,
	ENEMYSTATUS_GUARD_2,
	ENEMYSTATUS_DAMAGE1_2,
	ENEMYSTATUS_MAX_2
}ENEMYSTATUS_2;

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
}ENEMYCOLLISION_2;

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
	ENEMYSTATE_2 state;
	ENEMYSTATE2_2 state2;
	ENEMYSTATUS_2 status;
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
	ENEMYCOLLISION_2 Collision;
	PARTS_E_2 aModel[MAX_E_PARTS_2];
}ENEMY_2;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy_2(void);
void UninitEnemy_2(void);
void UpdateEnemy_2(void);
void DrawEnemy_2(void);

void HitEnemy_2(int nDamage, int nCntEnemy);

ENEMY_2 *GetEnemy_2(void);
ENEMYSTATUS_2 *GetEnemyStatus_2(void);

void HitEnemy_2(int nDamage, int nCntEnemy);

bool CollisionEnemy_2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
int SetEnemy_2(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyStatus_2(ENEMYSTATUS_2 status, int nCntEnemy);
void DeleteEnemy_2(int nCntEnemy);
#endif
