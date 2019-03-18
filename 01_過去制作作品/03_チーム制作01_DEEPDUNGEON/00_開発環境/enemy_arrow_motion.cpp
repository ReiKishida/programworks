//=============================================================================
//
// アニメーションの処理 [motion.cpp]
// Author : 
//
//=============================================================================
#include "enemy_arrow_motion.h"
#include "enemy_arrow_text.h"
#include "enemy_arrow.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define MOVE_PARTS (15)			// アニメーションで動くパーツ数

//------------------------------------------------------------------------
// グローバル変数宣言                                                      |
//------------------------------------------------------------------------
EnemyArrowTex *g_aEnemyArrowex;					// テキストデータの取得
ENEMYARROW *pEnemyArrow;				// エネミーデータの取得
EnemyArrowMotion g_EnemyArrowMotion;
int nNumEnemyArrowMotion = 0;
int g_CntEnemyArrowMotion = 0;

//------------------------------------------------------------------------
// 初期化処理															  |
//------------------------------------------------------------------------
void InitEnemyArrowMotion(void)
{
	// ファイルデータの取得
	g_aEnemyArrowex = GetEnemyArrowTex();

	// エネミーの取得
	pEnemyArrow = GetEnemyArrow();

	// 初期設定
	g_EnemyArrowMotion.nNumEnemyArrowMotion = 0;
	g_EnemyArrowMotion.nCntFlame = 0;
	g_EnemyArrowMotion.KeyFlame = 0;
	g_EnemyArrowMotion.MAX_FLAME = g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][0].MAX_FLAME;
	g_EnemyArrowMotion.Loop = g_aEnemyArrowex->LOOP[g_EnemyArrowMotion.nNumEnemyArrowMotion];		// ループするかどうか[0:ループしない / 1:ループする]

	for (int nCntParts = 0; nCntParts > MOVE_PARTS; nCntParts++)
	{// アニメーションするパーツの情報を取得
		g_EnemyArrowMotion.aParts[nCntParts].Pos = D3DXVECTOR3(pEnemyArrow->aModel[nCntParts].pos.x, pEnemyArrow->aModel[nCntParts].pos.y, pEnemyArrow->aModel[nCntParts].pos.z);
		g_EnemyArrowMotion.aParts[nCntParts].fPosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_EnemyArrowMotion.aParts[nCntParts].Rot = D3DXVECTOR3(pEnemyArrow->aModel[nCntParts].rot.x, pEnemyArrow->aModel[nCntParts].rot.y, pEnemyArrow->aModel[nCntParts].rot.z);
		g_EnemyArrowMotion.aParts[nCntParts].fRotAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//------------------------------------------------------------------------
// 更新処理																  |
//------------------------------------------------------------------------
void UpdateEnemyArrowMotion(void)
{
	g_CntEnemyArrowMotion++;

	if (g_CntEnemyArrowMotion % 1 == 0)
	{
		// エネミーの状態を取得
		ENEMY_ARROW_STATUS *pEnemyArrowState;
		pEnemyArrowState = GetEnemyArrowStatus();

		// ファイルデータの取得
		g_aEnemyArrowex = GetEnemyArrowTex();

		// エネミーの取得
		pEnemyArrow = GetEnemyArrow();

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 回転の差分をだして移動する
		 // テキストデータから回転情報を取得する
			g_EnemyArrowMotion.aParts[nCntMotion].RotDest = D3DXVECTOR3(g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][nCntMotion].MotionRotX,
				g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][nCntMotion].MotionRotY,
				g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][nCntMotion].MotionRotZ);
			// 差分を計算する
			g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle = g_EnemyArrowMotion.aParts[nCntMotion].RotDest - g_EnemyArrowMotion.aParts[nCntMotion].Rot;		// 差分 = 目的 - 現在

			if (g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
			}

			pEnemyArrow->aModel[nCntMotion].rot += g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle / (g_EnemyArrowMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemyArrow->aModel[nCntMotion].rot >= g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle)
			{
				pEnemyArrow->aModel[nCntMotion].rot += g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle / (g_EnemyArrowMotion.MAX_FLAME * 1.0f);
			}
			if (pEnemyArrow->aModel[nCntMotion].rot <= g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle)
			{
				pEnemyArrow->aModel[nCntMotion].rot += g_EnemyArrowMotion.aParts[nCntMotion].fRotAngle / (g_EnemyArrowMotion.MAX_FLAME * 1.0f);
			}

			g_EnemyArrowMotion.aParts[nCntMotion].Rot = pEnemyArrow->aModel[nCntMotion].rot;		// 現在の更新
		}

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 座標の差分をだして移動する
		 // テキストデータから座標情報を取得する
			g_EnemyArrowMotion.aParts[nCntMotion].PosDest = D3DXVECTOR3(g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][nCntMotion].MotionPosX,
				g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][nCntMotion].MotionPosY,
				g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][nCntMotion].MotionPosZ);
			// 差分を計算する
			g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.x = g_EnemyArrowMotion.aParts[nCntMotion].PosDest.x - g_EnemyArrowMotion.aParts[nCntMotion].Pos.x + g_aEnemyArrowex->aModelTex[nCntMotion].PosX;		// 差分 = 目的 - 現在
			g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.y = g_EnemyArrowMotion.aParts[nCntMotion].PosDest.y - g_EnemyArrowMotion.aParts[nCntMotion].Pos.y + g_aEnemyArrowex->aModelTex[nCntMotion].PosY;
			g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.z = g_EnemyArrowMotion.aParts[nCntMotion].PosDest.z - g_EnemyArrowMotion.aParts[nCntMotion].Pos.z + g_aEnemyArrowex->aModelTex[nCntMotion].PosZ;

			if (g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.x > D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.x < -D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.y > D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.y < -D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.z > D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.z < -D3DX_PI)
			{
				g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle.z += D3DX_PI * 2;
			}

			pEnemyArrow->aModel[nCntMotion].pos += g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle / (g_EnemyArrowMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemyArrow->aModel[nCntMotion].pos >= g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle)
			{
				pEnemyArrow->aModel[nCntMotion].pos += g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle / (g_EnemyArrowMotion.MAX_FLAME * 1.0f);
			}
			if (pEnemyArrow->aModel[nCntMotion].pos <= g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle)
			{
				pEnemyArrow->aModel[nCntMotion].pos += g_EnemyArrowMotion.aParts[nCntMotion].fPosAngle / (g_EnemyArrowMotion.MAX_FLAME * 1.0f);
			}

			g_EnemyArrowMotion.aParts[nCntMotion].Pos = pEnemyArrow->aModel[nCntMotion].pos;		// 現在の更新
		}

		// キーフレームの更新
		if (g_EnemyArrowMotion.MAX_FLAME > 0)
		{// !!!	↑これ消したらエラーで落ちます	!!!
			if (g_EnemyArrowMotion.nCntFlame % g_EnemyArrowMotion.MAX_FLAME == 0)
			{// フレームが指定した分、進んだらキーフレームを次に進める
				if (g_EnemyArrowMotion.KeyFlame == g_aEnemyArrowex->KEYFLAME[g_EnemyArrowMotion.nNumEnemyArrowMotion] - 1)
				{// 最後のキーになったら0に戻す
					g_EnemyArrowMotion.KeyFlame = 0;
				}
				else
				{// キーを次に進めてテキストデータから次のフレームを取得する
					g_EnemyArrowMotion.KeyFlame++;
					g_EnemyArrowMotion.MAX_FLAME = g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][g_EnemyArrowMotion.KeyFlame][0].MAX_FLAME;
				}
			}
		}

		// ループの更新
		g_EnemyArrowMotion.Loop = g_aEnemyArrowex->LOOP[g_EnemyArrowMotion.nNumEnemyArrowMotion];		// ループするかどうか[0:ループしない / 1:ループする]

																					// フレームの加算
		g_EnemyArrowMotion.nCntFlame++;
	}
}

//=============================================================================
// モーションの設定															   |
//=============================================================================
void SetEnemyArrowMotion(int nNumEnemyArrowMotion)
{
	// モーションの番号
	g_EnemyArrowMotion.nNumEnemyArrowMotion = nNumEnemyArrowMotion;

	// テキストデータから次のフレームを取得する
	g_EnemyArrowMotion.MAX_FLAME = g_aEnemyArrowex->aMotionTex[g_EnemyArrowMotion.nNumEnemyArrowMotion][0][0].MAX_FLAME;

	if (nNumEnemyArrowMotion > 0)
	{// 0以外だったらキーフレームを初期化する
		g_EnemyArrowMotion.KeyFlame = 0;
	}
}

//=============================================================================
// モーションの情報の取得													   |
//=============================================================================
EnemyArrowMotion *GetEnemyArrowMotion(void)
{
	return &g_EnemyArrowMotion;
}