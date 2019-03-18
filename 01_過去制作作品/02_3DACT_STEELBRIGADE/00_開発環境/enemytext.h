//=============================================================================
//
// テキスト処理 [text.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _ENEYTEXT_H_
#define _ENEYTEXT_H_
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "stdio.h"
#include "conio.h"
#include "string.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************



typedef struct
{
	char FileName[256];	//ファイル名
	float Index;
	float ModelPosX, ModelPosY, ModelPosZ;
	float ModelRotX, ModelRotY, ModelRotZ;
}EnemyTex;


typedef struct
{
	int MAX_FLAME;
	float MotionPosX, MotionPosY, MotionPosZ;
	float MotionRotX, MotionRotY, MotionRotZ;
}EnemyMotionTex;

typedef struct
{
	int MAX_TEXTPARTS;//パーツ最大数
	int MAX_MOTION;//モーションの最大数
	int LOOP[25];	//ループするか
	int KEYFLAME[25];//キーフレーム数
	int TOTALFRAME[25];//フレームの合計数
	int MOVEPARTS[25];//モーション時に動くパーツ数
	EnemyTex aModelTex[100];//パーツ
	EnemyMotionTex aMotionTex[25][50][50];//モーション
}EnemyText;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void LoadEnemyText(void);
EnemyText *GetEnemyText(void);

#endif

