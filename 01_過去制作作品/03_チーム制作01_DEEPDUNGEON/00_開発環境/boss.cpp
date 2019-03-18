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
#include "boss.h"
#include "sound.h"
#include "bosstext.h"
#include "bossmotion.h"
#include "SencerCol.h"
#include "SencerLock.h"
#include "SencerEnemy.h"
#include "SencerAtk.h"
#include "SencerEnableBoss.h"
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

BOSS g_Boss;
BOSSSTATUS g_BossStatus = BOSSSTATUS_NEUTRAL;
BOSSSTATUS g_BossStatusNow = BOSSSTATUS_NEUTRAL;

//STAGE g_aLimit[4];
//テキスト情報取得

float fGravity_Boss;	//重力

int g_nCntBoss;

bool bLand_Boss; //着地判定
//=============================================================================
// 初期化処理
//=============================================================================
void InitBoss(void)
{


	BossTex *pBossText;
	pBossText = GetBossTex();
	g_nCntBoss = 3;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	
		for (int nCntIdx = 0; nCntIdx < pBossText->MAX_PARTS; nCntIdx++)
		{//各種値の代入
		 //読み込むモデル名
			g_Boss.aModel[nCntIdx].FileName[0] = pBossText->aModelTex[nCntIdx].FileName[0];

			//親子関係
			g_Boss.aModel[nCntIdx].nIdxModelParent = pBossText->aModelTex[nCntIdx].Index;

			//位置
			g_Boss.aModel[nCntIdx].pos = D3DXVECTOR3(pBossText->aModelTex[nCntIdx].PosX, pBossText->aModelTex[nCntIdx].PosY, pBossText->aModelTex[nCntIdx].PosZ);

			//回転
			g_Boss.aModel[nCntIdx].rot = D3DXVECTOR3(pBossText->aModelTex[nCntIdx].RotX,
				pBossText->aModelTex[nCntIdx].RotY,
				pBossText->aModelTex[nCntIdx].RotZ);

			//モデルの設定
			D3DXLoadMeshFromX(&pBossText->aModelTex[nCntIdx].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Boss.aModel[nCntIdx].pBuffMat,
				NULL,
				&g_Boss.aModel[nCntIdx].nNumMat,
				&g_Boss.aModel[nCntIdx].pMesh);
		}
		g_Boss.Collision.bUse = false;
		g_Boss.Collision.nCntMotion = 0;
		g_Boss.Collision.StartFlame = 5;
		g_Boss.Collision.fRadius = 40.0f;
		g_Boss.Collision.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		g_Boss.Collision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Boss.Collision.nIdxParent = 0;
		g_Boss.nCntCoolDown = 120;

		SetSencerAtk(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 50.0f));
		SetSencerAtk(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 40.0f));
		SetSencerAtk(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 30.0f));
		SetSencerAtk(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 20.0f));
		SetSencerAtk(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 10.0f));
		SetSencerAtk(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43));


		
		//ボスの状態
		g_Boss.nCoolDown = 0;

		//着地しているか
		bLand_Boss = false;

		//ボスが使用されているか
		g_Boss.bUse = false;

	



	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ
	
		//頂点数を取得
		nNumVtx = g_Boss.aModel[0].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Boss.aModel[0].pMesh->GetFVF());

		//頂点バッファをロック
		g_Boss.aModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_Boss.aModel[0].VtxMinModel.x)
			{
				g_Boss.aModel[0].VtxMinModel.x = Vtx.x;
			}

			if (Vtx.y < g_Boss.aModel[0].VtxMinModel.y)
			{
				g_Boss.aModel[0].VtxMinModel.y = Vtx.y;
			}

			if (Vtx.z < g_Boss.aModel[0].VtxMinModel.z)
			{
				g_Boss.aModel[0].VtxMinModel.z = Vtx.x;
			}


			if (Vtx.x > g_Boss.aModel[0].VtxMaxModel.x)
			{
				g_Boss.aModel[0].VtxMaxModel.x = Vtx.x + 20.0f;
			}

			if (Vtx.y > g_Boss.aModel[0].VtxMaxModel.y)
			{
				g_Boss.aModel[0].VtxMaxModel.y = Vtx.y + 30.0f;
			}

			if (Vtx.z > g_Boss.aModel[0].VtxMaxModel.z)
			{
				g_Boss.aModel[0].VtxMaxModel.z = Vtx.z + 20.0f;
			}
			pVtxBuff += sizeFVF;
		}

		g_Boss.VtxMinBoss.x = g_Boss.aModel[0].VtxMinModel.x;
		g_Boss.VtxMaxBoss.x = g_Boss.aModel[0].VtxMaxModel.x;

		g_Boss.VtxMinBoss.y = g_Boss.aModel[0].VtxMinModel.y;
		g_Boss.VtxMaxBoss.y = g_Boss.aModel[0].VtxMaxModel.y;

		g_Boss.VtxMinBoss.z = g_Boss.aModel[0].VtxMinModel.z;
		g_Boss.VtxMaxBoss.z = g_Boss.aModel[0].VtxMaxModel.z;

		g_Boss.fAngle = 0.0f;			//差分



	CAMERA *pCamera;
	pCamera = GetCamera();


}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{


	
		// メッシュの開放
		if (g_Boss.aModel[0].pMesh != NULL)
		{
			g_Boss.aModel[0].pMesh->Release();
			g_Boss.aModel[0].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_Boss.aModel[0].pBuffMat != NULL)
		{
			g_Boss.aModel[0].pBuffMat->Release();
			g_Boss.aModel[0].pBuffMat = NULL;
		}
	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{

	BossMotion *pBoss_Motion = GetBossMotion();					//モーションデータの所得
	BossTex *pBoss_Text = GetBossTex();							//ファイルデータの取得

	BossTex *pBossText;
	pBossText = GetBossTex();
	STAGEMODE g_Stage = GetStageMode();				//現在のステージ番号を取得


#if 1


		g_Boss.nCntCoolDown++;
		g_Boss.posold = g_Boss.pos;

		CAMERA pCamera;
		pCamera = *GetCamera();
		PLAYER *pPlayer;
		//ボスの取得
		pPlayer = GetPlayer();
		D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
		BulletMove.x = -30.0f;
		BulletMove.z = -30.0f;

		D3DXVECTOR3 BulletMove2(0.0f, 0.0f, 0.0f);
		BulletMove2.x = -7.0f;
		BulletMove2.z = -7.0f;

		D3DXVECTOR3 VecP;
		D3DXVECTOR3 VecLength;


		VecP.x = (pPlayer->pos.x - g_Boss.pos.x);
		VecP.y = pPlayer->pos.y - g_Boss.pos.y;
		VecP.z = (pPlayer->pos.z - g_Boss.pos.z) + 45.0f;
		D3DXVec3Normalize(&VecP, &VecP);

		VecLength = pPlayer->pos - g_Boss.pos;









		if (g_Boss.bUse == true)
		{
			switch (g_Boss.state)
			{
			case BOSSSTATE_NORMAL:
				break;

			case BOSSSTATE_DAMAGE:
				g_Boss.nCntCoolDown--;
				if (g_Boss.nCntCoolDown <= 0)
				{
					g_Boss.state = BOSSSTATE_NORMAL;
				}
			}



			static float fAngle;
			//fAngle = atan2f(pPlayer->pos.x - g_Boss.pos.x, pPlayer->pos.z - g_Boss.pos.z);
			fAngle = atan2f(g_Boss.pos.x - pPlayer->pos.x, g_Boss.pos.z - pPlayer->pos.z);

			g_Boss.fLength = sqrtf((pPlayer->pos.x - g_Boss.pos.x) * (pPlayer->pos.x - g_Boss.pos.x) + (pPlayer->pos.z - g_Boss.pos.z) *(pPlayer->pos.z - g_Boss.pos.z));//距離


			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}

			if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}


				g_Boss.move.x = VecP.x * 2.0f;
				g_Boss.move.z = VecP.z * 2.0f;

				if (g_BossStatusNow == BOSSSTATUS_NEUTRAL)
				{//ムーヴへの移行
					SetBossStatus(BOSSSTATUS_MOVE);
				}
				if (CollisionSencer_EnableBoss(&pPlayer->pos) == true)
				{
					if (g_BossStatusNow == BOSSSTATUS_NEUTRAL || g_BossStatusNow == BOSSSTATUS_MOVE)
					{//ムーヴへの移行
						pBoss_Motion->nCntFlame = 0;
						if (g_Boss.nCntCoolDown % 30 == 0)
						{
							SetBossStatus(BOSSSTATUS_ATTACK);
						}
					}

					else if (g_BossStatusNow == BOSSSTATUS_ATTACK)
					{
						if (pBoss_Motion->nCntFlame % g_Boss.Collision.StartFlame == 0)
						{
							PlaySound(SOUND_LABEL_SE_SLASH);
						}
						if (pBoss_Motion->nCntFlame > g_Boss.Collision.StartFlame)
						{
							CollisionAttackBoss(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 50.0f)
								, g_Boss.Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY_BOSS);
							CollisionAttackBoss(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 40.0f)
								, g_Boss.Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY_BOSS);
							CollisionAttackBoss(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 30.0f)
								, g_Boss.Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY_BOSS);
							CollisionAttackBoss(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 20.0f)
								, g_Boss.Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY_BOSS);
							CollisionAttackBoss(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43 - 10.0f)
								, g_Boss.Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY_BOSS);
							CollisionAttackBoss(D3DXVECTOR3(g_Boss.Collision.mtxWorldCollision._41, g_Boss.Collision.mtxWorldCollision._42, g_Boss.Collision.mtxWorldCollision._43)
								, g_Boss.Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY_BOSS);

							//CollisionAttackEnemy(D3DXVECTOR3(g_Enemy[nCntEnemy].Collision.mtxWorldCollision._41, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._42, g_Enemy[nCntEnemy].Collision.mtxWorldCollision._43)
							//	, g_Enemy[nCntEnemy].Collision.fRadius
							//	, pPlayer->nIdxSencer
							//	, SENCERCOLTYPE_ENEMY);
						}
					}
				}
				g_Boss.rot.y = fAngle;

			g_Boss.pos += g_Boss.move;

			g_Boss.move.y -= GRAVITY_E;
			if (g_Boss.move.y > GRAVITY_E_MAX)
			{
				g_Boss.move.y = GRAVITY_E_MAX;
			}
			if (g_Boss.pos.y <= 0.0)
			{

				g_Boss.pos.y = 0.0f;
				g_Boss.move.y = 0.0f;

			}



			SetPositionSencerCollision(g_Boss.nIndexSencer, g_Boss.pos);//センサーの位置
			SetPositionShadow(g_Boss.nIndexShadow, g_Boss.pos);
			SetPositionSencerEnemy(g_Boss.nIndexSencerBoss,D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y, g_Boss.pos.z));
			SetPositionSencerEnableBoss(g_Boss.nIndexSencerEnableAttack, g_Boss.pos);
			CollisionSencer_Lock(&g_Boss.pos);




			switch (g_BossStatus)
			{
			case BOSSSTATUS_NEUTRAL:
				if (pBoss_Motion->KeyFlame == pBoss_Text->KEYFLAME[pBoss_Motion->nNumBossMotion] - 1)
				{
					if (g_BossStatusNow == BOSSSTATUS_NEUTRAL)
					{

						SetBossMotion(0);
					}
					else
					{
						pBoss_Motion->KeyFlame = 0;
						SetBossMotion(0);
						g_BossStatusNow = BOSSSTATUS_NEUTRAL;
						g_BossStatus = BOSSSTATUS_NEUTRAL;
					}
				}
				break;
			case BOSSSTATUS_MOVE:
				g_BossStatusNow = BOSSSTATUS_MOVE;
				g_BossStatus = BOSSSTATUS_NEUTRAL;
				SetBossMotion(1);
				break;
			case BOSSSTATUS_ATTACK:
				g_BossStatusNow = BOSSSTATUS_ATTACK;
				g_BossStatus = BOSSSTATUS_NEUTRAL;
				SetBossMotion(2);
				break;
			case BOSSSTATUS_JUMP:
				g_BossStatusNow = BOSSSTATUS_JUMP;
				g_BossStatus = BOSSSTATUS_NEUTRAL;
				SetBossMotion(3);
				break;
			case BOSSSTATUS_LANDING:
				g_BossStatusNow = BOSSSTATUS_LANDING;
				g_BossStatus = BOSSSTATUS_NEUTRAL;
				SetBossMotion(4);
				break;
			case BOSSSTATUS_GUARD:
				g_BossStatusNow = BOSSSTATUS_GUARD;
				g_BossStatus = BOSSSTATUS_NEUTRAL;
				SetBossMotion(5);
				break;
			case BOSSSTATUS_DAMAGE1:
				g_BossStatusNow = BOSSSTATUS_DAMAGE1;
				g_BossStatus = BOSSSTATUS_NEUTRAL;
				SetBossMotion(6);
				break;

			}//switch
			

			if (g_Stage == STAGEMODE_STAGE1)
			{//現在のステージがステージ1
			 //奥壁のあたり判定
				CollisionWall1_WIDTHBack_Ene(&g_Boss.pos, &g_Boss.posold);

				//手前壁のあたり判定
				CollisionWall1_WIDTHThisSide_Ene(&g_Boss.pos, &g_Boss.posold);

				//右壁のあたり判定
				CollisionWall1_RIGHT_Ene(&g_Boss.pos, &g_Boss.posold);

				//右壁のあたり判定
				CollisionWall1_LEFT_Ene(&g_Boss.pos, &g_Boss.posold);
			}
			else if (g_Stage == STAGEMODE_STAGE2)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWall2_WIDTHBack_Ene(&g_Boss.pos, &g_Boss.posold);

				//手前壁のあたり判定
				CollisionWall2_WIDTHThisSide_Ene(&g_Boss.pos, &g_Boss.posold);

				//右壁のあたり判定
				CollisionWall2_RIGHT_Ene(&g_Boss.pos, &g_Boss.posold);

				//右壁のあたり判定
				CollisionWall2_LEFT_Ene(&g_Boss.pos, &g_Boss.posold);
			}
			else if (g_Stage == STAGEMODE_STAGE3)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWallBoss_WIDTHBack_Ene(&g_Boss.pos, &g_Boss.posold);

				//手前壁のあたり判定
				CollisionWallBoss_WIDTHThisSide_Ene(&g_Boss.pos, &g_Boss.posold);

				//右壁のあたり判定
				CollisionWallBoss_RIGHT_Ene(&g_Boss.pos, &g_Boss.posold);

				//右壁のあたり判定
				CollisionWallBoss_LEFT_Ene(&g_Boss.pos, &g_Boss.posold);
			}
		}//buse
#endif

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
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
	//ボスのインデックス//
	//========================//
	
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Boss.mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Boss.rot.y, g_Boss.rot.x, g_Boss.rot.z);

		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxrot);


		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans,
			g_Boss.pos.x, g_Boss.pos.y, g_Boss.pos.z);
		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxtrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss.mtxWorld);


		for (int nCntModelParts = 0; nCntModelParts < MAX_BOSS_PARTS; nCntModelParts++)
		{
			if (g_Boss.bUse == true)
			{

				if (g_Boss.aModel[nCntModelParts].nIdxModelParent == -1)
				{//ボスのマトリックスに
					mtxParent = g_Boss.mtxWorld;
				}
				else if (g_Boss.aModel[nCntModelParts].nIdxModelParent == 0)
				{//体のマトリックスに
					mtxParent = g_Boss.aModel[0].mtxWorld;
				}
				else if (g_Boss.aModel[nCntModelParts].nIdxModelParent == 1)
				{//軸１に
					mtxParent = g_Boss.aModel[1].mtxWorld;
				}
				else if (g_Boss.aModel[nCntModelParts].nIdxModelParent == 2)
				{//軸２に
					mtxParent = g_Boss.aModel[2].mtxWorld;
				}
				else if (g_Boss.aModel[nCntModelParts].nIdxModelParent == 5)
				{//軸２に
					mtxParent = g_Boss.aModel[5].mtxWorld;
				}
				else if (g_Boss.aModel[nCntModelParts].nIdxModelParent == 8)
				{//軸２に
					mtxParent = g_Boss.aModel[8].mtxWorld;
				}
				else
				{//その他
					mtxParent = g_Boss.aModel[nCntModelParts - 1].mtxWorld;
				}
				//=======================================================================//
				//						    親モデルのインデックス						//
				//=====================================================================//
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Boss.aModel[nCntModelParts].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_Boss.aModel[nCntModelParts].rot.y, g_Boss.aModel[nCntModelParts].rot.x, g_Boss.aModel[nCntModelParts].rot.z);
				D3DXMatrixMultiply(&g_Boss.aModel[nCntModelParts].mtxWorld, &g_Boss.aModel[nCntModelParts].mtxWorld, &mtxrot);


				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_Boss.aModel[nCntModelParts].pos.x, g_Boss.aModel[nCntModelParts].pos.y, g_Boss.aModel[nCntModelParts].pos.z);
				D3DXMatrixMultiply(&g_Boss.aModel[nCntModelParts].mtxWorld, &g_Boss.aModel[nCntModelParts].mtxWorld, &mtxtrans);

				//親のマトリックス反映
				D3DXMatrixMultiply(&g_Boss.aModel[nCntModelParts].mtxWorld, &g_Boss.aModel[nCntModelParts].mtxWorld, &mtxParent);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Boss.aModel[nCntModelParts].mtxWorld);


				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Boss.aModel[nCntModelParts].pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)g_Boss.aModel[nCntModelParts].nNumMat; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//pDevice->SetTexture(0, g_Boss.aModel[0].pBossTexture);
					pDevice->SetTexture(0, 0);
					// モデル(パーツ)の描画
					g_Boss.aModel[nCntModelParts].pMesh->DrawSubset(nCntMat);
				}
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}

		/////////////////
		//当たり判定用//
		///////////////
		
		if (g_Boss.Collision.nIdxParent == 0)
		{// 体の時、ボスのマトリックスに入れる
			mtxParent = g_Boss.aModel[15].mtxWorld;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Boss.Collision.mtxWorldCollision);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot, g_Boss.Collision.rot.y, g_Boss.Collision.rot.x, g_Boss.Collision.rot.z);
		D3DXMatrixMultiply(&g_Boss.Collision.mtxWorldCollision, &g_Boss.Collision.mtxWorldCollision, &mtxrot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans, g_Boss.Collision.pos.x, g_Boss.Collision.pos.y, g_Boss.Collision.pos.z);
		D3DXMatrixMultiply(&g_Boss.Collision.mtxWorldCollision, &g_Boss.Collision.mtxWorldCollision, &mtxtrans);

		//親のマトリックス反映
		D3DXMatrixMultiply(&g_Boss.Collision.mtxWorldCollision, &g_Boss.Collision.mtxWorldCollision, &mtxParent);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss.Collision.mtxWorldCollision);

	}

//=============================================================================
// ヒット処理
//=============================================================================
void HitBoss(int nDamage)
{
	D3DXVECTOR3 VecP;
	
	PLAYER *pPlayer;

	

	//ボスの取得
	pPlayer = GetPlayer();

	if (g_Boss.bUse == true)
	{
		g_Boss.nLife += nDamage;

		VecP.x = g_Boss.pos.x - pPlayer->pos.x;
		VecP.y = g_Boss.pos.y - pPlayer->pos.y;
		VecP.z = g_Boss.pos.z - pPlayer->pos.z * -1;


		D3DXVec3Normalize(&VecP, &VecP);
		//g_Boss.pos += VecP * 80.0f;

		

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			SetParticle(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + 30.0f, g_Boss.pos.z), 60, D3DXCOLOR(1.0f, 0.0f, 0.8f, 1.0f), PARTICLETYPE_EXPLOSION);
		}
		if (g_Boss.nLife <= 0)
		{
			DecreaseBrazen(-30);
			g_Boss.bUse = false;
			DeletShadow(g_Boss.nIndexShadow);
			DeleteSencerCol(g_Boss.nIndexSencer);
			DeleteSencerEnemy(g_Boss.nIndexSencerBoss);
			DeleteSencerEnableBoss(g_Boss.nIndexSencerEnableAttack);

			int pBrazen;
			pBrazen = GetBrazen();

			if (pBrazen >= 75)
			{
				SetGameState(GAMESTATE_CLEAR_75);
			}
			if (pBrazen >= 50 && pBrazen <= 74)
			{
				SetGameState(GAMESTATE_CLEAR_50);
			}
			if (pBrazen >= 1 && pBrazen <= 49)
			{
				SetGameState(GAMESTATE_CLEAR_50);
			}
			if (pBrazen <= 0)
			{
				SetGameState(GAMESTATE_CLEAR_TRUE);
			}
		}

	}
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CollisionBoss(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosold, D3DXVECTOR3 * VtxMax, D3DXVECTOR3 * VtxMin)
{
	bool bCollision = false; //着地したか

	
		if (g_Boss.bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_Boss.VtxMinBoss.x + g_Boss.pos.x && pPos->x - VtxMax->z <= g_Boss.VtxMaxBoss.x + g_Boss.pos.x
				&&pPos->y + VtxMax->y >= g_Boss.VtxMinBoss.y + g_Boss.pos.y && pPos->y - VtxMax->y <= g_Boss.VtxMaxBoss.y + g_Boss.pos.y
				&&pPos->z + VtxMax->z >= g_Boss.VtxMinBoss.z + g_Boss.pos.z && pPos->z - VtxMax->z <= g_Boss.VtxMaxBoss.z + g_Boss.pos.z)
			{
				if (pPosold->x + VtxMax->z <= g_Boss.VtxMinBoss.x + g_Boss.pos.x  && pPos->x + VtxMax->z >= g_Boss.VtxMinBoss.x + g_Boss.pos.x)
				{//左からきたとき
					pPos->x = g_Boss.VtxMinBoss.x + g_Boss.pos.x - VtxMax->z;
					bCollision = true;
				}

				else if (pPosold->x - VtxMax->z >= g_Boss.VtxMaxBoss.x + g_Boss.pos.x  && pPos->x - VtxMax->z <= g_Boss.VtxMaxBoss.x + g_Boss.pos.x)
				{//右からきたとき
					pPos->x = g_Boss.VtxMaxBoss.x + g_Boss.pos.x + VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z + VtxMax->z <= g_Boss.VtxMinBoss.z + g_Boss.pos.z && pPos->z + VtxMax->z >= g_Boss.VtxMinBoss.z + g_Boss.pos.z)
				{//手前から来た時
					pPos->z = g_Boss.VtxMinBoss.z + g_Boss.pos.z - VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z - VtxMax->z >= g_Boss.VtxMaxBoss.z + g_Boss.pos.z && pPos->z - VtxMax->z <= g_Boss.VtxMaxBoss.z + g_Boss.pos.z)
				{//奥から来た時
					pPos->z = g_Boss.VtxMaxBoss.z + g_Boss.pos.z + VtxMax->z;
					bCollision = true;

				}

				if (pPosold->y + VtxMax->y <= g_Boss.VtxMinBoss.y + g_Boss.pos.y && pPos->y + VtxMax->y >= g_Boss.VtxMinBoss.y + g_Boss.pos.y)
				{//上から来た時
					pPos->y = g_Boss.VtxMinBoss.y + g_Boss.pos.y + VtxMin->y;
					bCollision = true;

				}
				if (pPosold->y - VtxMax->y >= g_Boss.VtxMaxBoss.y + g_Boss.pos.y && pPos->y - VtxMax->y <= g_Boss.VtxMaxBoss.y + g_Boss.pos.y)
				{//下から来た時
					pPos->y = g_Boss.VtxMaxBoss.y + g_Boss.pos.y + VtxMax->y;
					bCollision = true;

				}
			}

		}
	
	return bCollision;
}

//////////////////////////////////////////////////////////////////////////
//////				敵の設置								  ////////////
//////////////////////////////////////////////////////////////////////////
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
		if (g_Boss.bUse == false)
		{
			g_Boss.pos = pos;
			g_Boss.rot = rot;
			g_Boss.nLife = nLife;
			g_Boss.nIndexSencer = SetSencerCollision(g_Boss.pos, g_Boss.rot, SENCERCOLTYPE_ENEMY_BOSS);
			g_Boss.nIndexSencerBoss = SetSencerEnemy(g_Boss.pos, g_Boss.rot);
			g_Boss.nIndexSencerEnableAttack = SetSencerEnableBoss(g_Boss.pos, g_Boss.rot);
			g_Boss.nIndexShadow = SetShadow(g_Boss.pos, g_Boss.rot);
			g_Boss.bUse = true;
		}
	
}

//------------------------------------------------------------------------
// エネミーの状態の設定												      |
//------------------------------------------------------------------------
void SetBossStatus(BOSSSTATUS status)
{
	g_BossStatus = status;
}

////////////////////////////////////////////////////////////
//				敵の消去								  //
////////////////////////////////////////////////////////////
void DeleteBoss(int nCntEnemy)
{
	g_Boss.bUse = false;
	DeletShadow(g_Boss.nIndexShadow);
	DeleteSencerCol(g_Boss.nIndexSencer);
	DeleteSencerEnemy(g_Boss.nIndexSencerBoss);
	DeleteSencerEnableBoss(g_Boss.nIndexSencerEnableAttack);

}

////////////////////////////////////////////////////////////
//				敵の取得								  //
////////////////////////////////////////////////////////////
BOSS *GetBoss(void)
{
	return &g_Boss;
}
///////////////////////////////////////////////////////////
//				敵状態の取得							//
//////////////////////////////////////////////////////////
BOSSSTATUS *GetBossStatus(void)
{
	return &g_Boss.status;
}


