//=============================================================================
//
// タイトル処理 [title.h]
// Author : ABE YUUTAROU
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAMRE "data/TEXTURE/title000.png" //読み込むテクスチャ
#define TITLE_TEXTURENAMRE2 "data/TEXTURE/title002.png" //読み込むテクスチャ
#define TITLE_POS_X  (0)　　　//背景の左上X座標
#define TITLE_POS_Y  (0)　　　//背景の左上Y座標
#define TITLE_WIDTH   (SCREEN_WIDTH)//背景の幅
#define TITLE_HEIGHT   (SCREEN_HEIGHT)//背景の高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTITLE[2] = {};		//テクスチャポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTITLE = NULL;  //頂点バッファのポイント
D3DXVECTOR3 g_posTitle[2];
int g_title;
int g_title2;
TITLESTATE g_TitleMode;
float g_TitlePOSX = 0;
float g_TitlePOSY = 0;

//=============================================================================
//初期化処理タイトル
//=============================================================================
void InitTITLE(void)
{//初期化処理タイトル
	InitBG();
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
													//デバイス取得
	pDevice = GetDevice();

	g_TitlePOSX = 1280;
	g_TitlePOSY = 300;
	g_TitleMode = TITLE_NOMAL;
	g_title = 0;
	g_title2 = 120;
	g_posTitle[1] = D3DXVECTOR3(600, 650, 0.0f);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAMRE, &g_pTextureTITLE[0]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAMRE2, &g_pTextureTITLE[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTITLE,
		NULL);

	VERTEX_2D*pVtx;									//頂点情報のポインタ
													//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffTITLE->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定
	//タイトル
	//ポジション
	pVtx[0].pos = D3DXVECTOR3(g_TitlePOSX - 400.0f, g_TitlePOSY - 250.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_TitlePOSX + 400.0f, g_TitlePOSY - 250.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_TitlePOSX - 400.0f, g_TitlePOSY + 250.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_TitlePOSX + 400.0f, g_TitlePOSY + 250.0f, 0.0f);
	//RHW
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//カラー
	pVtx[0].col = D3DCOLOR_RGBA(100, 100, 100, 255);
	pVtx[1].col = D3DCOLOR_RGBA(100, 100, 100, 255);
	pVtx[2].col = D3DCOLOR_RGBA(100, 100, 100, 255);
	pVtx[3].col = D3DCOLOR_RGBA(100, 100, 100, 255);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファアンロックする
	g_pVtxBuffTITLE->Unlock();

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffTITLE->Lock(0, 0, (void**)&pVtx, 0);
	//プレスエンター
	//ポジション
	pVtx[4].pos = D3DXVECTOR3(g_posTitle[1].x - 100, g_posTitle[1].y - 30, 0.0f);		 //右上
	pVtx[5].pos = D3DXVECTOR3(g_posTitle[1].x + 100, g_posTitle[1].y - 30, 0.0f);	     //右下
	pVtx[6].pos = D3DXVECTOR3(g_posTitle[1].x - 100, g_posTitle[1].y + 30, 0.0f);		 //左上
	pVtx[7].pos = D3DXVECTOR3(g_posTitle[1].x + 100, g_posTitle[1].y + 30, 0.0f);		 //左下
																						 //RHW
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;
	//カラー
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//テクスチャ設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファアンロックする
	g_pVtxBuffTITLE->Unlock();

}
//=============================================================================
//終了処理更新処理ポリゴン
//=============================================================================
void UninitTITLE(void)
{
	//終了処理タイトル
	UnInitBG();
	for (int nCount = 0; nCount < 2; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureTITLE[nCount] != NULL)
		{
			g_pTextureTITLE[nCount]->Release();
			g_pTextureTITLE[nCount] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTITLE != NULL)
	{
		g_pVtxBuffTITLE->Release();
		g_pVtxBuffTITLE = NULL;
	}

}
//=============================================================================
//更新処理ポリゴン
//=============================================================================
void UpdateTITLE(void)
{
	//終了処理タイトル
	UpdateBG();
	FADE pFade;
	pFade = *GetFade();

	if (g_TitlePOSX >= 600)
	{
		g_TitlePOSX -= 1.5f;
	}
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_TitlePOSX = 600;
	}

	if (pFade == FADE_NONE)
	{
		//キーボード設定
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターでセレクト画面へ
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//モード設定
			SetFade(MODE_TUTO
			);
		}

	}
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

							//頂点バッファをロックして、頂点データへのポインタを取得
	g_pVtxBuffTITLE->Lock(0, 0, (void**)&pVtx, 0);

	//頂点バッファのアンロック
	pVtx[0].pos = D3DXVECTOR3(g_TitlePOSX - 385.0f, g_TitlePOSY - 75.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_TitlePOSX + 385.0f, g_TitlePOSY - 75.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_TitlePOSX - 385.0f, g_TitlePOSY + 75.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_TitlePOSX + 385.0f, g_TitlePOSY + 75.0f, 0.0f);

	g_pVtxBuffTITLE->Unlock();
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//エンターで点滅最大
		g_TitleMode = TITLE_END;
	}
	g_title++;
	g_title2++;

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffTITLE->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_TitleMode)
	{
	case TITLE_NOMAL:

		if (GetKeyboardTrigger(DIK_RETURN) == false)
		{
			//タイトルカラー
			pVtx[0].col = D3DCOLOR_RGBA(g_title + 10, g_title + 110, g_title + 150, 255);
			pVtx[1].col = D3DCOLOR_RGBA(g_title + 10, g_title + 120, g_title + 160, 255);
			pVtx[2].col = D3DCOLOR_RGBA(g_title + 10, g_title + 130, g_title + 170, 255);
			pVtx[3].col = D3DCOLOR_RGBA(g_title + 10, g_title + 140, g_title + 180, 255);
		}
		if (GetKeyboardTrigger(DIK_RETURN) == false)
		{
			//プレスエンターカラー
			pVtx[4].col = D3DCOLOR_RGBA(g_title2 * 2, 0, 0, g_title2 * 2);
			pVtx[5].col = D3DCOLOR_RGBA(g_title2 * 2, 0, 0, g_title2 * 2);
			pVtx[6].col = D3DCOLOR_RGBA(g_title2 * 2, 0, 0, g_title2 * 2);
			pVtx[7].col = D3DCOLOR_RGBA(g_title2 * 2, 0, 0, g_title2 * 2);
		}
		break;
	case TITLE_END:
		//タイトルカラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		//プレスエンターカラー
		pVtx[4].col = D3DCOLOR_RGBA((g_title2 * 250) * 5, 0, 0, (g_title2 * 250) * 5);
		pVtx[5].col = D3DCOLOR_RGBA((g_title2 * 250) * 5, 0, 0, (g_title2 * 250) * 5);
		pVtx[6].col = D3DCOLOR_RGBA((g_title2 * 250) * 5, 0, 0, (g_title2 * 250) * 5);
		pVtx[7].col = D3DCOLOR_RGBA((g_title2 * 250) * 5, 0, 0, (g_title2 * 250) * 5);

		break;
	}
	//頂点バッファアンロックする
	g_pVtxBuffTITLE->Unlock();
}

//=============================================================================
//描画処理ポリゴン
//=============================================================================
void DrawTITLE(void)
{
	//終了処理タイトル
	DrawBG();
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイス取得
	pDevice = GetDevice();
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTITLE, 0, sizeof(VERTEX_2D));
	//頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCount = 0; nCount < 2; nCount++)
	{
		//テェクスチャの設定
		pDevice->SetTexture(0, g_pTextureTITLE[nCount]);
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}