//=============================================================================
//
// スコアアイテム処理 [ScoreItem.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "scoreitem.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TYPE (1)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshScoreItem = NULL;	//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatScoreItem = NULL;	//マテリアル情報へのポインタ

DWORD g_nNumMatScoreItem;	//マテリアルの情報の数


SCOREITMEM g_aScoreItem[MAX_SCOREITEM];
//=============================================================================
// 初期化処理
//=============================================================================
void InitScoreItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		// 位置・向きの初期設定
		g_aScoreItem[nCntScoreItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aScoreItem[nCntScoreItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aScoreItem[nCntScoreItem].VtxMinScoreItem = D3DXVECTOR3(10000, 10000, 10000);
		g_aScoreItem[nCntScoreItem].VtxMaxScoreItem = D3DXVECTOR3(-10000, -10000, -10000);
		g_aScoreItem[nCntScoreItem].bUse = false;

	}


	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/haguruma/gear.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatScoreItem,
		NULL,
		&g_nNumMatScoreItem,
		&g_pMeshScoreItem);


	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		//頂点数を取得
		nNumVtx = g_pMeshScoreItem->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshScoreItem->GetFVF());

		//頂点バッファをロック
		g_pMeshScoreItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x)
			{
				g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x = Vtx.x;
			}

 			if (Vtx.y < g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y)
			{
				g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y = Vtx.y;
			}

			if (Vtx.z < g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z)
			{
				g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z = Vtx.x;
			}


			if (Vtx.x > g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x)
			{
				g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x = Vtx.x;
			}

			if (Vtx.y > g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y)
			{
				g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y = Vtx.y;
			}

			if (Vtx.z > g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z)
			{
				g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//頂点バッファをアンロック
		g_pMeshScoreItem->UnlockVertexBuffer();


	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScoreItem(void)
{

	// メッシュの開放
	if (g_pMeshScoreItem != NULL)
	{
		g_pMeshScoreItem->Release();
		g_pMeshScoreItem = NULL;
	}

	// マテリアルの開放
	if (g_pBuffMatScoreItem != NULL)
	{
		g_pBuffMatScoreItem->Release();
		g_pBuffMatScoreItem = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScoreItem(void)
{
	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == true)
		{
			g_aScoreItem[nCntScoreItem].rot.y += 0.02f;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScoreItem(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ


	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aScoreItem[nCntScoreItem].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aScoreItem[nCntScoreItem].rot.y, g_aScoreItem[nCntScoreItem].rot.x, g_aScoreItem[nCntScoreItem].rot.z);

			D3DXMatrixMultiply(&g_aScoreItem[nCntScoreItem].mtxWorld, &g_aScoreItem[nCntScoreItem].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aScoreItem[nCntScoreItem].pos.x, g_aScoreItem[nCntScoreItem].pos.y, g_aScoreItem[nCntScoreItem].pos.z);

			D3DXMatrixMultiply(&g_aScoreItem[nCntScoreItem].mtxWorld, &g_aScoreItem[nCntScoreItem].mtxWorld, &mtxtrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aScoreItem[nCntScoreItem].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatScoreItem->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatScoreItem; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, 0);

				// モデル(パーツ)の描画
				g_pMeshScoreItem->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionScoreItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bHit = false; //着地したか

	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x && pPos->x - VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x
				&&pPos->y + VtxMax->y >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y && pPos->y - VtxMax->y <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y
				&&pPos->z + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z && pPos->z - VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z)
			{
				if (pPosold->x + VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x  && pPos->x + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x)
				{//左からきたとき
					//pPos->x = g_aScoreItem[nCntScoreItem].VtxMinScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x - VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x  && pPos->x - VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x)
				{//右からきたとき
					//pPos->x = g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.x + g_aScoreItem[nCntScoreItem].pos.x + VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;



				}
				else if (pPosold->z + VtxMax->z <= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z && pPos->z + VtxMax->z >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z)
				{//手前から来た時
					//pPos->z = g_aScoreItem[nCntScoreItem].VtxMinScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z - VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;

				}
				else if (pPosold->z + VtxMin->z >= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z && pPos->z + VtxMin->z <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z)
				{//奥から来た時
					//pPos->z = g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.z + g_aScoreItem[nCntScoreItem].pos.z + VtxMax->z;
					g_aScoreItem[nCntScoreItem].bUse = false;
					bHit = true;

				}
				if (pPosold->y - VtxMax->y >= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y && pPos->y - VtxMax->y <= g_aScoreItem[nCntScoreItem].VtxMaxScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y)
				{//上から来た時
					//pMove->y = 0.0f;

					bHit = true;
					g_aScoreItem[nCntScoreItem].bUse = false;


				}
				if (pPosold->y + VtxMax->y <= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y && pPos->y + VtxMax->y >= g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y)
				{//下から来た時
					//pPos->y = g_aScoreItem[nCntScoreItem].VtxMinScoreItem.y + g_aScoreItem[nCntScoreItem].pos.y + VtxMin->y;

					bHit = true;
					g_aScoreItem[nCntScoreItem].bUse = false;

				}

			}
		}
	}
	return bHit;
}


void SetScoreItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	//頂点バッファをロック
	g_pMeshScoreItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntScoreItem = 0; nCntScoreItem < MAX_SCOREITEM; nCntScoreItem++)
	{
		if (g_aScoreItem[nCntScoreItem].bUse == false)	//ブロックが使用されていない
		{
			g_aScoreItem[nCntScoreItem].pos = pos;

			g_aScoreItem[nCntScoreItem].rot = rot;

			//g_aScoreItem[nCntScoreItem].nType = nType;

			//影の設定
			//g_aScoreItem[nCntScoreItem].nIndexShadow = SetShadow(g_aScoreItem[nCntScoreItem].pos, g_aScoreItem[nCntScoreItem].rot, 10.0f, 10.0f);

			g_aScoreItem[nCntScoreItem].bUse = true;	//使用している状態にする

			break;
		}

	}
	//頂点バッファをアンロック
	g_pMeshScoreItem->UnlockVertexBuffer();
}





SCOREITMEM *GetScoreItem(void)
{
	return &g_aScoreItem[0];
}