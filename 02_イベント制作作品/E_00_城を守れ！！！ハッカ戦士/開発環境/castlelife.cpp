//=============================================================================
//
// チャージバー処理 [CastleLife.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "castlelife.h"
#include "main.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON (2)
#define MAX_BAR (2)
#define CASTLELIFE_WIDTH		(80)				//背景の幅
#define CASTLELIFE_HEIGHT		(5)				//背景の高さ

#define CASTLELIFE_WIDTH2 (1.0f)

#define CASTLELIFE_TEXTURENAME_0	"data/TEXTURE/chargebar.png"		//背景のテクスチャ1
#define CASTLELIFE_TEXTURENAME_1	"data/TEXTURE/chargebar.png"		//背景のテクスチャ1

#define CASTLELIFE_SPEED (5.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexCastleLife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCastleLife[MAX_BAR] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCastleLife = NULL;	// 頂点バッファへのポインタ
CASTLELIFE					g_CastleLife[MAX_BAR];					// フェード状態

//=============================================================================
// 初期化処理
//=============================================================================
void InitCastleLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCastleLife = 0; nCntCastleLife < MAX_BAR; nCntCastleLife++)
	{
		g_CastleLife[nCntCastleLife].pos = D3DXVECTOR3(110, 220, 0.0f);
		g_CastleLife[nCntCastleLife].nType = nCntCastleLife;
	}




	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, CASTLELIFE_TEXTURENAME_0, &g_pTextureCastleLife[0]);
	D3DXCreateTextureFromFile(pDevice, CASTLELIFE_TEXTURENAME_1, &g_pTextureCastleLife[1]);


	// 頂点情報の設定
	MakeVertexCastleLife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitCastleLife(void)
{
	for (int nCntCastleLife = 0; nCntCastleLife < MAX_BAR; nCntCastleLife++)
	{
		//テクスチャの破棄
		if (g_pTextureCastleLife[nCntCastleLife] != NULL)
		{
			g_pTextureCastleLife[nCntCastleLife]->Release();
			g_pTextureCastleLife[nCntCastleLife] = NULL;
		}

	}

	//頂点バッファの破棄
	if (g_pVtxBuffCastleLife != NULL)
	{
		g_pVtxBuffCastleLife->Release();
		g_pVtxBuffCastleLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCastleLife(void)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	//float CastleLife;

	switch (pPlayer->state)
	{


	case PLAYERSTATE_NORMAL:



		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffCastleLife->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー
		pVtx[4].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);



		//頂点バッファをアンロック
		g_pVtxBuffCastleLife->Unlock();

		break;

	}


	
	

	//if (GetKeyboardPress(DIK_RETURN) != true)
	//{

	//	//頂点バッファをロックし、頂点データへのポインタを取得
	//	g_pVtxBuffCastleLife->Lock(0, 0, (void**)&pVtx, 0);

	//	//頂点の座標
	//	pVtx[4].pos = D3DXVECTOR3(g_CastleLife[1].pos.x - 1, g_CastleLife[1].pos.y - CASTLELIFE_HEIGHT, 0.0f);
	//	pVtx[5].pos = D3DXVECTOR3(g_CastleLife[1].pos.x + 1, g_CastleLife[1].pos.y - CASTLELIFE_HEIGHT, 0.0f);
	//	pVtx[6].pos = D3DXVECTOR3(g_CastleLife[1].pos.x - 1, g_CastleLife[1].pos.y + CASTLELIFE_HEIGHT, 0.0f);
	//	pVtx[7].pos = D3DXVECTOR3(g_CastleLife[1].pos.x + 1, g_CastleLife[1].pos.y + CASTLELIFE_HEIGHT, 0.0f);


	//	//頂点カラー
	//	pVtx[4].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[5].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[6].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[7].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	//	//頂点バッファをアンロック
	//	g_pVtxBuffCastleLife->Unlock();
	//}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawCastleLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffCastleLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCastleLife = 0; nCntCastleLife < MAX_BAR; nCntCastleLife++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCastleLife[g_CastleLife[nCntCastleLife].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCastleLife * 4, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexCastleLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCastleLife,
		NULL);


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCastleLife->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_CastleLife[0].pos.x - 18, g_CastleLife[0].pos.y - (CASTLELIFE_HEIGHT + 10), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_CastleLife[0].pos.x + (CASTLELIFE_WIDTH + 30), g_CastleLife[0].pos.y - (CASTLELIFE_HEIGHT + 10), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_CastleLife[0].pos.x - 18, g_CastleLife[0].pos.y + (CASTLELIFE_HEIGHT + 10), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_CastleLife[0].pos.x + (CASTLELIFE_WIDTH + 30), g_CastleLife[0].pos.y + (CASTLELIFE_HEIGHT + 10), 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.5f);
	pVtx[1].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.5f);
	pVtx[2].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.5f);
	pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.5f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_CastleLife[1].pos.x - 1, g_CastleLife[1].pos.y - CASTLELIFE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_CastleLife[1].pos.x + CASTLELIFE_WIDTH, g_CastleLife[1].pos.y - CASTLELIFE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_CastleLife[1].pos.x - 1, g_CastleLife[1].pos.y + CASTLELIFE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_CastleLife[1].pos.x + CASTLELIFE_WIDTH, g_CastleLife[1].pos.y + CASTLELIFE_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[4].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);


	//テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffCastleLife->Unlock();
}


//=============================================================================
// ダメージ
//=============================================================================
//void HitCastle(int nDamage)
//{
//	
//	VERTEX_2D*pVtx;	//頂点情報へのポインタ
//
//			
//
//			//頂点バッファをロックし、頂点データへのポインタを取得
//			g_pVtxBuffCastleLife->Lock(0, 0, (void**)&pVtx, 0);
//
//			//頂点の座標
//			pVtx[4].pos = D3DXVECTOR3(g_CastleLife[1].pos.x - 1.0f, g_CastleLife[1].pos.y - CASTLELIFE_HEIGHT, 0.0f);
//			pVtx[5].pos = D3DXVECTOR3(g_CastleLife[1].pos.x + 1.0f + (CASTLELIFE_SPEED * nDamage), g_CastleLife[1].pos.y - CASTLELIFE_HEIGHT, 0.0f);
//			pVtx[6].pos = D3DXVECTOR3(g_CastleLife[1].pos.x - 1.0f, g_CastleLife[1].pos.y + CASTLELIFE_HEIGHT, 0.0f);
//			pVtx[7].pos = D3DXVECTOR3(g_CastleLife[1].pos.x + 1.0f + (CASTLELIFE_SPEED * nDamage), g_CastleLife[1].pos.y + CASTLELIFE_HEIGHT, 0.0f);
//
//			//頂点バッファをアンロック
//			g_pVtxBuffCastleLife->Unlock();
//
//		
//	
//}