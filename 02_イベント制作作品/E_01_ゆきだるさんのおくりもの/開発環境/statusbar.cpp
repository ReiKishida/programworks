//=============================================================================
//
// ステータスバー処理 [statusbar.cpp]
// Author : komatsu keisuke
//
//=============================================================================
#include "statusbar.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define STATUSBAR_TEXTURENAME	"data/TEXTURE/bg001.png"	//表示するファイルの名前
#define MAX_TEXTURE			(1)								//読み込むテクスチャの数
#define TEX_X				(1.0f)							//テクスチャのX
#define TEX_Y				(1.0f)							//テクスチャのY

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureStatusBar[MAX_TEXTURE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStatusBar = NULL;			//頂点バッファへのポインタ

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitStatusBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, STATUSBAR_TEXTURENAME, &g_pTextureStatusBar[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffStatusBar,
								NULL);
	//頂点バッファへのポインタ
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStatusBar->Lock(0, 0, (void**)&pVtx, 0);

	//ステータスバー
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f);

	//rwhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TEX_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_Y);
	pVtx[3].tex = D3DXVECTOR2(TEX_X, TEX_Y);

	//頂点バッファをアンロックする
	g_pVtxBuffStatusBar->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStatusBar(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureStatusBar[nCntTex] != NULL)
		{
			g_pTextureStatusBar[nCntTex]->Release();
			g_pTextureStatusBar[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffStatusBar != NULL)
	{
		g_pVtxBuffStatusBar->Release();
		g_pVtxBuffStatusBar = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStatusBar(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStatusBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスを取得

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStatusBar, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureStatusBar[nCntTex]);
	
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}