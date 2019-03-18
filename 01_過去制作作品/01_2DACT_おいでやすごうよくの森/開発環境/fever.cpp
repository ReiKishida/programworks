//=============================================================================
//
// 背景処理 [fever.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "fever.h"
#include "main.h"
#include "timer.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	MAX_FEVER		(1)								// 背景の種類
#define FEVER_TEXTURENAME_0	"data/TEXTURE/fever.png"		//背景のテクスチャ3


#define BG_POS_X		(0)							//背景左上X座標
#define BG_POS_Y		(0)							//背景左上Y座標
#define BG_WIDTH		(SCREEN_WIDTH)				//背景の幅
#define BG_HEIGHT		(SCREEN_HEIGHT)				//背景の高さ
//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFever = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFever = NULL;					// 頂点バッファへのポインタ

FEVER g_Fever[MAX_FEVER];

//=============================================================================
// 初期化処理
//=============================================================================
void InitFever(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntFever;	//背景のカウント

				// デバイスの取得
	pDevice = GetDevice();

	
	g_Fever[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 2, SCREEN_HEIGHT /2, 0.0f);
	g_Fever[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT / 2, 0.0f);
	g_Fever[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 4, SCREEN_HEIGHT / 2, 0.0f);

	for (int nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{
		g_Fever[nCntFever].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Fever[nCntFever].bUse = false;
	}
	

	// テクスチャの読み込み(3体分)
	D3DXCreateTextureFromFile(pDevice, FEVER_TEXTURENAME_0, &g_pTextureFever);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FEVER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFever,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFever->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffFever->Unlock();



}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitFever(void)
{


	
		//テクスチャの破棄
		if (g_pTextureFever != NULL)
		{
			g_pTextureFever->Release();
			g_pTextureFever = NULL;
		}
	


		// 頂点バッファの開放
		if (g_pVtxBuffFever != NULL)
		{
			g_pVtxBuffFever->Release();
			g_pVtxBuffFever = NULL;
		}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFever(void)
{
	TIMER pTimer;
	pTimer = GetTimer();

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	if (pTimer.nTime <= 13)
	{

		//PlaySound(SOUND_LABEL_SE_TRUMPET);

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFever->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntFever = 0; nCntFever < 1; nCntFever++)
		{

			g_Fever[nCntFever].bUse = true;

			g_Fever[nCntFever].move.x -= 0.055f;

			g_Fever[nCntFever].pos.x += g_Fever[nCntFever].move.x;

			
		}
		// 頂点バッファをアンロックする
		g_pVtxBuffFever->Unlock();
	}

	if (pTimer.nTime != 30 && pTimer.nTime != 20 && pTimer.nTime != 0 && pTimer.nTime  %  10 == 0)
	{

		PlaySound(SOUND_LABEL_SE_TRUMPET);
	}




	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFever->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x - SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Fever[nCntFever].pos.x + SCREEN_WIDTH / 2, g_Fever[nCntFever].pos.y + SCREEN_HEIGHT / 2, 0.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffFever->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFever(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画


	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFever, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFever);


	for (int nCntFever = 0; nCntFever < MAX_FEVER; nCntFever++)
	{

	

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFever * 4, 2);

	}
}


