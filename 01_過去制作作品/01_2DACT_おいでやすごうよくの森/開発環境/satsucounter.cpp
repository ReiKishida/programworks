//=============================================================================
//
// 札ウンター処理 [satsucounter.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "satsucounter.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSatsuCounter(LPDIRECT3DDEVICE9 pDevice);

#define MAX_SATSUCOUNTER (3)	//アイテム数
#define MAX_SASTU (3)	//桁数
#define SCORE_X (12.0f)
#define SCORE_Y (27.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureSatsuCounter = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSatsuCounter = NULL;		// 頂点バッファへのポインタ
int						g_nSatsuCounter;					// スコア
int						g_aSatsuNumber[MAX_SASTU];
D3DXVECTOR3				g_aSatsuCounterpos;//スコア中心座標

//=============================================================================
// 初期化処理
//=============================================================================
void InitSatsuCounter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	
		g_nSatsuCounter = 0;
	



	g_aSatsuCounterpos = D3DXVECTOR3(520, 40, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_SCORE,		// ファイルの名前
		&g_pTextureSatsuCounter);	// テクスチャへのポインタ

									// 頂点情報の作成
	MakeVertexSatsuCounter(pDevice);


}
//=============================================================================
// 終了処理
//=============================================================================
void UninitSatsuCounter(void)
{

	// テクスチャの開放
	if (g_pTextureSatsuCounter != NULL)
	{
		g_pTextureSatsuCounter->Release();
		g_pTextureSatsuCounter = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffSatsuCounter != NULL)
	{
		g_pVtxBuffSatsuCounter->Release();
		g_pVtxBuffSatsuCounter = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSatsuCounter(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSatsuCounter(void)
{
	int nCntSatsu;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSatsuCounter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSatsuCounter);

	for (nCntSatsu = 0; nCntSatsu < (MAX_SASTU * MAX_SATSUCOUNTER); nCntSatsu++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSatsu * 4, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexSatsuCounter(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_SASTU * MAX_SATSUCOUNTER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSatsuCounter,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSatsuCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSatsu = 0; nCntSatsu < MAX_SASTU; nCntSatsu++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_aSatsuCounterpos.x - SCORE_X, g_aSatsuCounterpos.y - SCORE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSatsuCounterpos.x + SCORE_X, g_aSatsuCounterpos.y - SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aSatsuCounterpos.x - SCORE_X, g_aSatsuCounterpos.y + SCORE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSatsuCounterpos.x + SCORE_X, g_aSatsuCounterpos.y + SCORE_Y, 0.0f);


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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


		pVtx += 4;

		g_aSatsuCounterpos.x -= 30;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffSatsuCounter->Unlock();
}


//=============================================================================
// スコアの加算
//=============================================================================
void AddSatsuCounter(int nValue)
{

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	
		g_nSatsuCounter += nValue;
	

	//g_nSatsuCounter += g_nSatsuCounter;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSatsuCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSatsu = 0; nCntSatsu < MAX_SASTU; nCntSatsu++)
	{
		g_aSatsuNumber[0] = g_nSatsuCounter % 10;
		g_aSatsuNumber[1] = g_nSatsuCounter % 100 / 10;
		g_aSatsuNumber[2] = g_nSatsuCounter % 1000 / 100;


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aSatsuNumber[nCntSatsu], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aSatsuNumber[nCntSatsu], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aSatsuNumber[nCntSatsu], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aSatsuNumber[nCntSatsu], 1.0f);


		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffSatsuCounter->Unlock();

}

//=============================================================================
// スコアの取得
//=============================================================================
int GetSatsuCounter(void)
{
	return g_nSatsuCounter;
}