//=============================================================================
//
// エネミー処理 [Enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYAXE_H_
#define _ENEMYAXE_H_

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
}PARTS_AXE;

typedef enum
{
	ENEMYAXESTATE_NONE = 0,
	ENEMYAXESTATE_BACKHOME,
	ENEMYAXESTATE_DISCOVERY,
	ENEMYAXESTATE_MAX
}ENEMYAXESTATE;

typedef enum
{
	ENEMYAXESTATE2_NORMAL = 0,
	ENEMYAXESTATE2_DAMAGE,
	ENEMYAXESTATE2_MAX
}ENEMYAXESTATE2;


typedef enum
{
	ENEMYAXESTATUS_NEUTRAL = 0,
	ENEMYAXESTATUS_MOVE,
	ENEMYAXESTATUS_ATTACK,
	ENEMYAXESTATUS_JUMP,
	ENEMYAXESTATUS_LANDING,
	ENEMYAXESTATUS_GUARD,
	ENEMYAXESTATUS_DAMAGE1,
	ENEMYAXESTATUS_MAX
}ENEMYAXESTATUS;

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
}ENEMYAXECOLLISION;

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
	ENEMYAXESTATE state;
	ENEMYAXESTATE2 state2;
	ENEMYAXESTATUS status;
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
	ENEMYAXECOLLISION Collision;
	PARTS_AXE aModel[MAX_AXE_PARTS];
}ENEMYAXE;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemyAxe(void);
void UninitEnemyAxe(void);
void UpdateEnemyAxe(void);
void DrawEnemyAxe(void);

void HitEnemyAxe(int nDamage, int nCntEnemy);

ENEMYAXE *GetEnemyAxe(void);
ENEMYAXESTATUS *GetEnemyAxeStatus(void);


bool CollisionEnemyAxe(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
int SetEnemyAxe(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyAxeStatus(ENEMYAXESTATUS status, int nCntEnemy);
void DeleteEnemyAxe(int nCntEnemy);
#endif
