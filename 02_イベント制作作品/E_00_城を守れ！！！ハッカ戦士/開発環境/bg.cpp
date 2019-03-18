//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "bg.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_BG		(3)								// 背景の種類
#define BG_TEXTURENAME_0	"data/TEXTURE/sky.png"		//背景のテクスチャ3
#define BG_TEXTURENAME_1	"data/TEXTURE/back.png"		//背景のテクスチャ2
#define BG_TEXTURENAME_2	"data/TEXTURE/ground.png"		//背景のテクスチャ1


#define BG_POS_X		(0)							//背景左上X座標
#define BG_POS_Y		(0)							//背景左上Y座標
#define BG_WIDTH		(SCREEN_WIDTH)				//背景の幅
#define BG_HEIGHT		(SCREEN_HEIGHT)				//背景の高さ
//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBG[MAX_TYPE_BG] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;					// 頂点バッファへのポインタ

D3DXVECTOR2 g_BG[2];

//=============================================================================
// 初期化処理
//=============================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;	//背景のカウント

	// デバイスの取得
	pDevice = GetDevice();

	
	for (int nCount = 0; nCount < MAX_TYPE_BG; nCount++)
	{
		g_BG[0] = D3DXVECTOR2(0.0f, 0.0f);
		g_BG[1] = D3DXVECTOR2(0.0f, 0.0f);
		g_BG[2] = D3DXVECTOR2(0.0f, 0.0f);
	}

	// テクスチャの読み込み(3体分)
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME_0, &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME_1, &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME_2, &g_pTextureBG[2]);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < MAX_TYPE_BG; nCntBG++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(0 ,0 , 0.0f);
		pVtx[1].pos = D3DXVECTOR3( BG_WIDTH ,0 , 0.0f);
		pVtx[2].pos = D3DXVECTOR3( 0 , BG_HEIGHT , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BG_WIDTH , BG_HEIGHT , 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);


					//頂点の座標
		pVtx[4].pos = D3DXVECTOR3(0, 250, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(BG_WIDTH, 250, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0, 600, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(BG_WIDTH, 600, 0.0f);

		//1.0fで固定
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//頂点カラー
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[4].tex = D3DXVECTOR2(0.0, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx[8].pos = D3DXVECTOR3(0, 580, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(BG_WIDTH, 580, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(0, BG_HEIGHT, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

		//1.0fで固定
		pVtx[8].rhw = 1.0f;
		pVtx[9].rhw = 1.0f;
		pVtx[10].rhw = 1.0f;
		pVtx[11].rhw = 1.0f;

		//頂点カラー
		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[9].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[10].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[11].tex = D3DXVECTOR2(0.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();

	

}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitBG(void)
{
	

	// テクスチャの開放
	for (int nCount = 0; nCount < MAX_TYPE_BG; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureBG[nCount] != NULL)
		{
			g_pTextureBG[nCount]->Release();
			g_pTextureBG[nCount] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void)
{
	//int nCounter = 0;

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	
	g_BG[0].x -= 0.001f;
	//g_BG[1].x -= 0.002f;
	//g_BG[2].x -= 0.003f;

	
	

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	
			//テクスチャ座標を更新
			pVtx[0].tex = D3DXVECTOR2(g_BG[0].x, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_BG[0].x - 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_BG[0].x, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_BG[0].x - 1.0f, 1.0f);

			//pVtx += 4;	//頂点データのポインタを4つ進める

			//テクスチャ座標を更新
			pVtx[4].tex = D3DXVECTOR2(g_BG[1].x, 0.0f);
			pVtx[5].tex = D3DXVECTOR2(g_BG[1].x - 1.0f, 0.0f);
			pVtx[6].tex = D3DXVECTOR2(g_BG[1].x, 1.0f);
			pVtx[7].tex = D3DXVECTOR2(g_BG[1].x - 1.0f, 1.0f);
			//pVtx += 4;	//頂点データのポインタを4つ進める

			//テクスチャ座標を更新
			pVtx[8].tex = D3DXVECTOR2(g_BG[2].x, 0.0f);
			pVtx[9].tex = D3DXVECTOR2(g_BG[2].x - 1.0f, 0.0f);
			pVtx[10].tex = D3DXVECTOR2(g_BG[2].x, 1.0f);
			pVtx[11].tex = D3DXVECTOR2(g_BG[2].x - 1.0f, 1.0f);
	
	// 頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

		
			// デバイスの取得
			pDevice = GetDevice();

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntBG = 0; nCntBG < MAX_TYPE_BG; nCntBG++)
	{

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
		
	}
}


