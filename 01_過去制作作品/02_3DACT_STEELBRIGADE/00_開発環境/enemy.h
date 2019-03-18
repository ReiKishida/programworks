//=============================================================================
//
// エネミー処理 [Enemy.h]
// Author :
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY (3)

//#define MAX_PARTS (15)
#define MAX_PARTS (11)
//*****************************************************************************
// プレイヤーの構造体
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
}PARTS_E;



typedef enum
{
	ENEMYSTATUS_NEUTRAL = 0,
	ENEMYSTATUS_MOVE,
	ENEMYSTATUS_MAX
}ENEMYSTATUS;


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
	D3DXVECTOR3 Rmove;	//逆移動
	ENEMYSTATUS status;	//敵の状態
	bool bUseJump;	//ジャンプできるか
	float fDestAngle;		//目的の角度
	float fAngle;			//差分
	float fLength;	//距離
	int nIndexShadow;		//影の番号
	int nLife;	//ライフ
	bool bUse;	//使用しているか
	bool bUseQick;	//クイックブーストを使用しているか
	int nCoolDown = 200;	//クールダウン
	PARTS_E aModel[MAX_PARTS];
}ENEMY;




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);	//敵の初期化処理
void UninitEnemy(void);	//敵の終了処理
void UpdateEnemy(void);	//敵の更新処理
void DrawEnemy(void);	//敵の描画処理

//void HitEnemy(int nDamage);

ENEMY *GetEnemy(void);	//敵の情報の取得
ENEMYSTATUS *GetEnemyStatus(void);//敵の状態の取得

void HitEnemy(int nDamage, int nCntEnemy);//敵の被弾

bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);//敵の当たり判定
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);//敵の設置
#endif
