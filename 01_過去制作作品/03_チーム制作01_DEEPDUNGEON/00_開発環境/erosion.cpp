//=============================================================================
//
// 侵食度処理 [Erosion.cpp]
// Author : 
//
//=============================================================================
#include "erosion.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "died.h"
#include "life.h"
#include "brazen.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EROSION_VERTEX ((g_Eosion_H + 1) * (g_Erosion_V + 1))
#define EROSION_INDEX ( (g_Eosion_H * g_Erosion_V )* 2 + (4 * (g_Erosion_V - 1)) + 2 )
#define EROSION_POLYGON ( (g_Eosion_H * g_Erosion_V) * 2 + (4 * (g_Erosion_V - 1)) )

#define EROSION_RADIUS (30.0f)
#define EROSION_RADIUS2 (6.0f)
#define EROSION_TIME (60)

#define EROSION_HEIGHT (5.0f)

#define EROSION_H (100) //メッシュの分割数(H)
#define EROSION_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexErosion(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexErosion(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureErosion = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffErosion = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffErosion = NULL; //インデックスバッファへのポインタ

D3DXVECTOR3 g_posErosion;		//位置(中心座標)
D3DXVECTOR3 g_rotErosion;		//向き
D3DXMATRIX g_mtxWorldErosion;	//ワールドマトリックス
D3DXCOLOR g_colErosion;
EROSION g_Erosion;
int nCntOneSecond;	//60フレーム計測用
int nCntSecond;	//秒数計測
//float g_fAngle;

int g_Eosion_H, g_Erosion_V;
//=============================================================================
// 初期化処理
//=============================================================================
void InitErosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_Eosion_H = EROSION_H;
	g_Erosion_V = EROSION_V;
	//g_fAngle = 1.0f / (g_Eosion_H / 2);

	int Polygon = (g_Eosion_H * g_Erosion_V) * 2 + (4 * (g_Erosion_V - 1));
	g_Erosion.nErosion = -200;
	g_Erosion.nErosionNum = 0;
	MakeVertexErosion(pDevice);
	MakeIndexErosion(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	g_posErosion = D3DXVECTOR3(0.0f, 10.0f, -50.0f) + pPlayer.pos;
	g_rotErosion = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f);
	g_colErosion = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	nCntOneSecond = 0;
	nCntOneSecond = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitErosion(void)
{
	//テクスチャの破棄
	if (g_pTextureErosion != NULL)
	{
		g_pTextureErosion->Release();
		g_pTextureErosion = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffErosion != NULL)
	{
		g_pVtxBuffErosion->Release();
		g_pVtxBuffErosion = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffErosion != NULL)
	{
		g_pIdxBuffErosion->Release();
		g_pIdxBuffErosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateErosion(void)
{
	int pBrazen;
	pBrazen = GetBrazen();


	nCntOneSecond++;
	if (nCntOneSecond % 1050 == 0)
	{
		DecreaseMaxLife(-40);
		IncreaseErosion(10);
	}

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA pCamera;
	pCamera = *GetCamera();

	
	
		/*if (nCntSecond % EROSION_TIME == 0)
		{
			DecreaseMaxLife(-30);
			IncreaseErosion(5);
		}*/
	
	/*else if (pBrazen >= 75)
	{
		if (nCntSecond % EROSION_TIME == 0)
		{
			DecreaseMaxLife(-30);
			IncreaseErosion(5);

		}
	}
	else if (pBrazen >= 50)
	{
		if (nCntSecond % EROSION_TIME == 0)
		{
			DecreaseMaxLife(-30);
			IncreaseErosion(5);

		}
	}
	else if (pBrazen >= 25)
	{
		if (nCntSecond % EROSION_TIME == 0)
		{
			DecreaseMaxLife(-30);
			IncreaseErosion(5);

		}
	}*/
	


	g_posErosion = D3DXVECTOR3(pPlayer.pos.x, pPlayer.pos.y + 35.0f, pPlayer.pos.z);
	g_rotErosion = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f + pPlayer.rot.y, 0.0f);

	/*if (GetKeyboardTrigger(DIK_M) == true)
	{
		DecreaseMaxLife(-30);
		IncreaseErosion(5);
	}*/
	//頂点バッファをロック
	g_pVtxBuffErosion->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEROSION_V = 0; nCntEROSION_V < g_Erosion_V + 1; nCntEROSION_V++)
	{
		for (int nCntEROSION_H = 0; nCntEROSION_H < g_Eosion_H + 1; nCntEROSION_H++)
		{
			if (nCntEROSION_V == 0)
			{
				//頂点カラー
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].col = g_colErosion;
			}
			if (nCntEROSION_V == 1)
			{
				//頂点カラー
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].col = g_colErosion;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffErosion->Unlock();

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawErosion(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;




	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldErosion);
	//pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotErosion.y, g_rotErosion.x, g_rotErosion.z);

	D3DXMatrixMultiply(&g_mtxWorldErosion, &g_mtxWorldErosion, &mtxrot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		g_posErosion.x, g_posErosion.y, g_posErosion.z);

	D3DXMatrixMultiply(&g_mtxWorldErosion, &g_mtxWorldErosion, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldErosion);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffErosion, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffErosion);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, 0);


	// メッシュシリンダーの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, EROSION_VERTEX, 0, EROSION_POLYGON + g_Erosion.nErosion);



	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//=============================================================================
// 侵食の増加
//=============================================================================
void IncreaseErosion(int nNumErosion)
{
	

	PLAYER *pPlayer = GetPlayer();
	DIED *pDied = GetDied();
	DecreaseMaxLife(-5);

	g_Erosion.nErosion += nNumErosion * 2;
	g_Erosion.nErosionNum += nNumErosion;

	if (g_Erosion.nErosionNum >= 100)
	{
		g_Erosion.nErosionNum = 100;
		HitDamage(-2000);
		//HitDamage(-2!000);

	}
}

int GetnCntErosion(void)
{
	return nCntOneSecond;
}





//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexErosion(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureErosion);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * EROSION_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffErosion,
		NULL);



	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffErosion->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI / g_Eosion_H;
	float fDestAngle;
	for (int nCntEROSION_V = 0; nCntEROSION_V < g_Erosion_V + 1; nCntEROSION_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntEROSION_H = 0; nCntEROSION_H < g_Eosion_H + 1; nCntEROSION_H++)
		{
			if (nCntEROSION_V == 0)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].pos.x = sinf(fDestAngle) * EROSION_RADIUS;
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].pos.y = 1.0f;
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].pos.z = cosf(fDestAngle) * EROSION_RADIUS;
				//法線の設定
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


				//頂点カラー
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//テクスチャ座標
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].tex = D3DXVECTOR2(nCntEROSION_H * (1.0f / (g_Eosion_H / 2)), nCntEROSION_V * -1.0f);

				fDestAngle += fAngle;
			}
			if (nCntEROSION_V == 1)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].pos.x = sinf(fDestAngle) * (EROSION_RADIUS - EROSION_RADIUS2);
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].pos.y = 1.0f;
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].pos.z = cosf(fDestAngle) * (EROSION_RADIUS - EROSION_RADIUS2);
				//法線の設定
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

				//頂点カラー
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//テクスチャ座標
				pVtx[nCntEROSION_V * (g_Eosion_H + 1) + nCntEROSION_H].tex = D3DXVECTOR2(nCntEROSION_H * (1.0f / (g_Eosion_H / 2)), nCntEROSION_V * -1.0f);

				fDestAngle += fAngle;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffErosion->Unlock();



}

//=============================================================================
// インデックス情報の作成
//=============================================================================
void MakeIndexErosion(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * EROSION_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffErosion,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffErosion->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_Erosion_V; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_Eosion_H; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_Eosion_H + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_Eosion_H && nCntIndex_V <= g_Erosion_V)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_Eosion_H + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffErosion->Unlock();

}




