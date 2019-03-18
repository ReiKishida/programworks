//=============================================================================
//
// エネミー処理 [Boss.h]
// Author : KishidaRei 
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//#define MAX_PARTS (15)
#define MAX_BOSS_PARTS (16)
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
}PARTS_BOSS;


typedef enum
{
	BOSSSTATE_NORMAL = 0,
	BOSSSTATE_DAMAGE,
	BOSSSTATE_DEAD,
	BOSSSTATE_MAX
}BOSSSTATE;


typedef enum
{
	BOSSSTATUS_NEUTRAL = 0,
	BOSSSTATUS_MOVE,
	BOSSSTATUS_ATTACK,
	BOSSSTATUS_JUMP,
	BOSSSTATUS_LANDING,
	BOSSSTATUS_GUARD,
	BOSSSTATUS_DAMAGE1,
	BOSSSTATUS_MAX
}BOSSSTATUS;

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
}BOSSCOLLISION;

typedef struct
{
	DWORD nNumMatBoss = 0;	//マテリアルの情報の数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 move;	//向き
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 VtxMinBoss = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxBoss = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	BOSSSTATE state;
	BOSSSTATUS status;
	float fDestAngle;					//目的の角度
	float fAngle;						//差分
	float fLength;
	int nIndexShadow;					//影の番号
	int nIndexSencer;					//センサー(当たり判定用)の番号
	int nIndexSencerBoss;				//センサー(AI用)の番号
	int nIndexSencerEnableAttack;		//センサー(攻撃可能距離)の番号
	int nLife;
	bool bUse;
	bool bHit;
	int nCntCoolDown;
	int nCoolDown = 200;
	BOSSCOLLISION Collision;
	PARTS_BOSS aModel[MAX_BOSS_PARTS];
}BOSS;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

void HitBoss(int nDamage);

BOSS *GetBoss(void);
BOSSSTATUS *GetBossStatus(void);


bool CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetBossStatus(BOSSSTATUS status);
#endif
