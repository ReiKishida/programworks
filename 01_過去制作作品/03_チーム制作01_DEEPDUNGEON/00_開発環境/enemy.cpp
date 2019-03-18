//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "particle.h"
#include "model.h"
#include "game.h"
#include "fade.h"
#include "enemy.h"
#include "sound.h"
#include "enemytext.h"
#include "enemymotion.h"
#include "SencerCol.h"
#include "SencerLock.h"
#include "SencerEnemy.h"
#include "SencerAtk.h"
#include "SencerEnableAttack.h"
#include "Wall1.h"
#include "Wall2.h"
#include "WallBoss.h"
#include "brazen.h"

#include <math.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define JUMP_POWER (12.00f)	//ジャンプ力
#define GRAVITY_E (0.8f)		//重力
#define GRAVITY_E_MAX (150.0f)	//重力の最大量

#define DASHSPEED  (4.0f)
#define ROT_SPEED (0.05f)
#define ROT_SPEED2 (0.2f)

#define STAGE_LIMIT_MAX_X (1450.0f)
#define STAGE_LIMIT_MIN_X (-40.0f)
#define STAGE_LIMIT_MAX_Z (30.0f)
#define STAGE_LIMIT_MIN_Z (-3800.0f)

#define EFFECT_MAX (50)

#define MOVE_BULLET_Y (25.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

ENEMY g_Enemy[MAX_ENEMY];
ENEMYSTATUS g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
ENEMYSTATUS g_EnemyStatusNow = ENEMYSTATUS_NEUTRAL;

//STAGE g_aLimit[4];
//テキスト情報取得

float fJump_E;	//ジャンプ力
float fGRAVITY_E_E;	//重力

int g_nCntSound_E = 0;
int g_nCntEnemy;

bool bLand_E; //着地判定
 //=============================================================================
 // 初期化処理
 //=============================================================================
void InitEnemy(void)
{
	

	EnemyTex *pEnemyText;
	pEnemyText = GetEnemyTex();
	g_nCntEnemy = 3;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCntIdx = 0; nCntIdx < pEnemyText->MAX_PARTS; nCntIdx++)
		{//各種値の代入
		 //読み込むモデル名
			g_Enemy[nCntEnemy].aModel[nCntIdx].FileName[0] = pEnemyText->aModelTex[nCntIdx].FileName[0];

			//親子関係
			g_Enemy[nCntEnemy].aModel[nCntIdx].nIdxModelParent = pEnemyText->aModelTex[nCntIdx].Index;

			//位置
			g_Enemy[nCntEnemy].aModel[nCntIdx].pos = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].PosX, pEnemyText->aModelTex[nCntIdx].PosY, pEnemyText->aModelTex[nCntIdx].PosZ);

			//回転
			g_Enemy[nCntEnemy].aModel[nCntIdx].rot = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].RotX,
				pEnemyText->aModelTex[nCntIdx].RotY,
				pEnemyText->aModelTex[nCntIdx].RotZ);

			//モデルの設定
			D3DXLoadMeshFromX(&pEnemyText->aModelTex[nCntIdx].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Enemy[nCntEnemy].aModel[nCntIdx].pBuffMat,
				NULL,
				&g_Enemy[nCntEnemy].aModel[nCntIdx].nNumMat,
				&g_Enemy[nCntEnemy].aModel[nCntIdx].pMesh);
		}
		g_Enemy[nCntEnemy].Collision.bUse = false;
		g_Enemy[nCntEnemy].Collision.nCntMotion = 0;
		g_Enemy[nCntEnemy].Collision.StartFlame = 5;
		g_Enemy[nCntEnemy].Collision.fRadius = 40.0f;
		g_Enemy[nCntEnemy].Collision.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		g_Enemy[nCntEnemy].Collision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[nCntEnemy].Collision.nIdxParent = 0;
		g_Enemy[nCntEnemy].nCntCoolDown = 120;

		SetSencerAtk(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43 - 30.0f));
		SetSencerAtk(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43 - 20.0f));
		SetSencerAtk(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43 - 10.0f));
		SetSencerAtk(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43));


		//ジャンプの状態
		g_Enemy[nCntEnemy].bUseJump = false;
		//プレイヤーの状態
		g_Enemy[nCntEnemy].nCoolDown = 0;

		//着地しているか
		bLand_E = false;

		//プレイヤーが使用されているか
		g_Enemy[nCntEnemy].bUse = false;

	}









	fJump_E = JUMP_POWER;
	fGRAVITY_E_E = GRAVITY_E;


	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点数を取得
		nNumVtx = g_Enemy[nCntEnemy].aModel[0].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Enemy[nCntEnemy].aModel[0].pMesh->GetFVF());

		//頂点バッファをロック
		g_Enemy[nCntEnemy].aModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.x)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.x = Vtx.x;
			}

			if (Vtx.y < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y = Vtx.y;
			}

			if (Vtx.z < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z = Vtx.x;
			}


			if (Vtx.x > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x = Vtx.x + 20.0f;
			}

			if (Vtx.y > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y = Vtx.y + 30.0f;
			}

			if (Vtx.z > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.z)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.z = Vtx.z + 20.0f;
			}
			pVtxBuff += sizeFVF;
		}

		g_Enemy[nCntEnemy].VtxMinEnemy.x = g_Enemy[nCntEnemy].aModel[0].VtxMinModel.x;
		g_Enemy[nCntEnemy].VtxMaxEnemy.x = g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x;

		g_Enemy[nCntEnemy].VtxMinEnemy.y = g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y;
		g_Enemy[nCntEnemy].VtxMaxEnemy.y = g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y;

		g_Enemy[nCntEnemy].VtxMinEnemy.z = g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z;
		g_Enemy[nCntEnemy].VtxMaxEnemy.z = g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.z;

		g_Enemy[nCntEnemy].fAngle = 0.0f;			//差分

	}

	CAMERA *pCamera;
	pCamera = GetCamera();


}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// メッシュの開放
		if (g_Enemy[nCntEnemy].aModel[0].pMesh != NULL)
		{
			g_Enemy[nCntEnemy].aModel[0].pMesh->Release();
			g_Enemy[nCntEnemy].aModel[0].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_Enemy[nCntEnemy].aModel[0].pBuffMat != NULL)
		{
			g_Enemy[nCntEnemy].aModel[0].pBuffMat->Release();
			g_Enemy[nCntEnemy].aModel[0].pBuffMat = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{

	EnemyMotion *pE_Motion = GetEnemyMotion();					//モーションデータの所得
	EnemyTex *pE_Text = GetEnemyTex();							//ファイルデータの取得

	EnemyTex *pEnemyText;
	pEnemyText = GetEnemyTex();
	STAGEMODE g_Stage = GetStageMode();				//現在のステージ番号を取得


#if 1




	//nCntUpdateRogic++;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{




		g_Enemy[nCntEnemy].nCntCoolDown++;
		g_Enemy[nCntEnemy].posold = g_Enemy[nCntEnemy].pos;

		CAMERA pCamera;
		pCamera = *GetCamera();
		PLAYER *pPlayer;
		//プレイヤーの取得
		pPlayer = GetPlayer();
		D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
		BulletMove.x = -30.0f;
		BulletMove.z = -30.0f;

		D3DXVECTOR3 BulletMove2(0.0f, 0.0f, 0.0f);
		BulletMove2.x = -7.0f;
		BulletMove2.z = -7.0f;

		D3DXVECTOR3 VecP;
		D3DXVECTOR3 VecLength;


		VecP.x = (pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x);
		VecP.y = pPlayer->pos.y - g_Enemy[nCntEnemy].pos.y;
		VecP.z = (pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z) + 45.0f;
		D3DXVec3Normalize(&VecP, &VecP);

		VecLength = pPlayer->pos - g_Enemy[nCntEnemy].pos;









		if (g_Enemy[nCntEnemy].bUse == true)
		{
			switch (g_Enemy[nCntEnemy].state2)
			{
			case ENEMYSTATE2_NORMAL:
				break;

			case ENEMYSTATE2_DAMAGE:
				g_Enemy[nCntEnemy].nCntCoolDown--;
				if (g_Enemy[nCntEnemy].nCntCoolDown <= 0)
				{
					g_Enemy[nCntEnemy].state2 = ENEMYSTATE2_NORMAL;
				}
			}



			static float fAngle;
			//fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z);
			fAngle = atan2f(g_Enemy[nCntEnemy].pos.x - pPlayer->pos.x, g_Enemy[nCntEnemy].pos.z - pPlayer->pos.z);

			g_Enemy[nCntEnemy].fLength = sqrtf((pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x) * (pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x) + (pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z) *(pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z));//距離


			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}

			if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}




			switch (g_Enemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NONE:

				if (CollisionSencer_Enemy(&pPlayer->pos, g_Enemy[nCntEnemy].nIndexSencerEnemy) == true)
				{
					g_Enemy[nCntEnemy].state = ENEMYSTATE_DISCOVERY;
				}
				else
				{
					g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}

				break;

			case ENEMYSTATE_BACKHOME:

				g_Enemy[nCntEnemy].DestHome = atan2f(g_Enemy[nCntEnemy].HomePoint.x - g_Enemy[nCntEnemy].pos.x,
					g_Enemy[nCntEnemy].HomePoint.z - g_Enemy[nCntEnemy].pos.z);

				g_Enemy[nCntEnemy].move.x = g_Enemy[nCntEnemy].DestHome * 1.2f;
				g_Enemy[nCntEnemy].move.z = g_Enemy[nCntEnemy].DestHome * 1.2f;

				if (g_Enemy[nCntEnemy].pos.x <= g_Enemy[nCntEnemy].HomePoint.x + 50.0f || g_Enemy[nCntEnemy].pos.x >= g_Enemy[nCntEnemy].HomePoint.x - 50.0f
					|| g_Enemy[nCntEnemy].pos.z <= g_Enemy[nCntEnemy].HomePoint.z + 50.0f || g_Enemy[nCntEnemy].pos.z >= g_Enemy[nCntEnemy].HomePoint.z - 50.0f)
				{
					g_Enemy[nCntEnemy].state = ENEMYSTATE_NONE;
				}

				break;
			case ENEMYSTATE_DISCOVERY:
				g_Enemy[nCntEnemy].move.x = VecP.x * 1.8f;
				g_Enemy[nCntEnemy].move.z = VecP.z * 1.8f;
				fAngle = atan2f(g_Enemy[nCntEnemy].pos.x - pPlayer->pos.x, g_Enemy[nCntEnemy].pos.z - pPlayer->pos.z);
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}

				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				if (CollisionSencer_Enemy(&pPlayer->pos, g_Enemy[nCntEnemy].nIndexSencerEnemy) == false)
				{
					g_Enemy[nCntEnemy].state = ENEMYSTATE_BACKHOME;
				}
				if (g_EnemyStatusNow == ENEMYSTATUS_NEUTRAL)
				{//ムーヴへの移行
					SetEnemyStatus(ENEMYSTATUS_MOVE, nCntEnemy);
				}
				if (CollisionSencer_Enable(&pPlayer->pos) == true)
				{
					if (g_EnemyStatusNow == ENEMYSTATUS_NEUTRAL || g_EnemyStatusNow == ENEMYSTATUS_MOVE)
					{//ムーヴへの移行
						pE_Motion->nCntFlame = 0;
						if (g_Enemy[nCntEnemy].nCntCoolDown % 40 == 0)
						{
							SetEnemyStatus(ENEMYSTATUS_ATTACK, nCntEnemy);
						}
					}

					else if (g_EnemyStatusNow == ENEMYSTATUS_ATTACK)
					{
						if (pE_Motion->nCntFlame % g_Enemy[nCntEnemy].Collision.StartFlame == 0)
						{
							PlaySound(SOUND_LABEL_SE_SLASH);
						}
						if (pE_Motion->nCntFlame > g_Enemy[nCntEnemy].Collision.StartFlame)
						{
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43 - 30.0f)
								, g_Enemy[nCntEnemy].Collision.fRadius
								, g_Enemy[nCntEnemy].nIndexSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43 - 20.0f)
								, g_Enemy[nCntEnemy].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43 - 10.0f)
								, g_Enemy[nCntEnemy].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43)
								, g_Enemy[nCntEnemy].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
						}
					}
					g_Enemy[nCntEnemy].rot.y = fAngle;
				}
				break;
			}//switch文

			g_Enemy[nCntEnemy].rot.y = fAngle;

#if 1

#endif			

			g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

			g_Enemy[nCntEnemy].move.y -= GRAVITY_E;
			if (g_Enemy[nCntEnemy].move.y > GRAVITY_E_MAX)
			{
				g_Enemy[nCntEnemy].move.y = GRAVITY_E_MAX;
			}
			if (g_Enemy[nCntEnemy].pos.y <= 0.0)
			{

				g_Enemy[nCntEnemy].pos.y = 0.0f;
				g_Enemy[nCntEnemy].move.y = 0.0f;

			}



#endif
			SetPositionSencerCollision(g_Enemy[nCntEnemy].nIndexSencer, g_Enemy[nCntEnemy].pos);//センサーの位置
			SetPositionShadow(g_Enemy[nCntEnemy].nIndexShadow, g_Enemy[nCntEnemy].pos);
			SetPositionSencerEnemy(g_Enemy[nCntEnemy].nIndexSencerEnemy,
				D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z));
			SetPositionSencerEnableAttack(g_Enemy[nCntEnemy].nIndexSencerEnableAttack, g_Enemy[nCntEnemy].pos);
			CollisionSencer_Lock(&g_Enemy[nCntEnemy].pos);
			//g_Enemy[nCntEnemy].rot.y += 0.01f;




			switch (g_EnemyStatus)
			{
			case ENEMYSTATUS_NEUTRAL:
				if (pE_Motion->KeyFlame == pE_Text->KEYFLAME[pE_Motion->nNumEnemymotion] - 1)
				{
					if (g_EnemyStatusNow == ENEMYSTATUS_NEUTRAL)
					{

						SetEnemyMotion(0);
					}
					else
					{
						pE_Motion->KeyFlame = 0;
						SetEnemyMotion(0);
						g_EnemyStatusNow = ENEMYSTATUS_NEUTRAL;
						g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
					}
				}
				break;
			case ENEMYSTATUS_MOVE:
				g_EnemyStatusNow = ENEMYSTATUS_MOVE;
				g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
				SetEnemyMotion(1);
				break;
			case ENEMYSTATUS_ATTACK:
				g_EnemyStatusNow = ENEMYSTATUS_ATTACK;
				g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
				SetEnemyMotion(2);
				break;
			case ENEMYSTATUS_JUMP:
				g_EnemyStatusNow = ENEMYSTATUS_JUMP;
				g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
				SetEnemyMotion(3);
				break;
			case ENEMYSTATUS_LANDING:
				g_EnemyStatusNow = ENEMYSTATUS_LANDING;
				g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
				SetEnemyMotion(4);
				break;
			case ENEMYSTATUS_GUARD:
				g_EnemyStatusNow = ENEMYSTATUS_GUARD;
				g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
				SetEnemyMotion(5);
				break;
			case ENEMYSTATUS_DAMAGE1:
				g_EnemyStatusNow = ENEMYSTATUS_DAMAGE1;
				g_EnemyStatus = ENEMYSTATUS_NEUTRAL;
				SetEnemyMotion(6);
				break;

			}//switch
		//if

			if (g_Stage == STAGEMODE_STAGE1)
			{//現在のステージがステージ1
			 //奥壁のあたり判定
				CollisionWall1_WIDTHBack_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//手前壁のあたり判定
				CollisionWall1_WIDTHThisSide_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall1_RIGHT_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall1_LEFT_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);
			}
			else if (g_Stage == STAGEMODE_STAGE2)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWall2_WIDTHBack_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//手前壁のあたり判定
				CollisionWall2_WIDTHThisSide_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall2_RIGHT_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall2_LEFT_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);
			}
			else if (g_Stage == STAGEMODE_STAGE3)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWallBoss_WIDTHBack_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//手前壁のあたり判定
				CollisionWallBoss_WIDTHThisSide_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWallBoss_RIGHT_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWallBoss_LEFT_Ene(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold);
			}
		}//buse

	}//for
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxParent2;

	//========================//
	//プレイヤーのインデックス//
	//========================//
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);

		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxrot);


		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans,
			g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxtrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorld);


		for (int nCntModelParts = 0; nCntModelParts < MAX_E_PARTS; nCntModelParts++)
		{
			if (g_Enemy[nCntEnemy].bUse == true)
			{

				if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == -1)
				{//プレイヤーのマトリックスに
					mtxParent = g_Enemy[nCntEnemy].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 0)
				{//体のマトリックスに
					mtxParent = g_Enemy[nCntEnemy].aModel[0].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 1)
				{//軸１に
					mtxParent = g_Enemy[nCntEnemy].aModel[1].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 2)
				{//軸２に
					mtxParent = g_Enemy[nCntEnemy].aModel[2].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 5)
				{//軸２に
					mtxParent = g_Enemy[nCntEnemy].aModel[5].mtxWorld;
				}
				else if (g_Enemy[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 8)
				{//軸２に
					mtxParent = g_Enemy[nCntEnemy].aModel[8].mtxWorld;
				}
				else
				{//その他
					mtxParent = g_Enemy[nCntEnemy].aModel[nCntModelParts - 1].mtxWorld;
				}
				//=======================================================================//
				//						    親モデルのインデックス						//
				//=====================================================================//
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_Enemy[nCntEnemy].aModel[nCntModelParts].rot.y, g_Enemy[nCntEnemy].aModel[nCntModelParts].rot.x, g_Enemy[nCntEnemy].aModel[nCntModelParts].rot.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxrot);


				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_Enemy[nCntEnemy].aModel[nCntModelParts].pos.x, g_Enemy[nCntEnemy].aModel[nCntModelParts].pos.y, g_Enemy[nCntEnemy].aModel[nCntModelParts].pos.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxtrans);

				//親のマトリックス反映
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxParent);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].aModel[nCntModelParts].mtxWorld);


				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].aModel[nCntModelParts].pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].aModel[nCntModelParts].nNumMat; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//pDevice->SetTexture(0, g_Enemy[nCntEnemy].aModel[0].pEnemyTexture);
					pDevice->SetTexture(0, 0);
					// モデル(パーツ)の描画
					g_Enemy[nCntEnemy].aModel[nCntModelParts].pMesh->DrawSubset(nCntMat);
				}
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}

		/////////////////
		//当たり判定用//
		///////////////
		if (g_Enemy[nCntEnemy].Collision.nIdxParent == -1)
		{// 体の時、プレイヤーのマトリックスに入れる
			mtxParent = g_Enemy[nCntEnemy].aModel[16].mtxWorld;
		}
		if (g_Enemy[nCntEnemy].Collision.nIdxParent == 0)
		{// 体の時、プレイヤーのマトリックスに入れる
			mtxParent = g_Enemy[nCntEnemy].aModel[15].mtxWorld;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy[nCntEnemy].Collision.mtxWorldCollision);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot, g_Enemy[nCntEnemy].Collision.rot.y, g_Enemy[nCntEnemy].Collision.rot.x, g_Enemy[nCntEnemy].Collision.rot.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Collision.mtxWorldCollision, &g_Enemy[nCntEnemy].Collision.mtxWorldCollision, &mtxrot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans, g_Enemy[nCntEnemy].Collision.pos.x, g_Enemy[nCntEnemy].Collision.pos.y, g_Enemy[nCntEnemy].Collision.pos.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Collision.mtxWorldCollision, &g_Enemy[nCntEnemy].Collision.mtxWorldCollision, &mtxtrans);

		//親のマトリックス反映
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Collision.mtxWorldCollision, &g_Enemy[nCntEnemy].Collision.mtxWorldCollision, &mtxParent);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].Collision.mtxWorldCollision);
	}
}
//=============================================================================
// ヒット処理
//=============================================================================
void HitEnemy(int nDamage, int nCntEnemy)
{
	D3DXVECTOR3 VecP;
	
	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	if (g_Enemy[nCntEnemy].bUse == true)
	{
		g_Enemy[nCntEnemy].nLife += nDamage;

		VecP.x =   g_Enemy[nCntEnemy].pos.x - pPlayer->pos.x;
		VecP.y = g_Enemy[nCntEnemy].pos.y - pPlayer->pos.y;
		VecP.z = g_Enemy[nCntEnemy].pos.z - pPlayer->pos.z * -1;


		D3DXVec3Normalize(&VecP, &VecP);
		//g_Enemy[nCntEnemy].pos += VecP * 80.0f;

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			SetParticle(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 30.0f, g_Enemy[nCntEnemy].pos.z), 60, D3DXCOLOR(1.0f, 0.0f, 0.8f, 1.0f), PARTICLETYPE_EXPLOSION);
		}
		if (g_Enemy[nCntEnemy].nLife <= 0)
		{

			g_Enemy[nCntEnemy].bUse = false;
			DeletShadow(g_Enemy[nCntEnemy].nIndexShadow);
			DeleteSencerCol(g_Enemy[nCntEnemy].nIndexSencer);
			DeleteSencerEnemy(g_Enemy[nCntEnemy].nIndexSencerEnemy);
			DeleteSencerEnableAttack(g_Enemy[nCntEnemy].nIndexSencerEnableAttack);
			DecreaseBrazen(-2);

		}

	}
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CollisionEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosold, D3DXVECTOR3 * VtxMax, D3DXVECTOR3 * VtxMin)
{
	bool bCollision = false; //着地したか

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x && pPos->x - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x
				&&pPos->y + VtxMax->y >= g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y && pPos->y - VtxMax->y <= g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y
				&&pPos->z + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z && pPos->z - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z)
			{
				if (pPosold->x + VtxMax->z <= g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x  && pPos->x + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x)
				{//左からきたとき
					pPos->x = g_Enemy[nCntEnemy].VtxMinEnemy.x + g_Enemy[nCntEnemy].pos.x - VtxMax->z;
					bCollision = true;
				}

				else if (pPosold->x - VtxMax->z >= g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x  && pPos->x - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x)
				{//右からきたとき
					pPos->x = g_Enemy[nCntEnemy].VtxMaxEnemy.x + g_Enemy[nCntEnemy].pos.x + VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z + VtxMax->z <= g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z && pPos->z + VtxMax->z >= g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z)
				{//手前から来た時
					pPos->z = g_Enemy[nCntEnemy].VtxMinEnemy.z + g_Enemy[nCntEnemy].pos.z - VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z - VtxMax->z >= g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z && pPos->z - VtxMax->z <= g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z)
				{//奥から来た時
					pPos->z = g_Enemy[nCntEnemy].VtxMaxEnemy.z + g_Enemy[nCntEnemy].pos.z + VtxMax->z;
					bCollision = true;

				}

				if (pPosold->y + VtxMax->y <= g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y && pPos->y + VtxMax->y >= g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y)
				{//上から来た時
					pPos->y = g_Enemy[nCntEnemy].VtxMinEnemy.y + g_Enemy[nCntEnemy].pos.y + VtxMin->y;
					bCollision = true;

				}
				if (pPosold->y - VtxMax->y >= g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y && pPos->y - VtxMax->y <= g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y)
				{//下から来た時
					pPos->y = g_Enemy[nCntEnemy].VtxMaxEnemy.y + g_Enemy[nCntEnemy].pos.y + VtxMax->y;
					bCollision = true;

				}
			}

		}
	}
	return bCollision;
}

//////////////////////////////////////////////////////////////////////////
//////				敵の設置								  ////////////
//////////////////////////////////////////////////////////////////////////
int SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	int nCntEnemy;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == false)
		{
			g_Enemy[nCntEnemy].pos = pos;
			g_Enemy[nCntEnemy].HomePoint = pos;
			g_Enemy[nCntEnemy].rot = rot;
			g_Enemy[nCntEnemy].nLife = nLife;
			g_Enemy[nCntEnemy].nIndexSencer = SetSencerCollision(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].rot, SENCERCOLTYPE_ENEMY);
			g_Enemy[nCntEnemy].nIndexSencerEnemy = SetSencerEnemy(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].rot);
			g_Enemy[nCntEnemy].nIndexSencerEnableAttack = SetSencerEnableAttack(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].rot);
			g_Enemy[nCntEnemy].nIndexShadow = SetShadow(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].rot);
			g_Enemy[nCntEnemy].state2 = ENEMYSTATE2_NORMAL;
			g_Enemy[nCntEnemy].bUse = true;
			break;
		}
	}
	return nCntEnemy;
}

//------------------------------------------------------------------------
// エネミーの状態の設定												      |
//------------------------------------------------------------------------
void SetEnemyStatus(ENEMYSTATUS status, int nCntEnemy)
{
	g_EnemyStatus = status;
}

////////////////////////////////////////////////////////////
//				敵の消去								  //
////////////////////////////////////////////////////////////
void DeleteEnemy(int nCntEnemy)
{
	g_Enemy[nCntEnemy].bUse = false;
	DeletShadow(g_Enemy[nCntEnemy].nIndexShadow);
	DeleteSencerCol(g_Enemy[nCntEnemy].nIndexSencer);
	DeleteSencerEnemy(g_Enemy[nCntEnemy].nIndexSencerEnemy);
	DeleteSencerEnableAttack(g_Enemy[nCntEnemy].nIndexSencerEnableAttack);

}

////////////////////////////////////////////////////////////
//				敵の取得								  //
////////////////////////////////////////////////////////////
ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}
///////////////////////////////////////////////////////////
//				敵状態の取得							//
//////////////////////////////////////////////////////////
ENEMYSTATUS *GetEnemyStatus(void)
{
	return &g_Enemy[0].status;
}


