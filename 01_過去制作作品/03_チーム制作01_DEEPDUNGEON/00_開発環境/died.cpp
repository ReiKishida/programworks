//===========================================================================
//
// 死亡時処理 [Died.cpp]
// Author : TEAM_Fascinator
//
//===========================================================================
#include "main.h"
#include "died.h"
#include "input.h"
#include "inputx.h"
#include "fade.h"
#include "sound.h"

//===========================================================================
// マクロ定義
//===========================================================================
#define DIED_TEXTURE_NAME		"data/TEXTURE/die.png"	// テクスチャ
#define DIED_SIZE_X				(320.0f)						// 左上X座標
#define DIED_SIZE_Y				(140.0f)						// 左上Y座標
#define DIED_WIDTH				(SCREEN_WIDTH)				// 幅
#define DIED_HEIGHT			(SCREEN_HEIGHT)				// 高さ
#define DIED_TEXTURE_UV_U		(1.0f)						// アニメーションU範囲
#define DIED_TEXTURE_UV_V		(1.0f)						// アニメーションV範囲
#define DIED_SPEED				(0.003f)					// スピード

//===========================================================================
// グローバル変数
//===========================================================================
LPDIRECT3DTEXTURE9 g_pTextureDied = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDied = NULL;		// 頂点バッファへのポインタ
JoyState	g_JoyStateDied;							// ジョイパッドの状態
DIED g_Died[2];
//===========================================================================
// 初期化処理
//===========================================================================
void InitDied(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Died[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 -100.0f, 0.0f);	// 中心値
	g_Died[0].Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	g_Died[0].type = DIEDTYPE_APPER;
	g_Died[0].bUse = false;


	g_Died[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 0.0f);	// 中心値
	g_Died[1].Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_Died[1].type = DIEDTYPE_APPER;
	g_Died[1].bUse = false;

	g_JoyStateDied.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, DIED_TEXTURE_NAME, &g_pTextureDied);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDied,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	g_pVtxBuffDied->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(-DIED_SIZE_X, -DIED_SIZE_Y, 0.0f) + g_Died[0].pos;
	pVtx[1].pos = D3DXVECTOR3(DIED_SIZE_X, -DIED_SIZE_Y, 0.0f) + g_Died[0].pos;
	pVtx[2].pos = D3DXVECTOR3(-DIED_SIZE_X, DIED_SIZE_Y, 0.0f) + g_Died[0].pos;
	pVtx[3].pos = D3DXVECTOR3(DIED_SIZE_X, DIED_SIZE_Y, 0.0f) + g_Died[0].pos;

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = g_Died[0].Col;
	pVtx[1].col = g_Died[0].Col;
	pVtx[2].col = g_Died[0].Col;
	pVtx[3].col = g_Died[0].Col;

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(DIED_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, DIED_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(DIED_TEXTURE_UV_U, DIED_TEXTURE_UV_V);

	pVtx += 4;
	
	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(-SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, 0.0f) + g_Died[1].pos;
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f) + g_Died[1].pos;
	pVtx[2].pos = D3DXVECTOR3(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f) + g_Died[1].pos;
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f) + g_Died[1].pos;

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = g_Died[1].Col;
	pVtx[1].col = g_Died[1].Col;
	pVtx[2].col = g_Died[1].Col;
	pVtx[3].col = g_Died[1].Col;

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(DIED_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, DIED_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(DIED_TEXTURE_UV_U, DIED_TEXTURE_UV_V);
	// 頂点バッファをアンロック
	g_pVtxBuffDied->Unlock();
}

//===========================================================================
// 終了処理
//===========================================================================
void UninitDied(void)
{
	// テクスチャ破棄
	if (g_pTextureDied != NULL)
	{
		g_pTextureDied->Release();
		g_pTextureDied = NULL;
	}
	// 頂点バッファ破棄
	if (g_pVtxBuffDied != NULL)
	{
		g_pVtxBuffDied->Release();
		g_pVtxBuffDied = NULL;
	}
}

//===========================================================================
// 更新処理
//===========================================================================
void UpdateDied(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	for (int nCntDied = 0; nCntDied < 2; nCntDied++)
	{
		if (g_Died[nCntDied].bUse == true)
		{
			if (g_Died[0].Col.a == 0.01f)
			{
				PlaySound(SOUND_LABEL_SE_DEAD);
			}

			switch (g_Died[nCntDied].type)
			{
			case DIEDTYPE_APPER:
				g_Died[0].Col.a += 0.01f;
				if (g_Died[0].Col.a >= 1.0f)
				{
					g_Died[0].Col.a = 1.0f;
					g_Died[nCntDied].type = DIEDTYPE_NORMAL;
				}

				g_Died[1].Col.a += 0.01f;
				if (g_Died[1].Col.a >= 0.4f)
				{
					g_Died[1].Col.a = 0.4f;

				}
				break;
			case DIEDTYPE_NORMAL:

				break;
			}
		}
	}
	// 頂点バッファをロック
	g_pVtxBuffDied->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntDied = 0; nCntDied < 2; nCntDied++)
	{
		// 頂点カラー設定
		pVtx[0].col = g_Died[nCntDied].Col;
		pVtx[1].col = g_Died[nCntDied].Col;
		pVtx[2].col = g_Died[nCntDied].Col;
		pVtx[3].col = g_Died[nCntDied].Col;

		pVtx += 4;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffDied->Unlock();

}

//===========================================================================
// 描画処理
//===========================================================================
void DrawDied(void)
{
	for (int nCntDied = 0; nCntDied < 2; nCntDied++)
	{

		if (g_Died[nCntDied].bUse == true)
		{

			LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

														// 頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffDied, 0, sizeof(VERTEX_2D));

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャ設定
			pDevice->SetTexture(0, 0);

			// 描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureDied);

			// 描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

DIED *GetDied(void)
{
	return &g_Died[0];
}