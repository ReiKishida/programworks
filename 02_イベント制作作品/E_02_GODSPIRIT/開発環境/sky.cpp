////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 空（球体）処理 [sky.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sky.h"
#include "camera.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_SKY	(1)
#define BLOCK_X	(50)
#define BLOCK_Y	(30)
#define VERTEX_WIDTH	(2+2*BLOCK_X)
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 中心座標
	D3DXVECTOR3 rot;			// 向き
	SKYTYPE nType;		// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	float fDepth;				// 奥行き
	float fLength;				// 半径
} SkyInfo;
typedef struct
{
	char *pFileName;
}SkyTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexSky(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSky = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_apTextureSky[SKYTYPE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSky = NULL;						// インデックスバッファへのポインタ
Sky g_aSky[(BLOCK_X + 1) * (BLOCK_Y + 1)];

int g_nNumVertexSky;	// 頂点数
int g_nNumIndexSky;	// インデックス数
int g_nNumPolygonSky;	// ポリゴン数
float g_fTexmove;
SkyInfo g_aSkyInfo[] =
{
	{ D3DXVECTOR3(0.0f, -300.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SKYTYPE_0, 200.0f,200.0f, 0.0f,20000.0f },
};
SkyTexture g_aSkyTexInfo[SKYTYPE_MAX] =
{
	{ "data/TEXTURE/sky001.jpg" },
};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	
	for (int nCntSkyTex = 0; nCntSkyTex < SKYTYPE_MAX; nCntSkyTex++)
	{// テクスチャの設定
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_aSkyTexInfo[nCntSkyTex].pFileName,	// ファイルの名前
			&g_apTextureSky[nCntSkyTex]);			// テクスチャへのポインタ
	}
	g_nNumVertexSky = (BLOCK_X + 1) * (BLOCK_Y + 1);					// 頂点数
	g_nNumIndexSky = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1) + 2;	// インデックス数
	g_nNumPolygonSky = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1);	// ポリゴン数

	for (int nCntSky = 0; nCntSky < g_nNumVertexSky; nCntSky++)
	{
		g_aSky[nCntSky].pos = g_aSkyInfo[nCntSky].pos;			// 位置
		g_aSky[nCntSky].rot = g_aSkyInfo[nCntSky].rot;			// 向き
		g_aSky[nCntSky].fWidth = g_aSkyInfo[nCntSky].fWidth;	// 幅
		g_aSky[nCntSky].fHeight = g_aSkyInfo[nCntSky].fHeight;	// 高さ
		g_aSky[nCntSky].fDepth = g_aSkyInfo[nCntSky].fDepth;	// 奥行き
		g_aSky[nCntSky].fLength = g_aSkyInfo[nCntSky].fLength;	// 半径
		g_aSky[nCntSky].nType = g_aSkyInfo[nCntSky].nType;		// 種類
		g_aSky[nCntSky].bUse = true;										// 使用されているかどうか

	}
	g_fTexmove = 0;
	MakeVertexSky(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitSky(void)
{
	// テクスチャの破棄
	for (int nCntSky = 0; nCntSky < SKYTYPE_MAX; nCntSky++)
	{
		if (g_apTextureSky[nCntSky] != NULL)
		{
			g_apTextureSky[nCntSky]->Release();
			g_apTextureSky[nCntSky] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffSky != NULL)
	{
		g_pVtxBuffSky->Release();
		g_pVtxBuffSky = NULL;
	}
	// インデックスバッファバッファの破棄
	if (g_pIdxBuffSky != NULL)
	{
		g_pIdxBuffSky->Release();
		g_pIdxBuffSky = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateSky(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffSky->Lock(0, 0, (void**)&pVtx, 0);
	g_fTexmove += 0.0003f;
	if (g_fTexmove >= 0.5f)
	{
		g_fTexmove = 0.0f;
	}
	for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
	{// テクスチャの設定
		for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
		{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
		 // テクスチャの設定
			pVtx[nCntH + (BLOCK_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / BLOCK_X) / 100.0f + g_fTexmove, float(nCntV * 100 / BLOCK_Y) / 100.0f);
		}
	}
	// 頂点バッファをアンロックする
	g_pIdxBuffSky->Unlock();
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawSky(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxProjection;				// 計算用マトリックス
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	for (int nCntSky = 0; nCntSky < MAX_SKY; nCntSky++)
	{
		if (g_aSky[nCntSky].bUse == true) 
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSky[nCntSky].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSky[nCntSky].rot.y, g_aSky[nCntSky].rot.x, g_aSky[nCntSky].rot.z);
			D3DXMatrixMultiply(&g_aSky[nCntSky].mtxWorld, &g_aSky[nCntSky].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSky[nCntSky].pos.x, g_aSky[nCntSky].pos.y, g_aSky[nCntSky].pos.z);
			D3DXMatrixMultiply(&g_aSky[nCntSky].mtxWorld, &g_aSky[nCntSky].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSky[nCntSky].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSky, 0, sizeof(VERTEX_3D));
			
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSky);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureSky[g_aSky[nCntSky].nType]);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexSky, 0, g_nNumPolygonSky);

		}
	}
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexSky(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexSky * MAX_SKY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSky,
		NULL);
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD)*g_nNumIndexSky * MAX_SKY,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSky,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffSky->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntSky = 0; nCntSky < MAX_SKY; nCntSky++, pVtx += g_nNumVertexSky)
	{
		
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				// 頂点座標の設定
				pVtx[nCntH + (BLOCK_X+1)*nCntV].pos = D3DXVECTOR3(sinf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*(sinf(0.1f + D3DX_PI/2/BLOCK_Y*nCntV)*g_aSky[nCntSky].fLength),
					 (cosf(0 + D3DX_PI / 2 / BLOCK_Y*nCntV)*g_aSky[nCntSky].fHeight*BLOCK_Y),
					cosf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*(sinf(0.1f + D3DX_PI/2 / BLOCK_Y*nCntV)*g_aSky[nCntSky].fLength));
			}
		}
		
		for (int nCnt = 0; nCnt < g_nNumVertexSky; nCnt++)
		{
			// 法線の設定
			pVtx[nCnt].nor = pVtx[nCnt].pos / g_aSky[nCntSky].fLength;
		}
		for (int nCnt = 0; nCnt < g_nNumVertexSky; nCnt++)
		{
			// 頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{// テクスチャの設定
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
			// テクスチャの設定
				pVtx[nCntH + (BLOCK_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH*100/BLOCK_X)/100.0f, float(nCntV*100 / BLOCK_Y)/100.0f);
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffSky->Unlock();

	WORD *pIdx;	// インデックスデータへのポインタ
	// インデックスバッファをロックし頂点データへのポインタを取得
	g_pIdxBuffSky->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIdxSky = 0; nCntIdxSky < MAX_SKY; nCntIdxSky++, pIdx += g_nNumIndexSky)
	{// インデックスの設定
		int nCnt = 0;
		// 頂点座標の設定
		for (int nCntX = 0; nCntX < g_nNumIndexSky; nCntX += 2)
		{
			if (BLOCK_Y > 1 && nCntX % (VERTEX_WIDTH + 2) == VERTEX_WIDTH)
			{// 重なり部分
				pIdx[nCntX] = pIdx[nCntX - 1];
				pIdx[nCntX + 1] = (BLOCK_X + 1) + int(nCnt / 2);
			}
			else
			{// 通常部分
				pIdx[nCntX] = (BLOCK_X+1) + int(nCnt / 2);
				pIdx[nCntX + 1] = int(nCnt /2);
				nCnt += 2;
			}
		}
	}	
	// 頂点バッファをアンロックする
	g_pIdxBuffSky->Unlock();
}
