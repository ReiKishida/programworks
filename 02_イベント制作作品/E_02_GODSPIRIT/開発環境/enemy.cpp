//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : 佐藤亮太
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS				// ファイル読み込みで必要
#include <stdio.h>							// ファイル読み込みで必要
#include <string.h>							// ファイル読み込みで必要
#include <stdlib.h>							//rand関数に必要
#include <time.h>							//rand関数を時間で値を決めさせるのに必要
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "effect.h"
#include "main.h"
#include "meshField.h"
#include "meshWall.h"
#include "BIllLife.h"
#include "light.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "model.h"
#include "Item.h"
#include "hit.h"
#include "MessageWindow.h"
#include "result.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

// 係数関係のマクロ
#define ENEMY_INERTIA				(0.15f)						// 敵の慣性
#define ENEMY_AIR_INERTIA			(0.15f)						// 敵のY軸の慣性
#define ENEMY_DIFF_ANGLE			(0.15f)						// 角度を変える量
#define ENEMY_GRAVITY				(-1.5f)						// 重力
#define ENEMY_JUMP_DOWN				(0.1f)						// 敵のジャンプ力の減少量
#define ENEMY_FLY_SIDE_DOWN			(2.0f)						// 敵の吹っ飛ぶ高さの減少量
#define ENEMY_FLY_INERTIA			(0.25f)						// 敵の吹っ飛ぶ慣性

// 敵の行動系のマクロ
#define ENEMY_MOVE00_RADIUS			(1000.0f)					// 敵がプレイヤーに向かって走ってくる反応をする範囲
#define ENEMY_MOVE01_RADIUS			(500.0f)					// 敵が一度様子を見る範囲
#define ENEMY_ATTACK_RADIUS			(100.0f)					// 敵がプレイヤーに攻撃してくる反応をする範囲
#define ENEMY_OZIGI_RADIUS			(200.0f)					// 敵がお辞儀する範囲

#define ENEMY_ATTACK_WAIT_MAX_TIME	(120)						// 敵が攻撃した後の隙の最大の長さ
#define ENEMY_ATTACK_WAIT_MIN_TIME	(90)						// 敵が攻撃した後の隙の最小の長さ
#define ENEMY_ACT_MAX_TIME			(30)						// 敵の行動を決める間隔の最大の長さ
#define ENEMY_ACT_MIN_TIME			(20)						// 敵の行動を決める間隔の最小の長さ

#define ENEMY_DELETE_RADIUS			(100000000000000.0f)					// 敵を消去する範囲

// 敵のStateを切り替えるフレーム数のマクロ
#define ENEMY_DAMAGE_COUNTER		(15)						// ダメージ時の無敵時間
#define ENEMY_DEATH_COUNTER			(60)						// 死亡時に消えるまでの時間

// いろいろ
#define MAX_CHAR					(256)						// テキストファイルから読み込む文字の最大数
#define ENEMY_MOTION_BLEND_FRAME	(10)						// モーションブレンドのフレーム数
#define ENEMY_MOVE_NORM				(0.25f)						// 敵が動いてる基準の移動量
#define ENEMY_MOTION_MOVE_SPEED		(0.4f)						// モーション時の動く量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Enemy_info	g_EnemyInfo[ENEMY_TYPE_MAX];				// 敵の種類ごとの情報
Enemy		g_aEnemy[MAX_ENEMY];						// 敵の情報
int			g_nNumEnemy;								// 敵の人数
float		g_EnemyDeleteRadius;						// 敵を消去する範囲
int			g_nEnemyCounter;							// 現在のフレーム数
//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	// 乱数の初期化
	srand((unsigned int)time(0));								// <-これでrand関数を時間で数値が変わるようにする

	//----------------------------------------------------------------------
	// 変数の初期化
	//----------------------------------------------------------------------
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の総数分ループ

		// 敵の情報の初期化
		g_aEnemy[nCntEnemy].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期設定
		g_aEnemy[nCntEnemy].posOld			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置の初期設定
		g_aEnemy[nCntEnemy].InitPos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
		g_aEnemy[nCntEnemy].Move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の移動量の初期化
		g_aEnemy[nCntEnemy].rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期設定
		g_aEnemy[nCntEnemy].rotDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目的の向きを初期化
		g_aEnemy[nCntEnemy].nIdxLight		= -1;								// -1に初期化

		// 攻撃できない時間をランダムに設定
		g_aEnemy[nCntEnemy].nAttackWaitTime	= rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;				
		g_aEnemy[nCntEnemy].nAttackCounter	= 0;								// 攻撃するまでのカウンターを初期化
		g_aEnemy[nCntEnemy].nLife			= 0;								// ライフを初期化
		g_aEnemy[nCntEnemy].nMaxLife		= 0;								// ライフの最大値を初期化
		g_aEnemy[nCntEnemy].nDownCounter	= 0;								// ダウン状態のカウンター変数を初期化

		// 行動系の変数の初期化
		g_aEnemy[nCntEnemy].nActTime			= rand() % (ENEMY_ACT_MAX_TIME - ENEMY_ACT_MIN_TIME + 1) + ENEMY_ACT_MIN_TIME;		// 行動の時間の変数をランダムで設定
		g_aEnemy[nCntEnemy].nActNowCounter	= 0;								// 行動のカウンター変数を初期化
		g_aEnemy[nCntEnemy].bMoved			= false;							// まだ動いていない状態にする

		g_aEnemy[nCntEnemy].fRadius			= 0.0f;								// 当たり判定の半径を初期化
		g_aEnemy[nCntEnemy].fPosLeght		= 0.0f;								// プレイイヤーとの距離を初期化
		g_aEnemy[nCntEnemy].fJampPower		= 0.0f;								// ジャンプ力を設定
		g_aEnemy[nCntEnemy].fFlyPower		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 吹っ飛び力を設定
		g_aEnemy[nCntEnemy].bUse				= false;							// 使用していない状態にする
		g_aEnemy[nCntEnemy].Motionstate		= ENEMY_MOTION_NEUTRAL;				// モーションをニュートラルに初期化
		g_aEnemy[nCntEnemy].MotionstateOld	= ENEMY_MOTION_MAX;					// 前回のモーションをニュートラルに初期化

		// 状態の変数の初期化
		g_aEnemy[nCntEnemy].State			= ENEMY_STATE_NORMAL;				// 通常状態に初期化
		g_aEnemy[nCntEnemy].nCounterState	= 0;								// ステートのカウンターを初期化

		// モーションの変数の初期化
		g_aEnemy[nCntEnemy].nCounterKey		= 0;								// 現在のキーフレームを初期化
		g_aEnemy[nCntEnemy].nCounterFrame	= 0;								// 現在のフレーム数を初期化
		g_aEnemy[nCntEnemy].nCounterAllFrame = 0;								// 現在の全フレーム数を初期化
		g_aEnemy[nCntEnemy].nFrame			= 0;								// 今回のフレーム数を初期化
		g_aEnemy[nCntEnemy].pMeshField		= NULL;								// メッシュフィールド情報
		g_aEnemy[nCntEnemy].pModel			 = NULL;							// モデル情報
		for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
		{// モデルの数分ループ
			// 変数の初期化
			g_aEnemy[nCntEnemy].posMotion[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置の差分を初期化
			g_aEnemy[nCntEnemy].rotMotion[nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 回転の差分を初期化
		}
		// 攻撃状態を設定
		for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
		{
			g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;			// 攻撃中じゃない状態に設定
			g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;			// 攻撃の準備状態じゃない状態に設定
		}
	}

	g_nNumEnemy		= 0;				// 敵の人数を初期化
	g_nEnemyCounter = 0;			// 現在のフレーム数を初期化

	// 更新する範囲を初期化
	g_EnemyDeleteRadius = ENEMY_DELETE_RADIUS * ENEMY_DELETE_RADIUS;			// 消去する距離

	// パーツの位置の設定
	EnemyModelSet();

	// 敵の配置情報の読み込み
	EnemySetLode();

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// 敵の種類分ループ

		for (int nCntModel = 0; nCntModel < g_EnemyInfo[nCntEnemyType].MaxModel; nCntModel++)
		{// モデルの数分ループ

			// メッシュの開放
			if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh != NULL)
			{// 中身が入ってたら
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->Release();
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh = NULL;
			}

			// マテリアルの開放
			if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat != NULL)
			{// 中身が入ってたら
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat->Release();
				g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat = NULL;
			}

			for (int nCntTexture = 0; nCntTexture < (int)g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat; nCntTexture++)
			{// マテリアルの数分ループ
				// テクスチャの開放
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntTexture] != NULL)
				{
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntTexture]->Release();
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntTexture] = NULL;
				}
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	// 変数宣言
	MODE	pMode	= GetMode();			// 現在のモードを取得
	Camera *pCamera = GetCamera();			// カメラの情報を取得
	Player *pPlayer = GetPlayer();			// プレイヤーの情報の取得

	float fDiffAngle;						// 差分

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の総数分ループ
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 使われてなければ

			if (pMode == MODE_GAME ||
				pMode == MODE_TUTORIAL)
			{// ゲームとチュートリアル時のとき
			 // 敵とプレイヤーの距離の計算
				float posX = (g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x);				// (敵の位置座標－プレイヤーの位置座標)
				float posZ = (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z);				// (敵の位置座標－プレイヤーの位置座標)
				g_aEnemy[nCntEnemy].fPosLeght = (posX * posX) + (posZ * posZ);			// それぞれを2乗して距離を計算

				g_aEnemy[nCntEnemy].Move.y += ENEMY_GRAVITY;			// 重力の加算
				if (g_aEnemy[nCntEnemy].pos.y <= -500)
				{// カウンターが0以下になったら
					DeletEnemy(nCntEnemy, false);							// 敵を削除する
				}
				
				//-----------------------------------------------
				// 敵の操作
				//-----------------------------------------------
				if (g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_LANDING	&&
					g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_DAMAGE	&&
					g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_FLY		&&
					g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_GETUP)
				{// 特定のモーション時には操作を受け付けない
					EnemyController(nCntEnemy);										// 敵の操作と位置か回転の更新
				}
				if (g_aEnemy[nCntEnemy].fJampPower > 0.0f)
				{// ジャンプ力があったら
					g_aEnemy[nCntEnemy].Move.y += g_aEnemy[nCntEnemy].fJampPower;		// 上昇していく
					g_aEnemy[nCntEnemy].fJampPower -= ENEMY_JUMP_DOWN;				// ジャンプ力を減らしていく

				}
				else if (g_aEnemy[nCntEnemy].fJampPower < 0.0f)
				{// ジャンプ力が0.0fより下回ったら
					g_aEnemy[nCntEnemy].fJampPower = 0.0f;
				}
				if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
				{// 吹っ飛び力がまだあったら
					if (g_aEnemy[nCntEnemy].fFlyPower.x >= 0.1f ||
						g_aEnemy[nCntEnemy].fFlyPower.x <= -0.1f)
					{// 水平方向の吹っ飛び力があるなら
						g_aEnemy[nCntEnemy].Move.x += g_aEnemy[nCntEnemy].fFlyPower.x;										// 吹っ飛び力を加算
					}
					if (g_aEnemy[nCntEnemy].fFlyPower.z >= 0.1f ||
						g_aEnemy[nCntEnemy].fFlyPower.z <= -0.1f)
					{// 水平方向の吹っ飛び力があるなら
						g_aEnemy[nCntEnemy].Move.z += g_aEnemy[nCntEnemy].fFlyPower.z;										// 吹っ飛び力を加算
					}

					if (g_aEnemy[nCntEnemy].nFlyCounter > 0)
					{// 吹っ飛ぶカウンターが0より大きかったら
						g_aEnemy[nCntEnemy].fFlyPower.y -= ENEMY_JUMP_DOWN;			// ジャンプ力を減らしていく

						if (g_aEnemy[nCntEnemy].fFlyPower.y > 0.0f)
						{// ジャンプ力がまだあったら
							g_aEnemy[nCntEnemy].Move.y += g_aEnemy[nCntEnemy].fFlyPower.y;			// 上昇していく
						}
					}
				}


				if (g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_ATTACK)
				{// 攻撃モーションの時じゃなければ向きを回転させる

					// 目的の角度を修正する
					if (g_aEnemy[nCntEnemy].rotDest.y > D3DX_PI)
					{// +3.14より、超えてしまったら
						g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
					}
					if (g_aEnemy[nCntEnemy].rotDest.y < -D3DX_PI)
					{// -3.14より、下回ったら
						g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
					}

					// 差分を入れる
					fDiffAngle = (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y);

					// 差分の角度を修正する
					if (fDiffAngle > D3DX_PI)
					{// +3.14より、超えてしまったら
						fDiffAngle -= D3DX_PI * 2.0f;
					}
					if (fDiffAngle < -D3DX_PI)
					{// -3.14より、下回ったら
						fDiffAngle += D3DX_PI * 2.0f;
					}

					// 差分を加算していく
					g_aEnemy[nCntEnemy].rot.y += fDiffAngle * ENEMY_DIFF_ANGLE;

					// 現在の角度を修正する
					if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
					{// +3.14より、超えてしまったら
						g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
					}
					if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
					{// -3.14より、下回ったら
						g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
					}
				}

				// 慣性
				g_aEnemy[nCntEnemy].Move.x += (0.0f - g_aEnemy[nCntEnemy].Move.x) * ENEMY_INERTIA;
				g_aEnemy[nCntEnemy].Move.z += (0.0f - g_aEnemy[nCntEnemy].Move.z) * ENEMY_INERTIA;
				g_aEnemy[nCntEnemy].Move.y += (0.0f - g_aEnemy[nCntEnemy].Move.y) * ENEMY_AIR_INERTIA;

				// 敵の位置を更新
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].Move;

				// 影の位置の更新
				SetPositionShadow(g_aEnemy[nCntEnemy].nIdyShadow, D3DXVECTOR3(g_aEnemy[nCntEnemy].aModel[0].mtxWorld._41, g_aEnemy[nCntEnemy].aModel[0].mtxWorld._42, g_aEnemy[nCntEnemy].aModel[0].mtxWorld._43), g_aEnemy[nCntEnemy].InitPos);
				// ライフの移動
				SetPositionBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos);

				//-----------------------------------------------
				// 当たり判定
				//-----------------------------------------------
				CollisionEnemy(&g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].fRadius);
				if (CollisionModelMinMax(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius, &g_aEnemy[nCntEnemy].pModel) != 0)
				{
					if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
					{// ジャンプしていたら
					 //g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = true;					// 着地状態にする
					}
					Shadow *pShadow = GetShadow();
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].pos.y = g_aEnemy[nCntEnemy].pos.y + 0.1f;
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].col.a = 1.0f;
				}
				if (CollisionModelMinMax(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius, &g_aEnemy[nCntEnemy].pModel) != 0)
				{// フィールドの当たり判定
					if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
					{// ジャンプしていたら
					 //g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = true;					// 着地状態にする
					}

					g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] = false;						// ジャンプできる状態にする

					if (g_aEnemy[nCntEnemy].Motionstate == ENEMY_MOTION_FLY)
					{// 吹っ飛びモーション時着地したら
						if (g_aEnemy[nCntEnemy].State != ENEMY_STATE_DEATH)
						{// 死んでなければ
							if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
							{// 吹っ飛び最中なら

								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = false;						// 倒れている状態から復活
								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = true;						// 倒れている状態から復活
								g_aEnemy[nCntEnemy].nDownCounter = rand() % (120 - 60 + 1) + 60;	// 起き上がる時間を設定
							}
						}

						// 吹っ飛び力を0にして、滑らせない
						g_aEnemy[nCntEnemy].fFlyPower.x = 0.0f;
						g_aEnemy[nCntEnemy].fFlyPower.z = 0.0f;
					}

				}
				// メッシュウォール
				if (CollisionVecMeshWall(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius) == true)
				{

				}
				if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] == false)
				{// 跳ね返り優先
					if (CollisionMeshWall(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move, g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius) == true)
					{// 壁に当たったら
					}
				}

				if (CollisionMeshField(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].Move,
					g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius, &g_aEnemy[nCntEnemy].pMeshField) == true)
				{
					Shadow *pShadow = GetShadow();
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].pos.y = g_aEnemy[nCntEnemy].pos.y + 0.1f;
					pShadow[g_aEnemy[nCntEnemy].nIdyShadow].col.a = 1.0f;

					if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
					{// ジャンプしていたら
						//g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = true;					// 着地状態にする
					}

					g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] = false;						// ジャンプできる状態にする

					if (g_aEnemy[nCntEnemy].Motionstate == ENEMY_MOTION_FLY)
					{// 吹っ飛びモーション時着地したら
						if (g_aEnemy[nCntEnemy].State != ENEMY_STATE_DEATH)
						{// 死んでなければ
							if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
							{// 吹っ飛び最中なら

								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = false;						// 倒れている状態から復活
								g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = true;							// 倒れている状態から復活
								g_aEnemy[nCntEnemy].nDownCounter = rand() % (120 - 60 + 1) + 60;	// 起き上がる時間を設定
							}

						}

						// 吹っ飛び力を0にして、滑らせない
						g_aEnemy[nCntEnemy].fFlyPower.x = 0.0f;
						g_aEnemy[nCntEnemy].fFlyPower.z = 0.0f;
					}
				}

				//-------------------------------------------------------
				// 攻撃の当たり判定
				//-------------------------------------------------------
				for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
				{// キャラクターの攻撃の当たり判定の総数分ループ
					if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse == true)
					{// 当たり判定が使われているとき
						if (int(g_aEnemy[nCntEnemy].Motionstate) == g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nMotionType)
						{// 当たり判定のモーションが一緒だったら
							if (g_aEnemy[nCntEnemy].nCounterAllFrame >= g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionStart &&
								g_aEnemy[nCntEnemy].nCounterAllFrame <= g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionEnd)
							{// 当たり判定が発生するフレーム数いないだったら

								// プレイヤーへの攻撃判定
								CollisionPlayer(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._43), g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].fRadius, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nAttackPower, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].fDamageSide, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].fDamageVertical);

								// 攻撃時にエフェクト
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld._43), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, 1.0f), g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].fRadius, 10, EFFECTTYPE_EXPLOSION);
							}

						}
					}
				}

#if 0	// 体の当たり判定にエフェクトを出して見えるようにするか

				//-------------------------------------------------------
				// 体の当たり判定
				//-------------------------------------------------------
				for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_Enemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
				{// キャラクターの体の当たり判定の総数分ループ

					if (g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
					{// 体の当たり判定が使われているとき
						SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41,
							g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42,
							g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
							D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),
							g_Enemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius,
							1, EffectType_Normal);	// 当たり判定を可視化

					}
				}
#endif

				//-----------------------------------------------
				// モーションの更新
				//-----------------------------------------------
				if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] == true)
				{// ダメージ中だったら
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_FLY;										// 吹っ飛び中に設定
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] == true)
				{// ダメージ中だったら
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_DAMAGE;									// ダメージ中に設定
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] == true)
				{// ダメージ中だったら
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_GETUP;									// ダウン中に設定
				}
				else if (g_aEnemy[nCntEnemy].bEnemyAttack[ENEMY_ATTACKTYPE_ATTACK] == true)
				{// 敵が攻撃中なら
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_ATTACK;					// 攻撃モーションに設定
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] == true)
				{// 敵がジャンプ中なら
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_JUMP;						// ジャンプモーションに設定
				}
				else if (g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] == true)
				{// 着地中なら
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_LANDING;					// 着地モーションを設定
				}
				else if (g_aEnemy[nCntEnemy].Move.x > ENEMY_MOVE_NORM ||
					g_aEnemy[nCntEnemy].Move.x < -ENEMY_MOVE_NORM ||
					g_aEnemy[nCntEnemy].Move.z >  ENEMY_MOVE_NORM ||
					g_aEnemy[nCntEnemy].Move.z < -ENEMY_MOVE_NORM)
				{// 敵が動いているとき
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_MOVE;				// 移動状態にする
				}
				else
				{// 敵が動いてないとき
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_NEUTRAL;			// ニュートラル状態にする
				}

				EnemyMotion(nCntEnemy);												// 敵のモーションの更新


				// 敵の前回の情報を保存
				g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;				// 前回の位置を保存
				g_aEnemy[nCntEnemy].MotionstateOld = g_aEnemy[nCntEnemy].Motionstate;		// 前回のモーションのステートを保存

				if (g_EnemyDeleteRadius < g_aEnemy[nCntEnemy].fPosLeght)
				{// 消去する範囲の外にいたら
					DeletEnemy(nCntEnemy, false);										// 敵を消去する
				}
			}
			else
			{// それ以外の時

				// 敵とカメラの距離の計算
				float posX = (g_aEnemy[nCntEnemy].pos.x -pCamera->posV.x);				// (敵の座標－カメラの視点の座標)を2乗する
				float posZ = (g_aEnemy[nCntEnemy].pos.z -pCamera->posV.z);				// (敵の座標－カメラの視点の座標)を2乗する
				float fPosLenght = (posX * posX) + (posZ * posZ);						// それぞれを2乗して距離を計算

				// お辞儀をする半径の距離の計算
				float fRadiusLenght = (ENEMY_OZIGI_RADIUS) * (ENEMY_OZIGI_RADIUS);		// (お辞儀をする半径)を2乗して半径の距離を計算

				// 座標の距離と半径の距離を比較
				if (fPosLenght <  fRadiusLenght &&
					fPosLenght > -fRadiusLenght)
				{// 半径の距離よりも位置の距離のほうが0に近かったら
					g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_ATTACK;				// モーションをさせる

				}

				EnemyMotion(nCntEnemy);													// 敵のモーションの更新
				g_aEnemy[nCntEnemy].MotionstateOld = g_aEnemy[nCntEnemy].Motionstate;		// 前回のモーションのステートを保存
			}

			//---------------------------------------------
			// 敵の状態別の処理
			//---------------------------------------------
			switch (g_aEnemy[nCntEnemy].State)
			{// 敵の状態が
			case ENEMY_STATE_NORMAL:		// 通常状態なら
				// 何もしない
				break;

			case ENEMY_STATE_DAMAGE:		// ダメージ状態なら

				g_aEnemy[nCntEnemy].nCounterState--;						// カウンターを減らしていく

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// カウンターが0以下になったら
					g_aEnemy[nCntEnemy].State = ENEMY_STATE_NORMAL;		// 通常状態に戻す
				}

				break;

			case ENEMY_STATE_DEATH:		// 死亡状態なら

				g_aEnemy[nCntEnemy].nCounterState--;						// カウンターを減らしていく

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// カウンターが0以下になったら
					DeletEnemy(nCntEnemy, true);							// 敵を削除する
				}

				break;
			}
		}
	}

	// 現在のフレーム数を加算
	g_nEnemyCounter++;

	if (g_nNumEnemy <= 0)
	{
		if (GetGameState() != GAMESTATE_END &&GetGameState() != GAMESTATE_END2)
		{
			SetResultType(RESULTTYPE_CLEAR);		// ゲームクリア！を設定
			StopSound(SOUND_LABEL_GAMEBGM1);			// ボスBGM停止
			PlaySound(SOUND_LABEL_GAMECLEARE);		// クリアBGM再生
			SetGameState(GAMESTATE_END);			// ゲームを終了
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの初期化
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DXMATRIX mtxParent;							// 階層構造用のマトリックス
	D3DMATERIAL9 matDef;							// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;								// マテリアルデータへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の総数分ループ

		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 使われていれば

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// 回転(向き)を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aEnemy[nCntEnemy].rot.y,
				g_aEnemy[nCntEnemy].rot.x,
				g_aEnemy[nCntEnemy].rot.z);	// ゲームを制作するときは一番使うＹ軸から入れる

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_aEnemy[nCntEnemy].mtxWorld,
				&mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aEnemy[nCntEnemy].pos.x,				// 位置はX座標から入れていく
				g_aEnemy[nCntEnemy].pos.y,
				g_aEnemy[nCntEnemy].pos.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_aEnemy[nCntEnemy].mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
			{// パーツの数分ループ
				
				if (g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel == -1)
				{// 親モデルがいないとき
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;				// 敵のマトリックスを代入
				}
				else
				{// 親のモデルがいるとき
					mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel].mtxWorld;		// 親のマトリックスを代入
				}

				//---------------------------
				// モデルの描画
				//---------------------------
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				// 回転(向き)を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y,
					g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x,
					g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);	// ゲームを制作するときは一番使うＹ軸から入れる

				// 回転(向き)を設定
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
					&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,
					&mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTrans,
					g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x,				// 位置はX座標から入れていく
					g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y,
					g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);

				// 移動を設定
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
					&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,
					&mtxTrans);


				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,			// 親のワールドマトリックスと子のワールドマトリックスをかけている
					&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,
					&mtxParent);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].nNumMat; nCntMat++)
				{						
						// マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						// テクスチャの設定
						pDevice->SetTexture(0, g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].pTexture[nCntMat]);

						// 敵(パーツ)の描画
						g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelAddress[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}

			//---------------------------
			// 攻撃の当たり判定のワールドマトリクスの設定
			//---------------------------
			for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
			{// キャラクターの攻撃の当たり判定の総数分ループ

				if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse == true)
				{// 攻撃の当たり判定が使われている状態なら処理する
					if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent == -1)
					{// 親モデルがいないとき
						mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// 敵のワールドマトリックスを代入
					}
					else
					{// 親モデルがいるとき
						mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent].mtxWorld;		// 親モデルのマトリックスを代入
					}

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);

					// 移動を反映
					D3DXMatrixTranslation(&mtxTrans,
						g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.x,			// 位置はX座標から入れていく
						g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.y,
						g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.z);

					// 移動を設定
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
						&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,
						&mtxTrans);


					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,			// 親のワールドマトリックスと子のワールドマトリックスをかけている
						&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,
						&mtxParent);

					// ワールドマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);
				}
			}
				//---------------------------------
				// 体の当たり判定
				//---------------------------------
			for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
			{// キャラクターの体の当たり判定の総数分ループ

				if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
				{// 体の当たり判定が使われている状態なら処理する

					if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent == -1)
					{// 親モデルがいないとき
						mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// 敵のワールドマトリックスを代入
					}
					else
					{// 親モデルがいるとき
						mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent].mtxWorld;		// 親モデルのマトリックスを代入
					}

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);

					// 移動を反映
					D3DXMatrixTranslation(&mtxTrans,
						g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.x,			// 位置はX座標から入れていく
						g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.y,
						g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.z);

					// 移動を設定
					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
						&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,
						&mtxTrans);


					D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,			// 親のワールドマトリックスと子のワールドマトリックスをかけている
						&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,
						&mtxParent);

					// ワールドマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);

				}
			}
		}
	}
}

//=============================================================================
// 敵の動き
//=============================================================================
void EnemyController(int nCntEnemy)
{
	Player *pPlayer = GetPlayer();												// プレイヤーの情報の取得

	// 敵が反応する距離の計算
	float fMoveReactionLenght00 = ENEMY_MOVE00_RADIUS * ENEMY_MOVE00_RADIUS;	// 敵がプレイヤーに向かって走る範囲を2乗して距離を計算
	float fMoveReactionLenght01 = ENEMY_MOVE01_RADIUS * ENEMY_MOVE01_RADIUS;	// 敵が一度様子を見る範囲を2乗して距離を計算
	float fAttackReactionLenght = ENEMY_ATTACK_RADIUS * ENEMY_ATTACK_RADIUS;	// 敵が攻撃してくる距離

	int nActType;																// 敵の行動のタイプを入れる
	int nMinAct;																// 割合の最小値を入れる変数
	int nMaxAct;																// 割合の最大値を入れる変数

	float Angle;																// 敵からプレイヤーへの角度

	//----------------------------------------------------------------
	// 敵が行動する割合を設定
	//----------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].fPosLeght  >  fMoveReactionLenght00 &&
		-g_aEnemy[nCntEnemy].fPosLeght < -fMoveReactionLenght00)
	{// 敵が反応範囲よりも遠い場合
		switch (g_aEnemy[nCntEnemy].EnemyType)
		{// 敵の種類が
		case ENEMY_TYPE_ARMOR:		// 鎧の場合
			if (g_aEnemy[nCntEnemy].bMoved == false)
			{// 動いていない状態の行動

				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	 = 1.0f;		// 棒立ち割合を高めに設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		 = 0.0f;		// プレイヤーを見る割合を設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	 = 0.0f;		// うろつきを低めにして、適度に行動させる
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る割合を0にして行動させない
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	 = 0.0f;		// 攻撃する割合を0にして行動させない
			}
			else
			{// 動いた後の行動
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	 = 0.5f;		// 棒立ち割合を低めに設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		 = 0.0f;		// プレイヤーを見る割合を設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	 = 0.5f;		// うろつく割合を低めに設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る行動を高確率でさせる
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	 = 0.0f;		// 攻撃する割合を0にして行動させない

			}

			break;

		case ENEMY_TYPE_HITOME:		// 一つ目の場合

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	 = 0.75f;			// 棒立ち割合を高めに設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		 = 0.0f;			// プレイヤーを見る割合を設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	 = 0.25f;			// うろつきを低めにして、適度に行動させる
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// プレイヤーに向かって走る割合を0にして行動させない
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	 = 0.0f;			// 攻撃する割合を0にして行動させない

			break;

		case ENEMY_TYPE_HIGECHAN:	// ひげちゃんの場合

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.75f;			// 棒立ち割合を高めに設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;			// プレイヤーを見る割合を設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.25f;			// うろつきを低めにして、適度に行動させる
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// プレイヤーに向かって走る割合を0にして行動させない
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// 攻撃する割合を0にして行動させない

			break;

		case ENEMY_TYPE_HONCHAN:	// 本ちゃんの場合

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.75f;			// 棒立ち割合を高めに設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;			// プレイヤーを見る割合を設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.25f;			// うろつきを低めにして、適度に行動させる
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// プレイヤーに向かって走る割合を0にして行動させない
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// 攻撃する割合を0にして行動させない

			break;

		}
	}
	else
	{// 敵が反応範囲の場合
		if (g_aEnemy[nCntEnemy].fPosLeght  >  fMoveReactionLenght01 &&
			-g_aEnemy[nCntEnemy].fPosLeght < -fMoveReactionLenght01)
		{// 一度様子を見る範囲よりも外だったら
			switch (g_aEnemy[nCntEnemy].EnemyType)
			{// 敵の種類が
			case ENEMY_TYPE_ARMOR:		// 鎧の場合
				if (g_aEnemy[nCntEnemy].bMoved == false)
				{// 動いていない状態の行動
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 1.0f;		// プレイヤーを攻撃する前は棒立ち
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// うろつく割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る行動を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// 攻撃する割合を設定する
				}
				else
				{// 動いた後の行動
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を低めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.1f;		// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// うろつく割合を低めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.8f;		// プレイヤーに向かって走る行動を高確率でさせる
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// 攻撃する割合を0にして行動させない

				}

				break;

			case ENEMY_TYPE_HITOME:		// 一つ目の場合

			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を低めに設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.1f;		// プレイヤーを見る割合を設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// うろつく割合を低めに設定する
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.8f;		// プレイヤーに向かって走る行動を高確率でさせる
			g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// 攻撃する割合を0にして行動させない

			break;

			case ENEMY_TYPE_HIGECHAN:	// ひげちゃんの場合

				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を高めに設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.25f;		// プレイヤーを見る割合を設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.25f;		// うろつきを低めにして、適度に行動させる
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.5f;		// プレイヤーに向かって走る割合を0にして行動させない
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// 攻撃する割合を0にして行動させない

				break;

			case ENEMY_TYPE_HONCHAN:	// 本ちゃんの場合

				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;			// 棒立ち割合を高めに設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.5f;			// プレイヤーを見る割合を設定する
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.5f;			// うろつきを低めにして、適度に行動させる
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// プレイヤーに向かって走る割合を0にして行動させない
				g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// 攻撃する割合を0にして行動させない

				break;
			}
		}
		else
		{// 様子見する範囲内だったら

			if (g_aEnemy[nCntEnemy].fPosLeght  < fAttackReactionLenght &&
				g_aEnemy[nCntEnemy].fPosLeght  > -fAttackReactionLenght)
			{// プレイヤーが敵が攻撃する範囲に入ったら
				switch (g_aEnemy[nCntEnemy].EnemyType)
				{// 敵の種類が
				case ENEMY_TYPE_ARMOR:		// 鎧の場合
					if (g_aEnemy[nCntEnemy].bMoved == false)
					{// 動いていない状態の行動
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を低めに設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// プレイヤーを見る割合を設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// うろつく
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る割合を0にして行動させない
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 1.0f;		// 攻撃する割合を1.0にして高確率で行動させる
					}
					else
					{// 動いた後の行動
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を低めに設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.2f;		// プレイヤーを見る割合を設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// うろつく割合を低めに設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る行動を高確率でさせる
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// 攻撃する割合を0にして行動させない

					}

					break;

				case ENEMY_TYPE_HITOME:		// 一つ目の場合

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を低めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.2f;		// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// うろつく
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る割合を0にして行動させない
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// 攻撃する割合を1.0にして高確率で行動させる

					break;

				case ENEMY_TYPE_HIGECHAN:	// ひげちゃんの場合

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を高めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.1f;		// うろつきを低めにして、適度に行動させる
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.2f;		// プレイヤーに向かって走る割合を0にして行動させない
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// 攻撃する割合を0にして行動させない

					break;

				case ENEMY_TYPE_HONCHAN:	// 本ちゃんの場合

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を高めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.3f;		// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// うろつきを低めにして、適度に行動させる
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る割合を0にして行動させない
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.7f;		// 攻撃する割合を0にして行動させない

					break;


				}
			}
			else
			{// 攻撃範囲内より外の場合
				switch (g_aEnemy[nCntEnemy].EnemyType)
				{// 敵の種類が
				case ENEMY_TYPE_ARMOR:		// 鎧の場合

					if (g_aEnemy[nCntEnemy].bMoved == false)
					{// 動いていない状態の行動
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 1.0f;		// 棒立ち割合を低めに設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.0f;		// プレイヤーを見る割合を設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;		// うろつく
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;		// プレイヤーに向かって走る割合を1.0にして高確率で行動させる
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// 攻撃する割合を0にして行動させない
					}
					else
					{// 動いた後の行動
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を低めに設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.35f;		// プレイヤーを見る割合を設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.35f;		// うろつく割合を低めに設定する
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.3f;		// プレイヤーに向かって走る行動を高確率でさせる
						g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// 攻撃する割合を0にして行動させない

					}

					break;

				case ENEMY_TYPE_HITOME:		// 一つ目の場合

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;		// 棒立ち割合を低めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.35f;		// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.35f;		// うろつく
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.3f;		// プレイヤーに向かって走る割合を1.0にして高確率で行動させる
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;		// 攻撃する割合を0にして行動させない

					break;

				case ENEMY_TYPE_HIGECHAN:	// ひげちゃんの場合

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;			// 棒立ち割合を高めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.35f;			// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.35f;			// うろつきを低めにして、適度に行動させる
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.0f;			// プレイヤーに向かって走る割合を0にして行動させない
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// 攻撃する割合を0にして行動させない

					break;

				case ENEMY_TYPE_HONCHAN:	// 本ちゃんの場合

					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL]	  = 0.0f;			// 棒立ち割合を高めに設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK]		  = 0.5f;			// プレイヤーを見る割合を設定する
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER]	  = 0.0f;			// うろつきを低めにして、適度に行動させる
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES] = 0.5f;			// プレイヤーに向かって走る割合を0にして行動させない
					g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK]	  = 0.0f;			// 攻撃する割合を0にして行動させない

					break;


				}
			}
		}
	}

	//---------------------------------------------------------
	// 敵の行動を決定
	//---------------------------------------------------------
	if (g_aEnemy[nCntEnemy].Motionstate != ENEMY_MOTION_ATTACK)
	{// 攻撃モーション中じゃなければ

		if (g_aEnemy[nCntEnemy].nActNowCounter % g_aEnemy[nCntEnemy].nActTime == 0)
		{// 一定フレームごとに行動を変更

			// 敵の行動を決定
			nActType = rand() % int((100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_NEUTRAL])	 +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_LOOK])		 +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_WANDER])	 +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_APPROACHES]) +
									(100 * g_aEnemy[nCntEnemy].fActProportion[ENEMY_ACT_TYPE_ATTACK])	 )
									+ 1;

			// 割合の数値の初期化
			nMinAct = 1;
			nMaxAct = nMinAct + int(100 * g_aEnemy[nCntEnemy].fActProportion[0]);

			//--------------------------------------------------------
			// 行動の判定
			//--------------------------------------------------------
			for (int nCntAct = 0; nCntAct < ENEMY_ACT_TYPE_MAX; nCntAct++)
			{// 行動の種類分ループ
				if (nActType >= nMinAct &&
					nActType < nMaxAct)
				{// 割合の中に入っているかを判定
					g_aEnemy[nCntEnemy].nActType = nCntAct;								// 行動を決定

					if (nCntAct == ENEMY_ACT_TYPE_WANDER)
					{// うろつく行動に決定したら
					 // ランダムに角度を設定
						g_aEnemy[nCntEnemy].fWanderAngle = float(rand() % (628) + 0);								// 0～628(3.14 * 100 * 2をした値(角度を整数としてあらわす))をランダムで出す
						g_aEnemy[nCntEnemy].fWanderAngle = g_aEnemy[nCntEnemy].fWanderAngle / 100 - 3.14f;	// 整数で出した角度を100で割って3.14で引くと正確な角度を求める

					}
				}

				// 判定する範囲をずらす
				nMinAct += int(100 * g_aEnemy[nCntEnemy].fActProportion[nCntAct]);		// 前回の最大値を最小値に設定
				nMaxAct += int(100 * g_aEnemy[nCntEnemy].fActProportion[nCntAct + 1]);	// 前回の最大値に今回の数値を加算

			}
		}

		//---------------------------------------------------------
		// 行動を実行
		//---------------------------------------------------------
		switch (g_aEnemy[nCntEnemy].nActType)
		{
		case ENEMY_ACT_TYPE_NEUTRAL:		// 棒立ちの時
			// 何もしない
			break;

		case ENEMY_ACT_TYPE_LOOK:			// プレイヤーを見る時
			// プレイヤーへの角度を算出
			Angle = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z);

			g_aEnemy[nCntEnemy].rotDest.y = Angle - D3DX_PI;								// キャラをプレイヤーの方向に向かせる

			break;

		case ENEMY_ACT_TYPE_WANDER:			// うろつく時
			// ランダムな方向に動く
			g_aEnemy[nCntEnemy].Move.x += sinf(g_aEnemy[nCntEnemy].fWanderAngle) * g_aEnemy[nCntEnemy].fMoveSpeed;
			g_aEnemy[nCntEnemy].Move.z += cosf(g_aEnemy[nCntEnemy].fWanderAngle) * g_aEnemy[nCntEnemy].fMoveSpeed;

			g_aEnemy[nCntEnemy].rotDest.y = g_aEnemy[nCntEnemy].fWanderAngle - D3DX_PI;								// キャラを走っている方向に向かせる

			break;


		case ENEMY_ACT_TYPE_APPROACHES:		// 接近の時
			// プレイヤーへの角度を算出
			Angle = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z);

			// プレイヤーに向かって走っていく
			g_aEnemy[nCntEnemy].Move.x += sinf(Angle) * g_aEnemy[nCntEnemy].fMoveSpeed;
			g_aEnemy[nCntEnemy].Move.z += cosf(Angle) * g_aEnemy[nCntEnemy].fMoveSpeed;

			g_aEnemy[nCntEnemy].rotDest.y = Angle - D3DX_PI;								// キャラを走っている方向に向かせる

			break;

		case ENEMY_ACT_TYPE_ATTACK:		// 攻撃の時
			if (pPlayer->bMotionState[PLAYER_BOOL_DAMAGE] == false &&
				pPlayer->bMotionState[PLAYER_BOOL_FLY]	 == false &&
				pPlayer->bMotionState[PLAYER_BOOL_GETUP]	 == false )
			{// プレイヤーがやられている状態じゃなければ

				if (g_aEnemy[nCntEnemy].nAttackWaitTime <= g_aEnemy[nCntEnemy].nAttackCounter)
				{// 敵が攻撃の待機時間が終わっていたら
					g_aEnemy[nCntEnemy].bEnemyAttack[ENEMY_ATTACKTYPE_ATTACK] = true;			// 攻撃中に設定
					g_aEnemy[nCntEnemy].nAttackCounter = 0;										// 攻撃までの時間を初期化
					g_aEnemy[nCntEnemy].bMoved = true;											// 動いた状態に設定

				}
			}

			break;
		}

		if (pPlayer->bMotionState[PLAYER_BOOL_DAMAGE] == false &&
			pPlayer->bMotionState[PLAYER_BOOL_FLY]	 == false &&
			pPlayer->bMotionState[PLAYER_BOOL_GETUP]	 == false )
		{// プレイヤーがやられている状態じゃなければ

			// 攻撃するまでの時間を加算
			g_aEnemy[nCntEnemy].nAttackCounter++;

		}

		// 行動のカウンターを加算
		g_aEnemy[nCntEnemy].nActNowCounter++;												// 毎フレーム加算する
	}
}

//=============================================================================
// 敵各モデルの設定
//=============================================================================
void EnemyModelSet(void) 
{
	// 敵モデルの設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の総数分ループ
		for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
		{// パーツの数分ループ
			g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxModel = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].nIdxModel;		// 親モデルのインデックス
			g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].nIdxParentModel;		// 親モデルのインデックス
			g_aEnemy[nCntEnemy].aModel[nCntParts].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].pos;								// 位置の設定
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].rot;								// 向きの設定

		}
	}
}

//=============================================================================
// 敵のモーション
//=============================================================================
void EnemyMotion(int nCntEnemy)
{
	//-----------------------------------------------------------------------
	// モーションブレンドの設定
	//-----------------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].Motionstate != g_aEnemy[nCntEnemy].MotionstateOld)
	{// モーションのステートが変わったら
		g_aEnemy[nCntEnemy].nCounterFrame = 0;						// 現在のキーフレームのフレーム数を初期化
		g_aEnemy[nCntEnemy].nCounterKey = 0;							// 現在のキーフレームの初期化
		g_aEnemy[nCntEnemy].nCounterAllFrame = 0;					// モーションの全フレーム数を初期化
		g_aEnemy[nCntEnemy].nFrame = ENEMY_MOTION_BLEND_FRAME;		// モーションブレンドのフレーム数を設定
	}

	//-----------------------------------------------
	// モーション時の動作
	//-----------------------------------------------

	switch (g_aEnemy[nCntEnemy].Motionstate)
	{// 敵のモーションが
	case ENEMY_MOTION_ATTACK:														// 攻撃モーション00だったら
		
		if (g_aEnemy[nCntEnemy].nCounterAllFrame >= 20 &&
			g_aEnemy[nCntEnemy].nCounterAllFrame <= 30)
		{// 攻撃モーションの殴るとき
			// 前に前進する
			g_aEnemy[nCntEnemy].Move.x += (sinf(g_aEnemy[nCntEnemy].rot.y - D3DX_PI) * ENEMY_MOTION_MOVE_SPEED);
			g_aEnemy[nCntEnemy].Move.z += (cosf(g_aEnemy[nCntEnemy].rot.y - D3DX_PI) * ENEMY_MOTION_MOVE_SPEED);
			break;
		}
	}

	//-----------------------------------------------
	// モーション時の効果音
	//-----------------------------------------------
	switch (g_aEnemy[nCntEnemy].Motionstate)
	{// 敵のモーションが
	case ENEMY_MOTION_GETUP:														// ダウンモーションだったら
		if (g_aEnemy[nCntEnemy].nCounterAllFrame == 0)
		{// 起き上がりの音
		}
		break;
	}
	

	//-----------------------------------------------------------------------
	// キーフレームの切り替わり
	//-----------------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].nCounterFrame == g_aEnemy[nCntEnemy].nFrame)
	{// モーションブレンドの時、フレーム数が最後になったら
		g_aEnemy[nCntEnemy].nCounterFrame = 0;															// 現在のキーフレームのフレーム数を初期化
		g_aEnemy[nCntEnemy].nFrame = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[g_aEnemy[nCntEnemy].nCounterKey].nFrame;	// モーションのフレーム数を設定
		g_aEnemy[nCntEnemy].nCounterKey++;																// キーフレームを進める

			if (g_aEnemy[nCntEnemy].nCounterKey == g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nNumKey)
			{// キーフレームが最後まで来たら
				if (g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nLoop == 0)
				{// ループしない場合
					switch (g_aEnemy[nCntEnemy].Motionstate)
					{// 敵のモーションが
					case ENEMY_MOTION_ATTACK:																		// 攻撃モーションだったら
						g_aEnemy[nCntEnemy].bEnemyAttack[ENEMY_ATTACKTYPE_ATTACK] = false;							// 攻撃中じゃない状態に設定
						// 攻撃できない時間をランダムに設定
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;				
						g_aEnemy[nCntEnemy].nCounterKey--;															// キーフレームを戻す
						break;

					case ENEMY_MOTION_JUMP:																// ジャンプモーションだったら
						g_aEnemy[nCntEnemy].nCounterKey--;															// キーフレームを戻す
						break;

					case ENEMY_MOTION_LANDING:															// 着地モーションだったら
						g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = false;														// 着地状態じゃない状態にする
						g_aEnemy[nCntEnemy].nCounterKey--;															// キーフレームを戻す

						break;

					case ENEMY_MOTION_DAMAGE:															// ダメージモーションだったら
						g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] = false;												// ダメージ状態じゃない状態にする

						// 攻撃状態のリセット
						for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
						{// 攻撃の種類分ループ
							g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack]		 = false;					// 攻撃状態をリセットする
							g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;					// 攻撃状態をリセットする
						}
						// 攻撃できない時間をランダムに設定
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;

						g_aEnemy[nCntEnemy].bMoved = true;												// 動いた状態に設定

						g_aEnemy[nCntEnemy].nCounterKey--;												// キーフレームを戻す

						break;

					case ENEMY_MOTION_FLY:																// 吹っ飛びモーションだったら

						// 攻撃状態のリセット
						for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
						{// 攻撃の種類分ループ
							g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;						// 攻撃状態をリセットする
							g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;					// 攻撃状態をリセットする
						}
						// 攻撃できない時間をランダムに設定
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;

						g_aEnemy[nCntEnemy].nCounterKey--;												// キーフレームを戻す

						break;

					case ENEMY_MOTION_GETUP:			// 起き上がり状態モーションだったら
						// 起き上がり状態のリセット
						for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
						{// 攻撃の種類分ループ
							g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;						// 攻撃状態をリセットする
							g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;					// 攻撃状態をリセットする
						}

						g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = false;		// 起き上がり状態解除

						g_aEnemy[nCntEnemy].bMoved = true;											// 動いた状態に設定


						// 攻撃できない時間をランダムに設定
						g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;

						g_aEnemy[nCntEnemy].nCounterKey--;												// キーフレームを戻す

						break;
					}
				}
				else
				{// ループする
					g_aEnemy[nCntEnemy].nCounterKey = (g_aEnemy[nCntEnemy].nCounterKey) % g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].nNumKey;					// キーフレームを進める
					g_aEnemy[nCntEnemy].nCounterAllFrame = 0;																			// モーションのフレーム数を初期化
				}
			}
	}

	//-----------------------------------------------------------------------
	// モーションの差分を代入
	//-----------------------------------------------------------------------
	if (g_aEnemy[nCntEnemy].nCounterFrame == 0)
	{// フレーム数が最初の時
		for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
		{// パーツの数分ループ
			// モーションの位置差分を代入
			g_aEnemy[nCntEnemy].posMotion[nCntParts] = (g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].pos + g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[g_aEnemy[nCntEnemy].nCounterKey].pos[nCntParts]) - g_aEnemy[nCntEnemy].aModel[nCntParts].pos;
			// モーションの回転差分を代入
			g_aEnemy[nCntEnemy].rotMotion[nCntParts] = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[g_aEnemy[nCntEnemy].nCounterKey].rot[nCntParts] - g_aEnemy[nCntEnemy].aModel[nCntParts].rot;

			//----------------------------------------------------
			// 差分の回転を修正する
			//----------------------------------------------------
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].x > D3DX_PI)
			{// +3.14より、超えてしまったら
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].x -= D3DX_PI * 2.0f;
			}
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].x < -D3DX_PI)
			{// -3.14より、下回ったら
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].x += D3DX_PI * 2.0f;
			}
			// モデルの回転を修正する
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].y > D3DX_PI)
			{// +3.14より、超えてしまったら	
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].y -= D3DX_PI * 2.0f;
			}
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].y < -D3DX_PI)
			{// -3.14より、下回ったら		   
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].y += D3DX_PI * 2.0f;
			}
			// モデルの回転を修正する
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].z > D3DX_PI)
			{// +3.14より、超えてしまったら	   
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].z -= D3DX_PI * 2.0f;
			}
			if (g_aEnemy[nCntEnemy].rotMotion[nCntParts].z < -D3DX_PI)
			{// -3.14より、下回ったら		   
				g_aEnemy[nCntEnemy].rotMotion[nCntParts].z += D3DX_PI * 2.0f;
			}
		}
	}

	//----------------------------------------------
	// モーションの動作
	//----------------------------------------------
	for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
	{// パーツの数分ループ
		// 位置差分をフレーム数で割った値を加算していく
		g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x += g_aEnemy[nCntEnemy].posMotion[nCntParts].x / g_aEnemy[nCntEnemy].nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y += g_aEnemy[nCntEnemy].posMotion[nCntParts].y / g_aEnemy[nCntEnemy].nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z += g_aEnemy[nCntEnemy].posMotion[nCntParts].z / g_aEnemy[nCntEnemy].nFrame;		// 位置の差分をフレーム数で割った値を加算していく

		// 回転差分をフレーム数で割った値を加算していく
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x += g_aEnemy[nCntEnemy].rotMotion[nCntParts].x / g_aEnemy[nCntEnemy].nFrame;		// 回転の差分をフレーム数で割った値を加算していく
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y += g_aEnemy[nCntEnemy].rotMotion[nCntParts].y / g_aEnemy[nCntEnemy].nFrame;		// 回転の差分をフレーム数で割った値を加算していく
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z += g_aEnemy[nCntEnemy].rotMotion[nCntParts].z / g_aEnemy[nCntEnemy].nFrame;		// 回転の差分をフレーム数で割った値を加算していく
																		
		// モデルの回転を修正する
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x > D3DX_PI)
		{// +3.14より、超えてしまったら
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x < -D3DX_PI)
		{// -3.14より、下回ったら
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x += D3DX_PI * 2.0f;
		}
		// モデルの回転を修正する
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y > D3DX_PI)
		{// +3.14より、超えてしまったら	
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y < -D3DX_PI)
		{// -3.14より、下回ったら		   
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y += D3DX_PI * 2.0f;
		}
		// モデルの回転を修正する
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z > D3DX_PI)
		{// +3.14より、超えてしまったら	   
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z < -D3DX_PI)
		{// -3.14より、下回ったら		   
			g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z += D3DX_PI * 2.0f;
		}
	}
	g_aEnemy[nCntEnemy].nCounterFrame++;		// 現在キーフレームのフレーム数を加算
	g_aEnemy[nCntEnemy].nCounterAllFrame++;	// モーションのフレーム数を加算
}


//=============================================================================
// 敵の設置
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE EnemyType,bool bMove)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の総数分ループ

		if (g_aEnemy[nCntEnemy].bUse == false)
		{// 使われてなければ
			g_aEnemy[nCntEnemy].pos = pos;									// 位置を設定
			g_aEnemy[nCntEnemy].posOld = pos;								// 前回位置の初期設定
			g_aEnemy[nCntEnemy].InitPos = pos;								// 初期位置
			g_aEnemy[nCntEnemy].Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の移動量の初期化
			g_aEnemy[nCntEnemy].rot = rot;									// 向きの初期設定
			g_aEnemy[nCntEnemy].rotDest = rot;								// 目的の向きを初期化

			// 敵の種類＆ステータスを設定
			g_aEnemy[nCntEnemy].EnemyType = EnemyType;												// 敵のタイプを設定
			g_aEnemy[nCntEnemy].bMoved = bMove;														// 動いた状態かどうか
			g_aEnemy[nCntEnemy].fMoveSpeed = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].fMoveSpeed;	// 移動速度を設定
			g_aEnemy[nCntEnemy].fJamp = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].fJamp;			// ジャンプ力を設定
			g_aEnemy[nCntEnemy].fRadius = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].fRadius;		// 当たり判定の半径を設定
			g_aEnemy[nCntEnemy].nLife = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nLife;			// ライフを設定
			g_aEnemy[nCntEnemy].nMaxLife = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nLife;		// ライフを設定
			g_aEnemy[nCntEnemy].fJampPower = 0.0f;													// ジャンプ力を初期化
			g_aEnemy[nCntEnemy].fFlyPower = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 吹っ飛び力を初期化

			// 状態の変数の初期化
			g_aEnemy[nCntEnemy].State = ENEMY_STATE_NORMAL;				// 通常状態に初期化
			g_aEnemy[nCntEnemy].nCounterState = 0;						// ステートのカウンターを初期化

			// 行動系の変数の初期化
			g_aEnemy[nCntEnemy].nAttackWaitTime = rand() % (ENEMY_ATTACK_WAIT_MAX_TIME - ENEMY_ATTACK_WAIT_MIN_TIME + 1) + ENEMY_ATTACK_WAIT_MIN_TIME;
			g_aEnemy[nCntEnemy].nAttackCounter = 0;								// 攻撃するまでのカウンターを初期化
			g_aEnemy[nCntEnemy].nActTime = rand() % (ENEMY_ACT_MAX_TIME - ENEMY_ACT_MIN_TIME + 1) + ENEMY_ACT_MIN_TIME;		// 行動の時間の変数をランダムで設定
			g_aEnemy[nCntEnemy].nActNowCounter = 0;								// 行動のカウンター変数を初期化

			g_aEnemy[nCntEnemy].nDownCounter = 0;								// 倒れている時間を初期化
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_JAMP] = false;		// ジャンプ状態を初期化
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_LANDING] = false;	// 着地中じゃない状態に初期化
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] = false;		// ダメージ中じゃない状態に初期化
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = false;		// 吹っ飛び中じゃない状態に初期化
			g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_GETUP] = false;		// ダウン中じゃない状態に初期化

			g_aEnemy[nCntEnemy].Motionstate = ENEMY_MOTION_NEUTRAL;				// モーションをニュートラルに初期化
			g_aEnemy[nCntEnemy].MotionstateOld = ENEMY_MOTION_MAX;				// 前回のモーションをニュートラルに初期化

			// モーションの変数の初期化
			g_aEnemy[nCntEnemy].nCounterKey = 0;								// 現在のキーフレームを初期化
			g_aEnemy[nCntEnemy].nCounterFrame = 0;								// 現在のフレーム数を初期化
			g_aEnemy[nCntEnemy].nCounterAllFrame = 0;							// 現在の全フレーム数を初期化
			g_aEnemy[nCntEnemy].nFrame = 0;										// 今回のフレーム数を初期化

			// 攻撃状態を設定
			for (int nCntAttack = 0; nCntAttack < ENEMY_ATTACKTYPE_MAX; nCntAttack++)
			{// 攻撃の種類分ループ
				g_aEnemy[nCntEnemy].bEnemyAttack[nCntAttack] = false;			// 攻撃中じゃない状態に設定
				g_aEnemy[nCntEnemy].bEnemyAttackReady[nCntAttack] = false;		// 攻撃の準備状態じゃない状態に設定
			}

			// UIの設定
			if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_ARMOR)
			{// 鎧の時の設定
				//　ライフ設定
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 120, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 120, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 120, true);

			}
			else if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_HITOME)
			{// 一つ目の時の設定
				//　ライフ設定
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 100, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 100, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 100, true);


			}
			else if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_HIGECHAN)
			{// ひげちゃんの時の設定
				// 奪踊のコマンドUIの設定（オフセットの位置,中心からの距離,敵の番号,奪踊のタイプ）
				//　ライフ設定
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 50, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 50, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 50, true);
			}
			else if (g_aEnemy[nCntEnemy].EnemyType == ENEMY_TYPE_HONCHAN)
			{// 本ちゃんの時の設定
				//　ライフ設定
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBG, 100, 1, 60.0f, 9.0f, 80, false);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BILLLIFETYPE_ENEMYBGRED, 100, 1, 50.0f, 7.0f, 80, true);
				SetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BILLLIFETYPE_ENEMY, 100, 1, 50.0f, 7.0f, 80, true);
			}

			// 影を設定
			g_aEnemy[nCntEnemy].nIdyShadow = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), g_aEnemy[nCntEnemy].rot,g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius);

			//----------------------------------------------
			// 攻撃の当たり判定の設定
			//----------------------------------------------
			for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
			{// キャラクターの攻撃の当たり判定の総数分ループ
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nIdxParent;		// 親パーツの番号の設定
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].pos;				// オフセットの設定
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].fRadius = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].fRadius;			// 半径の設定
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionStart = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nCollisionStart;	// 判定が始まるフレームの設定
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nCollisionEnd = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nCollisionEnd;		// 判定が終わるフレームの設定
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nMotionType = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionAttack[nCntCollisionAttack].nMotionType;		// 判定がつくのモーションの設定
				g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse = true;																								// 当たり判定を使用状態に設定
			}

			//----------------------------------------------
			// 体の当たり判定の設定
			//----------------------------------------------
			for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
			{// キャラクターの体の当たり判定の総数分ループ
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionBody[nCntCollisionBody].nIdxParent;		// 親パーツの番号の設定
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionBody[nCntCollisionBody].pos;				// オフセットの設定
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].CollisionBody[nCntCollisionBody].fRadius;			// 半径の設定
				g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse = true;																								// 当たり判定を使用状態に設定
			}

			g_aEnemy[nCntEnemy].bUse = true;		// 使われている状態に設定
			g_nNumEnemy++;							// 敵の人数を増やす

			//--------------------------
			// 初期ポーズを設定
			//--------------------------
			for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
			{// パーツの数分ループ
				// モデルの位置をニュートラルモーションに初期化
				g_aEnemy[nCntEnemy].aModel[nCntParts].pos = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].ModelInfo[nCntParts].pos + g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[0].pos[nCntParts];
				// モデルの回転をニュートラルモーションに初期化
				g_aEnemy[nCntEnemy].aModel[nCntParts].rot = g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[g_aEnemy[nCntEnemy].Motionstate].Motion[0].rot[nCntParts];
			}

			//------------------------------------------------------------------
			// ワールドマトリクスの設定
			//------------------------------------------------------------------
			LPDIRECT3DDEVICE9	pDevice = GetDevice();		// デバイスの初期化
			D3DXMATRIX			mtxParent;					// 階層構造用のマトリックス
			D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス

			 // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// 回転(向き)を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,g_aEnemy[nCntEnemy].rot.y,g_aEnemy[nCntEnemy].rot.x,g_aEnemy[nCntEnemy].rot.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,&g_aEnemy[nCntEnemy].mtxWorld,&mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y,g_aEnemy[nCntEnemy].pos.z);

			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld,&g_aEnemy[nCntEnemy].mtxWorld,&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			for (int nCntParts = 0; nCntParts < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].MaxParts; nCntParts++)
			{// パーツの数分ループ

				if (g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel == -1)
				{// 親モデルがいないとき
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;				// 敵のマトリックスを代入
				}
				else
				{// 親のモデルがいるとき
					mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nIdxParentModel].mtxWorld;		// 親のマトリックスを代入
				}

				//---------------------------
				// モデルのワールドマトリクスの設定
				//---------------------------
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);
				// 回転(向き)を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y,g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x,g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);
				// 回転(向き)を設定
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,	&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,&mtxRot);
				// 移動を反映
				D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x,g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y,g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);
				// 移動を設定
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,	&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,&mtxTrans);
				// 親のワールドマトリックスを反映
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,	&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld,&mtxParent);
				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				//---------------------------------
				// 攻撃の当たり判定
				//---------------------------------
				for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
				{// 当たり判定の数分ループ

					if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse == true)
					{// 攻撃の当たり判定が使われている状態なら処理する

						if (g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent == -1)
						{// 親モデルがいないとき
							mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// 敵のワールドマトリックスを代入
						}
						else
						{// 親モデルがいるとき
							mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].nIdxParent].mtxWorld;		// 親モデルのマトリックスを代入
						}

						// ワールドマトリックスの初期化
						D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);
						// 移動を反映
						D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.x,	g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.y,g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].pos.z);
						// 移動を設定
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&mtxTrans);
						// 親のワールドマトリックスを反映
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld,&mtxParent);
						// ワールドマトリックスの設定
						pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].mtxWorld);
					}
				}

				//---------------------------------
				// 体の当たり判定
				//---------------------------------
				for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
				{// 当たり判定の数分ループ

					if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
					{// 体の当たり判定が使われている状態なら処理する
						if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent == -1)
						{// 親モデルがいないとき
							mtxParent = g_aEnemy[nCntEnemy].mtxWorld;		// 敵のワールドマトリックスを代入
						}
						else
						{// 親モデルがいるとき
							mtxParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].nIdxParent].mtxWorld;		// 親モデルのマトリックスを代入
						}

						// ワールドマトリックスの初期化
						D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);
						// 移動を反映
						D3DXMatrixTranslation(&mtxTrans,g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.x,g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.y,g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].pos.z);
						// 移動を設定、２番目と３番目の引数を掛け算された値を一番目の引数に代入している
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,&mtxTrans);
						// 親のワールドマトリックスを反映
						D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,	&g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld,&mtxParent);
						// ワールドマトリックスの設定
						pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld);
					}
				}
			}
			break;
		}

	}
}

//=============================================================================
// 敵が重ならないための当たり判定
//=============================================================================
void CollisionEnemy(D3DXVECTOR3 *pPos, float fRadius)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の数分ループ
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 敵が使われているとき
			if (g_aEnemy[nCntEnemy].pos.x != pPos->x &&
				g_aEnemy[nCntEnemy].pos.y != pPos->y &&
				g_aEnemy[nCntEnemy].pos.z != pPos->z)
			{// 敵とポインタの位置が一緒じゃなければ(同じキャラ同士)

				// 当たり判定の座標の距離の計算
				float posX = (g_aEnemy[nCntEnemy].pos.x - pPos->x);					// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
				float posY = (g_aEnemy[nCntEnemy].pos.y - pPos->y);					// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
				float posZ = (g_aEnemy[nCntEnemy].pos.z - pPos->z);					// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
				float fPosLenght = (posX * posX) + (posY * posY) + (posZ * posZ);	// それぞれを2乗して距離を計算

				// 当たり判定の半径の距離の計算
				float fRadiusLenght = (g_aEnemy[nCntEnemy].fRadius + fRadius) * (g_aEnemy[nCntEnemy].fRadius + fRadius);			// (ボックスの半径－攻撃の半径)を2乗して距離を計算

																																// 座標の距離と半径の距離の比較
				if (fPosLenght  < fRadiusLenght)
				{// 半径の距離より、座標の距離のほうが小さかったら
					float fPosAngle = atan2f(g_aEnemy[nCntEnemy].pos.x - pPos->x,		// 敵とキャラの位置の角度を計算
											 g_aEnemy[nCntEnemy].pos.z - pPos->z);

					g_aEnemy[nCntEnemy].pos.x = pPos->x + (sinf(fPosAngle)*(g_aEnemy[nCntEnemy].fRadius + fRadius));	// キャラの位置をずらす
					g_aEnemy[nCntEnemy].pos.z = pPos->z + (cosf(fPosAngle)*(g_aEnemy[nCntEnemy].fRadius + fRadius));	// キャラの位置をずらす

				}
			}
		}
	}
}

//=============================================================================
// 敵の体の当たり判定
//=============================================================================
void CollisionEnemyBody(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, int nPower, float fDamageSide, float fDamageVertical)
{
	Player *pPlayer = GetPlayer();	// プレイヤーの情報を取得

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の数分ループ

		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 敵が使われているとき

			if (g_aEnemy[nCntEnemy].State == ENEMY_STATE_NORMAL)
			{// 通常状態なら

				for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
				{// 当たり判定の数分ループ

					if (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse == true)
					{// 当たり判定が使われているとき

						// 当たり判定の座標の距離の計算
						float posX = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41 - mtxWorld.x);				// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
						float posY = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42 - mtxWorld.y);				// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
						float posZ = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43 - mtxWorld.z);				// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
						float fPosLenght = (posX * posX) + (posY * posY) + (posZ * posZ);											// それぞれを2乗して距離を計算

						// 当たり判定の半径の距離の計算
						float fRadiusLenght = (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius + fRadius) *
											  (g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].fRadius + fRadius);			// (ボックスの半径+攻撃の半径)を2乗して距離を計算

						// 座標の距離と半径の距離の比較
						if (fPosLenght  < fRadiusLenght)
						{// 半径の距離より、座標の距離のほうが小さかったら
							float fAttackAngle = atan2f(pos.x - g_aEnemy[nCntEnemy].pos.x,		// 当たった角度を計算
														pos.z - g_aEnemy[nCntEnemy].pos.z);

							// ライフ減少
							g_aEnemy[nCntEnemy].nLife -= nPower;

							GetBillLife(nCntEnemy, g_aEnemy[nCntEnemy].nLife);	// 連動

							AddHit(1);	// ヒット

				


							for (int nCntEffect = 0; nCntEffect < 11; nCntEffect++)
							{// 火花
								float nDirection = SetRandomEffect();
								float nDirectio2 = SetRandomEffect();
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
									D3DXVECTOR3(sinf(nDirection) * (float(rand() % 50 / 10.0f) + 3), cosf(nDirection) * (float(rand() % 50 / 10.0f) + 3), cosf(nDirectio2) * (float(rand() % 30 / 10.0f) + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(1.0f, float(rand() % 100 / 100.0f), 0.0f, 0.8f), float(rand() % 20) + 10, 30, EFFECTTYPE_HITEXPLOSION);
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
									D3DXVECTOR3(sinf(nDirection) *(float(rand() % 50 / 10.0f) + 3), cosf(nDirectio2) * (float(rand() % 30 / 10.0f) + 3), cosf(nDirection) * (float(rand() % 50 / 10.0f) + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(0.8f, float(rand() % 100 / 100.0f), 0.0f, 0.8f), float(rand() % 30) + 10, 30, EFFECTTYPE_HITEXPLOSION);
							}
							for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
							{// 衝撃は
								SetEffect(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._41, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._42, g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].mtxWorld._43),
									D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXCOLOR(0.8f, 0.5f, 0.0f, 0.8f), 10, 30, EFFECTTYPE_IMPACT);
							}
								// 敵が死んだかどうか
								if (g_aEnemy[nCntEnemy].nLife <= 0)
								{// ライフが0以下になったら

									// 敵の状態を設定
									g_aEnemy[nCntEnemy].State = ENEMY_STATE_DEATH;				// 死んだ状態にする
									g_aEnemy[nCntEnemy].nCounterState = ENEMY_DEATH_COUNTER;		// カウンターを設定する

									g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = true;								// 吹っ飛び中に設定

								}
								else
								{// ライフが残っているとき

									// 敵の状態を設定
									g_aEnemy[nCntEnemy].State = ENEMY_STATE_DAMAGE;				// ダメージ状態にする
									g_aEnemy[nCntEnemy].nCounterState = ENEMY_DAMAGE_COUNTER;	// カウンターを設定する

								}

								if (fDamageSide > 0.0f || fDamageVertical > 0.0f)
								{// 吹っ飛び力があるとき

									// 水平方向の吹っ飛び力を代入
									g_aEnemy[nCntEnemy].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
									g_aEnemy[nCntEnemy].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;

									// 垂直方向の吹っ飛び力を代入
									g_aEnemy[nCntEnemy].fFlyPower.y = fDamageVertical;

									g_aEnemy[nCntEnemy].nFlyCounter = 10;

									g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_FLY] = true;												// 吹っ飛び中に設定

									g_aEnemy[nCntEnemy].rotDest.y = fAttackAngle - D3DX_PI;								// キャラクターの向きを変更

									// 攻撃された吹っ飛び力を代入
									g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[ENEMY_MOTION_FLY].fDamageSide = fDamageSide;							// 吹っ飛び力を上書きする
									g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].Motion[ENEMY_MOTION_FLY].fDamageVertical = fDamageVertical;					// 吹っ飛び力を上書きする

									// 効果音再生
									PlaySound(SOUND_LABEL_SE_HIT_0);

								}
								else
								{// 吹っ飛び力がないとき
									g_aEnemy[nCntEnemy].bActionFlag[ENEMY_BOOLTYPE_DAMAGE] = true;											// ダメージ中に設定

									// 効果音再生
									PlaySound(SOUND_LABEL_SE_HIT_1);
								}

								break;																					// ほかの当たり判定は処理しなくする
						}
					}
				}
			}
		}
	}
}


//=============================================================================
// 敵の削除
//=============================================================================
void DeletEnemy(int nCntEnemy, bool bKo)
{
	if (bKo == true)
	{// 敵を倒したなら

		// ライフゲージの削除
		DeleteBillLife(nCntEnemy);
		DeleteShadow(g_aEnemy[nCntEnemy].nIdyShadow);				// 影を削除

		// エフェクト
		for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
		{
			float fRandRadius = float(rand() % 60 + 30);
			float nDirection = SetRandomEffect();
			float nDirection2 = SetRandomEffect();
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection) * 7, cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) * 7), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 20, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 7, cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) * 7), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 20, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 3, cosf(nDirection2) * 3, cosf(nDirection) * 3), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 3, sinf(nDirection) * 3, cosf(nDirection) * 3), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 2, cosf(nDirection2) * 1, cosf(nDirection) * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 30, 30, EFFECTTYPE_DEATH);
			SetEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(nDirection2) * 2, sinf(nDirection) * 1, cosf(nDirection) * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 30, 30, EFFECTTYPE_DEATH);
		}
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		// アイテムを出現させる
		SetItem(D3DXVECTOR3(g_aEnemy[nCntEnemy].CollisionBody[0].mtxWorld._41,				// 設置する位置
			g_aEnemy[nCntEnemy].CollisionBody[0].mtxWorld._42,				// 設置する位置
			g_aEnemy[nCntEnemy].CollisionBody[0].mtxWorld._43),				// 設置する位置
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),												// 移動量
			ITEM_KOUTOKU);													// 設置する種類


		// 敵の総数を減らす
		g_nNumEnemy--;

		g_aEnemy[nCntEnemy].bUse = false;							// 敵を使用していない状態にする

		// 効果音再生
		PlaySound(SOUND_LABEL_SE_HIT_1);
	}
	else
	{// 倒したわけじゃないなら
		// ライフゲージの削除
		DeleteBillLife(nCntEnemy);

		DeleteShadow(g_aEnemy[nCntEnemy].nIdyShadow);				// 影を削除

		//----------------------------------------------
		// 攻撃の当たり判定の削除
		//----------------------------------------------
		for (int nCntCollisionAttack = 0; nCntCollisionAttack < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionAttack; nCntCollisionAttack++)
		{// キャラクターの攻撃の当たり判定の総数分ループ
			g_aEnemy[nCntEnemy].CollisionAttack[nCntCollisionAttack].bUse = false;	// 当たり判定を使用していない状態にする
		}

		//----------------------------------------------
		// 体の当たり判定の削除
		//----------------------------------------------
		for (int nCntCollisionBody = 0; nCntCollisionBody < g_EnemyInfo[g_aEnemy[nCntEnemy].EnemyType].nCntMaxCollisionBody; nCntCollisionBody++)
		{// キャラクターの体の当たり判定の総数分ループ
			g_aEnemy[nCntEnemy].CollisionBody[nCntCollisionBody].bUse = false;				// 当たり判定を使用していない状態にする
		}

		// 敵の総数を減らす
		g_nNumEnemy--;

		g_aEnemy[nCntEnemy].bUse = false;							// 敵を使用していない状態にする
	}
	
}

//=============================================================================
// 敵の情報のロード
//=============================================================================
void EnemyLode(void)
{
	FILE *pFile = NULL;									// ファイルのポインタ変数
	char acLine[MAX_CHAR];							// 1行丸ごと記憶するための変数
	char acData[MAX_CHAR];							// 一部の分を記憶するための変数

	//--------------------------------------------------------------------------
	// モデルのアドレス＆モーション情報の読み込み
	//--------------------------------------------------------------------------
	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// 敵の種類分ループ

		// 変数宣言&初期化
		int nCntModel		  = 0;								// 敵のパーツ数
		int nCntModelIdx	  = 0;								// パーツのインデックス
		int nCntMotion		  = 0;								// モーションの数
		int nCntParts;		  									// パーツのカウント変数
		int nCntKey			  = 0;								// キーフレームのカウント変数
		int nCntCollisionAttack	  = 0;								// 攻撃の当たり判定のカウント変数
		int nCntCollisionBody = 0;								// 体の当たり判定のカウント変数
		g_EnemyInfo[nCntEnemyType].nCntMaxCollisionAttack = 0;	// 攻撃の当たり判定の最大数を初期化
		g_EnemyInfo[nCntEnemyType].nCntMaxCollisionBody	  = 0;	// 体の当たり判定の最大数を初期化

		// ファイルを開く
		if (nCntEnemyType == ENEMY_TYPE_ARMOR)
		{// 鎧の読み込み
			pFile = fopen("data/TEXT/motion_armor.txt", "r");
		}
		else if (nCntEnemyType == ENEMY_TYPE_HITOME)
		{// 一つ目の読み込み
			pFile = fopen("data/TEXT/motion_hitome.txt", "r");
		}
		else if (nCntEnemyType == ENEMY_TYPE_HIGECHAN)
		{// ひげちゃんの読み込み
			pFile = fopen("data/TEXT/motion_higechan.txt", "r");
		}
		else if (nCntEnemyType == ENEMY_TYPE_HONCHAN)
		{// 本ちゃんの読み込み
			pFile = fopen("data/TEXT/motion_honchan.txt", "r");
		}


		//-----------------------------------------------------------------------------
		// 敵のモデル・モーションの読み込み
		//-----------------------------------------------------------------------------
		if (pFile != NULL)
		{// ヌルチェック

			while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
			{
				sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

				if (strcmp(acData, "NUM_MODEL") == 0)
				{// モデルの最大数があったら
					sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].MaxModel);		// モデルの最大数を取得
				}
				//----------------------------------------
				// モデルのアドレスの取得
				//----------------------------------------
				if (strcmp(acData, "MODEL_FILENAME") == 0)
				{// ファイルのアドレスがあったら
					sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].cIP[0]);		// モデルのアドレスを取得				
					nCntModel++;
				}

				//----------------------------------------
				// キャラのオフセットの取得
				//----------------------------------------
				if (strcmp(acData, "CHARACTERSET") == 0)
				{// キャラセットの文字列があったら
					while (strcmp(acData, "END_CHARACTERSET") != 0)
					{// END_CHARACTERSETが見つかるまでループする
						fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
						sscanf(&acLine[0], "%s", &acData);					// データをコピー

						if (strcmp(acData, "NUM_PARTS") == 0)
						{// NUM_PARTSの文字列があったら
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].MaxParts);		// パーツ数を取得
						}
						else if (strcmp(acData, "MOVE") == 0)
						{// MOVEの文字列があったら
							sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].fMoveSpeed);		// 移動速度を取得
						}
						else if (strcmp(acData, "JUMP") == 0)
						{// JUMPの文字列があったら
							sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].fJamp);		// ジャンプ力を取得
						}
						else if (strcmp(acData, "RADIUS") == 0)
						{// RADIUSの文字列があったら
							sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].fRadius);		// 半径を取得
						}
						else if (strcmp(acData, "LIFE") == 0)
						{// LIFEの文字列があったら
							sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].nLife);			// 体力を取得
						}
						else if (strcmp(acData, "PARTSSET") == 0)
						{// PARTSSETの文字列があったら
							while (strcmp(acData, "END_PARTSSET") != 0)
							{// END_PARTSSETが見つかるまでループする
								fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
								sscanf(&acLine[0], "%s", &acData);					// データをコピー

								if (strcmp(acData, "INDEX") == 0)
								{// INDEXがあったら
									sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].nIdxModel);	// インデックスを取得
								}
								else if (strcmp(acData, "PARENT") == 0)
								{// PARENTがあったら
									sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].nIdxParentModel);	// モデルの親のインデックスを取得
								}
								else if (strcmp(acData, "POS") == 0)
								{// POSがあったら
									sscanf(&acLine[0],
										"%s %s %f %f %f",
										&acData[0],
										&acData[0],
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].pos.x,	// オフセットの位置を取得
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].pos.y,	// オフセットの位置を取得
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].pos.z);	// オフセットの位置を取得
								}
								else if (strcmp(acData, "ROT") == 0)
								{// ROTがあったら
									sscanf(&acLine[0],
										"%s %s %f %f %f",
										&acData[0],
										&acData[0],
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].rot.x,	// オフセットの位置を取得
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].rot.y,	// オフセットの位置を取得
										&g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModelIdx].rot.z);	// オフセットの位置を取得
								}


							}
							nCntModelIdx++;			// モデルのカウントを進める
						}
						if (strcmp(acData, "BODY_COLLISION") == 0)
						{// COLLISIONがあったら
							sscanf(&acLine[0],
								"%s %s %hhd %f %f %f %f",
								&acData[0],
								&acData[0],
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].nIdxParent,			// オフセットの親パーツのインデックスを取得
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].pos.x,					// オフセットの位置X座標
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].pos.y,					// オフセットの位置Y座標
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].pos.z,					// オフセットの位置Z座標
								&g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].fRadius); 				// 半径
							g_EnemyInfo[nCntEnemyType].CollisionBody[nCntCollisionBody].bUse = true;				// 当たり判定をtrueにする
							g_EnemyInfo[nCntEnemyType].nCntMaxCollisionBody++;						// 体の当たり判定の総数をカウントする
							nCntCollisionBody++;														// カウント変数を進める
						}

					}
				}

				//----------------------------------------
				// キャラのモーションの取得
				//----------------------------------------
				if (nCntMotion < ENEMY_MOTION_MAX)
				{// モーションの総数以上は設定しない
					if (strcmp(acData, "MOTIONSET") == 0)
					{// MOTIONSETの文字列があったら
						nCntKey = 0;
						while (strcmp(acData, "END_MOTIONSET") != 0)
						{// END_MOTIONSETが見つかるまでループする
							fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
							sscanf(&acLine[0], "%s", &acData);					// データをコピー

							if (strcmp(acData, "LOOP") == 0)
							{// LOOPがあったら
								sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].nLoop);	// ループするかどうかを取得

							}
							else if (strcmp(acData, "NUM_KEY") == 0)
							{// NUM_KEYがあったら
								sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].nNumKey);	// キー数を取得
							}
							else if (strcmp(acData, "COLLISION") == 0)
							{// COLLISIONがあったら
								sscanf(&acLine[0],
									"%s %s %hhd %f %f %f %f %hd %hd",
									&acData[0],
									&acData[0],
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nIdxParent,				// オフセットの親パーツのインデックスを取得
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].pos.x,					// オフセットの位置X座標
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].pos.y,					// オフセットの位置Y座標
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].pos.z,					// オフセットの位置Z座標
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].fRadius, 				// 半径
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nCollisionStart, 		// 判定開始のフレーム数
									&g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nCollisionEnd);			// 判定終了のフレーム数
								g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].nMotionType = nCntMotion;		// 判定がつくモーションを設定
								g_EnemyInfo[nCntEnemyType].CollisionAttack[nCntCollisionAttack].bUse = true;					// 当たり判定をtrueにする

								g_EnemyInfo[nCntEnemyType].nCntMaxCollisionAttack++;									// 攻撃の当たり判定の総数をカウントする
								nCntCollisionAttack++;																		// カウント変数を進める
							}
							else if (strcmp(acData, "COLLISION_DAMAGE") == 0)
							{// COLLISIONがあったら
								sscanf(&acLine[0],
									"%s %s %f %f %hhd",
									&acData[0],
									&acData[0],
									&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].fDamageSide,
									&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].fDamageVertical,
									&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].nAttackPower);								// モーションの攻撃力を設定
							}

							else if (strcmp(acData, "KEYSET") == 0)
							{// KEYSETがあったら
								nCntParts = 0;						// パーツのカウント変数を初期化
								while (strcmp(acData, "END_KEYSET") != 0)
								{// END_KEYSETがあるまでループ
									fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
									sscanf(&acLine[0], "%s", &acData);					// データをコピー

									if (strcmp(acData, "FRAME") == 0)
									{// FRAMEがあったら
										sscanf(&acLine[0], "%s %s %hhd", &acData[0], &acData[0], &g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].nFrame);	// キーのフレーム数を取得
									}
									else if (strcmp(acData, "KEY") == 0)
									{// KEYがあったら
										while (strcmp(acData, "END_KEY") != 0)
										{// END_KEYがあるまでループ
											fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
											sscanf(&acLine[0], "%s", &acData);					// データをコピー

											if (strcmp(acData, "POS") == 0)
											{// POSがあったら
												sscanf(&acLine[0],
													"%s %s %f %f %f",
													&acData[0],
													&acData[0],
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].pos[nCntParts].x,		// 各パーツのモーションの位置を取得
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].pos[nCntParts].y,		// 各パーツのモーションの位置を取得
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].pos[nCntParts].z);		// 各パーツのモーションの位置を取得
											}
											else if (strcmp(acData, "ROT") == 0)
											{// ROTがあったら
												sscanf(&acLine[0],
													"%s %s %f %f %f",
													&acData[0],
													&acData[0],
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].rot[nCntParts].x,		// 各パーツのモーションの回転を取得
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].rot[nCntParts].y,		// 各パーツのモーションの回転を取得
													&g_EnemyInfo[nCntEnemyType].Motion[nCntMotion].Motion[nCntKey].rot[nCntParts].z);		// 各パーツのモーションの回転を取得
											}

										}
										nCntParts++;			// パーツのカウントを進める
									}
								}
								nCntKey++;						// キーフレームを進める
							}
						}
						nCntMotion++;								// モーションを変える
					}
				}
			}
			fclose(pFile);
		}
	}


	//-------------------------------------------------------------------------------
	// Xファイルの読み込み
	//-------------------------------------------------------------------------------

	LPDIRECT3DDEVICE9 pDevice = GetDevice();					// デバイスの取得
	D3DXMATERIAL *pMat;											// マテリアルデータへのポインタ

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// 敵の種類分ループ
		for (int nCntModel = 0; nCntModel < g_EnemyInfo[nCntEnemyType].MaxModel; nCntModel++)
		{// 敵のモデルの数分ループ
			D3DXLoadMeshFromX(g_EnemyInfo[nCntEnemyType].ModelAddress[g_EnemyInfo[nCntEnemyType].ModelInfo[nCntModel].nIdxModel].cIP,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat,
				NULL,
				&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat,
				&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh);


			//-------------------------------------------------------------------------------
			// テクスチャの読み込み
			//-------------------------------------------------------------------------------

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pBuffMat->GetBufferPointer();

			// マテリアル分容量を増やす
			g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat];

			for (int nCntMat = 0; nCntMat < (int)g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].nNumMat; nCntMat++)
			{// マテリアルの数分ループ
				if (pMat[nCntMat].pTextureFilename != NULL)
				{// テクスチャのアドレスが存在していたら
					D3DXCreateTextureFromFile(pDevice,												// 3Dデバイスのアドレス
						pMat[nCntMat].pTextureFilename,												// 読み込むテクスチャのファイルのアドレス
						&g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntMat]);		// テクスチャのポインタにアドレスに設定する
				}
				else
				{// テクスチャのアドレスがないなら
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pTexture[nCntMat] = NULL;	// NULLに設定する
				}
			}
		}
	}

	//-------------------------------------------------------------------------------
	// モデルのMin&Maxの取得
	//-------------------------------------------------------------------------------

	// 変数宣言
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMY_TYPE_MAX; nCntEnemyType++)
	{// 敵の種類分ループ

		for (int nCntModel = 0; nCntModel < g_EnemyInfo[nCntEnemyType].MaxModel; nCntModel++)
		{// モデルの数分ループ

			// 頂点数を取得
			nNumVtx = g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->GetNumVertices();

			// 頂点フォーマットを取得
			sizeFVF = D3DXGetFVFVertexSize(g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->GetFVF());

			// 頂点バッファをロック
			g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			// 敵の最小値と最大値を比較
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// 頂点数分ループ
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

																// 最小値と比較
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.x > vtx.x)
				{// 今持っている最小値のxよりも小さい値だったら
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.x = vtx.x;						// 最小値のxを代入
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.y > vtx.y)
				{// 今持っている最小値のyよりも小さい値だったら
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.y = vtx.y;						// 最小値のyを代入
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.z > vtx.z)
				{// 今持っている最小値のyよりも小さい値だったら
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMin.z = vtx.z;						// 最小値のzを代入
				}

				// 最大値と比較
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.x < vtx.x)
				{// 今持っている最大値のxよりも大きい値だったら
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.x = vtx.x;						// 最大値のxを代入
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.y < vtx.y)
				{// 今持っている最大値のxよりも大きい値だったら
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.y = vtx.y;						// 最大値のyを代入
				}
				if (g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.z < vtx.z)
				{// 今持っている最大値のxよりも大きい値だったら
					g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].vtxMax.z = vtx.z;						// 最大値のzを代入
				}

				pVtxBuff += sizeFVF;							// ポインタをサイズ分進める
			}

			// 頂点バッファをアンロック
			g_EnemyInfo[nCntEnemyType].ModelAddress[nCntModel].pMesh->UnlockVertexBuffer();
		}
	}

}

//=============================================================================
// 敵の配置情報の読み込み
//=============================================================================
void EnemySetLode(void)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ変数
	char acLine[MAX_CHAR];			// 1行丸ごと記憶するための変数
	char acData[MAX_CHAR];			// 一部の分を記憶するための変数

	D3DXVECTOR3	pos;				// 位置を入れる変数
	D3DXVECTOR3 rot;				// 向きを入れる変数
	int			nType;				// 敵の種類を入れる変数
	ENEMY_TYPE	EnemyType;			// 敵の種類を入れる変数 

	// ファイルを開く
	if (GetMode() == MODE_GAME)
	{// ゲームの時に開くファイル
		pFile = fopen("data/TEXT/Game_SetEnemy.txt", "r");
	}
	else if (GetMode() == MODE_TITLE)
	{// タイトルの時に開くファイル
		pFile = fopen("data/TEXT/Title_SetEnemy.txt", "r");
	}

	if (pFile != NULL)
	{// ヌルチェック

		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{// テキストファイルの一番下の行が来るまでループ
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (strcmp(acData, "ENEMYSET") == 0)
			{// KEYSETがあったら
				EnemyType = ENEMY_TYPE_ARMOR;				// 敵のタイプを初期化

				while (strcmp(acData, "END_ENEMYSET") != 0)
				{// END_KEYSETがあるまでループ
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー

					if (strcmp(acData, "POS") == 0)
					{// POSがあったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// 位置を取得
					}
					else if (strcmp(acData, "ROT") == 0)
					{// ROTがあったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// 向きを取得
						rot.x = (D3DX_PI * 2 / 360 * rot.x);													// 3.14を2倍して360度で割って1度分の値を出して、テキストの角度にする
						rot.y = (D3DX_PI * 2 / 360 * rot.y);													// 3.14を2倍して360度で割って1度分の値を出して、テキストの角度にする
						rot.z = (D3DX_PI * 2 / 360 * rot.z);													// 3.14を2倍して360度で割って1度分の値を出して、テキストの角度にする
					}
					else if (strcmp(acData, "TYPE") == 0)
					{// ROTがあったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nType);		// 種類を取得
						if (nType == 0)
						{// タイプが鎧だったら
							EnemyType = ENEMY_TYPE_ARMOR;
						}
						else if (nType == 1)
						{// タイプが一つ目だったら
							EnemyType = ENEMY_TYPE_HITOME;
						}
						else if (nType == 2)
						{// タイプがひげちゃんだったら
							EnemyType = ENEMY_TYPE_HIGECHAN;
						}
						else if (nType == 3)
						{// タイプが本チャンだったら
							EnemyType = ENEMY_TYPE_HONCHAN;
						}

					}

				}

				// 敵の設置
				SetEnemy(pos, rot, EnemyType, false);

			}
		}
		fclose(pFile);
	}

}

//=============================================================================
// 敵情報の取得
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// 敵の人数を取得
//=============================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}