//=============================================================================
//
// フェード処理 [StageFade.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "StageFade.h"
#include "game.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON (2)
#define VTX_POS_X (0)
#define VTX_POS_Y (0)
#define FADE_TEXTUER " "

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexStageFade(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureStageFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStageFade = NULL;	// 頂点バッファへのポインタ
STAGEFADE				g_StageFade;				// フェード状態
STAGEMODE				g_modeStageNext;			// 次のモード
D3DXCOLOR				g_colorStageFade;			// フェード色

//=============================================================================
// 初期化処理
//=============================================================================
void InitStageFade(STAGEMODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//値の初期化
	g_StageFade = STAGEFADE_IN;
	g_modeStageNext = modeNext;
	g_colorStageFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 頂点情報の設定
	MakeVertexStageFade(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStageFade(void)
{
	// テクスチャの開放
	if (g_pTextureStageFade != NULL)
	{
		g_pTextureStageFade->Release();
		g_pTextureStageFade = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffStageFade != NULL)
	{
		g_pVtxBuffStageFade->Release();
		g_pVtxBuffStageFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStageFade(void)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	PLAYER *pPlayer = GetPlayer();			//プレイヤー情報の取得

	if (g_StageFade != STAGEFADE_NONE)
	{
		if (g_StageFade == STAGEFADE_IN)//フェードイン状態
		{
			g_colorStageFade.a -= 0.03f;
			if (g_colorStageFade.a <= 0.0f)
			{
				g_colorStageFade.a = 0.0f;
				g_StageFade = STAGEFADE_NONE;//何もしてない状態に
				//pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//プレイヤーの位置を移動させる

			}
		}
		else if (g_StageFade == STAGEFADE_OUT)//フェードアウト状態
		{
			g_colorStageFade.a += 0.03f;
			if (g_colorStageFade.a >= 1.0f)
			{
				g_colorStageFade.a = 1.0f;
				g_StageFade = STAGEFADE_IN;//フェードイン状態に

				//モードの設定
				SetStageMode(g_modeStageNext);
				//pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//プレイヤーの位置を移動させる

			}
		}

		//頂点をロックして頂点データへのポインタを取得
		g_pVtxBuffStageFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの更新
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorStageFade.a);//一つ目の頂点の色指定（右回り）
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorStageFade.a);//二つ目の頂点の色指定（右回り）
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorStageFade.a);//三つ目の頂点の色指定（右回り）
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorStageFade.a);//三つ目の頂点の色指定（右回り）
														
		//頂点バッファのアンロック
		g_pVtxBuffStageFade->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStageFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffStageFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureStageFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexStageFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, FADE_TEXTUER, &g_pTextureStageFade);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStageFade,
		NULL);

	//頂点をロックして頂点データへのポインタを取得
	g_pVtxBuffStageFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファの情報を設定
	pVtx[0].pos = D3DXVECTOR3(VTX_POS_X, VTX_POS_Y, 0.0f);//一つ目の頂点の場所指定
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, VTX_POS_Y, 0.0f);//二つ目の頂点の場所指定（右回り）
	pVtx[2].pos = D3DXVECTOR3(VTX_POS_X, SCREEN_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点色
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//二つ目の頂点の色指定（右回り）
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//三つ目の頂点の色指定（右回り）
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//三つ目の頂点の色指定（右回り）
	
	//頂点バッファのアンロック
	g_pVtxBuffStageFade->Unlock();
}

//=============================================================================
// フェードの設定
//=============================================================================
void SetStageFade(STAGEMODE modeNext)
{
	g_StageFade = STAGEFADE_OUT;
	g_modeStageNext = modeNext;
	g_colorStageFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// フェードの取得
//=============================================================================
STAGEFADE GetStageFade(void)
{
	return g_StageFade;
}

