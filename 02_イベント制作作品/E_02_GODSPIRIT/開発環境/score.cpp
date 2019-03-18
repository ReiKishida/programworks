////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// スコア処理 [score.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "score.h"
#include "effect.h"
#include "ranking.h"
#include "result.h"
#include "input.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ブロックの中心座標
	D3DXCOLOR  col;			// 移動量
	SCORETEX nType;			// ブロックの種類
	float fWidth;				// ブロックの幅
	float fHeight;				// ブロックの高さ
	int nNumCnt;				// 文字数
} ScoreInfo;
typedef struct
{
	char *pFileName;
}ScoreTexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_SCORE	(30)
#define MAX_LEN_SCORE	(8)// スコアの桁数

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore[SCORETEX_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ
Score					g_aScore[MAX_SCORE];
int						g_nScore;					// スコア
int						g_nScoreDest;					// スコア
ScoreInfo g_aScoreInfo[] =
{
	// スコア背景
	{ D3DXVECTOR3(250,45.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.6f),SCORETEX_SCORE_BG,480,90 ,1 },
	// SCORE
	//{ D3DXVECTOR3(410.0f,40.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),SCORETEX_SCORE_NAME,70,70,1 },
	// スコア数字
	{ D3DXVECTOR3(380,40.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),SCORETEX_SCORE,40,60,MAX_LEN_SCORE },
};

// テクスチャ名
ScoreTexture  g_aScoreTex[SCORETEX_MAX] =
{
	{ "data/TEXTURE/scoreBG001.png" },	// スコア背景
	{ "data/TEXTURE/score001.png" },	// スコア名前
	{ "data/TEXTURE/number000.png" },	// スコア数字	
};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < SCORETEX_MAX; nTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			g_aScoreTex[nTex].pFileName,		// ファイルの名前
			&g_pTextureScore[nTex]);	// テクスチャへのポインタ
	}
	// 初期化
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aScore[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心
		g_aScore[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
		g_aScore[nCnt].nType = SCORETEX_MAX;						// 種類
		g_aScore[nCnt].fWidth = 0;									// 幅
		g_aScore[nCnt].fHeight = 0;									// 高さ
		g_aScore[nCnt].bUse = false;								// 使用されているかどうか
	}
	g_nScore = 0;	// スコア
	g_nScoreDest = 0;
	// 頂点情報の作成
	MakeVertexScore(pDevice);
	for (int nCnt = 0; nCnt < sizeof g_aScoreInfo / sizeof(ScoreInfo); nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < g_aScoreInfo[nCnt].nNumCnt; nCnt2++)
		{
			SetScore(g_aScoreInfo[nCnt].pos - D3DXVECTOR3((g_aScoreInfo[nCnt].fWidth)*nCnt2, 0.0f, 0.0f), g_aScoreInfo[nCnt].col, g_aScoreInfo[nCnt].nType, g_aScoreInfo[nCnt].fWidth, g_aScoreInfo[nCnt].fHeight);
		}
	}
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitScore(void)
{
	// テクスチャの開放
	for (int nTex = 0; nTex < SCORETEX_MAX; nTex++)
	{
		if (g_pTextureScore[nTex] != NULL)
		{
			g_pTextureScore[nTex]->Release();
			g_pTextureScore[nTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
	GetScore(g_nScore);
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateScore(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	if (g_nScoreDest <= g_nScore)
	{
		g_nScoreDest += rand() % 100;
	}
	if (g_nScoreDest >= g_nScore)
	{
		g_nScoreDest = g_nScore;
	}
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPow = 0;
	// テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++, pVtx += 4)
	{
		if (g_aScore[nCnt].nType == SCORETEX_SCORE)
		{// スコアのみ加算でずれた分で累乗を正しくする
			int nData = int(powf(10, float(nCntPow)));

			pVtx[0].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData) * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nScoreDest % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
			nCntPow++;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nTex = 0; nTex < MAX_SCORE; nTex++)
	{
		if (g_aScore[nTex].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore[g_aScore[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
						// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++, pVtx += 4)
	{
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}
//==============================================================================================================================
// スコアの設定
//==============================================================================================================================
void SetScore(D3DXVECTOR3 pos, D3DXCOLOR col, SCORETEX nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
						// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++, pVtx += 4)
	{
		if (g_aScore[nCnt].bUse == false)
		{
			g_aScore[nCnt].pos = pos;			// 中心
			g_aScore[nCnt].col = col;			// 色
			g_aScore[nCnt].nType = nType;		// 種類
			g_aScore[nCnt].fWidth = fWidth;		// 幅
			g_aScore[nCnt].fHeight = fHeight;	// 高さ
			g_aScore[nCnt].bUse = true;

			// 頂点情報を設定
			pVtx[0].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, -g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aScore[nCnt].fWidth / 2, g_aScore[nCnt].fHeight / 2, 0.0f) + g_aScore[nCnt].pos;
			if (g_aScore[nCnt].nType == SCORETEX_SCORE)
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
			}
			// 頂点カラーの設定
			pVtx[0].col = g_aScore[nCnt].col;
			pVtx[1].col = g_aScore[nCnt].col;
			pVtx[2].col = g_aScore[nCnt].col;
			pVtx[3].col = g_aScore[nCnt].col;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==============================================================================================================================
// スコアの加算
//==============================================================================================================================
int AddScore(int nValue)
{
	g_nScore += nValue;
	return g_nScore;
}
