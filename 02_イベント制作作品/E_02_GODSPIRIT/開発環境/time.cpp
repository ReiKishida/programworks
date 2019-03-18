////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// タイム処理 [time.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "time.h"
#include "result.h"
#include "fade.h"
#include "game.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define TIME_POS_X	(45)	// タイムの横幅
#define TIME_POS_Y	(70)	// タイムの縦幅
#define TIME_PLACE	(D3DXVECTOR3(SCREEN_WIDTH/2-50,50,0.0f))// タイムの場所
#define GAME_TIME	(0)	// タイム
#define TIME_REN	(3)	// タイムの桁数
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		// 頂点バッファへのポインタ
int						g_nTime;					// タイム
int						g_nFrameCount ;				// 秒数減算
int						g_nFrameTimeCount;			// 秒数減算（終了判定用）
bool					g_bUse[TIME_REN];						// 使用しているかどうか
bool					g_bStop;						// タイムが止まっているかどうか
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// スコアの初期化
	g_nTime = GAME_TIME;
	g_nFrameCount = 0;
	g_nFrameTimeCount = GAME_TIME;
	g_bStop = false;
	g_bUse[0] = true;
	g_bUse[1] = true;
	g_bUse[2] = true;
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_SCORE,		// ファイルの名前
								&g_pTextureTime);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexTime(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
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
	GetTime(g_nTime);
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateTime(void)
{	
	if (g_bStop == false)
	{
		g_nFrameCount++;

		if (g_nFrameCount % 60 == 0)
		{		
			AddTime(1);			
		}
	}

	if (g_nTime >= 0)
	{
		g_bUse[0] = true;
		g_bUse[1] = false;
		g_bUse[2] = false;

	}
	if (g_nTime >= 10)
	{
		g_bUse[0] = true;
		g_bUse[1] = true;
		g_bUse[2] = false;

	}
	if (g_nTime >= 100)
	{
		g_bUse[0] = true;
		g_bUse[1] = true;
		g_bUse[2] = true;
	}

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < TIME_REN; nCnt++)
	{
		if (g_bUse[nCnt] == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCnt,
				2);
		}
	}
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_REN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TIME_REN; nCnt++, pVtx += 4)
	{

		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(float(TIME_POS_X * -nCnt - (TIME_POS_X / 2) + TIME_POS_X) + 10, -(TIME_POS_Y / 2), 0.0f) + TIME_PLACE;
		pVtx[1].pos = D3DXVECTOR3(float(TIME_POS_X * -nCnt + (TIME_POS_X / 2) + TIME_POS_X) + 10, -(TIME_POS_Y / 2), 0.0f) + TIME_PLACE;
		pVtx[2].pos = D3DXVECTOR3(float(TIME_POS_X * -nCnt - (TIME_POS_X / 2) + TIME_POS_X) + 10, (TIME_POS_Y / 2), 0.0f) + TIME_PLACE;
		pVtx[3].pos = D3DXVECTOR3(float(TIME_POS_X * -nCnt + (TIME_POS_X / 2) + TIME_POS_X) + 10, (TIME_POS_Y / 2), 0.0f) + TIME_PLACE;

		int nData = int(powf(10,float(nCnt)));

		pVtx[0].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData) * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData)* 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);

		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//==============================================================================================================================
// タイムの加算
//==============================================================================================================================
void AddTime(int nTime)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	g_nTime += nTime;

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	for (int nCnt = 0; nCnt < TIME_REN; nCnt++, pVtx += 4)
	{
		int nData = 1;

		for (int nCnt2 = nCnt; nCnt2 != 0; nCnt2--)
		{
			nData *= 10;
		}

		pVtx[0].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData) * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData)* 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((g_nTime % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//==============================================================================================================================
// タイムの停止
//==============================================================================================================================
void StopTime(void)
{
	g_bStop = true;
}