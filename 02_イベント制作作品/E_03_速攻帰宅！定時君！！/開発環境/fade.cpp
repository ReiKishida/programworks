////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// フェード処理 [fade.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "fade.h"
#include "main.h"

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define FADE_TEX "data/TEXTURE/black.png"				// 読み込むテクスチャ
#define FADE_TEX1 "data/TEXTURE/black.png"		// 読み込むテクスチャ
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFade[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ
FADE					g_fade;					// フェード状態
MODE					g_modeNext;				// 次のモード
D3DXCOLOR				g_colorFade;			// フェード色
int						g_nCounterAnim;
int						g_nPatternAnim;
bool					g_bChangeMode;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_fade = FADE_IN;									// フェードイン状態にする
	g_modeNext = modeNext;								// モード設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒い画面（透明）にしておく
	g_nCounterAnim = 0;
	g_nPatternAnim = 0;
	g_bChangeMode = false;
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
						FADE_TEX,
						&g_pTextureFade[0]);
	D3DXCreateTextureFromFile(pDevice,
						FADE_TEX1,
						&g_pTextureFade[1]);

	// 頂点情報の設定
	MakeVertexFade(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitFade(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// テクスチャの破棄
		if (g_pTextureFade[nCnt] != NULL)
		{
			g_pTextureFade[nCnt]->Release();
			g_pTextureFade[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateFade(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)			// フェードイン状態に
		{
			g_colorFade.a -= 0.02f;	// 画面を透明にしていく
			if (g_colorFade.a <= 0.0f)
			{// 透明になったら
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade == FADE_OUT)	// フェードアウト状態に
		{
			g_colorFade.a += 0.02f;	// 画面を不透明にしていく
			if (g_colorFade.a >= 1.0f)
			{// 不透明になったら
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;	// フェードイン状態に
				if (g_bChangeMode == true)
				{
					// モードの設定
					SetMode(g_modeNext);
				}
			}
		}
	}

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	pVtx[4].col = g_colorFade;
	pVtx[5].col = g_colorFade;
	pVtx[6].col = g_colorFade;
	pVtx[7].col = g_colorFade;
	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//==============================================================================================================================
// タイトル画面
//==============================================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade[0]);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_pTextureFade[1]);
	//// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	// rhwの設定
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
	}
	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(-(70*1.6f / 2) + (SCREEN_WIDTH/2) - 50, -(140 / 2)+ (630), 0.0f);
	pVtx[5].pos = D3DXVECTOR3((70 * 1.6f / 2) +( SCREEN_WIDTH / 2) - 50, -(140 / 2) +(630), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-(70 * 1.6f / 2) + (SCREEN_WIDTH / 2) - 50, (140 / 2) + (630), 0.0f);
	pVtx[7].pos = D3DXVECTOR3((70 * 1.6f / 2) + (SCREEN_WIDTH / 2) - 50, (140 / 2) + (630), 0.0f);
	// rhwの設定
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt+4].rhw = 1.0f;
	}
	// 頂点カラーの設定
	pVtx[4].col = g_colorFade;
	pVtx[5].col = g_colorFade;
	pVtx[6].col = g_colorFade;
	pVtx[7].col = g_colorFade;
	// テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

}

//==============================================================================================================================
// フェードの設定
//==============================================================================================================================
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;									// フェードアウト状態にする
	g_modeNext = modeNext;								// モード設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒い画面（不透明）にしておく
	g_bChangeMode = true;								// モード変える
}

//==============================================================================================================================
// フェードの設定
//==============================================================================================================================
void SetFadeNone(void)
{
	g_fade = FADE_OUT;									// フェードアウト状態にする
	g_bChangeMode = false;								// モード変えない
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒い画面（不透明）にしておく
}

//==============================================================================================================================
// フェードの取得
//==============================================================================================================================
FADE GetFade(void)
{
	return g_fade;
}

