//=============================================================================
//
// ポーズ背景処理 [bg.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "SelectBG.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_TEXTUER		"data/TEXTURE/nextstage.png"	//読み込むテクスチャ
#define PAUSE_POS_X			(230)					    //ポーズ背景左上X
#define PAUSE_POS_Y			(180)					    //ポーズ背景左上Y
#define PAUSE_WIDTH			(990)					    //ポーズ背景の幅
#define PAUSE_HEIGHT		(580)					    //ポーズ背景高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureSelectBG = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffSelectBG = NULL;

//=============================================================================
// ポリゴン初期化処理[InitPolygon]
//=============================================================================
void InitSelectBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D*pVtx;//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTUER, &g_pTextureSelectBG);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectBG, NULL);

	//頂点をロックして頂点データへのポインタを取得
	g_pVtxBuffSelectBG->Lock(0, 0, (void**)&pVtx, 0);

	//----------マスク-----------
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//一つ目の頂点の場所指定
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);//二つ目の頂点の場所指定（右回り）
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）

	 //rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//ポーズ背景色設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);//一つ目の頂点の色指定（右回り）
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);//一つ目の頂点の色指定（右回り）
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);//一つ目の頂点の色指定（右回り）
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);//一つ目の頂点の色指定（右回り）

	//ポーズ背景テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//背景--------------------------------
	//ポーズ背景の座標設定
	pVtx[4].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);//一つ目の頂点の場所指定
	pVtx[5].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);//二つ目の頂点の場所指定（右回り）
	pVtx[6].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）
	pVtx[7].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）

	//rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//ポーズ背景色設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//二つ目の頂点の色指定（右回り）
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//三つ目の頂点の色指定（右回り）
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//四つ目の頂点の色指定（右回り）

	//ポーズ背景テクスチャの設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	g_pVtxBuffSelectBG->Unlock();
}

//=============================================================================
//ポリゴン終了処理 [UninitPolygon]
//=============================================================================
void UninitSelectBG(void)
{
	//テクスチャの破棄
	if (g_pTextureSelectBG != NULL)
	{
		g_pTextureSelectBG->Release();
		g_pTextureSelectBG = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSelectBG != NULL)
	{
		g_pVtxBuffSelectBG->Release();
		g_pVtxBuffSelectBG = NULL;
	}

}

//=============================================================================
// ポリゴン更新処理[UpdataPolygon]
//=============================================================================
void UpdateSelectBG(void)
{

}

//=============================================================================
// ポリゴン描写処理[DrawPolygon]
//=============================================================================
void DrawSelectBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスを取得する

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelectBG);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}