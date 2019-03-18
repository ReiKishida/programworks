//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// リザルト処理 [ranking.cpp]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "main.h"
#include "ranking.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "icon.h"
#include "rankTime.h"
#include "bg.h"
#include "gamepad.h"
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define TIME_RANK1				(17*60*60 + 31*60 + 00)			// ランク1の記録（時*60*60 + 分*60 + 秒
#define TIME_RANK2				(17*60*60 + 31*60 + 20)			// ランク2の記録（時*60*60 + 分*60 + 秒
#define TIME_RANK3				(17*60*60 + 31*60 + 40)			// ランク3の記録（時*60*60 + 分*60 + 秒
#define TIME_RANK4				(17*60*60 + 32*60 + 10)			// ランク4の記録（時*60*60 + 分*60 + 秒
#define TIME_RANK5				(17*60*60 + 32*60 + 50)			// ランク5の記録（時*60*60 + 分*60 + 秒

//#define TIME_RANK1				(01*60*60 + 23*60 + 45)			// ランク1の記録（時*60*60 + 分*60 + 秒
//#define TIME_RANK2				(23*60*60 + 45*60 + 67)			// ランク2の記録（時*60*60 + 分*60 + 秒
//#define TIME_RANK3				(45*60*60 + 67*60 + 89)			// ランク3の記録（時*60*60 + 分*60 + 秒
//#define TIME_RANK4				(67*60*60 + 89*60 + 01)			// ランク4の記録（時*60*60 + 分*60 + 秒
//#define TIME_RANK5				(89*60*60 + 01*60 + 23)			// ランク5の記録（時*60*60 + 分*60 + 秒

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
int			g_nRankingCounter;
static int	g_nRanking[MAX_RANKING] = { TIME_RANK1,TIME_RANK2,TIME_RANK3,TIME_RANK4,TIME_RANK5 };	// スコア
int			g_nTimeNow;
bool		g_bHeightTime;
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// リザルト初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitRanking(void)
{
	g_nRankingCounter = 0;
	// 各種初期化処理
	// カメラの初期化処理
	InitCamera();
	// ライトの初期化処理
	InitLight();

	// ランキングの時間初期化
	InitRankTime();
	// アイコンの初期化
	InitIcon();
	// 背景の初期化
	InitBG();

	SetIcon(D3DXVECTOR3(SCREEN_WIDTH/2.0f, 100.0f, 0.0f), ICON_TEX_RANKINGLOGO, 93.0f * 4.0f, 22.0f * 4.0f);


	SetIcon(D3DXVECTOR3(250.0f, 200.0f, 0.0f), ICON_TEX_RANK1, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 300.0f, 0.0f), ICON_TEX_RANK2, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 400.0f, 0.0f), ICON_TEX_RANK3, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 500.0f, 0.0f), ICON_TEX_RANK4, 150.0f, 150.0f);
	SetIcon(D3DXVECTOR3(250.0f, 600.0f, 0.0f), ICON_TEX_RANK5, 150.0f, 150.0f);

	for (int nCntRank = 0;nCntRank < MAX_RANKING;nCntRank++)
	{
		SetRankTimeResult(nCntRank, g_nRanking[nCntRank]);
	}

	SetBg(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BG_TEX_RESULT, BG_TYPE_NORMAL);

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// リザルト終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitRanking(void)
{
	// 各種終了処理
	// カメラの終了処理
	UninitCamera();
	// ライトの終了処理
	UninitLight();

	// ランキングの終了処理
	UninitRankTime();
	// アイコンの終了処理
	UninitIcon();
	// 背景の終了処理
	UninitBG();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// リザルト更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateRanking(void)
{
	g_nRankingCounter++;
	FADE pFade;
	// フェードモードの取得
	pFade = GetFade();
	if (pFade == FADE_NONE)
	{
		// 決定キーが押されたかどうか
		if (GetKeyboardTrigger(DIK_RETURN) == true||GetGamePadTrigger(0,BUTTON_B))
		{
			// フェード設定
			SetFade(MODE_GAME);
		}
	}

	// 各種更新処理
	// カメラの更新処理
	UpdateCamera();
	// ライトの更新処理
	UpdateLight();

	// ランキングの更新処理
	UpdateRankTime();
	// アイコンの更新処理
	UpdateIcon();
	// 背景の更新処理
	UpdateBG();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// リザルト描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawRanking(void)
{
	// 各種描画処理
	// カメラの設定
	SetCamera(0);

	// 背景の描画
	DrawBG();

	// アイコンの描画処理
	DrawIcon();
	// ランキングの時間初期化
	DrawRankTime();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ランキングの入れ替わり処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SortRanking(int nTime)
{
	g_bHeightTime = false;

	int nRank;
	int aData[6];
	aData[5] = nTime;

	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		aData[nCntNum] = g_nRanking[nCntNum];
	}
	for (int nCntNum = 1; nCntNum < 6; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aData[nCntNum1 - 1] >= aData[nCntNum1])
			{
				nRank = aData[nCntNum1];
				aData[nCntNum1] = aData[nCntNum1 - 1];
				aData[nCntNum1 - 1] = nRank;
			}
		}
	}
	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		g_nRanking[nCntNum] = aData[nCntNum];
	}
	g_bHeightTime = true;
	g_nTimeNow = nTime;

}
