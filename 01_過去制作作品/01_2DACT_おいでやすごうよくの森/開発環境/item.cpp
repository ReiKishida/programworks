//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "item.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_ITEM		(12)								// アイテムの種類
#define ITEM_TEXTURENAME_0	"data/TEXTURE/coin000.png"		//アイテムのテクスチャ1
#define ITEM_TEXTURENAME_1	"data/TEXTURE/vodka.png"		//アイテムのテクスチャ2
#define ITEM_TEXTURENAME_2	"data/TEXTURE/arei.png"		//アイテムのテクスチャ3
#define ITEM_TEXTURENAME_3	"data/TEXTURE/watch.png"		//アイテムのテクスチャ4

#define ITEM_TEXTURENAME_4	"data/TEXTURE/coin000.png"		//アイテムのテクスチャ5
#define ITEM_TEXTURENAME_5	"data/TEXTURE/vodka.png"		//アイテムのテクスチャ6
#define ITEM_TEXTURENAME_6	"data/TEXTURE/arei2.png"		//アイテムのテクスチャ7
#define ITEM_TEXTURENAME_7	"data/TEXTURE/watch.png"		//アイテムのテクスチャ8

#define ITEM_TEXTURENAME_8	"data/TEXTURE/coinscore.png"		//アイテムのテクスチャ9
#define ITEM_TEXTURENAME_9	"data/TEXTURE/vodkascore.png"		//アイテムのテクスチャ10
#define ITEM_TEXTURENAME_10	"data/TEXTURE/satsuscore.png"		//アイテムのテクスチャ12
#define ITEM_TEXTURENAME_11	"data/TEXTURE/areyscore.png"		//アイテムのテクスチャ11

#define ITEM_X (50)
#define ITEM_Y (50)

#define ITEMLOST (0)

#define ITEM_JUMP_POWER (-9.0f)	//ジャンプ力
#define ITEM_GRAVITY (1.1f)		//重力
#define PAPER_GRAVITY (0.98f)		//重力

#define ITEM_GRAVITY_MAX (25.5f)	//重力の最大量

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureItem[MAX_TYPE_ITEM] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;					// 頂点バッファへのポインタ

ITEM g_aItem[MAX_ITEM];	//ブロック

//bool bJump;

float fItemJump;	//ジャンプ力
float fIitemITEM_GRAVITY;	//重力

//=============================================================================
// 初期化処理
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	

	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの情報の初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aItem[nCntItem].state = ITEMSTATE_NOMAL;
		g_aItem[nCntItem].bUse = false;
		g_aItem[nCntItem].fHeight = 0.0f;
		g_aItem[nCntItem].fWidth = 0.0f;
		g_aItem[nCntItem].bItemLand = false;
		//g_aItem[nCntItem].nCntLost = 0;
		//g_aItem[nCntItem].nCntGet = 0;
		
	}
	fIitemITEM_GRAVITY = ITEM_GRAVITY;
	
	

	/*for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		g_aItem[nCntItem].fLength = sqrtf((g_aItem[nCntItem].fWidth / 2 + g_aItem[nCntItem].fWidth / 2) * (g_aItem[nCntItem].fHeight / 2 + g_aItem[nCntItem].fHeight / 2));
		
		g_aItem[nCntItem].fAngle = atan2f(g_aItem[nCntItem].fWidth / 2, g_aItem[nCntItem].fHeight / 2);


	}*/

	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_0, &g_apTextureItem[0]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_1, &g_apTextureItem[1]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_2, &g_apTextureItem[2]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_3, &g_apTextureItem[3]);

	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_4, &g_apTextureItem[4]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_5, &g_apTextureItem[5]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_6, &g_apTextureItem[6]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_7, &g_apTextureItem[7]);

	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_8, &g_apTextureItem[8]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_9, &g_apTextureItem[9]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_10, &g_apTextureItem[10]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_11, &g_apTextureItem[11]);


	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(-D3DX_PI + g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
								  g_aItem[nCntItem].pos.y + cosf(-D3DX_PI + g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
								  0.0f);


		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(D3DX_PI - g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
								  g_aItem[nCntItem].pos.y + cosf(D3DX_PI - g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
								  0.0f);


		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(-g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
								  g_aItem[nCntItem].pos.y + cosf(-g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength, 
								  0.0f);


		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
								  g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
								  0.0f);

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
	g_pVtxBuffItem->Unlock();


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitItem(void)
{

	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_ITEM; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureItem[nCntTex] != NULL)
		{
			g_apTextureItem[nCntTex]->Release();
			g_apTextureItem[nCntTex] = NULL;
		}
	}


	// 頂点バッファの開放
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

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].posold = g_aItem[nCntItem].pos;
	}


	
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx+= 4)
	{
		if (g_aItem[nCntItem].bUse == true)	//アイテムが使用されている
		{

			switch (g_aItem[nCntItem].ItemType)
			{
			case ITEMTYPE_COIN:
				g_aItem[nCntItem].rot.x += 0.4f;
				g_aItem[nCntItem].rot.y += 0.4f;
				g_aItem[nCntItem].nCntAnim++;
				if ((g_aItem[nCntItem].nCntAnim % 10) == 0)
				{
					g_aItem[nCntItem].nPatternAnim = (g_aItem[nCntItem].nPatternAnim + 1) % 4;//アニメーションのパターンNo.を初期値に戻す
					//テクスチャ座標を更新
					pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
				}

				g_aItem[nCntItem].move.y += ITEM_GRAVITY;
				if (g_aItem[nCntItem].move.y >= ITEM_GRAVITY_MAX)
				{
					g_aItem[nCntItem].move.y = ITEM_GRAVITY_MAX;
				}


				g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;
				break;

			case ITEMTYPE_VODKA:
				g_aItem[nCntItem].rot.x += 0.3f;
				g_aItem[nCntItem].rot.y += 0.3f;
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				g_aItem[nCntItem].move.y += ITEM_GRAVITY;
				if (g_aItem[nCntItem].move.y >= ITEM_GRAVITY_MAX)
				{
					g_aItem[nCntItem].move.y = ITEM_GRAVITY_MAX;
				}

				/*g_aItem[nCntItem].move.x += (0.0f - g_aItem[nCntItem].move.x) * 0.15f;
				g_aItem[nCntItem].move.y += (0.0f - g_aItem[nCntItem].move.y) * 0.2f;*/

				g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

				break;
			case ITEMTYPE_AREY:
				g_aItem[nCntItem].rot.x += 0.25f;
				g_aItem[nCntItem].rot.y += 0.25f;
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



				g_aItem[nCntItem].move.y += ITEM_GRAVITY;
				if (g_aItem[nCntItem].move.y >= ITEM_GRAVITY_MAX)
				{
					g_aItem[nCntItem].move.y = ITEM_GRAVITY_MAX;
				}


				g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;
				break;

			case ITEMTYPE_PAPER:
				g_aItem[nCntItem].rot.x += 0.3f;
				g_aItem[nCntItem].rot.y += 0.3f;
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



				g_aItem[nCntItem].move.y += PAPER_GRAVITY;
				if (g_aItem[nCntItem].move.y >= ITEM_GRAVITY_MAX)
				{
					g_aItem[nCntItem].move.y = ITEM_GRAVITY_MAX;
				}


				g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
				g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;
				break;

			case ITEMTYPE_H_COIN:
				//g_aItem[nCntItem].fLength += 2.0f;
				g_aItem[nCntItem].nCntAnim++;
				if ((g_aItem[nCntItem].nCntAnim % 10) == 0)
				{
					g_aItem[nCntItem].nPatternAnim = (g_aItem[nCntItem].nPatternAnim + 1) % 4;//アニメーションのパターンNo.を初期値に戻す
																							  //テクスチャ座標を更新
					pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
				}
				break;

			case ITEMTYPE_H_VODKA:



				break;

			case ITEMTYPE_H_AREY:
				break;

			case ITEMTYPE_H_PAPER:
				break;

			case ITEMTYPE_COIN_SCORE:
				g_aItem[nCntItem].col.a -= 0.03f;

				g_aItem[nCntItem].nCntGet--;
				if (g_aItem[nCntItem].nCntGet <= 0)
				{
					g_aItem[nCntItem].bUse = false;
				}
				break;

			case ITEMTYPE_VODKA_SCORE:

				g_aItem[nCntItem].col.a -= 0.03f;

				g_aItem[nCntItem].nCntGet--;
				if (g_aItem[nCntItem].nCntGet <= 0)
				{
					g_aItem[nCntItem].bUse = false;
				}
				break;


			case ITEMTYPE_SATSU_SCORE:

				g_aItem[nCntItem].col.a -= 0.03f;

				g_aItem[nCntItem].nCntGet--;
				if (g_aItem[nCntItem].nCntGet <= 0)
				{
					g_aItem[nCntItem].bUse = false;
				}
				break;

			case ITEMTYPE_AREY_SCORE:
				g_aItem[nCntItem].col.a -= 0.03f;

				g_aItem[nCntItem].nCntGet--;
				if (g_aItem[nCntItem].nCntGet <= 0)
				{
					g_aItem[nCntItem].bUse = false;
				}
				break;
			}

			switch (g_aItem[nCntItem].state)
			{
			case ITEMSTATE_NOMAL:
				break;
			case ITEMSTATE_LOST:

				break;


			case ITEMSTATE_LAND:

				g_aItem[nCntItem].nCntLost--;
				if (g_aItem[nCntItem].nCntLost <= 0)
				{
					g_aItem[nCntItem].bUse = false;
				}
				break;
			}


			if (g_aItem[nCntItem].state == ITEMSTATE_NOMAL || g_aItem[nCntItem].state == ITEMSTATE_LAND)
			{
				//if (CollisionBlockItem(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posold, &g_aItem[nCntItem].move, g_aItem[nCntItem].fLength) == true)
				if (CollisionBlockItem2(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posold, &g_aItem[nCntItem].move, g_aItem[nCntItem].fWidth, g_aItem[nCntItem].fHeight) == true)
				{//接触判定

				 //g_aItem[nCntItem].bUse = false;
					if (g_aItem[nCntItem].state == ITEMSTATE_NOMAL)
					{
						g_aItem[nCntItem].move.y = (g_aItem[nCntItem].posold.y - g_aItem[nCntItem].pos.y) + 8;

						g_aItem[nCntItem].state = ITEMSTATE_LAND;
						g_aItem[nCntItem].nCntLost = 40;
					}
					if (g_aItem[nCntItem].ItemType == ITEMTYPE_AREY)
					{
						g_aItem[nCntItem].move.y = (g_aItem[nCntItem].posold.y - g_aItem[nCntItem].pos.y);

						g_aItem[nCntItem].state = ITEMSTATE_LAND;
						g_aItem[nCntItem].nCntLost = 30;
						g_aItem[nCntItem].move.x *= -1;

						/*g_aItem[nCntItem].rot.x -= 0.4f;
						g_aItem[nCntItem].rot.y -= 0.4f;*/
					}
				}

			}
		}
		//pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	

	


	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].pos.x >= 1290)
		{
			g_aItem[nCntItem].bUse = false;
		}

		if (g_aItem[nCntItem].pos.x <= -45)
		{
			g_aItem[nCntItem].bUse = false;
		}
		
	}

	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
	
			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(-D3DX_PI + g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
				g_aItem[nCntItem].pos.y + cosf(-D3DX_PI + g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
				0.0f);


			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(D3DX_PI - g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
				g_aItem[nCntItem].pos.y + cosf(D3DX_PI - g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
				0.0f);


			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(-g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
				g_aItem[nCntItem].pos.y + cosf(-g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
				0.0f);


			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
				g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
				0.0f);
		
		

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureItem[g_aItem[nCntItem].ItemType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}
}


//=============================================================================
// アイテムの設定
//=============================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, D3DXVECTOR3 move)
{
	int nCntItem;
	VERTEX_2D *pVtx;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		g_aItem[nCntItem].fLength = sqrtf((g_aItem[nCntItem].fWidth / 2 + g_aItem[nCntItem].fWidth / 2) * (g_aItem[nCntItem].fHeight / 2 + g_aItem[nCntItem].fHeight / 2));

		g_aItem[nCntItem].fAngle = atan2f(g_aItem[nCntItem].fWidth / 2, g_aItem[nCntItem].fHeight / 2);


	}

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aItem[nCntItem].bUse == false)	//アイテムが使用されていない
		{
			pVtx += (nCntItem * 4);

			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].move = move;
			g_aItem[nCntItem].state = ITEMSTATE_NOMAL;
			g_aItem[nCntItem].nCntGet = 25;

			if (type == ITEMTYPE_COIN || type == ITEMTYPE_H_COIN)
			{
				g_aItem[nCntItem].fHeight = 40;
				g_aItem[nCntItem].fWidth = 40;
			}
			if (type == ITEMTYPE_AREY || type == ITEMTYPE_H_AREY)
			{
				g_aItem[nCntItem].fHeight = 70;
				g_aItem[nCntItem].fWidth = 50;
			}
			if (type == ITEMTYPE_H_VODKA || type == ITEMTYPE_VODKA)
			{
				g_aItem[nCntItem].fHeight = 60;
				g_aItem[nCntItem].fWidth = 30;
			}
			if (type == ITEMTYPE_H_PAPER || type == ITEMTYPE_PAPER)
			{
				g_aItem[nCntItem].fHeight = 45;
				g_aItem[nCntItem].fWidth = 55;
			}
			if (type == ITEMTYPE_AREY_SCORE || type == ITEMTYPE_COIN_SCORE || type == ITEMTYPE_VODKA_SCORE || type == ITEMTYPE_SATSU_SCORE )
			{
				g_aItem[nCntItem].fHeight = 60;
				g_aItem[nCntItem].fWidth = 80;
			}

			if (type == ITEMTYPE_COIN || type == ITEMTYPE_H_COIN || type == ITEMTYPE_AREY || type == ITEMTYPE_H_AREY || type == ITEMTYPE_H_VODKA || type == ITEMTYPE_VODKA
				|| type == ITEMTYPE_H_PAPER || type == ITEMTYPE_PAPER || type == ITEMTYPE_H_PAPER || type == ITEMTYPE_PAPER)
			{
				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(-D3DX_PI + g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
					g_aItem[nCntItem].pos.y + cosf(-D3DX_PI + g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
					0.0f);


				pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(D3DX_PI - g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
					g_aItem[nCntItem].pos.y + cosf(D3DX_PI - g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
					0.0f);


				pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(-g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
					g_aItem[nCntItem].pos.y + cosf(-g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
					0.0f);


				pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength,
					g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength,
					0.0f);
			}

		

			if (type == ITEMTYPE_COIN || type == ITEMTYPE_H_COIN )
			{
				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
			}
			else
			{
				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			g_aItem[nCntItem].ItemType = type;

			g_aItem[nCntItem].bUse = true;	//使用している状態にする

			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}






//=============================================================================
// アイテムの取得
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}
