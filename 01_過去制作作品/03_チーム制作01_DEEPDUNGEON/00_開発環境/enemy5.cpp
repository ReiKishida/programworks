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
#include "enemy5.h"
#include "sound.h"
#include "enemytext5.h"
#include "enemymotion5.h"
#include "SencerCol.h"
#include "SencerLock.h"
#include "SencerEnemy.h"
#include "SencerAtk.h"
#include "SencerEnableAttack.h"
#include "Wall1.h"
#include "Wall2.h"
#include "WallBoss.h"
#include <math.h>
#include "brazen.h"

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

ENEMY_5 g_Enemy_5[MAX_ENEMY_5];
ENEMYSTATUS_5 g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
ENEMYSTATUS_5 g_Enemy_5StatusNow = ENEMYSTATUS_NEUTRAL_5;

//STAGE g_aLimit[4];
//テキスト情報取得

float fJump_E_5;	//ジャンプ力
float fGRAVITY_E_E_5;	//重力

int g_nCntSound_E_5 = 0;
int g_nCntEnemy_5;

bool bLand_E_5; //着地判定
 //=============================================================================
 // 初期化処理
 //=============================================================================
void InitEnemy_5(void)
{
	

	EnemyTex_5 *pEnemyText;
	pEnemyText = GetEnemyTex_5();
	g_nCntEnemy_5 = 3;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_5; nCntEnemy++)
	{
		for (int nCntIdx = 0; nCntIdx < pEnemyText->MAX_PARTS; nCntIdx++)
		{//各種値の代入
		 //読み込むモデル名
			g_Enemy_5[nCntEnemy].aModel[nCntIdx].FileName[0] = pEnemyText->aModelTex[nCntIdx].FileName[0];

			//親子関係
			g_Enemy_5[nCntEnemy].aModel[nCntIdx].nIdxModelParent = pEnemyText->aModelTex[nCntIdx].Index;

			//位置
			g_Enemy_5[nCntEnemy].aModel[nCntIdx].pos = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].PosX, pEnemyText->aModelTex[nCntIdx].PosY, pEnemyText->aModelTex[nCntIdx].PosZ);

			//回転
			g_Enemy_5[nCntEnemy].aModel[nCntIdx].rot = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].RotX,
				pEnemyText->aModelTex[nCntIdx].RotY,
				pEnemyText->aModelTex[nCntIdx].RotZ);

			//モデルの設定
			D3DXLoadMeshFromX(&pEnemyText->aModelTex[nCntIdx].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Enemy_5[nCntEnemy].aModel[nCntIdx].pBuffMat,
				NULL,
				&g_Enemy_5[nCntEnemy].aModel[nCntIdx].nNumMat,
				&g_Enemy_5[nCntEnemy].aModel[nCntIdx].pMesh);
		}
		g_Enemy_5[nCntEnemy].Collision.bUse = false;
		g_Enemy_5[nCntEnemy].Collision.nCntMotion = 0;
		g_Enemy_5[nCntEnemy].Collision.StartFlame = 5;
		g_Enemy_5[nCntEnemy].Collision.fRadius = 40.0f;
		g_Enemy_5[nCntEnemy].Collision.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		g_Enemy_5[nCntEnemy].Collision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy_5[nCntEnemy].Collision.nIdxParent = 0;
		g_Enemy_5[nCntEnemy].nCntCoolDown = 120;

		SetSencerAtk(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43 - 30.0f));
		SetSencerAtk(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43 - 20.0f));
		SetSencerAtk(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43 - 10.0f));
		SetSencerAtk(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43));


		//ジャンプの状態
		g_Enemy_5[nCntEnemy].bUseJump = false;
		//プレイヤーの状態
		g_Enemy_5[nCntEnemy].nCoolDown = 0;

		//着地しているか
		bLand_E_5 = false;

		//プレイヤーが使用されているか
		g_Enemy_5[nCntEnemy].bUse = false;

	}









	fJump_E_5 = JUMP_POWER;
	fGRAVITY_E_E_5 = GRAVITY_E;


	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_5; nCntEnemy++)
	{
		//頂点数を取得
		nNumVtx = g_Enemy_5[nCntEnemy].aModel[0].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Enemy_5[nCntEnemy].aModel[0].pMesh->GetFVF());

		//頂点バッファをロック
		g_Enemy_5[nCntEnemy].aModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.x)
			{
				g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.x = Vtx.x;
			}

			if (Vtx.y < g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.y)
			{
				g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.y = Vtx.y;
			}

			if (Vtx.z < g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.z)
			{
				g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.z = Vtx.x;
			}


			if (Vtx.x > g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.x)
			{
				g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.x = Vtx.x + 20.0f;
			}

			if (Vtx.y > g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.y)
			{
				g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.y = Vtx.y + 30.0f;
			}

			if (Vtx.z > g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.z)
			{
				g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.z = Vtx.z + 20.0f;
			}
			pVtxBuff += sizeFVF;
		}

		g_Enemy_5[nCntEnemy].VtxMinEnemy.x = g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.x;
		g_Enemy_5[nCntEnemy].VtxMaxEnemy.x = g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.x;

		g_Enemy_5[nCntEnemy].VtxMinEnemy.y = g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.y;
		g_Enemy_5[nCntEnemy].VtxMaxEnemy.y = g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.y;

		g_Enemy_5[nCntEnemy].VtxMinEnemy.z = g_Enemy_5[nCntEnemy].aModel[0].VtxMinModel.z;
		g_Enemy_5[nCntEnemy].VtxMaxEnemy.z = g_Enemy_5[nCntEnemy].aModel[0].VtxMaxModel.z;

		g_Enemy_5[nCntEnemy].fAngle = 0.0f;			//差分

	}

	CAMERA *pCamera;
	pCamera = GetCamera();


}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy_5(void)
{
	

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_5; nCntEnemy++)
	{
		// メッシュの開放
		if (g_Enemy_5[nCntEnemy].aModel[0].pMesh != NULL)
		{
			g_Enemy_5[nCntEnemy].aModel[0].pMesh->Release();
			g_Enemy_5[nCntEnemy].aModel[0].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_Enemy_5[nCntEnemy].aModel[0].pBuffMat != NULL)
		{
			g_Enemy_5[nCntEnemy].aModel[0].pBuffMat->Release();
			g_Enemy_5[nCntEnemy].aModel[0].pBuffMat = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy_5(void)
{

	EnemyMotion_5 *pE_Motion = GetEnemyMotion_5();					//モーションデータの所得
	EnemyTex_5 *pE_Text = GetEnemyTex_5();							//ファイルデータの取得

	EnemyTex_5 *pEnemyText;
	pEnemyText = GetEnemyTex_5();
	STAGEMODE g_Stage = GetStageMode();				//現在のステージ番号を取得


#if 1




	//nCntUpdateRogic++;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_5; nCntEnemy++)
	{




		g_Enemy_5[nCntEnemy].nCntCoolDown++;
		g_Enemy_5[nCntEnemy].posold = g_Enemy_5[nCntEnemy].pos;

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


		VecP.x = (pPlayer->pos.x - g_Enemy_5[nCntEnemy].pos.x);
		VecP.y = pPlayer->pos.y - g_Enemy_5[nCntEnemy].pos.y;
		VecP.z = (pPlayer->pos.z - g_Enemy_5[nCntEnemy].pos.z) + 45.0f;
		D3DXVec3Normalize(&VecP, &VecP);

		VecLength = pPlayer->pos - g_Enemy_5[nCntEnemy].pos;









		if (g_Enemy_5[nCntEnemy].bUse == true)
		{
			switch (g_Enemy_5[nCntEnemy].state2)
			{
			case ENEMYSTATE2_NORMAL_5:
				break;

			case ENEMYSTATE2_DAMAGE_5:
				g_Enemy_5[nCntEnemy].nCntCoolDown--;
				if (g_Enemy_5[nCntEnemy].nCntCoolDown <= 0)
				{
					g_Enemy_5[nCntEnemy].state2 = ENEMYSTATE2_NORMAL_5;
				}
			}



			static float fAngle;
			//fAngle = atan2f(pPlayer->pos.x - g_Enemy_5[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy_5[nCntEnemy].pos.z);
			fAngle = atan2f(g_Enemy_5[nCntEnemy].pos.x - pPlayer->pos.x, g_Enemy_5[nCntEnemy].pos.z - pPlayer->pos.z);

			g_Enemy_5[nCntEnemy].fLength = sqrtf((pPlayer->pos.x - g_Enemy_5[nCntEnemy].pos.x) * (pPlayer->pos.x - g_Enemy_5[nCntEnemy].pos.x) + (pPlayer->pos.z - g_Enemy_5[nCntEnemy].pos.z) *(pPlayer->pos.z - g_Enemy_5[nCntEnemy].pos.z));//距離


			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}

			if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}




			switch (g_Enemy_5[nCntEnemy].state)
			{
			case ENEMYSTATE_NONE_5:

				if (CollisionSencer_Enemy(&pPlayer->pos, g_Enemy_5[nCntEnemy].nIndexSencerEnemy) == true)
				{
					g_Enemy_5[nCntEnemy].state = ENEMYSTATE_DISCOVERY_5;
				}
				else
				{
					g_Enemy_5[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}

				break;

			case ENEMYSTATE_BACKHOME_5:

				g_Enemy_5[nCntEnemy].DestHome = atan2f(g_Enemy_5[nCntEnemy].HomePoint.x - g_Enemy_5[nCntEnemy].pos.x,
					g_Enemy_5[nCntEnemy].HomePoint.z - g_Enemy_5[nCntEnemy].pos.z);

				g_Enemy_5[nCntEnemy].move.x = g_Enemy_5[nCntEnemy].DestHome * 1.2f;
				g_Enemy_5[nCntEnemy].move.z = g_Enemy_5[nCntEnemy].DestHome * 1.2f;

				if (g_Enemy_5[nCntEnemy].pos.x <= g_Enemy_5[nCntEnemy].HomePoint.x + 50.0f || g_Enemy_5[nCntEnemy].pos.x >= g_Enemy_5[nCntEnemy].HomePoint.x - 50.0f
					|| g_Enemy_5[nCntEnemy].pos.z <= g_Enemy_5[nCntEnemy].HomePoint.z + 50.0f || g_Enemy_5[nCntEnemy].pos.z >= g_Enemy_5[nCntEnemy].HomePoint.z - 50.0f)
				{
					g_Enemy_5[nCntEnemy].state = ENEMYSTATE_NONE_5;
				}

				break;
			case ENEMYSTATE_DISCOVERY_5:
				g_Enemy_5[nCntEnemy].move.x = VecP.x * 1.5f;
				g_Enemy_5[nCntEnemy].move.z = VecP.z * 1.5f;
				fAngle = atan2f(g_Enemy_5[nCntEnemy].pos.x - pPlayer->pos.x, g_Enemy_5[nCntEnemy].pos.z - pPlayer->pos.z);
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}

				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				if (CollisionSencer_Enemy(&pPlayer->pos, g_Enemy_5[nCntEnemy].nIndexSencerEnemy) == false)
				{
					g_Enemy_5[nCntEnemy].state = ENEMYSTATE_BACKHOME_5;
				}
				if (g_Enemy_5StatusNow == ENEMYSTATUS_NEUTRAL_5)
				{//ムーヴへの移行
					SetEnemyStatus_5(ENEMYSTATUS_MOVE_5, nCntEnemy);
				}
				if (CollisionSencer_Enable(&pPlayer->pos) == true)
				{
					if (g_Enemy_5StatusNow == ENEMYSTATUS_NEUTRAL_5 || g_Enemy_5StatusNow == ENEMYSTATUS_MOVE_5)
					{//ムーヴへの移行
						pE_Motion->nCntFlame = 0;
						if (g_Enemy_5[nCntEnemy].nCntCoolDown % 30 == 0)
						{
							SetEnemyStatus_5(ENEMYSTATUS_ATTACK_5, nCntEnemy);
						}
					}

					else if (g_Enemy_5StatusNow == ENEMYSTATUS_ATTACK_5)
					{
						if (pE_Motion->nCntFlame % g_Enemy_5[nCntEnemy].Collision.StartFlame == 0)
						{
							PlaySound(SOUND_LABEL_SE_SLASH);
						}
						if (pE_Motion->nCntFlame > g_Enemy_5[nCntEnemy].Collision.StartFlame)
						{
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43 - 30.0f)
								, g_Enemy_5[nCntEnemy].Collision.fRadius
								, g_Enemy_5[nCntEnemy].nIndexSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43 - 20.0f)
								, g_Enemy_5[nCntEnemy].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43 - 10.0f)
								, g_Enemy_5[nCntEnemy].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision._43)
								, g_Enemy_5[nCntEnemy].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
						}
					}
					g_Enemy_5[nCntEnemy].rot.y = fAngle;
				}
				break;
			}//switch文

			g_Enemy_5[nCntEnemy].rot.y = fAngle;

#if 1

#endif			

			g_Enemy_5[nCntEnemy].pos += g_Enemy_5[nCntEnemy].move;

			g_Enemy_5[nCntEnemy].move.y -= GRAVITY_E;
			if (g_Enemy_5[nCntEnemy].move.y > GRAVITY_E_MAX)
			{
				g_Enemy_5[nCntEnemy].move.y = GRAVITY_E_MAX;
			}
			if (g_Enemy_5[nCntEnemy].pos.y <= 0.0)
			{

				g_Enemy_5[nCntEnemy].pos.y = 0.0f;
				g_Enemy_5[nCntEnemy].move.y = 0.0f;

			}



#endif
			SetPositionSencerCollision(g_Enemy_5[nCntEnemy].nIndexSencer, g_Enemy_5[nCntEnemy].pos);//センサーの位置
			SetPositionShadow(g_Enemy_5[nCntEnemy].nIndexShadow, g_Enemy_5[nCntEnemy].pos);
			SetPositionSencerEnemy(g_Enemy_5[nCntEnemy].nIndexSencerEnemy,
				D3DXVECTOR3(g_Enemy_5[nCntEnemy].pos.x, g_Enemy_5[nCntEnemy].pos.y, g_Enemy_5[nCntEnemy].pos.z));
			SetPositionSencerEnableAttack(g_Enemy_5[nCntEnemy].nIndexSencerEnableAttack, g_Enemy_5[nCntEnemy].pos);
			CollisionSencer_Lock(&g_Enemy_5[nCntEnemy].pos);
			//g_Enemy_5[nCntEnemy].rot.y += 0.01f;




			switch (g_Enemy_5Status)
			{
			case ENEMYSTATUS_NEUTRAL_5:
				if (pE_Motion->KeyFlame == pE_Text->KEYFLAME[pE_Motion->nNumEnemymotion] - 1)
				{
					if (g_Enemy_5StatusNow == ENEMYSTATUS_NEUTRAL_5)
					{

						SetEnemyMotion_5(0);
					}
					else
					{
						pE_Motion->KeyFlame = 0;
						SetEnemyMotion_5(0);
						g_Enemy_5StatusNow = ENEMYSTATUS_NEUTRAL_5;
						g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
					}
				}
				break;
			case ENEMYSTATUS_MOVE_5:
				g_Enemy_5StatusNow = ENEMYSTATUS_MOVE_5;
				g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
				SetEnemyMotion_5(1);
				break;
			case ENEMYSTATUS_ATTACK_5:
				g_Enemy_5StatusNow = ENEMYSTATUS_ATTACK_5;
				g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
				SetEnemyMotion_5(2);
				break;
			case ENEMYSTATUS_JUMP_5:
				g_Enemy_5StatusNow = ENEMYSTATUS_JUMP_5;
				g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
				SetEnemyMotion_5(3);
				break;
			case ENEMYSTATUS_LANDING_5:
				g_Enemy_5StatusNow = ENEMYSTATUS_LANDING_5;
				g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
				SetEnemyMotion_5(4);
				break;
			case ENEMYSTATUS_GUARD_5:
				g_Enemy_5StatusNow = ENEMYSTATUS_GUARD_5;
				g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
				SetEnemyMotion_5(5);
				break;
			case ENEMYSTATUS_DAMAGE1_5:
				g_Enemy_5StatusNow = ENEMYSTATUS_DAMAGE1_5;
				g_Enemy_5Status = ENEMYSTATUS_NEUTRAL_5;
				SetEnemyMotion_5(6);
				break;

			}//switch
		//if

			if (g_Stage == STAGEMODE_STAGE1)
			{//現在のステージがステージ1
			 //奥壁のあたり判定
				CollisionWall1_WIDTHBack_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);

				//手前壁のあたり判定
				CollisionWall1_WIDTHThisSide_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall1_RIGHT_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall1_LEFT_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);
			}
			else if (g_Stage == STAGEMODE_STAGE2)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWall2_WIDTHBack_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);

				//手前壁のあたり判定
				CollisionWall2_WIDTHThisSide_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall2_RIGHT_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);

				//右壁のあたり判定
				CollisionWall2_LEFT_Ene5(&g_Enemy_5[nCntEnemy].pos, &g_Enemy_5[nCntEnemy].posold);
			}

		}//buse

	}//for
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy_5(void)
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
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_5; nCntEnemy++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy_5[nCntEnemy].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Enemy_5[nCntEnemy].rot.y, g_Enemy_5[nCntEnemy].rot.x, g_Enemy_5[nCntEnemy].rot.z);

		D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].mtxWorld, &g_Enemy_5[nCntEnemy].mtxWorld, &mtxrot);


		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans,
			g_Enemy_5[nCntEnemy].pos.x, g_Enemy_5[nCntEnemy].pos.y, g_Enemy_5[nCntEnemy].pos.z);
		D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].mtxWorld, &g_Enemy_5[nCntEnemy].mtxWorld, &mtxtrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy_5[nCntEnemy].mtxWorld);


		for (int nCntModelParts = 0; nCntModelParts < MAX_E_PARTS_5; nCntModelParts++)
		{
			if (g_Enemy_5[nCntEnemy].bUse == true)
			{

				if (g_Enemy_5[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == -1)
				{//プレイヤーのマトリックスに
					mtxParent = g_Enemy_5[nCntEnemy].mtxWorld;
				}
				else if (g_Enemy_5[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 0)
				{//体のマトリックスに
					mtxParent = g_Enemy_5[nCntEnemy].aModel[0].mtxWorld;
				}
				else if (g_Enemy_5[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 1)
				{//軸１に
					mtxParent = g_Enemy_5[nCntEnemy].aModel[1].mtxWorld;
				}
				else if (g_Enemy_5[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 2)
				{//軸２に
					mtxParent = g_Enemy_5[nCntEnemy].aModel[2].mtxWorld;
				}
				else if (g_Enemy_5[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 5)
				{//軸２に
					mtxParent = g_Enemy_5[nCntEnemy].aModel[5].mtxWorld;
				}
				else if (g_Enemy_5[nCntEnemy].aModel[nCntModelParts].nIdxModelParent == 8)
				{//軸２に
					mtxParent = g_Enemy_5[nCntEnemy].aModel[8].mtxWorld;
				}
				else
				{//その他
					mtxParent = g_Enemy_5[nCntEnemy].aModel[nCntModelParts - 1].mtxWorld;
				}
				//=======================================================================//
				//						    親モデルのインデックス						//
				//=====================================================================//
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_Enemy_5[nCntEnemy].aModel[nCntModelParts].rot.y, g_Enemy_5[nCntEnemy].aModel[nCntModelParts].rot.x, g_Enemy_5[nCntEnemy].aModel[nCntModelParts].rot.z);
				D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxrot);


				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_Enemy_5[nCntEnemy].aModel[nCntModelParts].pos.x, g_Enemy_5[nCntEnemy].aModel[nCntModelParts].pos.y, g_Enemy_5[nCntEnemy].aModel[nCntModelParts].pos.z);
				D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxtrans);

				//親のマトリックス反映
				D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld, &g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld, &mtxParent);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Enemy_5[nCntEnemy].aModel[nCntModelParts].mtxWorld);


				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Enemy_5[nCntEnemy].aModel[nCntModelParts].pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)g_Enemy_5[nCntEnemy].aModel[nCntModelParts].nNumMat; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//pDevice->SetTexture(0, g_Enemy_5[nCntEnemy].aModel[0].pEnemyTexture);
					pDevice->SetTexture(0, 0);
					// モデル(パーツ)の描画
					g_Enemy_5[nCntEnemy].aModel[nCntModelParts].pMesh->DrawSubset(nCntMat);
				}
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}

		/////////////////
		//当たり判定用//
		///////////////
		if (g_Enemy_5[nCntEnemy].Collision.nIdxParent == -1)
		{// 体の時、プレイヤーのマトリックスに入れる
			mtxParent = g_Enemy_5[nCntEnemy].aModel[16].mtxWorld;
		}
		if (g_Enemy_5[nCntEnemy].Collision.nIdxParent == 0)
		{// 体の時、プレイヤーのマトリックスに入れる
			mtxParent = g_Enemy_5[nCntEnemy].aModel[15].mtxWorld;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot, g_Enemy_5[nCntEnemy].Collision.rot.y, g_Enemy_5[nCntEnemy].Collision.rot.x, g_Enemy_5[nCntEnemy].Collision.rot.z);
		D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision, &g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision, &mtxrot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans, g_Enemy_5[nCntEnemy].Collision.pos.x, g_Enemy_5[nCntEnemy].Collision.pos.y, g_Enemy_5[nCntEnemy].Collision.pos.z);
		D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision, &g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision, &mtxtrans);

		//親のマトリックス反映
		D3DXMatrixMultiply(&g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision, &g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision, &mtxParent);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy_5[nCntEnemy].Collision.mtxWorldCollision);
	}
}
//=============================================================================
// ヒット処理
//=============================================================================
void HitEnemy_5(int nDamage, int nCntEnemy)
{
	D3DXVECTOR3 VecP;
	
	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	if (g_Enemy_5[nCntEnemy].bUse == true)
	{
		g_Enemy_5[nCntEnemy].nLife += nDamage;

		VecP.x =   g_Enemy_5[nCntEnemy].pos.x - pPlayer->pos.x;
		VecP.y = g_Enemy_5[nCntEnemy].pos.y - pPlayer->pos.y;
		VecP.z = g_Enemy_5[nCntEnemy].pos.z - pPlayer->pos.z * -1;


		D3DXVec3Normalize(&VecP, &VecP);
		//g_Enemy_5[nCntEnemy].pos += VecP * 80.0f;

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			SetParticle(D3DXVECTOR3(g_Enemy_5[nCntEnemy].pos.x, g_Enemy_5[nCntEnemy].pos.y + 30.0f, g_Enemy_5[nCntEnemy].pos.z), 60, D3DXCOLOR(1.0f, 0.0f, 0.8f, 1.0f), PARTICLETYPE_EXPLOSION);
		}
		if (g_Enemy_5[nCntEnemy].nLife <= 0)
		{

			g_Enemy_5[nCntEnemy].bUse = false;
			DeletShadow(g_Enemy_5[nCntEnemy].nIndexShadow);
			DeleteSencerCol(g_Enemy_5[nCntEnemy].nIndexSencer);
			DeleteSencerEnemy(g_Enemy_5[nCntEnemy].nIndexSencerEnemy);
			DeleteSencerEnableAttack(g_Enemy_5[nCntEnemy].nIndexSencerEnableAttack);
			DecreaseBrazen(-1);
		}

	}
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CollisionEnemy_5(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosold, D3DXVECTOR3 * VtxMax, D3DXVECTOR3 * VtxMin)
{
	bool bCollision = false; //着地したか

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_5; nCntEnemy++)
	{
		if (g_Enemy_5[nCntEnemy].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_Enemy_5[nCntEnemy].VtxMinEnemy.x + g_Enemy_5[nCntEnemy].pos.x && pPos->x - VtxMax->z <= g_Enemy_5[nCntEnemy].VtxMaxEnemy.x + g_Enemy_5[nCntEnemy].pos.x
				&&pPos->y + VtxMax->y >= g_Enemy_5[nCntEnemy].VtxMinEnemy.y + g_Enemy_5[nCntEnemy].pos.y && pPos->y - VtxMax->y <= g_Enemy_5[nCntEnemy].VtxMaxEnemy.y + g_Enemy_5[nCntEnemy].pos.y
				&&pPos->z + VtxMax->z >= g_Enemy_5[nCntEnemy].VtxMinEnemy.z + g_Enemy_5[nCntEnemy].pos.z && pPos->z - VtxMax->z <= g_Enemy_5[nCntEnemy].VtxMaxEnemy.z + g_Enemy_5[nCntEnemy].pos.z)
			{
				if (pPosold->x + VtxMax->z <= g_Enemy_5[nCntEnemy].VtxMinEnemy.x + g_Enemy_5[nCntEnemy].pos.x  && pPos->x + VtxMax->z >= g_Enemy_5[nCntEnemy].VtxMinEnemy.x + g_Enemy_5[nCntEnemy].pos.x)
				{//左からきたとき
					pPos->x = g_Enemy_5[nCntEnemy].VtxMinEnemy.x + g_Enemy_5[nCntEnemy].pos.x - VtxMax->z;
					bCollision = true;
				}

				else if (pPosold->x - VtxMax->z >= g_Enemy_5[nCntEnemy].VtxMaxEnemy.x + g_Enemy_5[nCntEnemy].pos.x  && pPos->x - VtxMax->z <= g_Enemy_5[nCntEnemy].VtxMaxEnemy.x + g_Enemy_5[nCntEnemy].pos.x)
				{//右からきたとき
					pPos->x = g_Enemy_5[nCntEnemy].VtxMaxEnemy.x + g_Enemy_5[nCntEnemy].pos.x + VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z + VtxMax->z <= g_Enemy_5[nCntEnemy].VtxMinEnemy.z + g_Enemy_5[nCntEnemy].pos.z && pPos->z + VtxMax->z >= g_Enemy_5[nCntEnemy].VtxMinEnemy.z + g_Enemy_5[nCntEnemy].pos.z)
				{//手前から来た時
					pPos->z = g_Enemy_5[nCntEnemy].VtxMinEnemy.z + g_Enemy_5[nCntEnemy].pos.z - VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z - VtxMax->z >= g_Enemy_5[nCntEnemy].VtxMaxEnemy.z + g_Enemy_5[nCntEnemy].pos.z && pPos->z - VtxMax->z <= g_Enemy_5[nCntEnemy].VtxMaxEnemy.z + g_Enemy_5[nCntEnemy].pos.z)
				{//奥から来た時
					pPos->z = g_Enemy_5[nCntEnemy].VtxMaxEnemy.z + g_Enemy_5[nCntEnemy].pos.z + VtxMax->z;
					bCollision = true;

				}

				if (pPosold->y + VtxMax->y <= g_Enemy_5[nCntEnemy].VtxMinEnemy.y + g_Enemy_5[nCntEnemy].pos.y && pPos->y + VtxMax->y >= g_Enemy_5[nCntEnemy].VtxMinEnemy.y + g_Enemy_5[nCntEnemy].pos.y)
				{//上から来た時
					pPos->y = g_Enemy_5[nCntEnemy].VtxMinEnemy.y + g_Enemy_5[nCntEnemy].pos.y + VtxMin->y;
					bCollision = true;

				}
				if (pPosold->y - VtxMax->y >= g_Enemy_5[nCntEnemy].VtxMaxEnemy.y + g_Enemy_5[nCntEnemy].pos.y && pPos->y - VtxMax->y <= g_Enemy_5[nCntEnemy].VtxMaxEnemy.y + g_Enemy_5[nCntEnemy].pos.y)
				{//下から来た時
					pPos->y = g_Enemy_5[nCntEnemy].VtxMaxEnemy.y + g_Enemy_5[nCntEnemy].pos.y + VtxMax->y;
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
int SetEnemy_5(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	int nCntEnemy;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY_5; nCntEnemy++)
	{
		if (g_Enemy_5[nCntEnemy].bUse == false)
		{
			g_Enemy_5[nCntEnemy].pos = pos;
			g_Enemy_5[nCntEnemy].HomePoint = pos;
			g_Enemy_5[nCntEnemy].rot = rot;
			g_Enemy_5[nCntEnemy].nLife = nLife;
			g_Enemy_5[nCntEnemy].nIndexSencer = SetSencerCollision(g_Enemy_5[nCntEnemy].pos, g_Enemy_5[nCntEnemy].rot, SENCERCOLTYPE_ENEMY5);
			g_Enemy_5[nCntEnemy].nIndexSencerEnemy = SetSencerEnemy(g_Enemy_5[nCntEnemy].pos, g_Enemy_5[nCntEnemy].rot);
			g_Enemy_5[nCntEnemy].nIndexSencerEnableAttack = SetSencerEnableAttack(g_Enemy_5[nCntEnemy].pos, g_Enemy_5[nCntEnemy].rot);
			g_Enemy_5[nCntEnemy].nIndexShadow = SetShadow(g_Enemy_5[nCntEnemy].pos, g_Enemy_5[nCntEnemy].rot);
			g_Enemy_5[nCntEnemy].state2 = ENEMYSTATE2_NORMAL_5;
			g_Enemy_5[nCntEnemy].bUse = true;
			break;
		}
	}
	return nCntEnemy;
}

//------------------------------------------------------------------------
// エネミーの状態の設定												      |
//------------------------------------------------------------------------
void SetEnemyStatus_5(ENEMYSTATUS_5 status, int nCntEnemy)
{
	g_Enemy_5Status = status;
}

////////////////////////////////////////////////////////////
//				敵の消去								  //
////////////////////////////////////////////////////////////
void DeleteEnemy_5(int nCntEnemy)
{
	g_Enemy_5[nCntEnemy].bUse = false;
	DeletShadow(g_Enemy_5[nCntEnemy].nIndexShadow);
	DeleteSencerCol(g_Enemy_5[nCntEnemy].nIndexSencer);
	DeleteSencerEnemy(g_Enemy_5[nCntEnemy].nIndexSencerEnemy);
	DeleteSencerEnableAttack(g_Enemy_5[nCntEnemy].nIndexSencerEnableAttack);

}

////////////////////////////////////////////////////////////
//				敵の取得								  //
////////////////////////////////////////////////////////////
ENEMY_5 *GetEnemy_5(void)
{
	return &g_Enemy_5[0];
}
///////////////////////////////////////////////////////////
//				敵状態の取得							//
//////////////////////////////////////////////////////////
ENEMYSTATUS_5 *GetEnemyStatus_5(void)
{
	return &g_Enemy_5[0].status;
}


