//=============================================================================
//
// チャージバー処理 [Stamina.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "stamina.h"
#include "main.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON (2)
#define MAX_BAR (2)
#define STAMINA_WIDTH		(80)				//背景の幅
#define STAMINA_HEIGHT		(5)				//背景の高さ

#define STAMINA_WIDTH2 (1.0f)

#define STAMINA_TEXTURENAME_0	"data/TEXTURE/chargebar.png"		//背景のテクスチャ1
#define STAMINA_TEXTURENAME_1	"data/TEXTURE/chargebar.png"		//背景のテクスチャ1

#define STAMINA_SPEED (5.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexStamina(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStamina[MAX_BAR] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStamina = NULL;	// 頂点バッファへのポインタ
STAMINA					g_Stamina[MAX_BAR];					// フェード状態

//=============================================================================
// 初期化処理
//=============================================================================
void InitStamina(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCharge = 0; nCntCharge < MAX_BAR; nCntCharge++)
	{
		g_Stamina[nCntCharge].pos = D3DXVECTOR3(560, 450, 0.0f);
		g_Stamina[nCntCharge].nType = nCntCharge;
	}




	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, STAMINA_TEXTURENAME_0, &g_pTextureStamina[0]);
	D3DXCreateTextureFromFile(pDevice, STAMINA_TEXTURENAME_1, &g_pTextureStamina[1]);


	// 頂点情報の設定
	MakeVertexStamina(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitStamina(void)
{
	for (int nCntCharge = 0; nCntCharge < MAX_BAR; nCntCharge++)
	{
		//テクスチャの破棄
		if (g_pTextureStamina[nCntCharge] != NULL)
		{
			g_pTextureStamina[nCntCharge]->Release();
			g_pTextureStamina[nCntCharge] = NULL;
		}

	}

	//頂点バッファの破棄
	if (g_pVtxBuffStamina != NULL)
	{
		g_pVtxBuffStamina->Release();
		g_pVtxBuffStamina = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStamina(void)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	//float Stamina;

	switch (pPlayer->state)
	{


		case PLAYERSTATE_NORMAL:
		


			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffStamina->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラー
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		

			//頂点バッファをアンロック
			g_pVtxBuffStamina->Unlock();

			break;
		
	


	case PLAYERSTATE_TIRED:
	
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffStamina->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー
		pVtx[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//頂点の座標
		pVtx[4].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1, g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1, g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1, g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1, g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);

		//頂点バッファをアンロック
		g_pVtxBuffStamina->Unlock();
	
		break;

}
	
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{

			pPlayer->nCntStamina++;

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffStamina->Lock(0, 0, (void**)&pVtx, 0);

			//頂点の座標
			pVtx[4].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1.0f, g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
			pVtx[5].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1.0f + (STAMINA_SPEED * pPlayer->nCntStamina), g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
			pVtx[6].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1.0f, g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);
			pVtx[7].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1.0f + (STAMINA_SPEED * pPlayer->nCntStamina), g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);

			//頂点バッファをアンロック
			g_pVtxBuffStamina->Unlock();

		}
	}

	//if (GetKeyboardPress(DIK_RETURN) != true)
	//{

	//	//頂点バッファをロックし、頂点データへのポインタを取得
	//	g_pVtxBuffStamina->Lock(0, 0, (void**)&pVtx, 0);

	//	//頂点の座標
	//	pVtx[4].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1, g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
	//	pVtx[5].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1, g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
	//	pVtx[6].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1, g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);
	//	pVtx[7].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1, g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);


	//	//頂点カラー
	//	pVtx[4].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[5].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[6].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[7].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	//	//頂点バッファをアンロック
	//	g_pVtxBuffStamina->Unlock();
	//}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawStamina(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffStamina, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCharge = 0; nCntCharge < MAX_BAR; nCntCharge++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureStamina[g_Stamina[nCntCharge].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCharge * 4, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexStamina(LPDIRECT3DDEVICE9 pDevice)
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
		&g_pVtxBuffStamina,
		NULL);


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStamina->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Stamina[0].pos.x - 18, g_Stamina[0].pos.y - (STAMINA_HEIGHT + 6), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Stamina[0].pos.x + (STAMINA_WIDTH + 40), g_Stamina[0].pos.y - (STAMINA_HEIGHT + 6), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Stamina[0].pos.x - 18, g_Stamina[0].pos.y + (STAMINA_HEIGHT + 6), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Stamina[0].pos.x + (STAMINA_WIDTH + 40), g_Stamina[0].pos.y + (STAMINA_HEIGHT + 6), 0.0f);

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
	pVtx[0].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1, g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1, g_Stamina[1].pos.y - STAMINA_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Stamina[1].pos.x - 1, g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Stamina[1].pos.x + 1, g_Stamina[1].pos.y + STAMINA_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffStamina->Unlock();
}



