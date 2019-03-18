//=============================================================================
//
// モーション処理 [enemymotion.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "enemymotion.h"
#include "enemytext.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
EnemyText *pEnemyText;
PLAYER *pPlayer_E;
EnemyMotion g_Motion_E;

int nNumMotion_E = 0;

//=============================================================================
// メイン関数
//=============================================================================
void InitEnemyMotion(void)
{
	pEnemyText = GetEnemyText();

	pPlayer_E = GetPlayer();

	g_Motion_E.nNumEnemyMotion = 0;
	g_Motion_E.nCntFlame = 0;
	g_Motion_E.KeyFlame = 0;
	g_Motion_E.MAX_FLAME = pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][0].MAX_FLAME;
	g_Motion_E.Loop = pEnemyText->LOOP[g_Motion_E.nNumEnemyMotion];

	for (int nCntparts = 0; nCntparts < MAX_PARTS; nCntparts++)
	{
		g_Motion_E.aParts[nCntparts].Pos = D3DXVECTOR3(pPlayer_E->aModel[nCntparts].pos.x, pPlayer_E->aModel[nCntparts].pos.y, pPlayer_E->aModel[nCntparts].pos.z);
		g_Motion_E.aParts[nCntparts].fPosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


		g_Motion_E.aParts[nCntparts].Rot = D3DXVECTOR3(pPlayer_E->aModel[nCntparts].rot.x, pPlayer_E->aModel[nCntparts].rot.y, pPlayer_E->aModel[nCntparts].rot.z);
		g_Motion_E.aParts[nCntparts].fRotAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	}
}

void UninitEnemyMotion(void)
{

}


void UpdateEnemyMotion(void)
{
	PLAYERSTATUS *pPlayer_EStatus;

	pPlayer_EStatus = GetPlayerStatus();


	pEnemyText = GetEnemyText();

	pPlayer_E = GetPlayer();


	for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
	{
		g_Motion_E.aParts[nCntMotion].RotDest = D3DXVECTOR3(pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][nCntMotion].MotionRotX,
			pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][nCntMotion].MotionRotY,
			pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][nCntMotion].MotionRotZ);
		g_Motion_E.aParts[nCntMotion].fRotAngle = g_Motion_E.aParts[nCntMotion].RotDest - g_Motion_E.aParts[nCntMotion].Rot;

		if (g_Motion_E.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
		{
			g_Motion_E.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
		}
		if (g_Motion_E.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
		{
			g_Motion_E.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
		}

		if (g_Motion_E.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
		{
			g_Motion_E.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
		}
		if (g_Motion_E.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
		{
			g_Motion_E.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
		}

		if (g_Motion_E.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
		{
			g_Motion_E.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
		}
		if (g_Motion_E.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
		{
			g_Motion_E.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
		}

		g_Motion_E.aParts[nCntMotion].Rot += g_Motion_E.aParts[nCntMotion].fRotAngle / (g_Motion_E.MAX_FLAME * 1.0f);	//回転を反映


		if (pPlayer_E->aModel[nCntMotion].rot >= g_Motion_E.aParts[nCntMotion].fRotAngle)
		{
			pPlayer_E->aModel[nCntMotion].rot += g_Motion_E.aParts[nCntMotion].fRotAngle / (g_Motion_E.MAX_FLAME * 1.0f);
		}
		if (pPlayer_E->aModel[nCntMotion].rot <= g_Motion_E.aParts[nCntMotion].fRotAngle)
		{
			pPlayer_E->aModel[nCntMotion].rot += g_Motion_E.aParts[nCntMotion].fRotAngle / (g_Motion_E.MAX_FLAME * 1.0f);
		}

		g_Motion_E.aParts[nCntMotion].Rot = pPlayer_E->aModel[nCntMotion].rot;	//現在の位置



	}

	for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
	{
		g_Motion_E.aParts[nCntMotion].PosDest = D3DXVECTOR3(pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][nCntMotion].MotionPosX, pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][nCntMotion].MotionPosY, pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][nCntMotion].MotionPosZ);
		g_Motion_E.aParts[nCntMotion].fPosAngle.x = g_Motion_E.aParts[nCntMotion].PosDest.x - g_Motion_E.aParts[nCntMotion].Pos.x + pEnemyText->aModelTex[nCntMotion].ModelPosX;
		g_Motion_E.aParts[nCntMotion].fPosAngle.y = g_Motion_E.aParts[nCntMotion].PosDest.y - g_Motion_E.aParts[nCntMotion].Pos.y + pEnemyText->aModelTex[nCntMotion].ModelPosY;
		g_Motion_E.aParts[nCntMotion].fPosAngle.z = g_Motion_E.aParts[nCntMotion].PosDest.z - g_Motion_E.aParts[nCntMotion].Pos.z + pEnemyText->aModelTex[nCntMotion].ModelPosZ;


		g_Motion_E.aParts[nCntMotion].Pos += g_Motion_E.aParts[nCntMotion].fPosAngle / (g_Motion_E.MAX_FLAME * 1.0f);	//位置を反映


		if (pPlayer_E->aModel[nCntMotion].pos >= g_Motion_E.aParts[nCntMotion].fPosAngle)
		{
			pPlayer_E->aModel[nCntMotion].pos += g_Motion_E.aParts[nCntMotion].fPosAngle / (g_Motion_E.MAX_FLAME * 1.0f);
		}
		if (pPlayer_E->aModel[nCntMotion].pos <= g_Motion_E.aParts[nCntMotion].fPosAngle)
		{
			pPlayer_E->aModel[nCntMotion].pos += g_Motion_E.aParts[nCntMotion].fPosAngle / (g_Motion_E.MAX_FLAME * 1.0f);
		}
		g_Motion_E.aParts[nCntMotion].Pos = pPlayer_E->aModel[nCntMotion].pos;	//現在の位置
	}

	if (g_Motion_E.MAX_FLAME > 0)
	{
		if (g_Motion_E.nCntFlame % g_Motion_E.MAX_FLAME == 0)
		{
			if (g_Motion_E.KeyFlame == pEnemyText->KEYFLAME[g_Motion_E.nNumEnemyMotion] - 1)
			{
				g_Motion_E.KeyFlame = 0;
			}
			else
			{
				g_Motion_E.KeyFlame++;
				g_Motion_E.MAX_FLAME = pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][g_Motion_E.KeyFlame][0].MAX_FLAME;
			}
		}
	}
	g_Motion_E.Loop = pEnemyText->LOOP[g_Motion_E.nNumEnemyMotion];//	ループ

	g_Motion_E.nCntFlame++;



}
//=============================================================================
//	モーション設定
//=============================================================================
void SetEnemyMotion(int nNumEnemyMotion)
{
	g_Motion_E.nNumEnemyMotion = nNumEnemyMotion;

	g_Motion_E.MAX_FLAME = pEnemyText->aMotionTex[g_Motion_E.nNumEnemyMotion][0][0].MAX_FLAME;


	if (nNumEnemyMotion > 0)
	{
		g_Motion_E.KeyFlame = 0;
	}
}

EnemyMotion *GetEnemyMotion(void)
{
	return &g_Motion_E;
}








