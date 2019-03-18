//=============================================================================
//
// アイテムカウンター処理 [itemcounter.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "itemcounter.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexCounter(LPDIRECT3DDEVICE9 pDevice);

#define MAX_ITEMCOUNTER (3)	//アイテム数
#define MAX_SCORE (3)	//桁数
#define SCORE_X (12.0f)
#define SCORE_Y (27.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCounter = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCounter = NULL;		// 頂点バッファへのポインタ
int						g_nCoinScore;					// スコア
int						g_aCoinNumber[MAX_SCORE];
D3DXVECTOR3				g_aCounterpos;	//スコア中心座標

//=============================================================================
// 初期化処理
//=============================================================================
void InitItemCounter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	
		g_nCoinScore = 0;
	


	g_aCounterpos = D3DXVECTOR3(150, 40, 0.0f);


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_SCORE,		// ファイルの名前
		&g_pTextureCounter);	// テクスチャへのポインタ

							// 頂点情報の作成
	MakeVertexCounter(pDevice);


}
//=============================================================================
// 終了処理
//=============================================================================
void UninitItemCounter(void)
{

	// テクスチャの開放
	if (g_pTextureCounter != NULL)
	{
		g_pTextureCounter->Release();
		g_pTextureCounter = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffCounter != NULL)
	{
		g_pVtxBuffCounter->Release();
		g_pVtxBuffCounter = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItemCounter(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItemCounter(void)
{
	int nCntCounter;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCounter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCounter);

	for (nCntCounter = 0; nCntCounter < (MAX_SCORE * MAX_ITEMCOUNTER); nCntCounter++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCounter * 4, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexCounter(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_SCORE * MAX_ITEMCOUNTER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCounter,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCounter = 0; nCntCounter < MAX_SCORE; nCntCounter++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_aCounterpos.x - SCORE_X, g_aCounterpos.y - SCORE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCounterpos.x + SCORE_X, g_aCounterpos.y - SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCounterpos.x - SCORE_X, g_aCounterpos.y + SCORE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCounterpos.x + SCORE_X, g_aCounterpos.y + SCORE_Y, 0.0f);


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

		g_aCounterpos.x -= 30;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffCounter->Unlock();
}


//=============================================================================
// スコアの加算
//=============================================================================
void AddCounter(int nValue)
{

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	
		g_nCoinScore += nValue;
	


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCounter = 0; nCntCounter < MAX_SCORE; nCntCounter++)
	{
		g_aCoinNumber[0] = g_nCoinScore % 10;
		g_aCoinNumber[1] = g_nCoinScore % 100 / 10;
		g_aCoinNumber[2] = g_nCoinScore % 1000 / 100;

	
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aCoinNumber[nCntCounter], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aCoinNumber[nCntCounter], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aCoinNumber[nCntCounter], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aCoinNumber[nCntCounter], 1.0f);


		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffCounter->Unlock();

}

//=============================================================================
// スコアの取得
//=============================================================================
int GetItemCounter(void)
{
	return g_nCoinScore;
}