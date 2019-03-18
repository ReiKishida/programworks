//=============================================================================
//
// ランキング処理 [HighScore.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "highscore.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//#include "bg.h"
//#include "block.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_GAME_HIGHSCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_GAME_HIGHSCOREWAKU		"data/TEXTURE/highs.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexGameHighScore(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexGameHighScoreWaku(LPDIRECT3DDEVICE9 pDevice); //ハイスコアの枠

#define MAX_GAMEHIGHSCORE (8)	//桁数
#define GAMEHIGHSCORE_X (30.0f) //Xの大きさ
#define GAMEHIGHSCORE_Y (60.0f)//Yの大きさ

#define GAMEHIGHSCOREWAKU_X (150.0f) //Xの大きさ
#define GAMEHIGHSCOREWAKU_Y (60.0f)//Yの大きさ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureGameHighScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameHighScore = NULL;		// 頂点バッファへのポインタ
int						g_nGameHighScore;					// スコア
int						g_aGameHighScoreNumber[MAX_GAMEHIGHSCORE];	//桁数
D3DXVECTOR3				g_GameHighScorepos;//スコア中心座標


LPDIRECT3DTEXTURE9		g_pTextureGameHighScoreWaku = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameHighScoreWaku = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3				g_GameHighScoreWakupos;//スコア中心座標


float fGameWidth;
float fGameHeight;

bool HighScore;

//=============================================================================
// 初期化処理
//=============================================================================
void InitGameHighScore(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	if (HighScore == false)
	{
		g_nGameHighScore = 600000;
	}

	MODE *pMode;
	pMode = GetMode();

	g_GameHighScorepos = D3DXVECTOR3(980,82, 0.0f);
	g_GameHighScoreWakupos = D3DXVECTOR3(1080, 65, 0.0f);

	fGameWidth = 15.0f;
	fGameHeight = 30.0f;

	//HighScore = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME_HIGHSCORE, &g_pTextureGameHighScore);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME_HIGHSCOREWAKU, &g_pTextureGameHighScoreWaku);



	// 頂点情報の作成
	MakeVertexGameHighScore(pDevice);

	MakeVertexGameHighScoreWaku(pDevice);

	//SetGameHighScore(500000);
}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitGameHighScore(void)
{



	// テクスチャの開放
	if (g_pTextureGameHighScore != NULL)
	{
		g_pTextureGameHighScore->Release();
		g_pTextureGameHighScore = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffGameHighScore != NULL)
	{
		g_pVtxBuffGameHighScore->Release();
		g_pVtxBuffGameHighScore = NULL;
	}


	// テクスチャの開放
	if (g_pTextureGameHighScoreWaku != NULL)
	{
		g_pTextureGameHighScoreWaku->Release();
		g_pTextureGameHighScoreWaku = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffGameHighScoreWaku != NULL)
	{
		g_pVtxBuffGameHighScoreWaku->Release();
		g_pVtxBuffGameHighScoreWaku = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameHighScore(void)
{


	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	VERTEX_2D *pVtx;


	g_aGameHighScoreNumber[0] = g_nGameHighScore / 100000000;
	g_aGameHighScoreNumber[1] = g_nGameHighScore % 10000000 / 1000000;
	g_aGameHighScoreNumber[2] = g_nGameHighScore % 1000000 / 100000;
	g_aGameHighScoreNumber[3] = g_nGameHighScore % 100000 / 10000;
	g_aGameHighScoreNumber[4] = g_nGameHighScore % 10000 / 1000;
	g_aGameHighScoreNumber[5] = g_nGameHighScore % 1000 / 100;
	g_aGameHighScoreNumber[6] = g_nGameHighScore % 100 / 10;
	g_aGameHighScoreNumber[7] = g_nGameHighScore % 10;


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_GAMEHIGHSCORE; nCntScore++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aGameHighScoreNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aGameHighScoreNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffGameHighScore->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGameHighScore(void)
{

	int nCntGameHighScore;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameHighScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGameHighScore);

	for (nCntGameHighScore = 0; nCntGameHighScore < MAX_GAMEHIGHSCORE; nCntGameHighScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGameHighScore * 4, 2);
	}






	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameHighScoreWaku, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGameHighScoreWaku);

	
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexGameHighScore(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAMEHIGHSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameHighScore,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGameHighScore = 0; nCntGameHighScore < MAX_GAMEHIGHSCORE; nCntGameHighScore++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_GameHighScorepos.x - fGameWidth, g_GameHighScorepos.y - fGameHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameHighScorepos.x + fGameWidth, g_GameHighScorepos.y - fGameHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameHighScorepos.x - fGameWidth, g_GameHighScorepos.y + fGameHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameHighScorepos.x + fGameWidth, g_GameHighScorepos.y + fGameHeight, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aGameHighScoreNumber[nCntGameHighScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntGameHighScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aGameHighScoreNumber[nCntGameHighScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aGameHighScoreNumber[nCntGameHighScore], 1.0f);



		pVtx += 4;

		g_GameHighScorepos.x += 30;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffGameHighScore->Unlock();
}


//=============================================================================
// 頂点情報の作成(ハイスコア枠)
//=============================================================================
void MakeVertexGameHighScoreWaku(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameHighScoreWaku,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameHighScoreWaku->Lock(0, 0, (void**)&pVtx, 0);

	


		pVtx[0].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x - GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y - GAMEHIGHSCOREWAKU_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x + GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y - GAMEHIGHSCOREWAKU_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x - GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y + GAMEHIGHSCOREWAKU_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameHighScoreWakupos.x + GAMEHIGHSCOREWAKU_X, g_GameHighScoreWakupos.y + GAMEHIGHSCOREWAKU_Y, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	

	
	// 頂点バッファをアンロックする
	g_pVtxBuffGameHighScoreWaku->Unlock();
}




//=============================================================================
// スコアのセット
//=============================================================================
void SetGameHighScore(int nScore)
{
	if (nScore >= g_nGameHighScore)
	{
		// 頂点情報の作成
		//		VERTEX_2D *pVtx;


		g_nGameHighScore = nScore;

		//g_nGameHighScore += g_nGameHighScore;


		g_aGameHighScoreNumber[0] = g_nGameHighScore / 100000000;
		g_aGameHighScoreNumber[1] = g_nGameHighScore % 10000000 / 1000000;
		g_aGameHighScoreNumber[2] = g_nGameHighScore % 1000000 / 100000;
		g_aGameHighScoreNumber[3] = g_nGameHighScore % 100000 / 10000;
		g_aGameHighScoreNumber[4] = g_nGameHighScore % 10000 / 1000;
		g_aGameHighScoreNumber[5] = g_nGameHighScore % 1000 / 100;
		g_aGameHighScoreNumber[6] = g_nGameHighScore % 100 / 10;
		g_aGameHighScoreNumber[7] = g_nGameHighScore % 10;

		HighScore = true;


	}
}