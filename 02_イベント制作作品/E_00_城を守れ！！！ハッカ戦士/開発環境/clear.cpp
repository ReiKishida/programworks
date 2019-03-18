//=============================================================================
//
//	クリア処理 [clear.h]
// Author : ABE YUUTAROU
//
//=============================================================================
#include "main.h"
#include "clear.h"
#include "fade.h"
#include "sound.h"
#include "input.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define CLEAR_TEXTURENAMRE "data/TEXTURE/clear000.png" //読み込むテクスチャ
#define CLEAR_TEXTURENAMRE1 "data/TEXTURE/CLEAR001.png" //読み込むテクスチャ
#define CLEAR_TEXTURENAMRE2 "data/TEXTURE/title002.png" //読み込むテクスチャ
#define CLEAR_POS_X  (0)　　　//背景の左上X座標
#define CLEAR_POS_Y  (0)　　　//背景の左上Y座標
#define CLEAR_WIDTH   (SCREEN_WIDTH)//背景の幅
#define CLEAR_HEIGHT   (SCREEN_HEIGHT)//背景の高さ
#define MAX_CLEAR (3)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCLEAR[MAX_CLEAR] = {};		//テクスチャポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCLEAR = NULL;  //頂点バッファのポイント
D3DXVECTOR3 g_posClear[2];
int g_aClear;

//=============================================================================
//初期化処理ポリゴン
//=============================================================================
void InitCLEAR(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	//デバイス取得
	pDevice = GetDevice();
	g_aClear = 0;
	g_posClear[0] = D3DXVECTOR3(600, 250, 0.0f);
	g_posClear[1] = D3DXVECTOR3(1000, 600, 0.0f);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, CLEAR_TEXTURENAMRE,  &g_pTextureCLEAR[0]);
	D3DXCreateTextureFromFile(pDevice, CLEAR_TEXTURENAMRE1, &g_pTextureCLEAR[1]);
	D3DXCreateTextureFromFile(pDevice, CLEAR_TEXTURENAMRE2, &g_pTextureCLEAR[2]);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4* MAX_CLEAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCLEAR,
		NULL);

	VERTEX_2D*pVtx;									//頂点情報のポインタ
	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffCLEAR->Lock(0, 0, (void**)&pVtx,0);
	
	//頂点座標設定
	//ポジション
	pVtx[0].pos = D3DXVECTOR3(CLEAR_WIDTH, 0, 0.0f);		 //右上
	pVtx[1].pos = D3DXVECTOR3(CLEAR_WIDTH, CLEAR_HEIGHT, 0.0f);//右下
	pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);				 //左上
	pVtx[3].pos = D3DXVECTOR3(0, CLEAR_HEIGHT, 0.0f);		 //左下
	//RHW
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(1.0f,0.0f);				//右上
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);				//右下
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);				//左上
	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);				//左下

	//頂点バッファアンロックする
	g_pVtxBuffCLEAR->Unlock();
	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffCLEAR->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定
	//タイトル
	//ポジション
	pVtx[4].pos = D3DXVECTOR3(g_posClear[0].x - 400, g_posClear[0].y - 100, 0.0f);		 //右上
	pVtx[5].pos = D3DXVECTOR3(g_posClear[0].x + 400, g_posClear[0].y - 100, 0.0f);		 //右下
	pVtx[6].pos = D3DXVECTOR3(g_posClear[0].x - 400, g_posClear[0].y + 100, 0.0f);		 //左上
	pVtx[7].pos = D3DXVECTOR3(g_posClear[0].x + 400, g_posClear[0].y + 100, 0.0f);		 //左下
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
	g_pVtxBuffCLEAR->Unlock();

	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffCLEAR->Lock(0, 0, (void**)&pVtx, 0);
	//プレスエンター
	//ポジション
	pVtx[8].pos = D3DXVECTOR3(g_posClear[1].x - 100, g_posClear[1].y - 30, 0.0f);		 //右上
	pVtx[9].pos = D3DXVECTOR3(g_posClear[1].x + 100, g_posClear[1].y - 30, 0.0f);	     //右下
	pVtx[10].pos = D3DXVECTOR3(g_posClear[1].x - 100, g_posClear[1].y + 30, 0.0f);		 //左上
	pVtx[11].pos = D3DXVECTOR3(g_posClear[1].x + 100, g_posClear[1].y + 30, 0.0f);		 //左下
																						 //RHW
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;
	//カラー
	pVtx[8].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx[9].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx[10].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx[11].col = D3DCOLOR_RGBA(0, 0, 0, 0);
	//テクスチャ設定
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファアンロックする
	g_pVtxBuffCLEAR->Unlock();

	
}
//=============================================================================
//終了処理更新処理ポリゴン
//=============================================================================
void UninitCLEAR(void)
{
	for (int nCount = 0; nCount < MAX_CLEAR; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureCLEAR[nCount] != NULL)
		{
			g_pTextureCLEAR[nCount]->Release();
			g_pTextureCLEAR[nCount] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffCLEAR != NULL)
	{
		g_pVtxBuffCLEAR->Release();
		g_pVtxBuffCLEAR = NULL;
	}
}
//=============================================================================
//更新処理ポリゴン
//=============================================================================
void UpdateCLEAR(void)
{
	FADE pFade;

	pFade = *GetFade();
	VERTEX_2D*pVtx;									//頂点情報のポインタ
	g_aClear++;

	if (pFade == FADE_NONE)
	{
		//キーボード設定
		if (GetKeyboardPress(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//モード設定
			SetFade(MODE_RANKING);
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) == false)
	{
		//頂点バッファをロックし、頂点データへのポインタ
		g_pVtxBuffCLEAR->Lock(0, 0, (void**)&pVtx, 0);
			//プレスエンターカラー
			pVtx[8].col = D3DCOLOR_RGBA(g_aClear * 2, 0, 0, 0);
			pVtx[9].col = D3DCOLOR_RGBA(g_aClear * 2, 0, 0, 0);
			pVtx[10].col = D3DCOLOR_RGBA(g_aClear * 2, 0, 0, 0);
			pVtx[11].col = D3DCOLOR_RGBA(g_aClear * 2, 0, 0, 0);
			//頂点バッファアンロックする
			g_pVtxBuffCLEAR->Unlock();
	}
	

}
//=============================================================================
//描画処理ポリゴン
//=============================================================================
void DrawCLEAR(void)
{
	
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	//デバイス取得
	pDevice = GetDevice();
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCLEAR, 0, sizeof(VERTEX_2D));
	//頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCount = 0; nCount < MAX_CLEAR; nCount++)
	{
		//テェクスチャの設定
		pDevice->SetTexture(0, g_pTextureCLEAR[nCount]);
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}

