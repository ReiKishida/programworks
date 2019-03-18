//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: 攻撃時の当たり判定がどこにくっついてんのか
// Author : KishidaRei 
//
//=============================================================================
#include "SencerAtk.h"
#include "input.h"
#include "camera.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERATK (128)
#define MAX_TEXTURE (2)

#define SENCERATK_VERTEX (  (g_SencerAtk_h + 1) * (g_SencerAtk_v + 1)   )	//頂点数
#define SENCERATK_INDEX (   (g_SencerAtk_h * g_SencerAtk_v )* 2 + (4 * (g_SencerAtk_v - 1)) + 2   ) //インデックス数
#define SENCERATK_POLYGON ( (g_SencerAtk_h * g_SencerAtk_v) * 2 + (4 * (g_SencerAtk_v - 1)) )	//ポリゴン数

#define SENCERATK_RADIUS (8.0f)
#define SENCERATK_HEIGHT (3.0f)

#define MESH_H (20) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerAtk(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerAtk(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerAtk = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerAtk = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerAtk = NULL; //インデックスバッファへのポインタ

SENCERATK g_aSencerAtk[MAX_SENCERATK];

int g_SencerAtk_h, g_SencerAtk_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerAtk(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerAtk_h = MESH_H;
	g_SencerAtk_v = MESH_V;


	MakeVertexSencerAtk(pDevice);
	MakeIndexSencerAtk(pDevice);

	for (int nCntSencerAtk = 0; nCntSencerAtk < MAX_SENCERATK; nCntSencerAtk++)
	{
		g_aSencerAtk[nCntSencerAtk].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSencerAtk[nCntSencerAtk].rot = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);
		g_aSencerAtk[nCntSencerAtk].fRadius = SENCERATK_RADIUS;
		g_aSencerAtk[nCntSencerAtk].nIdxParent = -1;
		g_aSencerAtk[nCntSencerAtk].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerAtk(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerAtk != NULL)
	{
		g_pTextureSencerAtk->Release();
		g_pTextureSencerAtk = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerAtk != NULL)
	{
		g_pVtxBuffSencerAtk->Release();
		g_pVtxBuffSencerAtk = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerAtk != NULL)
	{
		g_pIdxBuffSencerAtk->Release();
		g_pIdxBuffSencerAtk = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerAtk(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerAtk(void)
{
#if 1
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;
	D3DXMATRIX mtxParent;						// 親モデルの情報


	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング

	

	for (int nCntSencerAtk = 0; nCntSencerAtk < MAX_SENCERATK; nCntSencerAtk++)
	{
		if (g_aSencerAtk[nCntSencerAtk].bUse == true)
		{
			if (g_aSencerAtk[nCntSencerAtk].nIdxParent == -1)
			{
				mtxParent = pPlayer->aModel[16].mtxWorldPlayerModel;
			}

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerAtk[nCntSencerAtk].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerAtk[nCntSencerAtk].rot.y, g_aSencerAtk[nCntSencerAtk].rot.x, g_aSencerAtk[nCntSencerAtk].rot.z);

			D3DXMatrixMultiply(&g_aSencerAtk[nCntSencerAtk].mtxWorld, &g_aSencerAtk[nCntSencerAtk].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerAtk[nCntSencerAtk].pos.x, g_aSencerAtk[nCntSencerAtk].pos.y, g_aSencerAtk[nCntSencerAtk].pos.z);

			D3DXMatrixMultiply(&g_aSencerAtk[nCntSencerAtk].mtxWorld, &g_aSencerAtk[nCntSencerAtk].mtxWorld, &mtxtrans);
			
			//親のマトリックス反映
			D3DXMatrixMultiply(&g_aSencerAtk[nCntSencerAtk].mtxWorld, &g_aSencerAtk[nCntSencerAtk].mtxWorld, &mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerAtk[nCntSencerAtk].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerAtk, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerAtk);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERATK_VERTEX, 0, SENCERATK_POLYGON);

		}
	}


#endif
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerAtk(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERATK_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerAtk,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerAtk->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerAtk_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerAtk_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerAtk_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerAtk_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERATK_RADIUS;
			pVtx[nCntLife_V * (g_SencerAtk_h + 1) + nCntLife_H].pos.y = SENCERATK_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerAtk_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERATK_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerAtk_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerAtk_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerAtk_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerAtk_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerAtk->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerAtk(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERATK_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerAtk,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerAtk->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerAtk_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerAtk_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerAtk_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerAtk_h && nCntIndex_V <= g_SencerAtk_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerAtk_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerAtk->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
void SetSencerAtk(D3DXVECTOR3 pos)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//頂点バッファをロック
	g_pVtxBuffSencerAtk->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntSencerAtk = 0; nCntSencerAtk < MAX_SENCERATK; nCntSencerAtk++)
	{
		if (g_aSencerAtk[nCntSencerAtk].bUse == false)
		{
			g_aSencerAtk[nCntSencerAtk].pos = pos ;
			
			g_aSencerAtk[nCntSencerAtk].bUse = true;

			break;
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerAtk->Unlock();
}


//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionSencerAtk(float *fRadius, D3DXVECTOR3 *pos)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadiusSum = 0.0f;

	for (int nCntSencerAtk = 0; nCntSencerAtk < MAX_SENCERATK; nCntSencerAtk++)
	{
		//fxpos = (g_aSencerAtk[nCntSencerAtk].pos.x - pos->x) * (g_aSencerAtk[nCntSencerAtk].pos.x - pos->x);
		//fzpos = (g_aSencerAtk[nCntSencerAtk].pos.z - pos->z) * (g_aSencerAtk[nCntSencerAtk].pos.z - pos->z);
		fxpos = (pos->x - g_aSencerAtk[nCntSencerAtk].pos.x) * (pos->x - g_aSencerAtk[nCntSencerAtk].pos.x);
		fzpos = (pos->z - g_aSencerAtk[nCntSencerAtk].pos.z) * (pos->z - g_aSencerAtk[nCntSencerAtk].pos.z);
		float fRadiusSum = (SENCERATK_RADIUS + *fRadius) * (SENCERATK_RADIUS + *fRadius);

		if (g_aSencerAtk[nCntSencerAtk].bUse == true)
		{
			if (fxpos + fzpos <= *fRadius)
			{
				bCol = true;
			}

		}
	}
	return bCol;
	
}
//=============================================================================
// センサー判定
//=============================================================================
bool CollisionSencerAtk(D3DXVECTOR3 *pos)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencerAtk = 0; nCntSencerAtk < MAX_SENCERATK; nCntSencerAtk++)
	{
		//fxpos = powf((pos->x - g_aSencerAtk[nCntSencerAtk].pos.x), 2.0f);
		//fzpos = powf((pos->z - g_aSencerAtk[nCntSencerAtk].pos.z), 2.0f);
		//fpos = fxpos + fzpos;
		//fRadius = powf(SENCERATK_RADIUS, 2.0f);

		fxpos = (g_aSencerAtk[nCntSencerAtk].pos.x - pos->x) * (g_aSencerAtk[nCntSencerAtk].pos.x - pos->x);
		fzpos = (g_aSencerAtk[nCntSencerAtk].pos.z - pos->z) * (g_aSencerAtk[nCntSencerAtk].pos.z - pos->z);
		fRadius = SENCERATK_RADIUS * SENCERATK_RADIUS;


		//fxpos = (pos->x - g_aSencerAtk[nCntSencerAtk].pos.x);
		//fzpos = (pos->z - g_aSencerAtk[nCntSencerAtk].pos.z);
		//fpos = fxpos + fzpos;
		//fRadius = SENCERATK_RADIUS;
		if (g_aSencerAtk[nCntSencerAtk].bUse == true)
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
