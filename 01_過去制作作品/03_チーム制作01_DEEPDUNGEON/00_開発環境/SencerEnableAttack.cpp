//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: エネミーの追従するときの距離
// Author : KishidaRei 
//
//=============================================================================
#include "SencerEnableAttack.h"
#include "input.h"
#include "camera.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERENABLEATTCK (5)
#define MAX_TEXTURE (2)

#define SENCERENABLEATTACK_VERTEX (  (g_SencerEnableAttack_h + 1) * (g_SencerEnableAttack_v + 1)   )	//頂点数
#define SENCERENABLEATTACK_INDEX (   (g_SencerEnableAttack_h * g_SencerEnableAttack_v )* 2 + (4 * (g_SencerEnableAttack_v - 1)) + 2   ) //インデックス数
#define SENCERENABLEATTACK_POLYGON ( (g_SencerEnableAttack_h * g_SencerEnableAttack_v) * 2 + (4 * (g_SencerEnableAttack_v - 1)) )	//ポリゴン数

#define SENCERENABLEATTACK_RADIUS (50.0f)
#define SENCERENABLEATTACK_HEIGHT (8.0f)

#define MESH_H (20) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerEnableAttack(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerEnableAttack(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerEnableAttack = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerEnableAttack = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerEnableAttack = NULL; //インデックスバッファへのポインタ

SENCERENABLEATTACK g_aSencerEnableAttack[MAX_SENCERENABLEATTCK];

int g_SencerEnableAttack_h, g_SencerEnableAttack_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerEnableAttack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerEnableAttack_h = MESH_H;
	g_SencerEnableAttack_v = MESH_V;


	MakeVertexSencerEnableAttack(pDevice);
	MakeIndexSencerEnableAttack(pDevice);

	for (int nCntSencerEnableAttack = 0; nCntSencerEnableAttack < MAX_SENCERENABLEATTCK; nCntSencerEnableAttack++)
	{
		g_aSencerEnableAttack[nCntSencerEnableAttack].fRadius = SENCERENABLEATTACK_RADIUS;
		g_aSencerEnableAttack[nCntSencerEnableAttack].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerEnableAttack(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerEnableAttack != NULL)
	{
		g_pTextureSencerEnableAttack->Release();
		g_pTextureSencerEnableAttack = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerEnableAttack != NULL)
	{
		g_pVtxBuffSencerEnableAttack->Release();
		g_pVtxBuffSencerEnableAttack = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerEnableAttack != NULL)
	{
		g_pIdxBuffSencerEnableAttack->Release();
		g_pIdxBuffSencerEnableAttack = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerEnableAttack(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerEnableAttack(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;



	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング



	for (int nCntSencerEnableAttack = 0; nCntSencerEnableAttack < MAX_SENCERENABLEATTCK; nCntSencerEnableAttack++)
	{
		if (g_aSencerEnableAttack[nCntSencerEnableAttack].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerEnableAttack[nCntSencerEnableAttack].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerEnableAttack[nCntSencerEnableAttack].rot.y, g_aSencerEnableAttack[nCntSencerEnableAttack].rot.x, g_aSencerEnableAttack[nCntSencerEnableAttack].rot.z);

			D3DXMatrixMultiply(&g_aSencerEnableAttack[nCntSencerEnableAttack].mtxWorld, &g_aSencerEnableAttack[nCntSencerEnableAttack].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerEnableAttack[nCntSencerEnableAttack].pos.x, g_aSencerEnableAttack[nCntSencerEnableAttack].pos.y, g_aSencerEnableAttack[nCntSencerEnableAttack].pos.z);

			D3DXMatrixMultiply(&g_aSencerEnableAttack[nCntSencerEnableAttack].mtxWorld, &g_aSencerEnableAttack[nCntSencerEnableAttack].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerEnableAttack[nCntSencerEnableAttack].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerEnableAttack, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerEnableAttack);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERENABLEATTACK_VERTEX, 0, SENCERENABLEATTACK_POLYGON);

		}
	}


#endif
}

//=============================================================================
// センサーの位置設定
//=============================================================================
void SetPositionSencerEnableAttack(int nIndexSencerEnableAttack, D3DXVECTOR3 pos)
{
	g_aSencerEnableAttack[nIndexSencerEnableAttack].pos.x = pos.x;
	g_aSencerEnableAttack[nIndexSencerEnableAttack].pos.y = pos.y + 30.0f;
	g_aSencerEnableAttack[nIndexSencerEnableAttack].pos.z = pos.z;

}
//=============================================================================
// センサーの消去
//=============================================================================
void DeleteSencerEnableAttack(int nIdxSencerEnableAttack)
{
	g_aSencerEnableAttack[nIdxSencerEnableAttack].bUse = false;
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerEnableAttack(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERENABLEATTACK_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerEnableAttack,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerEnableAttack->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerEnableAttack_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerEnableAttack_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerEnableAttack_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerEnableAttack_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERENABLEATTACK_RADIUS;
			pVtx[nCntLife_V * (g_SencerEnableAttack_h + 1) + nCntLife_H].pos.y = SENCERENABLEATTACK_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerEnableAttack_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERENABLEATTACK_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerEnableAttack_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerEnableAttack_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerEnableAttack_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerEnableAttack_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerEnableAttack->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerEnableAttack(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERENABLEATTACK_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerEnableAttack,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerEnableAttack->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerEnableAttack_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerEnableAttack_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerEnableAttack_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerEnableAttack_h && nCntIndex_V <= g_SencerEnableAttack_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerEnableAttack_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerEnableAttack->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencerEnableAttack(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntSencerEnableAttack;
	// 頂点情報の設定

	for (nCntSencerEnableAttack = 0; nCntSencerEnableAttack < MAX_SENCERENABLEATTCK; nCntSencerEnableAttack++)
	{
		if (g_aSencerEnableAttack[nCntSencerEnableAttack].bUse == false)
		{
			g_aSencerEnableAttack[nCntSencerEnableAttack].pos = pos;
			g_aSencerEnableAttack[nCntSencerEnableAttack].rot = rot;
			g_aSencerEnableAttack[nCntSencerEnableAttack].bUse = true;

			break;
		}
	}

	return nCntSencerEnableAttack;
}


//=============================================================================
// センサー判定(プレイヤーとの当たり判定)
//=============================================================================
bool CollisionSencer_Enable(D3DXVECTOR3 *pos)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencerEnableAttack = 0; nCntSencerEnableAttack < MAX_SENCERENABLEATTCK; nCntSencerEnableAttack++)
	{


		fxpos = (g_aSencerEnableAttack[nCntSencerEnableAttack].pos.x - pos->x) * (g_aSencerEnableAttack[nCntSencerEnableAttack].pos.x - pos->x);
		fzpos = (g_aSencerEnableAttack[nCntSencerEnableAttack].pos.z - pos->z) * (g_aSencerEnableAttack[nCntSencerEnableAttack].pos.z - pos->z);
		fRadius = SENCERENABLEATTACK_RADIUS * SENCERENABLEATTACK_RADIUS;


		if (g_aSencerEnableAttack[nCntSencerEnableAttack].bUse == true)
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

