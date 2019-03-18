//=============================================================================
//
// 背景処理 [bgtitle.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "bgtitle.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_BGTITLE		(5)								// 背景の種類
#define BGTITLE_TEXTURENAME_0	"data/TEXTURE/pause100.png"		//背景のテクスチャ3
#define BGTITLE_TEXTURENAME_1	"data/TEXTURE/bg.png"		//背景のテクスチャ2
#define BGTITLE_TEXTURENAME_2	"data/TEXTURE/cloud2.png"		//背景のテクスチャ1
#define BGTITLE_TEXTURENAME_3	"data/TEXTURE/title01.png"		//背景のテクスチャ1
#define BGTITLE_TEXTURENAME_4	"data/TEXTURE/ground.png"		//背景のテクスチャ1


#define BG_POS_X		(0)							//背景左上X座標
#define BG_POS_Y		(0)							//背景左上Y座標
#define BG_WIDTH		(SCREEN_WIDTH)				//背景の幅
#define BG_HEIGHT		(SCREEN_HEIGHT)				//背景の高さ
//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBGTitle[MAX_TYPE_BGTITLE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBGTitle = NULL;					// 頂点バッファへのポインタ

D3DXVECTOR2 g_BGTitle[2];

int nCntTime;
//=============================================================================
// 初期化処理
//=============================================================================
void InitBGTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;	//背景のカウント

	// デバイスの取得
	pDevice = GetDevice();

	nCntTime = 0;
	for (int nCount = 0; nCount < MAX_TYPE_BGTITLE; nCount++)
	{
		g_BGTitle[0] = D3DXVECTOR2(0.0f, 0.0f);
		g_BGTitle[1] = D3DXVECTOR2(0.0f, 0.0f);
		g_BGTitle[2] = D3DXVECTOR2(0.0f, 0.0f);
	}

	// テクスチャの読み込み(3体分)
	D3DXCreateTextureFromFile(pDevice, BGTITLE_TEXTURENAME_0, &g_pTextureBGTitle[0]);
	D3DXCreateTextureFromFile(pDevice, BGTITLE_TEXTURENAME_1, &g_pTextureBGTitle[1]);
	D3DXCreateTextureFromFile(pDevice, BGTITLE_TEXTURENAME_2, &g_pTextureBGTitle[2]);
	D3DXCreateTextureFromFile(pDevice, BGTITLE_TEXTURENAME_3, &g_pTextureBGTitle[3]);
	D3DXCreateTextureFromFile(pDevice, BGTITLE_TEXTURENAME_4, &g_pTextureBGTitle[4]);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_BGTITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBGTitle,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBGTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < MAX_TYPE_BGTITLE; nCntBG++)
	{
		//====================================
		//
		//	よくわかんない
		//
		//====================================
		//頂点の座標
		//pVtx[0].pos = D3DXVECTOR3(0 , 0.0f , 0.0f);
		//pVtx[1].pos = D3DXVECTOR3( BG_WIDTH , 0.0f , 0.0f);
		//pVtx[2].pos = D3DXVECTOR3( 0 , BG_HEIGHT , 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(BG_WIDTH , BG_HEIGHT, 0.0f);

		////1.0fで固定
		//pVtx[0].rhw = 1.0f;
		//pVtx[1].rhw = 1.0f;
		//pVtx[2].rhw = 1.0f;
		//pVtx[3].rhw = 1.0f;

		////頂点カラー
		//pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		////テクスチャ座標
		//
		//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);







		//====================================
		//
		//	背景
		//
		//====================================
		//頂点の座標
		pVtx[4].pos = D3DXVECTOR3(0, 0.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(BG_WIDTH , 0.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0, SCREEN_HEIGHT , 0.0f);
		pVtx[7].pos = D3DXVECTOR3(BG_WIDTH , SCREEN_HEIGHT, 0.0f);

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






		//====================================
		//
		//	雲
		//
		//====================================
		pVtx[8].pos = D3DXVECTOR3(0, -50, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(BG_WIDTH, -50, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(0, BG_HEIGHT - 150, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT -150, 0.0f);

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


		//====================================
		//
		//	街
		//
		//====================================
		pVtx[12].pos = D3DXVECTOR3(0, -50, 0.0f);
		pVtx[13].pos = D3DXVECTOR3(BG_WIDTH, -50, 0.0f);
		pVtx[14].pos = D3DXVECTOR3(0, 720, 0.0f);
		pVtx[15].pos = D3DXVECTOR3(BG_WIDTH, 720, 0.0f);

		//1.0fで固定
		pVtx[12].rhw = 1.0f;
		pVtx[13].rhw = 1.0f;
		pVtx[14].rhw = 1.0f;
		pVtx[15].rhw = 1.0f;

		//頂点カラー
		pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標

		pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);



		//====================================
		//
		//	地面
		//
		//====================================
		pVtx[16].pos = D3DXVECTOR3(0 ,450 , 0.0f);
		pVtx[17].pos = D3DXVECTOR3( BG_WIDTH ,450 , 0.0f);
		pVtx[18].pos = D3DXVECTOR3( 0 , BG_HEIGHT , 0.0f);
		pVtx[19].pos = D3DXVECTOR3(BG_WIDTH , BG_HEIGHT, 0.0f);

		//1.0fで固定
		pVtx[16].rhw = 1.0f;
		pVtx[17].rhw = 1.0f;
		pVtx[18].rhw = 1.0f;
		pVtx[19].rhw = 1.0f;

		//頂点カラー
		pVtx[16].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[17].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[18].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[19].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		
		pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);


		
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBGTitle->Unlock();

	

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBGTitle(void)
{
	

	// テクスチャの開放
	for (int nCount = 0; nCount < MAX_TYPE_BGTITLE; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureBGTitle[nCount] != NULL)
		{
			g_pTextureBGTitle[nCount]->Release();
			g_pTextureBGTitle[nCount] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffBGTitle != NULL)
	{
		g_pVtxBuffBGTitle->Release();
		g_pVtxBuffBGTitle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBGTitle(void)
{
	//int nCounter = 0;

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	
	g_BGTitle[0].x -= 0.002f;
	//g_BGTitle[1].x -= 0.002f;
	g_BGTitle[2].x -= 0.0025f;

	
	nCntTime++;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBGTitle->Lock(0, 0, (void**)&pVtx, 0);
	
			//テクスチャ座標を更新
			pVtx[0].tex = D3DXVECTOR2(g_BGTitle[0].x, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_BGTitle[0].x - 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_BGTitle[0].x, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_BGTitle[0].x - 1.0f, 1.0f);

			//pVtx += 4;	//頂点データのポインタを4つ進める

			////テクスチャ座標を更新
			//pVtx[4].tex = D3DXVECTOR2(g_BGTitle[1].x, 0.0f);
			//pVtx[5].tex = D3DXVECTOR2(g_BGTitle[1].x - 1.0f, 0.0f);
			//pVtx[6].tex = D3DXVECTOR2(g_BGTitle[1].x, 1.0f);
			//pVtx[7].tex = D3DXVECTOR2(g_BGTitle[1].x - 1.0f, 1.0f);
			////pVtx += 4;	//頂点データのポインタを4つ進める

			//テクスチャ座標を更新
			pVtx[8].tex = D3DXVECTOR2(g_BGTitle[2].x, 0.0f);
			pVtx[9].tex = D3DXVECTOR2(g_BGTitle[2].x - 1.0f, 0.0f);
			pVtx[10].tex = D3DXVECTOR2(g_BGTitle[2].x, 1.0f);
			pVtx[11].tex = D3DXVECTOR2(g_BGTitle[2].x - 1.0f, 1.0f);


			////頂点カラー
			//pVtx[0].col = D3DXCOLOR(0.7f - (0.00054f * nCntTime), 0.8f - (0.00085f * nCntTime), 1.0f - (0.0018f * nCntTime), 1.0f);
			//pVtx[1].col = D3DXCOLOR(0.7f - (0.00054f * nCntTime), 0.8f - (0.00085f * nCntTime), 1.0f - (0.0018f * nCntTime), 1.0f);
			//pVtx[2].col = D3DXCOLOR(0.8f - (0.00055f * nCntTime), 0.9f - (0.0055f * nCntTime), 1.0f - (0.0016f * nCntTime), 1.0f);
			//pVtx[3].col = D3DXCOLOR(0.8f - (0.00055f * nCntTime), 0.9f - (0.0055f * nCntTime), 1.0f - (0.0016f * nCntTime), 1.0f);

			////頂点カラー
			//pVtx[4].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			//pVtx[5].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			//pVtx[6].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			//pVtx[7].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);


	
			////頂点
			//pVtx[8].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);
			//pVtx[9].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);
			//pVtx[10].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);
			//pVtx[11].col = D3DXCOLOR(1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f - (0.00045f * nCntTime), 1.0f);


			////頂点カラー
			//pVtx[12].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			//pVtx[13].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			//pVtx[14].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);
			//pVtx[15].col = D3DXCOLOR(1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f - (0.0003f * nCntTime), 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBGTitle->Unlock();

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBGTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

		
			// デバイスの取得
			pDevice = GetDevice();

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffBGTitle, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntBG = 0; nCntBG < MAX_TYPE_BGTITLE; nCntBG++)
	{

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBGTitle[nCntBG]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
		
	}
}


