//=============================================================================
//
// ファイル読み込みの処理 [txt.cpp]
// Author : 
//
//=============================================================================
#include "bosstext.h"
//#include "char_select.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define ARMORTEXNAME "data/MOTION/motion_skeltonmaster.txt"				// ロードするファイル名
//------------------------------------------------------------------------
//グローバル変数宣言                                                      |
//------------------------------------------------------------------------
BossTex g_BossTex;
BossComments g_BossComments;

//------------------------------------------------------------------------
// テキストデータのロード処理											  |
//------------------------------------------------------------------------
void LoadBossData(void)
{
	FILE *pFile = NULL;


	pFile = fopen(ARMORTEXNAME, "r");		// ファイルを開く


	if (pFile != NULL)
	{   // ファイルが開けたとき
		fscanf_s(pFile, "%d", &g_BossTex.MAX_PARTS);		// パーツの最大数の読み取り
		for (int nCntFile = 0; nCntFile < g_BossTex.MAX_PARTS; nCntFile++)
		{// ファイル名の読み取り
			fscanf_s(pFile, "%s", &g_BossTex.aModelTex[nCntFile].FileName, 256);
		}
		for (int nCntIdx = 0; nCntIdx < g_BossTex.MAX_PARTS; nCntIdx++)
		{// 親子関係の読み取り
			fscanf_s(pFile, "%f", &g_BossTex.aModelTex[nCntIdx].Index);
		}
		for (int nCntPos = 0; nCntPos < g_BossTex.MAX_PARTS; nCntPos++)
		{// パーツの座標の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_BossTex.aModelTex[nCntPos].PosX, &g_BossTex.aModelTex[nCntPos].PosY, &g_BossTex.aModelTex[nCntPos].PosZ);
		}
		for (int nCntRot = 0; nCntRot < g_BossTex.MAX_PARTS; nCntRot++)
		{// パーツの回転の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_BossTex.aModelTex[nCntRot].RotX, &g_BossTex.aModelTex[nCntRot].RotY, &g_BossTex.aModelTex[nCntRot].RotZ);
		}
		fscanf_s(pFile, "%d", &g_BossTex.MAX_MOTION);		// モーションの最大数の読み取り
		for (int nCntMotionType = 0; nCntMotionType < g_BossTex.MAX_MOTION; nCntMotionType++)
		{
			fscanf_s(pFile, "%d", &g_BossTex.MOVEPARTS[nCntMotionType]);									// 動くモデルの数の読み取り
			fscanf_s(pFile, "%d", &g_BossTex.LOOP[nCntMotionType]);											// ループ番号の読み取り
			fscanf_s(pFile, "%d", &g_BossTex.KEYFLAME[nCntMotionType]);										// キーフレーム数の読み込み
			for (int nCntKey = 0; nCntKey < g_BossTex.KEYFLAME[nCntMotionType]; nCntKey++)
			{// パーツの位置情報の読み取り
				fscanf_s(pFile, "%d", &g_BossTex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME);			// フレーム数の読み取り
				for (int nCntMotionPos = 0; nCntMotionPos < g_BossTex.MOVEPARTS[nCntMotionType]; nCntMotionPos++)
				{// パーツの位置の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_BossTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosX, &g_BossTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosY, &g_BossTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosZ);
				}
				for (int nCntMotionRot = 0; nCntMotionRot < g_BossTex.MOVEPARTS[nCntMotionType]; nCntMotionRot++)
				{// パーツの回転の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_BossTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotX, &g_BossTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotY, &g_BossTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotZ);
				}
				g_BossTex.TOTALFLAME[nCntMotionType] += g_BossTex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME;
			}
		}
		fclose(pFile);   //ファイルを閉じる
		_getch();
	}
	else
	{   // ファイルを開けなかったとき
		MessageBox(NULL, "エラー", NULL, MB_YESNO);
	}
}

//------------------------------------------------------------------------
// テキストデータの取得													  |
//------------------------------------------------------------------------
BossTex *GetBossTex(void)
{
	return &g_BossTex;
}