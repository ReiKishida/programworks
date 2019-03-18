//=============================================================================
//
// エネミー処理 [Enemy1.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYARCHER1_H_
#define _ENEMYARCHER1_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYARCHER_1 (2)

//#define MAX_PARTS (15)
#define MAX_EARCHER_PARTS_1 (16)
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
}PARTS_EARCHER_1;

typedef enum
{
	ENEMYARCHERSTATE_NONE_1 = 0,
	ENEMYARCHERSTATE_BACKHOME_1,
	ENEMYARCHERSTATE_DISCOVERY_1,
	ENEMYARCHERSTATE_MAX_1
}ENEMYARCHERSTATE_1;

typedef enum
{
	ENEMYARCHERSTATE2_NORMAL_1 = 0,
	ENEMYARCHERSTATE2_DAMAGE_1,
	ENEMYARCHERSTATE2_MAX_1
}ENEMYARCHERSTATE2_1;


typedef enum
{
	ENEMYARCHERSTATUS_NEUTRAL_1 = 0,
	ENEMYARCHERSTATUS_MOVE_1,
	ENEMYARCHERSTATUS_ATTACK_1,
	ENEMYARCHERSTATUS_JUMP_1,
	ENEMYARCHERSTATUS_LANDING_1,
	ENEMYARCHERSTATUS_GUARD_1,
	ENEMYARCHERSTATUS_DAMAGE1_1,
	ENEMYARCHERSTATUS_MAX_1
}ENEMYARCHERSTATUS_1;

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
}ENEMYARCHERCOLLISION_1;

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
	ENEMYARCHERSTATE_1 state;
	ENEMYARCHERSTATE2_1 state2;
	ENEMYARCHERSTATUS_1 status;
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
	ENEMYARCHERCOLLISION_1 Collision;
	PARTS_EARCHER_1 aModel[MAX_EARCHER_PARTS_1];
}ENEMYARCHER_1;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemyARCHER_1(void);
void UninitEnemyARCHER_1(void);
void UpdateEnemyARCHER_1(void);
void DrawEnemyARCHER_1(void);

void HitEnemyARCHER_1(int nDamage, int nCntEnemy);

ENEMYARCHER_1 *GetEnemyARCHER_1(void);
ENEMYARCHERSTATUS_1 *GetEnemyARCHERStatus_1(void);

void HitEnemyARCHER_1(int nDamage, int nCntEnemy);

bool CollisionEnemyARCHER_1(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
int SetEnemyARCHER_1(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyARCHERStatus_1(ENEMYARCHERSTATUS_1 status, int nCntEnemy);
void DeleteEnemyARCHER_1(int nCntEnemy);
#endif
