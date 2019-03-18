//=============================================================================
//
// モデル処理 [modelwall.cpp]
// Author :  Kishida Rei
//
//=============================================================================
#include "modelwall.h"
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
//LPDIRECT3DTEXTURE9 g_pTextureModelWall = NULL;		//テクスチャへのポインタ
//LPD3DXMESH g_pMeshModelWall = NULL;	//メッシュ情報へのポインタ
//LPD3DXBUFFER g_pBuffMatModelWall = NULL;	//マテリアル情報へのポインタ
//DWORD g_nNumMatModelWall;	//マテリアルの情報の数


WALL g_aModelWall[MAX_MODELWALL];
//=============================================================================
// 初期化処理
//=============================================================================
void InitModelWall(void)
{


	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		g_aModelWall[nCntModelWall].nCntWall = 0;

		g_aModelWall[nCntModelWall].bUse = false;

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[0].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[1].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[2].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[3].pTexture);
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall02.jpg", &g_aModelWall[nCntModelWall].aBuilding[4].pTexture);


		// Xファイルの読み込み
		D3DXLoadMeshFromX("data/MODEL/wall.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[0].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[0].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[0].pMesh);

		// Xファイルの読み込み
		D3DXLoadMeshFromX("data/MODEL/wall02.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[1].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[1].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[1].pMesh);

		D3DXLoadMeshFromX("data/MODEL/wall03.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[2].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[2].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[2].pMesh);


		D3DXLoadMeshFromX("data/MODEL/wall04.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[3].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[3].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[3].pMesh);

		D3DXLoadMeshFromX("data/MODEL/wall05.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[4].pBuffMat,
			NULL,
			&g_aModelWall[nCntModelWall].aBuilding[4].nNumMat,
			&g_aModelWall[nCntModelWall].aBuilding[4].pMesh);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModelWall(void)
{
	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		for (int nCntTypeModelWall = 0; nCntTypeModelWall < MAXTYPE_MODELWALL; nCntTypeModelWall++)
		{
			//テクスチャの破棄
			if (g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pTexture != NULL)
			{
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pTexture->Release();
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pTexture = NULL;
			}

			// メッシュの開放
			if (g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pMesh != NULL)
			{
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pMesh->Release();
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pMesh = NULL;
			}

			// マテリアルの開放
			if (g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pBuffMat != NULL)
			{
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pBuffMat->Release();
				g_aModelWall[nCntModelWall].aBuilding[nCntTypeModelWall].pBuffMat = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModelWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModelWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ


	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		for (int nCntTypeModelWall = 0; nCntTypeModelWall < MAXTYPE_MODELWALL; nCntTypeModelWall++)
		{
			if (g_aModelWall[nCntModelWall].bUse == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aModelWall[nCntModelWall].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_aModelWall[nCntModelWall].rot.y, g_aModelWall[nCntModelWall].rot.x, g_aModelWall[nCntModelWall].rot.z);

				D3DXMatrixMultiply(&g_aModelWall[nCntModelWall].mtxWorld, &g_aModelWall[nCntModelWall].mtxWorld, &mtxrot);


				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_aModelWall[nCntModelWall].pos.x, g_aModelWall[nCntModelWall].pos.y, g_aModelWall[nCntModelWall].pos.z);

				D3DXMatrixMultiply(&g_aModelWall[nCntModelWall].mtxWorld, &g_aModelWall[nCntModelWall].mtxWorld, &mtxtrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aModelWall[nCntModelWall].mtxWorld);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].nNumMat; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					pDevice->SetTexture(0, g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pTexture);

					// モデル(パーツ)の描画
					g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}


bool CollisionModelWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //着地したか

	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{
		if (g_aModelWall[nCntModelWall].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x && pPos->x - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x
				&&pPos->y + VtxMax->y >= g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y && pPos->y - VtxMax->y <= g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y
				&&pPos->z + VtxMax->z >= g_aModelWall[nCntModelWall].VtxMinModelWall.z + g_aModelWall[nCntModelWall].pos.z && pPos->z - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x  && pPos->x + VtxMax->z >= g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x)
				{//左からきたとき
					pPos->x = g_aModelWall[nCntModelWall].VtxMinModelWall.x + g_aModelWall[nCntModelWall].pos.x - VtxMax->z;

					//bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x  && pPos->x - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x)
				{//右からきたとき
					pPos->x = g_aModelWall[nCntModelWall].VtxMaxModelWall.x + g_aModelWall[nCntModelWall].pos.x + VtxMax->z;
					//bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= (g_aModelWall[nCntModelWall].VtxMinModelWall.z + 50.0f) + g_aModelWall[nCntModelWall].pos.z && pPos->z + VtxMax->z >= (g_aModelWall[nCntModelWall].VtxMinModelWall.z + 50.0f) + g_aModelWall[nCntModelWall].pos.z)
				{//手前から来た時
					pPos->z = (g_aModelWall[nCntModelWall].VtxMinModelWall.z + 50.0f) + g_aModelWall[nCntModelWall].pos.z - VtxMax->z ;
					//bLand = true;

				}

				else if (pPosold->z - VtxMax->z >= g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z && pPos->z - VtxMax->z <= g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z)
				{//奥から来た時
					pPos->z = g_aModelWall[nCntModelWall].VtxMaxModelWall.z + g_aModelWall[nCntModelWall].pos.z + VtxMax->z;
					//bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y && pPos->y - VtxMax->y <= g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y)
				{//上から来た時
					pMove->y = 0.0f;

					pPos->y = g_aModelWall[nCntModelWall].VtxMaxModelWall.y + g_aModelWall[nCntModelWall].pos.y + VtxMax->y;
					//bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y && pPos->y + VtxMax->y >= g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y)
				{//下から来た時
					pPos->y = g_aModelWall[nCntModelWall].VtxMinModelWall.y + g_aModelWall[nCntModelWall].pos.y + VtxMin->y;


					//bLand = true;

				}

			}
		}
	}
	return bLand;

}




void SetModelWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	BYTE *pVtxBuff;	//頂点バッファのポインタ

	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット

	for (int nCntModelWall = 0; nCntModelWall < MAX_MODELWALL; nCntModelWall++)
	{

		if (g_aModelWall[nCntModelWall].bUse == false)	//ブロックが使用されていない
		{
			g_aModelWall[nCntModelWall].pos = pos;

			g_aModelWall[nCntModelWall].rot = rot;

			g_aModelWall[nCntModelWall].nType = nType;


			g_aModelWall[nCntModelWall].VtxMinModelWall = D3DXVECTOR3(100000, 100000, 100000);
			g_aModelWall[nCntModelWall].VtxMaxModelWall = D3DXVECTOR3(-100000, -100000, -100000);

			//頂点数を取得
			nNumVtx = g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->GetFVF());

			//頂点バッファをロック
			g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

				if (Vtx.x < g_aModelWall[nCntModelWall].VtxMinModelWall.x)
				{
					g_aModelWall[nCntModelWall].VtxMinModelWall.x = Vtx.x;
				}

				if (Vtx.y < g_aModelWall[nCntModelWall].VtxMinModelWall.y)
				{
					g_aModelWall[nCntModelWall].VtxMinModelWall.y = Vtx.y;
				}

				if (Vtx.z < g_aModelWall[nCntModelWall].VtxMinModelWall.z)
				{
					g_aModelWall[nCntModelWall].VtxMinModelWall.z = Vtx.z - 28.0f;
				}


				if (Vtx.x > g_aModelWall[nCntModelWall].VtxMaxModelWall.x)
				{
					g_aModelWall[nCntModelWall].VtxMaxModelWall.x = Vtx.x;
				}

				if (Vtx.y > g_aModelWall[nCntModelWall].VtxMaxModelWall.y)
				{
					g_aModelWall[nCntModelWall].VtxMaxModelWall.y = Vtx.y;
				}

				if (Vtx.z > g_aModelWall[nCntModelWall].VtxMaxModelWall.z)
				{
					g_aModelWall[nCntModelWall].VtxMaxModelWall.z = Vtx.z;
				}
				pVtxBuff += sizeFVF;
			}

			g_aModelWall[nCntModelWall].bUse = true;	//使用している状態にする
			break;
		}
		//頂点バッファをアンロック
		g_aModelWall[nCntModelWall].aBuilding[g_aModelWall[nCntModelWall].nType].pMesh->UnlockVertexBuffer();
	}
}





WALL *GetModelWall(void)
{
	return &g_aModelWall[0];
}