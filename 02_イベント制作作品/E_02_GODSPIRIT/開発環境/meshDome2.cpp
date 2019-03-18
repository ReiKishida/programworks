//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// メッシュシドーム処理 [meshDome2.cpp]
// Author : 圷和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "meshDome2.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_MESHDOME2	(1)

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXMATRIX	mtxWorld;				// ワールドマトリックス
	float fRadius;						// 半径
	int nHBlock;						// X方向のブロック数
	int nVBlock;						// Y方向のブロック数
	int nNumVertex;						// 頂点数
	int nNumIndex;						// インデックス数
	int nNumPolygon;					// ポリゴン数
	int nTexType;						// テクスチャー種類
	bool bInside;						// 内側かどうか
	bool bUse;							// 使用しているかどうか
}MeshDome2;

typedef enum
{
	POLYGON_TEX_000 = 0,	// ポリゴンテクスチャ000
	POLYGON_TEX_MAX,		// 総数
}MeshDome2TexName;

typedef struct
{
	char *pFilename;	// ファイル名
} MeshDome2TexInfo;
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexMeshDome2(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexMeshDome2(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9			g_apTextureMeshDome2[POLYGON_TEX_MAX] = {};	// テクスチャへのポインタ

MeshDome2 g_aMeshDome2[MAX_MESHDOME2];				// メッシュドームの情報

// 読み込むテクスチャ名の設定
MeshDome2TexInfo g_apTextureMeshDome2Info[POLYGON_TEX_MAX] =
{
	{ "data//TEXTURE//sky001.jpg" },		// テクスチャ0
};

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitMeshDome2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	for (int nCntMeshDome2Tex = 0; nCntMeshDome2Tex < POLYGON_TEX_MAX; nCntMeshDome2Tex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureMeshDome2Info[nCntMeshDome2Tex].pFilename,
			&g_apTextureMeshDome2[nCntMeshDome2Tex]);
	}
	
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		g_aMeshDome2[nCntMeshDome2].pVtxBuff = NULL;
		g_aMeshDome2[nCntMeshDome2].pIdxBuff = NULL;
		g_aMeshDome2[nCntMeshDome2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshDome2[nCntMeshDome2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshDome2[nCntMeshDome2].nHBlock = 0;
		g_aMeshDome2[nCntMeshDome2].nVBlock = 0;
		g_aMeshDome2[nCntMeshDome2].fRadius = 0.0f;
		g_aMeshDome2[nCntMeshDome2].nNumVertex = 0;
		g_aMeshDome2[nCntMeshDome2].nNumIndex = 0;
		g_aMeshDome2[nCntMeshDome2].nNumPolygon = 0;
		g_aMeshDome2[nCntMeshDome2].nTexType = 0;
		g_aMeshDome2[nCntMeshDome2].bInside = false;
	}
	// 配置メッシュの情報
	g_aMeshDome2[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshDome2[0].nHBlock = 64;
	g_aMeshDome2[0].nVBlock = 16;
	g_aMeshDome2[0].fRadius = 10000.0f;
	g_aMeshDome2[0].bInside = false;

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// インデックス数計算	
		g_aMeshDome2[nCntMeshDome2].nNumIndex = 2 + g_aMeshDome2[nCntMeshDome2].nHBlock * 2;
		g_aMeshDome2[nCntMeshDome2].nNumIndex += (g_aMeshDome2[nCntMeshDome2].nNumIndex + 2) * (g_aMeshDome2[nCntMeshDome2].nVBlock - 1);

		// ポリゴン数計算
		g_aMeshDome2[nCntMeshDome2].nNumPolygon = (g_aMeshDome2[nCntMeshDome2].nHBlock * g_aMeshDome2[nCntMeshDome2].nVBlock) * 2 + 4 * (g_aMeshDome2[nCntMeshDome2].nVBlock - 1);

		// 頂点数計算
		g_aMeshDome2[nCntMeshDome2].nNumVertex = (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)*(g_aMeshDome2[nCntMeshDome2].nVBlock + 1);
	}

	// 頂点情報の作成
	MakeVertexMeshDome2(pDevice);

	// インデックス情報の作成
	MakeIndexMeshDome2(pDevice);

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		VERTEX_3D *pVtx;		// 頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報の設定	座標の直接指定はここでは基本的にはしない(g_pos~~等を足し引きはしないD3DXMatrixTranslationでやってる
		for (int nCntPos = 0; nCntPos < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntPos++, pVtx++)
		{
			pVtx[0].tex += D3DXVECTOR2(0.5f, 0.0f);
		}
		// 頂点バッファをアンロックする
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Unlock();
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitMeshDome2(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < POLYGON_TEX_MAX; nCntTex++)
	{
		if (g_apTextureMeshDome2[nCntTex] != NULL)
		{
			g_apTextureMeshDome2[nCntTex]->Release();
			g_apTextureMeshDome2[nCntTex] = NULL;
		}
	}

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// 頂点バッファの開放
		if (g_aMeshDome2[nCntMeshDome2].pVtxBuff != NULL)
		{
			g_aMeshDome2[nCntMeshDome2].pVtxBuff->Release();
			g_aMeshDome2[nCntMeshDome2].pVtxBuff = NULL;
		}

		// インデックスバッファの開放
		if (g_aMeshDome2[nCntMeshDome2].pIdxBuff != NULL)
		{
			g_aMeshDome2[nCntMeshDome2].pIdxBuff->Release();
			g_aMeshDome2[nCntMeshDome2].pIdxBuff = NULL;
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateMeshDome2(void)
{
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		g_aMeshDome2[nCntMeshDome2].rot.y += 0.001f;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawMeshDome2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot,mtxTrans;					// 計算用マトリックス距離

	// ライトオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshDome2[nCntMeshDome2].mtxWorld);

		// 回転を反映	Y,X,Zの順番が大切
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshDome2[nCntMeshDome2].rot.y, g_aMeshDome2[nCntMeshDome2].rot.x, g_aMeshDome2[nCntMeshDome2].rot.z);
		D3DXMatrixMultiply(&g_aMeshDome2[nCntMeshDome2].mtxWorld, &g_aMeshDome2[nCntMeshDome2].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aMeshDome2[nCntMeshDome2].pos.x, g_aMeshDome2[nCntMeshDome2].pos.y, g_aMeshDome2[nCntMeshDome2].pos.z);
		D3DXMatrixMultiply(&g_aMeshDome2[nCntMeshDome2].mtxWorld, &g_aMeshDome2[nCntMeshDome2].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshDome2[nCntMeshDome2].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_aMeshDome2[nCntMeshDome2].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_aMeshDome2[nCntMeshDome2].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureMeshDome2[0]);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aMeshDome2[nCntMeshDome2].nNumVertex, 0, g_aMeshDome2[nCntMeshDome2].nNumPolygon);
	}
	// ライトオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexMeshDome2(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// 頂点バッファを生成
		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshDome2[nCntMeshDome2].nNumVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_aMeshDome2[nCntMeshDome2].pVtxBuff,
			NULL);

		VERTEX_3D *pVtx;		// 頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		// 頂点情報の設定	座標の直接指定はここでは基本的にはしない(g_pos~~等を足し引きはしないD3DXMatrixTranslationでやってる
		for (int nCntPos = 0; nCntPos < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntPos++)
		{
			int nInside = 1;
			if (g_aMeshDome2[nCntMeshDome2].bInside == true)
			{
				nInside *= -1;
			}

			pVtx[nCntPos].pos =
				D3DXVECTOR3(float(sin(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * (nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * (cos(-D3DX_PI + ((D3DX_PI / 2.0f) / g_aMeshDome2[nCntMeshDome2].nVBlock) * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius)) * -1.0f,
					float(sin(D3DX_PI- ((D3DX_PI / 2.0f) / g_aMeshDome2[nCntMeshDome2].nVBlock) * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius),
					float(cos(float(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * float(nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)))) * (cos(-D3DX_PI + ((D3DX_PI / 2.0f) / g_aMeshDome2[nCntMeshDome2].nVBlock) * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius))) * -1.0f;

			/*pVtx[nCntPos].pos = 
				D3DXVECTOR3(float(sin(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * (nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))) * g_aMeshDome2[nCntMeshDome2].fRadius),
				float((g_aMeshDome2[nCntMeshDome2].nVBlock * g_aMeshDome2[nCntMeshDome2].fVertical) - g_aMeshDome2[nCntMeshDome2].fVertical * (nCntPos / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))),
				float(cos(float(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome2[nCntMeshDome2].nHBlock) * float(nCntPos % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)))) * g_aMeshDome2[nCntMeshDome2].fRadius));*/
		}

		// 法線の設定　法線は「1」にしなくてはならない？「1.0」以下にしなければならない？
		for (int nCntNor = 0; nCntNor < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntNor++)
		{
			D3DXVec3Normalize(&pVtx[nCntNor].nor, &pVtx[nCntNor].pos);		// 正規化する
		}

		// 頂点カラーの設定
		for (int nCntCol = 0; nCntCol < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntCol++)
		{
			pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャの設定
		for (int nCntTex = 0; nCntTex < g_aMeshDome2[nCntMeshDome2].nNumVertex; nCntTex++)
		{
			pVtx[nCntTex].tex = D3DXVECTOR2(1.0f / 2.0f / g_aMeshDome2[nCntMeshDome2].nHBlock * (nCntTex % (g_aMeshDome2[nCntMeshDome2].nHBlock + 1))*2, // 最後の*2はその場しのぎ
				1.0f / 2.0f / g_aMeshDome2[nCntMeshDome2].nVBlock * (nCntTex / (g_aMeshDome2[nCntMeshDome2].nHBlock + 1)));
		}


		// 頂点バッファをアンロックする
		g_aMeshDome2[nCntMeshDome2].pVtxBuff->Unlock();
	}

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// インデックス情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeIndexMeshDome2(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntMeshDome2 = 0; nCntMeshDome2 < MAX_MESHDOME2; nCntMeshDome2++)
	{
		// インデックスバッファを生成
		// インデックス情報の作成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome2[nCntMeshDome2].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,			// 一つの数字に2バイトしか使わない
			D3DPOOL_MANAGED,
			&g_aMeshDome2[nCntMeshDome2].pIdxBuff,
			NULL);

		WORD *pIdx;				// インデックスデータへのポインタ

		// インデックスバッファをロックし、インデックスデータへのポインタを取得
		g_aMeshDome2[nCntMeshDome2].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// インデックスの設定
		for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aMeshDome2[nCntMeshDome2].nNumIndex; nCntIdx += 2)
		{
			if ((nCntIdx / 2) % (g_aMeshDome2[nCntMeshDome2].nHBlock + 2) == g_aMeshDome2[nCntMeshDome2].nHBlock + 1)
			{
				pIdx[nCntIdx] = nCntPos - 1;
				pIdx[nCntIdx + 1] = nCntPos + g_aMeshDome2[nCntMeshDome2].nHBlock + 1;
			}
			else
			{
				pIdx[nCntIdx] = nCntPos + g_aMeshDome2[nCntMeshDome2].nHBlock + 1;
				pIdx[nCntIdx + 1] = nCntPos;

				nCntPos++;
			}
		}

		// 頂点バッファをアンロックする
		g_aMeshDome2[nCntMeshDome2].pIdxBuff->Unlock();
	}
}
