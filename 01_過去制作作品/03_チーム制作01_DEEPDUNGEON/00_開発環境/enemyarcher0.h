//=============================================================================
//
// エネミー処理 [Enemy1.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYARCHER0_H_
#define _ENEMYARCHER0_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYARCHER_0 (2)

//#define MAX_PARTS (15)
#define MAX_EARCHER_PARTS_0 (16)
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
}PARTS_EARCHER_0;

typedef enum
{
	ENEMYARCHERSTATE_NONE_0 = 0,
	ENEMYARCHERSTATE_BACKHOME_0,
	ENEMYARCHERSTATE_DISCOVERY_0,
	ENEMYARCHERSTATE_MAX_0
}ENEMYARCHERSTATE_0;

typedef enum
{
	ENEMYARCHERSTATE2_NORMAL_0 = 0,
	ENEMYARCHERSTATE2_DAMAGE_0,
	ENEMYARCHERSTATE2_MAX_0
}ENEMYARCHERSTATE2_0;


typedef enum
{
	ENEMYARCHERSTATUS_NEUTRAL_0 = 0,
	ENEMYARCHERSTATUS_MOVE_0,
	ENEMYARCHERSTATUS_ATTACK_0,
	ENEMYARCHERSTATUS_JUMP_0,
	ENEMYARCHERSTATUS_LANDING_0,
	ENEMYARCHERSTATUS_GUARD_0,
	ENEMYARCHERSTATUS_DAMAGE1_0,
	ENEMYARCHERSTATUS_MAX_0
}ENEMYARCHERSTATUS_0;

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
}ENEMYARCHERCOLLISION_0;

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
	ENEMYARCHERSTATE_0 state;
	ENEMYARCHERSTATE2_0 state2;
	ENEMYARCHERSTATUS_0 status;
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
	ENEMYARCHERCOLLISION_0 Collision;
	PARTS_EARCHER_0 aModel[MAX_EARCHER_PARTS_0];
}ENEMYARCHER_0;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemyARCHER_0(void);
void UninitEnemyARCHER_0(void);
void UpdateEnemyARCHER_0(void);
void DrawEnemyARCHER_0(void);

void HitEnemyARCHER_0(int nDamage, int nCntEnemy);

ENEMYARCHER_0 *GetEnemyARCHER_0(void);
ENEMYARCHERSTATUS_0 *GetEnemyARCHERStatus_0(void);

void HitEnemyARCHER_0(int nDamage, int nCntEnemy);

bool CollisionEnemyARCHER_0(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
int SetEnemyARCHER_0(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyARCHERStatus_0(ENEMYARCHERSTATUS_0 status, int nCntEnemy);
void DeleteEnemyARCHER_0(int nCntEnemy);
#endif
