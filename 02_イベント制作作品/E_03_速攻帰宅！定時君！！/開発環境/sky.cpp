////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 空処理 [sky.cpp]
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
	D3DXCOLOR col;			// 色
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
LPDIRECT3DTEXTURE9		g_apTextureSky[SKYTYPE_MAX] = {};		// テクスチャへのポインタ
Sky g_aSky[MAX_SKY];
float g_fTexmove;
SkyInfo g_aSkyInfo[] =
{// 座標	向き	タイプ	幅	高さ	奥行き	半径
	{ D3DXVECTOR3(0.0f, 0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SKYTYPE_0,  100.0f,100.0f, 0.0f,30000.0f },
};
SkyTexture g_aGuardTexInfo[SKYTYPE_MAX] =
{
	{ "data/TEXTURE/sky001.jpg" },
};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCnt = 0; nCnt < SKYTYPE_MAX; nCnt++)
	{// テクスチャの設定
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_aGuardTexInfo[nCnt].pFileName,	// ファイルの名前
			&g_apTextureSky[nCnt]);			// テクスチャへのポインタ
	}
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		g_aSky[nCnt].nNumVertex = (BLOCK_X + 1) * (BLOCK_Y + 1);					// 頂点数
		g_aSky[nCnt].nNumIndex = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1) + 2;	// インデックス数
		g_aSky[nCnt].nNumPolygon = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1);	// ポリゴン数


		g_aSky[nCnt].pos = g_aSkyInfo[nCnt].pos;			// 位置
		g_aSky[nCnt].rot = g_aSkyInfo[nCnt].rot;			// 向き
		g_aSky[nCnt].col = g_aSkyInfo[nCnt].col;			// 色
		g_aSky[nCnt].fWidth = g_aSkyInfo[nCnt].fWidth;	// 幅
		g_aSky[nCnt].fHeight = g_aSkyInfo[nCnt].fHeight;	// 高さ
		g_aSky[nCnt].fDepth = g_aSkyInfo[nCnt].fDepth;	// 奥行き
		g_aSky[nCnt].fLength = g_aSkyInfo[nCnt].fLength;	// 半径
		g_aSky[nCnt].nType = g_aSkyInfo[nCnt].nType;		// 種類
		g_aSky[nCnt].bUse = true;										// 使用されているかどうか
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
	for (int nCntGuard = 0; nCntGuard < SKYTYPE_MAX; nCntGuard++)
	{
		if (g_apTextureSky[nCntGuard] != NULL)
		{
			g_apTextureSky[nCntGuard]->Release();
			g_apTextureSky[nCntGuard] = NULL;
		}
	}
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		// 頂点バッファの破棄
		if (g_aSky[nCnt].pVtxBuff != NULL)
		{
			g_aSky[nCnt].pVtxBuff->Release();
			g_aSky[nCnt].pVtxBuff = NULL;
		}
		// インデックスバッファバッファの破棄
		if (g_aSky[nCnt].pIdxBuff != NULL)
		{
			g_aSky[nCnt].pIdxBuff->Release();
			g_aSky[nCnt].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateSky(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		if (g_aSky[nCnt].bUse)
		{
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aSky[nCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
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
			g_aSky[nCnt].pVtxBuff->Unlock();
		}
	}
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

	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		if (g_aSky[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSky[nCnt].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSky[nCnt].rot.y, g_aSky[nCnt].rot.x, g_aSky[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aSky[nCnt].mtxWorld, &g_aSky[nCnt].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSky[nCnt].pos.x, g_aSky[nCnt].pos.y, g_aSky[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aSky[nCnt].mtxWorld, &g_aSky[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSky[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aSky[nCnt].pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aSky[nCnt].pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureSky[g_aSky[nCnt].nType]);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aSky[nCnt].nNumVertex, 0, g_aSky[nCnt].nNumPolygon);

		}
	}
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化
																// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexSky(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{

		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aSky[nCnt].nNumVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_aSky[nCnt].pVtxBuff,
			NULL);
		// インデックスバッファの作成
		pDevice->CreateIndexBuffer(sizeof(WORD)*g_aSky[nCnt].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_aSky[nCnt].pIdxBuff,
			NULL);

		// 頂点バッファをロックし頂点データへのポインタを取得
		g_aSky[nCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				// 頂点座標の設定
				pVtx[nCntH + (BLOCK_X + 1)*nCntV].pos =
					D3DXVECTOR3(sinf(0 + (D3DX_PI * 2 / (BLOCK_X))* nCntH)*(sinf(D3DX_PI / BLOCK_Y*nCntV)*g_aSky[nCnt].fLength),
					(cosf(0 + D3DX_PI / BLOCK_Y*nCntV)*g_aSky[nCnt].fHeight*BLOCK_Y),
					cosf(0 + (D3DX_PI * 2 / (BLOCK_X))* nCntH)*(sinf(0.1f + D3DX_PI / BLOCK_Y*nCntV)*g_aSky[nCnt].fLength));
			}
		}

		for (int nCntVtx = 0; nCntVtx < g_aSky[nCnt].nNumVertex; nCntVtx++)
		{
			// 法線の設定
			pVtx[nCntVtx].nor = pVtx[nCntVtx].pos / g_aSky[nCnt].fLength;
		}
		for (int nCntVtx = 0; nCntVtx < g_aSky[nCnt].nNumVertex; nCntVtx++)
		{
			// 頂点カラーの設定
			pVtx[nCntVtx].col = g_aSky[nCnt].col;
		}
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{// テクスチャの設定
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
			// テクスチャの設定
				pVtx[nCntH + (BLOCK_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / BLOCK_X) / 100.0f, float(nCntV * 100 / BLOCK_Y) / 100.0f);
			}
		}

		// 頂点バッファをアンロックする
		g_aSky[nCnt].pVtxBuff->Unlock();
	}
	for (int nCntIdxGuard = 0; nCntIdxGuard < MAX_SKY; nCntIdxGuard++)
	{// インデックスの設定
		int nCnt = 0;

		WORD *pIdx;	// インデックスデータへのポインタ
		// インデックスバッファをロックし頂点データへのポインタを取得
		g_aSky[nCntIdxGuard].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
		// 頂点座標の設定
		for (int nCntX = 0; nCntX < g_aSky[nCntIdxGuard].nNumIndex; nCntX += 2)
		{
			if (BLOCK_Y > 1 && nCntX % (VERTEX_WIDTH + 2) == VERTEX_WIDTH)
			{// 重なり部分
				pIdx[nCntX] = pIdx[nCntX - 1];
				pIdx[nCntX + 1] = (BLOCK_X + 1) + int(nCnt / 2);
			}
			else
			{// 通常部分
				pIdx[nCntX] = (BLOCK_X + 1) + int(nCnt / 2);
				pIdx[nCntX + 1] = int(nCnt / 2);
				nCnt += 2;
			}
		}

		// 頂点バッファをアンロックする
		g_aSky[nCntIdxGuard].pIdxBuff->Unlock();
	}

}
//==============================================================================================================================
// セット
//==============================================================================================================================
void SetSky(D3DXVECTOR3 pos, float fLength)
{
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{

		g_aSky[nCnt].bUse = true;
		g_aSky[nCnt].pos = pos;
		g_aSky[nCnt].fLength = fLength;
		g_aSky[nCnt].fHeight = g_aSkyInfo[nCnt].fHeight*fLength;
	}
}

