//=============================================================================
//
// ファイル読み込みの処理 [txt.cpp]
// Author : 
//
//=============================================================================
#include "enemy_arrow_text.h"
//#include "char_select.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define ARMORTEXNAME "data/MOTION/motion_skelton_archer.txt"				// ロードするファイル名
//------------------------------------------------------------------------
//グローバル変数宣言                                                      |
//------------------------------------------------------------------------
EnemyArrowTex g_EnemyArrowTex;

EnemyArrowComments g_EnemyComments;

//------------------------------------------------------------------------
// テキストデータのロード処理											  |
//------------------------------------------------------------------------
void LoadEnemyArrowData(void)
{
	FILE *pFile = NULL;


	pFile = fopen(ARMORTEXNAME, "r");		// ファイルを開く


	if (pFile != NULL)
	{   // ファイルが開けたとき
		fscanf_s(pFile, "%d", &g_EnemyArrowTex.MAX_PARTS);		// パーツの最大数の読み取り
		for (int nCntFile = 0; nCntFile < g_EnemyArrowTex.MAX_PARTS; nCntFile++)
		{// ファイル名の読み取り
			fscanf_s(pFile, "%s", &g_EnemyArrowTex.aModelTex[nCntFile].FileName, 256);
		}
		for (int nCntIdx = 0; nCntIdx < g_EnemyArrowTex.MAX_PARTS; nCntIdx++)
		{// 親子関係の読み取り
			fscanf_s(pFile, "%f", &g_EnemyArrowTex.aModelTex[nCntIdx].Index);
		}
		for (int nCntPos = 0; nCntPos < g_EnemyArrowTex.MAX_PARTS; nCntPos++)
		{// パーツの座標の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_EnemyArrowTex.aModelTex[nCntPos].PosX, &g_EnemyArrowTex.aModelTex[nCntPos].PosY, &g_EnemyArrowTex.aModelTex[nCntPos].PosZ);
		}
		for (int nCntRot = 0; nCntRot < g_EnemyArrowTex.MAX_PARTS; nCntRot++)
		{// パーツの回転の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_EnemyArrowTex.aModelTex[nCntRot].RotX, &g_EnemyArrowTex.aModelTex[nCntRot].RotY, &g_EnemyArrowTex.aModelTex[nCntRot].RotZ);
		}
		fscanf_s(pFile, "%d", &g_EnemyArrowTex.MAX_MOTION);		// モーションの最大数の読み取り
		for (int nCntMotionType = 0; nCntMotionType < g_EnemyArrowTex.MAX_MOTION; nCntMotionType++)
		{
			fscanf_s(pFile, "%d", &g_EnemyArrowTex.MOVEPARTS[nCntMotionType]);									// 動くモデルの数の読み取り
			fscanf_s(pFile, "%d", &g_EnemyArrowTex.LOOP[nCntMotionType]);											// ループ番号の読み取り
			fscanf_s(pFile, "%d", &g_EnemyArrowTex.KEYFLAME[nCntMotionType]);										// キーフレーム数の読み込み
			for (int nCntKey = 0; nCntKey < g_EnemyArrowTex.KEYFLAME[nCntMotionType]; nCntKey++)
			{// パーツの位置情報の読み取り
				fscanf_s(pFile, "%d", &g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME);			// フレーム数の読み取り
				for (int nCntMotionPos = 0; nCntMotionPos < g_EnemyArrowTex.MOVEPARTS[nCntMotionType]; nCntMotionPos++)
				{// パーツの位置の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosX, &g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosY, &g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosZ);
				}
				for (int nCntMotionRot = 0; nCntMotionRot < g_EnemyArrowTex.MOVEPARTS[nCntMotionType]; nCntMotionRot++)
				{// パーツの回転の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotX, &g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotY, &g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotZ);
				}
				g_EnemyArrowTex.TOTALFLAME[nCntMotionType] += g_EnemyArrowTex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME;
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
EnemyArrowTex *GetEnemyArrowTex(void)
{
	return &g_EnemyArrowTex;
}