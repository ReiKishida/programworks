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
#include "sound.h"
#include "enemy_axe.h"
#include "enemy_axe_text.h"
#include "enemy_axe_motion.h"
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

ENEMYAXE g_EnemyAxe[MAX_ENEMYAXE];
ENEMYAXESTATUS g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
ENEMYAXESTATUS g_EnemyAxeStatusNow = ENEMYAXESTATUS_NEUTRAL;

//STAGE g_aLimit[4];
//テキスト情報取得

float fJump_Axe;	//ジャンプ力
float fGRAVITY_Axe;	//重力

int g_nCntSound_Axe = 0;
int g_nCntEnemyAxeAxe;

bool bLand_Axe; //着地判定
//=============================================================================
// 初期化処理
 //=============================================================================
void InitEnemyAxe(void)
{


	EnemyAxeTex *pEnemyAxeText;
	pEnemyAxeText = GetEnemyAxeTex();
	g_nCntEnemyAxeAxe = 3;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEnemyAxe = 0; nCntEnemyAxe < MAX_ENEMYAXE; nCntEnemyAxe++)
	{
		for (int nCntIdx = 0; nCntIdx < pEnemyAxeText->MAX_PARTS; nCntIdx++)
		{//各種値の代入
		 //読み込むモデル名
			g_EnemyAxe[nCntEnemyAxe].aModel[nCntIdx].FileName[0] = pEnemyAxeText->aModelTex[nCntIdx].FileName[0];

			//親子関係
			g_EnemyAxe[nCntEnemyAxe].aModel[nCntIdx].nIdxModelParent = pEnemyAxeText->aModelTex[nCntIdx].Index;

			//位置
			g_EnemyAxe[nCntEnemyAxe].aModel[nCntIdx].pos = D3DXVECTOR3(pEnemyAxeText->aModelTex[nCntIdx].PosX, pEnemyAxeText->aModelTex[nCntIdx].PosY, pEnemyAxeText->aModelTex[nCntIdx].PosZ);

			//回転
			g_EnemyAxe[nCntEnemyAxe].aModel[nCntIdx].rot = D3DXVECTOR3(pEnemyAxeText->aModelTex[nCntIdx].RotX,
				pEnemyAxeText->aModelTex[nCntIdx].RotY,
				pEnemyAxeText->aModelTex[nCntIdx].RotZ);

			//モデルの設定
			D3DXLoadMeshFromX(&pEnemyAxeText->aModelTex[nCntIdx].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_EnemyAxe[nCntEnemyAxe].aModel[nCntIdx].pBuffMat,
				NULL,
				&g_EnemyAxe[nCntEnemyAxe].aModel[nCntIdx].nNumMat,
				&g_EnemyAxe[nCntEnemyAxe].aModel[nCntIdx].pMesh);
		}
		g_EnemyAxe[nCntEnemyAxe].Collision.bUse = false;
		g_EnemyAxe[nCntEnemyAxe].Collision.nCntMotion = 0;
		g_EnemyAxe[nCntEnemyAxe].Collision.StartFlame = 5;
		g_EnemyAxe[nCntEnemyAxe].Collision.fRadius = 40.0f;
		g_EnemyAxe[nCntEnemyAxe].Collision.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		g_EnemyAxe[nCntEnemyAxe].Collision.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyAxe[nCntEnemyAxe].Collision.nIdxParent = 0;
		g_EnemyAxe[nCntEnemyAxe].nCntCoolDown = 120;

		SetSencerAtk(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43 - 30.0f));
		SetSencerAtk(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43 - 20.0f));
		SetSencerAtk(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43 - 10.0f));
		SetSencerAtk(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43));


		//ジャンプの状態
		g_EnemyAxe[nCntEnemyAxe].bUseJump = false;
		//プレイヤーの状態
		g_EnemyAxe[nCntEnemyAxe].nCoolDown = 0;

		//着地しているか
		bLand_Axe = false;

		//プレイヤーが使用されているか
		g_EnemyAxe[nCntEnemyAxe].bUse = false;

	}









	fJump_Axe = JUMP_POWER;
	fGRAVITY_Axe = GRAVITY_E;


	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ
	for (int nCntEnemyAxe = 0; nCntEnemyAxe < MAX_ENEMYAXE; nCntEnemyAxe++)
	{
		//頂点数を取得
		nNumVtx = g_EnemyAxe[nCntEnemyAxe].aModel[0].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_EnemyAxe[nCntEnemyAxe].aModel[0].pMesh->GetFVF());

		//頂点バッファをロック
		g_EnemyAxe[nCntEnemyAxe].aModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.x)
			{
				g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.x = Vtx.x;
			}

			if (Vtx.y < g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.y)
			{
				g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.y = Vtx.y;
			}

			if (Vtx.z < g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.z)
			{
				g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.z = Vtx.x;
			}


			if (Vtx.x > g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.x)
			{
				g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.x = Vtx.x + 20.0f;
			}

			if (Vtx.y > g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.y)
			{
				g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.y = Vtx.y + 30.0f;
			}

			if (Vtx.z > g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.z)
			{
				g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.z = Vtx.z + 20.0f;
			}
			pVtxBuff += sizeFVF;
		}

		g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.x = g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.x;
		g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.x = g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.x;

		g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.y = g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.y;
		g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.y = g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.y;

		g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.z = g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMinModel.z;
		g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.z = g_EnemyAxe[nCntEnemyAxe].aModel[0].VtxMaxModel.z;

		g_EnemyAxe[nCntEnemyAxe].fAngle = 0.0f;			//差分

	}

	CAMERA *pCamera;
	pCamera = GetCamera();


}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyAxe(void)
{


	for (int nCntEnemyAxe = 0; nCntEnemyAxe < MAX_ENEMYAXE; nCntEnemyAxe++)
	{
		// メッシュの開放
		if (g_EnemyAxe[nCntEnemyAxe].aModel[0].pMesh != NULL)
		{
			g_EnemyAxe[nCntEnemyAxe].aModel[0].pMesh->Release();
			g_EnemyAxe[nCntEnemyAxe].aModel[0].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_EnemyAxe[nCntEnemyAxe].aModel[0].pBuffMat != NULL)
		{
			g_EnemyAxe[nCntEnemyAxe].aModel[0].pBuffMat->Release();
			g_EnemyAxe[nCntEnemyAxe].aModel[0].pBuffMat = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemyAxe(void)
{

	EnemyAxeMotion *pAxe_Motion = GetEnemyAxeMotion();					//モーションデータの所得
	EnemyAxeTex *pAxe_Text = GetEnemyAxeTex();							//ファイルデータの取得

	EnemyAxeTex *pEnemyAxeText;
	pEnemyAxeText = GetEnemyAxeTex();
	STAGEMODE g_Stage = GetStageMode();				//現在のステージ番号を取得


#if 1




													//nCntUpdateRogic++;
	for (int nCntEnemyAxe = 0; nCntEnemyAxe < MAX_ENEMYAXE; nCntEnemyAxe++)
	{




		g_EnemyAxe[nCntEnemyAxe].nCntCoolDown++;
		g_EnemyAxe[nCntEnemyAxe].posold = g_EnemyAxe[nCntEnemyAxe].pos;

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


		VecP.x = (pPlayer->pos.x - g_EnemyAxe[nCntEnemyAxe].pos.x);
		VecP.y = pPlayer->pos.y - g_EnemyAxe[nCntEnemyAxe].pos.y;
		VecP.z = (pPlayer->pos.z - g_EnemyAxe[nCntEnemyAxe].pos.z) + 45.0f;
		D3DXVec3Normalize(&VecP, &VecP);

		VecLength = pPlayer->pos - g_EnemyAxe[nCntEnemyAxe].pos;









		if (g_EnemyAxe[nCntEnemyAxe].bUse == true)
		{
			switch (g_EnemyAxe[nCntEnemyAxe].state2)
			{
			case ENEMYAXESTATE2_NORMAL:
				break;

			case ENEMYAXESTATE2_DAMAGE:
				g_EnemyAxe[nCntEnemyAxe].nCntCoolDown--;
				if (g_EnemyAxe[nCntEnemyAxe].nCntCoolDown <= 0)
				{
					g_EnemyAxe[nCntEnemyAxe].state2 = ENEMYAXESTATE2_NORMAL;
				}
			}



			static float fAngle;
			//fAngle = atan2f(pPlayer->pos.x - g_EnemyAxe[nCntEnemyAxe].pos.x, pPlayer->pos.z - g_EnemyAxe[nCntEnemyAxe].pos.z);
			fAngle = atan2f(g_EnemyAxe[nCntEnemyAxe].pos.x - pPlayer->pos.x, g_EnemyAxe[nCntEnemyAxe].pos.z - pPlayer->pos.z);

			g_EnemyAxe[nCntEnemyAxe].fLength = sqrtf((pPlayer->pos.x - g_EnemyAxe[nCntEnemyAxe].pos.x) * (pPlayer->pos.x - g_EnemyAxe[nCntEnemyAxe].pos.x) + (pPlayer->pos.z - g_EnemyAxe[nCntEnemyAxe].pos.z) *(pPlayer->pos.z - g_EnemyAxe[nCntEnemyAxe].pos.z));//距離


			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}

			if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}




			switch (g_EnemyAxe[nCntEnemyAxe].state)
			{
			case ENEMYAXESTATE_NONE:

				if (CollisionSencer_Enemy(&pPlayer->pos, g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnemy) == true)
				{
					g_EnemyAxe[nCntEnemyAxe].state = ENEMYAXESTATE_DISCOVERY;
				}
				else
				{
					g_EnemyAxe[nCntEnemyAxe].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}

				break;

			case ENEMYAXESTATE_BACKHOME:

				g_EnemyAxe[nCntEnemyAxe].DestHome = atan2f(g_EnemyAxe[nCntEnemyAxe].HomePoint.x - g_EnemyAxe[nCntEnemyAxe].pos.x,
					g_EnemyAxe[nCntEnemyAxe].HomePoint.z - g_EnemyAxe[nCntEnemyAxe].pos.z);

				g_EnemyAxe[nCntEnemyAxe].move.x = g_EnemyAxe[nCntEnemyAxe].DestHome * 1.2f;
				g_EnemyAxe[nCntEnemyAxe].move.z = g_EnemyAxe[nCntEnemyAxe].DestHome * 1.2f;

				if (g_EnemyAxe[nCntEnemyAxe].pos.x <= g_EnemyAxe[nCntEnemyAxe].HomePoint.x + 50.0f || g_EnemyAxe[nCntEnemyAxe].pos.x >= g_EnemyAxe[nCntEnemyAxe].HomePoint.x - 50.0f
					|| g_EnemyAxe[nCntEnemyAxe].pos.z <= g_EnemyAxe[nCntEnemyAxe].HomePoint.z + 50.0f || g_EnemyAxe[nCntEnemyAxe].pos.z >= g_EnemyAxe[nCntEnemyAxe].HomePoint.z - 50.0f)
				{
					g_EnemyAxe[nCntEnemyAxe].state = ENEMYAXESTATE_NONE;
				}

				break;
			case ENEMYAXESTATE_DISCOVERY:
				g_EnemyAxe[nCntEnemyAxe].move.x = VecP.x * 1.4f;
				g_EnemyAxe[nCntEnemyAxe].move.z = VecP.z * 1.4f;
				fAngle = atan2f(g_EnemyAxe[nCntEnemyAxe].pos.x - pPlayer->pos.x, g_EnemyAxe[nCntEnemyAxe].pos.z - pPlayer->pos.z);
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}

				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				if (CollisionSencer_Enemy(&pPlayer->pos, g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnemy) == false)
				{
					g_EnemyAxe[nCntEnemyAxe].state = ENEMYAXESTATE_BACKHOME;
				}
				if (g_EnemyAxeStatusNow == ENEMYAXESTATUS_NEUTRAL)
				{//ムーヴへの移行
					SetEnemyAxeStatus(ENEMYAXESTATUS_MOVE, nCntEnemyAxe);
				}
				if (CollisionSencer_Enable(&pPlayer->pos) == true)
				{
					if (g_EnemyAxeStatusNow == ENEMYAXESTATUS_NEUTRAL || g_EnemyAxeStatusNow == ENEMYAXESTATUS_MOVE)
					{//ムーヴへの移行
						pAxe_Motion->nCntFlame = 0;
						if (g_EnemyAxe[nCntEnemyAxe].nCntCoolDown % 30 == 0)
						{
							SetEnemyAxeStatus(ENEMYAXESTATUS_ATTACK, nCntEnemyAxe);
						}
					}

					else if (g_EnemyAxeStatusNow == ENEMYAXESTATUS_ATTACK)
					{
						if (pAxe_Motion->nCntFlame % g_EnemyAxe[nCntEnemyAxe].Collision.StartFlame == 0)
						{
							PlaySound(SOUND_LABEL_SE_SLASH);
						}
						if (pAxe_Motion->nCntFlame > g_EnemyAxe[nCntEnemyAxe].Collision.StartFlame)
						{
							CollisionAttackEnemy(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43 - 30.0f)
								, g_EnemyAxe[nCntEnemyAxe].Collision.fRadius
								, g_EnemyAxe[nCntEnemyAxe].nIndexSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43 - 20.0f)
								, g_EnemyAxe[nCntEnemyAxe].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43 - 10.0f)
								, g_EnemyAxe[nCntEnemyAxe].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
							CollisionAttackEnemy(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._41, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._42, g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision._43)
								, g_EnemyAxe[nCntEnemyAxe].Collision.fRadius
								, pPlayer->nIdxSencer
								, SENCERCOLTYPE_ENEMY);
						}
					}
					g_EnemyAxe[nCntEnemyAxe].rot.y = fAngle;
				}
				break;
			}//switch文

			g_EnemyAxe[nCntEnemyAxe].rot.y = fAngle;

#if 1

#endif			

			g_EnemyAxe[nCntEnemyAxe].pos += g_EnemyAxe[nCntEnemyAxe].move;

			g_EnemyAxe[nCntEnemyAxe].move.y -= GRAVITY_E;
			if (g_EnemyAxe[nCntEnemyAxe].move.y > GRAVITY_E_MAX)
			{
				g_EnemyAxe[nCntEnemyAxe].move.y = GRAVITY_E_MAX;
			}
			if (g_EnemyAxe[nCntEnemyAxe].pos.y <= 0.0)
			{

				g_EnemyAxe[nCntEnemyAxe].pos.y = 0.0f;
				g_EnemyAxe[nCntEnemyAxe].move.y = 0.0f;

			}



#endif
			SetPositionSencerCollision(g_EnemyAxe[nCntEnemyAxe].nIndexSencer, g_EnemyAxe[nCntEnemyAxe].pos);//センサーの位置
			SetPositionShadow(g_EnemyAxe[nCntEnemyAxe].nIndexShadow, g_EnemyAxe[nCntEnemyAxe].pos);
			SetPositionSencerEnemy(g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnemy,
				D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].pos.x, g_EnemyAxe[nCntEnemyAxe].pos.y, g_EnemyAxe[nCntEnemyAxe].pos.z));
			SetPositionSencerEnableAttack(g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnableAttack, g_EnemyAxe[nCntEnemyAxe].pos);
			CollisionSencer_Lock(&g_EnemyAxe[nCntEnemyAxe].pos);
			//g_EnemyAxe[nCntEnemyAxe].rot.y += 0.01f;




			switch (g_EnemyAxeStatus)
			{
			case ENEMYAXESTATUS_NEUTRAL:
				if (pAxe_Motion->KeyFlame == pAxe_Text->KEYFLAME[pAxe_Motion->nNumEnemyAxeMotion] - 1)
				{
					if (g_EnemyAxeStatusNow == ENEMYAXESTATUS_NEUTRAL)
					{

						SetEnemyAxeMotion(0);
					}
					else
					{
						pAxe_Motion->KeyFlame = 0;
						SetEnemyAxeMotion(0);
						g_EnemyAxeStatusNow = ENEMYAXESTATUS_NEUTRAL;
						g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
					}
				}
				break;
			case ENEMYAXESTATUS_MOVE:
				g_EnemyAxeStatusNow = ENEMYAXESTATUS_MOVE;
				g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
				SetEnemyAxeMotion(1);
				break;
			case ENEMYAXESTATUS_ATTACK:
				g_EnemyAxeStatusNow = ENEMYAXESTATUS_ATTACK;
				g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
				SetEnemyAxeMotion(2);
				break;
			case ENEMYAXESTATUS_JUMP:
				g_EnemyAxeStatusNow = ENEMYAXESTATUS_JUMP;
				g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
				SetEnemyAxeMotion(3);
				break;
			case ENEMYAXESTATUS_LANDING:
				g_EnemyAxeStatusNow = ENEMYAXESTATUS_LANDING;
				g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
				SetEnemyAxeMotion(4);
				break;
			case ENEMYAXESTATUS_GUARD:
				g_EnemyAxeStatusNow = ENEMYAXESTATUS_GUARD;
				g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
				SetEnemyAxeMotion(5);
				break;
			case ENEMYAXESTATUS_DAMAGE1:
				g_EnemyAxeStatusNow = ENEMYAXESTATUS_DAMAGE1;
				g_EnemyAxeStatus = ENEMYAXESTATUS_NEUTRAL;
				SetEnemyAxeMotion(6);
				break;

			}//switch
			 //if

			if (g_Stage == STAGEMODE_STAGE1)
			{//現在のステージがステージ1
			 //奥壁のあたり判定
				CollisionWall1_WIDTHBack_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//手前壁のあたり判定
				CollisionWall1_WIDTHThisSide_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//右壁のあたり判定
				CollisionWall1_RIGHT_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//右壁のあたり判定
				CollisionWall1_LEFT_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);
			}
			else if (g_Stage == STAGEMODE_STAGE2)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWall2_WIDTHBack_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//手前壁のあたり判定
				CollisionWall2_WIDTHThisSide_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//右壁のあたり判定
				CollisionWall2_RIGHT_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//右壁のあたり判定
				CollisionWall2_LEFT_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);
			}
			else if (g_Stage == STAGEMODE_STAGE3)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWallBoss_WIDTHBack_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//手前壁のあたり判定
				CollisionWallBoss_WIDTHThisSide_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//右壁のあたり判定
				CollisionWallBoss_RIGHT_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);

				//右壁のあたり判定
				CollisionWallBoss_LEFT_Ene(&g_EnemyAxe[nCntEnemyAxe].pos, &g_EnemyAxe[nCntEnemyAxe].posold);
			}
		}//buse

	}//for
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyAxe(void)
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
	for (int nCntEnemyAxe = 0; nCntEnemyAxe < MAX_ENEMYAXE; nCntEnemyAxe++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_EnemyAxe[nCntEnemyAxe].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_EnemyAxe[nCntEnemyAxe].rot.y, g_EnemyAxe[nCntEnemyAxe].rot.x, g_EnemyAxe[nCntEnemyAxe].rot.z);

		D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].mtxWorld, &g_EnemyAxe[nCntEnemyAxe].mtxWorld, &mtxrot);


		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans,
			g_EnemyAxe[nCntEnemyAxe].pos.x, g_EnemyAxe[nCntEnemyAxe].pos.y, g_EnemyAxe[nCntEnemyAxe].pos.z);
		D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].mtxWorld, &g_EnemyAxe[nCntEnemyAxe].mtxWorld, &mtxtrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_EnemyAxe[nCntEnemyAxe].mtxWorld);


		for (int nCntModelParts = 0; nCntModelParts < MAX_AXE_PARTS; nCntModelParts++)
		{
			if (g_EnemyAxe[nCntEnemyAxe].bUse == true)
			{

				if (g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].nIdxModelParent == -1)
				{//プレイヤーのマトリックスに
					mtxParent = g_EnemyAxe[nCntEnemyAxe].mtxWorld;
				}
				else if (g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].nIdxModelParent == 0)
				{//体のマトリックスに
					mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[0].mtxWorld;
				}
				else if (g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].nIdxModelParent == 1)
				{//軸１に
					mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[1].mtxWorld;
				}
				else if (g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].nIdxModelParent == 2)
				{//軸２に
					mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[2].mtxWorld;
				}
				else if (g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].nIdxModelParent == 5)
				{//軸２に
					mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[5].mtxWorld;
				}
				else if (g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].nIdxModelParent == 8)
				{//軸２に
					mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[8].mtxWorld;
				}
				else
				{//その他
					mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts - 1].mtxWorld;
				}
				//=======================================================================//
				//						    親モデルのインデックス						//
				//=====================================================================//
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].rot.y, g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].rot.x, g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].rot.z);
				D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld, &g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld, &mtxrot);


				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].pos.x, g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].pos.y, g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].pos.z);
				D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld, &g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld, &mtxtrans);

				//親のマトリックス反映
				D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld, &g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld, &mtxParent);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].mtxWorld);


				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].nNumMat; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//pDevice->SetTexture(0, g_EnemyAxe[nCntEnemyAxe].aModel[0].pEnemyAxeTexture);
					pDevice->SetTexture(0, 0);
					// モデル(パーツ)の描画
					g_EnemyAxe[nCntEnemyAxe].aModel[nCntModelParts].pMesh->DrawSubset(nCntMat);
				}
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}

		/////////////////
		//当たり判定用//
		///////////////
		if (g_EnemyAxe[nCntEnemyAxe].Collision.nIdxParent == -1)
		{// 体の時、プレイヤーのマトリックスに入れる
			mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[16].mtxWorld;
		}
		if (g_EnemyAxe[nCntEnemyAxe].Collision.nIdxParent == 0)
		{// 体の時、プレイヤーのマトリックスに入れる
			mtxParent = g_EnemyAxe[nCntEnemyAxe].aModel[15].mtxWorld;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot, g_EnemyAxe[nCntEnemyAxe].Collision.rot.y, g_EnemyAxe[nCntEnemyAxe].Collision.rot.x, g_EnemyAxe[nCntEnemyAxe].Collision.rot.z);
		D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision, &g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision, &mtxrot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans, g_EnemyAxe[nCntEnemyAxe].Collision.pos.x, g_EnemyAxe[nCntEnemyAxe].Collision.pos.y, g_EnemyAxe[nCntEnemyAxe].Collision.pos.z);
		D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision, &g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision, &mtxtrans);

		//親のマトリックス反映
		D3DXMatrixMultiply(&g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision, &g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision, &mtxParent);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_EnemyAxe[nCntEnemyAxe].Collision.mtxWorldCollision);
	}
}
//=============================================================================
// ヒット処理
//=============================================================================
void HitEnemyAxe(int nDamage, int nCntEnemyAxe)
{
	D3DXVECTOR3 VecP;

	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	if (g_EnemyAxe[nCntEnemyAxe].bUse == true)
	{
		g_EnemyAxe[nCntEnemyAxe].nLife += nDamage;

		VecP.x = g_EnemyAxe[nCntEnemyAxe].pos.x - pPlayer->pos.x;
		VecP.y = g_EnemyAxe[nCntEnemyAxe].pos.y - pPlayer->pos.y;
		VecP.z = g_EnemyAxe[nCntEnemyAxe].pos.z - pPlayer->pos.z * -1;


		D3DXVec3Normalize(&VecP, &VecP);
		//g_EnemyAxe[nCntEnemyAxe].pos += VecP * 80.0f;

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			SetParticle(D3DXVECTOR3(g_EnemyAxe[nCntEnemyAxe].pos.x, g_EnemyAxe[nCntEnemyAxe].pos.y + 30.0f, g_EnemyAxe[nCntEnemyAxe].pos.z), 60, D3DXCOLOR(1.0f, 0.0f, 0.8f, 1.0f), PARTICLETYPE_EXPLOSION);
		}
		if (g_EnemyAxe[nCntEnemyAxe].nLife <= 0)
		{

			g_EnemyAxe[nCntEnemyAxe].bUse = false;
			DeletShadow(g_EnemyAxe[nCntEnemyAxe].nIndexShadow);
			DeleteSencerCol(g_EnemyAxe[nCntEnemyAxe].nIndexSencer);
			DeleteSencerEnemy(g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnemy);
			DeleteSencerEnableAttack(g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnableAttack);
			DecreaseBrazen(-2);

		}

	}
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CollisionEnemyAxe(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosold, D3DXVECTOR3 * VtxMax, D3DXVECTOR3 * VtxMin)
{
	bool bCollision = false; //着地したか

	for (int nCntEnemyAxe = 0; nCntEnemyAxe < MAX_ENEMYAXE; nCntEnemyAxe++)
	{
		if (g_EnemyAxe[nCntEnemyAxe].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x && pPos->x - VtxMax->z <= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x
				&&pPos->y + VtxMax->y >= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y && pPos->y - VtxMax->y <= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y
				&&pPos->z + VtxMax->z >= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z && pPos->z - VtxMax->z <= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z)
			{
				if (pPosold->x + VtxMax->z <= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x  && pPos->x + VtxMax->z >= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x)
				{//左からきたとき
					pPos->x = g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x - VtxMax->z;
					bCollision = true;
				}

				else if (pPosold->x - VtxMax->z >= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x  && pPos->x - VtxMax->z <= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x)
				{//右からきたとき
					pPos->x = g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.x + g_EnemyAxe[nCntEnemyAxe].pos.x + VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z + VtxMax->z <= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z && pPos->z + VtxMax->z >= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z)
				{//手前から来た時
					pPos->z = g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z - VtxMax->z;
					bCollision = true;

				}
				else if (pPosold->z - VtxMax->z >= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z && pPos->z - VtxMax->z <= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z)
				{//奥から来た時
					pPos->z = g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.z + g_EnemyAxe[nCntEnemyAxe].pos.z + VtxMax->z;
					bCollision = true;

				}

				if (pPosold->y + VtxMax->y <= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y && pPos->y + VtxMax->y >= g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y)
				{//上から来た時
					pPos->y = g_EnemyAxe[nCntEnemyAxe].VtxMinEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y + VtxMin->y;
					bCollision = true;

				}
				if (pPosold->y - VtxMax->y >= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y && pPos->y - VtxMax->y <= g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y)
				{//下から来た時
					pPos->y = g_EnemyAxe[nCntEnemyAxe].VtxMaxEnemyAxe.y + g_EnemyAxe[nCntEnemyAxe].pos.y + VtxMax->y;
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
int SetEnemyAxe(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	int nCntEnemyAxe;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (nCntEnemyAxe = 0; nCntEnemyAxe < MAX_ENEMYAXE; nCntEnemyAxe++)
	{
		if (g_EnemyAxe[nCntEnemyAxe].bUse == false)
		{
			g_EnemyAxe[nCntEnemyAxe].pos = pos;
			g_EnemyAxe[nCntEnemyAxe].HomePoint = pos;
			g_EnemyAxe[nCntEnemyAxe].rot = rot;
			g_EnemyAxe[nCntEnemyAxe].nLife = nLife;
			g_EnemyAxe[nCntEnemyAxe].nIndexSencer = SetSencerCollision(g_EnemyAxe[nCntEnemyAxe].pos, g_EnemyAxe[nCntEnemyAxe].rot, SENCERCOLTYPE_ENEMY_AXE);
			g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnemy = SetSencerEnemy(g_EnemyAxe[nCntEnemyAxe].pos, g_EnemyAxe[nCntEnemyAxe].rot);
			g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnableAttack = SetSencerEnableAttack(g_EnemyAxe[nCntEnemyAxe].pos, g_EnemyAxe[nCntEnemyAxe].rot);
			g_EnemyAxe[nCntEnemyAxe].nIndexShadow = SetShadow(g_EnemyAxe[nCntEnemyAxe].pos, g_EnemyAxe[nCntEnemyAxe].rot);
			g_EnemyAxe[nCntEnemyAxe].state2 = ENEMYAXESTATE2_NORMAL;
			g_EnemyAxe[nCntEnemyAxe].bUse = true;
			break;
		}
	}
	return nCntEnemyAxe;
}

//------------------------------------------------------------------------
// エネミーの状態の設定												      |
//------------------------------------------------------------------------
void SetEnemyAxeStatus(ENEMYAXESTATUS status, int nCntEnemyAxe)
{
	g_EnemyAxeStatus = status;
}

////////////////////////////////////////////////////////////
//				敵の消去								  //
////////////////////////////////////////////////////////////
void DeleteEnemyAxe(int nCntEnemyAxe)
{
	g_EnemyAxe[nCntEnemyAxe].bUse = false;
	DeletShadow(g_EnemyAxe[nCntEnemyAxe].nIndexShadow);
	DeleteSencerCol(g_EnemyAxe[nCntEnemyAxe].nIndexSencer);
	DeleteSencerEnemy(g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnemy);
	DeleteSencerEnableAttack(g_EnemyAxe[nCntEnemyAxe].nIndexSencerEnableAttack);

}

////////////////////////////////////////////////////////////
//				敵の取得								  //
////////////////////////////////////////////////////////////
ENEMYAXE *GetEnemyAxe(void)
{
	return &g_EnemyAxe[0];
}
///////////////////////////////////////////////////////////
//				敵状態の取得							//
//////////////////////////////////////////////////////////
ENEMYAXESTATUS *GetEnemyAxeStatus(void)
{
	return &g_EnemyAxe[0].status;
}


