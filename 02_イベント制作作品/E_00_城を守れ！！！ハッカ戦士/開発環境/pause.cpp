//=============================================================================
//
// ポーズ処理 [Pause.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "pause.h"
#include "main.h"
#include "input.h"
#include "pausemenu.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON (2)
#define BG_WIDTH		(SCREEN_WIDTH)				//背景の幅
#define BG_HEIGHT		(SCREEN_HEIGHT)				//背景の高さ
#define BG_POS_X		(0)							//背景左上X座標
#define BG_POS_Y		(0)							//背景左上Y座標


#define BG2_WIDTH		(200)				//背景の幅
#define BG2_HEIGHT		(200)				//背景の高さ
#define BG2_POS_X		(0)							//背景左上X座標
#define BG2_POS_Y		(0)							//背景左上Y座標
#define BG2_TEXTURENAME_0	"data/TEXTURE/pause100.png"		//背景のテクスチャ1



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexPause2(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePause = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	// 頂点バッファへのポインタ
PAUSE					g_Pause;					// フェード状態

LPDIRECT3DTEXTURE9		g_pTexturePause2 = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause2 = NULL;	// 頂点バッファへのポインタ
PAUSE2					g_Pause2;					// フェード状態


//=============================================================================
// 初期化処理
//=============================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Pause.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	//黒い画面にしておく

	g_Pause2.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);	//

	g_Pause2.col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 0.5f);	//黒い画面にしておく

// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG2_TEXTURENAME_0, &g_pTexturePause2);


	// 頂点情報の設定
	MakeVertexPause2(pDevice);

	// 頂点情報の設定
	MakeVertexPause(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitPause(void)
{
	//テクスチャの破棄
	if (g_pTexturePause != NULL)
	{
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//テクスチャの破棄
	if (g_pTexturePause2 != NULL)
	{
		g_pTexturePause2->Release();
		g_pTexturePause2 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause2 != NULL)
	{
		g_pVtxBuffPause2->Release();
		g_pVtxBuffPause2 = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{

	//PushPauseMenu(nNowSelect);

	////VERTEX_2D *pVtx;
	//		
	//
	//		if (GetKeyboardTrigger(DIK_W) == true)
	//		//if (GetKeyboardPress(DIK_W) == true)
	//		{//Wキー
	//			PlaySound(SOUND_LABEL_SE_SELECT);
	//			nNowSelect = (nNowSelect + (MAX_MENU - 1)) % MAX_MENU;//選択状態を一つ上げる
	//			PushPauseMenu(nNowSelect);
	//		}


	//		else if (GetKeyboardTrigger(DIK_S) == true)
	//		//else if (GetKeyboardPress(DIK_S) == true)
	//		{//Sキー
	//			PlaySound(SOUND_LABEL_SE_SELECT);
	//			nNowSelect = (nNowSelect + 1 )% MAX_MENU;//選択状態を一つ下げる
	//			PushPauseMenu(nNowSelect);




	//		}

	//		else if (GetKeyboardTrigger(DIK_RETURN) == true)
	//		{//RETURNキー
	//			PushPauseMenu(nNowSelect);
	//		}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePause);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);



	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPause2, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePause2);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// 頂点バッファの情報を設定
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0 ,0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH , 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0 ,BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH , BG_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

	////テクスチャ座標
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPause2(LPDIRECT3DDEVICE9 pDevice)
{

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause2,
		NULL);

	// 頂点バッファの情報を設定
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPause2->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Pause2.pos.x - BG2_WIDTH, g_Pause2.pos.y - BG2_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Pause2.pos.x + BG2_WIDTH, g_Pause2.pos.y - BG2_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Pause2.pos.x - BG2_WIDTH, g_Pause2.pos.y + BG2_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Pause2.pos.x + BG2_WIDTH, g_Pause2.pos.y + BG2_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.0f, 0.5f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファをアンロック
	g_pVtxBuffPause2->Unlock();

}