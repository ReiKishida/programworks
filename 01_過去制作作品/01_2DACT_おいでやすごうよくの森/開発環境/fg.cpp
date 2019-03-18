//=============================================================================
//
// アイテム処理 [Fg.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "Fg.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "block.h"
#include "fg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_FG		(8)								// アイテムの種類
#define Fg_TEXTURENAME_0	"data/TEXTURE/glass000-1.png"		//アイテムのテクスチャ1
#define Fg_TEXTURENAME_1	"data/TEXTURE/glass002-1.png"		//アイテムのテクスチャ2
#define Fg_TEXTURENAME_2	"data/TEXTURE/glass2525.png"		//アイテムのテクスチャ3
#define Fg_TEXTURENAME_3	"data/TEXTURE/watch.png"		//アイテムのテクスチャ4
#define Fg_TEXTURENAME_4	"data/TEXTURE/back5.png"		//アイテムのテクスチャ1
#define Fg_TEXTURENAME_5	"data/TEXTURE/vodka.png"		//アイテムのテクスチャ2
#define Fg_TEXTURENAME_6	"data/TEXTURE/arei2.png"		//アイテムのテクスチャ3
#define Fg_TEXTURENAME_7	"data/TEXTURE/watch.png"		//アイテムのテクスチャ4

#define Fg_X (50)
#define Fg_Y (50)

#define FgLOST (0)

#define Fg_JUMP_POWER (-9.0f)	//ジャンプ力
#define Fg_GRAVITY (1.1f)		//重力
#define PAPER_GRAVITY (0.98f)		//重力

#define Fg_GRAVITY_MAX (25.5f)	//重力の最大量

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureFg[MAX_TYPE_FG] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFg = NULL;					// 頂点バッファへのポインタ

FG g_aFg[MAX_FG];	//ブロック

						//bool bJump;


//=============================================================================
// 初期化処理
//=============================================================================
void InitFg(void)
{
	LPDIRECT3DDEVICE9 pDevice;


	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの情報の初期化
	for (int nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		g_aFg[nCntFg].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg[nCntFg].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFg[nCntFg].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aFg[nCntFg].bUse = false;
		g_aFg[nCntFg].fHeight = 0;
		g_aFg[nCntFg].fWidth = 0;

	}




	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_0, &g_apTextureFg[0]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_1, &g_apTextureFg[1]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_2, &g_apTextureFg[2]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_3, &g_apTextureFg[3]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_4, &g_apTextureFg[4]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_5, &g_apTextureFg[5]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_6, &g_apTextureFg[6]);
	D3DXCreateTextureFromFile(pDevice, Fg_TEXTURENAME_7, &g_apTextureFg[7]);


	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFg,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);

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
	g_pVtxBuffFg->Unlock();


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitFg(void)
{

	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_FG; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureFg[nCntTex] != NULL)
		{
			g_apTextureFg[nCntTex]->Release();
			g_apTextureFg[nCntTex] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffFg != NULL)
	{
		g_pVtxBuffFg->Release();
		g_pVtxBuffFg = NULL;
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFg(void)
{

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		if (g_aFg[nCntFg].bUse == true)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureFg[g_aFg[nCntFg].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFg * 4, 2);
		}
	}
}


//=============================================================================
// アイテムの設定
//=============================================================================
void SetFg(D3DXVECTOR3 pos, int type, float fHeight, float fWidth)
{
	int nCntFg;
	VERTEX_2D *pVtx;



	for (nCntFg = 0; nCntFg < MAX_FG; nCntFg++)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffFg->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aFg[nCntFg].bUse == false)	//アイテムが使用されていない
		{
			pVtx += (nCntFg * 4);
			
				g_aFg[nCntFg].pos = pos;
				g_aFg[nCntFg].fHeight = fHeight;
				g_aFg[nCntFg].fWidth = fWidth;

				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y - g_aFg[nCntFg].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x - g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aFg[nCntFg].pos.x + g_aFg[nCntFg].fWidth, g_aFg[nCntFg].pos.y, 0.0f);

				g_aFg[nCntFg].nType = type;

				g_aFg[nCntFg].bUse = true;	//使用している状態にする

				break;
			




			//頂点バッファをアンロック
			g_pVtxBuffFg->Unlock();
		}
	}
}



//=============================================================================
// アイテムの取得
//=============================================================================
//Fg *GetFg(void)
//{
//	return &g_aFg[0];
//}
