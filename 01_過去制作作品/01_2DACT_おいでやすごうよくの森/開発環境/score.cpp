//=============================================================================
//
// スコア処理 [score.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

#define MAX_SCORE (8)
#define SCORE_X (15.0f)
#define SCORE_Y (30.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ
static int				g_nScore;					// スコア
int						g_aNumber[MAX_SCORE];				
D3DXVECTOR3				g_Scorepos;//スコア中心座標

//=============================================================================
// 初期化処理
//=============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	g_nScore = 0;

	//AddScore(99999);

	g_Scorepos = D3DXVECTOR3(1050, -200, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_SCORE,		// ファイルの名前
		&g_pTextureScore);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexScore(pDevice);

	
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{

	// テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore *4, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	

	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_Scorepos.x - SCORE_X, g_Scorepos.y - SCORE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Scorepos.x + SCORE_X, g_Scorepos.y - SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Scorepos.x - SCORE_X, g_Scorepos.y + SCORE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Scorepos.x + SCORE_X, g_Scorepos.y + SCORE_Y, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


		pVtx += 4;

		g_Scorepos.x += 30;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}


//=============================================================================
// スコアの加算
//=============================================================================
void AddScore(int nValue)
{

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	g_nScore += nValue;
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
     	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		g_aNumber[0] = g_nScore / 100000000;
		g_aNumber[1] = g_nScore % 10000000 / 1000000;
		g_aNumber[2] = g_nScore % 1000000 / 100000;
		g_aNumber[3] = g_nScore % 100000 / 10000;
		g_aNumber[4] = g_nScore % 10000 / 1000;
		g_aNumber[5] = g_nScore % 1000 / 100;
		g_aNumber[6] = g_nScore % 100 / 10;
		g_aNumber[7] = g_nScore % 10;


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aNumber[nCntScore], 1.0f);


		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();

}

//=============================================================================
// スコアの取得
//=============================================================================
int GetScore(void)
{
	return g_nScore;
}