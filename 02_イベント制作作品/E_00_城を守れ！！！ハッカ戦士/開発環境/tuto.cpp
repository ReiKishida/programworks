//=============================================================================
//
// チュートリアル画面処理 [tuto.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "tuto.h"
#include "fade.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME	"DATA/TEXTURE/TUTO.png"	//タイトルのテクスチャ名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTuto = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTuto = NULL;	//頂点バッファへのポインタ

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTuto(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME, &g_pTextureTuto);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTuto,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTuto->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UnInitTuto(void)
{
	//テクスチャの破棄
	if (g_pTextureTuto != NULL)
	{
		g_pTextureTuto->Release();
		g_pTextureTuto = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTuto != NULL)
	{
		g_pVtxBuffTuto->Release();
		g_pVtxBuffTuto = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateTuto(void)
{
	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//モード選択
			//SetMode(MODE_GAME);
			SetFade(MODE_GAME);
		}
	}
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawTuto(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

								//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTuto, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTuto);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
