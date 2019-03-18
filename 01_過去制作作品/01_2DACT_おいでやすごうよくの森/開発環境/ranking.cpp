//=============================================================================
//
// ランキング処理 [Ranking.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "ranking.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "bg.h"
#include "block.h"
#include "fg.h"
#include "fg2.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RANKING		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING_CHARACTER	"data/TEXTURE/ranking.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING_PLAYER	"data/TEXTURE/p01.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING_SHOOTER	"data/TEXTURE/z.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

#define MAX_TEXTURE_TYPE (4) //テクスチャ数
#define RANKING_DIGIT (8)	//桁数
#define MAX_RANKING (5)	//ランキング数
#define MAX_OTHER (3)
#define CHANGE_RANKING (6)//入れ替え用
#define MAX_NUMER (40) // 数字の総数


#define RANKING_X (60.0f) //数字の大きさ_X
#define RANKING_Y (80.0f)//数字の大きさ_Y
#define RANK_CHARACTER_X	(280)
#define RANK_CHARACTER_Y	(250)


#define RANK_PLAYER_X	(280)
#define RANK_PLAYER_Y	(570)

#define RANK_SHOOTER_X	(270)
#define RANK_SHOOTER_Y	(400)


#define RANKING_POS ((D3DXVECTOR3(SCREEN_WIDTH/2 + RANKING_X * 4, 180.0f, 0.0f))) //ランキングの位置
#define RANKINGPOS_X ((-(RANKING_X / 2) - (RANKING_X * nCntDigit)))	//ランキングＸ位置のずらし
#define RANKINGPOS_Y ((- (RANKING_Y / 2) + ((40 + RANKING_Y) * nCntRanking)))	//ランキングＹ位置のずらし

#define RANKING_CHARACTER_POS ((D3DXVECTOR3(SCREEN_WIDTH / 2 - 500, 80.0f , 0.0f))) 
#define RANKING_PLAYER_POS ((D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, SCREEN_HEIGHT /2 + 70 , 0.0f))) 
#define RANKING_SHOOTER_POS ((D3DXVECTOR3(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT /2 + 80 , 0.0f))) 

int g_nCntRanking; //タイトルへの移行時間のカウント
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE_TYPE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;		// 頂点バッファへのポインタ
static int				g_nRanking[MAX_RANKING] = {600000, 580000, 550000, 500000, 450000};					// スコア
D3DXCOLOR				g_nScoreCol[MAX_RANKING + 1];								// 今のスコアの色
int g_nYourScore;

bool bYourScore;
int nCntHighScore;
int g_nCntFade; //自動フェード

//=============================================================================
// 初期化処理
//=============================================================================
void InitRanking(void)
{
	//背景初期化
	InitBG();

	//ブロックの初期化
	InitBlock();

	InitFg();

	InitFg2();

	SetBlock(D3DXVECTOR3(0.0f, 580, 0), 1280.0f, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);



	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 75, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);

	//草
	SetFg(D3DXVECTOR3(100, 640, 0.0f), 2, 50, 25);
	SetFg(D3DXVECTOR3(640, 630, 0.0f), 2, 60, 30);
	SetFg(D3DXVECTOR3(450, 630, 0.0f), 2, 50, 30);
	SetFg(D3DXVECTOR3(600, 650, 0.0f), 2, 40, 20);


	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2 + 10, 588, 0.0f), 0, 30, 470);

	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);


	g_nCntFade = 0;
	nCntHighScore = 0;
	for (int nCnt = 0; nCnt < MAX_RANKING + 1; nCnt++)
	{
		g_nScoreCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//bYourScore = false;


	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// テクスチャの読み込み(数字)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING, &g_pTextureRanking[0]);
	// テクスチャの読み込み(ランキングモジ)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING_CHARACTER, &g_pTextureRanking[1]);
	// テクスチャの読み込み(ランキングプレイヤー)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING_PLAYER, &g_pTextureRanking[2]);
	// テクスチャの読み込み(ランキング森の神)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING_SHOOTER, &g_pTextureRanking[3]);

	// 頂点情報の作成
	MakeVertexRanking(pDevice);


	PlaySound(SOUND_LABEL_SE_PEOPLE2);

}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitRanking(void)
{
	UnInitBG();

	//ブロックの初期化
	UnInitBlock();

	UnInitFg();
	UnInitFg2();


	for (int nCntRankingTex = 0; nCntRankingTex < MAX_TEXTURE_TYPE; nCntRankingTex++)
	{
		// テクスチャの開放
		if (g_pTextureRanking[nCntRankingTex] != NULL)
		{
			g_pTextureRanking[nCntRankingTex]->Release();
			g_pTextureRanking[nCntRankingTex] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}



	

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRanking(void)
{
	UpdateBG();

	//ブロックの更新
	UpdateBlock();

	UpdateFg();
	UpdateFg2();


	// 頂点情報の作成
	VERTEX_2D *pVtx;
	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	//VERTEX_2D *pVtx;

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//モード選択
			//SetMode(MODE_TITLE);
			SetFade(MODE_TITLE);
			bYourScore = false;
		}
	}
	g_nCntFade++;
		if (g_nCntFade % 620 == 0)
		{
			if (pFade == FADE_NONE)
			{
				g_nCntFade = 0;
				//モード選択
				SetFade(MODE_TITLE);
				bYourScore = false;

			}
		}




		nCntHighScore++;
		nCntHighScore = nCntHighScore % 4;
		// 頂点バッファをロックし頂点データへのポインタを取得
		g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
		if (bYourScore == true)
		{

			for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
			{
				for (int nCnt = 0; nCnt < 8; nCnt++, pVtx += 4)
				{
					if (g_nRanking[nCntRanking] == g_nYourScore)
					{
						if (nCntHighScore < 2)
						{
							g_nScoreCol[nCntRanking] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						}
						else if (nCntHighScore >= 2)
						{
							g_nScoreCol[nCntRanking] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
						}
						// 頂点カラーの設定
						pVtx[0].col = g_nScoreCol[nCntRanking];
						pVtx[1].col = g_nScoreCol[nCntRanking];
						pVtx[2].col = g_nScoreCol[nCntRanking];
						pVtx[3].col = g_nScoreCol[nCntRanking];

					}
				}
			}
		}
		
		// 頂点バッファをアンロックする
		g_pVtxBuffRanking->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRanking(void)
{
	DrawBG();


	//ブロックの描画
	DrawBlock();

	DrawFg();

	DrawFg2();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	//=============================================================//
	//					ランキング森の神						   //
	//=============================================================//
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking[3]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (8 * MAX_RANKING) + 12, 2);

	//=============================================================//
	//					ランキングプレイヤー					   //
	//=============================================================//
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking[2]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (8 * MAX_RANKING) + 8, 2);


	
	//=============================================================//
	//					ランキング文字							   //
	//=============================================================//
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking[1]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (8 * MAX_RANKING) + 4, 2);


	
	//=============================================================//
	//					ランキング数字							   //
	//=============================================================//
	// テクスチャの設定
	
	for (int nCntRanking = 0; nCntRanking < (8 * MAX_RANKING); nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRanking[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);

	}

	
	
	
}



//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 8 * (MAX_RANKING + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//=============================================================================//
	// ランキングのスコア(数字)													   //
	//=============================================================================//
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{

		for (int nCntDigit = 0; nCntDigit < 8; nCntDigit++, pVtx += 4)
		{

			pVtx[0].pos = D3DXVECTOR3(-(RANKING_X / 2) - (RANKING_X * nCntDigit), -(RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[1].pos = D3DXVECTOR3((RANKING_X / 2) - (RANKING_X * nCntDigit), -(RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[2].pos = D3DXVECTOR3(-(RANKING_X / 2) - (RANKING_X * nCntDigit), (RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[3].pos = D3DXVECTOR3((RANKING_X / 2) - (RANKING_X * nCntDigit), (RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			
			//1.0fで固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			if (nCntRanking == 0)
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else if (nCntRanking == 1)
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			}
			else if (nCntRanking == 2)
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
			}
			else
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);

			
		}
	
	}

	pVtx += 4;


#if 1
	//=============================================================================//
	// 左上のランキングの文字											    	   //
	//=============================================================================//
	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-RANK_CHARACTER_X / 2, -RANK_CHARACTER_Y / 2, 0.0f) + RANKING_CHARACTER_POS;
	pVtx[1].pos = D3DXVECTOR3(RANK_CHARACTER_X / 2, -RANK_CHARACTER_Y / 2, 0.0f) + RANKING_CHARACTER_POS;
	pVtx[2].pos = D3DXVECTOR3(-RANK_CHARACTER_X / 2, RANK_CHARACTER_Y / 2, 0.0f) + RANKING_CHARACTER_POS;
	pVtx[3].pos = D3DXVECTOR3(RANK_CHARACTER_X / 2, RANK_CHARACTER_Y / 2, 0.0f) + RANKING_CHARACTER_POS;
	// rhwの設定
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;
	}
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	pVtx += 4;


	//=============================================================================//
	// プレイヤー立ち絵													    	   //
	//=============================================================================//
	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-RANK_PLAYER_X / 2, -RANK_PLAYER_Y / 2, 0.0f) + RANKING_PLAYER_POS;
	pVtx[1].pos = D3DXVECTOR3(RANK_PLAYER_X / 2, -RANK_PLAYER_Y / 2, 0.0f) + RANKING_PLAYER_POS;
	pVtx[2].pos = D3DXVECTOR3(-RANK_PLAYER_X / 2, RANK_PLAYER_Y / 2, 0.0f) + RANKING_PLAYER_POS;
	pVtx[3].pos = D3DXVECTOR3(RANK_PLAYER_X / 2, RANK_PLAYER_Y / 2, 0.0f) + RANKING_PLAYER_POS;
	// rhwの設定
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;
	}
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	pVtx += 4;


	//=============================================================================//
	// 森の神立ち絵														    	   //
	//=============================================================================//
	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-RANK_SHOOTER_X / 2, -RANK_SHOOTER_Y / 2, 0.0f) + RANKING_SHOOTER_POS;
	pVtx[1].pos = D3DXVECTOR3(RANK_SHOOTER_X / 2, -RANK_SHOOTER_Y / 2, 0.0f) + RANKING_SHOOTER_POS;
	pVtx[2].pos = D3DXVECTOR3(-RANK_SHOOTER_X / 2, RANK_SHOOTER_Y / 2, 0.0f) + RANKING_SHOOTER_POS;
	pVtx[3].pos = D3DXVECTOR3(RANK_SHOOTER_X / 2, RANK_SHOOTER_Y / 2, 0.0f) + RANKING_SHOOTER_POS;
	// rhwの設定
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;
	}
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
#endif
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
	

}


//=============================================================================
// スコアのセット
//=============================================================================
void SetRanking(int nScore)
{
	int nRank;
	int aEmptyData[6];
	aEmptyData[5] = nScore;

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{

		aEmptyData[nCntNum] = g_nRanking[nCntNum];

	}

	for (int nCntNum = 1; nCntNum < 6; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] <= aEmptyData[nCntNum1])
			{
				nRank = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nRank;
			}
		}
	}


	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		g_nRanking[nCntNum] = aEmptyData[nCntNum];

	}
	bYourScore = true;

	g_nYourScore = nScore;

}
