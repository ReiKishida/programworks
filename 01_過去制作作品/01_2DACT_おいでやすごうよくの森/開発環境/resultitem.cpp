//=============================================================================
//
// リザルトアイテム処理 [resultitem.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultitem.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULTITEM_NUMBER		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexResultCoin(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//桁数
#define NUMBER_X (25.0f) //Xの大きさ
#define NUMBER_Y (45.0f)//Yの大きさ


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultCoin = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultCoin = NULL;		// 頂点バッファへのポインタ
int						g_nResultCoin = 0;					// スコア
int						g_aResultCoinNumber[MAX_RIGIT];	//桁数
D3DXVECTOR3				g_ResultCoinpos;//スコア中心座標






float fNumberWidth;
float fNumberHeight;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResultItem(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	g_nResultCoin = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultCoinpos = D3DXVECTOR3(555, 100, 0.0f);
	fNumberWidth = 20.0f;
	fNumberHeight = 40.0f;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTITEM_NUMBER, &g_pTextureResultCoin);



	// 頂点情報の作成
	MakeVertexResultCoin(pDevice);


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitResultItem(void)
{



	// テクスチャの開放
	if (g_pTextureResultCoin != NULL)
	{
		g_pTextureResultCoin->Release();
		g_pTextureResultCoin = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffResultCoin != NULL)
	{
		g_pVtxBuffResultCoin->Release();
		g_pVtxBuffResultCoin = NULL;
	}




}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResultItem(void)
{


	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	VERTEX_2D *pVtx;



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultCoinNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultCoinNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultCoin->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultItem(void)
{

	int nCntResultItem;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultCoin, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultCoin);

	for (nCntResultItem = 0; nCntResultItem < MAX_RIGIT; nCntResultItem++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultItem * 4, 2);
	}





}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexResultCoin(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultCoin,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultItem = 0; nCntResultItem < MAX_RIGIT; nCntResultItem++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultCoinpos.x - fNumberWidth, g_ResultCoinpos.y - fNumberHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultCoinpos.x + fNumberWidth, g_ResultCoinpos.y - fNumberHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultCoinpos.x - fNumberWidth, g_ResultCoinpos.y + fNumberHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultCoinpos.x + fNumberWidth, g_ResultCoinpos.y + fNumberHeight, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultCoinNumber[nCntResultItem], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntResultItem], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultCoinNumber[nCntResultItem], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultCoinNumber[nCntResultItem], 1.0f);



		pVtx += 4;

		g_ResultCoinpos.x += 40.0f;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultCoin->Unlock();
}




//=============================================================================
// スコアのセット
//=============================================================================
void SetResultCoin(int nScore)
{
	
		// 頂点情報の作成
		//		VERTEX_2D *pVtx;


		g_nResultCoin += nScore;

		//g_nResultCoin += g_nResultCoin;


		
		g_aResultCoinNumber[0] = g_nResultCoin % 1000 / 100;
		g_aResultCoinNumber[1] = g_nResultCoin % 100 / 10;
		g_aResultCoinNumber[2] = g_nResultCoin % 10;




	
}