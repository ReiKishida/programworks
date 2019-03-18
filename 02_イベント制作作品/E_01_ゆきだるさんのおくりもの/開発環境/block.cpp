//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : komatsu
//
//=============================================================================
#include "block.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "time.h"
#include "player.h"
#include "score.h"
#include "itemstate.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BLOCK_TEXTURENAME1 "data/TEXTURE/Home01.png"		//テクスチャ1
#define BLOCK_TEXTURENAME2 "data/TEXTURE/Home02.png"		//テクスチャ2
#define BLOCK_TEXTURENAME3 "data/TEXTURE/Home03.png"		//テクスチャ3
#define BLOCK_TEXTURENAME4 "data/TEXTURE/Home04.png"		//テクスチャ4
#define MAX_BLOCKTEXTURE	(4)								//テクスチャの最大数
#define	MAX_BLOCK			(128)							//ブロックの最大数
#define MAX_PATTERN			(1)								//パターンの数
#define ANIMATION_SPEED		(1)								//アニメーション速度
#define COLLISION_AREA		(10.0f)							//当たり判定の調整
#define TEX_X				(1.0f)							//テクスチャのX
#define TEX_Y				(1.0f)							//テクスチャのY

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureBlock[MAX_BLOCK] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;				// 頂点バッファへのポインタ
Block g_aBlock[MAX_BLOCK];									//ブロックの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// ブロックの情報の初期化
	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].nCounterAnim = 0;
		g_aBlock[nCntBlock].nPatternAnim = 0;
		g_aBlock[nCntBlock].bChange = false;
		g_aBlock[nCntBlock].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME1, &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME2, &g_pTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME3, &g_pTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME4, &g_pTextureBlock[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight, g_aBlock[nCntBlock].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight, g_aBlock[nCntBlock].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, g_aBlock[nCntBlock].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, g_aBlock[nCntBlock].pos.z);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	int nCntTex;

	for (nCntTex = 0; nCntTex < MAX_BLOCKTEXTURE; nCntTex++)
	{
		// テクスチャの開放
		if (g_pTextureBlock[nCntTex] != NULL)
		{
			g_pTextureBlock[nCntTex]->Release();
			g_pTextureBlock[nCntTex] = NULL;
		}
	}
	
	// 頂点バッファの開放
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{
	Player pPlayer = *GetPlayer();

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight - pPlayer.move.y, g_aBlock[nCntBlock].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight - pPlayer.move.y, g_aBlock[nCntBlock].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight - pPlayer.move.y, g_aBlock[nCntBlock].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight - pPlayer.move.y, g_aBlock[nCntBlock].pos.z);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)	//ブロックが使用されている
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 + (4 * nCntBlock), 2);
		}
	}
}
//=============================================================================
// ブロックの設定処理
//=============================================================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)	
		{//ブロックが使用されていない
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].nType = nType;
			g_aBlock[nCntBlock].bChange = false;
			g_aBlock[nCntBlock].bUse = true;
			break;
		}
	}
}

//=============================================================================
// ブロックの当たり判定処理
//=============================================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fWidth)
{
	bool bLand = false;
	Player *pPlayer;
	pPlayer = GetPlayer();

	ItemState *pItemState;
	pItemState = GetItemState();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//ブロックが使用されている
			if (g_aBlock[nCntBlock].pos.x + (fWidth - COLLISION_AREA) >= pPos->x - g_aBlock[nCntBlock].fWidth		//左
				&& g_aBlock[nCntBlock].pos.x - (fWidth - COLLISION_AREA) <= pPos->x + g_aBlock[nCntBlock].fWidth)	//右
			{//ブロックと同じy軸にいる
				if (g_aBlock[nCntBlock].pos.y + fHeight >= pPos->y - g_aBlock[nCntBlock].fHeight					//上
					&& g_aBlock[nCntBlock].pos.y <= pPos->y + g_aBlock[nCntBlock].fHeight)							//下
				{//ブロックと同じx軸にいる
					if (pPos->y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight
						&& pPosOld->y <= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight)
					{//上から
						pPos->y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight;
						pMove->y = 0.0f;
					}
					else if (pPos->y <= g_aBlock[nCntBlock].pos.y + fHeight
						&& pPosOld->y >= g_aBlock[nCntBlock].pos.y + fHeight)
					{//下から
						if (pPlayer->bItem == true && g_aBlock[nCntBlock].bChange == false)
						{
							pItemState->bUse = false;
							g_aBlock[nCntBlock].bChange = true;
							pPlayer->bItem = false;
							AddScore(1000);
						}
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y + fHeight;
						pMove->y = 0.0f;
					}
					else if (pPos->x >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth - (fWidth - COLLISION_AREA)
						&& pPosOld->x <= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth)
					{//左から
						pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth - (fWidth - COLLISION_AREA);
						pMove->x = 0.0f;
					}
					else if (pPos->x <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + (fWidth - COLLISION_AREA)
						&& pPosOld->x >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
					{//右から
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + (fWidth - COLLISION_AREA);
						pMove->x = 0.0f;
					}
				}
			}
		}
	}
	return bLand;
}

Block *GetBlock(void)
{
	return &g_aBlock[0];
}