//=============================================================================
//
// モーション処理 [motion.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "motion.h"
#include "text.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Text *pText;
PLAYER *pPlayer;
Motion g_Motion;

int nNumMotion = 0;

//=============================================================================
// メイン関数
//=============================================================================
void InitMotion(void)
{
	pText = GetText();

	pPlayer = GetPlayer();

	g_Motion.nNumPlayerMotion = 0;
	g_Motion.nCntFlame = 0;
	g_Motion.KeyFlame = 0;
	g_Motion.MAX_FLAME = pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][0].MAX_FLAME;
	g_Motion.Loop = pText->LOOP[g_Motion.nNumPlayerMotion];

	for (int nCntparts = 0; nCntparts < MAX_PARTS; nCntparts++)
	{
		g_Motion.aParts[nCntparts].Pos = D3DXVECTOR3(pPlayer->aModel[nCntparts].pos.x, pPlayer->aModel[nCntparts].pos.y, pPlayer->aModel[nCntparts].pos.z);
		g_Motion.aParts[nCntparts].fPosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


		g_Motion.aParts[nCntparts].Rot = D3DXVECTOR3(pPlayer->aModel[nCntparts].rot.x, pPlayer->aModel[nCntparts].rot.y, pPlayer->aModel[nCntparts].rot.z);
		g_Motion.aParts[nCntparts].fRotAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	}
}

void UninitMotion(void)
{

}


void UpdateMotion(void)
{
	PLAYERSTATUS *pPlayerStatus;

	pPlayerStatus = GetPlayerStatus();


	pText = GetText();

	pPlayer = GetPlayer();


	for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
	{
		g_Motion.aParts[nCntMotion].RotDest = D3DXVECTOR3(pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][nCntMotion].MotionRotX,
														  pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][nCntMotion].MotionRotY,
														  pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][nCntMotion].MotionRotZ);
		g_Motion.aParts[nCntMotion].fRotAngle = g_Motion.aParts[nCntMotion].RotDest - g_Motion.aParts[nCntMotion].Rot;

		if (g_Motion.aParts[nCntMotion].fRotAngle.x > D3DX_PI)
		{
			g_Motion.aParts[nCntMotion].fRotAngle.x -= D3DX_PI * 2;
		}
		if (g_Motion.aParts[nCntMotion].fRotAngle.x < -D3DX_PI)
		{
			g_Motion.aParts[nCntMotion].fRotAngle.x += D3DX_PI * 2;
		}

		if (g_Motion.aParts[nCntMotion].fRotAngle.y > D3DX_PI)
		{
			g_Motion.aParts[nCntMotion].fRotAngle.y -= D3DX_PI * 2;
		}
		if (g_Motion.aParts[nCntMotion].fRotAngle.y < -D3DX_PI)
		{
			g_Motion.aParts[nCntMotion].fRotAngle.y += D3DX_PI * 2;
		}

		if (g_Motion.aParts[nCntMotion].fRotAngle.z > D3DX_PI)
		{
			g_Motion.aParts[nCntMotion].fRotAngle.z -= D3DX_PI * 2;
		}
		if (g_Motion.aParts[nCntMotion].fRotAngle.z < -D3DX_PI)
		{
			g_Motion.aParts[nCntMotion].fRotAngle.z += D3DX_PI * 2;
		}

		g_Motion.aParts[nCntMotion].Rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);	//回転を反映


		if (pPlayer->aModel[nCntMotion].rot >= g_Motion.aParts[nCntMotion].fRotAngle)
		{
			pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);
		}
		if (pPlayer->aModel[nCntMotion].rot <= g_Motion.aParts[nCntMotion].fRotAngle)
		{
			pPlayer->aModel[nCntMotion].rot += g_Motion.aParts[nCntMotion].fRotAngle / (g_Motion.MAX_FLAME * 1.0f);
		}

		g_Motion.aParts[nCntMotion].Rot = pPlayer->aModel[nCntMotion].rot;	//現在の位置



	}

	for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
	{
		g_Motion.aParts[nCntMotion].PosDest = D3DXVECTOR3(pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][nCntMotion].MotionPosX, pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][nCntMotion].MotionPosY, pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][nCntMotion].MotionPosZ);
		g_Motion.aParts[nCntMotion].fPosAngle.x = g_Motion.aParts[nCntMotion].PosDest.x - g_Motion.aParts[nCntMotion].Pos.x + pText->aModelTex[nCntMotion].ModelPosX;
		g_Motion.aParts[nCntMotion].fPosAngle.y = g_Motion.aParts[nCntMotion].PosDest.y - g_Motion.aParts[nCntMotion].Pos.y + pText->aModelTex[nCntMotion].ModelPosY;
		g_Motion.aParts[nCntMotion].fPosAngle.z = g_Motion.aParts[nCntMotion].PosDest.z - g_Motion.aParts[nCntMotion].Pos.z + pText->aModelTex[nCntMotion].ModelPosZ;


		g_Motion.aParts[nCntMotion].Pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);	//位置を反映


		if (pPlayer->aModel[nCntMotion].pos >= g_Motion.aParts[nCntMotion].fPosAngle)
		{
			pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);
		}
		if (pPlayer->aModel[nCntMotion].pos <= g_Motion.aParts[nCntMotion].fPosAngle)
		{
			pPlayer->aModel[nCntMotion].pos += g_Motion.aParts[nCntMotion].fPosAngle / (g_Motion.MAX_FLAME * 1.0f);
		}
		g_Motion.aParts[nCntMotion].Pos = pPlayer->aModel[nCntMotion].pos;	//現在の位置
	}

		if (g_Motion.MAX_FLAME > 0)
		{
			if (g_Motion.nCntFlame % g_Motion.MAX_FLAME == 0)
			{
				if (g_Motion.KeyFlame == pText->KEYFLAME[g_Motion.nNumPlayerMotion] - 1)
				{
					g_Motion.KeyFlame = 0;
				}
				else
				{
					g_Motion.KeyFlame++;
					g_Motion.MAX_FLAME = pText->aMotionTex[g_Motion.nNumPlayerMotion][g_Motion.KeyFlame][0].MAX_FLAME;
				}
			}
		}
		g_Motion.Loop = pText->LOOP[g_Motion.nNumPlayerMotion];//	ループ

		g_Motion.nCntFlame++;



}
//=============================================================================
//	モーション設定
//=============================================================================
void SetMotion(int nNumPlayermotion)
{
	g_Motion.nNumPlayerMotion = nNumPlayermotion;

	g_Motion.MAX_FLAME = pText->aMotionTex[g_Motion.nNumPlayerMotion][0][0].MAX_FLAME;


	if (nNumPlayermotion > 0)
	{
		g_Motion.KeyFlame = 0;
	}
}

Motion * GetMotion(void)
{
	return &g_Motion;
}








