//=============================================================================
//
// ファイル読み込みの処理 [txt.cpp]
// Author : 
//
//=============================================================================
#include "txt.h"
#include "char_select.h"

//------------------------------------------------------------------------
// マクロ定義															  |
//------------------------------------------------------------------------
#define SEVEFILENAME "data/MOTION/motion0.txt"					// ロードするファイル名
#define ARMORTEXNAME "data/MOTION/motion_armor.txt"				// ロードするファイル名
#define ARMORTEXNAME0 "data/MOTION/motion_armor0.txt"			// ロードするファイル名
#define ARMORTEXNAME1 "data/MOTION/motion_armor1.txt"			// ロードするファイル名
#define WARRIOR_TEXT	"data/MOTION/motion_warrior.txt"
#define SORCERER_TEXT	"data/MOTION/magi_motion.txt"
//------------------------------------------------------------------------
//グローバル変数宣言                                                      |
//------------------------------------------------------------------------
Tex g_Tex;
Comments g_Comments;

//------------------------------------------------------------------------
// テキストデータのロード処理											  |
//------------------------------------------------------------------------
void LoadData(void)
{
	CHARACTER_TYPE pCharaType = GetCharaType();		// 選択キャラクター情報の取得
	FILE *pFile = NULL;

	if (pCharaType == CHARACTER_TYPE_SOLDIER)
	{
		pFile = fopen(ARMORTEXNAME0, "r");		// ファイルを開く
	}
	else if (pCharaType == CHARACTER_TYPE_WARRIOR)
	{
		pFile = fopen(WARRIOR_TEXT, "r");		// ファイルを開く
	}
	else if (pCharaType == CHARACTER_TYPE_SORCERER)
	{
		pFile = fopen(SORCERER_TEXT, "r");		// ファイルを開く
	}

	if (pFile != NULL)
	{   // ファイルが開けたとき
		fscanf_s(pFile, "%d", &g_Tex.MAX_PARTS);		// パーツの最大数の読み取り
		for (int nCntFile = 0; nCntFile < g_Tex.MAX_PARTS; nCntFile++)
		{// ファイル名の読み取り
			fscanf_s(pFile, "%s", &g_Tex.aModelTex[nCntFile].FileName,256);
		}
		for (int nCntIdx = 0; nCntIdx < g_Tex.MAX_PARTS; nCntIdx++)
		{// 親子関係の読み取り
			fscanf_s(pFile, "%f", &g_Tex.aModelTex[nCntIdx].Index);
		}
		for (int nCntPos = 0; nCntPos < g_Tex.MAX_PARTS; nCntPos++)
		{// パーツの座標の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_Tex.aModelTex[nCntPos].PosX, &g_Tex.aModelTex[nCntPos].PosY, &g_Tex.aModelTex[nCntPos].PosZ);
		}
		for (int nCntRot = 0; nCntRot < g_Tex.MAX_PARTS; nCntRot++)
		{// パーツの回転の読み取り
			fscanf_s(pFile, "%f , %f , %f", &g_Tex.aModelTex[nCntRot].RotX, &g_Tex.aModelTex[nCntRot].RotY, &g_Tex.aModelTex[nCntRot].RotZ);
		}
		fscanf_s(pFile, "%d", &g_Tex.MAX_MOTION);		// モーションの最大数の読み取り
		for (int nCntMotionType = 0; nCntMotionType < g_Tex.MAX_MOTION; nCntMotionType++)
		{
			fscanf_s(pFile, "%d", &g_Tex.MOVEPARTS[nCntMotionType]);									// 動くモデルの数の読み取り
			fscanf_s(pFile, "%d", &g_Tex.LOOP[nCntMotionType]);											// ループ番号の読み取り
			fscanf_s(pFile, "%d", &g_Tex.KEYFLAME[nCntMotionType]);										// キーフレーム数の読み込み
			for (int nCntKey = 0; nCntKey < g_Tex.KEYFLAME[nCntMotionType]; nCntKey++)
			{// パーツの位置情報の読み取り
				fscanf_s(pFile, "%d", &g_Tex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME);			// フレーム数の読み取り
				for (int nCntMotionPos = 0; nCntMotionPos < g_Tex.MOVEPARTS[nCntMotionType]; nCntMotionPos++)
				{// パーツの位置の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_Tex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosX, &g_Tex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosY, &g_Tex.aMotionTex[nCntMotionType][nCntKey][nCntMotionPos].MotionPosZ);
				}
				for (int nCntMotionRot = 0; nCntMotionRot < g_Tex.MOVEPARTS[nCntMotionType]; nCntMotionRot++)
				{// パーツの回転の読み取り
					fscanf_s(pFile, "%f , %f , %f", &g_Tex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotX, &g_Tex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotY, &g_Tex.aMotionTex[nCntMotionType][nCntKey][nCntMotionRot].MotionRotZ);
				}
				g_Tex.TOTALFLAME[nCntMotionType] += g_Tex.aMotionTex[nCntMotionType][nCntKey][0].MAX_FLAME;
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
Tex *GetTex(void)
{
	return &g_Tex;
}