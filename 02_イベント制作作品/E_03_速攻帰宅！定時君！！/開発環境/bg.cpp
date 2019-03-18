//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : 圷 和也
//
//=============================================================================
#include "main.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WIDTH			(SCREEN_WIDTH)				// 背景の幅
#define BG_HEIGHT			(SCREEN_HEIGHT)				// 背景の高さ
#define MAX_BG				(10)						// 背景の最大数
#define MAX_TEX_TYPE_BG		(2)							// 背景のテクスチャの種類

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	float fMove;				// スクロール量
	float fScrollCut;			// 背景スクロールの分割数
	float fScrollCutPattern;	// スクロール分割の一コマ分
	float fScrollCounter;		// スクロールのカウンター
	D3DXCOLOR col;				// 色
	float fWidth;				// 背景の幅
	float fHeight;				// 背景の高さ
	int nTexType;				// テクスチャの種類
	BG_TYPE type;				// 種類
	bool bUse;					// 使用しているかどうか
}Bg;


typedef struct
{
	char *pFilename;	// ファイル名
} BgTexInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureBg[BG_TEX_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;			// 頂点バッファへのポインタ
Bg g_aBg[MAX_BG];										// 敵の情報
D3DXVECTOR3 g_moveBg;									// 背景の移動量
														// 読み込むテクスチャ名の設定
BgTexInfo g_apTextureBgInfo[BG_TEX_MAX] =
{
	{ "data//TEXTURE//rizaruto.jpg" },			// 空
	{ "data//TEXTURE//result001.jpg" },			// 空

};
//=============================================================================
// 背景初期化処理
//=============================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// 背景の情報の初期化
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		g_aBg[nCntBg].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[nCntBg].fMove = 0.0f;
		g_aBg[nCntBg].fScrollCut = 4.0f;
		g_aBg[nCntBg].fScrollCutPattern = float(1 / g_aBg[nCntBg].fScrollCut);
		g_aBg[nCntBg].fScrollCounter = 0.0f;
		g_aBg[nCntBg].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBg[nCntBg].fWidth = 0.0f;
		g_aBg[nCntBg].fHeight = 0.0f;
		g_aBg[nCntBg].nTexType = 0;
		g_aBg[nCntBg].type = BG_TYPE_NORMAL;
		g_aBg[nCntBg].bUse = false;
	}

	// テクスチャの読み込み
	for (int nCntBgTex = 0; nCntBgTex < BG_TEX_MAX; nCntBgTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureBgInfo[nCntBgTex].pFilename,
			&g_apTextureBg[nCntBgTex]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBg,
								NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[0].col = g_aBg[nCntBg].col;
		pVtx[1].col = g_aBg[nCntBg].col;
		pVtx[2].col = g_aBg[nCntBg].col;
		pVtx[3].col = g_aBg[nCntBg].col;
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// 背景終了処理
//=============================================================================
void UninitBG(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < BG_TEX_MAX; nCntTex++)
	{
		if (g_apTextureBg[nCntTex] != NULL)
		{
			g_apTextureBg[nCntTex]->Release();
			g_apTextureBg[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//=============================================================================
// 背景更新処理
//=============================================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++, pVtx += 4)
	{
		if (g_aBg[nCntBg].bUse == true)
		{ // 背景が使用されている
			// 位置更新
			if (g_aBg[nCntBg].type == BG_TYPE_SCROLL)
			{ // スクロールする背景の場合
				g_aBg[nCntBg].fScrollCounter += g_aBg[nCntBg].fMove;
				if (g_aBg[nCntBg].fScrollCounter > 1.0f - g_aBg[nCntBg].fScrollCutPattern)
				{
					g_aBg[nCntBg].fScrollCounter = 0.0f;
				}
			}
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);

			if (g_aBg[nCntBg].type == BG_TYPE_SCROLL)
			{
				pVtx[0].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter + g_aBg[nCntBg].fScrollCutPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aBg[nCntBg].fScrollCounter + g_aBg[nCntBg].fScrollCutPattern, 1.0f);
			}
			else
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
// 背景描画処理
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (g_aBg[nCntBg].bUse == true)
		{ // 敵が使用されている
		  // テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBg[g_aBg[nCntBg].nTexType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
		}
	}
}

//=============================================================================
// 背景の設定
//=============================================================================
void SetBg(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fWidth, float fHeight ,int nTexType, BG_TYPE type)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++, pVtx += 4)
	{
		if (g_aBg[nCntBg].bUse == false)
		{ // 爆発が使用されていない場合
		  // 頂点位置の設定
			g_aBg[nCntBg].pos = pos;
			g_aBg[nCntBg].fMove = fMove;
			g_aBg[nCntBg].col = col;
			g_aBg[nCntBg].fWidth = fWidth;
			g_aBg[nCntBg].fHeight = fHeight;
			g_aBg[nCntBg].nTexType = nTexType;
			g_aBg[nCntBg].type = type;

			pVtx[0].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y - g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x - g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBg[nCntBg].pos.x + g_aBg[nCntBg].fWidth / 2, g_aBg[nCntBg].pos.y + g_aBg[nCntBg].fHeight / 2, 0.0f);
			pVtx[0].col = g_aBg[nCntBg].col;
			pVtx[1].col = g_aBg[nCntBg].col;
			pVtx[2].col = g_aBg[nCntBg].col;
			pVtx[3].col = g_aBg[nCntBg].col;

			g_aBg[nCntBg].bUse = true;	// 使用する状態にする
			break;	// このbreakはかなり大切
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

