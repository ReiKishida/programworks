//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : komatsu
//
//=============================================================================
#include "item.h"
#include "player.h"
#include "score.h"	
#include "game.h"
#include "sound.h"
#include "itemstate.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ITEM_TEXTURENAME	"data/TEXTURE/snow_crystal001.png"	//表示するファイルの名前
#define MAX_ITEM_TEX		(1)									//読み込むテクスチャの数
#define ITEM_POS_X			(20.0f)								//アイテムの幅
#define ITEM_POS_Y			(20.0f)								//アイテムの高さ
#define COUNTER_ANIM_RESET	(4)									//パターン数
#define ANIMATION_SPEED		(10)								//アニメーション速度
#define TEX_X				(0.2f)								//テクスチャのX
#define TEX_Y				(1.0f)								//テクスチャのY

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureItem[MAX_ITEM] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//頂点バッファへのポインタ
Item g_aItem[MAX_ITEM];								//アイテムの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	// アイテムの情報の初期化
	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].nCounterAnim = 0;
		g_aItem[nCntItem].nPatternAnim = 0;
		g_aItem[nCntItem].fHeight = ITEM_POS_X;
		g_aItem[nCntItem].fWidth = ITEM_POS_Y;
		g_aItem[nCntItem].bUse = false;		
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,ITEM_TEXTURENAME,&g_pTextureItem[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);
		
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
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for (int nCntTex = 0; nCntTex < MAX_ITEM_TEX; nCntTex++)
	{
		//テクスチャの開放
		if (g_pTextureItem[nCntTex] != NULL)
		{
			g_pTextureItem[nCntTex]->Release();
			g_pTextureItem[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	Player pPlayer = *GetPlayer();

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);	

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{//使用している
			//頂点座標更新
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fHeight, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.z);

			g_aItem[nCntItem].nCounterAnim++;	//カウンタ加算

			if ((g_aItem[nCntItem].nCounterAnim % ANIMATION_SPEED) == 0)
			{//アニメーションの更新
				g_aItem[nCntItem].nPatternAnim = (g_aItem[nCntItem].nPatternAnim + 1 % COUNTER_ANIM_RESET);

				//テクスチャ座標更新
				pVtx[0].tex = D3DXVECTOR2(TEX_X * (g_aItem[nCntItem].nPatternAnim % COUNTER_ANIM_RESET), 1.0f *(g_aItem[nCntItem].nPatternAnim / COUNTER_ANIM_RESET));
				pVtx[1].tex = D3DXVECTOR2(TEX_X * (g_aItem[nCntItem].nPatternAnim % COUNTER_ANIM_RESET) + TEX_X, 1.0f *(g_aItem[nCntItem].nPatternAnim / COUNTER_ANIM_RESET));
				pVtx[2].tex = D3DXVECTOR2(TEX_X * (g_aItem[nCntItem].nPatternAnim % COUNTER_ANIM_RESET), 1.0f *(g_aItem[nCntItem].nPatternAnim / COUNTER_ANIM_RESET) + TEX_Y);
				pVtx[3].tex = D3DXVECTOR2(TEX_X * (g_aItem[nCntItem].nPatternAnim % COUNTER_ANIM_RESET) + TEX_X, 1.0f *(g_aItem[nCntItem].nPatternAnim / COUNTER_ANIM_RESET) + TEX_Y);
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)	//アイテムが使用されている
		{
			pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntItem].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 + (4 * nCntItem), 2);
		}
	}
}

//=============================================================================
// アイテムの設定処理
//=============================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{//アイテムが使用されていない
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;
			break;
		}
	}
}

//=============================================================================
// アイテムの当たり判定
//=============================================================================
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fHeight, float fWidth)
{
	Player *pPlayer;
	pPlayer = GetPlayer();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true && pPlayer->bItem == false)
		{//アイテムが使われている
			if (g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth >= pPos->x
				&& g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth <= pPos->x)
			{//アイテムと同じy軸にいる
				if (g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight <= pPos->y
					&& g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight >= pPos->y)
				{//アイテムと同じx軸にいる
					SetItemState(0);
					AddScore(500);
					pPlayer->bItem = true;
					g_aItem[nCntItem].bUse = false;
				}
			}
		}
	}
}