//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "fade.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON (2)
#define MAX_PATTERN (16)
#define BG_WIDTH		(SCREEN_WIDTH)				//背景の幅
#define BG_HEIGHT		(SCREEN_HEIGHT)				//背景の高さ
#define BG_POS_X		(0)							//背景左上X座標
#define BG_POS_Y		(0)							//背景左上Y座標
#define POS_X (0.125f * (g_nPatternAnim % 5))
#define POS_Y (0.5f * (g_nPatternAnim / 5))

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ

FADE					g_fade;					// フェード状態
MODE					g_modeNext;				// 次のモード
D3DXCOLOR				g_colorFade;			// フェード色
D3DXCOLOR				g_colorFade2;			// フェード色
D3DXVECTOR3           g_posfade;

int g_nCounterAnim;		//アニメーションカウンター
int g_nPatternAnim;		//アニメーションパターンNo.

//=============================================================================
// 初期化処理
//=============================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/load.png", &g_pTextureFade);


	// 値の初期化
	g_fade = FADE_IN;
	
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//黒い画面にしておく
	g_colorFade2 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒い画面にしておく
	g_posfade = D3DXVECTOR3(SCREEN_WIDTH / 2 + 350.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f);
	// 頂点情報の設定
	MakeVertexFade(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	//テクスチャの破棄
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{

			g_colorFade.a -= 0.02f;	//画面を透明にしていく
			g_colorFade2.a -= 0.02f;	//画面を透明にしていく
			if (g_colorFade.a <= 0.00f)
			{
				g_colorFade.a = 0.0f;
				g_colorFade2.a = 0.0f;

				g_fade = FADE_NONE;	//何もしていない状態
			}
		}

		else if (g_fade == FADE_OUT)
		{
			g_colorFade.a += 0.02f;	//画面を不透明に
			g_colorFade2.a += 0.02f;	//画面を不透明に
			
			
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_colorFade2.a = 1.0f;
				g_fade = FADE_IN;	//何もしていない状態
				//モードの設定
				SetMode(g_modeNext);
			}
		}

		

				
				
			
			
			
			

			if ((g_nCounterAnim % 40) == 0)
			{
				g_nPatternAnim = (g_nPatternAnim + 1) % MAX_PATTERN;
			}
			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
			//頂点カラー
			pVtx[0].col = g_colorFade;
			pVtx[1].col = g_colorFade;
			pVtx[2].col = g_colorFade;
			pVtx[3].col = g_colorFade;

			pVtx[4].col = g_colorFade2;
			pVtx[5].col = g_colorFade2;
			pVtx[6].col = g_colorFade2;
			pVtx[7].col = g_colorFade2;
			//テクスチャ座標
			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2(POS_X, POS_Y);
			pVtx[1].tex = D3DXVECTOR2(POS_X + 0.125f, POS_Y);
			pVtx[2].tex = D3DXVECTOR2(POS_X, POS_Y + 0.5f);
			pVtx[3].tex = D3DXVECTOR2(POS_X + 0.125f, POS_Y + 0.5f);
			//頂点バッファをアンロック
			g_pVtxBuffFade->Unlock();

		
	}

}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, 0);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	// 頂点バッファの情報を設定
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(-250.0f, -70.0f, 0.0f) + g_posfade;
	pVtx[1].pos = D3DXVECTOR3(250.0f, -70.0f, 0.0f) + g_posfade;
	pVtx[2].pos = D3DXVECTOR3(-250.0f, 70.0f, 0.0f) + g_posfade;
	pVtx[3].pos = D3DXVECTOR3(250.0f, 70.0f, 0.0f) + g_posfade;

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);


	//頂点の座標
	pVtx[4].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラー
	pVtx[4].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// フェードの設定
//=============================================================================
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	//黒い画面にしておく
	g_colorFade2 = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒い画面にしておく


}

//=============================================================================
// フェードの取得
//=============================================================================
FADE *GetFade(void)
{
	return &g_fade;
}

