//=============================================================================
//
// プレイヤー処理 [player.h]
// Author :
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
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
}PARTS;

typedef enum
{
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_OVERHEAT,
	PLAYERSTATE_MAX
}PLAYERSTATE;


typedef enum
{
	PLAYERSTATUS_NEUTRAL = 0,
	PLAYERSTATUS_MOVE,
	PLAYERSTATUS_MAX
}PLAYERSTATUS;


typedef struct
{
	DWORD nNumMatPlayer = 0;	//マテリアルの情報の数
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posold;	//位置
	D3DXVECTOR3 move;	//向き
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 VtxMinPlayer = D3DXVECTOR3(10000.0f,10000.0f,10000.0f);
	D3DXVECTOR3 VtxMaxPlayer = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	PLAYERSTATE state;
	PLAYERSTATUS status;
	bool bUseJump;
	float fDestAngle;		//目的の角度
	float fAngle;			//差分
	int nIndexShadow;		//影の番号
	int nLife;
	bool bUse;
	int nCoolDown = 200;
	PARTS aModel[MAX_PARTS];
}PLAYER;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//向き
	D3DXVECTOR3 rot;	//向き
}VECTOR;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	int nIdxModelParent = -1;	//親モデルのインデックス
}BULLET_POS;





//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);	//プレイヤーの初期化処理
void UninitPlayer(void);//プレイヤーの終了処理
void UpdatePlayer(void);//プレイヤーの更新処理
void DrawPlayer(void);//プレイヤーの描画処理

void HitPlayer(int nDamage);//プレイヤーの被弾

PLAYER *GetPlayer(void);
PLAYERSTATUS *GetPlayerStatus(void);
#endif
