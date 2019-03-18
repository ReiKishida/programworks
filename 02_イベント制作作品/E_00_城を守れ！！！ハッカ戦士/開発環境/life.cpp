//=============================================================================
//
// ライフ処理 [Life.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "life.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_LIFE		"data/TEXTURE/life000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);

#define LIFE_X (15.0f)
#define LIFE_Y (30.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureLife = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		// 頂点バッファへのポインタ
LIFE					g_aLife[MAX_LIFE];					// スコア

//=============================================================================
// 初期化処理
//=============================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	for (int nCntLife = 0; nCntLife < MAX_LIFE  ; nCntLife++)
	{
		g_aLife[nCntLife].pos = D3DXVECTOR3( 40, 40, 0.0f);
		g_aLife[nCntLife].nLife = MAX_LIFE;
		g_aLife[nCntLife].bUse = true;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_LIFE,		// ファイルの名前
		&g_pTextureLife);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexLife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{

	// テクスチャの開放
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{

			pVtx[0].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - LIFE_X, g_aLife[nCntLife].pos.y - LIFE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + LIFE_X, g_aLife[nCntLife].pos.y - LIFE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - LIFE_X, g_aLife[nCntLife].pos.y + LIFE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + LIFE_X, g_aLife[nCntLife].pos.y + LIFE_Y, 0.0f);


			//1.0fで固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;

			for (int nCntLife2 = 0; nCntLife2 < MAX_LIFE; nCntLife2++)
			{
				g_aLife[nCntLife2].pos.x += 50;
			}
			


	}
	// 頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}


//=============================================================================
// スコアの加算
//=============================================================================
void AddLife(int nLife)
{
	
	if (g_aLife[nLife].bUse == true)
	{
		g_aLife[nLife].bUse = false;
	}
}


//=============================================================================
// ライフの取得
//=============================================================================
LIFE *GetLife(void)
{
	return &g_aLife[0];
}



