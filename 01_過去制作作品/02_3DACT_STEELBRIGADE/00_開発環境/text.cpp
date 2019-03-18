//=============================================================================
//
// モーションテキスト処理 [text.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "text.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOADFILENAME "data/MOTION/motion_gear.txt"	//読み込むテキストファイル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Text g_Text;


//*****************************************************************************
// メイン関数
//*****************************************************************************
void LoadText(void)
{
	FILE *pFile;
	pFile = fopen(LOADFILENAME, "r");

	if (pFile != NULL)
	{
		fscanf_s(pFile, "%d", &g_Text.MAX_TEXTPARTS);

		for (int nCntFile = 0; nCntFile < g_Text.MAX_TEXTPARTS; nCntFile++)
		{
			fscanf_s(pFile, "%s", &g_Text.aModelTex[nCntFile].FileName,256);

		}
		for (int nCntIndex = 0; nCntIndex < g_Text.MAX_TEXTPARTS; nCntIndex++)
		{
			fscanf_s(pFile, "%f", &g_Text.aModelTex[nCntIndex].Index);

		}
		for (int nCntPos = 0; nCntPos < g_Text.MAX_TEXTPARTS; nCntPos++)
		{
			fscanf_s(pFile, "%f, %f, %f", &g_Text.aModelTex[nCntPos].ModelPosX, &g_Text.aModelTex[nCntPos].ModelPosY, &g_Text.aModelTex[nCntPos].ModelPosZ);

		}
		for (int nCntRot = 0; nCntRot < g_Text.MAX_TEXTPARTS; nCntRot++)
		{
			fscanf_s(pFile, "%f, %f, %f", &g_Text.aModelTex[nCntRot].ModelRotX, &g_Text.aModelTex[nCntRot].ModelRotY, &g_Text.aModelTex[nCntRot].ModelRotZ);

		}
		fscanf_s(pFile, "%d", &g_Text.MAX_MOTION);
		for (int nCntMotionType = 0; nCntMotionType < g_Text.MAX_MOTION; nCntMotionType++)
		{
			fscanf_s(pFile, "%d", &g_Text.MOVEPARTS[nCntMotionType]);
			fscanf_s(pFile, "%d", &g_Text.LOOP[nCntMotionType]);
			fscanf_s(pFile, "%d", &g_Text.KEYFLAME[nCntMotionType]);
			for (int nCntKey = 0; nCntKey < g_Text.KEYFLAME[nCntMotionType]; nCntKey++)
			{
				fscanf_s(pFile, "%d", &g_Text.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME);
				for (int nCntMotionPos = 0; nCntMotionPos < g_Text.MOVEPARTS[nCntMotionType]; nCntMotionPos++)
				{
					fscanf_s(pFile, "%f, %f, %f", &g_Text.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosX, &g_Text.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosY, &g_Text.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosZ);
				}
				for (int nCntMotionRot = 0; nCntMotionRot < g_Text.MOVEPARTS[nCntMotionType]; nCntMotionRot++)
				{
					fscanf_s(pFile, "%f, %f, %f", &g_Text.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotX, &g_Text.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotY, &g_Text.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotZ);
				}
				g_Text.TOTALFRAME[nCntMotionType] += g_Text.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME;
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


Text *GetText(void)
{
	return &g_Text;
}









