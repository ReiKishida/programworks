//=============================================================================
//
// 各種センサー処理処理 [sencer.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _SENCERCOL_H_
#define _SENCERCOL_H_

#include "main.h"


//*****************************************************************************
// センサーの構造体
//*****************************************************************************
typedef enum
{
	SENCERCOLTYPE_PLAYER = 0,
	SENCERCOLTYPE_ENEMY,
	SENCERCOLTYPE_ENEMY0,
	SENCERCOLTYPE_ENEMY1,
	SENCERCOLTYPE_ENEMY2,
	SENCERCOLTYPE_ENEMY3,
	SENCERCOLTYPE_ENEMY4,
	SENCERCOLTYPE_ENEMY5,
	SENCERCOLTYPE_ENEMY6,
	SENCERCOLTYPE_ENEMY7,

	SENCERCOLTYPE_ENEMY_AXE,
	SENCERCOLTYPE_ENEMY_AXE2,
	SENCERCOLTYPE_ENEMY_ARROW,
	SENCERCOLTYPE_ENEMY_ARROW0,
	SENCERCOLTYPE_ENEMY_ARROW1,
	SENCERCOLTYPE_ENEMY_ARROW2,

	SENCERCOLTYPE_ENEMY_BOSS,
	SENCERCOLTYPE_MAX
}SENCERCOLTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置(中心座標)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	SENCERCOLTYPE type;
	bool bUse;
	float fRadius;
}SENCERCOLLISION;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSencerCollision(void);		//センサー初期化
void UninitSencerCollision(void);	//センサー終了
void UpdateSencerCollision(void);	//センサー更新
void DrawSencerCollision(void);		//センサー描画

void SetPositionSencerCollision(int nIdxSencer, D3DXVECTOR3 pos);
void DeleteSencerCol(int nIdxSencerCol);

int SetSencerCollision(D3DXVECTOR3 pos, D3DXVECTOR3 rot, SENCERCOLTYPE type);


//通常攻撃(player->enemy)
bool CollisionAttackPlayer(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemy);//player -> enemyへの当たり判定
bool CollisionAttackPvA(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyArrow);//player -> enemy(Arrow)への当たり判定
bool CollisionAttackAxe(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyAxe);//player -> enemy(axe)への当たり判定
bool CollisionAttackPvB(D3DXVECTOR3 pos, float fRadius, int nCntSencer);//player -> bossへの当たり判定

//攻撃(Enemy->player)
bool CollisionAttackEnemy(D3DXVECTOR3 pos, float fRadius, int nCntSencer, SENCERCOLTYPE Type);//enemy -> playerへの当たり判定
bool CollisionAttackBoss(D3DXVECTOR3 pos, float fRadius, int nCntSencer, SENCERCOLTYPE Type);//enemy -> playerへの当たり判定

//瘴撃
bool Collision_L_AttackPlayer(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemy);//player -> enemyへの当たり判定
bool Collision_L_AttackPvB(D3DXVECTOR3 pos, float fRadius, int nCntSencer);//player -> bossへの当たり判定
bool Collision_L_AttackPvA(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyArrow);//player -> enemy(Arrow)への当たり判定
bool Collision_L_AttackAxe(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyArrow);//player -> enemy(Arrow)への当たり判定


bool CollisionSencer_Col(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold);

//魔法攻撃
bool CollisionSencer_Magic(D3DXVECTOR3 *pos, int nCntEnemy);

bool CollisionSencer_Magic_Boss(D3DXVECTOR3 *pos);

//敵弓攻撃
bool CollisionAttackEnemyArrow(D3DXVECTOR3 *pos);//enemy -> playerへの当たり判定(矢)

																								   
//衝撃魔法攻撃
bool CollisionSencer_L_Magic(D3DXVECTOR3 *pos, int nCntEnemy);
bool CollisionSencer_L_Magic_Boss(D3DXVECTOR3 *pos);


#endif
