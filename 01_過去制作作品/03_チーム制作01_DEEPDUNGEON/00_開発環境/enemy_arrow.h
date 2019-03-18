//=============================================================================
//
// エネミー処理 [Enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYARROW_H_
#define _ENEMYARROW_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY_ARROW (2)

//#define MAX_PARTS (15)
#define MAX_ARROW_PARTS (16)
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
}PARTS_ARROW;

typedef enum
{
	ENEMY_ARROW_STATE_NONE = 0,
	ENEMY_ARROW_STATE_BACKHOME,
	ENEMY_ARROW_STATE_DISCOVERY,
	ENEMY_ARROW_STATE_MAX
}ENEMY_ARROW_STATE;

typedef enum
{
	ENEMY_ARROW_STATE2_NORMAL = 0,
	ENEMY_ARROW_STATE2_DAMAGE,
	ENEMY_ARROW_STATE2_MAX
}ENEMY_ARROW_STATE2;


typedef enum
{
	ENEMY_ARROW_STATUS_NEUTRAL = 0,
	ENEMY_ARROW_STATUS_MOVE,
	ENEMY_ARROW_STATUS_ATTACK,
	ENEMY_ARROW_STATUS_JUMP,
	ENEMY_ARROW_STATUS_LANDING,
	ENEMY_ARROW_STATUS_GUARD,
	ENEMY_ARROW_STATUS_DAMAGE1,
	ENEMY_ARROW_STATUS_MAX
}ENEMY_ARROW_STATUS;

//------------------------------------------------------------------------
// 当たり判定の構造体													 |
//------------------------------------------------------------------------
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
	ENEMY_ARROW_STATE state;
	ENEMY_ARROW_STATE2 state2;
	ENEMY_ARROW_STATUS status;
	bool bUseJump;
	float fDestAngle;					//目的の角度
	float fAngle;						//差分
	float fLength;
	int nIndexShadow;					//影の番号
	int nIndexSencer;					//センサー(当たり判定用)の番号
	int nIndexSencerEnemyaArrow;				//センサー(AI用)の番号
	int nIndexSencerEnableAttack;		//センサー(攻撃可能距離)の番号
	int nLife;
	bool bUse;
	bool bHit;
	int nCntCoolDown;
	int nCoolDown = 200;
	int StartFlame;
	PARTS_ARROW aModel[MAX_ARROW_PARTS];
}ENEMYARROW;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemyArrow(void);
void UninitEnemyArrow(void);
void UpdateEnemyArrow(void);
void DrawEnemyArrow(void);

void HitEnemyArrow(int nDamage, int nCntEnemy);

ENEMYARROW *GetEnemyArrow(void);
ENEMY_ARROW_STATUS *GetEnemyArrowStatus(void);


int SetEnemyArrow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);
void SetEnemyStatusArrow(ENEMY_ARROW_STATUS status, int nCntEnemy);
void DeleteEnemyArrow(int nCntEnemy);
#endif
