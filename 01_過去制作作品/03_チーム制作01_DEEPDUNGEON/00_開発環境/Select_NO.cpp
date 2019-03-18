//=============================================================================
//
// セレクト画面No処理 [Select_No.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "Select_NO.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PAUSE_TEXTUER	"data/TEXTURE/no.png"		//読み込むテクスチャ
#define PAUSE_POS_X		(680)					    //ポーズ文字（NO）左上X
#define PAUSE_POS_Y		(350)					    //ポーズ文字（NO）左上Y
#define PAUSE_WIDTH		(920)					    //ポーズ文字（NO）の幅
#define PAUSE_HEIGHT	(500)					    //ポーズ文字（NO）高さ

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureSelectNO = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffSelectNO = NULL;
NOSTATE						g_NoState;

//=============================================================================
// 初期化処理[InitPolygon]
//=============================================================================
void InitSelectNo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	VERTEX_2D*pVtx;//頂点情報へのポインタ

	g_NoState = NOSTATE_NOSELECT;

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTUER, &g_pTextureSelectNO);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectNO, NULL);

	//頂点をロックして頂点データへのポインタを取得
	g_pVtxBuffSelectNO->Lock(0, 0, (void**)&pVtx, 0);

	//セレクト画面Noの座標設定
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);//一つ目の頂点の場所指定
	pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);//二つ目の頂点の場所指定（右回り）
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）
	pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//セレクト画面No色設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）

	//セレクト画面Noテクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	g_pVtxBuffSelectNO->Unlock();
}

//=============================================================================
//終了処理 [UninitPolygon]
//=============================================================================
void UninitSelectNo(void)
{
	//テクスチャの破棄
	if (g_pTextureSelectNO != NULL)
	{
		g_pTextureSelectNO->Release();
		g_pTextureSelectNO = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSelectNO != NULL)
	{
		g_pVtxBuffSelectNO->Release();
		g_pVtxBuffSelectNO = NULL;
	}
}

//=============================================================================
// 更新処理[UpdataPolygon]
//=============================================================================
void UpdateSelectNo(void)
{
	VERTEX_2D*pVtx;//頂点情報へのポインタ

   //頂点をロックして頂点データへのポインタを取得
	g_pVtxBuffSelectNO->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_NoState)
	{
	case NOSTATE_SELECT:
		//セレクト画面No色設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		break;

	case NOSTATE_NOSELECT:
		//セレクト画面No色設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		break;
	}

	//頂点バッファのアンロック
	g_pVtxBuffSelectNO->Unlock();
}

//=============================================================================
// 描写処理[DrawPolygon]
//=============================================================================
void DrawSelectNo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスを取得する

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectNO, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSelectNO);

	//の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// QUIT設定処理[DrawPolygon]
//=============================================================================
void SetSelectNo(NOSTATE state)
{
	g_NoState = state;
}
