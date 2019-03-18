//=============================================================================
//
// アニメーションの処理 [motion.cpp]
// Author : 
//
//=============================================================================
#include "bossmotion.h"
#include "bosstext.h"
#include "boss.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define MOVE_PARTS (15)			// アニメーションで動くパーツ数

//------------------------------------------------------------------------
// グローバル変数宣言                                                      |
//------------------------------------------------------------------------
BossTex *pBossText;					// テキストデータの取得
BOSS *pBoss;				// エネミーデータの取得
BossMotion g_BossMotion;
int nNumBossMotion = 0;
int g_CntBossMotion = 0;

//------------------------------------------------------------------------
// 初期化処理															  |
//------------------------------------------------------------------------
void InitBossMotion(void)
{
	// ファイルデータの取得
	pBossText = GetBossTex();

	// エネミーの取得
	pBoss = GetBoss();

	// 初期設定
	g_BossMotion.nNumBossMotion = 0;
	g_BossMotion.nCntFlame = 0;
	g_BossMotion.KeyFlame = 0;
	g_BossMotion.MAX_FLAME = pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][0].MAX_FLAME;
	g_BossMotion.Loop = pBossText->LOOP[g_BossMotion.nNumBossMotion];		// ループするかどうか[0:ループしない / 1:ループする]

	for (int nCntParts = 0; nCntParts > MOVE_PARTS; nCntParts++)
	{// アニメーションするパーツの情報を取得
		g_BossMotion.aParts[nCntParts].Pos = D3DXVECTOR3(pBoss->aModel[nCntParts].pos.x, pBoss->aModel[nCntParts].pos.y, pBoss->aModel[nCntParts].pos.z);
		g_BossMotion.aParts[nCntParts].fPosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_BossMotion.aParts[nCntParts].Rot = D3DXVECTOR3(pBoss->aModel[nCntParts].rot.x, pBoss->aModel[nCntParts].rot.y, pBoss->aModel[nCntParts].rot.z);
		g_BossMotion.aParts[nCntParts].fRotAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//------------------------------------------------------------------------
// 更新処理																  |
//------------------------------------------------------------------------
void UpdateBossMotion(void)
{
	g_CntBossMotion++;

	if (g_CntBossMotion % 1 == 0)
	{
		// エネミーの状態を取得
		BOSSSTATUS *pBossState;
		pBossState = GetBossStatus();

		// ファイルデータの取得
		pBossText = GetBossTex();

		// エネミーの取得
		pBoss = GetBoss();

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 回転の差分をだして移動する
		 // テキストデータから回転情報を取得する
			g_BossMotion.aParts[nCntMotion].RotDest = D3DXVECTOR3(pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][nCntMotion].MotionRotX,
				pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][nCntMotion].MotionRotY,
				pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][nCntMotion].MotionRotZ);
			// 差分を計算する
			g_BossMotion.aParts[nCntMotion].fRotAngle = g_BossMotion.aParts[nCntMotion].RotDest - g_BossMotion.aParts[nCntMotion].Rot;		// 差分 = 目的 - 現在

			if (g_BossMotion.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
			}
			if (g_BossMotion.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
			}

			if (g_BossMotion.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
			}
			if (g_BossMotion.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
			}

			if (g_BossMotion.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
			}
			if (g_BossMotion.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
			}

			pBoss->aModel[nCntMotion].rot += g_BossMotion.aParts[nCntMotion].fRotAngle / (g_BossMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pBoss->aModel[nCntMotion].rot >= g_BossMotion.aParts[nCntMotion].fRotAngle)
			{
				pBoss->aModel[nCntMotion].rot += g_BossMotion.aParts[nCntMotion].fRotAngle / (g_BossMotion.MAX_FLAME * 1.0f);
			}
			if (pBoss->aModel[nCntMotion].rot <= g_BossMotion.aParts[nCntMotion].fRotAngle)
			{
				pBoss->aModel[nCntMotion].rot += g_BossMotion.aParts[nCntMotion].fRotAngle / (g_BossMotion.MAX_FLAME * 1.0f);
			}

			g_BossMotion.aParts[nCntMotion].Rot = pBoss->aModel[nCntMotion].rot;		// 現在の更新
		}

		for (int nCntMotion = 0; nCntMotion < MOVE_PARTS; nCntMotion++)
		{// 座標の差分をだして移動する
		 // テキストデータから座標情報を取得する
			g_BossMotion.aParts[nCntMotion].PosDest = D3DXVECTOR3(pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][nCntMotion].MotionPosX,
				pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][nCntMotion].MotionPosY,
				pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][nCntMotion].MotionPosZ);
			// 差分を計算する
			g_BossMotion.aParts[nCntMotion].fPosAngle.x = g_BossMotion.aParts[nCntMotion].PosDest.x - g_BossMotion.aParts[nCntMotion].Pos.x + pBossText->aModelTex[nCntMotion].PosX;		// 差分 = 目的 - 現在
			g_BossMotion.aParts[nCntMotion].fPosAngle.y = g_BossMotion.aParts[nCntMotion].PosDest.y - g_BossMotion.aParts[nCntMotion].Pos.y + pBossText->aModelTex[nCntMotion].PosY;
			g_BossMotion.aParts[nCntMotion].fPosAngle.z = g_BossMotion.aParts[nCntMotion].PosDest.z - g_BossMotion.aParts[nCntMotion].Pos.z + pBossText->aModelTex[nCntMotion].PosZ;

			if (g_BossMotion.aParts[nCntMotion].fPosAngle.x > D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fPosAngle.x -= D3DX_PI * 2;
			}
			if (g_BossMotion.aParts[nCntMotion].fPosAngle.x < -D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fPosAngle.x += D3DX_PI * 2;
			}

			if (g_BossMotion.aParts[nCntMotion].fPosAngle.y > D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fPosAngle.y -= D3DX_PI * 2;
			}
			if (g_BossMotion.aParts[nCntMotion].fPosAngle.y < -D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fPosAngle.y += D3DX_PI * 2;
			}

			if (g_BossMotion.aParts[nCntMotion].fPosAngle.z > D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fPosAngle.z -= D3DX_PI * 2;
			}
			if (g_BossMotion.aParts[nCntMotion].fPosAngle.z < -D3DX_PI)
			{
				g_BossMotion.aParts[nCntMotion].fPosAngle.z += D3DX_PI * 2;
			}

			pBoss->aModel[nCntMotion].pos += g_BossMotion.aParts[nCntMotion].fPosAngle / (g_BossMotion.MAX_FLAME * 1.0f);

			// 更新
			if (pBoss->aModel[nCntMotion].pos >= g_BossMotion.aParts[nCntMotion].fPosAngle)
			{
				pBoss->aModel[nCntMotion].pos += g_BossMotion.aParts[nCntMotion].fPosAngle / (g_BossMotion.MAX_FLAME * 1.0f);
			}
			if (pBoss->aModel[nCntMotion].pos <= g_BossMotion.aParts[nCntMotion].fPosAngle)
			{
				pBoss->aModel[nCntMotion].pos += g_BossMotion.aParts[nCntMotion].fPosAngle / (g_BossMotion.MAX_FLAME * 1.0f);
			}

			g_BossMotion.aParts[nCntMotion].Pos = pBoss->aModel[nCntMotion].pos;		// 現在の更新
		}

		// キーフレームの更新
		if (g_BossMotion.MAX_FLAME > 0)
		{// !!!	↑これ消したらエラーで落ちます	!!!
			if (g_BossMotion.nCntFlame % g_BossMotion.MAX_FLAME == 0)
			{// フレームが指定した分、進んだらキーフレームを次に進める
				if (g_BossMotion.KeyFlame == pBossText->KEYFLAME[g_BossMotion.nNumBossMotion] - 1)
				{// 最後のキーになったら0に戻す
					g_BossMotion.KeyFlame = 0;
				}
				else
				{// キーを次に進めてテキストデータから次のフレームを取得する
					g_BossMotion.KeyFlame++;
					g_BossMotion.MAX_FLAME = pBossText->aMotionTex[g_BossMotion.nNumBossMotion][g_BossMotion.KeyFlame][0].MAX_FLAME;
				}
			}
		}

		// ループの更新
		g_BossMotion.Loop = pBossText->LOOP[g_BossMotion.nNumBossMotion];		// ループするかどうか[0:ループしない / 1:ループする]

																					// フレームの加算
		g_BossMotion.nCntFlame++;
	}
}

//=============================================================================
// モーションの設定															   |
//=============================================================================
void SetBossMotion(int nNumBossMotion)
{
	// モーションの番号
	g_BossMotion.nNumBossMotion = nNumBossMotion;

	// テキストデータから次のフレームを取得する
	g_BossMotion.MAX_FLAME = pBossText->aMotionTex[g_BossMotion.nNumBossMotion][0][0].MAX_FLAME;

	if (nNumBossMotion > 0)
	{// 0以外だったらキーフレームを初期化する
		g_BossMotion.KeyFlame = 0;
	}
}

//=============================================================================
// モーションの情報の取得													   |
//=============================================================================
BossMotion *GetBossMotion(void)
{
	return &g_BossMotion;
}