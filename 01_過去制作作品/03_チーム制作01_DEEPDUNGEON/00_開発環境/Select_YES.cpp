//=============================================================================
//
// セレクト画面Yes処理 [Select_Yes.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "Select_YES.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PAUSECONRINUE_TEXTUER	"data/TEXTURE/yes.png"		//読み込むテクスチャ
#define PAUSE_POS_X				(320)						//ポーズ文字（YES）左上X
#define PAUSE_POS_Y				(350)						//ポーズ文字（YES）左上Y
#define PAUSE_WIDTH				(540)						//ポーズ文字（YES）の幅
#define PAUSE_HEIGHT			(500)						//ポーズ文字（YES）高さ

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureSelectYes = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffSelectYes = NULL;
YESSTATE					g_YesState;

//=============================================================================
// 初期化処理[InitPolygon]
//=============================================================================
void InitSelectYes(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D*pVtx;//頂点情報へのポインタ
	
	g_YesState = YESSTATE_SELECT;

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, PAUSECONRINUE_TEXTUER, &g_pTextureSelectYes);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectYes, 
		NULL);

	//頂点をロックして頂点データへのポインタを取得
	g_pVtxBuffSelectYes->Lock(0, 0, (void**)&pVtx, 0);

	//背景の座標設定
	//Yes座標設定
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y , 0.0f);//一つ目の頂点の場所指定
	pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y , 0.0f);//二つ目の頂点の場所指定（右回り）
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）
	pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);//三つ目の頂点の場所指定（右回り）
	
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//背景色設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）

	//背景テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	g_pVtxBuffSelectYes->Unlock();
}

//=============================================================================
//終了処理 [UninitPolygon]
//=============================================================================
void UninitSelectYes(void)
{
	//テクスチャの破棄
	if (g_pTextureSelectYes != NULL)
	{
		g_pTextureSelectYes->Release();
		g_pTextureSelectYes = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSelectYes != NULL)
	{
		g_pVtxBuffSelectYes->Release();
		g_pVtxBuffSelectYes = NULL;
	}
}

//=============================================================================
// 更新処理[UpdataPolygon]
//=============================================================================
void UpdateSelectYes(void)
{
	VERTEX_2D*pVtx;//頂点情報へのポインタ

	//頂点をロックして頂点データへのポインタを取得
	g_pVtxBuffSelectYes->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_YesState)
	{
	case YESSTATE_SELECT:
		//背景色設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//一つ目の頂点の色指定（右回り）
		break;
	case YESSTATE_NOSELECT:
		//背景色設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//一つ目の頂点の色指定（右回り）
		break;
	}

	//頂点バッファのアンロック
	g_pVtxBuffSelectYes->Unlock();
}

//=============================================================================
// 描写処理[DrawPolygon]
//=============================================================================
void DrawSelectYes(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスを取得する

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectYes, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSelectYes);

	//の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

//=============================================================================
// QUIT設定処理[DrawPolygon]
//=============================================================================
void SetSelectYes(YESSTATE state)
{
	g_YesState = state;
}
