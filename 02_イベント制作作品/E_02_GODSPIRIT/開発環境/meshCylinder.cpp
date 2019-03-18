////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッシュ筒処理 [meshCylinder.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "meshCylinder.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_MESHCYLINDER	(1)
#define BLOCK_X	(100)
#define BLOCK_Y	(3)
#define VERTEX_WIDTH	(2+2*BLOCK_X)
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 中心座標
	D3DXVECTOR3 rot;			// 向き
	MESHCYLINDERTYPE nType;		// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	float fDepth;				// 奥行き
	float fLength;				// 半径
} MeshCylinderInfo;
typedef struct
{
	char *pFileName;
}MeshCylinderTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_apTextureMeshCylinder[MESHCYLINDERTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;						// インデックスバッファへのポインタ
MeshCylinder g_aMeshCylinder[(BLOCK_X + 1) * (BLOCK_Y + 1)];

int g_nNumVertexCylinder;	// 頂点数
int g_nNumIndexMeshCylinder;	// インデックス数
int g_nNumPolygonMeshCylinder;	// ポリゴン数
MeshCylinderInfo g_aMeshCylinderInfo[] =
{
	{ D3DXVECTOR3(0.0f,-700.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MESHCYLINDERTYPE_SEA, 10.0f,800.0f, 0.0f,19000.0f },
};
MeshCylinderTexture g_aMeshCylinderTexInfo[MESHCYLINDERTYPE_MAX] =
{
	{ "data/TEXTURE/mountain000.png" },
};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	
	for (int nCntMeshCylinderTex = 0; nCntMeshCylinderTex < MESHCYLINDERTYPE_MAX; nCntMeshCylinderTex++)
	{// テクスチャの設定
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_aMeshCylinderTexInfo[nCntMeshCylinderTex].pFileName,	// ファイルの名前
			&g_apTextureMeshCylinder[nCntMeshCylinderTex]);			// テクスチャへのポインタ
	}
	g_nNumVertexCylinder = (BLOCK_X + 1) * (BLOCK_Y + 1);					// 頂点数
	g_nNumIndexMeshCylinder = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1) + 2;	// インデックス数
	g_nNumPolygonMeshCylinder = 2 * (BLOCK_X*BLOCK_Y) + 4 * (BLOCK_Y - 1);	// ポリゴン数

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < g_nNumVertexCylinder; nCntMeshCylinder++)
	{
		g_aMeshCylinder[nCntMeshCylinder].pos = g_aMeshCylinderInfo[nCntMeshCylinder].pos;			// 位置
		g_aMeshCylinder[nCntMeshCylinder].rot = g_aMeshCylinderInfo[nCntMeshCylinder].rot;			// 向き
		g_aMeshCylinder[nCntMeshCylinder].fWidth = g_aMeshCylinderInfo[nCntMeshCylinder].fWidth;	// 幅
		g_aMeshCylinder[nCntMeshCylinder].fHeight = g_aMeshCylinderInfo[nCntMeshCylinder].fHeight;	// 高さ
		g_aMeshCylinder[nCntMeshCylinder].fDepth = g_aMeshCylinderInfo[nCntMeshCylinder].fDepth;	// 奥行き
		g_aMeshCylinder[nCntMeshCylinder].fLength = g_aMeshCylinderInfo[nCntMeshCylinder].fLength;	// 半径
		g_aMeshCylinder[nCntMeshCylinder].nType = g_aMeshCylinderInfo[nCntMeshCylinder].nType;		// 種類
		g_aMeshCylinder[nCntMeshCylinder].bUse = true;										// 使用されているかどうか

	}

	MakeVertexMeshCylinder(pDevice);

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitMeshCylinder(void)
{
	// テクスチャの破棄
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MESHCYLINDERTYPE_MAX; nCntMeshCylinder++)
	{
		if (g_apTextureMeshCylinder[nCntMeshCylinder] != NULL)
		{
			g_apTextureMeshCylinder[nCntMeshCylinder]->Release();
			g_apTextureMeshCylinder[nCntMeshCylinder] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
	// インデックスバッファバッファの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateMeshCylinder(void)
{
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい

	// Zバッファへの書き込み
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder[nCntMeshCylinder].rot.y, g_aMeshCylinder[nCntMeshCylinder].rot.x, g_aMeshCylinder[nCntMeshCylinder].rot.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder[nCntMeshCylinder].pos.x, g_aMeshCylinder[nCntMeshCylinder].pos.y, g_aMeshCylinder[nCntMeshCylinder].pos.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));
			
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMeshCylinder[g_aMeshCylinder[nCntMeshCylinder].nType]);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexCylinder, 0, g_nNumPolygonMeshCylinder);
		}
	}
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexCylinder * MAX_MESHCYLINDER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD)*g_nNumIndexMeshCylinder * MAX_MESHCYLINDER,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++, pVtx += g_nNumVertexCylinder)
	{
		
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				// 頂点座標の設定
				pVtx[nCntH + (BLOCK_X+1)*nCntV].pos = D3DXVECTOR3(sinf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*g_aMeshCylinder[nCntMeshCylinder].fLength,
					g_aMeshCylinder[nCntMeshCylinder].fHeight*BLOCK_Y - (g_aMeshCylinder[nCntMeshCylinder].fHeight)* nCntV,
					cosf(0 + (D3DX_PI*2 / (BLOCK_X ))* nCntH)*g_aMeshCylinder[nCntMeshCylinder].fLength);
			}
		}
		
		for (int nCnt = 0; nCnt < g_nNumVertexCylinder; nCnt++)
		{
			// 法線の設定
			D3DXVec3Normalize(&pVtx[nCnt].nor, &pVtx[nCnt].pos);//	 正規化する
		}
		for (int nCnt = 0; nCnt < g_nNumVertexCylinder; nCnt++)
		{
			// 頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//for (int nCnt = 0; nCnt < g_nNumVertexCylinder; nCnt++)
		//{// 1枚1枚
		//	// テクスチャの設定
		//	pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(BLOCK_X+1)), 0.0f + (nCnt / (BLOCK_X + 1)));
		//}
		for (int nCntV = 0; nCntV < BLOCK_Y + 1; nCntV++)
		{// テクスチャの設定
			for (int nCntH = 0; nCntH < BLOCK_X + 1; nCntH++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
			 // テクスチャの設定
				pVtx[nCntH + (BLOCK_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / BLOCK_X) / 50.0f, float(nCntV * 100 / BLOCK_Y) / 100.0f);
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;	// インデックスデータへのポインタ
	// インデックスバッファをロックし頂点データへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIdxMeshCylinder = 0; nCntIdxMeshCylinder < MAX_MESHCYLINDER; nCntIdxMeshCylinder++, pIdx += g_nNumIndexMeshCylinder)
	{// インデックスの設定
		int nCnt = 0;
		// 頂点座標の設定
		for (int nCntX = 0; nCntX < g_nNumIndexMeshCylinder; nCntX += 2)
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
	g_pIdxBuffMeshCylinder->Unlock();
}
