//=============================================================================
//
// ファイル読み込みの処理 [Archertxt.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMYARCHERTXT1_H_
#define _ENEMYARCHERTXT1_H_
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>

//------------------------------------------------------------------------
// コメントの構造体														  |
//------------------------------------------------------------------------
typedef struct
{
	char Comments[256];
	char *ret;
}EnemyArcherComments_1;

//------------------------------------------------------------------------
// モデルの構造体														  |
//------------------------------------------------------------------------
typedef struct
{
	char FileName[256];								// 読み込むテクスチャ名
	float Index;									// 親子関係
	float PosX, PosY, PosZ;							// 座標
	float RotX, RotY, RotZ;							// 回転
}EnemyArcherModelTex_1;

//------------------------------------------------------------------------
// モーションの構造体													  |
//------------------------------------------------------------------------
typedef struct
{
	int		MAX_FLAME;								// フレームの数
	float   MotionPosX, MotionPosY, MotionPosZ;		// 目的の座標データ
	float   MotionRotX, MotionRotY, MotionRotZ;		// 目的の回転データ
}EnemyArcherMotionTex_1;

//------------------------------------------------------------------------
// ファイルの構造体														  |
//------------------------------------------------------------------------
typedef struct
{
	int MAX_PARTS;						// パーツの最大数
	int MAX_MOTION;						// モーションの最大数
	int LOOP[25];						// モーションをループするかどうか[モーションの番号]
	int KEYFLAME[25];					// キーフレームの数[モーションの番号]
	int TOTALFLAME[25];					// フレームの合計数[モーションの番号]
	int MOVEPARTS[25];					// モーション時に動くパーツ数[モーションの番号]
	EnemyArcherModelTex_1 aModelTex[100];			// パーツ[各パーツの番号]
	EnemyArcherMotionTex_1 aMotionTex[25][50][50];	// モーション[モーションの番号][キーフレームの番号][パーツの番号]
}EnemyArcherTex_1;

//------------------------------------------------------------------------
// プロトタイプ宣言														  |
//------------------------------------------------------------------------
void LoadEnemyArcherData_1(void);    // テキストのロード
EnemyArcherTex_1 *GetEnemyArcherTex_1(void);		// テキストデータの取得

#endif