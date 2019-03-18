//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: 魔法陣との当たり判定用
// Author : KishidaRei 
//
//=============================================================================
#include "SencerNextStage.h"
#include "input.h"
#include "camera.h"
#include "SencerAtk.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERNEXT (50)
#define MAX_TEXTURE (2)

#define SENCERNEXT_VERTEX (  (g_SencerNext_h + 1) * (g_SencerNext_v + 1)   )	//頂点数
#define SENCERNEXT_INDEX (   (g_SencerNext_h * g_SencerNext_v )* 2 + (4 * (g_SencerNext_v - 1)) + 2   ) //インデックス数
#define SENCERNEXT_POLYGON ( (g_SencerNext_h * g_SencerNext_v) * 2 + (4 * (g_SencerNext_v - 1)) )	//ポリゴン数

#define SENCERNEXT_RADIUS (100.0f)
#define SENCERNEXT_HEIGHT (30.0f)

#define MESH_H (50) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerNext(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerNext(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerNext = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerNext = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerNext = NULL; //インデックスバッファへのポインタ

SENCERNEXT g_aSencerNext[MAX_SENCERNEXT];

int g_SencerNext_h, g_SencerNext_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerNext(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerNext_h = MESH_H;
	g_SencerNext_v = MESH_V;


	MakeVertexSencerNext(pDevice);
	MakeIndexSencerNext(pDevice);

	for (int nCntSencer = 0; nCntSencer < MAX_SENCERNEXT; nCntSencer++)
	{
		g_aSencerNext[nCntSencer].fRadius = SENCERNEXT_RADIUS;
		g_aSencerNext[nCntSencer].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerNext(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerNext != NULL)
	{
		g_pTextureSencerNext->Release();
		g_pTextureSencerNext = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerNext != NULL)
	{
		g_pVtxBuffSencerNext->Release();
		g_pVtxBuffSencerNext = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerNext != NULL)
	{
		g_pIdxBuffSencerNext->Release();
		g_pIdxBuffSencerNext = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerNext(void)
{

	for (int nCntSencer = 0; nCntSencer < MAX_SENCERNEXT; nCntSencer++)
	{
		CollisionSencerAtk(&g_aSencerNext[nCntSencer].fRadius, &g_aSencerNext[nCntSencer].pos);
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerNext(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;



	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング



	for (int nCntSencer = 0; nCntSencer < MAX_SENCERNEXT; nCntSencer++)
	{
		if (g_aSencerNext[nCntSencer].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerNext[nCntSencer].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerNext[nCntSencer].rot.y, g_aSencerNext[nCntSencer].rot.x, g_aSencerNext[nCntSencer].rot.z);

			D3DXMatrixMultiply(&g_aSencerNext[nCntSencer].mtxWorld, &g_aSencerNext[nCntSencer].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerNext[nCntSencer].pos.x, g_aSencerNext[nCntSencer].pos.y, g_aSencerNext[nCntSencer].pos.z);

			D3DXMatrixMultiply(&g_aSencerNext[nCntSencer].mtxWorld, &g_aSencerNext[nCntSencer].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerNext[nCntSencer].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerNext, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerNext);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERNEXT_VERTEX, 0, SENCERNEXT_POLYGON);

		}
	}

#endif	
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerNext(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERNEXT_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerNext,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerNext->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerNext_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerNext_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerNext_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerNext_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERNEXT_RADIUS;
			pVtx[nCntLife_V * (g_SencerNext_h + 1) + nCntLife_H].pos.y = SENCERNEXT_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerNext_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERNEXT_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerNext_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerNext_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerNext_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerNext_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerNext->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerNext(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERNEXT_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerNext,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerNext->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerNext_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerNext_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerNext_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerNext_h && nCntIndex_V <= g_SencerNext_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerNext_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerNext->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencerNext(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 頂点情報の設定
	//VERTEX_3D *pVtx;
	int nCntSencer;

	for (nCntSencer = 0; nCntSencer < MAX_SENCERNEXT; nCntSencer++)
	{
		if (g_aSencerNext[nCntSencer].bUse == false)
		{
			g_aSencerNext[nCntSencer].pos = pos;
			g_aSencerNext[nCntSencer].rot = rot;
			g_aSencerNext[nCntSencer].bUse = true;

			break;
		}
	}

	return nCntSencer;
}
//=============================================================================
// センサー判定(プレイヤーとセンサーのの判定)
//=============================================================================
bool CollisionSencerNext(D3DXVECTOR3 *pos)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCERNEXT; nCntSencer++)
	{


		fxpos = (g_aSencerNext[nCntSencer].pos.x - pos->x) * (g_aSencerNext[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencerNext[nCntSencer].pos.z - pos->z) * (g_aSencerNext[nCntSencer].pos.z - pos->z);
		fRadius = SENCERNEXT_RADIUS * SENCERNEXT_RADIUS;



		if (g_aSencerNext[nCntSencer].bUse == true)
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

void DeleteSencerNext(int nCnt)
{
	g_aSencerNext[nCnt].bUse = false;
}
