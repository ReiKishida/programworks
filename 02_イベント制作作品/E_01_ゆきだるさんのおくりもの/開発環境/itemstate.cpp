//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : komatsu
//
//=============================================================================
#include "itemstate.h"
#include "player.h"
#include "score.h"	
#include "game.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ITEM_TEXTURENAME	"data/TEXTURE/snow_crystal001.png"	//表示するファイルの名前
#define MAX_ITEM_TEX		(1)									//読み込むテクスチャの数
#define MAX_ITEM			(1)									//持てるアイテムの最大数
#define ITEM_POS_X			(30.0f)								//アイテムの幅
#define ITEM_POS_Y			(30.0f)								//アイテムの高さ
#define COUNTER_ANIM_RESET	(4)									//パターン数
#define ANIMATION_SPEED		(10)								//アニメーション速度
#define TEX_X				(0.2f)								//テクスチャのX
#define TEX_Y				(1.0f)								//テクスチャのY

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureItemState[MAX_ITEM] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItemState = NULL;				//頂点バッファへのポインタ
ItemState g_aItemState[MAX_ITEM];								//アイテムの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitItemState(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	// アイテムの情報の初期化
	for(int nCntItemState = 0; nCntItemState < MAX_ITEM; nCntItemState++)
	{
		g_aItemState[nCntItemState].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		g_aItemState[nCntItemState].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aItemState[nCntItemState].nCounterAnim = 0;
		g_aItemState[nCntItemState].nPatternAnim = 0;
		g_aItemState[nCntItemState].fHeight = ITEM_POS_X;
		g_aItemState[nCntItemState].fWidth = ITEM_POS_Y;
		g_aItemState[nCntItemState].bUse = false;		
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,ITEM_TEXTURENAME,&g_pTextureItemState[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItemState,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffItemState->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItemState = 0; nCntItemState < MAX_ITEM; nCntItemState++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aItemState[nCntItemState].pos.x - g_aItemState[nCntItemState].fHeight, g_aItemState[nCntItemState].pos.y - g_aItemState[nCntItemState].fWidth, g_aItemState[nCntItemState].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aItemState[nCntItemState].pos.x + g_aItemState[nCntItemState].fHeight, g_aItemState[nCntItemState].pos.y - g_aItemState[nCntItemState].fWidth, g_aItemState[nCntItemState].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aItemState[nCntItemState].pos.x - g_aItemState[nCntItemState].fHeight, g_aItemState[nCntItemState].pos.y + g_aItemState[nCntItemState].fWidth, g_aItemState[nCntItemState].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aItemState[nCntItemState].pos.x + g_aItemState[nCntItemState].fHeight, g_aItemState[nCntItemState].pos.y + g_aItemState[nCntItemState].fWidth, g_aItemState[nCntItemState].pos.z);
		
		//頂点
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItemState->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItemState(void)
{
	for (int nCntTex = 0; nCntTex < MAX_ITEM_TEX; nCntTex++)
	{
		//テクスチャの開放
		if (g_pTextureItemState[nCntTex] != NULL)
		{
			g_pTextureItemState[nCntTex]->Release();
			g_pTextureItemState[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffItemState != NULL)
	{
		g_pVtxBuffItemState->Release();
		g_pVtxBuffItemState = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItemState(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffItemState->Lock(0, 0, (void**)&pVtx, 0);	

	for (int nCntItemState = 0; nCntItemState < MAX_ITEM; nCntItemState++)
	{
		if (g_aItemState[nCntItemState].bUse == true)
		{//使用している
			g_aItemState[nCntItemState].nCounterAnim++;	//カウンタ加算

			if ((g_aItemState[nCntItemState].nCounterAnim % ANIMATION_SPEED) == 0)
			{//アニメーションの更新
				g_aItemState[nCntItemState].nPatternAnim = (g_aItemState[nCntItemState].nPatternAnim + 1 % COUNTER_ANIM_RESET);

				//テクスチャ座標更新
				pVtx[0].tex = D3DXVECTOR2(TEX_X * (g_aItemState[nCntItemState].nPatternAnim % COUNTER_ANIM_RESET), 1.0f *(g_aItemState[nCntItemState].nPatternAnim / COUNTER_ANIM_RESET));
				pVtx[1].tex = D3DXVECTOR2(TEX_X * (g_aItemState[nCntItemState].nPatternAnim % COUNTER_ANIM_RESET) + TEX_X, 1.0f *(g_aItemState[nCntItemState].nPatternAnim / COUNTER_ANIM_RESET));
				pVtx[2].tex = D3DXVECTOR2(TEX_X * (g_aItemState[nCntItemState].nPatternAnim % COUNTER_ANIM_RESET), 1.0f *(g_aItemState[nCntItemState].nPatternAnim / COUNTER_ANIM_RESET) + TEX_Y);
				pVtx[3].tex = D3DXVECTOR2(TEX_X * (g_aItemState[nCntItemState].nPatternAnim % COUNTER_ANIM_RESET) + TEX_X, 1.0f *(g_aItemState[nCntItemState].nPatternAnim / COUNTER_ANIM_RESET) + TEX_Y);
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItemState->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItemState(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffItemState, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for(int nCntItemState = 0; nCntItemState < MAX_ITEM; nCntItemState++)
	{
		if (g_aItemState[nCntItemState].bUse == true)	//アイテムが使用されている
		{
			pDevice->SetTexture(0, g_pTextureItemState[g_aItemState[nCntItemState].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 + (4 * nCntItemState), 2);
		}
	}
}

//=============================================================================
// アイテムの設定処理
//=============================================================================
void SetItemState(int nType)
{
	for (int nCntItemState = 0; nCntItemState < MAX_ITEM; nCntItemState++)
	{
		if (g_aItemState[nCntItemState].bUse == false)
		{//アイテムが使用されていない
			g_aItemState[nCntItemState].nType = nType;
			g_aItemState[nCntItemState].bUse = true;
			break;
		}
	}
}

ItemState *GetItemState(void)
{
	return &g_aItemState[0];
}