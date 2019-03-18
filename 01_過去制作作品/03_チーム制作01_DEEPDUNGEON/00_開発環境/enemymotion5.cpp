//=============================================================================
//
// アニメーションの処理 [motion.cpp]
// Author : 
//
//=============================================================================
#include "enemymotion5.h"
#include "enemytext5.h"
#include "enemy5.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define MOVE_PARTS (15)			// アニメーションで動くパーツ数

//------------------------------------------------------------------------
// グローバル変数宣言                                                      |
//------------------------------------------------------------------------
EnemyTex_5 *g_aEnemyTex;					// テキストデータの取得
ENEMY_5 *pEnemy;				// エネミーデータの取得
EnemyMotion_5 g_EnemyMotion;
int g_CntEnemyMotion_5 = 0;

//------------------------------------------------------------------------
// 初期化処理															  |
//------------------------------------------------------------------------
void InitEnemyMotion_5(void)
{
	int nNumEnemyMotion = 0;

	// ファイルデータの取得
	g_aEnemyTex = GetEnemyTex_5();

	// エネミーの取得
	pEnemy = GetEnemy_5();

	// 初期設定
	g_EnemyMotion.nNumEnemymotion = 0;
	g_EnemyMotion.nCntFlame = 0;
	g_EnemyMotion.KeyFlame = 0;
	g_EnemyMotion.MAX_FLAME = g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][0].MAX_FLAME;
	g_EnemyMotion.Loop = g_aEnemyTex->LOOP[g_EnemyMotion.nNumEnemymotion];		// ループするかどうか[0:ループしない / 1:ループする]
	for (int nCntParts = 0; nCntParts > MOVE_PARTS; nCntParts++)
	{// アニメーションするパーツの情報を取得
		g_EnemyMotion.aParts[nCntParts].Pos = D3DXVECTOR3(pEnemy->aModel[nCntParts].pos.x, pEnemy->aModel[nCntParts].pos.y, pEnemy->aModel[nCntParts].pos.z);
		g_EnemyMotion.aParts[nCntParts].fPosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_EnemyMotion.aParts[nCntParts].Rot = D3DXVECTOR3(pEnemy->aModel[nCntParts].rot.x, pEnemy->aModel[nCntParts].rot.y, pEnemy->aModel[nCntParts].rot.z);
		g_EnemyMotion.aParts[nCntParts].fRotAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//------------------------------------------------------------------------
// 更新処理																  |
//------------------------------------------------------------------------
void UpdateEnemyMotion_5(void)
{
	g_CntEnemyMotion_5++;

	if (g_CntEnemyMotion_5 % 1 == 0)
	{
		// エネミーの状態を取得
		ENEMYSTATUS_5 *pEnemyState;
		pEnemyState = GetEnemyStatus_5();

		// ファイルデータの取得
		g_aEnemyTex = GetEnemyTex_5();

		// エネミーの取得
		pEnemy = GetEnemy_5();

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 回転の差分をだして移動する
		 // テキストデータから回転情報を取得する
			g_EnemyMotion.aParts[nCntMotion].RotDest = D3DXVECTOR3(g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][nCntMotion].MotionRotX,
				g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][nCntMotion].MotionRotY,
				g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][nCntMotion].MotionRotZ);
			// 差分を計算する
			g_EnemyMotion.aParts[nCntMotion].fRotAngle = g_EnemyMotion.aParts[nCntMotion].RotDest - g_EnemyMotion.aParts[nCntMotion].Rot;		// 差分 = 目的 - 現在

			if (g_EnemyMotion.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyMotion.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyMotion.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyMotion.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyMotion.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyMotion.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
			}

			pEnemy->aModel[nCntMotion].rot += g_EnemyMotion.aParts[nCntMotion].fRotAngle / (g_EnemyMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemy->aModel[nCntMotion].rot >= g_EnemyMotion.aParts[nCntMotion].fRotAngle)
			{
				pEnemy->aModel[nCntMotion].rot += g_EnemyMotion.aParts[nCntMotion].fRotAngle / (g_EnemyMotion.MAX_FLAME * 1.0f);
			}
			if (pEnemy->aModel[nCntMotion].rot <= g_EnemyMotion.aParts[nCntMotion].fRotAngle)
			{
				pEnemy->aModel[nCntMotion].rot += g_EnemyMotion.aParts[nCntMotion].fRotAngle / (g_EnemyMotion.MAX_FLAME * 1.0f);
			}

			g_EnemyMotion.aParts[nCntMotion].Rot = pEnemy->aModel[nCntMotion].rot;		// 現在の更新
		}

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 座標の差分をだして移動する
		 // テキストデータから座標情報を取得する
			g_EnemyMotion.aParts[nCntMotion].PosDest = D3DXVECTOR3(g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][nCntMotion].MotionPosX,
				g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][nCntMotion].MotionPosY,
				g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][nCntMotion].MotionPosZ);
			// 差分を計算する
			g_EnemyMotion.aParts[nCntMotion].fPosAngle.x = g_EnemyMotion.aParts[nCntMotion].PosDest.x - g_EnemyMotion.aParts[nCntMotion].Pos.x + g_aEnemyTex->aModelTex[nCntMotion].PosX;		// 差分 = 目的 - 現在
			g_EnemyMotion.aParts[nCntMotion].fPosAngle.y = g_EnemyMotion.aParts[nCntMotion].PosDest.y - g_EnemyMotion.aParts[nCntMotion].Pos.y + g_aEnemyTex->aModelTex[nCntMotion].PosY;
			g_EnemyMotion.aParts[nCntMotion].fPosAngle.z = g_EnemyMotion.aParts[nCntMotion].PosDest.z - g_EnemyMotion.aParts[nCntMotion].Pos.z + g_aEnemyTex->aModelTex[nCntMotion].PosZ;

			if (g_EnemyMotion.aParts[nCntMotion].fPosAngle.x > D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fPosAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyMotion.aParts[nCntMotion].fPosAngle.x < -D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fPosAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyMotion.aParts[nCntMotion].fPosAngle.y > D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fPosAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyMotion.aParts[nCntMotion].fPosAngle.y < -D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fPosAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyMotion.aParts[nCntMotion].fPosAngle.z > D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fPosAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyMotion.aParts[nCntMotion].fPosAngle.z < -D3DX_PI)
			{
				g_EnemyMotion.aParts[nCntMotion].fPosAngle.z += D3DX_PI * 2;
			}

			pEnemy->aModel[nCntMotion].pos += g_EnemyMotion.aParts[nCntMotion].fPosAngle / (g_EnemyMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemy->aModel[nCntMotion].pos >= g_EnemyMotion.aParts[nCntMotion].fPosAngle)
			{
				pEnemy->aModel[nCntMotion].pos += g_EnemyMotion.aParts[nCntMotion].fPosAngle / (g_EnemyMotion.MAX_FLAME * 1.0f);
			}
			if (pEnemy->aModel[nCntMotion].pos <= g_EnemyMotion.aParts[nCntMotion].fPosAngle)
			{
				pEnemy->aModel[nCntMotion].pos += g_EnemyMotion.aParts[nCntMotion].fPosAngle / (g_EnemyMotion.MAX_FLAME * 1.0f);
			}

			g_EnemyMotion.aParts[nCntMotion].Pos = pEnemy->aModel[nCntMotion].pos;		// 現在の更新
		}

		// キーフレームの更新
		if (g_EnemyMotion.MAX_FLAME > 0)
		{// !!!	↑これ消したらエラーで落ちます	!!!
			if (g_EnemyMotion.nCntFlame % g_EnemyMotion.MAX_FLAME == 0)
			{// フレームが指定した分、進んだらキーフレームを次に進める
				if (g_EnemyMotion.KeyFlame == g_aEnemyTex->KEYFLAME[g_EnemyMotion.nNumEnemymotion] - 1)
				{// 最後のキーになったら0に戻す
					g_EnemyMotion.KeyFlame = 0;
				}
				else
				{// キーを次に進めてテキストデータから次のフレームを取得する
					g_EnemyMotion.KeyFlame++;
					g_EnemyMotion.MAX_FLAME = g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][g_EnemyMotion.KeyFlame][0].MAX_FLAME;
				}
			}
		}

		// ループの更新
		g_EnemyMotion.Loop = g_aEnemyTex->LOOP[g_EnemyMotion.nNumEnemymotion];		// ループするかどうか[0:ループしない / 1:ループする]

																		// フレームの加算
		g_EnemyMotion.nCntFlame++;
	}
}

//=============================================================================
// モーションの設定															   |
//=============================================================================
void SetEnemyMotion_5(int nNumEnemymotion)
{
	// モーションの番号
	g_EnemyMotion.nNumEnemymotion = nNumEnemymotion;

	// テキストデータから次のフレームを取得する
	g_EnemyMotion.MAX_FLAME = g_aEnemyTex->aMotionTex[g_EnemyMotion.nNumEnemymotion][0][0].MAX_FLAME;

	if (nNumEnemymotion > 0)
	{// 0以外だったらキーフレームを初期化する
		g_EnemyMotion.KeyFlame = 0;
	}
}

//=============================================================================
// モーションの情報の取得													   |
//=============================================================================
EnemyMotion_5 *GetEnemyMotion_5(void)
{
	return &g_EnemyMotion;
}