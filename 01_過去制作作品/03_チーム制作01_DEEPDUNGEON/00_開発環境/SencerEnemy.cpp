//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: エネミーの追従するときの距離
// Author : KishidaRei 
//
//=============================================================================
#include "SencerEnemy.h"
#include "input.h"
#include "camera.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERENEMY (5)
#define MAX_TEXTURE (2)

#define SENCERENEMY_VERTEX (  (g_SencerEnemy_h + 1) * (g_SencerEnemy_v + 1)   )	//頂点数
#define SENCERENEMY_INDEX (   (g_SencerEnemy_h * g_SencerEnemy_v )* 2 + (4 * (g_SencerEnemy_v - 1)) + 2   ) //インデックス数
#define SENCERENEMY_POLYGON ( (g_SencerEnemy_h * g_SencerEnemy_v) * 2 + (4 * (g_SencerEnemy_v - 1)) )	//ポリゴン数

#define SENCERENEMY_RADIUS (300.0f)
#define SENCERENEMY_HEIGHT (8.0f)

#define MESH_H (20) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerEnemy(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerEnemy(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerEnemy = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerEnemy = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerEnemy = NULL; //インデックスバッファへのポインタ

SENCERENEMY g_aSencerEnemy[MAX_SENCERENEMY];

int g_SencerEnemy_h, g_SencerEnemy_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerEnemy_h = MESH_H;
	g_SencerEnemy_v = MESH_V;


	MakeVertexSencerEnemy(pDevice);
	MakeIndexSencerEnemy(pDevice);

	for (int nCntSencerEnemy = 0; nCntSencerEnemy < MAX_SENCERENEMY; nCntSencerEnemy++)
	{
		g_aSencerEnemy[nCntSencerEnemy].fRadius = SENCERENEMY_RADIUS;
		g_aSencerEnemy[nCntSencerEnemy].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerEnemy(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerEnemy != NULL)
	{
		g_pTextureSencerEnemy->Release();
		g_pTextureSencerEnemy = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerEnemy != NULL)
	{
		g_pVtxBuffSencerEnemy->Release();
		g_pVtxBuffSencerEnemy = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerEnemy != NULL)
	{
		g_pIdxBuffSencerEnemy->Release();
		g_pIdxBuffSencerEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerEnemy(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerEnemy(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;



	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング



	for (int nCntSencerEnemy = 0; nCntSencerEnemy < MAX_SENCERENEMY; nCntSencerEnemy++)
	{
		if (g_aSencerEnemy[nCntSencerEnemy].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerEnemy[nCntSencerEnemy].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerEnemy[nCntSencerEnemy].rot.y, g_aSencerEnemy[nCntSencerEnemy].rot.x, g_aSencerEnemy[nCntSencerEnemy].rot.z);

			D3DXMatrixMultiply(&g_aSencerEnemy[nCntSencerEnemy].mtxWorld, &g_aSencerEnemy[nCntSencerEnemy].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerEnemy[nCntSencerEnemy].pos.x, g_aSencerEnemy[nCntSencerEnemy].pos.y, g_aSencerEnemy[nCntSencerEnemy].pos.z);

			D3DXMatrixMultiply(&g_aSencerEnemy[nCntSencerEnemy].mtxWorld, &g_aSencerEnemy[nCntSencerEnemy].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerEnemy[nCntSencerEnemy].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerEnemy, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerEnemy);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERENEMY_VERTEX, 0, SENCERENEMY_POLYGON);

		}
	}


#endif
}

//=============================================================================
// センサーの位置設定
//=============================================================================
void SetPositionSencerEnemy(int nIndexSencerEnemy, D3DXVECTOR3 pos)
{
	g_aSencerEnemy[nIndexSencerEnemy].pos.x = pos.x;
	g_aSencerEnemy[nIndexSencerEnemy].pos.y = pos.y + 30.0f;
	g_aSencerEnemy[nIndexSencerEnemy].pos.z = pos.z;

}
//=============================================================================
// センサーの消去
//=============================================================================
void DeleteSencerEnemy(int nIdxSencerEnemy)
{
	g_aSencerEnemy[nIdxSencerEnemy].bUse = false;
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERENEMY_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerEnemy,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerEnemy->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerEnemy_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerEnemy_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerEnemy_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerEnemy_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERENEMY_RADIUS;
			pVtx[nCntLife_V * (g_SencerEnemy_h + 1) + nCntLife_H].pos.y = SENCERENEMY_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerEnemy_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERENEMY_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerEnemy_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerEnemy_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerEnemy_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerEnemy_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerEnemy->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERENEMY_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerEnemy,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerEnemy->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerEnemy_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerEnemy_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerEnemy_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerEnemy_h && nCntIndex_V <= g_SencerEnemy_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerEnemy_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerEnemy->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencerEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntSencerEnemy;
	// 頂点情報の設定

	for (nCntSencerEnemy = 0; nCntSencerEnemy < MAX_SENCERENEMY; nCntSencerEnemy++)
	{
		if (g_aSencerEnemy[nCntSencerEnemy].bUse == false)
		{
			g_aSencerEnemy[nCntSencerEnemy].pos = pos;
			g_aSencerEnemy[nCntSencerEnemy].rot = rot;
			g_aSencerEnemy[nCntSencerEnemy].bUse = true;

			break;
		}
	}

	return nCntSencerEnemy;
}


//=============================================================================
// センサー判定(プレイヤーとの当たり判定)
//=============================================================================
bool CollisionSencer_Enemy(D3DXVECTOR3 *pos, int nCntSencerEnemy)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	


		fxpos = (g_aSencerEnemy[nCntSencerEnemy].pos.x - pos->x) * (g_aSencerEnemy[nCntSencerEnemy].pos.x - pos->x);
		fzpos = (g_aSencerEnemy[nCntSencerEnemy].pos.z - pos->z) * (g_aSencerEnemy[nCntSencerEnemy].pos.z - pos->z);
		fRadius = SENCERENEMY_RADIUS * SENCERENEMY_RADIUS;


		if (g_aSencerEnemy[nCntSencerEnemy].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				bCol = true;
			}

		}
	return bCol;
}

