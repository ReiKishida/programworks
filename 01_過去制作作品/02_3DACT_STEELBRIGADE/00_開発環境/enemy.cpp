//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author :
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "model.h"
#include "game.h"
#include "fade.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "boost.h"
#include "enemytext.h"
#include "enemymotion.h"
#include "smoke.h"
#include "modelwall.h"
#include "gate.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define JUMP_POWER (14.50f)	//ジャンプ力
#define GRAVITY_E (0.8f)		//重力
#define GRAVITY_E_MAX (150.0f)	//重力の最大量

#define DASHSPEED  (7.0f)	//ダッシュスピード
#define ROT_SPEED (0.05f)	//旋回速度
#define ROT_SPEED2 (0.2f)	//旋回速度

//ステージの限界値
#define STAGE_LIMIT_MAX_X (1450.0f)
#define STAGE_LIMIT_MIN_X (-40.0f)
#define STAGE_LIMIT_MAX_Z (30.0f)
#define STAGE_LIMIT_MIN_Z (-3800.0f)
//


#define EFFECT_MAX (50)//エフェクトの量

#define MOVE_BULLET_Y (25.0f)	//弾速
//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY g_Enemy[MAX_ENEMY];	//敵の情報
ENEMYSTATUS g_EnemyStatus = ENEMYSTATUS_NEUTRAL;	//敵の状態
ENEMYSTATUS g_EnemyStatusNow = ENEMYSTATUS_NEUTRAL;//敵の今の状態

//STAGE g_aLimit[4];

float fJump_E;	//ジャンプ力
float fGRAVITY_E_E;	//重力

int g_nCntSound_E = 0;
int g_nCntEnemy ;

bool bLand_E; //着地判定
//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	//スモーク初期化
	InitSmoke();

	EnemyText *pEnemyText;
	pEnemyText = GetEnemyText();
	g_nCntEnemy = 3;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCntIdx = 0; nCntIdx < pEnemyText->MAX_TEXTPARTS; nCntIdx++)
		{//各種値の代入
		 //読み込むモデル名
			g_Enemy[nCntEnemy].aModel[nCntIdx].FileName[0] = pEnemyText->aModelTex[nCntIdx].FileName[0];

			//親子関係
			g_Enemy[nCntEnemy].aModel[nCntIdx].nIdxModelParent = pEnemyText->aModelTex[nCntIdx].Index;

			//位置
			g_Enemy[nCntEnemy].aModel[nCntIdx].pos = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].ModelPosX, pEnemyText->aModelTex[nCntIdx].ModelPosY, pEnemyText->aModelTex[nCntIdx].ModelPosZ);

			//回転
			g_Enemy[nCntEnemy].aModel[nCntIdx].rot = D3DXVECTOR3(pEnemyText->aModelTex[nCntIdx].ModelRotX,
				pEnemyText->aModelTex[nCntIdx].ModelRotY,
				pEnemyText->aModelTex[nCntIdx].ModelRotZ);

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
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.x = Vtx.x + -20.0f;
			}

			if (Vtx.y < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.y = Vtx.y + -50.0f;
			}

			if (Vtx.z < g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMinModel.z = Vtx.x + -20.0f;
			}


			if (Vtx.x > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.x = Vtx.x + 20.0f;
			}

			if (Vtx.y > g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y)
			{
				g_Enemy[nCntEnemy].aModel[0].VtxMaxModel.y = Vtx.y + 50.0f;
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

	//プレイヤーのライフ
	//g_Enemy[0].nLife = 5;
	//g_Enemy[1].nLife = 5;
	//g_Enemy[2].nLife = 3;
	////g_Enemy[3].nLife = 3;

	//// 位置・向きの初期設定
	//g_Enemy[0].pos = D3DXVECTOR3(750.0f, 0.0f, -1400.0f);
	//g_Enemy[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//g_Enemy[1].pos = D3DXVECTOR3(750.0f, 0.0f, -500.0f);
	//g_Enemy[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//g_Enemy[2].pos = D3DXVECTOR3(750.0f, 0.0f, -2600.0f);
	//g_Enemy[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	/*g_Enemy[3].pos = D3DXVECTOR3(1150.0f, 0.0f, -2500.0f);
	g_Enemy[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	//スモーク終了
	UninitSmoke();

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
#if 1
	EnemyMotion *pMotion;
	pMotion = GetEnemyMotion();

	EnemyText *pEnemyText;
	pEnemyText = GetEnemyText();




	//nCntUpdateRogic++;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_Enemy[nCntEnemy].posold = g_Enemy[nCntEnemy].pos;
		SetPositionShadow(g_Enemy[nCntEnemy].nIndexShadow, g_Enemy[nCntEnemy].pos);//影の位置

		CAMERA pCamera;
		pCamera = *GetCamera();
		PLAYER *pPlayer;
		//プレイヤーの取得
		pPlayer = GetPlayer();
		D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
		BulletMove.x = -25.0f;
		BulletMove.z = -25.0f;

		D3DXVECTOR3 BulletMove2(0.0f, 0.0f, 0.0f);
		BulletMove2.x = -7.0f;
		BulletMove2.z = -7.0f;

		D3DXVECTOR3 VecP;
		D3DXVECTOR3 VecLength;

		g_Enemy[nCntEnemy].Rmove = pPlayer->move *-1;

		VecP.x = pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x;
		VecP.y = pPlayer->pos.y - g_Enemy[nCntEnemy].pos.y + 30.0f;
		VecP.z = pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z;
		D3DXVec3Normalize(&VecP, &VecP);

		VecLength = pPlayer->pos - g_Enemy[nCntEnemy].pos;



	/*	if (g_EnemyStatusNow == ENEMYSTATUS_NEUTRAL)
		{

			g_EnemyStatus = ENEMYSTATUS_MOVE;
		}*/


		if (VecP.y < 0.0)
		{
			BulletMove.y = VecP.y;
		}
		else
		{
			BulletMove.y = VecP.y * MOVE_BULLET_Y;

		}



		if (g_Enemy[nCntEnemy].bUse == true)
		{
			if (g_Enemy[nCntEnemy].nLife <= 1)
			{
				for (int nCnt = 0; nCnt < 50; nCnt++)
				{
					SetSmoke(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 27.0f, g_Enemy[nCntEnemy].pos.z), 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				if (rand() % 20 == 0)
				{
					SetParticle(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 27.0f, g_Enemy[nCntEnemy].pos.z), 30, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
				}
			}

			static float fAngle;
			fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z);



			g_Enemy[nCntEnemy].fLength = sqrtf((VecLength.x / 2 + VecLength.x / 2) * (VecLength.z / 2 + VecLength.z / 2));

			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}

			if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}


			g_Enemy[nCntEnemy].rot.y = fAngle;

			if (rand() % 90 == 0)
			{
				g_Enemy[nCntEnemy].bUseQick = true;
				if (g_Enemy[nCntEnemy].bUseQick == true)
				{
					g_Enemy[nCntEnemy].move.x -= sinf((D3DX_PI *  fAngle)) * DASHSPEED;
					g_Enemy[nCntEnemy].move.z -= cosf((D3DX_PI *  fAngle)) * DASHSPEED;
					g_Enemy[nCntEnemy].bUseQick = false;
				}
			}
			if (rand() % 50 == 0)
			{
				if (g_Enemy[nCntEnemy].fLength <= 200.0f)
				{

					g_Enemy[nCntEnemy].move.x = g_Enemy[nCntEnemy].Rmove.x;
					g_Enemy[nCntEnemy].move.y = g_Enemy[nCntEnemy].move.y;
					g_Enemy[nCntEnemy].move.y = g_Enemy[nCntEnemy].Rmove.z;

				}
				else
				{
					g_Enemy[nCntEnemy].move.x = fAngle * 1.5f;
					g_Enemy[nCntEnemy].move.z = fAngle * 0.1f;
				}
			}

			if (rand() % 100 == 0)
			{//ジャンプ
				if (g_Enemy[nCntEnemy].bUseJump == false)
				{
					g_Enemy[nCntEnemy].bUseJump = true;
					if (g_Enemy[nCntEnemy].bUseJump == true)
					{
						g_Enemy[nCntEnemy].move.y += 10.0f;
					}
				}
			}
			if (g_Enemy[nCntEnemy].fLength <= 480.0f)
			{
				if (rand() % 30 == 0)
				{
					SetBullet(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 18.0f, g_Enemy[nCntEnemy].pos.z), BulletMove, D3DXVECTOR3(g_Enemy[nCntEnemy].rot.x, fAngle, g_Enemy[nCntEnemy].rot.z), 40, 5.0f, BULLETTYPE_ENEMY);
				}
			}
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
				g_Enemy[nCntEnemy].bUseJump = false;

			}
			else
			{
				g_Enemy[nCntEnemy].bUseJump = true;

			}

			if (g_Enemy[nCntEnemy].pos.x > STAGE_LIMIT_MAX_X)
			{
				g_Enemy[nCntEnemy].pos.x = STAGE_LIMIT_MAX_X;
			}
			if (g_Enemy[nCntEnemy].pos.x < STAGE_LIMIT_MIN_X)
			{
				g_Enemy[nCntEnemy].pos.x = STAGE_LIMIT_MIN_X;
			}
			if (g_Enemy[nCntEnemy].pos.z > STAGE_LIMIT_MAX_Z)
			{
				g_Enemy[nCntEnemy].pos.z = STAGE_LIMIT_MAX_Z;
			}

			if (g_Enemy[nCntEnemy].pos.z < STAGE_LIMIT_MIN_Z)
			{
				g_Enemy[nCntEnemy].pos.z = STAGE_LIMIT_MIN_Z;
			}

			if (CollisionModel(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].VtxMaxEnemy, &g_Enemy[nCntEnemy].VtxMaxEnemy) == true)
			{
				g_Enemy[nCntEnemy].bUseJump = false;
				g_Enemy[nCntEnemy].move.x *= -1;
				g_Enemy[nCntEnemy].move.z *= -1;

			}
		}



		CollisionModelWall(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].VtxMaxEnemy, &g_Enemy[nCntEnemy].VtxMinEnemy);
		CollisionGate(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posold, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].VtxMaxEnemy, &g_Enemy[nCntEnemy].VtxMinEnemy);

	}

#endif
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


		for (int nCntModelParts = 0; nCntModelParts < MAX_PARTS; nCntModelParts++)
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
	}
}
	//=============================================================================
	// ヒット処理
	//=============================================================================
	void HitEnemy(int nDamage, int nCntEnemy)
	{
		GAMESTATE *pGame;
		pGame = GetGameState();

			if (g_Enemy[nCntEnemy].bUse == true)
			{
				g_Enemy[nCntEnemy].nLife += nDamage;

				if (g_Enemy[nCntEnemy].nLife <= 0)
				{
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

					g_Enemy[nCntEnemy].bUse = false;
					AddScore(25000);
					DeleteShadow(g_Enemy[nCntEnemy].nIndexShadow);
					for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
					{
						SetParticle(g_Enemy[nCntEnemy].pos, 60, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
						SetParticle(g_Enemy[nCntEnemy].pos, 60, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);

					}

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
	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_Enemy[nCntEnemy].bUse == false)
			{
				g_Enemy[nCntEnemy].pos = pos;
				g_Enemy[nCntEnemy].rot = rot;
				g_Enemy[nCntEnemy].nLife = nLife;
				g_Enemy[nCntEnemy].bUse = true;

				g_Enemy[nCntEnemy].nIndexShadow = SetShadow(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].rot, 28.0f, 28.0f);

				break;
			}
		}
	}

ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}

ENEMYSTATUS *GetEnemyStatus(void)
{
	return &g_Enemy[0].status;
}


