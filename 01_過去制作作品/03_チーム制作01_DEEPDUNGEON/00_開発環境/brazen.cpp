//=============================================================================
//
// 瘴気処理 [Brazen.cpp]
// Author : 
//
//=============================================================================
#include "brazen.h"
#include "input.h"
#include "camera.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BRAZEN_VERTEX ((g_Brazen_H + 1) * (g_Brazen_V + 1))
#define BRAZEN_INDEX ( (g_Brazen_H * g_Brazen_V )* 2 + (4 * (g_Brazen_V - 1)) + 2 )
#define BRAZEN_POLYGON ( (g_Brazen_H * g_Brazen_V) * 2 + (4 * (g_Brazen_V - 1)) )

#define BRAZEN_RADIUS (33.0f)
#define BRAZEN_RADIUS2 (6.0f)


#define BRAZEN_HEIGHT (5.0f)

#define BRAZEN_H (100) //メッシュの分割数(H)
#define BRAZEN_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBrazen(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexBrazen(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBrazen = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBrazen = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBrazen = NULL; //インデックスバッファへのポインタ

D3DXVECTOR3 g_posBrazen;		//位置(中心座標)
D3DXVECTOR3 g_rotBrazen;		//向き
D3DXMATRIX g_mtxWorldBrazen;	//ワールドマトリックス
D3DXCOLOR g_colBrazen;

//float g_fAngle;

int g_nBrazen;	//見かけの濃度
int nBrazen;	//内部数値の濃度

int g_Brazen_H, g_Brazen_V;
//=============================================================================
// 初期化処理
//=============================================================================
void InitBrazen(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_Brazen_H = BRAZEN_H;
	g_Brazen_V = BRAZEN_V;
	//g_fAngle = 1.0f / (g_Brazen_H / 2);

	int Polygon = (g_Brazen_H * g_Brazen_V) * 2 + (4 * (g_Brazen_V - 1));
	g_nBrazen = 0;
	nBrazen = 100;
	MakeVertexBrazen(pDevice);
	MakeIndexBrazen(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	g_posBrazen = D3DXVECTOR3(0.0f, 10.0f, -50.0f) + pPlayer.pos;
	g_rotBrazen = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f);
	g_colBrazen = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBrazen(void)
{
	//テクスチャの破棄
	if (g_pTextureBrazen != NULL)
	{
		g_pTextureBrazen->Release();
		g_pTextureBrazen = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBrazen != NULL)
	{
		g_pVtxBuffBrazen->Release();
		g_pVtxBuffBrazen = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffBrazen != NULL)
	{
		g_pIdxBuffBrazen->Release();
		g_pIdxBuffBrazen = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBrazen(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA pCamera;
	pCamera = *GetCamera();

	g_posBrazen = D3DXVECTOR3(pPlayer.pos.x, pPlayer.pos.y + 32.0f, pPlayer.pos.z );
	g_rotBrazen = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f + pPlayer.rot.y, 0.0f);


	
	
	//頂点バッファをロック
	g_pVtxBuffBrazen->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBRAZEN_V = 0; nCntBRAZEN_V < g_Brazen_V + 1; nCntBRAZEN_V++)
	{
		for (int nCntBRAZEN_H = 0; nCntBRAZEN_H < g_Brazen_H + 1; nCntBRAZEN_H++)
		{
			if (nCntBRAZEN_V == 0)
			{
				//頂点カラー
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].col = g_colBrazen;
			}
			if (nCntBRAZEN_V == 1)
			{
				//頂点カラー
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].col = g_colBrazen;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffBrazen->Unlock();

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawBrazen(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;


	

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBrazen);
	//pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotBrazen.y, g_rotBrazen.x, g_rotBrazen.z);

	D3DXMatrixMultiply(&g_mtxWorldBrazen, &g_mtxWorldBrazen, &mtxrot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		g_posBrazen.x, g_posBrazen.y, g_posBrazen.z);

	D3DXMatrixMultiply(&g_mtxWorldBrazen, &g_mtxWorldBrazen, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBrazen);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBrazen, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffBrazen);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, 0);


	// メッシュシリンダーの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, BRAZEN_VERTEX, 0, BRAZEN_POLYGON + g_nBrazen);



	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//=============================================================================
// 瘴気濃度の減少
//=============================================================================
void DecreaseBrazen(int nNumBrazen)
{
	g_nBrazen += nNumBrazen * 2;
	nBrazen += nNumBrazen ;

	if (nBrazen <= 0)
	{
		nBrazen = 0;
	}
}

int GetBrazen(void)
{
	return nBrazen;
}





//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexBrazen(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureBrazen);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BRAZEN_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBrazen,
		NULL);



	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBrazen->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI / g_Brazen_H;
	float fDestAngle;
	for (int nCntBRAZEN_V = 0; nCntBRAZEN_V < g_Brazen_V + 1; nCntBRAZEN_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntBRAZEN_H = 0; nCntBRAZEN_H < g_Brazen_H + 1; nCntBRAZEN_H++)
		{
			if (nCntBRAZEN_V == 0)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].pos.x = sinf(fDestAngle) * BRAZEN_RADIUS;
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].pos.y = 1.0f;
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].pos.z = cosf(fDestAngle) * BRAZEN_RADIUS;
				//法線の設定
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


				//頂点カラー
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].col = D3DXCOLOR(1.0f, 0.0f, 0.9f, 1.0f);

				//テクスチャ座標
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].tex = D3DXVECTOR2(nCntBRAZEN_H * (1.0f / (g_Brazen_H / 2)), nCntBRAZEN_V * -1.0f);

				fDestAngle += fAngle;
			}
			if (nCntBRAZEN_V == 1)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].pos.x = sinf(fDestAngle) * (BRAZEN_RADIUS - BRAZEN_RADIUS2);
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].pos.y = 1.0f;
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].pos.z = cosf(fDestAngle) * (BRAZEN_RADIUS - BRAZEN_RADIUS2);
				//法線の設定
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

				//頂点カラー
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].col = D3DXCOLOR(1.0f, 1.0f, 0.9f, 1.0f);

				//テクスチャ座標
				pVtx[nCntBRAZEN_V * (g_Brazen_H + 1) + nCntBRAZEN_H].tex = D3DXVECTOR2(nCntBRAZEN_H * (1.0f / (g_Brazen_H / 2)), nCntBRAZEN_V * -1.0f);

				fDestAngle += fAngle;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffBrazen->Unlock();



}

//=============================================================================
// インデックス情報の作成
//=============================================================================
void MakeIndexBrazen(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * BRAZEN_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBrazen,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffBrazen->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_Brazen_V; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_Brazen_H; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_Brazen_H + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_Brazen_H && nCntIndex_V <= g_Brazen_V)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_Brazen_H + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffBrazen->Unlock();

}




