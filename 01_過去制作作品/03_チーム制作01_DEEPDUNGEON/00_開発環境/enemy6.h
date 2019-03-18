//=============================================================================
//
// エネミー処理 [Enemy1.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY6_H_
#define _ENEMY6_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY_6 (2)

//#define MAX_PARTS (15)
#define MAX_E_PARTS_6 (17)
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
}PARTS_E_6;

typedef enum
{
	ENEMYSTATE_NONE_6 = 0,
	ENEMYSTATE_BACKHOME_6,
	ENEMYSTATE_DISCOVERY_6,
	ENEMYSTATE_MAX_6
}ENEMYSTATE_6;

typedef enum
{
	ENEMYSTATE2_NORMAL_6 = 0,
	ENEMYSTATE2_DAMAGE_6,
	ENEMYSTATE2_MAX_6
}ENEMYSTATE2_6;


typedef enum
{
	ENEMYSTATUS_NEUTRAL_6 = 0,
	ENEMYSTATUS_MOVE_6,
	ENEMYSTATUS_ATTACK_6,
	ENEMYSTATUS_JUMP_6,
	ENEMYSTATUS_LANDING_6,
	ENEMYSTATUS_GUARD_6,
	ENEMYSTATUS_DAMAGE1_6,
	ENEMYSTATUS_MAX_6
}ENEMYSTATUS_6;

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
}ENEMYCOLLISION_6;

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
	ENEMYSTATE_6 state;
	ENEMYSTATE2_6 state2;
	ENEMYSTATUS_6 status;
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
	ENEMYCOLLISION_6 Collision;
	PARTS_E_6 aModel[MAX_E_PARTS_6];
}ENEMY_6;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy_6(void);
void UninitEnemy_6(void);
void UpdateEnemy_6(void);
void DrawEnemy_6(void);

void HitEnemy_6(int nDamage, int nCntEnemy);

ENEMY_6 *GetEnemy_6(void);
ENEMYSTATUS_6 *GetEnemyStatus_6(void);

void HitEnemy_6(int nDamage, int nCntEnemy);

bool CollisionEnemy_6(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
int SetEnemy_6(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyStatus_6(ENEMYSTATUS_6 status, int nCntEnemy);
void DeleteEnemy_6(int nCntEnemy);
#endif
