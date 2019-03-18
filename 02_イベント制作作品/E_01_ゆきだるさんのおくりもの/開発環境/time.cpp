//=============================================================================
//
// タイム処理 [time.cpp]
// Author : komatsu
//
//=============================================================================
#include "time.h"
#include "game.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TIME_TEXTURENAME	"data/TEXTURE/number001.png"	//読み込むテクスチャファイル名
#define MAX_TIME			(3)								//タイムの桁数

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		// 頂点バッファへのポインタ
TIME g_Time;						// タイム
int g_nTimeCounter;

//=============================================================================
// 初期化処理
//=============================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// タイムの初期化
	g_Time.nTime = 45;
	g_Time.pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Time.col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	g_nTimeCounter = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TIME_TEXTURENAME, &g_pTextureTime);

	// 頂点情報の作成
	MakeVertexTime(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	// テクスチャの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ
	int nCntTime;
	int nTexData;

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nTimeCounter % 60 == 0 && g_Time.nTime >= 0)
	{
		g_Time.nTime--;		//時間経過
	}

	nTexData = g_Time.nTime;

	if (g_Time.nTime >= 0)
	{
		for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2((nTexData % 10) * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((nTexData % 10) * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((nTexData % 10) * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((nTexData % 10) * 0.1f + 0.1f, 1.0f);

			nTexData /= 10;

			if (g_Time.nTime <= 10)
			{
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			pVtx += 4;	//頂点データのポインタを4つ分進める
		}
	}

	g_nTimeCounter++;
	GAMESTATE pGamestate;
	pGamestate = GetGameState();
	if (g_Time.nTime == 0)
	{
		g_Time.nTime = 0;
		pGamestate = GAMESTATE_NORMAL;
		if (pGamestate == GAMESTATE_NORMAL && pGamestate != GAMESTATE_END)
		{
			SetFade(MODE_RANKING);
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	// ポリゴンの描画
	for (nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 + (4 * nCntScore), 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	int nCntTime;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0, g_Time.pos.x = 650; nCntTime < MAX_TIME; nCntTime++, g_Time.pos.x -= 30)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_Time.pos.x, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Time.pos.x + 30.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Time.pos.x, 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Time.pos.x + 30.0f, 70.0f, 0.0f);

		//頂点
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		if (nCntTime == 1)
		{
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.3f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.4f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.3f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);
		}
		else
		{
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// タイムの取得
//=============================================================================
TIME GetTime(void)
{
	return g_Time;
}

//=============================================================================
// タイムの取得
//=============================================================================
void TimeOver(void)
{
	GAMESTATE pGamestate;
	pGamestate = GetGameState();
	if (g_Time.nTime == 0)
	{
		g_Time.nTime = 0;
		pGamestate = GAMESTATE_NORMAL;
		if (pGamestate == GAMESTATE_NORMAL && pGamestate != GAMESTATE_END)
		{
			SetGameState(GAMESTATE_END);
		}
	}
}
