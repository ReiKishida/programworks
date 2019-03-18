//=============================================================================
//
// アイテム処理 [PopScore.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "popscore.h"
#include "player.h"
#include "main.h"
#include "input.h"
#include "game.h"
#include "block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_POPSCORE		(4)								// アイテムの種類
#define POPSCORE_TEXTURENAME_0	"data/TEXTURE/coinscore.png"		//アイテムのテクスチャ1
#define POPSCORE_TEXTURENAME_1	"data/TEXTURE/vodkascore.png"		//アイテムのテクスチャ2
#define POPSCORE_TEXTURENAME_2	"data/TEXTURE/satsuscore.png"		//アイテムのテクスチャ4
#define POPSCORE_TEXTURENAME_3	"data/TEXTURE/areyscore.png"		//アイテムのテクスチャ3


#define POPSCORE_JUMP_POWER (-9.0f)	//ジャンプ力
#define POPSCORE_GRAVITY (1.0f)		//重力

#define POPSCORE_GRAVITY_MAX (25.0f)	//重力の最大量

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTexturePopScore[MAX_TYPE_POPSCORE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIPopScore = NULL;					// 頂点バッファへのポインタ

POPSCORE g_aPopScore[MAX_POPSCORE];	//ブロック

						//bool bJump;


//=============================================================================
// 初期化処理
//=============================================================================
void InitPopScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;


	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの情報の初期化
	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		g_aPopScore[nCntPopScore].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPopScore[nCntPopScore].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPopScore[nCntPopScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPopScore[nCntPopScore].state = POPSCORESTATE_NOMAL;
		g_aPopScore[nCntPopScore].bUse = false;
		
		g_aPopScore[nCntPopScore].nCntGet = 0;

	}



	
	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_0, &g_apTexturePopScore[0]);
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_1, &g_apTexturePopScore[1]);
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_2, &g_apTexturePopScore[2]);
	D3DXCreateTextureFromFile(pDevice, POPSCORE_TEXTURENAME_3, &g_apTexturePopScore[3]);


	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POPSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIPopScore,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);

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
	g_pVtxBuffIPopScore->Unlock();


}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitPopScore(void)
{

	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_POPSCORE; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTexturePopScore[nCntTex] != NULL)
		{
			g_apTexturePopScore[nCntTex]->Release();
			g_apTexturePopScore[nCntTex] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffIPopScore != NULL)
	{
		g_pVtxBuffIPopScore->Release();
		g_pVtxBuffIPopScore = NULL;
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePopScore(void)
{

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		g_aPopScore[nCntPopScore].posold = g_aPopScore[nCntPopScore].pos;
	}


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		if (g_aPopScore[nCntPopScore].bUse == true)	//アイテムが使用されている
		{

			switch (g_aPopScore[nCntPopScore].PopScoreType)
			{
			case POPSCORETYPE_COIN_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
				g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}
				//g_aPopScore[nCntPopScore].col.a -= 0.03f;


				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;
				break;

			case POPSCORETYPE_VODKA_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
				g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}

				//g_aPopScore[nCntPopScore].col.a -= 0.03f;

				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;

				break;
			case POPSCORETYPE_SATSU_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
				g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}

				//g_aPopScore[nCntPopScore].col.a -= 0.03f;

				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;
				break;

			case POPSCORETYPE_AREY_SCORE:
				g_aPopScore[nCntPopScore].nCntLost--;
				if (g_aPopScore[nCntPopScore].nCntLost <= 0)
				{
					g_aPopScore[nCntPopScore].bUse = false;
				}
			//	g_aPopScore[nCntPopScore].col.a -= 0.03f;
			g_aPopScore[nCntPopScore].move.y += POPSCORE_GRAVITY;
				if (g_aPopScore[nCntPopScore].move.y >= POPSCORE_GRAVITY_MAX)
				{
					g_aPopScore[nCntPopScore].move.y = POPSCORE_GRAVITY_MAX;
				}


				g_aPopScore[nCntPopScore].pos.x += g_aPopScore[nCntPopScore].move.x;
				g_aPopScore[nCntPopScore].pos.y += g_aPopScore[nCntPopScore].move.y;
				break;

			
			}

		}

		pVtx[0].col = g_aPopScore[nCntPopScore].col;
		pVtx[1].col = g_aPopScore[nCntPopScore].col;
		pVtx[2].col = g_aPopScore[nCntPopScore].col;
		pVtx[3].col = g_aPopScore[nCntPopScore].col;

		pVtx += 4;	//頂点データのポインタを4つ進める

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffIPopScore->Unlock();




	

	

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);

		


		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffIPopScore->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPopScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffIPopScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
		if (g_aPopScore[nCntPopScore].bUse == true)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePopScore[g_aPopScore[nCntPopScore].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPopScore * 4, 2);
		}
	}
}


//=============================================================================
// アイテムの設定
//=============================================================================
void SetPopScore(D3DXVECTOR3 pos, int type, D3DXVECTOR3 move)
{
	int nCntPopScore;
	VERTEX_2D *pVtx;


	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffIPopScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntPopScore = 0; nCntPopScore < MAX_POPSCORE; nCntPopScore++)
	{
	
		if (g_aPopScore[nCntPopScore].bUse == false)	//アイテムが使用されていない
		{
			pVtx += (nCntPopScore * 4);

			g_aPopScore[nCntPopScore].pos = pos;
			g_aPopScore[nCntPopScore].move = move;
			
			g_aPopScore[nCntPopScore].nCntLost = 18;

			if (type == 0)
			{
				g_aPopScore[nCntPopScore].fHeight = 35;
				g_aPopScore[nCntPopScore].fWidth = 70;
			}

			if (type == 1)
			{
				g_aPopScore[nCntPopScore].fHeight = 40;
				g_aPopScore[nCntPopScore].fWidth = 75;
			}

			if (type == 2)
			{
				g_aPopScore[nCntPopScore].fHeight = 80;
				g_aPopScore[nCntPopScore].fWidth = 100;
			}

			if (type == 3)
			{
				g_aPopScore[nCntPopScore].fHeight = 40;
				g_aPopScore[nCntPopScore].fWidth = 80;
			}
			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y - g_aPopScore[nCntPopScore].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x - g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPopScore[nCntPopScore].pos.x + g_aPopScore[nCntPopScore].fWidth, g_aPopScore[nCntPopScore].pos.y, 0.0f);


			g_aPopScore[nCntPopScore].nType = type;

			g_aPopScore[nCntPopScore].bUse = true;	//使用している状態にする

			break;


			
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffIPopScore->Unlock();
}



//=============================================================================
// アイテムの取得
//=============================================================================
POPSCORE *GetPopScore(void)
{
		return &g_aPopScore[0];
}
