//=============================================================================
//
// 土台処理 [Pedestal.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "pedestal.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define STAGE_LIMIT_MAX_X (150.0f)
#define STAGE_LIMIT_MIN_X (-50.0f)
#define STAGE_LIMIT_MAX_Z (50.0f)
#define STAGE_LIMIT_MIN_Z (-150.0f)


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePedestal = NULL;		//テクスチャへのポインタ
LPD3DXMESH g_pMeshPedestal = NULL;	//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatPedestal = NULL;	//マテリアル情報へのポインタ

DWORD g_nNumMatPedestal;	//マテリアルの情報の数


PEDESTAL g_aPedestal[MAX_PEDESTAL];
//=============================================================================
// 初期化処理
//=============================================================================
void InitPedestal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		// 位置・向きの初期設定
		/*	g_aPedestal[nCntPedestal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPedestal[nCntPedestal].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/
		g_aPedestal[nCntPedestal].VtxMinPedestal = D3DXVECTOR3(10000, 10000, 10000);
		g_aPedestal[nCntPedestal].VtxMaxPedestal = D3DXVECTOR3(-10000, -10000, -10000);
		g_aPedestal[nCntPedestal].bUse = false;
	}


	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rockandtekkotu.jpg", &g_pTexturePedestal);


	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/pedestal.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatPedestal,
		NULL,
		&g_nNumMatPedestal,
		&g_pMeshPedestal);

	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		//頂点数を取得
		nNumVtx = g_pMeshPedestal->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshPedestal->GetFVF());

		//頂点バッファをロック
		g_pMeshPedestal->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_aPedestal[nCntPedestal].VtxMinPedestal.x)
			{
				g_aPedestal[nCntPedestal].VtxMinPedestal.x = Vtx.x;
			}

			if (Vtx.y < g_aPedestal[nCntPedestal].VtxMinPedestal.y)
			{
				g_aPedestal[nCntPedestal].VtxMinPedestal.y = Vtx.y;
			}

			if (Vtx.z < g_aPedestal[nCntPedestal].VtxMinPedestal.z)
			{
				g_aPedestal[nCntPedestal].VtxMinPedestal.z = Vtx.z;
			}


			if (Vtx.x > g_aPedestal[nCntPedestal].VtxMaxPedestal.x)
			{
				g_aPedestal[nCntPedestal].VtxMaxPedestal.x = Vtx.x;
			}

			if (Vtx.y > g_aPedestal[nCntPedestal].VtxMaxPedestal.y)
			{
				g_aPedestal[nCntPedestal].VtxMaxPedestal.y = Vtx.y;
			}

			if (Vtx.z > g_aPedestal[nCntPedestal].VtxMaxPedestal.z)
			{
				g_aPedestal[nCntPedestal].VtxMaxPedestal.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//頂点バッファをアンロック
		g_pMeshPedestal->UnlockVertexBuffer();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPedestal(void)
{

	//テクスチャの破棄
	if (g_pTexturePedestal != NULL)
	{
		g_pTexturePedestal->Release();
		g_pTexturePedestal = NULL;
	}

	// メッシュの開放
	if (g_pMeshPedestal != NULL)
	{
		g_pMeshPedestal->Release();
		g_pMeshPedestal = NULL;
	}

	// マテリアルの開放
	if (g_pBuffMatPedestal != NULL)
	{
		g_pBuffMatPedestal->Release();
		g_pBuffMatPedestal = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePedestal(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPedestal(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ


	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPedestal[nCntPedestal].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aPedestal[nCntPedestal].rot.y, g_aPedestal[nCntPedestal].rot.x, g_aPedestal[nCntPedestal].rot.z);

			D3DXMatrixMultiply(&g_aPedestal[nCntPedestal].mtxWorld, &g_aPedestal[nCntPedestal].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aPedestal[nCntPedestal].pos.x, g_aPedestal[nCntPedestal].pos.y, g_aPedestal[nCntPedestal].pos.z);

			D3DXMatrixMultiply(&g_aPedestal[nCntPedestal].mtxWorld, &g_aPedestal[nCntPedestal].mtxWorld, &mtxtrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPedestal[nCntPedestal].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatPedestal->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatPedestal; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, g_pTexturePedestal);

				// モデル(パーツ)の描画
				g_pMeshPedestal->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionPedestal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //着地したか

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x
				&&pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y
				&&pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//左からきたとき
					pPos->x = g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x - VtxMax->z;

					//bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//右からきたとき
					pPos->x = g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x + VtxMax->z;
					//bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//手前から来た時
					pPos->z = g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z - VtxMax->z;
					//	bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//奥から来た時
					pPos->z = g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z + VtxMax->z;
					//bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//上から来た時
					pMove->y = 0.0f;

					pPos->y = g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMax->y;
					//bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//下から来た時
					pPos->y = g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMin->y;


					//bLand = true;

				}

			}
		}
	}
	return bLand;

}


bool CollisionPedestalEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //着地したか

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x
				&&pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y
				&&pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//左からきたとき
					pPos->x = g_aPedestal[nCntPedestal].VtxMinPedestal.x + g_aPedestal[nCntPedestal].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x  && pPos->x - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x)
				{//右からきたとき
					pPos->x = g_aPedestal[nCntPedestal].VtxMaxPedestal.x + g_aPedestal[nCntPedestal].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z + VtxMax->z >= g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//手前から来た時
					pPos->z = g_aPedestal[nCntPedestal].VtxMinPedestal.z + g_aPedestal[nCntPedestal].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z && pPos->z - VtxMax->z <= g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z)
				{//奥から来た時
					pPos->z = g_aPedestal[nCntPedestal].VtxMaxPedestal.z + g_aPedestal[nCntPedestal].pos.z + VtxMax->z;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y - VtxMax->y <= g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//上から来た時
					pMove->y = 0.0f;

					pPos->y = g_aPedestal[nCntPedestal].VtxMaxPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y && pPos->y + VtxMax->y >= g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y)
				{//下から来た時
					pPos->y = g_aPedestal[nCntPedestal].VtxMinPedestal.y + g_aPedestal[nCntPedestal].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}


void SetPedestal(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	BYTE *pVtxBuff;	//頂点バッファのポインタ

					//頂点バッファをロック
	g_pMeshPedestal->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntPedestal = 0; nCntPedestal < MAX_PEDESTAL; nCntPedestal++)
	{
		if (g_aPedestal[nCntPedestal].bUse == false)	//ブロックが使用されていない
		{
			g_aPedestal[nCntPedestal].pos = pos;

			g_aPedestal[nCntPedestal].rot = rot;



			g_aPedestal[nCntPedestal].bUse = true;	//使用している状態にする

			break;
		}

	}
	//頂点バッファをアンロック
	g_pMeshPedestal->UnlockVertexBuffer();
}





PEDESTAL *GetPedestal(void)
{
	return &g_aPedestal[0];
}