//=============================================================================
//
// チャージバー処理 [chargebar.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "chargebar.h"
#include "main.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON (2)
#define MAX_BAR (2)
#define CHARGEBAR_WIDTH		(80)				//背景の幅
#define CHARGEBAR_HEIGHT		(5)				//背景の高さ
#define CHARGEBAR_POS_X		(0)							//背景左上X座標
#define CHARGEBAR_POS_Y		(0)							//背景左上Y座標

#define CHARGEBAR_TEXTURENAME_0	"data/TEXTURE/chargebar.png"		//背景のテクスチャ1
#define CHARGEBAR_TEXTURENAME_1	"data/TEXTURE/chargebar.png"		//背景のテクスチャ1

#define CHARGE_SPEED (0.55f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexChargebar(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureChargebar[MAX_BAR] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargebar = NULL;	// 頂点バッファへのポインタ
CHARGEBAR					g_aChargebar[MAX_BAR];					// フェード状態

//=============================================================================
// 初期化処理
//=============================================================================
void InitChargebar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCharge = 0; nCntCharge < MAX_BAR; nCntCharge++)
	{
		g_aChargebar[nCntCharge].pos = D3DXVECTOR3(560, 420, 0.0f);
		g_aChargebar[nCntCharge].nType = nCntCharge;
	}
	
	


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, CHARGEBAR_TEXTURENAME_0, &g_pTextureChargebar[0]);
	D3DXCreateTextureFromFile(pDevice, CHARGEBAR_TEXTURENAME_1, &g_pTextureChargebar[1]);


	// 頂点情報の設定
	MakeVertexChargebar(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitChargebar(void)
{
	for (int nCntCharge = 0; nCntCharge < MAX_BAR; nCntCharge++)
	{
		//テクスチャの破棄
		if (g_pTextureChargebar[nCntCharge] != NULL)
		{
			g_pTextureChargebar[nCntCharge]->Release();
			g_pTextureChargebar[nCntCharge] = NULL;
		}

	}

	//頂点バッファの破棄
	if (g_pVtxBuffChargebar != NULL)
	{
		g_pVtxBuffChargebar->Release();
		g_pVtxBuffChargebar = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateChargebar(void)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	//int ChargeBar;

	if (pPlayer->state == PLAYERSTATE_NORMAL )
	{
		if (GetKeyboardPress(DIK_RETURN) == true)
		{

			pPlayer->nCntCharge++;

			//ChargeBar = (0.8f * pPlayer->nCounterState);

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffChargebar->Lock(0, 0, (void**)&pVtx, 0);

			//頂点の座標
			pVtx[4].pos = D3DXVECTOR3(g_aChargebar[1].pos.x - 1.0f, g_aChargebar[1].pos.y - CHARGEBAR_HEIGHT, 0.0f);
			pVtx[5].pos = D3DXVECTOR3(g_aChargebar[1].pos.x + 1.0f + (CHARGE_SPEED * pPlayer->nCntCharge), g_aChargebar[1].pos.y - CHARGEBAR_HEIGHT, 0.0f);
			pVtx[6].pos = D3DXVECTOR3(g_aChargebar[1].pos.x - 1.0f, g_aChargebar[1].pos.y + CHARGEBAR_HEIGHT, 0.0f);
			pVtx[7].pos = D3DXVECTOR3(g_aChargebar[1].pos.x + 1.0f + (CHARGE_SPEED * pPlayer->nCntCharge), g_aChargebar[1].pos.y + CHARGEBAR_HEIGHT, 0.0f);

			//頂点バッファをアンロック
			g_pVtxBuffChargebar->Unlock();

		}
	}

	if (pPlayer->state == PLAYERSTATE_CHARGE)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffChargebar->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー
		pVtx[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffChargebar->Unlock();
	}


	if (GetKeyboardPress(DIK_RETURN) != true)
	{

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffChargebar->Lock(0, 0, (void**)&pVtx, 0);

		//頂点の座標
		pVtx[4].pos = D3DXVECTOR3(g_aChargebar[1].pos.x - 1, g_aChargebar[1].pos.y - CHARGEBAR_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_aChargebar[1].pos.x + 1, g_aChargebar[1].pos.y - CHARGEBAR_HEIGHT, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_aChargebar[1].pos.x - 1, g_aChargebar[1].pos.y + CHARGEBAR_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_aChargebar[1].pos.x + 1, g_aChargebar[1].pos.y + CHARGEBAR_HEIGHT, 0.0f);


		//頂点カラー
		pVtx[4].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffChargebar->Unlock();
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawChargebar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffChargebar, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCharge = 0; nCntCharge < MAX_BAR; nCntCharge++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureChargebar[g_aChargebar[nCntCharge].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCharge * 4, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexChargebar(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChargebar,
		NULL);

	
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffChargebar->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aChargebar[0].pos.x - 18, g_aChargebar[0].pos.y - (CHARGEBAR_HEIGHT + 8), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aChargebar[0].pos.x + (CHARGEBAR_WIDTH + 40), g_aChargebar[0].pos.y - (CHARGEBAR_HEIGHT + 8), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aChargebar[0].pos.x - 18, g_aChargebar[0].pos.y + (CHARGEBAR_HEIGHT + 8), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aChargebar[0].pos.x + (CHARGEBAR_WIDTH + 40), g_aChargebar[0].pos.y + (CHARGEBAR_HEIGHT + 8), 0.0f);

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
	pVtx[0].pos = D3DXVECTOR3(g_aChargebar[1].pos.x - 1,g_aChargebar[1].pos.y - CHARGEBAR_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aChargebar[1].pos.x + 1, g_aChargebar[1].pos.y - CHARGEBAR_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aChargebar[1].pos.x - 1, g_aChargebar[1].pos.y + CHARGEBAR_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aChargebar[1].pos.x + 1, g_aChargebar[1].pos.y + CHARGEBAR_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffChargebar->Unlock();
}



