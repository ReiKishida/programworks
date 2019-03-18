//=============================================================================
//
// ランキング処理 [Ranking.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "menuranking.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "bg.h"
#include "block.h"
#include "fg.h"
#include "fg2.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_MENURANKING		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_MENURANKING_CHARACTER	"data/TEXTURE/ranking.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMenuRanking(LPDIRECT3DDEVICE9 pDevice);

#define MAX_TEXTURE_TYPE (2) //テクスチャ数
#define MENURANKING_DIGIT (8)	//桁数
#define MAX_MENURANKING (5)	//ランキング数
#define CHANGE_MENURANKING (6)//入れ替え用
#define MAX_NUMER (40) // 数字の総数


#define MENURANKING_X (60.0f) //数字の大きさ_X
#define MENURANKING_Y (80.0f)//数字の大きさ_Y
#define MENURANK_CHARACTER_X	(280)
#define MENURANK_CHARACTER_Y	(250)


#define MENURANKING_POS ((D3DXVECTOR3(SCREEN_WIDTH/2 + MENURANKING_X * 4, 180.0f, 0.0f))) //ランキングの位置
#define MENURANKINGPOS_X ((-(MENURANKING_X / 2) - (MENURANKING_X * nCntDigit)))	//ランキングＸ位置のずらし
#define MENURANKINGPOS_Y ((- (MENURANKING_Y / 2) + ((40 + MENURANKING_Y) * nCntRanking)))	//ランキングＹ位置のずらし

#define MENURANKING_CHARACTER_POS ((D3DXVECTOR3(SCREEN_WIDTH / 2 - 500, 80.0f , 0.0f))) 

int g_nCntMenuRanking; //タイトルへの移行時間のカウント
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMenuRanking[MAX_TEXTURE_TYPE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenuRanking = NULL;		// 頂点バッファへのポインタ
static int				g_nMenuRanking[MAX_MENURANKING] = { 600000, 580000, 550000, 500000, 450000 };					// スコア

//int g_nYourScore;

//bool bYourScore;

int g_nCntMenuFade; //自動フェード
//=============================================================================
// 初期化処理
//=============================================================================
void InitMenuRanking(void)
{
	//背景初期化
	InitBG();

	//ブロックの初期化
	InitBlock();

	InitFg();
	InitFg2();

	//茂み
	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 75, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);

	//草
	SetFg(D3DXVECTOR3(100, 640, 0.0f), 2, 50, 25);
	SetFg(D3DXVECTOR3(640, 630, 0.0f), 2, 60, 30);
	SetFg(D3DXVECTOR3(450, 630, 0.0f), 2, 50, 30);
	SetFg(D3DXVECTOR3(600, 650, 0.0f), 2, 40, 20);


	//草並び
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2 + 10, 588, 0.0f), 0, 30, 470);

	//SetFg2(D3DXVECTOR3(620, 600, 0.0f), 3, 50, 25);
	//SetFg2(D3DXVECTOR3(500, 600, 0.0f), 3, 60, 30);



	//倒木
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2 + 10, 588, 0.0f), 0, 30, 470);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2 + 10, 588, 0.0f), 0, 30, 470);

	//SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2 , 660, 0.0f), 2, SCREEN_HEIGHT, SCREEN_WIDTH / 2 + 20);



	
	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);



	SetBlock(D3DXVECTOR3(0.0f, 580, 0), 1280.0f, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);


	g_nCntMenuFade = 0;

	//bYourScore = false;


	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// テクスチャの読み込み(数字)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MENURANKING, &g_pTextureMenuRanking[0]);
	// テクスチャの読み込み(ランキングモジ)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MENURANKING_CHARACTER, &g_pTextureMenuRanking[1]);

	// 頂点情報の作成
	MakeVertexMenuRanking(pDevice);




}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitMenuRanking(void)
{
	UnInitBG();

	//ブロックの初期化
	UnInitBlock();

	
	UnInitFg();
	UnInitFg2();


	for (int nCntRankingTex = 0; nCntRankingTex < MAX_TEXTURE_TYPE; nCntRankingTex++)
	{
		// テクスチャの開放
		if (g_pTextureMenuRanking[nCntRankingTex] != NULL)
		{
			g_pTextureMenuRanking[nCntRankingTex]->Release();
			g_pTextureMenuRanking[nCntRankingTex] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffMenuRanking != NULL)
	{
		g_pVtxBuffMenuRanking->Release();
		g_pVtxBuffMenuRanking = NULL;
	}





}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMenuRanking(void)
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
			SetFade(MODE_MENU);
		}
	}

	g_nCntMenuFade++;


	


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMenuRanking->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntRanking = 0; nCntRanking < MAX_MENURANKING; nCntRanking++)
	{
		for (int nCntDigit = 0; nCntDigit < 8; nCntDigit++, pVtx += 4)
		{

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
		}




	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMenuRanking->Lock(0, 0, (void**)&pVtx, 0);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMenuRanking(void)
{
	DrawBG();


	//ブロックの描画
	DrawBlock();

	DrawFg();
	DrawFg2();



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMenuRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);




	//=============================================================//
	//					ランキング文字							   //
	//=============================================================//
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMenuRanking[1]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (8 * MAX_MENURANKING) + 4, 2);



	//=============================================================//
	//					ランキング数字							   //
	//=============================================================//
	// テクスチャの設定

	for (int nCntRanking = 0; nCntRanking < (8 * MAX_MENURANKING); nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureMenuRanking[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);

	}




}



//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMenuRanking(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 8 * (MAX_MENURANKING + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenuRanking,
		NULL);



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMenuRanking->Lock(0, 0, (void**)&pVtx, 0);

	//=============================================================================//
	// ランキングのスコア(数字)													   //
	//=============================================================================//
	for (int nCntRanking = 0; nCntRanking < MAX_MENURANKING; nCntRanking++)
	{

		for (int nCntDigit = 0; nCntDigit < 8; nCntDigit++, pVtx += 4)
		{

			pVtx[0].pos = D3DXVECTOR3(-(MENURANKING_X / 2) - (MENURANKING_X * nCntDigit), -(MENURANKING_Y / 2) + ((30 + MENURANKING_Y) * nCntRanking), 0.0f) + MENURANKING_POS;
			pVtx[1].pos = D3DXVECTOR3((MENURANKING_X / 2) - (MENURANKING_X * nCntDigit), -(MENURANKING_Y / 2) + ((30 + MENURANKING_Y) * nCntRanking), 0.0f) + MENURANKING_POS;
			pVtx[2].pos = D3DXVECTOR3(-(MENURANKING_X / 2) - (MENURANKING_X * nCntDigit), (MENURANKING_Y / 2) + ((30 + MENURANKING_Y) * nCntRanking), 0.0f) + MENURANKING_POS;
			pVtx[3].pos = D3DXVECTOR3((MENURANKING_X / 2) - (MENURANKING_X * nCntDigit), (MENURANKING_Y / 2) + ((30 + MENURANKING_Y) * nCntRanking), 0.0f) + MENURANKING_POS;

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

			pVtx[0].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nMenuRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);


		}

	}

	pVtx += 4;


#if 1
	//=============================================================================//
	// 左上のランキングの文字											    	   //
	//=============================================================================//
	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-MENURANK_CHARACTER_X / 2, -MENURANK_CHARACTER_Y / 2, 0.0f) + MENURANKING_CHARACTER_POS;
	pVtx[1].pos = D3DXVECTOR3(MENURANK_CHARACTER_X / 2, -MENURANK_CHARACTER_Y / 2, 0.0f) + MENURANKING_CHARACTER_POS;
	pVtx[2].pos = D3DXVECTOR3(-MENURANK_CHARACTER_X / 2, MENURANK_CHARACTER_Y / 2, 0.0f) + MENURANKING_CHARACTER_POS;
	pVtx[3].pos = D3DXVECTOR3(MENURANK_CHARACTER_X / 2, MENURANK_CHARACTER_Y / 2, 0.0f) + MENURANKING_CHARACTER_POS;
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
	g_pVtxBuffMenuRanking->Lock(0, 0, (void**)&pVtx, 0);


}


//=============================================================================
// スコアのセット
//=============================================================================
void SetMenuRanking(int nScore)
{
	int nRank;
	int aEmptyData[6];
	aEmptyData[5] = nScore;

	for (int nCntNum = 0; nCntNum < MAX_MENURANKING; nCntNum++)
	{

		aEmptyData[nCntNum] = g_nMenuRanking[nCntNum];

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
		g_nMenuRanking[nCntNum] = aEmptyData[nCntNum];

	}
	//bYourScore = true;
	//g_nYourScore = nScore;

}
