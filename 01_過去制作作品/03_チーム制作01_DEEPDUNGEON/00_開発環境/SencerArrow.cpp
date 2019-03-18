//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: エネミーの追従するときの距離
// Author : KishidaRei 
//
//=============================================================================
#include "SencerArrow.h"
#include "input.h"
#include "camera.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERENEMYARROW (5)
#define MAX_TEXTURE (2)

#define SENCERENEMYARROW_VERTEX (  (g_SencerEnemyArrow_h + 1) * (g_SencerEnemyArrow_v + 1)   )	//頂点数
#define SENCERENEMYARROW_INDEX (   (g_SencerEnemyArrow_h * g_SencerEnemyArrow_v )* 2 + (4 * (g_SencerEnemyArrow_v - 1)) + 2   ) //インデックス数
#define SENCERENEMYARROW_POLYGON ( (g_SencerEnemyArrow_h * g_SencerEnemyArrow_v) * 2 + (4 * (g_SencerEnemyArrow_v - 1)) )	//ポリゴン数

#define SENCERENEMYARROW_RADIUS (600.0f)
#define SENCERENEMYARROW_HEIGHT (8.0f)

#define MESH_H (20) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerEnemyArrow(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerEnemyArrow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerEnemyArrow = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerEnemyArrow = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerEnemyArrow = NULL; //インデックスバッファへのポインタ

SENCERARROW g_aSencerEnemyArow[MAX_SENCERENEMYARROW];

int g_SencerEnemyArrow_h, g_SencerEnemyArrow_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerEnemyArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerEnemyArrow_h = MESH_H;
	g_SencerEnemyArrow_v = MESH_V;


	MakeVertexSencerEnemyArrow(pDevice);
	MakeIndexSencerEnemyArrow(pDevice);

	for (int nCntSencerEnemyArrow = 0; nCntSencerEnemyArrow < MAX_SENCERENEMYARROW; nCntSencerEnemyArrow++)
	{
		g_aSencerEnemyArow[nCntSencerEnemyArrow].fRadius = SENCERENEMYARROW_RADIUS;
		g_aSencerEnemyArow[nCntSencerEnemyArrow].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerEnemyArrow(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerEnemyArrow != NULL)
	{
		g_pTextureSencerEnemyArrow->Release();
		g_pTextureSencerEnemyArrow = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerEnemyArrow != NULL)
	{
		g_pVtxBuffSencerEnemyArrow->Release();
		g_pVtxBuffSencerEnemyArrow = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerEnemyArrow != NULL)
	{
		g_pIdxBuffSencerEnemyArrow->Release();
		g_pIdxBuffSencerEnemyArrow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerEnemyArrow(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerEnemyArrow(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;



	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング



	for (int nCntSencerEnemyArrow = 0; nCntSencerEnemyArrow < MAX_SENCERENEMYARROW; nCntSencerEnemyArrow++)
	{
		if (g_aSencerEnemyArow[nCntSencerEnemyArrow].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerEnemyArow[nCntSencerEnemyArrow].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerEnemyArow[nCntSencerEnemyArrow].rot.y, g_aSencerEnemyArow[nCntSencerEnemyArrow].rot.x, g_aSencerEnemyArow[nCntSencerEnemyArrow].rot.z);

			D3DXMatrixMultiply(&g_aSencerEnemyArow[nCntSencerEnemyArrow].mtxWorld, &g_aSencerEnemyArow[nCntSencerEnemyArrow].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerEnemyArow[nCntSencerEnemyArrow].pos.x, g_aSencerEnemyArow[nCntSencerEnemyArrow].pos.y, g_aSencerEnemyArow[nCntSencerEnemyArrow].pos.z);

			D3DXMatrixMultiply(&g_aSencerEnemyArow[nCntSencerEnemyArrow].mtxWorld, &g_aSencerEnemyArow[nCntSencerEnemyArrow].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerEnemyArow[nCntSencerEnemyArrow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerEnemyArrow, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerEnemyArrow);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERENEMYARROW_VERTEX, 0, SENCERENEMYARROW_POLYGON);

		}
	}


#endif
}

//=============================================================================
// センサーの位置設定
//=============================================================================
void SetPositionSencerEnemyArrow(int nIndexSencerEnemy, D3DXVECTOR3 pos)
{
	g_aSencerEnemyArow[nIndexSencerEnemy].pos.x = pos.x;
	g_aSencerEnemyArow[nIndexSencerEnemy].pos.y = pos.y + 30.0f;
	g_aSencerEnemyArow[nIndexSencerEnemy].pos.z = pos.z;

}
//=============================================================================
// センサーの消去
//=============================================================================
void DeleteSencerEnemyArrow(int nIdxSencerEnemy)
{
	g_aSencerEnemyArow[nIdxSencerEnemy].bUse = false;
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerEnemyArrow(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERENEMYARROW_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerEnemyArrow,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerEnemyArrow->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerEnemyArrow_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerEnemyArrow_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerEnemyArrow_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerEnemyArrow_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERENEMYARROW_RADIUS;
			pVtx[nCntLife_V * (g_SencerEnemyArrow_h + 1) + nCntLife_H].pos.y = SENCERENEMYARROW_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerEnemyArrow_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERENEMYARROW_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerEnemyArrow_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerEnemyArrow_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerEnemyArrow_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerEnemyArrow_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerEnemyArrow->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerEnemyArrow(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERENEMYARROW_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerEnemyArrow,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerEnemyArrow->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerEnemyArrow_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerEnemyArrow_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerEnemyArrow_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerEnemyArrow_h && nCntIndex_V <= g_SencerEnemyArrow_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerEnemyArrow_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerEnemyArrow->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencerEnemyArrow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntSencerEnemyArrow;
	// 頂点情報の設定

	for (nCntSencerEnemyArrow = 0; nCntSencerEnemyArrow < MAX_SENCERENEMYARROW; nCntSencerEnemyArrow++)
	{
		if (g_aSencerEnemyArow[nCntSencerEnemyArrow].bUse == false)
		{
			g_aSencerEnemyArow[nCntSencerEnemyArrow].pos = pos;
			g_aSencerEnemyArow[nCntSencerEnemyArrow].rot = rot;
			g_aSencerEnemyArow[nCntSencerEnemyArrow].bUse = true;

			break;
		}
	}

	return nCntSencerEnemyArrow;
}


//=============================================================================
// センサー判定(プレイヤーとの当たり判定)
//=============================================================================
bool CollisionSencer_EnemyArrow(D3DXVECTOR3 *pos, int nCntSencerEnemyArrow)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;



	fxpos = (g_aSencerEnemyArow[nCntSencerEnemyArrow].pos.x - pos->x) * (g_aSencerEnemyArow[nCntSencerEnemyArrow].pos.x - pos->x);
	fzpos = (g_aSencerEnemyArow[nCntSencerEnemyArrow].pos.z - pos->z) * (g_aSencerEnemyArow[nCntSencerEnemyArrow].pos.z - pos->z);
	fRadius = SENCERENEMYARROW_RADIUS * SENCERENEMYARROW_RADIUS;


	if (g_aSencerEnemyArow[nCntSencerEnemyArrow].bUse == true)
	{
		if (fxpos + fzpos <= fRadius)
		{
			bCol = true;
		}

	}
	return bCol;
}

