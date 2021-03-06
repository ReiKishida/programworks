//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "timer.h"
#include "main.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_Timer		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);

#define MAX_TIMER (4)
#define TIMER_X (30.0f)
#define TIMER_Y (45.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nCounter;	//状態管理のカウンター
	int nTime;		//タイム
}TIMER;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTimer = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// 頂点バッファへのポインタ
TIMER					g_Timer;					// スコア
int						g_aTimer[MAX_TIMER];		//




//=============================================================================
// 初期化処理
 //=============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	g_Timer.nTime = 3000;

	g_Timer.pos = D3DXVECTOR3(600, 50, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_Timer,		// ファイルの名前
		&g_pTextureTimer);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexTimer(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{

	// テクスチャの開放
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	
	
	g_Timer.nTime--;

	if (g_Timer.nTime == 0)
	{
		g_Timer.nTime = 0;
		SetGameState(GAMESTATE_FAILED);
	}
		
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
		{
			g_aTimer[0] = g_Timer.nTime % 10000 / 1000;
			g_aTimer[1] = g_Timer.nTime % 1000 / 100;
			g_aTimer[2] = g_Timer.nTime % 100 / 10;
			g_aTimer[3] = g_Timer.nTime % 10;


			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aTimer[nCntTimer], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * g_aTimer[nCntTimer], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * g_aTimer[nCntTimer], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * g_aTimer[nCntTimer], 1.0f);



			pVtx += 4;

		}
		// 頂点バッファをアンロックする
		g_pVtxBuffTimer->Unlock();
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	int nCntTimer;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimer);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_Timer.pos.x - TIMER_X, g_Timer.pos.y - TIMER_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Timer.pos.x + TIMER_X, g_Timer.pos.y - TIMER_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Timer.pos.x - TIMER_X, g_Timer.pos.y + TIMER_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Timer.pos.x + TIMER_X, g_Timer.pos.y + TIMER_Y, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.7f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.7f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.7f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.7f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


		pVtx += 4;

		if (nCntTimer == 1)
		{
			
			g_Timer.pos.x += 80;
		}
		else
		{
			g_Timer.pos.x += 50;
		}

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}


