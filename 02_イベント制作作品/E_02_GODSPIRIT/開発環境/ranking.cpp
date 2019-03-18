////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ランキング処理 [ranking.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "effect.h"
#include "sound.h"
#include "gamepad.h"
#include "game.h"

#include "camera.h"
#include "light.h"
#include "effectUP.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "enemy.h"
#include "effect.h"
#include "meshField.h"
#include "meshWall.h"
#include "meshCylinder.h"
#include "sky.h"
#include "pause.h"
#include "fade.h"
#include "item.h"
#include "line.h"
#include "orbit.h"
#include "billboard.h"
#include "BillLife.h"
#include "effect2D.h"
#include "light.h"
#include "MessageWindow.h"
#include "effect3D.h"
#include "meshDome.h"
#include "meshDome2.h"

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ブロックの中心座標
	D3DXCOLOR  col;			// 移動量
	RANKINGTYPE nType;			// ブロックの種類
	float fWidth;				// ブロックの幅
	float fHeight;				// ブロックの高さ
	int nNumCnt;				// 文字数
} RankingInfo;
typedef struct
{
	char *pFileName;
}RankingTexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_RANKING (5)
#define MAX_RANKING_POLYGON	(51)
#define AUTO_NEXT_TIME		(300)		// 自動画面推移
#define FLASHTIME			(8)			// 点滅頻度 /2
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[RANKINGTYPE_MAX] = {};							// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;									// 頂点バッファへのポインタ
static int				g_nRanking[MAX_RANKING] = {	500000,400000,300000,250000,100000};	// スコア
int						nCounterFade;												// フェードカウンタ－
int						g_nScoreNow;												// 今のスコア
int						nCountScore;												// スコアの点滅
bool					bHeightScore;
int g_nRankCnt;
int g_nLenRank;
int nAnimCounter;
Ranking g_aRanking[MAX_RANKING_POLYGON];
RankingInfo g_aRankingInfo[] =
{

	{ D3DXVECTOR3(SCREEN_WIDTH / 2 ,100 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG1,SCREEN_WIDTH - 200, 0 ,1 },
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,250 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG2,SCREEN_WIDTH - 200, 0 ,1 },
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,400 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG3,SCREEN_WIDTH - 100, 0 ,1},
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,540 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG4,SCREEN_WIDTH, 0 ,1 },
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,650 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCOREBG5,SCREEN_WIDTH - 300, 0 ,1 },

	{ D3DXVECTOR3(950,100 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE1,100, 120 ,8 },
	{ D3DXVECTOR3(900,230 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE2,90, 100 ,8 },
	{ D3DXVECTOR3(850,360 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE3,70, 90 ,8 },
	{ D3DXVECTOR3(800,490 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE4,60, 80 ,8 },
	{ D3DXVECTOR3(800,620 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORE5,60, 80 ,8},

	{ D3DXVECTOR3(100,100 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM1,120, 120 ,1 },
	{ D3DXVECTOR3(100,230 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM2,110, 100 ,1 },
	{ D3DXVECTOR3(100,360 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM3,90, 90 ,1 },
	{ D3DXVECTOR3(100,490 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM4,80, 80 ,1 },
	{ D3DXVECTOR3(100,620 + 700,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), RANKINGTYPE_SCORENUM5,80, 80 ,1 },
};
RankingTexture g_aRankingTex[RANKINGTYPE_MAX] = 
{
	{ "data/TEXTURE/sori.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/sori.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/sori.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/sori.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/sori.png" },	// 読み込むテクスチャファイル名

	{ "data/TEXTURE/number000.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/number000.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/number000.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/number000.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/number000.png" },	// 読み込むテクスチャファイル名

	{ "data/TEXTURE/rank1.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/rank2.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/rank3.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/rank4.png" },	// 読み込むテクスチャファイル名
	{ "data/TEXTURE/rank5.png" },	// 読み込むテクスチャファイル名

};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitRanking(void)
{
	// カメラの初期化処理
	InitCamera();
	// 線の初期化処理
	InitLine();
	// ライトの初期化処理
	InitLight();
	// 空の初期化処理
	InitSky();
	// 下向きエフェクトの初期化処理
	InitEffectUP();
	// メッシュ床の初期化処理
	InitMeshField();
	// メッシュ壁の初期化処理
	InitMeshWall();
	// メッシュ筒の初期化処理
	InitMeshCylinder();
	// 影の初期化処理
	InitShadow();
	// プレイヤーの初期化処理
	InitPlayer();
	// モデルの初期化処理
	InitModel();
	// ビルボードの初期化処理
	InitBillboard();
	// ビルボードライフの初期化処理
	InitBillLife();
	// 敵の初期化処理
	InitEnemy();
	// アイテムの初期化処理
	InitItem();
	// ポーズの初期化処理
	InitPause();
	// エフェクト２Dの初期化処理
	InitEffect2D();
	// メッセージウィンドウの初期化処理
	InitMessageWindow();
	// エフェクト３Dの初期化処理
	InitEffect3D();
	// エフェクトの初期化処理
	InitEffect();
	// メッシュドームの初期化処理
	InitMeshDome();
	InitMeshDome2();

	SetStagePos("data/stageTitle.txt");
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < RANKINGTYPE_MAX; nTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			g_aRankingTex[nTex].pFileName,		// ファイルの名前
			&g_pTextureRanking[nTex]);			// テクスチャへのポインタ
	}
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON; nCntRanking++)
	{
		g_aRanking[nCntRanking].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRanking[nCntRanking].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRanking[nCntRanking].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRanking[nCntRanking].nType = RANKINGTYPE_SCOREBG1;
		g_aRanking[nCntRanking].fWidth = 0;
		g_aRanking[nCntRanking].fHeight = 0;
		g_aRanking[nCntRanking].bUse = false;
	}
	nCounterFade = 0;
	nCountScore = 0;
	g_nRankCnt = 0;		// 数字の累乗表示
	g_nLenRank = 0;		// ５つつくるため
	nAnimCounter = 0;	// ランキングアニメーション
	// 頂点情報の作成
	MakeVertexRanking(pDevice);

	for (int nCnt = 0; nCnt < sizeof g_aRankingInfo / sizeof(RankingInfo); nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < g_aRankingInfo[nCnt].nNumCnt; nCnt2++)
		{
			SetRanking(g_aRankingInfo[nCnt].pos - D3DXVECTOR3(g_aRankingInfo[nCnt].fWidth * nCnt2, 0, 0.0f), g_aRankingInfo[nCnt].col, g_aRankingInfo[nCnt].nType, g_aRankingInfo[nCnt].fWidth, g_aRankingInfo[nCnt].fHeight);
		}
	}

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitRanking(void)
{
	// カメラの終了処理
	UninitCamera();
	// ラインの終了処理
	UninitLine();
	// ライトの終了処理
	UninitLight();
	// 空の終了処理
	UninitSky();
	// 下向きエフェクトの終了処理
	UninitEffectUP();
	// メッシュ筒の終了処理
	UninitMeshCylinder();
	// ビルボードの終了処理
	UninitBillboard();
	// 影の終了処理
	UninitShadow();
	// アイテムの終了処理
	UninitItem();
	// ポーズの終了処理
	UninitPause();
	// ビルボードライフの終了処理
	UninitBillLife();
	// エフェクト２Dの終了処理
	UninitEffect2D();
	// メッセージウィンドウの終了処理
	UninitMessageWindow();
	// エフェクトの終了処理
	UninitEffect();
	// エフェクト3Dの終了処理
	UninitEffect3D();
	// メッシュドームの終了処理
	UninitMeshDome();
	UninitMeshDome2();

	// テクスチャの開放
	for (int nCnt = 0; nCnt < RANKINGTYPE_MAX; nCnt++)
	{
		if (g_pTextureRanking[nCnt] != NULL)
		{
			g_pTextureRanking[nCnt]->Release();
			g_pTextureRanking[nCnt] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateRanking(void)
{
	UpdateCamera();
	// 線の更新処理
	UpdateLine();
	// ライトの更新処理
	UpdateLight();
	// 空の更新処理
	UpdateSky();
	// エフェクトの更新処理
	UpdateEffect();
	// 下向きエフェクトの更新処理
	UpdateEffectUP();
	// メッシュ床の更新処理
	UpdateMeshField();
	// メッシュ壁の更新処理
	UpdateMeshWall();
	// メッシュ筒の初期化処理
	UpdateMeshCylinder();
	// プレイヤーの更新処理
	UpdatePlayer(0);
	// モデルの更新処理
	UpdateModel();
	// ビルボードの更新処理
	UpdateBillboard();
	// 影の更新処理
	UpdateShadow();
	// 敵の更新処理
	UpdateEnemy();
	// アイテムの更新処理
	UpdateItem();
	// 軌跡の更新処理
	UpdateOrbit();
	// ビルボードライフの更新処理
	UpdateBillLife();
	// エフェクト２Dの更新処理
	UpdateEffect2D();
	// エフェクト3Dの更新処理
	UpdateEffect3D();
	// メッセージウィンドウ
	UpdateMessageWindow();
	// メッシュドームの更新処理
	UpdateMeshDome();
	UpdateMeshDome2();

	FADE pFADE;
	// フェードの取得
	pFADE = GetFade();

	if (pFADE == FADE_NONE)
	{
		for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
		{
			// 決定キー押したかどうか
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(nCntKey) == true)
			{
				// モード設定
				SetFade(MODE_TITLE);
				bHeightScore = false;
			}
		}
		nCounterFade++;
		if (nCounterFade % AUTO_NEXT_TIME == 0)
		{// 自動でタイトルに推移
			SetFade(MODE_TITLE);
			bHeightScore = false;
		}
	}
//-----------------------------------/ ランキング色点滅 /--------------------------------------------------------//

	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	nCountScore++;
	nCountScore = nCountScore % FLASHTIME;
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * RANKINGTYPE_SCORE1;
	for (int nCntRanking = 0; nCntRanking < 5; nCntRanking++)
	{
		if (g_aRanking[nCntRanking].bUse == true)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++, pVtx += 4)
			{
				if (g_nRanking[nCntRanking] == g_nScoreNow)
				{
					if (nCountScore < FLASHTIME / 2)
					{
						g_aRanking[nCntRanking].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
					}
					else if (nCountScore >= FLASHTIME / 2)
					{
						g_aRanking[nCntRanking].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);
					}
					// 頂点カラーの設定
					pVtx[0].col = g_aRanking[nCntRanking].col;
					pVtx[1].col = g_aRanking[nCntRanking].col;
					pVtx[2].col = g_aRanking[nCntRanking].col;
					pVtx[3].col = g_aRanking[nCntRanking].col;

				}
			}

		}
	}
		
	
	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

//-----------------------------------/ ランキングの移動 /--------------------------------------------------------//
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
	nAnimCounter++;

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON; nCntRanking++, pVtx += 4)
	{
		if (g_aRanking[nCntRanking].bUse == true)
		{
			g_aRanking[nCntRanking].pos += g_aRanking[nCntRanking].move;

			if (nAnimCounter >= 0)
			{// １位
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE1)
				{// スコア移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 100) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG1)
				{// スコア背景移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 100) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM1)
				{// 順位
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 100) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 20)
			{// ２位
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE2 )
				{// スコア移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 230) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG2)
				{// スコア背景移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 250) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM2)
				{// 順位
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 230) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 40)
			{// ３位
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE3 )
				{// スコア移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 360) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG3)
				{// スコア背景移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 400) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM3)
				{// 順位
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 360) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 60)
			{// ４位
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE4)
				{// スコア移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 490) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG4)
				{// スコア背景移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 540) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM4)
				{// 順位
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 490) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}
			if (nAnimCounter >= 80)
			{// ５位
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE5)
				{// スコア移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 620) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280) g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCOREBG5)
				{// スコア背景移動
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 640) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
				if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORENUM5)
				{// 順位
					g_aRanking[nCntRanking].move.y = -10.0f;
					if (g_aRanking[nCntRanking].pos.y <= 620) g_aRanking[nCntRanking].move.y = 0;
					if (nAnimCounter >= 280)g_aRanking[nCntRanking].move.y = -10.0f;
				}
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawRanking(void)
{
	SetCamera(0);
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		DrawLine();
	}	
	// メッシュドームの更新処理
	DrawMeshDome();
	DrawMeshDome2();

	// 空の描画処理
	//DrawSky();
	// メッシュ壁の描画処理
	DrawMeshWall();
	// メッシュ筒の初期化処理
	//DrawMeshCylinder();
	// プレイヤーの描画処理
	DrawPlayer(0);
	// モデルの描画処理
	DrawModel();
	// メッシュ床の描画処理
	DrawMeshField();
	// 影の描画処理
	DrawShadow();
	// 下向きエフェクトの描画処理
	DrawEffectUP();
	// ビルボードの描画処理
	DrawBillboard();
	// 敵の描画処理
	DrawEnemy();
	// エフェクトの描画処理
	DrawEffect();
	// 軌跡の描画処理
	DrawOrbit();
	// アイテムの描画処理
	DrawItem();
	// エフェクト#Dの描画処理
	DrawEffect3D();
	// メッセージウィンドウの描画処理
	DrawMessageWindow();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_RANKING_POLYGON;nCnt++)
	{
		if (g_aRanking[nCnt].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRanking[g_aRanking[nCnt].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCnt,
				2);
		}
	}
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKING_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
//-----------------------------------/ ランキング /--------------------------------------------------------//
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON ; nCntRanking++, pVtx += 4)
	{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			// rhwの設定
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].rhw = 1.0f;
			}
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

}

//==============================================================================================================================
// ランキングの入れ替わり処理
//==============================================================================================================================
void SetRanking(D3DXVECTOR3 pos, D3DXCOLOR  col, RANKINGTYPE nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
	//-----------------------------------/ ランキング /--------------------------------------------------------//
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING_POLYGON; nCntRanking++, pVtx += 4)
	{
		if (g_aRanking[nCntRanking].bUse == false)
		{
			g_aRanking[nCntRanking].pos = pos;
			g_aRanking[nCntRanking].col = col;
			g_aRanking[nCntRanking].nType = nType;
			g_aRanking[nCntRanking].fWidth = fWidth;
			g_aRanking[nCntRanking].fHeight = fHeight;
			g_aRanking[nCntRanking].bUse = true;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), -(g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aRanking[nCntRanking].fWidth / 2), (g_aRanking[nCntRanking].fHeight / 2), 0.0f) + g_aRanking[nCntRanking].pos;
			
			if (g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE1||g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE2 || g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE3 ||
				g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE4 || g_aRanking[nCntRanking].nType == RANKINGTYPE_SCORE5)
			{
				int nData = int(powf(10, float(g_nRankCnt)));

				pVtx[0].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nRanking[g_nLenRank] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nRankCnt++;
				if (g_nRankCnt >= 8)
				{
					g_nRankCnt = 0;
					g_nLenRank++;
				}
			}
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//==============================================================================================================================
// ランキングの入れ替わり処理
//==============================================================================================================================
void SortRanking(int nScore)
{
	int nRank;
	int aData[6];
	aData[5] = nScore;

	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		aData[nCntNum] = g_nRanking[nCntNum];
	}
	for (int nCntNum = 1; nCntNum < 6; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aData[nCntNum1 - 1] <= aData[nCntNum1])
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
	bHeightScore = true;
	g_nScoreNow = nScore;

}
