//=============================================================================
//
// 雪処理 [snow.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "snow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SNOW_TEXTURENAME	"data/TEXTURE/shadow000.jpg"
#define MAX_SNOW		(2020)	// 雪の最大数
#define SNOW_APPEAR		(65)		// 出現する間隔

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSnow = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSnow = NULL;	// 頂点バッファへのポインタ
SNOW g_aSnow[MAX_SNOW];
int g_nCntSnow;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitSnow(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ジェネレータの情報を初期化
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		g_aSnow[nCntSnow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSnow[nCntSnow].fRadius = 0;
		g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSnow[nCntSnow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSnow[nCntSnow].bUse = false;
	}

	g_nCntSnow = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SNOW_TEXTURENAME, &g_pTextureSnow);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SNOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSnow,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSnow->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffSnow->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitSnow(void)
{
	// テクスチャの破棄
	if (g_pTextureSnow != NULL)
	{
		g_pTextureSnow->Release();
		g_pTextureSnow = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffSnow != NULL)
	{
		g_pVtxBuffSnow->Release();
		g_pVtxBuffSnow = NULL;
	}
}

//=============================================================================
// ポリゴン更新処理
//=============================================================================
void UpdateSnow(void)
{
	int nCntSnow;
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSnow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		g_nCntSnow = (g_nCntSnow + 1) % SNOW_APPEAR;	// 雪を出す間隔のカウンタ

		if (g_aSnow[nCntSnow].bUse == true)
		{// 使われている
			if (g_aSnow[nCntSnow].pos.y > SCREEN_HEIGHT || g_aSnow[nCntSnow].pos.x < 0 || g_aSnow[nCntSnow].pos.x > SCREEN_WIDTH)
			{
				g_aSnow[nCntSnow].bUse = false;
			}

			g_aSnow[nCntSnow].pos += g_aSnow[nCntSnow].move;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aSnow[nCntSnow].pos.x - g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].pos.y - g_aSnow[nCntSnow].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].pos.x + g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].pos.y - g_aSnow[nCntSnow].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSnow[nCntSnow].pos.x - g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].pos.y + g_aSnow[nCntSnow].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].pos.x + g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].pos.y + g_aSnow[nCntSnow].fRadius, 0.0f);
		}
		else
		{
			if (g_nCntSnow == 0)
			{// 一定時間経過で雪を降らす
				g_aSnow[nCntSnow].pos = D3DXVECTOR3(rand() % SCREEN_WIDTH * 1.0f, -10.0f, 0.0f);
				g_aSnow[nCntSnow].move = D3DXVECTOR3(rand() % 3 * 0.1f, 1.0f, 0.0f);
				g_aSnow[nCntSnow].fRadius = 5.0f;
				g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				g_aSnow[nCntSnow].bUse = true;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);

				// 頂点カラー
				pVtx[0].col = g_aSnow[nCntSnow].col;
				pVtx[1].col = g_aSnow[nCntSnow].col;
				pVtx[2].col = g_aSnow[nCntSnow].col;
				pVtx[3].col = g_aSnow[nCntSnow].col;

				break;
			}
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffSnow->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawSnow(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffSnow, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		if (g_aSnow[nCntSnow].bUse == true)
		{//ブロックが使用されている
		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureSnow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSnow, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
