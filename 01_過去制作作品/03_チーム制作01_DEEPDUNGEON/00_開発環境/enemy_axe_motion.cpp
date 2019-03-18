//=============================================================================
//
// アニメーションの処理 [motion.cpp]
// Author : 
//
//=============================================================================
#include "enemy_axe_motion.h"
#include "enemy_axe_text.h"
#include "enemy_axe.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define MOVE_PARTS (15)			// アニメーションで動くパーツ数

//------------------------------------------------------------------------
// グローバル変数宣言                                                      |
//------------------------------------------------------------------------
EnemyAxeTex *g_aEnemyAxeTex;					// テキストデータの取得
ENEMYAXE *pEnemyAxe;				// エネミーデータの取得
EnemyAxeMotion g_EnemyAxeMotion;
int nNumEnemyAxeMotion = 0;
int g_CntEnemyAxeMotion = 0;

//------------------------------------------------------------------------
// 初期化処理															  |
//------------------------------------------------------------------------
void InitEnemyAxeMotion(void)
{
	// ファイルデータの取得
	g_aEnemyAxeTex = GetEnemyAxeTex();

	// エネミーの取得
	pEnemyAxe = GetEnemyAxe();

	// 初期設定
	g_EnemyAxeMotion.nNumEnemyAxeMotion = 0;
	g_EnemyAxeMotion.nCntFlame = 0;
	g_EnemyAxeMotion.KeyFlame = 0;
	g_EnemyAxeMotion.MAX_FLAME = g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][0].MAX_FLAME;
	g_EnemyAxeMotion.Loop = g_aEnemyAxeTex->LOOP[g_EnemyAxeMotion.nNumEnemyAxeMotion];		// ループするかどうか[0:ループしない / 1:ループする]

	for (int nCntParts = 0; nCntParts > MOVE_PARTS; nCntParts++)
	{// アニメーションするパーツの情報を取得
		g_EnemyAxeMotion.aParts[nCntParts].Pos = D3DXVECTOR3(pEnemyAxe->aModel[nCntParts].pos.x, pEnemyAxe->aModel[nCntParts].pos.y, pEnemyAxe->aModel[nCntParts].pos.z);
		g_EnemyAxeMotion.aParts[nCntParts].fPosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_EnemyAxeMotion.aParts[nCntParts].Rot = D3DXVECTOR3(pEnemyAxe->aModel[nCntParts].rot.x, pEnemyAxe->aModel[nCntParts].rot.y, pEnemyAxe->aModel[nCntParts].rot.z);
		g_EnemyAxeMotion.aParts[nCntParts].fRotAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//------------------------------------------------------------------------
// 更新処理																  |
//------------------------------------------------------------------------
void UpdateEnemyAxeMotion(void)
{
	g_CntEnemyAxeMotion++;

	if (g_CntEnemyAxeMotion % 1 == 0)
	{
		// エネミーの状態を取得
		ENEMYAXESTATUS *pEnemyAxeState;
		pEnemyAxeState = GetEnemyAxeStatus();

		// ファイルデータの取得
		g_aEnemyAxeTex = GetEnemyAxeTex();

		// エネミーの取得
		pEnemyAxe = GetEnemyAxe();

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 回転の差分をだして移動する
		 // テキストデータから回転情報を取得する
			g_EnemyAxeMotion.aParts[nCntMotion].RotDest = D3DXVECTOR3(g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][nCntMotion].MotionRotX,
				g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][nCntMotion].MotionRotY,
				g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][nCntMotion].MotionRotZ);
			// 差分を計算する
			g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle = g_EnemyAxeMotion.aParts[nCntMotion].RotDest - g_EnemyAxeMotion.aParts[nCntMotion].Rot;		// 差分 = 目的 - 現在

			if (g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
			}

			pEnemyAxe->aModel[nCntMotion].rot += g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle / (g_EnemyAxeMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemyAxe->aModel[nCntMotion].rot >= g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle)
			{
				pEnemyAxe->aModel[nCntMotion].rot += g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle / (g_EnemyAxeMotion.MAX_FLAME * 1.0f);
			}
			if (pEnemyAxe->aModel[nCntMotion].rot <= g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle)
			{
				pEnemyAxe->aModel[nCntMotion].rot += g_EnemyAxeMotion.aParts[nCntMotion].fRotAngle / (g_EnemyAxeMotion.MAX_FLAME * 1.0f);
			}

			g_EnemyAxeMotion.aParts[nCntMotion].Rot = pEnemyAxe->aModel[nCntMotion].rot;		// 現在の更新
		}

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 座標の差分をだして移動する
		 // テキストデータから座標情報を取得する
			g_EnemyAxeMotion.aParts[nCntMotion].PosDest = D3DXVECTOR3(g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][nCntMotion].MotionPosX,
				g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][nCntMotion].MotionPosY,
				g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][nCntMotion].MotionPosZ);
			// 差分を計算する
			g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.x = g_EnemyAxeMotion.aParts[nCntMotion].PosDest.x - g_EnemyAxeMotion.aParts[nCntMotion].Pos.x + g_aEnemyAxeTex->aModelTex[nCntMotion].PosX;		// 差分 = 目的 - 現在
			g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.y = g_EnemyAxeMotion.aParts[nCntMotion].PosDest.y - g_EnemyAxeMotion.aParts[nCntMotion].Pos.y + g_aEnemyAxeTex->aModelTex[nCntMotion].PosY;
			g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.z = g_EnemyAxeMotion.aParts[nCntMotion].PosDest.z - g_EnemyAxeMotion.aParts[nCntMotion].Pos.z + g_aEnemyAxeTex->aModelTex[nCntMotion].PosZ;

			if (g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.x > D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.x -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.x < -D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.x += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.y > D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.y -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.y < -D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.y += D3DX_PI * 2;
			}

			if (g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.z > D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.z -= D3DX_PI * 2;
			}
			if (g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.z < -D3DX_PI)
			{
				g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle.z += D3DX_PI * 2;
			}

			pEnemyAxe->aModel[nCntMotion].pos += g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle / (g_EnemyAxeMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pEnemyAxe->aModel[nCntMotion].pos >= g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle)
			{
				pEnemyAxe->aModel[nCntMotion].pos += g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle / (g_EnemyAxeMotion.MAX_FLAME * 1.0f);
			}
			if (pEnemyAxe->aModel[nCntMotion].pos <= g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle)
			{
				pEnemyAxe->aModel[nCntMotion].pos += g_EnemyAxeMotion.aParts[nCntMotion].fPosAngle / (g_EnemyAxeMotion.MAX_FLAME * 1.0f);
			}

			g_EnemyAxeMotion.aParts[nCntMotion].Pos = pEnemyAxe->aModel[nCntMotion].pos;		// 現在の更新
		}

		// キーフレームの更新
		if (g_EnemyAxeMotion.MAX_FLAME > 0)
		{// !!!	↑これ消したらエラーで落ちます	!!!
			if (g_EnemyAxeMotion.nCntFlame % g_EnemyAxeMotion.MAX_FLAME == 0)
			{// フレームが指定した分、進んだらキーフレームを次に進める
				if (g_EnemyAxeMotion.KeyFlame == g_aEnemyAxeTex->KEYFLAME[g_EnemyAxeMotion.nNumEnemyAxeMotion] - 1)
				{// 最後のキーになったら0に戻す
					g_EnemyAxeMotion.KeyFlame = 0;
				}
				else
				{// キーを次に進めてテキストデータから次のフレームを取得する
					g_EnemyAxeMotion.KeyFlame++;
					g_EnemyAxeMotion.MAX_FLAME = g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][g_EnemyAxeMotion.KeyFlame][0].MAX_FLAME;
				}
			}
		}

		// ループの更新
		g_EnemyAxeMotion.Loop = g_aEnemyAxeTex->LOOP[g_EnemyAxeMotion.nNumEnemyAxeMotion];		// ループするかどうか[0:ループしない / 1:ループする]

																					// フレームの加算
		g_EnemyAxeMotion.nCntFlame++;
	}
}

//=============================================================================
// モーションの設定															   |
//=============================================================================
void SetEnemyAxeMotion(int nNumEnemyAxeMotion)
{
	// モーションの番号
	g_EnemyAxeMotion.nNumEnemyAxeMotion = nNumEnemyAxeMotion;

	// テキストデータから次のフレームを取得する
	g_EnemyAxeMotion.MAX_FLAME = g_aEnemyAxeTex->aMotionTex[g_EnemyAxeMotion.nNumEnemyAxeMotion][0][0].MAX_FLAME;

	if (nNumEnemyAxeMotion > 0)
	{// 0以外だったらキーフレームを初期化する
		g_EnemyAxeMotion.KeyFlame = 0;
	}
}

//=============================================================================
// モーションの情報の取得													   |
//=============================================================================
EnemyAxeMotion *GetEnemyAxeMotion(void)
{
	return &g_EnemyAxeMotion;
}