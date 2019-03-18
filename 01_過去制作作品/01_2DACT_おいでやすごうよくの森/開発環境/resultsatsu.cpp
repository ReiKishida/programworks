//=============================================================================
//
// リザルトおさつ処理 [resultsatsu.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultsatsu.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULTISATSU_NUMBER		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexResultSatsu(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//桁数
#define NUMBER_X (30.0f) //Xの大きさ
#define NUMBER_Y (50.0f)//Yの大きさ


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultSatsu = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultSatsu = NULL;		// 頂点バッファへのポインタ
int						g_nResultSatsu = 0;					// スコア
int						g_aResultSatsuNumber[MAX_RIGIT];	//桁数
D3DXVECTOR3				g_ResultSatsupos;//スコア中心座標






float fSatsuWidth;
float fSatsuHeight;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResultSatsu(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	g_nResultSatsu = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultSatsupos = D3DXVECTOR3(570, 300, 0.0f);
	fSatsuWidth = 20.0f;
	fSatsuHeight = 40.0f;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTISATSU_NUMBER, &g_pTextureResultSatsu);



	// 頂点情報の作成
	MakeVertexResultSatsu(pDevice);


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitResultSatsu(void)
{



	// テクスチャの開放
	if (g_pTextureResultSatsu != NULL)
	{
		g_pTextureResultSatsu->Release();
		g_pTextureResultSatsu = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffResultSatsu != NULL)
	{
		g_pVtxBuffResultSatsu->Release();
		g_pVtxBuffResultSatsu = NULL;
	}




}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResultSatsu(void)
{


	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	VERTEX_2D *pVtx;



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultSatsu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultSatsuNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultSatsuNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultSatsu->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultSatsu(void)
{

	int nCntResultSatsu;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultSatsu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultSatsu);

	for (nCntResultSatsu = 0; nCntResultSatsu < MAX_RIGIT; nCntResultSatsu++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultSatsu * 4, 2);
	}





}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexResultSatsu(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultSatsu,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultSatsu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultSatsu = 0; nCntResultSatsu < MAX_RIGIT; nCntResultSatsu++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultSatsupos.x - fSatsuWidth, g_ResultSatsupos.y - fSatsuHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultSatsupos.x + fSatsuWidth, g_ResultSatsupos.y - fSatsuHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultSatsupos.x - fSatsuWidth, g_ResultSatsupos.y + fSatsuHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultSatsupos.x + fSatsuWidth, g_ResultSatsupos.y + fSatsuHeight, 0.0f);


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
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultSatsuNumber[nCntResultSatsu], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntResultSatsu], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultSatsuNumber[nCntResultSatsu], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultSatsuNumber[nCntResultSatsu], 1.0f);



		pVtx += 4;

		g_ResultSatsupos.x += 40.0f;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultSatsu->Unlock();
}




//=============================================================================
// スコアのセット
//=============================================================================
void SetResultSatsu(int nScore)
{

	// 頂点情報の作成
	//		VERTEX_2D *pVtx;


	g_nResultSatsu += nScore;

	//g_nResultSatsu += g_nResultSatsu;



	g_aResultSatsuNumber[0] = g_nResultSatsu % 1000 / 100;
	g_aResultSatsuNumber[1] = g_nResultSatsu % 100 / 10;
	g_aResultSatsuNumber[2] = g_nResultSatsu % 10;





}