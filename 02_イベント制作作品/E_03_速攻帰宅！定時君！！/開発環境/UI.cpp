////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UI処理 [UI.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "UI.h"
#include "player.h"
#include "effect2D.h"
#include "input.h"
#include "game.h"
#include "result.h"
#include "gamepad.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ブロックの中心座標
	D3DXCOLOR  col;				// 色
	UITEX nType;				// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	D3DXVECTOR3 Stoppos;
} UIInfo;
typedef struct
{
	char *pFileName;
}UITexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************

#define MAX_UI	(17)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexUI(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureUI[UITEX_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;		// 頂点バッファへのポインタ
UI						g_aUI[MAX_UI];
// テクスチャ名
UITexture  g_aUITex[UITEX_MAX] =
{
	{ "data/TEXTURE/UI_R.png" },			// 黒い幕
	{ "data/TEXTURE/UI_L.png" },			// 黒い幕
	{ "data/TEXTURE/konma.png" },			// 黒い幕

};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < UITEX_MAX; nTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			g_aUITex[nTex].pFileName,		// ファイルの名前
			&g_pTextureUI[nTex]);	// テクスチャへのポインタ
	}
	// 初期化
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		g_aUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心
		g_aUI[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
		g_aUI[nCnt].nType = UITEX_MAX;						// 種類
		g_aUI[nCnt].fWidth = 0;									// 幅
		g_aUI[nCnt].fHeight = 0;									// 高さ
		g_aUI[nCnt].bUse = false;								// 使用されているかどうか
	}
	// 頂点情報の作成
	MakeVertexUI(pDevice);
	SetUI(D3DXVECTOR3(560, 80.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), UI_KONMA, 60, 70);
	SetUI(D3DXVECTOR3(720, 80.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), UI_KONMA, 60, 70);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitUI(void)
{
	// テクスチャの開放
	for (int nTex = 0; nTex < UITEX_MAX; nTex++)
	{
		if (g_pTextureUI[nTex] != NULL)
		{
			g_pTextureUI[nTex]->Release();
			g_pTextureUI[nTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateUI(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	Player *pPlayer = GetPlayer();
	int nCntPow = 0;
	int nCntPow2 = 0;
//-------------------------------------/ ヒット数に応じたもの /----------------------------------------------------------------//
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++, pVtx += 4)
	{
		if (g_aUI[nCntUI].bUse == true)
		{

			if (g_aUI[nCntUI].nType == UITEX_R)
			{
				if (GetKeyboardTrigger(DIK_J) || GetGamePadTrigger(0, BUTTON_R1))
				{
					g_aUI[nCntUI].fWidth = g_aUI[nCntUI].fWidthInit*1.2f;
					g_aUI[nCntUI].fHeight = g_aUI[nCntUI].fHeightInit*1.2f;

				}
				g_aUI[nCntUI].fWidth += (g_aUI[nCntUI].fWidthInit - g_aUI[nCntUI].fWidth)*0.1f;
				g_aUI[nCntUI].fHeight += (g_aUI[nCntUI].fHeightInit - g_aUI[nCntUI].fHeight)*0.1f;
			}
			if (g_aUI[nCntUI].nType == UITEX_L)
			{

				if (GetKeyboardTrigger(DIK_F) || GetGamePadTrigger(0, BUTTON_L1))
				{
					g_aUI[nCntUI].fWidth = g_aUI[nCntUI].fWidthInit*1.2f;
					g_aUI[nCntUI].fHeight = g_aUI[nCntUI].fHeightInit*1.2f;
				}
				g_aUI[nCntUI].fWidth += (g_aUI[nCntUI].fWidthInit - g_aUI[nCntUI].fWidth)*0.1f;
				g_aUI[nCntUI].fHeight += (g_aUI[nCntUI].fHeightInit - g_aUI[nCntUI].fHeight)*0.1f;

			}
			// 頂点情報を設定
			pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			// 頂点カラーの設定
			pVtx[0].col = g_aUI[nCntUI].col;
			pVtx[1].col = g_aUI[nCntUI].col;
			pVtx[2].col = g_aUI[nCntUI].col;
			pVtx[3].col = g_aUI[nCntUI].col;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}


//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nTex = 0; nTex < MAX_UI; nTex++)
	{
		if (g_aUI[nTex].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureUI[g_aUI[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexUI(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++, pVtx += 4)
	{
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCnt].fWidth / 2, -g_aUI[nCnt].fHeight/2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCnt].fWidth / 2, -g_aUI[nCnt].fHeight/2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCnt].fWidth / 2, g_aUI[nCnt].fHeight / 2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCnt].fWidth / 2, g_aUI[nCnt].fHeight / 2, 0.0f) + g_aUI[nCnt].pos;
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// 頂点カラーの設定
		pVtx[0].col = g_aUI[nCnt].col;
		pVtx[1].col = g_aUI[nCnt].col;
		pVtx[2].col = g_aUI[nCnt].col;
		pVtx[3].col = g_aUI[nCnt].col;
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}
//==============================================================================================================================
// スコアの設定
//==============================================================================================================================
void SetUI(D3DXVECTOR3 pos, D3DXCOLOR col, UITEX nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++, pVtx += 4)
	{
		if (g_aUI[nCntUI].bUse == false)
		{
			g_aUI[nCntUI].pos = pos;			// 中心
			g_aUI[nCntUI].col = col;			// 色
			g_aUI[nCntUI].nType = nType;		// 種類
			g_aUI[nCntUI].fWidth = fWidth;		// 幅
			g_aUI[nCntUI].fHeight = fHeight;	// 高さ
			g_aUI[nCntUI].fWidthInit = fWidth;		// 幅
			g_aUI[nCntUI].fHeightInit = fHeight;	// 高さ

			g_aUI[nCntUI].bUse = true;
			// 頂点情報を設定
			pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight/2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight/2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;

			// 頂点カラーの設定
			pVtx[0].col = g_aUI[nCntUI].col;
			pVtx[1].col = g_aUI[nCntUI].col;
			pVtx[2].col = g_aUI[nCntUI].col;
			pVtx[3].col = g_aUI[nCntUI].col;

			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}
void DeleteUI(UITEX nType)
{
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].nType == nType)
		{
			g_aUI[nCntUI].bUse = false;
		}

	}
}