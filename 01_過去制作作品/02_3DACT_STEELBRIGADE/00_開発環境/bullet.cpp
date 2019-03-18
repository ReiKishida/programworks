//=============================================================================
//
// 弾発射処理 [Bullet.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "camera.h"
#include "model.h"
#include "effect.h"
#include "bullet.h"
#include "particle.h"
#include "enemy.h"
#include "score.h"
#include "player.h"
#include "life.h"
#include "gate.h"
#include "modelwall.h"
#include "canon.h"
#include "hit.h"
#include "damage.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//ステージの限界範囲
#define STAGE_LIMIT_MAX_X (1450.0f)
#define STAGE_LIMIT_MIN_X (-40.0f)
#define STAGE_LIMIT_MAX_Z (30.0f)
#define STAGE_LIMIT_MIN_Z (-3800.0f)
#define STAGE_LIMIT_MAX_Y (1000.0f)
#define STAGE_LIMIT_MIN_Y (0.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);	//頂点情報生成

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ

BULLET g_aBullet[MAX_BULLET];//弾情報

int g_nIndexBulletShadow[MAX_BULLET];	//影の番号


//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得


	MakeVertexBullet(pDevice);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
		g_aBullet[nCntBullet].bUse = false;//使用しているか
		g_aBullet[nCntBullet].type = BULLETTYPE_NONE;//弾の状態
		g_aBullet[nCntBullet].nLife = 0;//たまの状態
		g_aBullet[nCntBullet].fRadius = 0.0f;//弾の半径
	}



}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	//カメラの取得
	CAMERA *pCamera;
	pCamera = GetCamera();

	//敵の取得
	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	//モデルの取得
	MODEL *pModel;
	pModel = GetModel();

	//プレイヤーの取得
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	//ゲートの取得
	GATE *pGate;
	pGate = GetGate();

	//壁の取得
	WALL *pWall;
	pWall = GetModelWall();

	//キャノン砲の取得
	CANON *pCanon;
	pCanon = GetCanon();

	//エフェクトの位置
	D3DXVECTOR3 EffectPos;


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されていたら

			g_aBullet[nCntBullet].posold = g_aBullet[nCntBullet].pos;
			EffectPos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);


			//弾の寿命を減らしていく
			g_aBullet[nCntBullet].nLife--;


			//////////////////////////////////////////////////////////////////////
			/////////////プレイヤーの弾//////////////////////////////////////////
			////////////////////////////////////////////////////////////////////
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				SetEffect(EffectPos, 5.0f, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f));
				g_aBullet[nCntBullet].pos.x -= sinf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += 0.1f *  g_aBullet[nCntBullet].move.y;
				g_aBullet[nCntBullet].pos.z -= cosf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.z;
				SetPositionShadow(g_nIndexBulletShadow[nCntBullet], D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z));//影の位置

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されていたら
						if (pEnemy->pos.x + pEnemy->aModel[0].VtxMinModel.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pEnemy->pos.x + pEnemy->aModel[0].VtxMaxModel.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius

							&&pEnemy->pos.y - pEnemy->aModel[0].VtxMinModel.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pEnemy->pos.y + pEnemy->aModel[0].VtxMaxModel.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius

							&&pEnemy->pos.z + pEnemy->aModel[0].VtxMinModel.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pEnemy->pos.z + pEnemy->aModel[0].VtxMaxModel.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{

							//敵に当たった
							HitEnemy(-1, nCntEnemy);

							//ヒットコールの表示
							HitCall();

							//スコア加算
							AddScore(500);

							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//着弾点にパーティクル生成
								SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							//弾を削除し、影を消す
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);

						}
					}
				}


				for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++, pGate++)
				{
					if (pGate->bUse == true)
					{//ゲートが使用されているとき
						if (pGate->pos.x + pGate->VtxMinGate.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.x + pGate->VtxMaxGate.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y + pGate->VtxMinGate.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y + pGate->VtxMaxGate.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMinGate.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMaxGate.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//着弾点にパーティクル生成
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							//ゲートに当たった
							HitGate(-1, nCntGate);
							//ヒットコールを出す
							HitCall();

							//弾を削除し、影を消す
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}
				for (int nCntCanon = 0; nCntCanon < MAX_CANON; nCntCanon++, pCanon++)
				{
					if (pCanon->bUse == true)
					{
						if (pCanon->pos.x + pCanon->VtxMinCanon.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.x + pCanon->VtxMaxCanon.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.y + pCanon->VtxMinCanon.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.y + pCanon->VtxMaxCanon.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.z + pCanon->VtxMinCanon.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pCanon->pos.z + pCanon->VtxMaxCanon.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//着弾点にパーティクル生成
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							//キャノンにダメージ加算
							HitCanon(-1, nCntCanon);
							//スコア加算
							AddScore(300);
							//ヒットコール
							HitCall();

							//弾を削除し、影を消す
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}

			}



			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//敵の弾の場合
				SetPositionShadow(g_nIndexBulletShadow[nCntBullet], D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z));//影の位置
				SetEffect(EffectPos, 5.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));


				g_aBullet[nCntBullet].pos.x -= sinf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				g_aBullet[nCntBullet].pos.z -= cosf((D3DX_PI *  0.0f) + g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].move.z;

				if (pPlayer.pos.x + pPlayer.aModel[0].VtxMinModel.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
					&&pPlayer.pos.x + pPlayer.aModel[0].VtxMaxModel.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius

					&&pPlayer.pos.y - pPlayer.aModel[0].VtxMinModel.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
					&&pPlayer.pos.y + pPlayer.aModel[0].VtxMaxModel.y <= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius



					&&pPlayer.pos.z + pPlayer.aModel[0].VtxMinModel.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
					&&pPlayer.pos.z + pPlayer.aModel[0].VtxMaxModel.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
					)
				{
					//プレイヤーにダメージ加算
					HitPlayer(-5);
					//ゲージを減らす
					HitDamage(-10);
					//ダメージコール
					HitDamage();

					for (int nCntPerticle = 0; nCntPerticle < 12; nCntPerticle++)
					{//着弾点にパーティクル生成
						SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
					}

					//弾を削除し、影を消す
					g_aBullet[nCntBullet].bUse = false;
					DeleteShadow(g_nIndexBulletShadow[nCntBullet]);


				}


				for (int nCntWall = 0; nCntWall < MAX_GATE; nCntWall++, pGate++)
				{
					if (pWall->bUse == true)
					{//壁が使用されているとき
						if (pGate->pos.x + pGate->VtxMinGate.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.x + pGate->VtxMaxGate.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y - pGate->VtxMinGate.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.y + pGate->VtxMaxGate.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMinGate.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pGate->pos.z + pGate->VtxMaxGate.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//着弾点にパーティクル生成
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}

							//弾を削除し、影を消す
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}
			}



			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER || g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				if (g_aBullet[nCntBullet].pos.x > STAGE_LIMIT_MAX_X || g_aBullet[nCntBullet].pos.x < STAGE_LIMIT_MIN_X
					|| g_aBullet[nCntBullet].pos.y > STAGE_LIMIT_MAX_Y || g_aBullet[nCntBullet].pos.y < STAGE_LIMIT_MIN_Y
					|| g_aBullet[nCntBullet].pos.z > STAGE_LIMIT_MAX_Z || g_aBullet[nCntBullet].pos.z < STAGE_LIMIT_MIN_Z
					)
				{
					g_aBullet[nCntBullet].bUse = false;
					DeleteShadow(g_nIndexBulletShadow[nCntBullet]);


					for (int nCntPerticle = 0; nCntPerticle < 5; nCntPerticle++)
					{//着弾点にパーティクル生成
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.5f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);

					}
				}

				if (g_aBullet[nCntBullet].nLife <= 0)
				{//弾の寿命がなくなったとき
					//弾を削除し、影を消す
					g_aBullet[nCntBullet].bUse = false;
					DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
					for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
					{//着弾点にパーティクル生成
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
						SetParticle(g_aBullet[nCntBullet].pos, 90, D3DXCOLOR(1.0f, rand() % 1 + 0.5f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);

					}
				}



				for (int nCnModeltWall = 0; nCnModeltWall < MAX_MODELWALL; nCnModeltWall++, pWall++)
				{
					if (pWall->bUse == true)
					{//壁が使用されていたら
						if (pWall->pos.x + pWall->VtxMinModelWall.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pWall->pos.x + pWall->VtxMaxModelWall.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pWall->pos.y - pWall->VtxMinModelWall.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pWall->pos.y + pWall->VtxMaxModelWall.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pWall->pos.z + pWall->VtxMinModelWall.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pWall->pos.z + pWall->VtxMaxModelWall.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{//壁の範囲に入ったら
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//着弾点にパーティクル生成
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}


							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}

				for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
				{
					if (pModel->bUse == true)
					{//モデルが使用されていたら
						if (pModel->pos.x + pModel->VtxMinModel.x <= g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
							&&pModel->pos.x + pModel->VtxMaxModel.x >= g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
							&&pModel->pos.y - pModel->VtxMinModel.y <= g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fRadius
							&&pModel->pos.y + pModel->VtxMaxModel.y >= g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fRadius
							&&pModel->pos.z + pModel->VtxMinModel.z <= g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
							&&pModel->pos.z + pModel->VtxMaxModel.z >= g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
							)
						{//モデルの範囲内なら
							for (int nCntPerticle = 0; nCntPerticle < 20; nCntPerticle++)
							{//着弾点にパーティクル生成
								SetParticle(g_aBullet[nCntBullet].pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.15f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
							g_aBullet[nCntBullet].bUse = false;
							DeleteShadow(g_nIndexBulletShadow[nCntBullet]);
						}
					}
				}
			}//プレイヤーの弾もしくは敵の弾
		}//弾が使用されいる
	}//for文(弾全体)

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;

	//カメラの取得
	CAMERA *pCamera;
	pCamera = GetCamera();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);
			pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

			g_aBullet[nCntBullet].mtxWorld._11 = pCamera->mtxView._11;
			g_aBullet[nCntBullet].mtxWorld._12 = pCamera->mtxView._21;
			g_aBullet[nCntBullet].mtxWorld._13 = pCamera->mtxView._31;
			g_aBullet[nCntBullet].mtxWorld._21 = pCamera->mtxView._12;
			g_aBullet[nCntBullet].mtxWorld._22 = pCamera->mtxView._22;
			g_aBullet[nCntBullet].mtxWorld._23 = pCamera->mtxView._32;
			g_aBullet[nCntBullet].mtxWorld._31 = pCamera->mtxView._13;
			g_aBullet[nCntBullet].mtxWorld._32 = pCamera->mtxView._23;
			g_aBullet[nCntBullet].mtxWorld._33 = pCamera->mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, g_pTextureBullet);


			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}


	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fRadius, g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fRadius, -g_aBullet[nCntBullet].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fRadius, -g_aBullet[nCntBullet].fRadius, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, float fRadius, BULLETTYPE type)
{
	// 頂点情報の設定
		VERTEX_3D *pVtx;

		//頂点バッファをロック
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
		{
			if (g_aBullet[nCntBullet].bUse == false)
			{

				g_aBullet[nCntBullet].pos = pos;	//位置情報
				g_aBullet[nCntBullet].move = move;	//移動量
				g_aBullet[nCntBullet].rot = rot;	//角度
				g_aBullet[nCntBullet].nLife = nLife;//弾の寿命
				g_aBullet[nCntBullet].type = type;//弾の種類
				g_aBullet[nCntBullet].fRadius = fRadius;//半径

				//影の生成
				g_nIndexBulletShadow[nCntBullet] = SetShadow(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					g_aBullet[nCntBullet].fRadius,
					g_aBullet[nCntBullet].fRadius);

				//使用する
				g_aBullet[nCntBullet].bUse = true;
				break;

			}
			pVtx += 4;
 		}
		//頂点バッファをアンロック
		g_pVtxBuffBullet->Unlock();
}


////=============================================================================
//// 弾の取得
////=============================================================================
BULLET *GetBullet(void)
{

	return &g_aBullet[0];

}



bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bHit = false;
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{


		if(pPos->x + VtxMin->x < g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fRadius
		 &&pPos->x + VtxMax->x > g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fRadius
		 &&pPos->z + VtxMin->z < g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fRadius
		 &&pPos->z + VtxMax->z > g_aBullet[nCntBullet].pos.z + g_aBullet[nCntBullet].fRadius
			)
		{
			bHit = true;
		}
	}
	return bHit;
}
