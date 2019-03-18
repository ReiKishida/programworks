//=============================================================================
//
// アレイカウンター処理 [areycounter.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "areycounter.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexAreyCounter(LPDIRECT3DDEVICE9 pDevice);

#define MAX_AREYCOUNTER (3)	//アイテム数
#define MAX_AREY (3)	//桁数
#define SCORE_X (12.0f)
#define SCORE_Y (27.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureAreyCounter = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAreyCounter = NULL;		// 頂点バッファへのポインタ
int						g_nAreyCounter;					// スコア
int						g_aAreyNumber[MAX_AREY];
D3DXVECTOR3				g_AreyCounterpos;//スコア中心座標

//=============================================================================
// 初期化処理
//=============================================================================
void InitAreyCounter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	
		g_nAreyCounter = 0;
	


	g_AreyCounterpos = D3DXVECTOR3(720, 40, 0.0f);


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_SCORE,		// ファイルの名前
		&g_pTextureAreyCounter);	// テクスチャへのポインタ

									// 頂点情報の作成
	MakeVertexAreyCounter(pDevice);


}
//=============================================================================
// 終了処理
//=============================================================================
void UninitAreyCounter(void)
{

	// テクスチャの開放
	if (g_pTextureAreyCounter != NULL)
	{
		g_pTextureAreyCounter->Release();
		g_pTextureAreyCounter = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffAreyCounter != NULL)
	{
		g_pVtxBuffAreyCounter->Release();
		g_pVtxBuffAreyCounter = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAreyCounter(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAreyCounter(void)
{
	int nCntArey;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAreyCounter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureAreyCounter);

	for (nCntArey = 0; nCntArey < (MAX_AREY * MAX_AREYCOUNTER); nCntArey++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntArey * 4, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexAreyCounter(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_AREY * MAX_AREYCOUNTER),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAreyCounter,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAreyCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntArey = 0; nCntArey < MAX_AREY; nCntArey++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_AreyCounterpos.x - SCORE_X, g_AreyCounterpos.y - SCORE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_AreyCounterpos.x + SCORE_X, g_AreyCounterpos.y - SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_AreyCounterpos.x - SCORE_X, g_AreyCounterpos.y + SCORE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_AreyCounterpos.x + SCORE_X, g_AreyCounterpos.y + SCORE_Y, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


		pVtx += 4;

		g_AreyCounterpos.x -= 30;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffAreyCounter->Unlock();
}


//=============================================================================
// スコアの加算
//=============================================================================
void AddAreyCounter(int nValue)
{

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	
		g_nAreyCounter += nValue;
	

	//g_nAreyCounter += g_nAreyCounter;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAreyCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntArey = 0; nCntArey < MAX_AREY; nCntArey++)
	{
		g_aAreyNumber[0] = g_nAreyCounter% 10;
		g_aAreyNumber[1] = g_nAreyCounter % 100 / 10;
		g_aAreyNumber[2] = g_nAreyCounter% 1000 / 100;


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aAreyNumber[nCntArey], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aAreyNumber[nCntArey], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aAreyNumber[nCntArey], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aAreyNumber[nCntArey], 1.0f);


		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffAreyCounter->Unlock();

}

//=============================================================================
// スコアの取得
//=============================================================================
int GetAreyCounter(void)
{
	return g_nAreyCounter;
}