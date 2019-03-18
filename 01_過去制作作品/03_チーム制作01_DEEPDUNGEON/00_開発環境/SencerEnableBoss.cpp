//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: エネミーの追従するときの距離
// Author : KishidaRei 
//
//=============================================================================
#include "SencerEnableBoss.h"
#include "input.h"
#include "camera.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERENABLEBOSS (5)
#define MAX_TEXTURE (2)

#define SENCERENABLEBOSS_VERTEX (  (g_SencerEnableBoss_h + 1) * (g_SencerEnableBoss_v + 1)   )	//頂点数
#define SENCERENABLEBOSS_INDEX (   (g_SencerEnableBoss_h * g_SencerEnableBoss_v )* 2 + (4 * (g_SencerEnableBoss_v - 1)) + 2   ) //インデックス数
#define SENCERENABLEBOSS_POLYGON ( (g_SencerEnableBoss_h * g_SencerEnableBoss_v) * 2 + (4 * (g_SencerEnableBoss_v - 1)) )	//ポリゴン数

#define SENCERENABLEBOSS_RADIUS (90.0f)
#define SENCERENABLEBOSS_HEIGHT (8.0f)

#define MESH_H (20) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerEnableBoss(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerEnableBoss(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerEnableBoss = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerEnableBoss = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerEnableBoss = NULL; //インデックスバッファへのポインタ

SENCERENABLEBOSS g_aSencerEnableBoss[MAX_SENCERENABLEBOSS];

int g_SencerEnableBoss_h, g_SencerEnableBoss_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerEnableBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerEnableBoss_h = MESH_H;
	g_SencerEnableBoss_v = MESH_V;


	MakeVertexSencerEnableBoss(pDevice);
	MakeIndexSencerEnableBoss(pDevice);

	for (int nCntSencerEnableBoss = 0; nCntSencerEnableBoss < MAX_SENCERENABLEBOSS; nCntSencerEnableBoss++)
	{
		g_aSencerEnableBoss[nCntSencerEnableBoss].fRadius = SENCERENABLEBOSS_RADIUS;
		g_aSencerEnableBoss[nCntSencerEnableBoss].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerEnableBoss(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerEnableBoss != NULL)
	{
		g_pTextureSencerEnableBoss->Release();
		g_pTextureSencerEnableBoss = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerEnableBoss != NULL)
	{
		g_pVtxBuffSencerEnableBoss->Release();
		g_pVtxBuffSencerEnableBoss = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerEnableBoss != NULL)
	{
		g_pIdxBuffSencerEnableBoss->Release();
		g_pIdxBuffSencerEnableBoss = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerEnableBoss(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerEnableBoss(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;



	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング



	for (int nCntSencerEnableBoss = 0; nCntSencerEnableBoss < MAX_SENCERENABLEBOSS; nCntSencerEnableBoss++)
	{
		if (g_aSencerEnableBoss[nCntSencerEnableBoss].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerEnableBoss[nCntSencerEnableBoss].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerEnableBoss[nCntSencerEnableBoss].rot.y, g_aSencerEnableBoss[nCntSencerEnableBoss].rot.x, g_aSencerEnableBoss[nCntSencerEnableBoss].rot.z);

			D3DXMatrixMultiply(&g_aSencerEnableBoss[nCntSencerEnableBoss].mtxWorld, &g_aSencerEnableBoss[nCntSencerEnableBoss].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerEnableBoss[nCntSencerEnableBoss].pos.x, g_aSencerEnableBoss[nCntSencerEnableBoss].pos.y, g_aSencerEnableBoss[nCntSencerEnableBoss].pos.z);

			D3DXMatrixMultiply(&g_aSencerEnableBoss[nCntSencerEnableBoss].mtxWorld, &g_aSencerEnableBoss[nCntSencerEnableBoss].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerEnableBoss[nCntSencerEnableBoss].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerEnableBoss, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerEnableBoss);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERENABLEBOSS_VERTEX, 0, SENCERENABLEBOSS_POLYGON);

		}
	}


#endif
}

//=============================================================================
// センサーの位置設定
//=============================================================================
void SetPositionSencerEnableBoss(int nIndexSencerEnableBoss, D3DXVECTOR3 pos)
{
	g_aSencerEnableBoss[nIndexSencerEnableBoss].pos.x = pos.x;
	g_aSencerEnableBoss[nIndexSencerEnableBoss].pos.y = pos.y + 30.0f;
	g_aSencerEnableBoss[nIndexSencerEnableBoss].pos.z = pos.z;

}
//=============================================================================
// センサーの消去
//=============================================================================
void DeleteSencerEnableBoss(int nIdxSencerEnableBoss)
{
	g_aSencerEnableBoss[nIdxSencerEnableBoss].bUse = false;
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerEnableBoss(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERENABLEBOSS_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerEnableBoss,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerEnableBoss->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerEnableBoss_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerEnableBoss_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerEnableBoss_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerEnableBoss_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERENABLEBOSS_RADIUS;
			pVtx[nCntLife_V * (g_SencerEnableBoss_h + 1) + nCntLife_H].pos.y = SENCERENABLEBOSS_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerEnableBoss_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERENABLEBOSS_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerEnableBoss_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerEnableBoss_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerEnableBoss_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerEnableBoss_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerEnableBoss->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerEnableBoss(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERENABLEBOSS_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerEnableBoss,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerEnableBoss->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerEnableBoss_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerEnableBoss_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerEnableBoss_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerEnableBoss_h && nCntIndex_V <= g_SencerEnableBoss_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerEnableBoss_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerEnableBoss->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencerEnableBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntSencerEnableBoss;
	// 頂点情報の設定

	for (nCntSencerEnableBoss = 0; nCntSencerEnableBoss < MAX_SENCERENABLEBOSS; nCntSencerEnableBoss++)
	{
		if (g_aSencerEnableBoss[nCntSencerEnableBoss].bUse == false)
		{
			g_aSencerEnableBoss[nCntSencerEnableBoss].pos = pos;
			g_aSencerEnableBoss[nCntSencerEnableBoss].rot = rot;
			g_aSencerEnableBoss[nCntSencerEnableBoss].bUse = true;

			break;
		}
	}

	return nCntSencerEnableBoss;
}


//=============================================================================
// センサー判定(プレイヤーとの当たり判定)
//=============================================================================
bool CollisionSencer_EnableBoss(D3DXVECTOR3 *pos)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencerEnableBoss = 0; nCntSencerEnableBoss < MAX_SENCERENABLEBOSS; nCntSencerEnableBoss++)
	{


		fxpos = (g_aSencerEnableBoss[nCntSencerEnableBoss].pos.x - pos->x) * (g_aSencerEnableBoss[nCntSencerEnableBoss].pos.x - pos->x);
		fzpos = (g_aSencerEnableBoss[nCntSencerEnableBoss].pos.z - pos->z) * (g_aSencerEnableBoss[nCntSencerEnableBoss].pos.z - pos->z);
		fRadius = SENCERENABLEBOSS_RADIUS * SENCERENABLEBOSS_RADIUS;


		if (g_aSencerEnableBoss[nCntSencerEnableBoss].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				bCol = true;
			}

		}
	}
#if  0
#endif
	return bCol;
}

