//=============================================================================
//
// アニメーションの処理 [motion.cpp]
// Author : 
//
//=============================================================================
#include "enemy_axe_motion2.h"
#include "enemy_axe_text2.h"
#include "enemy_axe2.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define MOVE_PARTS (15)			// アニメーションで動くパーツ数

//------------------------------------------------------------------------
// グローバル変数宣言                                                      |
//------------------------------------------------------------------------
EnemyAxeTex2 *g_aEnemyAxeTex2;					// テキストデータの取得
ENEMYAXE2 *pEnemyAxe2;				// エネミーデータの取得
EnemyAxeMotion2 g_EnemyAxeMotion2;
int nNumEnemyAxeMotion2 = 0;
int g_CntEnemyAxeMotion2 = 0;

//------------------------------------------------------------------------
// 初期化処理															  |
//------------------------------------------------------------------------
void InitEnemyAxeMotion2(void)
{
	// ファイルデータの取得
	g_aEnemyAxeTex2 = GetEnemyAxeTex2();

	// エネミーの取得
	pEnemyAxe2 = GetEnemyAxe2();

	// 初期設定
	g_EnemyAxeMotion2.nNumEnemyAxeMotion = 0;
	g_EnemyAxeMotion2.nCntFlame = 0;
	g_EnemyAxeMotion2.KeyFlame = 0;
	g_EnemyAxeMotion2.MAX_FLAME = g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][0].MAX_FLAME;
	g_EnemyAxeMotion2.Loop = g_aEnemyAxeTex2->LOOP[g_EnemyAxeMotion2.nNumEnemyAxeMotion];		// ループするかどうか[0:ループしない / 1:ループする]

	for (int nCntParts = 0; nCntParts > MOVE_PARTS; nCntParts++)
	{// アニメーションするパーツの情報を取得
		g_EnemyAxeMotion2.aParts[nCntParts].Pos = D3DXVECTOR3(pEnemyAxe2->aModel[nCntParts].pos.x, pEnemyAxe2->aModel[nCntParts].pos.y, pEnemyAxe2->aModel[nCntParts].pos.z);
		g_EnemyAxeMotion2.aParts[nCntParts].fPosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_EnemyAxeMotion2.aParts[nCntParts].Rot = D3DXVECTOR3(pEnemyAxe2->aModel[nCntParts].rot.x, pEnemyAxe2->aModel[nCntParts].rot.y, pEnemyAxe2->aModel[nCntParts].rot.z);
		g_EnemyAxeMotion2.aParts[nCntParts].fRotAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//------------------------------------------------------------------------
// 更新処理																  |
//------------------------------------------------------------------------
void UpdateEnemyAxeMotion2(void)
{
	g_CntEnemyAxeMotion2++;

	if (g_CntEnemyAxeMotion2 % 1 == 0)
	{
		// エネミーの状態を取得
		ENEMYAXESTATUS2 *pEnemyAxeState2;
		pEnemyAxeState2 = GetEnemyAxeStatus2();

		// ファイルデータの取得
		g_aEnemyAxeTex2 = GetEnemyAxeTex2();

		// エネミーの取得
		pEnemyAxe2 = GetEnemyAxe2();

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 回転の差分をだして移動する
		 // テキストデータから回転情報を取得する
			g_EnemyAxeMotion2.aParts[nCntMotion].RotDest = D3DXVECTOR3(g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][nCntMotion].MotionRotX,
				g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][nCntMotion].MotionRotY,
				g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][nCntMotion].MotionRotZ);
			// 差分を計算する
			g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle = g_EnemyAxeMotion2.aParts[nCntMotion].RotDest - g_EnemyAxeMotion2.aParts[nCntMotion].Rot;		// 差分 = 目的 - 現在

			if (g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
			}

			pEnemyAxe2->aModel[nCntMotion].rot += g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle / (g_EnemyAxeMotion2.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemyAxe2->aModel[nCntMotion].rot >= g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle)
			{
				pEnemyAxe2->aModel[nCntMotion].rot += g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle / (g_EnemyAxeMotion2.MAX_FLAME * 1.0f);
			}
			if (pEnemyAxe2->aModel[nCntMotion].rot <= g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle)
			{
				pEnemyAxe2->aModel[nCntMotion].rot += g_EnemyAxeMotion2.aParts[nCntMotion].fRotAngle / (g_EnemyAxeMotion2.MAX_FLAME * 1.0f);
			}

			g_EnemyAxeMotion2.aParts[nCntMotion].Rot = pEnemyAxe2->aModel[nCntMotion].rot;		// 現在の更新
		}

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 座標の差分をだして移動する
		 // テキストデータから座標情報を取得する
			g_EnemyAxeMotion2.aParts[nCntMotion].PosDest = D3DXVECTOR3(g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][nCntMotion].MotionPosX,
				g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][nCntMotion].MotionPosY,
				g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][nCntMotion].MotionPosZ);
			// 差分を計算する
			g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.x = g_EnemyAxeMotion2.aParts[nCntMotion].PosDest.x - g_EnemyAxeMotion2.aParts[nCntMotion].Pos.x + g_aEnemyAxeTex2->aModelTex[nCntMotion].PosX;		// 差分 = 目的 - 現在
			g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.y = g_EnemyAxeMotion2.aParts[nCntMotion].PosDest.y - g_EnemyAxeMotion2.aParts[nCntMotion].Pos.y + g_aEnemyAxeTex2->aModelTex[nCntMotion].PosY;
			g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.z = g_EnemyAxeMotion2.aParts[nCntMotion].PosDest.z - g_EnemyAxeMotion2.aParts[nCntMotion].Pos.z + g_aEnemyAxeTex2->aModelTex[nCntMotion].PosZ;

			if (g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.x > D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.x < -D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.y > D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.y < -D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.z > D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.z < -D3DX_PI)
			{
				g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle.z += D3DX_PI * 2;
			}

			pEnemyAxe2->aModel[nCntMotion].pos += g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle / (g_EnemyAxeMotion2.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemyAxe2->aModel[nCntMotion].pos >= g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle)
			{
				pEnemyAxe2->aModel[nCntMotion].pos += g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle / (g_EnemyAxeMotion2.MAX_FLAME * 1.0f);
			}
			if (pEnemyAxe2->aModel[nCntMotion].pos <= g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle)
			{
				pEnemyAxe2->aModel[nCntMotion].pos += g_EnemyAxeMotion2.aParts[nCntMotion].fPosAngle / (g_EnemyAxeMotion2.MAX_FLAME * 1.0f);
			}

			g_EnemyAxeMotion2.aParts[nCntMotion].Pos = pEnemyAxe2->aModel[nCntMotion].pos;		// 現在の更新
		}

		// キーフレームの更新
		if (g_EnemyAxeMotion2.MAX_FLAME > 0)
		{// !!!	↑これ消したらエラーで落ちます	!!!
			if (g_EnemyAxeMotion2.nCntFlame % g_EnemyAxeMotion2.MAX_FLAME == 0)
			{// フレームが指定した分、進んだらキーフレームを次に進める
				if (g_EnemyAxeMotion2.KeyFlame == g_aEnemyAxeTex2->KEYFLAME[g_EnemyAxeMotion2.nNumEnemyAxeMotion] - 1)
				{// 最後のキーになったら0に戻す
					g_EnemyAxeMotion2.KeyFlame = 0;
				}
				else
				{// キーを次に進めてテキストデータから次のフレームを取得する
					g_EnemyAxeMotion2.KeyFlame++;
					g_EnemyAxeMotion2.MAX_FLAME = g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][g_EnemyAxeMotion2.KeyFlame][0].MAX_FLAME;
				}
			}
		}

		// ループの更新
		g_EnemyAxeMotion2.Loop = g_aEnemyAxeTex2->LOOP[g_EnemyAxeMotion2.nNumEnemyAxeMotion];		// ループするかどうか[0:ループしない / 1:ループする]

																								// フレームの加算
		g_EnemyAxeMotion2.nCntFlame++;
	}
}

//=============================================================================
// モーションの設定															   |
//=============================================================================
void SetEnemyAxeMotion2(int nNumEnemyAxeMotion)
{
	// モーションの番号
	g_EnemyAxeMotion2.nNumEnemyAxeMotion = nNumEnemyAxeMotion;

	// テキストデータから次のフレームを取得する
	g_EnemyAxeMotion2.MAX_FLAME = g_aEnemyAxeTex2->aMotionTex[g_EnemyAxeMotion2.nNumEnemyAxeMotion][0][0].MAX_FLAME;

	if (nNumEnemyAxeMotion > 0)
	{// 0以外だったらキーフレームを初期化する
		g_EnemyAxeMotion2.KeyFlame = 0;
	}
}

//=============================================================================
// モーションの情報の取得													   |
//=============================================================================
EnemyAxeMotion2 *GetEnemyAxeMotion2(void)
{
	return &g_EnemyAxeMotion2;
}