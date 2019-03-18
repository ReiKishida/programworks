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
#define	TEXTURE_HIGHSCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexHighScore01(LPDIRECT3DDEVICE9 pDevice);

#define MAX_HIGHSCORE (8)	//桁数
#define HIGHSCORE_X (20.0f) //Xの大きさ
#define HIGHSCORE_Y (50.0f)//Yの大きさ

//#define HIGH_X (150.0f) //Xの大きさ
//#define HIGH_Y (60.0f)//Yの大きさ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureHighScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScore = NULL;		// 頂点バッファへのポインタ
int						g_nHighScore = 0;					// スコア
int						g_aHighScoreNumber[MAX_HIGHSCORE];	//桁数
D3DXVECTOR3				g_HighScorepos;//スコア中心座標

D3DXCOLOR				g_HighScoreColor0;
D3DXCOLOR				g_HighScoreColor1;
D3DXCOLOR				g_HighScoreColor2;
D3DXCOLOR				g_HighScoreColor3;


float fWidth;
float fHeight;

int nCntHigh;	//ハイスコア点滅

//=============================================================================
// 初期化処理
//=============================================================================
void InitHighScore(void)
{

	 
	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	g_nHighScore = 0;
	nCntHigh = 0;

	g_HighScoreColor0 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HighScoreColor1 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HighScoreColor2 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HighScoreColor3 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	MODE *pMode;
	pMode = GetMode();
	
		g_HighScorepos = D3DXVECTOR3(820, SCREEN_HEIGHT - 150, 0.0f);
		fWidth = 20.0f;
		fHeight = 50.0f;
	

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HIGHSCORE, &g_pTextureHighScore);



	// 頂点情報の作成
	MakeVertexHighScore01(pDevice);


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitHighScore(void)
{
	


	// テクスチャの開放
	if (g_pTextureHighScore != NULL)
	{
		g_pTextureHighScore->Release();
		g_pTextureHighScore = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffHighScore != NULL)
	{
		g_pVtxBuffHighScore->Release();
		g_pVtxBuffHighScore = NULL;
	}




}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHighScore(void)
{
	

	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	nCntHigh++;
	nCntHigh = nCntHigh % 30;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRanking = 0; nCntRanking < MAX_HIGHSCORE; nCntRanking++)
	{
		
			if (nCntHigh < 15)
			{
				g_HighScoreColor0 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
				g_HighScoreColor1 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
				g_HighScoreColor2 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
				g_HighScoreColor3 = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
			}
			else if (nCntHigh >= 15)
			{
				g_HighScoreColor0 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				g_HighScoreColor1 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				g_HighScoreColor2 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				g_HighScoreColor3 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			// 頂点カラーの設定
			pVtx[0].col = g_HighScoreColor0;
			pVtx[1].col = g_HighScoreColor1;
			pVtx[2].col = g_HighScoreColor2;
			pVtx[3].col = g_HighScoreColor3;

			pVtx += 4;
	}
		// 頂点バッファをアンロックする
		g_pVtxBuffHighScore->Unlock();

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntScore = 0; nCntScore < MAX_HIGHSCORE; nCntScore++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aHighScoreNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aHighScoreNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHighScore->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHighScore(void)
{
	
	int nCntHigh;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHighScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureHighScore);

	for (nCntHigh = 0; nCntHigh < MAX_HIGHSCORE; nCntHigh++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHigh * 4, 2);
	}





}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexHighScore01(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HIGHSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScore,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHigh = 0; nCntHigh < MAX_HIGHSCORE; nCntHigh++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_HighScorepos.x - fWidth, g_HighScorepos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_HighScorepos.x + fWidth, g_HighScorepos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_HighScorepos.x - fWidth, g_HighScorepos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_HighScorepos.x + fWidth, g_HighScorepos.y + fHeight, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aHighScoreNumber[nCntHigh], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntHigh], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aHighScoreNumber[nCntHigh], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aHighScoreNumber[nCntHigh], 1.0f);



		pVtx += 4;

		g_HighScorepos.x += 45;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHighScore->Unlock();
}




//=============================================================================
// スコアのセット
//=============================================================================
void SetHighScore(int nScore)
{
	if (nScore > g_nHighScore)
	{
		// 頂点情報の作成
		//		VERTEX_2D *pVtx;


		g_nHighScore = nScore;

		//g_nHighScore += g_nHighScore;


		g_aHighScoreNumber[0] = g_nHighScore / 100000000;
		g_aHighScoreNumber[1] = g_nHighScore % 10000000 / 1000000;
		g_aHighScoreNumber[2] = g_nHighScore % 1000000 / 100000;
		g_aHighScoreNumber[3] = g_nHighScore % 100000 / 10000;
		g_aHighScoreNumber[4] = g_nHighScore % 10000 / 1000;
		g_aHighScoreNumber[5] = g_nHighScore % 1000 / 100;
		g_aHighScoreNumber[6] = g_nHighScore % 100 / 10;
		g_aHighScoreNumber[7] = g_nHighScore % 10;




	}
}