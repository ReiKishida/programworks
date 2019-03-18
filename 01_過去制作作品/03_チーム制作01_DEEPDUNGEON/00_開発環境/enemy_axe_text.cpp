//=============================================================================
//
// ファイル読み込みの処理 [txt.cpp]
// Author : 
//
//=============================================================================
#include "enemy_axe_text.h"
//#include "char_select.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define ARMORTEXNAME "data/MOTION/motion_mouja_axe.txt"				// ロードするファイル名
//------------------------------------------------------------------------
//グローバル変数宣言                                                      |
//------------------------------------------------------------------------
EnemyAxeTex g_EnemyAxeTex;
EnemyAxeComments g_EnemyAxeComments;

//------------------------------------------------------------------------
// テキストデータのロード処理											  |
//------------------------------------------------------------------------
void LoadEnemyAxeData(void)
{
	FILE *pFile = NULL;


	pFile = fopen(ARMORTEXNAME, "r");		// ファイルを開く


	if (pFile != NULL)
	{   // ファイルが開けたとき
		fscanf_s(pFile, "%d", &g_EnemyAxeTex.MAX_PARTS);		// パーツの最大数の読み取り
		for (int nCntFile = 0; nCntFile < g_EnemyAxeTex.MAX_PARTS; nCntFile++)
		{// ファイル名の読み取り
			fscanf_s(pFile, "%s", &g_EnemyAxeTex.aModelTex[nCntFile].FileName, 256);
		}
		for (int nCntIdx = 0; nCntIdx < g_EnemyAxeTex.MAX_PARTS; nCntIdx++)
		{// 親子関係の読み取り
			fscanf_s(pFile, "%f", &g_EnemyAxeTex.aModelTex[nCntIdx].Index);
		}
		for (int nCntPos = 0; nCntPos < g_EnemyAxeTex.MAX_PARTS; nCntPos++)
		{// パーツの座標の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_EnemyAxeTex.aModelTex[nCntPos].PosX, &g_EnemyAxeTex.aModelTex[nCntPos].PosY, &g_EnemyAxeTex.aModelTex[nCntPos].PosZ);
		}
		for (int nCntRot = 0; nCntRot < g_EnemyAxeTex.MAX_PARTS; nCntRot++)
		{// パーツの回転の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_EnemyAxeTex.aModelTex[nCntRot].RotX, &g_EnemyAxeTex.aModelTex[nCntRot].RotY, &g_EnemyAxeTex.aModelTex[nCntRot].RotZ);
		}
		fscanf_s(pFile, "%d", &g_EnemyAxeTex.MAX_MOTION);		// モーションの最大数の読み取り
		for (int nCntMotionType = 0; nCntMotionType < g_EnemyAxeTex.MAX_MOTION; nCntMotionType++)
		{
			fscanf_s(pFile, "%d", &g_EnemyAxeTex.MOVEPARTS[nCntMotionType]);									// 動くモデルの数の読み取り
			fscanf_s(pFile, "%d", &g_EnemyAxeTex.LOOP[nCntMotionType]);											// ループ番号の読み取り
			fscanf_s(pFile, "%d", &g_EnemyAxeTex.KEYFLAME[nCntMotionType]);										// キーフレーム数の読み込み
			for (int nCntKey = 0; nCntKey < g_EnemyAxeTex.KEYFLAME[nCntMotionType]; nCntKey++)
			{// パーツの位置情報の読み取り
				fscanf_s(pFile, "%d", &g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME);			// フレーム数の読み取り
				for (int nCntMotionPos = 0; nCntMotionPos < g_EnemyAxeTex.MOVEPARTS[nCntMotionType]; nCntMotionPos++)
				{// パーツの位置の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosX, &g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosY, &g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosZ);
				}
				for (int nCntMotionRot = 0; nCntMotionRot < g_EnemyAxeTex.MOVEPARTS[nCntMotionType]; nCntMotionRot++)
				{// パーツの回転の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotX, &g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotY, &g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotZ);
				}
				g_EnemyAxeTex.TOTALFLAME[nCntMotionType] += g_EnemyAxeTex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME;
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
EnemyAxeTex *GetEnemyAxeTex(void)
{
	return &g_EnemyAxeTex;
}