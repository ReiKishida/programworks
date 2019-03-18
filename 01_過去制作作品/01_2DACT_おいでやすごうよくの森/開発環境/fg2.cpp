//=============================================================================
//
// アイテム処理 [Fg.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "Fg2.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "block.h"
#include "fg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_FG2		(8)								// アイテムの種類
#define Fg_TEXTURENAME_0	"data/TEXTURE/fg003-1.png"		//アイテムのテクスチャ1
#define Fg_TEXTURENAME_1	"data/TEXTURE/glass002.png"		//アイテムのテクスチャ2
#define Fg_TEXTURENAME_2	"data/TEXTURE/back4.png"		//アイテムのテクスチャ3
#define Fg_TEXTURENAME_3	"data/TEXTURE/glass2525.png"		//アイテムのテクスチャ4
#define Fg_TEXTURENAME_4	"data/TEXTURE/woodleft.png"		//アイテムのテクスチャ1
#define Fg_TEXTURENAME_5	"data/TEXTURE/woodright.png"		//アイテムのテクスチャ2
#define Fg_TEXTURENAME_6	"data/TEXTURE/fever.png"		//アイテムのテクスチャ3
#define Fg_TEXTURENAME_7	"data/TEXTURE/watch.png"		//アイテムのテクスチャ4




//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureFg2[MAX_TYPE_FG2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFg2 = NULL;					// 頂点バッファへのポインタ

FG g_aFg2[MAX_FG];	//ブロック

					//bool bJump;


//=============================================================================
// 初期化処理
//=============================================================================
void InitFg2(void)
{
	LPDIRECT3DDEVICE9 pDevice;


	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの情報の初期化
	for (int nCntFg2 = 0; nCntFg2 < MAX_FG; nCntFg2++)
	{
		g_aFg2[nCntFg2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg2[nCntFg2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg2[nCntFg2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aFg2[nCntFg2].bUse = false;
		g_aFg2[nCntFg2].fHeight = 0;
		g_aFg2[nCntFg2].fWidth = 0;

	}




	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_0, &g_apTextureFg2[0]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_1, &g_apTextureFg2[1]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_2, &g_apTextureFg2[2]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_3, &g_apTextureFg2[3]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_4, &g_apTextureFg2[4]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_5, &g_apTextureFg2[5]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_6, &g_apTextureFg2[6]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_7, &g_apTextureFg2[7]);


	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FG2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFg2,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFg2->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFg2 = 0; nCntFg2 < MAX_FG2; nCntFg2++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);

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

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffFg2->Unlock();


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitFg2(void)
{

	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_FG2; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureFg2[nCntTex] != NULL)
		{
			g_apTextureFg2[nCntTex]->Release();
			g_apTextureFg2[nCntTex] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffFg2 != NULL)
	{
		g_pVtxBuffFg2->Release();
		g_pVtxBuffFg2 = NULL;
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFg2(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFg2(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFg2, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntFg2 = 0; nCntFg2 < MAX_FG; nCntFg2++)
	{
		if (g_aFg2[nCntFg2].bUse == true)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureFg2[g_aFg2[nCntFg2].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFg2 * 4, 2);
		}
	}
}


//=============================================================================
// アイテムの設定
//=============================================================================
void SetFg2(D3DXVECTOR3 pos, int type, float fHeight, float fWidth)
{
	int nCntFg2;
	VERTEX_2D *pVtx;



	for (nCntFg2 = 0; nCntFg2 < MAX_FG2; nCntFg2++)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffFg2->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aFg2[nCntFg2].bUse == false)	//アイテムが使用されていない
		{
			pVtx += (nCntFg2 * 4);

			g_aFg2[nCntFg2].pos = pos;
			g_aFg2[nCntFg2].fHeight = fHeight;
			g_aFg2[nCntFg2].fWidth = fWidth;

			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y - g_aFg2[nCntFg2].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x - g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFg2[nCntFg2].pos.x + g_aFg2[nCntFg2].fWidth, g_aFg2[nCntFg2].pos.y, 0.0f);

			g_aFg2[nCntFg2].nType = type;

			g_aFg2[nCntFg2].bUse = true;	//使用している状態にする

			break;





			//頂点バッファをアンロック
			g_pVtxBuffFg2->Unlock();
		}
	}
}



//=============================================================================
// アイテムの取得
//=============================================================================
//Fg *GetFg(void)
//{
//	return &g_aFg2[0];
//}
