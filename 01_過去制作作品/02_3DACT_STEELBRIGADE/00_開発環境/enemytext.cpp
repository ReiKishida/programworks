//=============================================================================
//
// 敵のモーションテキスト処理 [enemymotion.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "enemytext.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOADFILENAME "data/MOTION/motion_enemy.txt"	//読み込むテキストファイル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
EnemyText g_EnemyText;


//*****************************************************************************
// メイン関数
//*****************************************************************************
void LoadEnemyText(void)
{
	FILE *pFile;
	pFile = fopen(LOADFILENAME, "r");

	if (pFile != NULL)
	{
		fscanf_s(pFile, "%d", &g_EnemyText.MAX_TEXTPARTS);

		for (int nCntFile = 0; nCntFile < g_EnemyText.MAX_TEXTPARTS; nCntFile++)
		{
			fscanf_s(pFile, "%s", &g_EnemyText.aModelTex[nCntFile].FileName, 256);

		}
		for (int nCntIndex = 0; nCntIndex < g_EnemyText.MAX_TEXTPARTS; nCntIndex++)
		{
			fscanf_s(pFile, "%f", &g_EnemyText.aModelTex[nCntIndex].Index);

		}
		for (int nCntPos = 0; nCntPos < g_EnemyText.MAX_TEXTPARTS; nCntPos++)
		{
			fscanf_s(pFile, "%f, %f, %f", &g_EnemyText.aModelTex[nCntPos].ModelPosX, &g_EnemyText.aModelTex[nCntPos].ModelPosY, &g_EnemyText.aModelTex[nCntPos].ModelPosZ);

		}
		for (int nCntRot = 0; nCntRot < g_EnemyText.MAX_TEXTPARTS; nCntRot++)
		{
			fscanf_s(pFile, "%f, %f, %f", &g_EnemyText.aModelTex[nCntRot].ModelRotX, &g_EnemyText.aModelTex[nCntRot].ModelRotY, &g_EnemyText.aModelTex[nCntRot].ModelRotZ);

		}
		fscanf_s(pFile, "%d", &g_EnemyText.MAX_MOTION);
		for (int nCntMotionType = 0; nCntMotionType < g_EnemyText.MAX_MOTION; nCntMotionType++)
		{
			fscanf_s(pFile, "%d", &g_EnemyText.MOVEPARTS[nCntMotionType]);
			fscanf_s(pFile, "%d", &g_EnemyText.LOOP[nCntMotionType]);
			fscanf_s(pFile, "%d", &g_EnemyText.KEYFLAME[nCntMotionType]);
			for (int nCntKey = 0; nCntKey < g_EnemyText.KEYFLAME[nCntMotionType]; nCntKey++)
			{
				fscanf_s(pFile, "%d", &g_EnemyText.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME);
				for (int nCntMotionPos = 0; nCntMotionPos < g_EnemyText.MOVEPARTS[nCntMotionType]; nCntMotionPos++)
				{
					fscanf_s(pFile, "%f, %f, %f", &g_EnemyText.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosX, &g_EnemyText.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosY, &g_EnemyText.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosZ);
				}
				for (int nCntMotionRot = 0; nCntMotionRot < g_EnemyText.MOVEPARTS[nCntMotionType]; nCntMotionRot++)
				{
					fscanf_s(pFile, "%f, %f, %f", &g_EnemyText.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotX, &g_EnemyText.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotY, &g_EnemyText.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotZ);
				}
				g_EnemyText.TOTALFRAME[nCntMotionType] += g_EnemyText.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME;
			}
		}
		fclose(pFile);
		_getch();
	}
	else
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", NULL, MB_YESNO);
	}
}


EnemyText *GetEnemyText(void)
{
	return &g_EnemyText;
}









