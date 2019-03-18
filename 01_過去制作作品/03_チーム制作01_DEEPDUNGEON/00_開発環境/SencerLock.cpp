//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: プレイヤーのロックオンできる距離
// Author : KishidaRei 
//
//=============================================================================
#include "SencerLock.h"
#include "input.h"
#include "camera.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERLOCK (5)
#define MAX_TEXTURE (2)

#define SENCERLOCK_VERTEX (  (g_SencerLock_h + 1) * (g_SencerLock_v + 1)   )	//頂点数
#define SENCERLOCK_INDEX (   (g_SencerLock_h * g_SencerLock_v )* 2 + (4 * (g_SencerLock_v - 1)) + 2   ) //インデックス数
#define SENCERLOCK_POLYGON ( (g_SencerLock_h * g_SencerLock_v) * 2 + (4 * (g_SencerLock_v - 1)) )	//ポリゴン数

#define SENCERLOCK_RADIUS (200.0f)
#define SENCERLOCK_HEIGHT (8.0f)

#define MESH_H (30) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerLock(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerLock(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerLock = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerLock = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerLock = NULL; //インデックスバッファへのポインタ

SENCERLOCK g_aSencerLock[MAX_SENCERLOCK];

int g_SencerLock_h, g_SencerLock_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerLock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerLock_h = MESH_H;
	g_SencerLock_v = MESH_V;


	MakeVertexSencerLock(pDevice);
	MakeIndexSencerLock(pDevice);

	for (int nCntSencerLock = 0; nCntSencerLock < MAX_SENCERLOCK; nCntSencerLock++)
	{
		g_aSencerLock[nCntSencerLock].fRadius = SENCERLOCK_RADIUS;
		g_aSencerLock[nCntSencerLock].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerLock(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerLock != NULL)
	{
		g_pTextureSencerLock->Release();
		g_pTextureSencerLock = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerLock != NULL)
	{
		g_pVtxBuffSencerLock->Release();
		g_pVtxBuffSencerLock = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerLock != NULL)
	{
		g_pIdxBuffSencerLock->Release();
		g_pIdxBuffSencerLock = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerLock(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerLock(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;



	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング



	for (int nCntSencerLock = 0; nCntSencerLock < MAX_SENCERLOCK; nCntSencerLock++)
	{
		if (g_aSencerLock[nCntSencerLock].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerLock[nCntSencerLock].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerLock[nCntSencerLock].rot.y, g_aSencerLock[nCntSencerLock].rot.x, g_aSencerLock[nCntSencerLock].rot.z);

			D3DXMatrixMultiply(&g_aSencerLock[nCntSencerLock].mtxWorld, &g_aSencerLock[nCntSencerLock].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerLock[nCntSencerLock].pos.x, g_aSencerLock[nCntSencerLock].pos.y, g_aSencerLock[nCntSencerLock].pos.z);

			D3DXMatrixMultiply(&g_aSencerLock[nCntSencerLock].mtxWorld, &g_aSencerLock[nCntSencerLock].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerLock[nCntSencerLock].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerLock, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerLock);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERLOCK_VERTEX, 0, SENCERLOCK_POLYGON);

		}
	}


#endif
}

//=============================================================================
// センサーの位置設定
//=============================================================================
void SetPositionSencerLock(int nIdxSencer, D3DXVECTOR3 pos)
{
	g_aSencerLock[nIdxSencer].pos.x = pos.x;
	g_aSencerLock[nIdxSencer].pos.y = pos.y;

	g_aSencerLock[nIdxSencer].pos.z = pos.z;

}
//=============================================================================
// センサーの消去
//=============================================================================
void DeleteSencerLock(int nIdxSencerLock)
{
	g_aSencerLock[nIdxSencerLock].bUse = false;
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerLock(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERLOCK_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerLock,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerLock->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerLock_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerLock_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerLock_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerLock_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERLOCK_RADIUS;
			pVtx[nCntLife_V * (g_SencerLock_h + 1) + nCntLife_H].pos.y = SENCERLOCK_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerLock_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERLOCK_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerLock_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerLock_h + 1) + nCntLife_H].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.3f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerLock_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerLock_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerLock->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerLock(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERLOCK_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerLock,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerLock->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerLock_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerLock_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerLock_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerLock_h && nCntIndex_V <= g_SencerLock_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerLock_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerLock->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencerLock(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntSencerLock;
	// 頂点情報の設定

	for (nCntSencerLock = 0; nCntSencerLock < MAX_SENCERLOCK; nCntSencerLock++)
	{
		if (g_aSencerLock[nCntSencerLock].bUse == false)
		{
			g_aSencerLock[nCntSencerLock].pos = pos;
			g_aSencerLock[nCntSencerLock].rot = rot;
			g_aSencerLock[nCntSencerLock].bUse = true;

			break;
		}
	}

	return nCntSencerLock;
}

//=============================================================================
// センサー判定（どのエネミーかの数字を返す）
//=============================================================================
int CollisionSencer_Lock_ReturnEnemy(D3DXVECTOR3 * pos, int nCntEnemy)
{
	bool bCol = false;
	int nCnt = 0;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencerLock = 0; nCntSencerLock < MAX_SENCERLOCK; nCntSencerLock++)
	{


		fxpos = (g_aSencerLock[nCntSencerLock].pos.x - pos->x) * (g_aSencerLock[nCntSencerLock].pos.x - pos->x);
		fzpos = (g_aSencerLock[nCntSencerLock].pos.z - pos->z) * (g_aSencerLock[nCntSencerLock].pos.z - pos->z);
		fRadius = SENCERLOCK_RADIUS * SENCERLOCK_RADIUS;


		if (g_aSencerLock[nCntSencerLock].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				bCol = true;
				nCnt = nCntEnemy;
			}

		}
	}
	return nCnt;

}

//=============================================================================
// センサー判定(敵との当たり判定)
//=============================================================================
bool CollisionSencer_Lock(D3DXVECTOR3 *pos)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencerLock = 0; nCntSencerLock < MAX_SENCERLOCK; nCntSencerLock++)
	{


		fxpos = (g_aSencerLock[nCntSencerLock].pos.x - pos->x) * (g_aSencerLock[nCntSencerLock].pos.x - pos->x);
		fzpos = (g_aSencerLock[nCntSencerLock].pos.z - pos->z) * (g_aSencerLock[nCntSencerLock].pos.z - pos->z);
		fRadius = SENCERLOCK_RADIUS * SENCERLOCK_RADIUS;


		if (g_aSencerLock[nCntSencerLock].bUse == true)
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

