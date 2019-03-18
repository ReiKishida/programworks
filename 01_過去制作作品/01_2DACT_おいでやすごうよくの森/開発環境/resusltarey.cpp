//=============================================================================
//
// リザルトアレイ処理 [resultarey.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultarey.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULTIAREY_NUMBER		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexResultArey(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RIGIT (3)	//桁数
#define NUMBER_X (30.0f) //Xの大きさ
#define NUMBER_Y (50.0f)//Yの大きさ


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultArey = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultArey = NULL;		// 頂点バッファへのポインタ
int						g_nResultArey = 0;					// スコア
int						g_aResultAreyNumber[MAX_RIGIT];	//桁数
D3DXVECTOR3				g_ResultAreypos;//スコア中心座標






float fAreyWidth;
float fAreyHeight;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResultArey(void)
{



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	g_nResultArey = 0;

	MODE *pMode;
	pMode = GetMode();

	g_ResultAreypos = D3DXVECTOR3(580, 400, 0.0f);
	fAreyWidth = 20.0f;
	fAreyHeight = 40.0f;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTIAREY_NUMBER, &g_pTextureResultArey);



	// 頂点情報の作成
	MakeVertexResultArey(pDevice);


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitResultArey(void)
{



	// テクスチャの開放
	if (g_pTextureResultArey != NULL)
	{
		g_pTextureResultArey->Release();
		g_pTextureResultArey = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffResultArey != NULL)
	{
		g_pVtxBuffResultArey->Release();
		g_pVtxBuffResultArey = NULL;
	}




}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResultArey(void)
{


	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	VERTEX_2D *pVtx;



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultArey->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RIGIT; nCntScore++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultAreyNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultAreyNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultArey->Unlock();


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultArey(void)
{

	int nCntResultArey;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultArey, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultArey);

	for (nCntResultArey = 0; nCntResultArey < 4; nCntResultArey++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultArey * 4, 2);
	}





}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexResultArey(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultArey,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultArey->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultArey = 0; nCntResultArey < MAX_RIGIT; nCntResultArey++)
	{


		pVtx[0].pos = D3DXVECTOR3(g_ResultAreypos.x - fAreyWidth, g_ResultAreypos.y - fAreyHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_ResultAreypos.x + fAreyWidth, g_ResultAreypos.y - fAreyHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_ResultAreypos.x - fAreyWidth, g_ResultAreypos.y + fAreyHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_ResultAreypos.x + fAreyWidth, g_ResultAreypos.y + fAreyHeight, 0.0f);


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
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aResultAreyNumber[nCntResultArey], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntResultArey], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aResultAreyNumber[nCntResultArey], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aResultAreyNumber[nCntResultArey], 1.0f);



		pVtx += 4;

		g_ResultAreypos.x += 40.0f;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultArey->Unlock();
}




//=============================================================================
// スコアのセット
//=============================================================================
void SetResultArey(int nScore)
{
	
		// 頂点情報の作成
		//		VERTEX_2D *pVtx;


		g_nResultArey += nScore;

		//g_nResultArey += g_nResultArey;



		g_aResultAreyNumber[0] = g_nResultArey % 1000 / 100;
		g_aResultAreyNumber[1] = g_nResultArey % 100 / 10;
		g_aResultAreyNumber[2] = g_nResultArey % 10;




	
}