//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// メッシュシドーム処理 [meshDome.cpp]
// Author : 圷和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "meshDome.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_MESHDOME	(1)

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
}MeshDome;

typedef enum
{
	POLYGON_TEX_000 = 0,	// ポリゴンテクスチャ000
	POLYGON_TEX_MAX,		// 総数
}MeshDomeTexName;

typedef struct
{
	char *pFilename;	// ファイル名
} MeshDomeTexInfo;
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexMeshDome(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexMeshDome(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9			g_apTextureMeshDome[POLYGON_TEX_MAX] = {};	// テクスチャへのポインタ

MeshDome g_aMeshDome[MAX_MESHDOME];				// メッシュドームの情報

// 読み込むテクスチャ名の設定
MeshDomeTexInfo g_apTextureMeshDomeInfo[POLYGON_TEX_MAX] =
{
	{ "data//TEXTURE//sky001.jpg" },		// テクスチャ0
};

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	for (int nCntMeshDomeTex = 0; nCntMeshDomeTex < POLYGON_TEX_MAX; nCntMeshDomeTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureMeshDomeInfo[nCntMeshDomeTex].pFilename,
			&g_apTextureMeshDome[nCntMeshDomeTex]);
	}
	
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		g_aMeshDome[nCntMeshDome].pVtxBuff = NULL;
		g_aMeshDome[nCntMeshDome].pIdxBuff = NULL;
		g_aMeshDome[nCntMeshDome].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshDome[nCntMeshDome].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshDome[nCntMeshDome].nHBlock = 0;
		g_aMeshDome[nCntMeshDome].nVBlock = 0;
		g_aMeshDome[nCntMeshDome].fRadius = 0.0f;
		g_aMeshDome[nCntMeshDome].nNumVertex = 0;
		g_aMeshDome[nCntMeshDome].nNumIndex = 0;
		g_aMeshDome[nCntMeshDome].nNumPolygon = 0;
		g_aMeshDome[nCntMeshDome].nTexType = 0;
		g_aMeshDome[nCntMeshDome].bInside = false;
	}
	// 配置メッシュの情報
	g_aMeshDome[0].pos = D3DXVECTOR3(0.0f, 00.0f, 0.0f);
	g_aMeshDome[0].nHBlock = 64;
	g_aMeshDome[0].nVBlock = 16;
	g_aMeshDome[0].fRadius = 10000.0f;
	g_aMeshDome[0].bInside = false;

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		// インデックス数計算	
		g_aMeshDome[nCntMeshDome].nNumIndex = 2 + g_aMeshDome[nCntMeshDome].nHBlock * 2;
		g_aMeshDome[nCntMeshDome].nNumIndex += (g_aMeshDome[nCntMeshDome].nNumIndex + 2) * (g_aMeshDome[nCntMeshDome].nVBlock - 1);

		// ポリゴン数計算
		g_aMeshDome[nCntMeshDome].nNumPolygon = (g_aMeshDome[nCntMeshDome].nHBlock * g_aMeshDome[nCntMeshDome].nVBlock) * 2 + 4 * (g_aMeshDome[nCntMeshDome].nVBlock - 1);

		// 頂点数計算
		g_aMeshDome[nCntMeshDome].nNumVertex = (g_aMeshDome[nCntMeshDome].nHBlock + 1)*(g_aMeshDome[nCntMeshDome].nVBlock + 1);
	}

	// 頂点情報の作成
	MakeVertexMeshDome(pDevice);

	// インデックス情報の作成
	MakeIndexMeshDome(pDevice);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitMeshDome(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < POLYGON_TEX_MAX; nCntTex++)
	{
		if (g_apTextureMeshDome[nCntTex] != NULL)
		{
			g_apTextureMeshDome[nCntTex]->Release();
			g_apTextureMeshDome[nCntTex] = NULL;
		}
	}

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		// 頂点バッファの開放
		if (g_aMeshDome[nCntMeshDome].pVtxBuff != NULL)
		{
			g_aMeshDome[nCntMeshDome].pVtxBuff->Release();
			g_aMeshDome[nCntMeshDome].pVtxBuff = NULL;
		}

		// インデックスバッファの開放
		if (g_aMeshDome[nCntMeshDome].pIdxBuff != NULL)
		{
			g_aMeshDome[nCntMeshDome].pIdxBuff->Release();
			g_aMeshDome[nCntMeshDome].pIdxBuff = NULL;
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateMeshDome(void)
{
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		g_aMeshDome[nCntMeshDome].rot.y += 0.001f;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot,mtxTrans;					// 計算用マトリックス距離

	// ライトオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshDome[nCntMeshDome].mtxWorld);

		// 回転を反映	Y,X,Zの順番が大切
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshDome[nCntMeshDome].rot.y, g_aMeshDome[nCntMeshDome].rot.x, g_aMeshDome[nCntMeshDome].rot.z);
		D3DXMatrixMultiply(&g_aMeshDome[nCntMeshDome].mtxWorld, &g_aMeshDome[nCntMeshDome].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aMeshDome[nCntMeshDome].pos.x, g_aMeshDome[nCntMeshDome].pos.y, g_aMeshDome[nCntMeshDome].pos.z);
		D3DXMatrixMultiply(&g_aMeshDome[nCntMeshDome].mtxWorld, &g_aMeshDome[nCntMeshDome].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshDome[nCntMeshDome].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_aMeshDome[nCntMeshDome].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_aMeshDome[nCntMeshDome].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureMeshDome[0]);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aMeshDome[nCntMeshDome].nNumVertex, 0, g_aMeshDome[nCntMeshDome].nNumPolygon);
	}
	// ライトオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexMeshDome(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		// 頂点バッファを生成
		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshDome[nCntMeshDome].nNumVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_aMeshDome[nCntMeshDome].pVtxBuff,
			NULL);

		VERTEX_3D *pVtx;		// 頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_aMeshDome[nCntMeshDome].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		// 頂点情報の設定	座標の直接指定はここでは基本的にはしない(g_pos~~等を足し引きはしないD3DXMatrixTranslationでやってる
		for (int nCntPos = 0; nCntPos < g_aMeshDome[nCntMeshDome].nNumVertex; nCntPos++)
		{
			int nInside = 1;
			if (g_aMeshDome[nCntMeshDome].bInside == true)
			{
				nInside *= -1;
			}

			pVtx[nCntPos].pos =
				D3DXVECTOR3(float(sin(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome[nCntMeshDome].nHBlock) * (nCntPos % (g_aMeshDome[nCntMeshDome].nHBlock + 1))) * (cos(-D3DX_PI + ((D3DX_PI / 2.0f) / g_aMeshDome[nCntMeshDome].nVBlock) * (nCntPos / (g_aMeshDome[nCntMeshDome].nHBlock + 1))) * g_aMeshDome[nCntMeshDome].fRadius)),
					float(sin(D3DX_PI- ((D3DX_PI / 2.0f) / g_aMeshDome[nCntMeshDome].nVBlock) * (nCntPos / (g_aMeshDome[nCntMeshDome].nHBlock + 1))) * g_aMeshDome[nCntMeshDome].fRadius),
					float(cos(float(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome[nCntMeshDome].nHBlock) * float(nCntPos % (g_aMeshDome[nCntMeshDome].nHBlock + 1)))) * (cos(-D3DX_PI + ((D3DX_PI / 2.0f) / g_aMeshDome[nCntMeshDome].nVBlock) * (nCntPos / (g_aMeshDome[nCntMeshDome].nHBlock + 1))) * g_aMeshDome[nCntMeshDome].fRadius)));

			/*pVtx[nCntPos].pos = 
				D3DXVECTOR3(float(sin(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome[nCntMeshDome].nHBlock) * (nCntPos % (g_aMeshDome[nCntMeshDome].nHBlock + 1))) * g_aMeshDome[nCntMeshDome].fRadius),
				float((g_aMeshDome[nCntMeshDome].nVBlock * g_aMeshDome[nCntMeshDome].fVertical) - g_aMeshDome[nCntMeshDome].fVertical * (nCntPos / (g_aMeshDome[nCntMeshDome].nHBlock + 1))),
				float(cos(float(D3DX_PI * nInside - ((D3DX_PI * 2.0f * nInside) / g_aMeshDome[nCntMeshDome].nHBlock) * float(nCntPos % (g_aMeshDome[nCntMeshDome].nHBlock + 1)))) * g_aMeshDome[nCntMeshDome].fRadius));*/
		}

		// 法線の設定　法線は「1」にしなくてはならない？「1.0」以下にしなければならない？
		for (int nCntNor = 0; nCntNor < g_aMeshDome[nCntMeshDome].nNumVertex; nCntNor++)
		{
			D3DXVec3Normalize(&pVtx[nCntNor].nor, &pVtx[nCntNor].pos);		// 正規化する
		}

		// 頂点カラーの設定
		for (int nCntCol = 0; nCntCol < g_aMeshDome[nCntMeshDome].nNumVertex; nCntCol++)
		{
			pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャの設定
		for (int nCntTex = 0; nCntTex < g_aMeshDome[nCntMeshDome].nNumVertex; nCntTex++)
		{
			pVtx[nCntTex].tex = D3DXVECTOR2(1.0f / 2.0f / g_aMeshDome[nCntMeshDome].nHBlock * (nCntTex % (g_aMeshDome[nCntMeshDome].nHBlock + 1))*2, // 最後の*2はその場しのぎ
				1.0f / 2.0f / g_aMeshDome[nCntMeshDome].nVBlock * (nCntTex / (g_aMeshDome[nCntMeshDome].nHBlock + 1)));
		}


		// 頂点バッファをアンロックする
		g_aMeshDome[nCntMeshDome].pVtxBuff->Unlock();
	}

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// インデックス情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeIndexMeshDome(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		// インデックスバッファを生成
		// インデックス情報の作成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome[nCntMeshDome].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,			// 一つの数字に2バイトしか使わない
			D3DPOOL_MANAGED,
			&g_aMeshDome[nCntMeshDome].pIdxBuff,
			NULL);

		WORD *pIdx;				// インデックスデータへのポインタ

		// インデックスバッファをロックし、インデックスデータへのポインタを取得
		g_aMeshDome[nCntMeshDome].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// インデックスの設定
		for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aMeshDome[nCntMeshDome].nNumIndex; nCntIdx += 2)
		{
			if ((nCntIdx / 2) % (g_aMeshDome[nCntMeshDome].nHBlock + 2) == g_aMeshDome[nCntMeshDome].nHBlock + 1)
			{
				pIdx[nCntIdx] = nCntPos - 1;
				pIdx[nCntIdx + 1] = nCntPos + g_aMeshDome[nCntMeshDome].nHBlock + 1;
			}
			else
			{
				pIdx[nCntIdx] = nCntPos + g_aMeshDome[nCntMeshDome].nHBlock + 1;
				pIdx[nCntIdx + 1] = nCntPos;

				nCntPos++;
			}
		}

		// 頂点バッファをアンロックする
		g_aMeshDome[nCntMeshDome].pIdxBuff->Unlock();
	}
}
