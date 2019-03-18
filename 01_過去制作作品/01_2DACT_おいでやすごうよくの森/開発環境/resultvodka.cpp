//=============================================================================
//
// リザルトウォッカ処理 [resultvodka.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultVodka.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULTIVODKA_NUMBER		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexResultVodka(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//桁数
#define NUMBER_X (30.0f) //Xの大きさ
#define NUMBER_Y (50.0f)//Yの大きさ


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultVodka = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultVodka = NULL;		// 頂点バッファへのポインタ
int						g_nResultVodka = 0;					// スコア
int						g_aResultVodkaNumber[MAX_RIGIT];	//桁数
D3DXVECTOR3				g_ResultVodkapos;//スコア中心座標






float fVodkaWidth;
float fVodkaHeight;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResultVodka(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	g_nResultVodka = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultVodkapos = D3DXVECTOR3(555, 200, 0.0f);
	fVodkaWidth = 20.0f;
	fVodkaHeight = 40.0f;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTIVODKA_NUMBER, &g_pTextureResultVodka);



	// 頂点情報の作成
	MakeVertexResultVodka(pDevice);


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitResultVodka(void)
{



	// テクスチャの開放
	if (g_pTextureResultVodka != NULL)
	{
		g_pTextureResultVodka->Release();
		g_pTextureResultVodka = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffResultVodka != NULL)
	{
		g_pVtxBuffResultVodka->Release();
		g_pVtxBuffResultVodka = NULL;
	}




}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResultVodka(void)
{


	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	VERTEX_2D *pVtx;



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultVodka->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultVodkaNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultVodkaNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultVodka->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultVodka(void)
{

	int nCntResultVodka;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultVodka, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultVodka);

	for (nCntResultVodka = 0; nCntResultVodka < MAX_RIGIT; nCntResultVodka++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultVodka * 4, 2);
	}





}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexResultVodka(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultVodka,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultVodka->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultVodka = 0; nCntResultVodka < MAX_RIGIT; nCntResultVodka++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultVodkapos.x - fVodkaWidth, g_ResultVodkapos.y - fVodkaHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultVodkapos.x + fVodkaWidth, g_ResultVodkapos.y - fVodkaHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultVodkapos.x - fVodkaWidth, g_ResultVodkapos.y + fVodkaHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultVodkapos.x + fVodkaWidth, g_ResultVodkapos.y + fVodkaHeight, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultVodkaNumber[nCntResultVodka], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntResultVodka], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultVodkaNumber[nCntResultVodka], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultVodkaNumber[nCntResultVodka], 1.0f);



		pVtx += 4;

		g_ResultVodkapos.x += 40.0f;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultVodka->Unlock();
}




//=============================================================================
// スコアのセット
//=============================================================================
void SetResultVodka(int nScore)
{

	// 頂点情報の作成
	//		VERTEX_2D *pVtx;


	g_nResultVodka += nScore;

	//g_nResultVodka += g_nResultVodka;



	g_aResultVodkaNumber[0] = g_nResultVodka % 1000 / 100;
	g_aResultVodkaNumber[1] = g_nResultVodka % 100 / 10;
	g_aResultVodkaNumber[2] = g_nResultVodka % 10;





}