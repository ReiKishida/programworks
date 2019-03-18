//=============================================================================
//
// エネミー処理 [Enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYAXE2_H_
#define _ENEMYAXE2_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYAXE (2)

//#define MAX_PARTS (15)
#define MAX_AXE_PARTS (16)
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
}PARTS_AXE2;

typedef enum
{
	ENEMYAXESTATE21_NONE = 0,
	ENEMYAXESTATE21_BACKHOME,
	ENEMYAXESTATE21_DISCOVERY,
	ENEMYAXESTATE21_MAX
}ENEMYAXESTATE21;

typedef enum
{
	ENEMYAXESTATE22_NORMAL = 0,
	ENEMYAXESTATE22_DAMAGE,
	ENEMYAXESTATE22_MAX
}ENEMYAXESTATE22;


typedef enum
{
	ENEMYAXESTATUS2_NEUTRAL = 0,
	ENEMYAXESTATUS2_MOVE,
	ENEMYAXESTATUS2_ATTACK,
	ENEMYAXESTATUS2_JUMP,
	ENEMYAXESTATUS2_LANDING,
	ENEMYAXESTATUS2_GUARD,
	ENEMYAXESTATUS2_DAMAGE1,
	ENEMYAXESTATUS2_MAX
}ENEMYAXESTATUS2;

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
}ENEMYAXECOLLISION2;

typedef struct
{
	DWORD nNumMatEnemy = 0;	//マテリアルの情報の数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 move;	//向き
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 VtxMinEnemyAxe = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxEnemyAxe = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	D3DXVECTOR3 HomePoint;
	float DestHome;
	ENEMYAXESTATE21 state;
	ENEMYAXESTATE22 state22;
	ENEMYAXESTATUS2 status;
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
	int nCntCoolDown;
	int nCoolDown = 200;
	ENEMYAXECOLLISION2 Collision;
	PARTS_AXE2 aModel[MAX_AXE_PARTS];
}ENEMYAXE2;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemyAxe2(void);
void UninitEnemyAxe2(void);
void UpdateEnemyAxe2(void);
void DrawEnemyAxe2(void);

void HitEnemyAxe2(int nDamage, int nCntEnemy);

ENEMYAXE2 *GetEnemyAxe2(void);
ENEMYAXESTATUS2 *GetEnemyAxeStatus2(void);


bool CollisionEnemyAxe2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
int SetEnemyAxe2(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyAxeStatus2(ENEMYAXESTATUS2 status, int nCntEnemy);
void DeleteEnemyAxe2(int nCntEnemy);
#endif
