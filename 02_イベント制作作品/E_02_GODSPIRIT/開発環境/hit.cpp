////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ヒット処理 [hit.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "hit.h"
#include "effect.h"
#include "input.h"
#include "result.h"
#include "score.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ブロックの中心座標
	D3DXCOLOR  col;				// 色
	HITTEX nType;				// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	int nNumCnt;				// 文字数
} HitInfo;
typedef struct
{
	char *pFileName;
}HitTexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_HIT	(5)
#define MAX_LEN_HIT	(3)// スコアの桁数
#define HIT_TIME	(300) // ヒットの継続時間
#define GET_HITMOVE (30.0f)	// ヒット挿入速度
#define HIT_STOPPOS	(500.0f) // ヒット停止位置
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexHit(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureHit[HITTEX_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHit = NULL;		// 頂点バッファへのポインタ
Hit						g_aHit[MAX_HIT];
HitInfo g_aHitInfo[] =
{
	// ヒット背景
	{ D3DXVECTOR3(150.0f,410.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_BG,510,100 ,1 },
	// ヒット文
	{ D3DXVECTOR3(320.0f,360.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HITTEXT,140,70 ,1 },
	// ヒット数
	{ D3DXVECTOR3(200.0f,360.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HIT,50,70,1 },
	{ D3DXVECTOR3(150.0f,363.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HIT,70,90,1 },
	{ D3DXVECTOR3(90.0f,366.0f,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),HITTEX_HIT,90,110,1 },
};
// テクスチャ名
HitTexture  g_aHitTex[HITTEX_MAX] =
{
	{ "data/TEXTURE/hitBG000.png" },			// ヒット背景
	{ "data/TEXTURE/hit.png" },				// ヒット
	{ "data/TEXTURE/HitNum001.png" },		// 数字
};
int						g_nHit;					// スコア
int						g_nHitMax;				// 最大ヒット数
int						g_nHitLength;			// ヒット数の文字数
int						g_nHitTime;				// ヒットの時間
D3DXCOLOR				g_HitColStart;			// 色上
D3DXCOLOR				g_HitColEnd;			// 色下
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitHit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < HITTEX_MAX; nTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			g_aHitTex[nTex].pFileName,		// ファイルの名前
			&g_pTextureHit[nTex]);	// テクスチャへのポインタ
	}
	// 初期化
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++)
	{
		g_aHit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心
		g_aHit[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
		g_aHit[nCnt].nType = HITTEX_MAX;						// 種類
		g_aHit[nCnt].fWidth = 0;									// 幅
		g_aHit[nCnt].fHeight = 0;									// 高さ
		g_aHit[nCnt].bUse = false;								// 使用されているかどうか
	}
	g_nHit = 0;	// ヒット
	g_nHitMax = 0; // ヒット数
	g_nHitLength = 1;
	g_nHitTime = 0;
	g_HitColStart = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_HitColEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// 頂点情報の作成
	MakeVertexHit(pDevice);
	for (int nCnt = 0; nCnt < sizeof g_aHitInfo / sizeof(HitInfo); nCnt++)
	{
		SetHit(g_aHitInfo[nCnt].pos , g_aHitInfo[nCnt].col, g_aHitInfo[nCnt].nType, g_aHitInfo[nCnt].fWidth, g_aHitInfo[nCnt].fHeight );
	}
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++)
	{
		g_aHit[nCnt].bUse = false;
	}
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitHit(void)
{
	// テクスチャの開放
	for (int nTex = 0; nTex < HITTEX_MAX; nTex++)
	{
		if (g_pTextureHit[nTex] != NULL)
		{
			g_pTextureHit[nTex]->Release();
			g_pTextureHit[nTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffHit != NULL)
	{
		g_pVtxBuffHit->Release();
		g_pVtxBuffHit = NULL;
	}
	GetHit(g_nHitMax);

}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateHit(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	int nCntPow = 0;

//-------------------------------------/ ヒット数に応じたもの /----------------------------------------------------------------//
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		if (g_nHitTime >= HIT_TIME)
		{
			g_nHit = 0;
			g_nHitTime = 0;
		}
		if (g_nHit < 1000)
		{// 3桁
			g_HitColStart = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			g_HitColEnd = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			g_nHitLength = 3;
			if (g_nHit < 100)
			{// 2桁
				g_HitColStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				g_HitColEnd = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				g_nHitLength = 2;
				if (g_nHit < 10)
				{// 1桁
					g_HitColStart = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					g_HitColEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					g_nHitLength = 1;
					if (g_nHit <= 0)
					{
						g_nHitLength = 0;
					}
				}
			}
		}
		if (g_nHitTime >= HIT_TIME - 50)
		{// ヒット数リセットするため画面外
			g_aHit[nCnt].pos.x -= GET_HITMOVE;
			if (g_aHit[nCnt].pos.x <= g_aHitInfo[nCnt].pos.x - HIT_STOPPOS)
			{
				g_aHit[nCnt].pos.x = g_aHitInfo[nCnt].pos.x - HIT_STOPPOS;
			}
		}
		else if (g_nHit > 0&&g_nHitTime <= 50)
		{// ヒットした
			g_aHit[nCnt].pos.x += GET_HITMOVE;
			if (g_aHit[nCnt].pos.x >= g_aHitInfo[nCnt].pos.x)
			{
				g_aHit[nCnt].pos.x = g_aHitInfo[nCnt].pos.x;
			}
		}
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHit->Unlock();
//-------------------------------------/ ヒット数増加 /----------------------------------------------------------------//
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		if (g_aHit[nCnt].nType == HITTEX_HIT)
		{// 加算でずれた分で累乗を正しくする
			g_aHit[nCnt].bUse = true;
			int nData = int(powf(10, float(nCntPow)));
			pVtx[0].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData) * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nHit % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
			// 頂点カラーの設定
			pVtx[0].col = g_HitColStart;
			pVtx[1].col = g_HitColStart;
			pVtx[2].col = g_HitColEnd;
			pVtx[3].col = g_HitColEnd;
			nCntPow++;
			if (nCntPow == g_nHitLength)
			{// 桁数が無駄にある場合取り除く処理
				for (int nFalse = nCntPow; nFalse < MAX_LEN_HIT; nFalse++)
				{// 最大桁数分　- 現在の桁数消す
					g_aHit[nCnt + nFalse].bUse = false;
				}
				break;
			}
			else if (g_nHitLength == 0)
			{
				g_aHit[nCnt].bUse = false;
			}
		}
	}
	g_nHitTime++;

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawHit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHit, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nTex = 0; nTex < MAX_HIT; nTex++)
	{
		if (g_aHit[nTex].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureHit[g_aHit[nTex].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nTex,
				2);
		}
	}

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexHit(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHit,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;		
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// 頂点カラーの設定
		pVtx[0].col = g_HitColStart;
		pVtx[1].col = g_HitColStart;
		pVtx[2].col = g_HitColEnd;
		pVtx[3].col = g_HitColEnd;
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHit->Unlock();
}
//==============================================================================================================================
// スコアの設定
//==============================================================================================================================
void SetHit(D3DXVECTOR3 pos, D3DXCOLOR col, HITTEX nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffHit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++, pVtx += 4)
	{
		if (g_aHit[nCnt].bUse == false)
		{
			g_aHit[nCnt].pos = pos;			// 中心
			g_aHit[nCnt].pos.x -= HIT_STOPPOS;	// 初期位置を画面外
			g_aHit[nCnt].col = col;			// 色
			g_aHit[nCnt].nType = nType;		// 種類
			g_aHit[nCnt].fWidth = fWidth;		// 幅
			g_aHit[nCnt].fHeight = fHeight;	// 高さ
			g_aHit[nCnt].bUse = true;

			// 頂点情報を設定
			pVtx[0].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, -g_aHit[nCnt].fHeight, 0.0f) + g_aHit[nCnt].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aHit[nCnt].fWidth / 2,0.0f, 0.0f) + g_aHit[nCnt].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aHit[nCnt].fWidth / 2, 0.0f, 0.0f) + g_aHit[nCnt].pos;
			if (g_aHit[nCnt].nType == HITTEX_HIT)
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
			}
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHit->Unlock();
}

//==============================================================================================================================
// スコアの加算
//==============================================================================================================================
void AddHit(int nHit)
{
	g_nHit += nHit;
	AddScore(g_nHit);
	g_nHitTime = 0;
	for (int nCnt = 0; nCnt < MAX_HIT; nCnt++)
	{
		if (g_aHit[nCnt].nType != HITTEX_HIT)
		{
			g_aHit[nCnt].bUse = true;
		}
	}
	if (g_nHitMax <= g_nHit)
	{// 最大ヒット数
		g_nHitMax = g_nHit;
	}
}